import copy
import json
import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
import historical_data as h
from compare_elf_functions import Section, Symbol


def pair(value=0x335370, addend=0, key="data", hi_pc=0x100000, lo_pc=0x100004):
    old_hi, old_lo = ((addend+0x8000) >> 16) & 65535, addend & 65535
    new_hi, new_lo = ((value+addend+0x8000) >> 16) & 65535, (value+addend) & 65535
    return [dict(kind=5, symbol=key, source=0x3c020000|old_hi, target=0x3c020000|new_hi, pc=hi_pc),
            dict(kind=6, symbol=key, source=0x24420000|old_lo, target=0x24420000|new_lo, pc=lo_pc)]


class RelocatedProviderTests(unittest.TestCase):
    def test_positive_negative_and_carry_addends(self):
        for addend in (0, 4, -4, 0x8004, 0x138000, -0x8004):
            with self.subTest(addend=addend):
                self.assertEqual(0x335370, h.solve_relocations(pair(addend=addend))["data"]["address"])

    def test_hi_instruction_may_follow_lo_in_code_but_not_table(self):
        records = pair(hi_pc=0x100008, lo_pc=0x100004)
        self.assertEqual([0x100004, 0x100008], h.solve_relocations(records)["data"]["witnesses"])
        with self.assertRaises(h.HistoricalDataError):
            h.solve_relocations(list(reversed(records)))

    def test_different_providers_are_not_conflated(self):
        result = h.solve_relocations(pair(key="@section:.data") + pair(value=0x1b0000, key="@section:.rodata"))
        self.assertEqual(0x335370, result["@section:.data"]["address"])
        self.assertEqual(0x1b0000, result["@section:.rodata"]["address"])

    def test_inconsistent_provider_rejected(self):
        with self.assertRaisesRegex(h.HistoricalDataError, "inconsistent"):
            h.solve_relocations(pair() + pair(value=0x335374))

    def test_multiple_hi_share_one_lo(self):
        first, second = pair(addend=0x8004), pair(addend=0x18004, hi_pc=0x100008)
        self.assertEqual(0x335370, h.solve_relocations([first[0], second[0], first[1]])["data"]["address"])

    def test_unpaired_hi_or_lonely_lo_rejected(self):
        for rows in ([pair()[0]], [pair()[1]]):
            with self.assertRaises(h.HistoricalDataError):
                h.solve_relocations(rows)

    def test_target_opcode_bits_cannot_be_masked_away(self):
        for index in (0, 1):
            rows = pair()
            rows[index]["target"] ^= 1 << 21
            with self.assertRaises(h.HistoricalDataError):
                h.solve_relocations(rows)

    def test_jump_relocation_has_exact_opcode_and_value(self):
        row = dict(kind=4, symbol="fn", source=0x0c000000, target=0x0c000000|(0x19c364>>2), pc=0x100000)
        self.assertEqual(0x19c364, h.solve_relocations([row])["fn"]["address"])
        row["target"] ^= 0x04000000
        with self.assertRaises(h.HistoricalDataError):
            h.solve_relocations([row])

    def test_unknown_relocation_fails_closed(self):
        row = pair()[0]
        row["kind"] = 7
        with self.assertRaisesRegex(h.HistoricalDataError, "unsupported"):
            h.solve_relocations([row])

    def test_section_symbol_key_does_not_collapse_blank_names(self):
        elf = SimpleNamespace(sections=[SimpleNamespace(name=""), SimpleNamespace(name=".data"), SimpleNamespace(name=".rodata")])
        self.assertNotEqual(h.symbol_key(elf, Symbol("",0,0,1,3)), h.symbol_key(elf, Symbol("",0,0,2,3)))
        with self.assertRaises(h.HistoricalDataError):
            h.symbol_key(elf, Symbol("",0,0,0,0))

    def test_unpinned_host_dependency_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            dep = root/"unit.d"
            dep.write_text("x.o: /usr/include/stdio.h\n")
            with self.assertRaisesRegex(h.HistoricalDataError, "unpinned"):
                h.dependency_hashes(dep, [("source",root)])

    def test_dependency_paths_are_normalized_and_hashed(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            source = root/"a.h"
            source.write_bytes(b"hello")
            dep = root/"unit.d"
            dep.write_text(f"x.o: {source}\n")
            self.assertEqual({"source/a.h": h.digest(b"hello")}, h.dependency_hashes(dep, [("source",root)]))


class HistoricalDataManifestTests(unittest.TestCase):
    def setUp(self):
        self.data = h.validate()

    def reject(self, modify):
        data = copy.deepcopy(self.data)
        modify(data)
        with tempfile.TemporaryDirectory() as tmp:
            manifest = Path(tmp)/"changed.json"
            manifest.write_text(json.dumps(data))
            with self.assertRaises((h.HistoricalDataError, ValueError, KeyError)):
                h.validate(manifest)

    def test_public_gate_needs_no_original_or_compiler(self):
        with patch.object(h.libgcc, "load_reference", side_effect=AssertionError("private")), \
             patch.object(h, "run", side_effect=AssertionError("process")):
            self.assertEqual(self.data, h.validate())

    def test_counts_and_scope(self):
        self.assertEqual(49, len(self.data["owners"]))
        self.assertEqual(810542, sum(r["size"] for r in self.data["owners"]))
        self.assertEqual(16, len({r["symbol"] for r in self.data["global_anchors"]}))
        self.assertFalse(self.data["replacement_elf"])

    def test_provider_extent_address_offset_or_identity_drift(self):
        for key, value in (("size",8), ("address",0x345064), ("source_offset",4), ("symbol","other")):
            self.reject(lambda d: d["owners"][0].update({key:value}))

    def test_unreviewed_provider_or_function_removed(self):
        self.reject(lambda d: d["owners"].pop())
        self.reject(lambda d: d["functions"].pop())

    def test_no_fake_completion_claim(self):
        self.reject(lambda d: d.update(replacement_elf=True))
        self.reject(lambda d: d["owners"][0].update(claim="entire original program reconstructed"))

    def test_recipe_changes_require_explicit_private_recapture(self):
        self.reject(lambda d: d.update(recipe_sha256="0"*64))

    def test_anchors_cannot_use_surrounding_unknown_code(self):
        key = next(iter(self.data["bindings"]["snaporig"]))
        self.reject(lambda d: d["bindings"]["snaporig"][key].update(witnesses=[0x200000]))

    def test_anchor_base_cannot_be_shifted(self):
        self.reject(lambda d: d["global_anchors"][0].update(address=0x345060))

    def test_private_hash_drift_is_rejected(self):
        with patch.object(h.libgcc, "load_reference", return_value=bytes(h.libgcc.TARGET_SIZE)):
            with self.assertRaises(h.HistoricalDataError):
                h.verify_reference(Path("unused"), self.data)

    def test_focused_crc_anchor_does_not_assert_string_pool_layout(self):
        self.assertEqual({"pcs": [0x1528b8, 0x1528c4]}, self.data["relocation_filters"]["memmap"])
        self.assertEqual(0x1b61f8, self.data["bindings"]["memmap"]["@section:.rodata"]["address"])
        self.assertEqual("selected-data-references", next(r for r in self.data["functions"] if r["unit"] == "memmap")["relocation_scope"])
        self.reject(lambda d: d["relocation_filters"]["memmap"].update(pcs=[0x1523e0, 0x1523ec]))

    def test_focused_proof_cannot_be_promoted_to_all_function_relocations(self):
        self.reject(lambda d: next(r for r in d["functions"] if r["unit"] == "ppu").update(relocation_scope="complete-function-relocations"))

    def test_extra_focus_symbol_or_pc_rejected(self):
        self.reject(lambda d: d["bindings"]["ppu"].update(unreviewed=dict(d["bindings"]["ppu"]["@section:.data"])))
        self.reject(lambda d: d["bindings"]["memmap"]["@section:.rodata"].update(witnesses=[0x1523e0, 0x1523ec]))

    def test_named_dsp_and_c4_anchors_cannot_disagree(self):
        self.reject(lambda d: next(r for r in d["section_anchors"] if r["unit"] == "c4").update(source_offset=999))
        self.reject(lambda d: d.update(section_anchors=[r for r in d["section_anchors"] if r["unit"] != "dsp1"]))

    def test_rtc_mismatch_and_unreviewed_c4_tables_are_not_promoted(self):
        names = {r["symbol"] for r in self.data["owners"]}
        self.assertFalse(names & {"rtc_f9", "C4TestPattern", "C4SinTable", "C4CosTable"})


class FocusedRelocationTests(unittest.TestCase):
    def infer(self, selection, records=None):
        section = Section(1, ".text", 1, 0, 0, 8, 0, 0, 0)
        sym = Symbol("fn", 0, 8, 1, 18)
        elf = SimpleNamespace(sections=[None, section], data=struct.pack("<II", 0x3c020000, 0x24420000),
                              find_symbol=lambda _name: sym)
        target = struct.pack("<II", 0x3c020033, 0x24424000)
        if records is None:
            records = [(0, 5, "@section:.data"), (4, 6, "@section:.data")]
        with patch.object(h, "relocations", return_value=records), \
             patch.object(h.comparison, "compare_function", return_value=SimpleNamespace(matching=True, unknown_relocation_types=())):
            return h.infer_bindings(elf, [("test", "fn", 0x100000, 8, h.V33)], target, selection)

    def test_exact_pcs_select_and_reapply_both_words(self):
        bindings, proof = self.infer({"pcs": (0x100000, 0x100004)})
        self.assertEqual(0x334000, bindings["@section:.data"]["address"])
        self.assertEqual("selected-data-references", proof[0]["relocation_scope"])

    def test_missing_focus_word_or_empty_selection_fails(self):
        for selection in ({"pcs": (0x100008,)}, {"symbols": ("absent",)}, {"pcs": (0x100000,)}):
            with self.assertRaises(h.HistoricalDataError):
                self.infer(selection)

    def test_focus_never_repairs_orphans_by_instruction_sorting(self):
        with self.assertRaises(h.HistoricalDataError):
            self.infer({"symbols": ("@section:.data",)}, [(4, 6, "@section:.data"), (0, 5, "@section:.data")])
