#!/usr/bin/env python3
"""Public regression guards for the Stage-3P code-window gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import code_windows as gate


class CodeWindowsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "code-windows.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.CodeWindowsError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_all_code_windows_are_exact(self):
        result = self.document["result"]
        self.assertEqual((65536 - 0x114) + 10 * 65536, result["source_bytes"])
        self.assertEqual(0, result["residual_bytes"])
        self.assertEqual(71896, result["listing_bytes"])
        self.assertEqual(720620, result["direct_object_bytes"])
        self.assertEqual(178, result["direct_candidate_object_inputs"])
        self.assertEqual(9596, result["evidence_candidate_object_bytes"])
        self.assertEqual(25748, result["evidence_listing_bytes"])
        self.assertEqual(0, result["incbin_payload_bytes"])
        self.assertEqual(0, result["incbin_payload_sections"])
        self.assertEqual((51, 0), (result["exact_chunks"], result["mismatching_chunks"]))
        self.assertEqual(list(range(51)), result["exact_chunk_indices"])
        self.assertEqual([], result["mismatching_chunk_indices"])
        self.assertEqual(gate.TARGET_SHA256, result["integrated_padded_sha256"])
        self.assertEqual(
            gate.EXPECTED["selected_source_contract_sha256"],
            result["selected_source_contract_sha256"],
        )

    def test_c4drawwireframe_links_from_canonical_cpp_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "c4draw-wireframe"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/snes9x/c4drawwireframe.o")
        self.assertEqual(row[2:6], (0x0010CDCC, 0, 0x01D8, 15))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {})
        self.assertEqual(row[8], 0x01D8)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_intr_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-intr"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_heap_runtime_recovered.o")
        self.assertEqual(row[2:6], (0x0019F018, 0, 0x0060, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x00A0)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_syscalls_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-syscalls"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_syscalls_recovered.o")
        self.assertEqual(row[2:6], (0x0019FCD0, 0, 0x0050, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x0050)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_syscalls2_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-syscalls2"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_syscalls2_recovered.o")
        self.assertEqual(row[2:6], (0x0019CE60, 0, 0x00B0, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x00B0)
        self.assertEqual(row[9], frozenset())

    def test_c4drawline_links_from_clean_cpp_function_section(self):
        slices = [
            row for row in gate.CLEAN_SOURCE_SLICES
            if row.name == "clean_c4_draw_line"
        ]
        self.assertEqual(len(slices), 1)
        row = slices[0]
        self.assertEqual(row.object, "build/code-windows/clean-source/native_c4_raster.o")
        self.assertEqual((row.address, row.size, row.symbol),
                         (0x0010CBB0, 540, "_Z10C4DrawLineiisiish"))
        units = [row for row in gate.CLEAN_SOURCE_UNITS if row[0] == "native_c4_raster"]
        self.assertEqual(
            units,
            [("native_c4_raster", "src/snes9x/native_c4_raster.cpp",
              "build/code-windows/clean-source/native_c4_raster.o", False)],
        )
        for name, address in {
            "_DAT_00335938": 0x00335938,
            "_DAT_0033593a": 0x0033593A,
            "DAT_0033593c": 0x0033593C,
            "DAT_0033593e": 0x0033593E,
            "DAT_00335940": 0x00335940,
            "DAT_00335942": 0x00335942,
            "DAT_00335944": 0x00335944,
            "C4TransfWireFrame2": 0x0010BBCC,
            "C4CalcWireFrame": 0x0010BECC,
        }.items():
            self.assertEqual(gate.CLEAN_SOURCE_PROVIDER_TARGETS[name], address)

    def test_final_linker_isolates_only_source_aggregate_models(self):
        source_input = Path("build/test/source-tree.partial.o")
        rule = gate.source_model_rule(source_input)
        self.assertIn(f'"{source_input.resolve().as_posix()}"(', rule)
        self.assertIn(".text.* .gnu.linkonce.t.* .rodata.native_dsp1_rom", rule)
        self.assertIn(
            ".data.*_ZZ9S9xSetPPUE8IncCount .data.*_ZZ9S9xSetPPUE5Shift",
            rule,
        )
        self.assertNotIn("(.data.*", rule)

    def test_private_payload_is_not_frozen(self):
        self.assertFalse(self.document["claims"]["private_target_bytes_stored"])
        self.assertTrue(self.document["claims"]["windows_0_through_6_exact"])
        self.assertTrue(self.document["claims"]["windows_1_through_6_exact"])
        self.assertTrue(self.document["claims"]["windows_0_through_10_exact"])
        self.assertTrue(self.document["claims"]["unpacked_hash_matched"])
        self.assertFalse(self.document["claims"]["replacement_elf"])
        self.assertNotIn(".incbin", json.dumps(self.document))

    def test_claim_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["claims"]["replacement_elf"] = True
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["result"]["exact_chunks"] = 45
        self.reject(changed)

    def test_evidence_rebuild_preserves_ledger_bytes(self):
        with tempfile.TemporaryDirectory() as name:
            ledgers = [Path(name) / "a.tsv", Path(name) / "b.tsv"]
            original = [b"user-ledger-a\n", b"user-ledger-b\n"]
            for path, data in zip(ledgers, original):
                path.write_bytes(data)

            def rebuild(*_args, **_kwargs):
                for path in ledgers:
                    path.write_bytes(b"generated-local-path-hash\n")
                return SimpleNamespace(returncode=0)

            args = gate.parse_args(["prepare"])
            with patch.object(gate, "EVIDENCE_MANIFESTS", tuple(map(str, ledgers))), \
                 patch.object(gate, "resolve_tool", return_value=Path("/tool/ee-g++")), \
                 patch.object(gate.subprocess, "run", side_effect=rebuild):
                gate.prepare_evidence(args)
            self.assertEqual(original, [path.read_bytes() for path in ledgers])


if __name__ == "__main__":
    unittest.main()
