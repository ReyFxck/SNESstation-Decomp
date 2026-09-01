import contextlib
import io
import tempfile
import unittest
from collections import Counter
from dataclasses import replace
from pathlib import Path
from unittest.mock import patch

import build_source_tree
import libgcc_contracts as libgcc
import runtime_members as runtime
from compare_elf_functions import RelocationMask


class RuntimeMemberTests(unittest.TestCase):
    def setUp(self):
        self.args = runtime.parse_args(["validate"])
        self.rows = libgcc.read_table(runtime.DEFAULT_MANIFEST, runtime.FIELDS)
        self.objects = libgcc.read_table(runtime.DEFAULT_OBJECTS, runtime.OBJECT_FIELDS)

    def check_changed(self, rows, kind, error):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "changed.tsv"
            fields = {"manifest": runtime.FIELDS, "objects": runtime.OBJECT_FIELDS,
                      "inputs": runtime.INPUT_FIELDS, "external_map": libgcc.EXTERNAL_FIELDS,
                      "contracts": libgcc.CONTRACT_FIELDS}[kind]
            path.write_text(runtime.render(fields, rows), encoding="utf-8")
            setattr(self.args, kind, path)
            with self.assertRaisesRegex(runtime.RuntimeMemberError, error):
                runtime.validate_manifest(self.args)

    def test_public_gate_uses_no_reference_compiler_or_network(self):
        with patch.object(runtime, "run", side_effect=AssertionError("public gate must not run commands")):
            rows, objects = runtime.validate_manifest(self.args)
        self.assertEqual({runtime.EXACT: 43, runtime.BLOCKED: 2}, dict(Counter(r["status"] for r in rows)))
        self.assertEqual({runtime.EXACT: 42, runtime.BLOCKED: 2}, dict(Counter(r["status"] for r in objects)))

    def test_overall_stage3d_counts_include_previous_subtranches_once(self):
        report = runtime.statistics(self.rows, self.objects)
        self.assertEqual((51, 53, 2), tuple(report[k] for k in ("stage3d_closed", "stage3d_total", "stage3d_open")))
        self.assertEqual(12_964, report["exact_text_bytes"])
        self.assertEqual(700, report["relocations_normalized"])
        self.assertEqual(42, report["selected_members"])

    def test_shared_sifcmd_member_is_not_double_counted(self):
        rows = [r for r in self.rows if r["symbol"] in ("SifInitCmd", "SifExitCmd")]
        self.assertEqual(2, len(rows))
        self.assertEqual({"kernel/sif_cmd_main.o"}, {r["member"] for r in rows})
        objects = [r for r in self.objects if r["member"] == "kernel/sif_cmd_main.o"]
        self.assertEqual(1, len(objects))
        self.assertEqual(616, int(objects[0]["text_size_hex"], 0))

    def test_multifunction_members_cover_helpers_and_terminal_gaps(self):
        members = {r["member"]: r for r in self.objects}
        self.assertEqual(5_044, int(members["mc/libmc.o"]["text_size_hex"], 0))
        self.assertEqual(1_040, int(members["kernel/SifRpcMain.o"]["text_size_hex"], 0))
        self.assertEqual(468, int(members["libc/malloc.o"]["text_size_hex"], 0))
        self.assertEqual(88, int(members["libc/strncpy.o"]["text_size_hex"], 0))
        self.assertEqual(84, runtime.CONTRACT_BY_SYMBOL["strncpy"].member_size)

    def test_later_migration_commits_are_pinned_source_witnesses(self):
        rows = runtime.validate_inputs(runtime.DEFAULT_INPUTS)
        self.assertEqual(39, len(rows))
        may = {r["path"] for r in rows if r["revision"] == runtime.MAY04}
        self.assertEqual({"ee/kernel/include/iopcontrol.h", "ee/libc/include/malloc.h"}, may)
        self.assertTrue(all(m.revision != runtime.MAY04 for m in runtime.MEMBERS))
        self.assertIn("-nostdinc", runtime.FLAGS)
        self.assertNotIn("-mlong64", runtime.FLAGS)

    def test_rejected_runtime_candidates_remain_blocked(self):
        rows = [r for r in self.rows if r["status"] == runtime.BLOCKED]
        self.assertEqual({"abort", "puts"}, {r["symbol"] for r in rows})
        self.assertTrue(all(r["target_symbol_size_hex"] != r["member_symbol_size_hex"] for r in rows))
        rejected = [r for r in self.objects if r["status"] == runtime.BLOCKED]
        self.assertTrue(all(not r["target_base"] and not r["target_sha256"] for r in rejected))

    def test_ownership_identifies_ps2lib_not_generic_newlib(self):
        readiness = build_source_tree.load_source_readiness()
        for spec in runtime.CONTRACTS:
            category, kind, owner, gate = build_source_tree.classify_external(spec.symbol, readiness)
            self.assertEqual("historical-archive", kind)
            self.assertIn(category, ("c-runtime", "ps2-runtime"))
            self.assertEqual(runtime.ownership(spec.symbol), (owner, gate))
        self.assertIn("PS2LIB libc/memcpy.o", runtime.ownership("memcpy")[0])
        self.assertIsNone(runtime.ownership("not_a_runtime_contract"))

    def test_missing_contract_is_rejected(self):
        self.check_changed(self.rows[:-1], "manifest", "45 sorted contracts")

    def test_duplicate_member_is_rejected(self):
        self.check_changed(self.objects + [self.objects[0]], "objects", "42 selected")

    def test_blocker_cannot_be_promoted_by_changing_status(self):
        next(r for r in self.rows if r["symbol"] == "puts")["status"] = runtime.EXACT
        self.check_changed(self.rows, "manifest", "puts status drift")

    def test_symbol_offset_drift_is_rejected(self):
        next(r for r in self.rows if r["symbol"] == "mcInit")["member_offset_hex"] = "0x0"
        self.check_changed(self.rows, "manifest", "mcInit member_offset_hex drift")

    def test_helper_excision_is_rejected(self):
        next(r for r in self.objects if r["member"] == "libc/malloc.o")["text_size_hex"] = "0x194"
        self.check_changed(self.objects, "objects", "malloc.o text_size_hex drift")

    def test_recipe_revision_cannot_drift(self):
        self.objects[0]["source_revision"] = runtime.MAY04
        self.check_changed(self.objects, "objects", "source_revision drift")

    def test_empty_hash_is_rejected(self):
        self.objects[0]["input_closure_sha256"] = ""
        self.check_changed(self.objects, "objects", "missing input_closure_sha256")

    def test_raw_exact_member_hashes_must_agree(self):
        next(r for r in self.objects if r["member"] == "libc/memcpy.o")["target_sha256"] = "0" * 64
        self.check_changed(self.objects, "objects", "raw-exact member hash drift")

    def test_rejected_member_cannot_claim_target_hash(self):
        next(r for r in self.objects if r["member"] == "libc/puts.o")["target_sha256"] = "0" * 64
        self.check_changed(self.objects, "objects", "rejected recipe must not claim")

    def test_input_path_traversal_is_rejected(self):
        rows = libgcc.read_table(runtime.DEFAULT_INPUTS, runtime.INPUT_FIELDS)
        rows[0]["path"] = "../escape.h"
        self.check_changed(rows, "inputs", "unsafe runtime input path")

    def test_input_hash_drift_is_rejected(self):
        rows = libgcc.read_table(runtime.DEFAULT_INPUTS, runtime.INPUT_FIELDS)
        rows[0]["sha256"] = "0" * 64
        self.check_changed(rows, "inputs", "pinned runtime input hashes drifted")

    def test_requester_ownership_drift_is_rejected(self):
        next(r for r in self.rows if r["symbol"] == "sprintf")["requesters"] = "ps2/incorrect.o"
        self.check_changed(self.rows, "manifest", "sprintf requesters drift")

    def test_external_ownership_drift_is_rejected(self):
        rows = libgcc.read_table(libgcc.DEFAULT_EXTERNAL, libgcc.EXTERNAL_FIELDS)
        next(r for r in rows if r["symbol"] == "memcpy")["owner"] = "newlib/libc"
        self.check_changed(rows, "external_map", "runtime ownership drift: memcpy")

    def test_target_binding_drift_is_rejected(self):
        rows = libgcc.read_table(libgcc.DEFAULT_CONTRACTS, libgcc.CONTRACT_FIELDS)
        next(r for r in rows if r["symbol"] == "sprintf")["target_address"] = "0x00100000"
        self.check_changed(rows, "contracts", "runtime target binding drift: sprintf")

    def test_downloaded_or_cached_payload_must_match_sha256(self):
        runtime.verify_payload(b"source", runtime.digest(b"source"), "fixture")
        with self.assertRaisesRegex(runtime.RuntimeMemberError, "input SHA-256 mismatch"):
            runtime.verify_payload(b"changed", runtime.digest(b"source"), "fixture")

    def test_dependency_list_handles_spaces_continuations_and_duplicates(self):
        base = Path("/tmp/runtime fixture")
        allowed = {base / "one.h": ("rev", "one.h", "1"), base / "two.h": ("rev", "two.h", "2")}
        text = "object.o: /tmp/runtime\\ fixture/one.h \\\n /tmp/runtime\\ fixture/two.h /tmp/runtime\\ fixture/one.h\n"
        expected = runtime.digest(b"rev\tone.h\t1\nrev\ttwo.h\t2\n")
        self.assertEqual(expected, runtime.dependency_digest(text, allowed))

    def test_unpinned_or_empty_dependencies_fail_closed(self):
        for text in ("object.o: /usr/include/host.h", "object.o:", "malformed"):
            with self.subTest(text=text), self.assertRaises(runtime.RuntimeMemberError):
                runtime.dependency_digest(text, {})

    @staticmethod
    def jal_mask():
        return RelocationMask(0, 4, 4, bytes.fromhex("ffffff03"), True)

    def test_only_relocation_controlled_bits_are_masked(self):
        runtime.compare_member(bytes.fromhex("0000000c00000000"), [self.jal_mask()],
                               bytes.fromhex("1234560c00000000"), "fixture")

    def test_opcode_changes_cannot_hide_behind_relocation(self):
        with self.assertRaisesRegex(runtime.RuntimeMemberError, "outside relocation bits"):
            runtime.compare_member(bytes.fromhex("0000000c00000000"), [self.jal_mask()],
                                   bytes.fromhex("1234560800000000"), "fixture")

    def test_terminal_padding_is_compared(self):
        with self.assertRaisesRegex(runtime.RuntimeMemberError, "outside relocation bits"):
            runtime.compare_member(bytes.fromhex("0000000c00000000"), [self.jal_mask()],
                                   bytes.fromhex("1234560c01000000"), "fixture")

    def test_unknown_or_out_of_range_masks_are_rejected(self):
        for mask in (replace(self.jal_mask(), known=False), replace(self.jal_mask(), start=-4),
                     replace(self.jal_mask(), end=12), replace(self.jal_mask(), start=1)):
            with self.subTest(mask=mask), self.assertRaisesRegex(runtime.RuntimeMemberError, "unsupported relocation"):
                runtime.compare_member(b"\0" * 8, [mask], b"\0" * 8, "fixture")

    def test_whole_member_size_must_match(self):
        with self.assertRaisesRegex(runtime.RuntimeMemberError, "member size"):
            runtime.compare_member(b"\0" * 8, [], b"\0" * 4, "fixture")

    def test_wrong_private_reference_fails_before_source_fetch(self):
        with tempfile.TemporaryDirectory() as temp:
            self.args.reference = Path(temp) / "wrong.bin"
            self.args.reference.write_bytes(b"not the original image")
            with patch.object(runtime, "build_members") as build:
                with self.assertRaisesRegex(libgcc.LibgccContractError, "reference identity mismatch"):
                    runtime.capture(self.args)
                build.assert_not_called()

    def test_missing_private_reference_fails_before_source_fetch(self):
        with tempfile.TemporaryDirectory() as temp:
            self.args.reference = Path(temp) / "missing.bin"
            with patch.object(runtime, "build_members") as build:
                with self.assertRaisesRegex(libgcc.LibgccContractError, "missing private unpacked"):
                    runtime.capture(self.args)
                build.assert_not_called()

    def test_verify_rejects_changed_private_proof(self):
        changed = [dict(row) for row in self.objects]
        changed[0]["text_sha256"] = "0" * 64
        with patch.object(runtime, "capture", return_value=(self.rows, changed, {})), contextlib.redirect_stdout(io.StringIO()) as output:
            self.assertEqual(1, runtime.main(["verify"]))
        self.assertIn("differs from frozen manifests", output.getvalue())


if __name__ == "__main__":
    unittest.main()
