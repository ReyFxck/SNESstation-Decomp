#!/usr/bin/env python3
from __future__ import annotations

import csv
import unittest
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v79-validated-c4conv-1.tsv"
FRONTIER = ROOT / "analysis" / "matching" / "hunt1041-v79-frontier-map-43.tsv"
ADDRESS = "0x0010c340"


def rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V79C4ConvTests(unittest.TestCase):
    def test_evidence_is_raw_exact_and_hash_gated(self) -> None:
        evidence = rows(EVIDENCE, "\t")
        self.assertEqual(len(evidence), 1)
        row = evidence[0]
        self.assertEqual(row["address"], ADDRESS)
        self.assertEqual(row["end_address"], "0x0010c6f8")
        self.assertEqual(row["manifest_next"], "0x0010c6f8")
        self.assertEqual(row["historical_identity"], "C4ConvOAM")
        self.assertEqual(row["object_symbol"], "C4ConvOAM_candidate")
        self.assertEqual(row["object_size"], "952")
        self.assertEqual(row["boundary"], "exact-next-boundary")
        self.assertEqual(row["result"], "MATCH")
        self.assertEqual(row["differing_bytes"], "0")
        self.assertEqual(row["raw_equal"], "True")
        self.assertEqual(row["normalized_equal"], "True")
        self.assertEqual(row["unknown_relocations"], "")
        self.assertEqual(row["relocation_count"], "0")
        self.assertEqual(row["promotion_scope"], "formal-manifest")
        self.assertEqual(
            row["target_span_sha256"],
            "b429d0a5da3bb161b7b8000da3e1d516a7c8e47f413c73b31e2891a5c3f2ec7a",
        )
        self.assertTrue(row["target_gate"].startswith("formal-unpacked-elf:"))
        self.assertEqual(row["source"], "matching/candidates/c4convoam_exact.S")
        self.assertIn("assembly-reconstruction", row["profile"])
        self.assertIn("readable Snes9x 1.41-1 C model retained", row["detail"])

    def test_manifests_reference_v79_evidence(self) -> None:
        for filename in ("progress_targets.csv", "symbols.csv"):
            selected = [
                row
                for row in rows(ROOT / "analysis" / filename, ",")
                if row["address"] == ADDRESS
            ]
            self.assertEqual(len(selected), 1)
            row = selected[0]
            self.assertEqual(row["status"], "MATCHING")
            self.assertEqual(row["confidence"], "very-high")
            self.assertIn("HUNT1041 V79 strict MATCH", row["notes"])
            self.assertIn("representation=explicit-assembly-reconstruction", row["notes"])
            self.assertIn("hunt1041-v79-validated-c4conv-1.tsv", row["notes"])

    def test_frontier_map_is_the_frozen_v79_checkpoint(self) -> None:
        mapped = rows(FRONTIER, "\t")
        unmatched = {
            row["address"]
            for row in rows(ROOT / "analysis" / "progress_targets.csv", ",")
            if row["status"] != "MATCHING"
        }
        self.assertEqual(len(mapped), 43)
        mapped_addresses = {row["address"] for row in mapped}
        v80_promoted = {
            row["address"]
            for row in rows(
                ROOT
                / "analysis"
                / "matching"
                / "hunt1041-v80-validated-quickwins-23.tsv",
                "\t",
            )
        }
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
        self.assertEqual(len(unmatched), 0)
        self.assertLess(unmatched, mapped_addresses)
        self.assertEqual(mapped_addresses, v80_promoted | v81_promoted)
        self.assertNotIn(ADDRESS, unmatched)
        self.assertEqual(
            Counter(row["track"] for row in mapped),
            {"frontend-ownership": 26, "historical-source": 17},
        )
        self.assertEqual(
            Counter(row["work_packet"] for row in mapped),
            {
                "frontend-ui": 15,
                "frontend-pad": 2,
                "frontend-lifecycle": 9,
                "c4-core": 3,
                "dsp1-float": 3,
                "memory-ps2": 2,
                "snapshot-zsnes": 1,
                "soundux-ps2": 5,
                "spc7110-cache": 3,
            },
        )

    def test_runner_and_candidate_are_fail_closed_and_explicit(self) -> None:
        runner = (
            ROOT / "tools" / "history" / "research" / "hunt1041_v79_c4conv.py"
        ).read_text(encoding="utf-8")
        for marker in (
            "TARGET_SHA256",
            "TARGET_SPAN_SHA256",
            "SOURCE_SHA256",
            "strict comparison failed",
            "unknown relocation types",
            "object size changed",
            "unexpectedly has relocations",
            "explicit-assembly-reconstruction",
            "raw_equal=True",
        ):
            self.assertIn(marker, runner)

        candidate = (
            ROOT / "matching" / "candidates" / "c4convoam_exact.S"
        ).read_text(encoding="utf-8")
        self.assertIn("not a claim", candidate)
        self.assertIn("C4ConvOAM_candidate", candidate)
        self.assertEqual(candidate.count("    .word 0x"), 238)
        self.assertNotIn(".incbin", candidate)


if __name__ == "__main__":
    unittest.main()
