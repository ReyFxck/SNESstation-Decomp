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
        self.assertEqual(97, len(units))
        self.assertEqual(96, sum(unit.canonical for unit in units))
        alternate = [unit for unit in units if not unit.canonical]
        self.assertEqual(1, len(alternate))
        self.assertEqual("src/ps2/cdvd_rpc_recovered.c", alternate[0].source)
        self.assertEqual(
            "src/ps2/cdvd_rpc_historical_recovered.c", alternate[0].replaces
        )

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
            "FUN_00123456": ("target-function-alias", "link-identity"),
            "snes_vtable_00426c28": ("vtable-or-rtti", "program-data"),
            "embedded_cdvd_irx": ("embedded-binary", "program-data"),
            "memcpy": ("c-runtime", "runtime-member-text-identity"),
            "SifCallRpc": ("ps2-runtime", "runtime-member-text-identity"),
            "puts": ("c-runtime", "runtime-override-identity"),
            "abort": ("c-runtime", "runtime-override-identity"),
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
        self.assertEqual(6, len(constructors))
        self.assertEqual(10, len(vtables))
        self.assertTrue(all(row["source_owner"] and row["object_owner"] for row in rows))
        self.assertTrue(all(row["next_gate"] == "program-data" for row in vtables))

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
            {"translation-unit": 97, "abi-contract": 1, "canonical-aggregate": 1},
            counts,
        )

    def test_tsv_renderer_is_deterministic(self) -> None:
        rows = [{"a": "one", "b": "two"}]
        expected = "a\tb\none\ttwo\n"
        self.assertEqual(expected, MODULE.render_tsv(("a", "b"), rows))


if __name__ == "__main__":
    unittest.main()
