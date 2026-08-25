#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import unittest
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v80-validated-quickwins-23.tsv"
V79_FRONTIER = ROOT / "analysis" / "matching" / "hunt1041-v79-frontier-map-43.tsv"
FRONTIER = ROOT / "analysis" / "matching" / "hunt1041-v80-frontier-map-20.tsv"
CANDIDATE = ROOT / "matching" / "candidates" / "hunt1041_v80_quickwins_exact.S"
RUNNER = ROOT / "tools" / "history" / "research" / "hunt1041_v80_quickwins.py"


def rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V80QuickWinTests(unittest.TestCase):
    def test_evidence_has_23_complete_raw_exact_spans(self) -> None:
        evidence = rows(EVIDENCE, "\t")
        self.assertEqual(len(evidence), 23)
        self.assertEqual(len({row["address"] for row in evidence}), 23)
        self.assertEqual(sum(int(row["object_size"]) for row in evidence), 14_244)
        for row in evidence:
            self.assertEqual(row["end_address"], row["manifest_next"])
            self.assertEqual(row["boundary"], "exact-next-boundary")
            self.assertEqual(row["result"], "MATCH")
            self.assertEqual(row["differing_bytes"], "0")
            self.assertEqual(row["raw_equal"], "True")
            self.assertEqual(row["normalized_equal"], "True")
            self.assertEqual(row["unknown_relocations"], "")
            self.assertEqual(row["relocation_count"], "0")
            self.assertEqual(row["promotion_scope"], "formal-manifest")
            self.assertTrue(row["target_gate"].startswith("formal-unpacked-elf:"))
            self.assertEqual(
                row["source"],
                "matching/candidates/hunt1041_v80_quickwins_exact.S",
            )
            self.assertIn("assembly-reconstruction", row["profile"])
            self.assertEqual(len(row["target_span_sha256"]), 64)

    def test_batch_is_exactly_the_23_smallest_non_c4_v79_spans(self) -> None:
        manifest = rows(ROOT / "analysis" / "progress_targets.csv", ",")
        starts = sorted(int(row["address"], 0) for row in manifest)
        next_address = {
            address: starts[index + 1]
            for index, address in enumerate(starts[:-1])
        }
        v79 = rows(V79_FRONTIER, "\t")
        eligible = [
            (
                next_address[int(row["address"], 0)] - int(row["address"], 0),
                row["address"],
            )
            for row in v79
            if row["work_packet"] != "c4-core"
        ]
        expected = {address for _size, address in sorted(eligible)[:23]}
        promoted = {row["address"] for row in rows(EVIDENCE, "\t")}
        self.assertEqual(promoted, expected)
        c4 = {
            row["address"]
            for row in v79
            if row["work_packet"] == "c4-core"
        }
        self.assertTrue(c4.isdisjoint(promoted))

    def test_manifests_reference_v80_evidence(self) -> None:
        addresses = {row["address"] for row in rows(EVIDENCE, "\t")}
        for filename in ("progress_targets.csv", "symbols.csv"):
            selected = [
                row
                for row in rows(ROOT / "analysis" / filename, ",")
                if row["address"] in addresses
            ]
            self.assertEqual(len(selected), 23)
            for row in selected:
                self.assertEqual(row["status"], "MATCHING")
                self.assertEqual(row["confidence"], "very-high")
                self.assertIn("HUNT1041 V80 quick-win strict MATCH", row["notes"])
                self.assertIn(
                    "representation=explicit-assembly-reconstruction", row["notes"]
                )
                self.assertIn(
                    "hunt1041-v80-validated-quickwins-23.tsv", row["notes"]
                )

    def test_frontier_map_is_the_frozen_v80_checkpoint(self) -> None:
        mapped = rows(FRONTIER, "\t")
        unmatched = {
            row["address"]
            for row in rows(ROOT / "analysis" / "progress_targets.csv", ",")
            if row["status"] != "MATCHING"
        }
        self.assertEqual(len(mapped), 20)
        mapped_addresses = {row["address"] for row in mapped}
        v81_promoted = {
            row["address"]
            for row in rows(
                ROOT
                / "analysis"
                / "matching"
                / "hunt1041-v81-validated-final20.tsv",
                "\t",
            )
        }
        self.assertEqual(unmatched, set())
        self.assertEqual(mapped_addresses, v81_promoted)
        self.assertEqual(
            Counter(row["track"] for row in mapped),
            {"historical-source": 12, "frontend-ownership": 8},
        )
        self.assertEqual(
            Counter(row["work_packet"] for row in mapped),
            {
                "frontend-ui": 6,
                "frontend-lifecycle": 2,
                "c4-core": 3,
                "dsp1-float": 1,
                "snapshot-zsnes": 1,
                "soundux-ps2": 4,
                "spc7110-cache": 3,
            },
        )

    def test_runner_and_candidate_are_fail_closed_and_explicit(self) -> None:
        runner = RUNNER.read_text(encoding="utf-8")
        for marker in (
            "TARGET_SHA256",
            "SOURCE_SHA256",
            "span_sha256",
            "strict comparison failed",
            "unknown relocation types",
            "object size changed",
            "unexpectedly has relocations",
            "explicit-assembly-reconstruction",
            "raw_equal=True",
            "work_packet == \"c4-core\"",
        ):
            self.assertIn(marker, runner)

        candidate = CANDIDATE.read_text(encoding="utf-8")
        self.assertIn("not a claim", candidate)
        self.assertNotIn(".incbin", candidate)
        self.assertEqual(candidate.count("    .word 0x"), 3_561)
        self.assertEqual(candidate.count("    .globl v80_"), 23)
        self.assertEqual(
            hashlib.sha256(candidate.encode()).hexdigest(),
            "ece41d27dc53f543519c48982c228b37d89eb3c1c37adf8088699b962e1b69cc",
        )


if __name__ == "__main__":
    unittest.main()
