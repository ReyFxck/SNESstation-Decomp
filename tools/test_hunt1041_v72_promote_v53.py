#!/usr/bin/env python3
from __future__ import annotations

import csv
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v72-validated-v53-6.tsv"
RECOVERED = ROOT / "analysis" / "matching" / "hunt1041-v53-recovered-target-spans.tsv"
ADDRESSES = {
    "0x00129af4",
    "0x0012a400",
    "0x0015068c",
    "0x00158b74",
    "0x00181bac",
    "0x00182638",
}


def read_rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V72PromotionTests(unittest.TestCase):
    def test_evidence_is_strict_and_matches_frozen_spans(self) -> None:
        evidence = read_rows(EVIDENCE, "\t")
        recovered = {
            row["address"]: row for row in read_rows(RECOVERED, "\t")
        }
        self.assertEqual(len(evidence), 6)
        self.assertEqual({row["address"] for row in evidence}, ADDRESSES)
        for row in evidence:
            self.assertEqual(row["result"], "MATCH")
            self.assertEqual(row["boundary"], "exact-next-boundary")
            self.assertEqual(row["differing_bytes"], "0")
            self.assertEqual(row["normalized_equal"], "True")
            self.assertEqual(row["unknown_relocations"], "")
            self.assertEqual(
                row["target_span_sha256"],
                recovered[row["address"]]["target_span_sha256"],
            )
            self.assertEqual(
                row["object_size"], recovered[row["address"]]["bytes"]
            )

    def test_dma_partitions_cover_one_symbol_without_gap(self) -> None:
        rows = {
            row["address"]: row for row in read_rows(EVIDENCE, "\t")
        }
        first = rows["0x00129af4"]
        second = rows["0x0012a400"]
        self.assertEqual(first["object_symbol"], "S9xDoDMA")
        self.assertEqual(second["object_symbol"], "S9xDoDMA")
        self.assertEqual(first["object_symbol_size"], "6388")
        self.assertEqual(second["object_symbol_size"], "6388")
        self.assertEqual(int(first["object_offset"]), 0)
        self.assertEqual(
            int(first["object_offset"]) + int(first["object_size"]),
            int(second["object_offset"]),
        )
        self.assertEqual(
            int(second["object_offset"]) + int(second["object_size"]), 6388
        )

    def test_authoritative_manifests_reference_v72_evidence(self) -> None:
        for filename in ("progress_targets.csv", "symbols.csv"):
            rows = read_rows(ROOT / "analysis" / filename, ",")
            selected = [row for row in rows if row["address"] in ADDRESSES]
            self.assertEqual(len(selected), 6)
            for row in selected:
                self.assertEqual(row["status"], "MATCHING")
                self.assertEqual(row["confidence"], "very-high")
                self.assertIn("HUNT1041 V72 strict MATCH", row["notes"])
                self.assertIn(
                    "analysis/matching/hunt1041-v72-validated-v53-6.tsv",
                    row["notes"],
                )

    def test_runner_keeps_all_fail_closed_gates(self) -> None:
        runner = (
            ROOT / "tools" / "history" / "research"
            / "hunt1041_v72_promote_v53.py"
        ).read_text(encoding="utf-8")
        for marker in (
            "TARGET_SHA256",
            "exact boundary changed",
            "strict comparison failed",
            "unknown relocation types",
            "frozen target-span SHA-256 mismatch",
            "S9xDoDMA partition coverage is no longer contiguous",
        ):
            self.assertIn(marker, runner)


if __name__ == "__main__":
    unittest.main()
