#!/usr/bin/env python3
from __future__ import annotations

import csv
import unittest
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v73-validated-2.tsv"
FRONTEND_MAP = ROOT / "analysis" / "matching" / "hunt1041-v73-frontend-map-40.tsv"
ADDRESSES = {"0x00114544", "0x001726ec"}
SPAN_HASHES = {
    "0x00114544": "5a0c72f83cad7ca411f73ddb552d5392f840d3e185ee754f073b6ec2504bdd94",
    "0x001726ec": "a850e4b3464d1f47f33d2a907b61addf958f463f3be28dfaf26520ae841f1470",
}


def rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V73HistoricalIoTests(unittest.TestCase):
    def test_evidence_is_strict_and_hash_gated(self) -> None:
        evidence = rows(EVIDENCE, "\t")
        self.assertEqual(len(evidence), 2)
        self.assertEqual({row["address"] for row in evidence}, ADDRESSES)
        for row in evidence:
            self.assertEqual(row["result"], "MATCH")
            self.assertEqual(row["differing_bytes"], "0")
            self.assertEqual(row["normalized_equal"], "True")
            self.assertEqual(row["unknown_relocations"], "")
            self.assertEqual(row["target_span_sha256"], SPAN_HASHES[row["address"]])
            self.assertTrue(row["target_gate"].startswith("formal-unpacked-elf:"))

    def test_boundary_modes_are_explicit(self) -> None:
        evidence = {row["address"]: row for row in rows(EVIDENCE, "\t")}
        cheat = evidence["0x00114544"]
        dump = evidence["0x001726ec"]
        self.assertEqual(cheat["object_size"], "304")
        self.assertEqual(cheat["end_address"], "0x00114674")
        self.assertEqual(cheat["boundary"], "terminal-control-flow-boundary")
        self.assertEqual(dump["object_size"], "488")
        self.assertEqual(dump["end_address"], dump["manifest_next"])
        self.assertEqual(dump["boundary"], "exact-next-boundary")

    def test_manifests_reference_v73_evidence(self) -> None:
        for filename in ("progress_targets.csv", "symbols.csv"):
            selected = [
                row for row in rows(ROOT / "analysis" / filename, ",")
                if row["address"] in ADDRESSES
            ]
            self.assertEqual(len(selected), 2)
            for row in selected:
                self.assertEqual(row["status"], "MATCHING")
                self.assertEqual(row["confidence"], "very-high")
                self.assertIn("HUNT1041 V73 strict MATCH", row["notes"])
                self.assertIn("hunt1041-v73-validated-2.tsv", row["notes"])

    def test_frontend_map_covers_all_40_unproven_entries(self) -> None:
        mapped = rows(FRONTEND_MAP, "\t")
        self.assertEqual(len(mapped), 40)
        self.assertEqual(
            Counter(row["work_packet"] for row in mapped),
            {
                "frontend-ui": 15,
                "multitap": 2,
                "frontend-lifecycle": 9,
                "apu-core": 12,
                "spc7110-record-io": 2,
            },
        )
        frontier = {
            row["address"] for row in rows(ROOT / "analysis" / "progress_targets.csv", ",")
            if row["status"] != "MATCHING"
            and row["area"] in {"frontend-core", "frontend", "multitap"}
        }
        self.assertEqual({row["address"] for row in mapped}, frontier)

    def test_runner_keeps_fail_closed_gates(self) -> None:
        runner = (
            ROOT / "tools" / "history" / "research"
            / "hunt1041_v73_historical_io.py"
        ).read_text(encoding="utf-8")
        for marker in (
            "TARGET_SHA256",
            "strict comparison failed",
            "unknown relocation types",
            "terminal jr-ra boundary changed",
            "following function prologue changed",
            "exact boundary changed",
        ):
            self.assertIn(marker, runner)


if __name__ == "__main__":
    unittest.main()
