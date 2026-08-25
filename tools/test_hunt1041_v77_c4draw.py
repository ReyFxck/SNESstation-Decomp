#!/usr/bin/env python3
from __future__ import annotations

import csv
import unittest
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v77-validated-c4draw-1.tsv"
FRONTIER = ROOT / "analysis" / "matching" / "hunt1041-v77-frontier-map-45.tsv"
ADDRESS = "0x0010cdcc"


def rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V77C4DrawTests(unittest.TestCase):
    def test_evidence_is_strict_and_hash_gated(self) -> None:
        evidence = rows(EVIDENCE, "\t")
        self.assertEqual(len(evidence), 1)
        row = evidence[0]
        self.assertEqual(row["address"], ADDRESS)
        self.assertEqual(row["end_address"], "0x0010cfa4")
        self.assertEqual(row["manifest_next"], "0x0010cfa4")
        self.assertEqual(row["historical_identity"], "C4DrawWireFrame")
        self.assertEqual(row["object_symbol"], "_Z15C4DrawWireFramev")
        self.assertEqual(row["object_size"], "472")
        self.assertEqual(row["boundary"], "exact-next-boundary")
        self.assertEqual(row["result"], "MATCH")
        self.assertEqual(row["differing_bytes"], "0")
        self.assertEqual(row["normalized_equal"], "True")
        self.assertEqual(row["unknown_relocations"], "")
        self.assertEqual(row["relocation_count"], "15")
        self.assertEqual(row["promotion_scope"], "formal-manifest")
        self.assertEqual(
            row["target_span_sha256"],
            "80ff14ccbe949105572ee52bb8f3cc4749586645b9965e3b030cd022c59ef77f",
        )
        self.assertTrue(row["target_gate"].startswith("formal-unpacked-elf:"))
        self.assertIn("c4emu.cpp", row["source"])
        self.assertIn("read3-mask-first", row["profile"])
        self.assertIn("declaring the mask before the loaded value", row["detail"])

    def test_manifests_reference_v77_evidence(self) -> None:
        for filename in ("progress_targets.csv", "symbols.csv"):
            selected = [
                row for row in rows(ROOT / "analysis" / filename, ",")
                if row["address"] == ADDRESS
            ]
            self.assertEqual(len(selected), 1)
            row = selected[0]
            self.assertEqual(row["status"], "MATCHING")
            self.assertEqual(row["confidence"], "very-high")
            self.assertIn("HUNT1041 V77 strict MATCH", row["notes"])
            self.assertIn("hunt1041-v77-validated-c4draw-1.tsv", row["notes"])

    def test_frontier_map_is_the_frozen_v77_checkpoint(self) -> None:
        mapped = rows(FRONTIER, "\t")
        unmatched = {
            row["address"]
            for row in rows(ROOT / "analysis" / "progress_targets.csv", ",")
            if row["status"] != "MATCHING"
        }
        self.assertEqual(len(mapped), 45)
        mapped_addresses = {row["address"] for row in mapped}
        self.assertEqual(len(unmatched), 20)
        self.assertLess(unmatched, mapped_addresses)
        self.assertEqual(len(mapped_addresses - unmatched), 25)
        self.assertNotIn(ADDRESS, unmatched)
        self.assertIn("0x0010c340", mapped_addresses)
        self.assertNotIn("0x0010c340", unmatched)
        self.assertIn("0x0010d2a8", mapped_addresses)
        self.assertNotIn("0x0010d2a8", unmatched)
        self.assertEqual(
            Counter(row["track"] for row in mapped),
            {"frontend-ownership": 26, "historical-source": 19},
        )
        self.assertEqual(
            Counter(row["work_packet"] for row in mapped),
            {
                "frontend-ui": 15,
                "frontend-pad": 2,
                "frontend-lifecycle": 9,
                "c4-core": 5,
                "dsp1-float": 3,
                "memory-ps2": 2,
                "snapshot-zsnes": 1,
                "soundux-ps2": 5,
                "spc7110-cache": 3,
            },
        )

    def test_runner_keeps_fail_closed_and_source_level_constraints(self) -> None:
        runner = (
            ROOT / "tools" / "history" / "research" / "hunt1041_v77_c4draw.py"
        ).read_text(encoding="utf-8")
        for marker in (
            "TARGET_SHA256",
            "TARGET_SPAN_SHA256",
            "PATCHED_SOURCE_SHA256",
            "PATCHED_MEMMAP_SHA256",
            "COMPAT_MEMORY_SHA256",
            "strict comparison failed",
            "unknown relocation types",
            "object size changed",
            'flag != "-fshort-double"',
            '"-fno-builtin"',
            "PACKED_READ_3WORD",
            "register uint32 mask = 0x00ffffff",
            "register uint32 value = ((Hunt1041PackedU32 *) source)->value",
        ):
            self.assertIn(marker, runner)
        self.assertNotIn('lwl %0', runner)
        self.assertNotIn('lwr %0', runner)


if __name__ == "__main__":
    unittest.main()
