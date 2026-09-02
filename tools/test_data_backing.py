#!/usr/bin/env python3
"""Public regression guards for section backing, not whole-object identity."""
from __future__ import annotations

import copy
import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import data_backing as gate
from compare_elf_functions import Section, Symbol


class DataBackingTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.rows, cls.sections = gate.validate(gate.parse_args(["validate"]))

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.args = gate.parse_args(["validate", "--manifest", str(Path(self.tmp.name)/"aliases.tsv"),
                                    "--sections", str(Path(self.tmp.name)/"sections.tsv")])
        self.rows, self.sections = copy.deepcopy(self.__class__.rows), copy.deepcopy(self.__class__.sections)
        self.write()

    def write(self):
        self.args.manifest.write_text(gate.runtime_members.render(gate.FIELDS, self.rows))
        self.args.sections.write_text(gate.runtime_members.render(gate.SECTION_FIELDS, self.sections))

    def reject(self):
        self.write()
        with self.assertRaises(gate.DataBackingError):
            gate.validate(self.args)

    def test_public_gate_requires_no_reference_or_process(self):
        with patch.object(gate.libgcc, "load_reference", side_effect=AssertionError("private read")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(gate.validate(self.args), (self.rows, self.sections))

    def test_counts_keep_stage3f_open(self):
        stats = gate.statistics(self.rows, self.sections)
        for key, expected in {"contracts_total": 1265, "section_backed_addresses": 1197,
                              "unbacked_addresses": 39, "reused_sections": 66, "new_sections": 104,
                              "new_backing_bytes": 762372, "new_zero_fill_bytes": 0,
                              "total_backing_bytes": 992890, "rom_offset_refactors_closed": 29,
                              "historical_source_sections": 53, "resolved_contracts": 1226}.items():
            self.assertEqual(stats[key], expected, key)
        self.assertFalse(stats["stage3f_closed"])
        self.assertFalse(stats["complete_object_extents_proved"])
        self.assertFalse(stats["replacement_elf"])

    def test_direct_accesses_fully_covered(self):
        rows = [r for r in self.rows if r["coverage_kind"] == "direct-access"]
        self.assertEqual(len(rows), 872)
        for row in rows:
            start = int(row["target_address"], 0)
            self.assertEqual(gate.subtract_ranges([(start, start+int(row["access_extent_hex"], 0))],
                             [gate.interval(r) for r in self.sections]), [])

    def test_interior_aliases_never_claim_extent(self):
        interior = [r for r in self.rows if r["coverage_kind"] == "interior-address-only"]
        self.assertEqual(len(interior), 113)
        self.assertTrue(all(not r["access_extent_hex"] and r["claim"] == gate.CLAIM for r in interior))

    def test_unbacked_rows_have_no_storage_or_extent(self):
        rows = [r for r in self.rows if r["status"] == gate.UNBACKED]
        self.assertEqual(len(rows), 39)
        for row in rows:
            self.assertTrue(all(not row[k] for k in ("section", "section_offset_hex", "access_extent_hex", "coverage_kind")))
            self.assertEqual(row["claim"], gate.UNBACKED_CLAIM)

    def test_missing_or_extra_contract_rejected(self):
        self.rows.pop()
        self.reject()
        self.rows = copy.deepcopy(self.__class__.rows)
        self.rows.append(self.rows[0])
        self.reject()

    def test_status_offset_and_claim_drift_rejected(self):
        index = next(i for i, row in enumerate(self.rows) if row["status"] == gate.BACKED)
        for key, value in (("status", gate.UNBACKED), ("section_offset_hex", "0xffffffff"),
                           ("claim", "complete object proved")):
            with self.subTest(key=key):
                self.rows = copy.deepcopy(self.__class__.rows)
                self.rows[index][key] = value
                self.reject()

    def test_interior_width_forgery_rejected(self):
        next(r for r in self.rows if r["coverage_kind"] == "interior-address-only")["access_extent_hex"] = "0x8"
        self.reject()

    def test_section_geometry_and_evidence_drift_rejected(self):
        for field, value in (("extent_hex", "0xffffffff"), ("evidence_sha256", "0"*64),
                             ("alignment_hex", "0x10"), ("origin", "guessed")):
            with self.subTest(field=field):
                self.sections = copy.deepcopy(self.__class__.sections)
                self.sections[0][field] = value
                self.reject()

    def test_section_hash_format_rejected(self):
        self.sections[0]["sha256"] = ""
        self.reject()

    def test_valid_looking_hash_requires_private_verification(self):
        self.sections[0]["sha256"] = "0"*64
        self.write()
        gate.validate(self.args)  # Offline validation cannot inspect private bytes.
        with patch.object(gate.libgcc, "load_reference", return_value=bytes(gate.libgcc.TARGET_SIZE)):
            with self.assertRaisesRegex(gate.DataBackingError, "fingerprint"):
                gate.verify_reference(self.args, self.sections)

    def test_merge_never_fills_holes(self):
        self.assertEqual(gate.merge_ranges([(8, 10), (0, 4), (2, 5), (5, 6)]), [(0, 6), (8, 10)])
        with self.assertRaises(gate.DataBackingError):
            gate.merge_ranges([(3, 3)])

    def test_subtraction_reuses_existing_storage(self):
        self.assertEqual(gate.subtract_ranges([(0, 20)], [(2, 4), (6, 10), (18, 25)]),
                         [(0, 2), (4, 6), (10, 18)])
        self.assertEqual(gate.subtract_ranges([(2, 3)], [(0, 20)]), [])

    def test_overlapping_or_duplicate_sections_rejected(self):
        gate.no_overlap(self.sections)
        with self.assertRaises(gate.DataBackingError):
            gate.no_overlap(self.sections + [self.sections[0]])
        other = dict(self.sections[0], section=".data.other")
        with self.assertRaises(gate.DataBackingError):
            gate.no_overlap(self.sections + [other])

    def test_boundary_and_unsafe_section_rejected(self):
        layout = {"base": 0x1000, "initialized_end": 0x2000, "memory_end": 0x3000}
        for name, start, end in ((".data.test", 0x1ffc, 0x2004),
                                 (".data.x; injected", 0x1000, 0x1004),
                                 (".data.test", 0x0ffc, 0x1004),
                                 (".data.test", 0x2000, 0x2004)):
            with self.assertRaises(gate.DataBackingError):
                gate.section_row(name, start, end, gate.NEW, 1, [], layout)

    def test_zero_fill_fingerprint_is_not_initialized_bytes(self):
        row = {"target_address": "0x00426de8", "extent_hex": "0x10", "region": "zero-fill"}
        self.assertEqual(gate.fingerprint(b"", row), gate.digest(bytes(16)))
        row["region"] = "initialized"
        with self.assertRaises(gate.DataBackingError):
            gate.fingerprint(b"", row)

    def test_new_assembly_has_no_object_sizes_or_globals(self):
        paths = {r["section"]: Path(self.tmp.name)/f"source {i}.bin" for i, r in enumerate(self.sections) if r["origin"] == gate.HISTORICAL}
        text = gate.render_additions(Path(self.tmp.name)/"reference.bin", self.sections, paths)
        self.assertEqual(text.count(".incbin"), 104)
        self.assertEqual(text.count("reference.bin"), 51)
        self.assertEqual(text.count("source "), 54)  # 53 paths and the explanatory comment
        self.assertNotIn(".globl", text)
        self.assertNotIn(".size", text)
        self.assertNotIn(".word", text)

    def test_rebinding_assigns_inside_output_section(self):
        text = gate.render_rebind(self.rows, self.sections)
        self.assertEqual(text.count(" = . + "), 1197)
        self.assertEqual(text.count("KEEP(*("), 170)
        self.assertIn("DAT_0034551c = . + 0x3c;", text)
        self.assertIn(".data.stage3ce.va_003454e0 0 : {", text)

    @staticmethod
    def fake_elf(symbols, sections=None, data=b"", file_type=1):
        return SimpleNamespace(elf_class=1, endian="<", machine=8, file_type=file_type, data=data,
                               symbols=symbols, sections=sections or [Section(0, "", 0, 0, 0, 0, 0, 0, 0),
                                                                      Section(1, ".data.test", 1, 0, 0, 4, 0, 0, 0)])

    def tiny_rows(self):
        return [{"symbol": "DAT_00100000", "target_address": "0x00100000", "status": gate.BACKED,
                 "section": ".data.test", "section_offset_hex": "0x0"},
                {"symbol": "DAT_00100010", "target_address": "0x00100010", "status": gate.UNBACKED}]

    def test_input_must_have_exact_absolute_zero_sized_anchors(self):
        rows = self.tiny_rows()
        good = [Symbol(r["symbol"], int(r["target_address"], 0), 0, 0xfff1, 16) for r in rows]
        gate.check_input(self.fake_elf(good), rows)
        for bad in (Symbol(good[0].name, 1, 0, 0xfff1, 16),
                    Symbol(good[0].name, good[0].value, 8, 0xfff1, 16),
                    Symbol(good[0].name, good[0].value, 0, 1, 16)):
            with self.assertRaises(gate.DataBackingError):
                gate.check_input(self.fake_elf([bad, good[1]]), rows)

    def test_output_alias_must_be_relative_and_keep_other_globals(self):
        rows = self.tiny_rows()
        before = self.fake_elf([Symbol(r["symbol"], int(r["target_address"], 0), 0, 0xfff1, 16) for r in rows])
        valid = [Symbol(rows[0]["symbol"], 0, 0, 1, 16), before.symbols[1]]
        gate.check_output(before, self.fake_elf(valid), rows)
        for wrong in ([Symbol(rows[0]["symbol"], 0, 0, 0xfff1, 16), valid[1]],
                       [valid[0], Symbol(rows[1]["symbol"], 0, 0, 0xfff1, 16)], [valid[0]]):
            with self.assertRaises(gate.DataBackingError):
                gate.check_output(before, self.fake_elf(wrong), rows)

    def test_duplicate_undefined_or_wrong_elf_rejected(self):
        for symbols in ([Symbol("x", 0, 0, 0, 16)], [Symbol("x", 1, 0, 1, 16)]*2):
            with self.assertRaises(gate.DataBackingError):
                gate.global_map(self.fake_elf(symbols))
        with self.assertRaises(gate.DataBackingError):
            gate.global_map(self.fake_elf([], file_type=2))

    def test_probe_declares_three_address_relocations_per_alias(self):
        text = gate.render_probe(self.rows)
        self.assertEqual(text.count(".word "), 1197)
        self.assertEqual(text.count("%hi("), 1197)
        self.assertEqual(text.count("%lo("), 1197)
        self.assertNotIn(".globl", text)
        placement = gate.render_probe_layout(self.sections)
        self.assertIn("0x00500000", placement)
        self.assertIn("0x00510000", placement)

    def test_reference_roster_retains_section_offset_type_and_name(self):
        elf = self.fake_elf([])
        with patch.object(gate.runtime_overrides, "named_relocations", return_value=[(0, 5, "wanted"), (4, 6, "other")]):
            self.assertEqual(gate.reference_roster(elf, {"wanted"}), [(".data.test", 0, 5, "wanted")])

    def test_final_probe_rejects_wrong_address_and_absolute_identity(self):
        rows = self.tiny_rows()[:1]
        rows[0]["target_address"] = "0x00108004"  # signed LO16 needs the carry-adjusted HI16
        address = 0x108004
        data = struct.pack("<III", address, 0x3C020011, 0x24428004)
        sections = [Section(0, "", 0, 0, 0, 0, 0, 0, 0),
                    Section(1, ".data.test", 1, address, 0, 4, 0, 0, 0),
                    Section(2, gate.POINTER_SECTION, 1, gate.POINTER_BASE, 0, 4, 0, 0, 0),
                    Section(3, gate.CODE_SECTION, 1, gate.CODE_BASE, 4, 8, 0, 0, 0)]
        elf = self.fake_elf([Symbol(rows[0]["symbol"], address, 0, 1, 16)], sections, data, 2)
        result = gate.check_final_probe(elf, rows, [{"section": ".data.test", "target_address": hex(address)}])
        self.assertEqual(result["relocations_proved"], 3)
        elf.data = struct.pack("<I", address+4) + data[4:]
        with self.assertRaises(gate.DataBackingError):
            gate.check_final_probe(elf, rows, [])
        elf.data = data
        elf.symbols = [Symbol(rows[0]["symbol"], address, 0, 0xfff1, 16)]
        with self.assertRaises(gate.DataBackingError):
            gate.check_final_probe(elf, rows, [])


if __name__ == "__main__":
    unittest.main()
