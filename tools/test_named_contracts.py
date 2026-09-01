import csv
import tempfile
import unittest
from collections import Counter
from pathlib import Path

import named_contracts
import named_data
from provider_frontier import COMPAT_STORAGE


class NamedContractTests(unittest.TestCase):
    def test_historical_212_row_ledger_is_fully_classified(self):
        args = named_contracts.parse_args(["validate"])
        rows, _layout = named_contracts.validate_manifest(args)
        self.assertEqual(212, len(rows))
        self.assertEqual(
            {
                named_contracts.TEXT_ALIAS_PROVED: 23,
                named_contracts.TARGET_RANGE_PROVED: 164,
                named_contracts.TARGET_ENTRY_PROVED: 2,
                named_contracts.EXTERNAL_ADDRESS_PROVED: 2,
                named_contracts.DATA_ALIAS_PROVED: 1,
                named_contracts.SOURCE_REFACTOR_CLOSED: 20,
            },
            dict(Counter(row["status"] for row in rows)),
        )

    def test_live_source_map_and_historical_ledger_are_distinguished(self):
        external = named_contracts.read_table(
            named_contracts.DEFAULT_EXTERNAL, named_contracts.EXTERNAL_FIELDS
        )
        self.assertEqual(
            {"3B": 337, "3C": 50, "3D": 49, "3E": 191, "3F": 1265},
            named_data.stage3_partition(external),
        )
        live_names = {row["symbol"] for row in external}
        self.assertTrue(set(named_contracts.CLOSED_SOURCE_REFACTORS).isdisjoint(live_names))
        self.assertNotIn("errno", live_names)

    def test_all_seven_zlib_peers_bind_to_recovered_text(self):
        rows = named_contracts.read_table(
            named_contracts.DEFAULT_MANIFEST, named_contracts.MANIFEST_FIELDS
        )
        zlib = [row for row in rows if row["category"] == "zlib-peer"]
        self.assertEqual(7, len(zlib))
        self.assertTrue(
            all(row["status"] == named_contracts.TEXT_ALIAS_PROVED for row in zlib)
        )
        self.assertTrue(all(row["canonical_symbol"] for row in zlib))

    def test_source_refactors_and_errno_alias_do_not_invent_providers(self):
        rows = {
            row["symbol"]: row
            for row in named_contracts.read_table(
                named_contracts.DEFAULT_MANIFEST, named_contracts.MANIFEST_FIELDS
            )
        }
        for symbol in named_contracts.CLOSED_SOURCE_REFACTORS:
            row = rows[symbol]
            self.assertEqual(named_contracts.SOURCE_REFACTOR_CLOSED, row["status"])
            self.assertEqual("", row["target_address"])
            self.assertEqual("", row["extent_hex"])
            self.assertEqual("", row["sha256"])
        self.assertEqual(named_contracts.DATA_ALIAS_PROVED, rows["errno"]["status"])
        self.assertEqual("0x00425a70", rows["errno"]["target_address"])
        self.assertEqual("ps2lib_errno_00425a70", rows["errno"]["canonical_symbol"])

    def test_exact_range_replacements_remove_all_compatibility_storage(self):
        stage3e = named_contracts.read_table(
            named_contracts.DEFAULT_MANIFEST, named_contracts.MANIFEST_FIELDS
        )
        stage3c = named_contracts.read_table(
            named_contracts.DEFAULT_STAGE3C, named_data.MANIFEST_FIELDS
        )
        frontier = named_contracts.read_table(
            named_contracts.DEFAULT_FRONTIER, named_contracts.FRONTIER_FIELDS
        )
        frontier_by_name = {row["symbol"]: row for row in frontier}
        stage3c_replacements = {
            row["symbol"]
            for row in stage3c
            if row["status"] == named_data.RANGE_PROVED
            and row["symbol"] in frontier_by_name
            and frontier_by_name[row["symbol"]]["resolution_kind"] == COMPAT_STORAGE
        }
        stage3e_replacements = {
            row["symbol"]
            for row in stage3e
            if row["status"] == named_contracts.TARGET_RANGE_PROVED
        }
        replacements = stage3c_replacements | stage3e_replacements
        self.assertEqual(32, len(stage3c_replacements))
        self.assertEqual(164, len(stage3e_replacements))
        self.assertEqual(196, len(replacements))
        self.assertFalse(
            any(
                row["resolution_kind"] == COMPAT_STORAGE
                for row in frontier
                if row["symbol"] not in replacements
            )
        )

    def test_stage3e_ranges_have_frozen_cluster_geometry(self):
        rows = named_contracts.read_table(
            named_contracts.DEFAULT_MANIFEST, named_contracts.MANIFEST_FIELDS
        )
        ranges = [
            row for row in rows
            if row["status"] == named_contracts.TARGET_RANGE_PROVED
        ]
        clusters = named_contracts.cluster_ranges(ranges)
        self.assertEqual(49, len(clusters))
        self.assertEqual(
            26_633,
            sum(int(cluster["end"]) - int(cluster["start"]) for cluster in clusters),
        )

    def test_public_gate_rejects_missing_range_hash(self):
        rows = named_contracts.read_table(
            named_contracts.DEFAULT_MANIFEST, named_contracts.MANIFEST_FIELDS
        )
        target = next(
            row for row in rows
            if row["status"] == named_contracts.TARGET_RANGE_PROVED
        )
        target["sha256"] = ""
        with tempfile.TemporaryDirectory() as tmp:
            manifest = Path(tmp) / "named_contracts.tsv"
            manifest.write_text(named_contracts.render_tsv(rows), encoding="utf-8")
            args = named_contracts.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaisesRegex(named_contracts.NamedContractError, "lacks SHA-256"):
                named_contracts.validate_manifest(args)


if __name__ == "__main__":
    unittest.main()
