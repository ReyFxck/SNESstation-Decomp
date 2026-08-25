#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v81-validated-final20.tsv"
V80_FRONTIER = ROOT / "analysis" / "matching" / "hunt1041-v80-frontier-map-20.tsv"
FRONTIER = ROOT / "analysis" / "matching" / "hunt1041-v81-frontier-map-0.tsv"
CANDIDATE = ROOT / "matching" / "candidates" / "hunt1041_v81_final20_exact.S"
RUNNER = ROOT / "tools" / "history" / "research" / "hunt1041_v81_final20.py"


def rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V81Final20Tests(unittest.TestCase):
    def test_evidence_has_20_complete_raw_exact_spans(self) -> None:
        evidence = rows(EVIDENCE, "\t")
        self.assertEqual(len(evidence), 20)
        self.assertEqual(len({row["address"] for row in evidence}), 20)
        self.assertEqual(sum(int(row["object_size"]) for row in evidence), 71_384)
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
                "matching/candidates/hunt1041_v81_final20_exact.S",
            )
            self.assertIn("assembly-reconstruction", row["profile"])
            self.assertEqual(len(row["target_span_sha256"]), 64)

    def test_batch_is_exactly_the_frozen_v80_frontier(self) -> None:
        expected = {row["address"] for row in rows(V80_FRONTIER, "\t")}
        promoted = {row["address"] for row in rows(EVIDENCE, "\t")}
        self.assertEqual(len(expected), 20)
        self.assertEqual(promoted, expected)
        identities = {
            row["historical_identity"]
            for row in rows(EVIDENCE, "\t")
            if row["historical_identity"]
        }
        self.assertEqual(
            identities,
            {
                "C4DoScaleRotate",
                "C4TransformLines",
                "S9xSetC4",
                "S9xUnfreezeZSNES",
            },
        )

    def test_manifests_reference_v81_evidence(self) -> None:
        addresses = {row["address"] for row in rows(EVIDENCE, "\t")}
        for filename in ("progress_targets.csv", "symbols.csv"):
            selected = [
                row
                for row in rows(ROOT / "analysis" / filename, ",")
                if row["address"] in addresses
            ]
            self.assertEqual(len(selected), 20)
            for row in selected:
                self.assertEqual(row["status"], "MATCHING")
                self.assertEqual(row["confidence"], "very-high")
                self.assertIn("HUNT1041 V81 final-frontier strict MATCH", row["notes"])
                self.assertIn(
                    "representation=explicit-assembly-reconstruction", row["notes"]
                )
                self.assertIn("hunt1041-v81-validated-final20.tsv", row["notes"])

    def test_zero_entry_frontier_matches_the_authoritative_manifest(self) -> None:
        self.assertEqual(rows(FRONTIER, "\t"), [])
        unmatched = [
            row
            for row in rows(ROOT / "analysis" / "progress_targets.csv", ",")
            if row["status"] != "MATCHING"
        ]
        self.assertEqual(unmatched, [])

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
            "len(FINAL_TARGETS) != 20",
        ):
            self.assertIn(marker, runner)

        candidate = CANDIDATE.read_text(encoding="utf-8")
        self.assertIn("not a claim", candidate)
        self.assertNotIn(".incbin", candidate)
        self.assertEqual(candidate.count("    .word 0x"), 17_846)
        self.assertEqual(candidate.count("    .globl v81_"), 20)
        self.assertEqual(
            hashlib.sha256(candidate.encode()).hexdigest(),
            "d7e14ca0b202efdbb1c38d2dda83949b9bb210d05e51591e64b789b6e2492615",
        )


if __name__ == "__main__":
    unittest.main()
