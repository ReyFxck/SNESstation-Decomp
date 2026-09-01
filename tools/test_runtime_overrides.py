import copy
import shutil
import struct
import subprocess
import tempfile
import unittest
from dataclasses import replace
from pathlib import Path
from unittest.mock import patch

import libgcc_contracts as libgcc
import runtime_members
import runtime_overrides as runtime
from compare_elf_functions import RelocationMask


class RuntimeOverrideTests(unittest.TestCase):
    def setUp(self):
        self.args = runtime.parse_args(["validate"])
        self.rows = libgcc.read_table(runtime.DEFAULT_MANIFEST, runtime.FIELDS)
        self.witnesses = libgcc.read_table(runtime.DEFAULT_WITNESSES, runtime.WITNESS_FIELDS)

    def changed(self, name, rows, fields, message):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "changed.tsv"
            path.write_text(runtime_members.render(fields, rows), encoding="utf-8")
            setattr(self.args, name, path)
            with self.assertRaisesRegex(runtime.RuntimeOverrideError, message):
                runtime.validate_manifest(self.args)

    def test_public_gate_has_no_private_or_toolchain_dependency(self):
        with patch.object(libgcc, "run", side_effect=AssertionError("no subprocess")):
            runtime.validate_manifest(self.args)

    def test_two_contracts_close_the_original_53_row_ledger(self):
        report = runtime.statistics(self.rows, self.witnesses)
        self.assertEqual((2, 104, 15, 53), tuple(report[k] for k in
                         ("contracts_closed", "provider_bytes", "incoming_named_calls", "stage3d_closed")))
        self.assertEqual(14_308, report["witness_bytes"])

    def test_two_complete_members_and_one_weak_function_are_distinct(self):
        self.assertEqual(["complete-member-text", "complete-member-text", "complete-weak-function-only"],
                         [r["scope"] for r in self.witnesses])
        self.assertEqual("0x24", self.witnesses[-1]["extent_hex"])

    def test_rejected_provider_candidates_are_not_promoted(self):
        rows, objects = runtime_members.validate_manifest(runtime_members.parse_args(["validate"]))
        self.assertEqual(2, sum(r["status"] == runtime_members.BLOCKED for r in rows))
        self.assertEqual(42, sum(r["status"] == runtime_members.EXACT for r in objects))

    def test_empty_or_extra_override_fails(self):
        for rows in ([], self.rows + [copy.copy(self.rows[0])]):
            with self.subTest(rows=len(rows)):
                self.changed("manifest", rows, runtime.FIELDS, "two providers")

    def test_source_hash_drift_fails(self):
        self.rows[0]["source_sha256"] = "0" * 64
        self.changed("manifest", self.rows, runtime.FIELDS, "source_sha256 drift")

    def test_profile_drift_fails(self):
        self.rows[0]["profile_sha256"] = "0" * 64
        self.changed("manifest", self.rows, runtime.FIELDS, "profile_sha256 drift")

    def test_canonical_owner_drift_fails(self):
        self.rows[0]["canonical_source"] = "src/unknown.c"
        self.changed("manifest", self.rows, runtime.FIELDS, "canonical_source drift")

    def test_incoming_count_cannot_be_reduced(self):
        self.rows[0]["incoming_calls"] = "1"
        self.changed("manifest", self.rows, runtime.FIELDS, "incoming_calls drift")

    def test_linked_hash_must_equal_target(self):
        self.rows[1]["linked_sha256"] = "0" * 64
        self.changed("manifest", self.rows, runtime.FIELDS, "final linked bytes")

    def test_target_hash_is_required(self):
        self.rows[1]["target_sha256"] = ""
        self.changed("manifest", self.rows, runtime.FIELDS, "missing target_sha256")

    def test_evidence_hash_drift_fails(self):
        self.rows[0]["evidence_sha256"] = "0" * 64
        self.changed("manifest", self.rows, runtime.FIELDS, "evidence_sha256 drift")

    def test_witness_cannot_claim_whole_terminate_member(self):
        self.witnesses[-1]["scope"] = "complete-member-text"
        self.changed("witnesses", self.witnesses, runtime.WITNESS_FIELDS, "scope drift")

    def test_witness_call_roster_is_immutable(self):
        self.witnesses[0]["call_offsets"] = "0x64"
        self.changed("witnesses", self.witnesses, runtime.WITNESS_FIELDS, "call_offsets drift")

    def test_witness_full_relocation_count_is_checked(self):
        self.witnesses[0]["relocation_count"] = "9"
        self.changed("witnesses", self.witnesses, runtime.WITNESS_FIELDS, "relocation count drift")

    def test_runtime_kind_does_not_claim_an_archive(self):
        rows = libgcc.read_table(libgcc.DEFAULT_EXTERNAL, libgcc.EXTERNAL_FIELDS)
        next(r for r in rows if r["symbol"] == "abort")["provider_kind"] = "historical-archive"
        self.changed("external_map", rows, libgcc.EXTERNAL_FIELDS, "ownership drift")

    @staticmethod
    def fixture(callee=0x107578):
        return (struct.pack("<II", 0x0C000000, 0),
                struct.pack("<II", 0x0C000000 | (callee >> 2), 0))

    def test_named_jal_proves_the_exact_selected_target(self):
        image, target = self.fixture()
        runtime.verify_named_calls(image, target, [(0, 4, "abort")], 0x100000, "abort", [0], 0x107578)

    def test_wrong_target_rejected_even_with_same_normalized_bytes(self):
        image, target = self.fixture(0x19C5A8)
        with self.assertRaisesRegex(runtime.RuntimeOverrideError, "different target"):
            runtime.verify_named_calls(image, target, [(0, 4, "abort")], 0x100000, "abort", [0], 0x107578)

    def test_wrong_relocation_symbol_type_or_count_is_rejected(self):
        image, target = self.fixture()
        for relocs in ([(0, 4, "exit")], [(0, 5, "abort")], [], [(0, 4, "abort"), (4, 4, "abort")]):
            with self.subTest(relocs=relocs), self.assertRaisesRegex(runtime.RuntimeOverrideError, "roster drift"):
                runtime.verify_named_calls(image, target, relocs, 0x100000, "abort", [0], 0x107578)

    def test_nonzero_addend_is_not_symbol_identity(self):
        _, target = self.fixture()
        image = struct.pack("<II", 0x0C000001, 0)
        with self.assertRaisesRegex(runtime.RuntimeOverrideError, "incoming addend"):
            runtime.verify_named_calls(image, target, [(0, 4, "abort")], 0x100000, "abort", [0], 0x107578)

    def test_jump_is_not_a_call(self):
        with self.assertRaisesRegex(runtime.RuntimeOverrideError, "expected direct JAL"):
            runtime.direct_callee(0x08000000, 0x100000)

    def test_jal_preserves_the_high_pc_region(self):
        self.assertEqual(0x90000040, runtime.direct_callee(0x0C000010, 0x90000000))

    def test_precise_mask_accepts_callee_but_not_opcode_or_delay_change(self):
        image, target = self.fixture()
        mask = RelocationMask(0, 4, 4, bytes.fromhex("ffffff03"), True)
        runtime.compare_text(image, [mask], target, "test")
        for changed in (target[:3] + b"\x08" + target[4:], target[:4] + b"\1\0\0\0"):
            with self.assertRaisesRegex(runtime.RuntimeOverrideError, "outside relocation"):
                runtime.compare_text(image, [mask], changed, "test")

    def test_broad_or_unknown_masks_fail(self):
        image, target = self.fixture()
        mask = RelocationMask(0, 4, 4, bytes.fromhex("ffffff03"), True)
        for changed in (replace(mask, mask_bytes=b"\xff" * 4), replace(mask, known=False),
                        replace(mask, start=-4), replace(mask, end=8), replace(mask, relocation_type=255)):
            with self.subTest(mask=changed), self.assertRaisesRegex(runtime.RuntimeOverrideError, "invalid span"):
                runtime.compare_text(image, [changed], target, "test")

    def test_target_span_bounds_are_checked(self):
        for address, size in ((0xFFFFC, 4), (0x100000, 12), (0x100000, 3), (0x100000, 0)):
            with self.subTest(address=address, size=size), self.assertRaises(runtime.RuntimeOverrideError):
                runtime.target_span(b"\0" * 8, address, size)

    @unittest.skipUnless(shutil.which("cc"), "host C compiler unavailable")
    def test_puts_keeps_target_semantics_without_newline_or_write_result(self):
        harness = r'''
#include <assert.h>
#include <string.h>
extern int recovered_puts(const char *);
static int calls, last_fd, last_size, returned;
static const void *last_buffer;
int fioWrite(int fd, void *buffer, int size) {
  ++calls; last_fd=fd; last_buffer=buffer; last_size=size; return returned;
}
int main(void) {
  char text[257]; int n, k;
  for (n=0; n<=256; ++n) {
    memset(text, 0xa5, sizeof(text)); text[n]=0;
    for (k=-1; k<=1; ++k) {
      calls=0; returned=k;
      assert(recovered_puts(text)==n);
      assert(calls==1 && last_fd==1 && last_size==n && last_buffer==text);
      assert(text[n]==0);
    }
  }
  return 0;
}
'''
        with tempfile.TemporaryDirectory() as temp:
            base = Path(temp)
            (base / "test.c").write_text(harness, encoding="utf-8")
            commands = [
                ["cc", "-std=c99", "-O2", "-fno-builtin", "-Dputs=recovered_puts", "-Dabort=recovered_abort",
                 "-c", str(runtime.SOURCE), "-o", str(base / "overrides.o")],
                ["cc", str(base / "test.c"), str(base / "overrides.o"), "-o", str(base / "test")],
                [str(base / "test")],
            ]
            for command in commands:
                result = subprocess.run(command, capture_output=True, text=True, timeout=30)
                self.assertEqual(0, result.returncode, result.stderr)


if __name__ == "__main__":
    unittest.main()
