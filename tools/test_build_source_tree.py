#!/usr/bin/env python3
"""Repository-only tests for the EE build-ready source-tree gate."""
from __future__ import annotations

import csv
import importlib.util
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MODULE_PATH = ROOT / "tools" / "build_source_tree.py"
SPEC = importlib.util.spec_from_file_location("build_source_tree", MODULE_PATH)
assert SPEC is not None and SPEC.loader is not None
MODULE = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = MODULE
SPEC.loader.exec_module(MODULE)


class BuildSourceTreeTests(unittest.TestCase):
    def test_manifest_freezes_complete_source_set(self) -> None:
        units = MODULE.read_manifest(
            ROOT / "analysis" / "source_tree" / "translation_units.tsv"
        )
        self.assertEqual(162, len(units))
        self.assertEqual(162, sum(unit.canonical for unit in units))
        alternate = [unit for unit in units if not unit.canonical]
        self.assertEqual([], alternate)
        cdvd = [unit for unit in units if unit.source == "src/ps2/cdvd_rpc.c"]
        self.assertEqual(1, len(cdvd))
        self.assertTrue(cdvd[0].canonical)
        sifrpc = [unit for unit in units if unit.source == "src/ps2/sifrpc.c"]
        self.assertEqual(1, len(sifrpc))
        self.assertTrue(sifrpc[0].canonical)
        kernel = [unit for unit in units if unit.source == "src/ps2/kernel.S"]
        self.assertEqual(1, len(kernel))
        self.assertTrue(kernel[0].canonical)
        self.assertEqual("asm-cpp", kernel[0].language)

        memcpy = [unit for unit in units if unit.source == "src/ps2/memcpy.S"]
        self.assertEqual(1, len(memcpy))
        self.assertTrue(memcpy[0].canonical)
        self.assertEqual("asm-cpp", memcpy[0].language)
        self.assertEqual("ps2/memcpy.o", memcpy[0].object)

        memset = [unit for unit in units if unit.source == "src/ps2/memset.S"]
        self.assertEqual(1, len(memset))
        self.assertTrue(memset[0].canonical)
        self.assertEqual("asm-cpp", memset[0].language)
        self.assertEqual("ps2/memset.o", memset[0].object)

        memmove = [unit for unit in units if unit.source == "src/ps2/memmove.S"]
        self.assertEqual(1, len(memmove))
        self.assertTrue(memmove[0].canonical)
        self.assertEqual("asm-cpp", memmove[0].language)
        self.assertEqual("ps2/memmove.o", memmove[0].object)

        strcat = [unit for unit in units if unit.source == "src/ps2/strcat.S"]
        self.assertEqual(1, len(strcat))
        self.assertTrue(strcat[0].canonical)
        self.assertEqual("asm-cpp", strcat[0].language)
        self.assertEqual("ps2/strcat.o", strcat[0].object)

        memcmp = [unit for unit in units if unit.source == "src/ps2/memcmp.S"]
        self.assertEqual(1, len(memcmp))
        self.assertTrue(memcmp[0].canonical)
        self.assertEqual("asm-cpp", memcmp[0].language)
        self.assertEqual("ps2/memcmp.o", memcmp[0].object)

        strcpy = [unit for unit in units if unit.source == "src/ps2/strcpy.S"]
        self.assertEqual(1, len(strcpy))
        self.assertTrue(strcpy[0].canonical)
        self.assertEqual("asm-cpp", strcpy[0].language)
        self.assertEqual("ps2/strcpy.o", strcpy[0].object)

        strlen = [unit for unit in units if unit.source == "src/ps2/strlen.S"]
        self.assertEqual(1, len(strlen))
        self.assertTrue(strlen[0].canonical)
        self.assertEqual("asm-cpp", strlen[0].language)
        self.assertEqual("ps2/strlen.o", strlen[0].object)

        strchr = [unit for unit in units if unit.source == "src/ps2/strchr.S"]
        self.assertEqual(1, len(strchr))
        self.assertTrue(strchr[0].canonical)
        self.assertEqual("asm-cpp", strchr[0].language)
        self.assertEqual("ps2/strchr.o", strchr[0].object)

        strcmp = [unit for unit in units if unit.source == "src/ps2/strcmp.S"]
        self.assertEqual(1, len(strcmp))
        self.assertTrue(strcmp[0].canonical)
        self.assertEqual("asm-cpp", strcmp[0].language)
        self.assertEqual("ps2/strcmp.o", strcmp[0].object)

        strncpy = [unit for unit in units if unit.source == "src/ps2/strncpy.S"]
        self.assertEqual(1, len(strncpy))
        self.assertTrue(strncpy[0].canonical)
        self.assertEqual("asm-cpp", strncpy[0].language)
        self.assertEqual("ps2/strncpy.o", strncpy[0].object)

        strncmp = [unit for unit in units if unit.source == "src/ps2/strncmp.S"]
        self.assertEqual(1, len(strncmp))
        self.assertTrue(strncmp[0].canonical)
        self.assertEqual("asm-cpp", strncmp[0].language)
        self.assertEqual("ps2/strncmp.o", strncmp[0].object)

        string = [unit for unit in units if unit.source == "src/ps2/string.c"]
        self.assertEqual(1, len(string))
        self.assertTrue(string[0].canonical)
        self.assertEqual("c", string[0].language)
        self.assertEqual("ps2/string.o", string[0].object)

        strstr = [unit for unit in units if unit.source == "src/ps2/strstr.c"]
        self.assertEqual(1, len(strstr))
        self.assertTrue(strstr[0].canonical)
        self.assertEqual("c", strstr[0].language)
        self.assertEqual("ps2/strstr.o", strstr[0].object)

        strtol = [unit for unit in units if unit.source == "src/ps2/strtol.c"]
        self.assertEqual(1, len(strtol))
        self.assertTrue(strtol[0].canonical)
        self.assertEqual("c", strtol[0].language)
        self.assertEqual("ps2/strtol.o", strtol[0].object)

        strcasecmp = [unit for unit in units if unit.source == "src/ps2/strcasecmp.c"]
        self.assertEqual(1, len(strcasecmp))
        self.assertTrue(strcasecmp[0].canonical)
        self.assertEqual("c", strcasecmp[0].language)
        self.assertEqual("ps2/strcasecmp.o", strcasecmp[0].object)

        strtok = [unit for unit in units if unit.source == "src/ps2/strtok.c"]
        self.assertEqual(1, len(strtok))
        self.assertTrue(strtok[0].canonical)
        self.assertEqual("c", strtok[0].language)
        self.assertEqual("ps2/strtok.o", strtok[0].object)

        for source, obj in (
            ("src/ps2/strncasecmp.c", "ps2/strncasecmp.o"),
            ("src/ps2/ctype.c", "ps2/ctype.o"),
            ("src/ps2/qsort.c", "ps2/qsort.o"),
            ("src/ps2/sbrk.c", "ps2/sbrk.o"),
            ("src/unzip/get_tree.S", "unzip/get_tree.o"),
            ("src/ps2/numtestf.S", "ps2/numtestf.o"),
            ("src/ps2/c4convoam.S", "ps2/c4convoam.o"),
            ("src/ps2/c4doscalerotate.S", "ps2/c4doscalerotate.o"),
            ("src/ps2/c4transformlines.S", "ps2/c4transformlines.o"),
            ("src/ps2/s9xsetc4.S", "ps2/s9xsetc4.o"),
            ("src/snes9x/c4drawwireframe.cpp", "snes9x/c4drawwireframe.o"),
            ("src/snes9x/s9xgetmempointer.cpp", "snes9x/s9xgetmempointer.o"),
            ("src/snes9x/s9xgetbyte.cpp", "snes9x/s9xgetbyte.o"),
            ("src/snes9x/s9xsetbyte.cpp", "snes9x/s9xsetbyte.o"),
            ("src/snes9x/s9xgetword.cpp", "snes9x/s9xgetword.o"),
            ("src/snes9x/s9xsetpcbase.cpp", "snes9x/s9xsetpcbase.o"),
            ("src/snes9x/s9xsetword.cpp", "snes9x/s9xsetword.o"),
            ("src/snes9x/cpushutdown.cpp", "snes9x/cpushutdown.o"),
        ):
            matches = [unit for unit in units if unit.source == source]
            self.assertEqual(1, len(matches))
            self.assertTrue(matches[0].canonical)
            expected_language = (
                "asm-cpp" if source.endswith(".S")
                else "cpp" if source.endswith(".cpp")
                else "c"
            )
            self.assertEqual(expected_language, matches[0].language)
            self.assertEqual(obj, matches[0].object)

        self.assertFalse(any(unit.source == "src/ps2/libkernel_strings_recovered.c" for unit in units))
        self.assertEqual("ps2/kernel.o", kernel[0].object)

    def test_original_ps2lib_tolower_is_owned_by_exact_ctype_source(self) -> None:
        path = ROOT / "analysis" / "source_tree" / "defined_symbol_ownership.tsv"
        with path.open(encoding="utf-8", newline="") as stream:
            rows = list(csv.DictReader(stream, delimiter="\t"))
        matches = [
            row for row in rows
            if row["symbol"] == "tolower"
            and row["binding"] == "global"
            and row["section_class"] == "text"
            and row["size_hex"] == "0x30"
            and row["source"] == "src/ps2/ctype.c"
            and row["object"] == "ps2/ctype.o"
        ]
        self.assertEqual(1, len(matches))

    def test_historical_strtol_omits_only_application_long64_flag(self) -> None:
        old = ["-G0", "-O2", "-mlong64", "-ffreestanding"]
        self.assertEqual(
            ["-G0", "-O2", "-ffreestanding", "-Os"],
            MODULE.effective_source_cflags(old, "src/ps2/strtol.c"),
        )
        self.assertEqual(
            ["-G0", "-O2", "-mlong64", "-ffreestanding", "-Os"],
            MODULE.effective_source_cflags(old, "src/ps2/strncpy.S"),
        )

    def test_c4drawwireframe_uses_frozen_v77_cxx_profile(self) -> None:
        flags = MODULE.effective_source_cflags(
            ["-G0", "-O2", "-fshort-double", "-ffreestanding"],
            "src/snes9x/c4drawwireframe.cpp",
        )
        self.assertIn("-Os", flags)
        self.assertIn("-fno-builtin", flags)
        self.assertIn("-DNO_INLINE_SET_GET", flags)
        self.assertNotIn("-fshort-double", flags)
        self.assertNotIn("-ffreestanding", flags)

    def test_s9xgetmempointer_uses_frozen_cxx_profile(self) -> None:
        flags = MODULE.effective_source_cflags(
            ["-G0", "-O2", "-ffreestanding"],
            "src/snes9x/s9xgetmempointer.cpp",
        )
        self.assertIn("-Os", flags)
        self.assertIn("-fshort-double", flags)
        self.assertIn("-DNO_INLINE_SET_GET", flags)
        self.assertNotIn("-O2", flags)
        self.assertNotIn("-ffreestanding", flags)

    def test_s9xgetbyte_uses_frozen_cxx_profile(self) -> None:
        flags = MODULE.effective_source_cflags(
            ["-G0", "-O2", "-ffreestanding"],
            "src/snes9x/s9xgetbyte.cpp",
        )
        self.assertIn("-Os", flags)
        self.assertIn("-fshort-double", flags)
        self.assertIn("-DNO_INLINE_SET_GET", flags)
        self.assertNotIn("-O2", flags)
        self.assertNotIn("-ffreestanding", flags)

    def test_s9xsetbyte_uses_frozen_cxx_profile(self) -> None:
        flags = MODULE.effective_source_cflags(
            ["-G0", "-O2", "-ffreestanding"],
            "src/snes9x/s9xsetbyte.cpp",
        )
        self.assertIn("-Os", flags)
        self.assertIn("-fshort-double", flags)
        self.assertIn("-DNO_INLINE_SET_GET", flags)
        self.assertNotIn("-O2", flags)
        self.assertNotIn("-ffreestanding", flags)

    def test_s9xgetword_uses_frozen_cxx_profile(self) -> None:
        flags = MODULE.effective_source_cflags(
            ["-G0", "-O2", "-ffreestanding"],
            "src/snes9x/s9xgetword.cpp",
        )
        self.assertIn("-Os", flags)
        self.assertIn("-fshort-double", flags)
        self.assertIn("-DNO_INLINE_SET_GET", flags)
        self.assertIn("-DFAST_LSB_WORD_ACCESS", flags)
        self.assertNotIn("-O2", flags)
        self.assertNotIn("-ffreestanding", flags)

    def test_abi_contract_records_the_nonstandard_ee_widths(self) -> None:
        text = (ROOT / "analysis" / "source_tree" / "ee_abi_contract.c").read_text(
            encoding="utf-8"
        )
        for assertion in (
            "long_is_8",
            "pointer_is_4",
            "size_type_is_8",
            "ptrdiff_type_is_8",
            "double_is_4",
        ):
            self.assertIn(assertion, text)

    def test_external_classification_keeps_later_gates_explicit(self) -> None:
        readiness = MODULE.load_source_readiness()
        cases = {
            "DAT_00341398": ("target-address-data", "program-data"),
            "LAB_0018428c": ("target-function-alias", "link-identity"),
            "LAB_0012f8a8": ("target-address-data", "program-data"),
            "FUN_00123456": ("target-function-alias", "link-identity"),
            "snes_vtable_00426c28": ("vtable-or-rtti", "program-data"),
            "embedded_cdvd_irx": ("embedded-binary", "program-data"),
            "memcpy": ("c-runtime", "runtime-member-text-identity"),
            "SifCallRpc": ("ps2-runtime", "runtime-member-text-identity"),
            "puts": ("c-runtime", "runtime-override-callsite-identity"),
            "abort": ("c-runtime", "runtime-override-callsite-identity"),
            "_Z10C4DrawLineiisiish": ("target-function-contract", "link-identity"),
            "S9xGetPPU": ("target-function-contract", "link-identity"),
            "S9xSetPPU": ("target-function-contract", "link-identity"),
            "g_CPU_blob": ("target-address-data", "program-data"),
            "g_OpenBus_byte": ("target-address-data", "program-data"),
            "g_SA1_blob": ("target-address-data", "program-data"),
            "g_s7r_blob": ("target-address-data", "program-data"),
            "__gxx_personality_v0": ("cxx-runtime", "archive-identity"),
        }
        for symbol, expected in cases.items():
            category, _provider, _owner, gate = MODULE.classify_external(symbol, readiness)
            self.assertEqual(expected, (category, gate), symbol)

    def test_frozen_maps_have_unique_rows_and_explicit_owners(self) -> None:
        defined_path = (
            ROOT / "analysis" / "source_tree" / "defined_symbol_ownership.tsv"
        )
        external_path = (
            ROOT / "analysis" / "source_tree" / "external_symbol_ownership.tsv"
        )
        if not defined_path.exists() or not external_path.exists():
            self.skipTest("ownership maps are created by source-tree-refresh")
        with defined_path.open(encoding="utf-8", newline="") as stream:
            defined = list(csv.DictReader(stream, delimiter="\t"))
        with external_path.open(encoding="utf-8", newline="") as stream:
            external = list(csv.DictReader(stream, delimiter="\t"))
        self.assertTrue(defined)
        self.assertTrue(external)
        self.assertEqual(len(external), len({row["symbol"] for row in external}))
        self.assertNotIn("common", {row["section_class"] for row in defined})
        self.assertTrue(all(row["owner"] and row["resolution_gate"] for row in external))

    def test_constructors_and_vtable_consumers_have_explicit_owners(self) -> None:
        path = ROOT / "analysis" / "source_tree" / "special_ownership.tsv"
        with path.open(encoding="utf-8", newline="") as stream:
            rows = list(csv.DictReader(stream, delimiter="\t"))
        constructors = [row for row in rows if row["kind"] == "constructor"]
        vtables = [row for row in rows if row["kind"] == "vtable"]
        retired_data = [row for row in rows if row["kind"] == "retired-target-data"]
        self.assertEqual(6, len(constructors))
        self.assertEqual(10, len(vtables))
        self.assertEqual(15, len(retired_data))
        self.assertTrue(all(row["source_owner"] and row["object_owner"] for row in rows))
        self.assertTrue(all(row["next_gate"] == "program-data" for row in vtables))
        self.assertTrue(all(row["object_owner"] == "reserved:target-data.o" for row in retired_data))
        self.assertTrue(all(row["stage2_state"] == "consumer-retired" for row in retired_data))
        self.assertTrue(all(row["next_gate"] == "program-data" for row in retired_data))

    def test_fingerprints_cover_every_unit_abi_and_aggregate(self) -> None:
        path = ROOT / "analysis" / "source_tree" / "object_fingerprints.tsv"
        if not path.exists():
            self.skipTest("fingerprints are created by source-tree-refresh")
        with path.open(encoding="utf-8", newline="") as stream:
            rows = list(csv.DictReader(stream, delimiter="\t"))
        counts = {kind: 0 for kind in ("translation-unit", "abi-contract", "canonical-aggregate")}
        for row in rows:
            counts[row["kind"]] = counts.get(row["kind"], 0) + 1
            self.assertRegex(row["sha256"], r"^[0-9a-f]{64}$")
        self.assertEqual(
            {"translation-unit": 162, "abi-contract": 1, "canonical-aggregate": 1},
            counts,
        )

    def test_tsv_renderer_is_deterministic(self) -> None:
        rows = [{"a": "one", "b": "two"}]
        expected = "a\tb\none\ttwo\n"
        self.assertEqual(expected, MODULE.render_tsv(("a", "b"), rows))


if __name__ == "__main__":
    unittest.main()
