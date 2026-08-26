import csv
import tempfile
import unittest
from collections import Counter
from pathlib import Path

import named_data


class NamedDataTests(unittest.TestCase):
    def test_original_stage3_partition_is_frozen(self):
        rows = named_data.read_table(named_data.DEFAULT_EXTERNAL, named_data.EXTERNAL_FIELDS)
        self.assertEqual(
            {"3B": 337, "3C": 54, "3D": 53, "3E": 212, "3F": 1265},
            named_data.stage3_partition(rows),
        )

    def test_public_manifest_has_exact_claim_counts(self):
        args = named_data.parse_args(["validate"])
        rows, _layout = named_data.validate_manifest(args)
        counts = Counter(row["status"] for row in rows)
        self.assertEqual(54, len(rows))
        self.assertEqual(10, counts[named_data.PRIVATE_BYTES])
        self.assertEqual(39, counts[named_data.RANGE_PROVED])
        self.assertEqual(3, counts[named_data.ADDRESS_PROVED])
        self.assertEqual(2, counts[named_data.SOURCE_REFACTOR])

    def test_two_target_addresses_were_recovered(self):
        rows = {
            row["symbol"]: row
            for row in named_data.read_manifest(named_data.DEFAULT_MANIFEST)
        }
        self.assertEqual("0x1ebaec", rows["g_memory_card_available"]["target_address"])
        self.assertEqual("0x448200", rows["g_shrink_workspace_recovered"]["target_address"])
        self.assertEqual("0x6000", rows["g_shrink_workspace_recovered"]["extent_hex"])

    def test_synthetic_adapters_are_not_given_fake_target_storage(self):
        rows = {
            row["symbol"]: row
            for row in named_data.read_manifest(named_data.DEFAULT_MANIFEST)
        }
        for symbol in ("g_unz_ops_recovered", "g_zip_io_recovered"):
            self.assertEqual(named_data.SOURCE_REFACTOR, rows[symbol]["status"])
            self.assertEqual("", rows[symbol]["target_address"])
            self.assertEqual("", rows[symbol]["sha256"])

    def test_exact_provider_replacement_is_33_symbols_in_9_clusters(self):
        named = named_data.read_manifest(named_data.DEFAULT_MANIFEST)
        frontier = named_data.read_table(
            named_data.DEFAULT_FRONTIER, named_data.FRONTIER_FIELDS
        )
        replacements = named_data.exact_provider_rows(named, frontier)
        self.assertEqual(33, len(replacements))
        self.assertEqual(9, len(named_data.cluster_ranges(replacements)))

    def test_range_materialization_crosses_into_zero_fill(self):
        layout = {
            "base": 0x1000,
            "initialized_end": 0x1004,
            "memory_end": 0x1008,
            "initialized_size": 4,
            "sha256": named_data.sha256_bytes(b"ABCD"),
        }
        row = {
            "symbol": "mixed",
            "target_address": "0x1002",
            "extent_hex": "0x4",
        }
        self.assertEqual(b"CD\0\0", named_data.range_bytes(b"ABCD", row, layout))

    def test_public_gate_rejects_missing_range_hash(self):
        with named_data.DEFAULT_MANIFEST.open(encoding="utf-8", newline="") as stream:
            rows = list(csv.DictReader(stream, delimiter="\t"))
        target = next(row for row in rows if row["status"] == named_data.RANGE_PROVED)
        target["sha256"] = ""
        with tempfile.TemporaryDirectory() as tmp:
            manifest = Path(tmp) / "named_data.tsv"
            manifest.write_text(named_data.render_tsv(rows), encoding="utf-8")
            args = named_data.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaisesRegex(named_data.NamedDataError, "lacks SHA-256"):
                named_data.validate_manifest(args)


if __name__ == "__main__":
    unittest.main()
