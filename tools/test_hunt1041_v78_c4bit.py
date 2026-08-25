#!/usr/bin/env python3
from __future__ import annotations

import csv
import unittest
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v78-validated-c4bit-1.tsv"
FRONTIER = ROOT / "analysis" / "matching" / "hunt1041-v78-frontier-map-44.tsv"
ADDRESS = "0x0010d2a8"


def rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V78C4BitTests(unittest.TestCase):
    def test_evidence_is_strict_and_hash_gated(self) -> None:
        evidence = rows(EVIDENCE, "\t")
        self.assertEqual(len(evidence), 1)
        row = evidence[0]
        self.assertEqual(row["address"], ADDRESS)
        self.assertEqual(row["end_address"], "0x0010d4f0")
        self.assertEqual(row["manifest_next"], "0x0010d4f0")
        self.assertEqual(row["historical_identity"], "C4BitPlaneWave")
        self.assertEqual(row["object_symbol"], "_Z14C4BitPlaneWavev")
        self.assertEqual(row["object_size"], "584")
        self.assertEqual(row["boundary"], "exact-next-boundary")
        self.assertEqual(row["result"], "MATCH")
        self.assertEqual(row["differing_bytes"], "0")
        self.assertEqual(row["normalized_equal"], "True")
        self.assertEqual(row["unknown_relocations"], "")
        self.assertEqual(row["relocation_count"], "18")
        self.assertEqual(row["promotion_scope"], "formal-manifest")
        self.assertEqual(
            row["target_span_sha256"],
            "238f6f0603598494941793416566184c83843985afe3bd3913b0df34b83e4c00",
        )
        self.assertTrue(row["target_gate"].startswith("formal-unpacked-elf:"))
        self.assertIn("c4emu.cpp", row["source"])
        self.assertIn("local-t5-before-t4", row["profile"])
        self.assertIn("isolated compiler profile", row["detail"])

    def test_manifests_reference_v78_evidence(self) -> None:
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
            self.assertIn("HUNT1041 V78 strict MATCH", row["notes"])
            self.assertIn("hunt1041-v78-validated-c4bit-1.tsv", row["notes"])

    def test_frontier_map_is_the_frozen_v78_checkpoint(self) -> None:
        mapped = rows(FRONTIER, "\t")
        unmatched = {
            row["address"]
            for row in rows(ROOT / "analysis" / "progress_targets.csv", ",")
            if row["status"] != "MATCHING"
        }
        self.assertEqual(len(mapped), 44)
        mapped_addresses = {row["address"] for row in mapped}
        self.assertLess(unmatched, mapped_addresses)
        self.assertEqual(mapped_addresses - unmatched, {"0x0010c340"})
        self.assertNotIn(ADDRESS, unmatched)
        self.assertEqual(
            Counter(row["track"] for row in mapped),
            {"frontend-ownership": 26, "historical-source": 18},
        )
        self.assertEqual(
            Counter(row["work_packet"] for row in mapped),
            {
                "frontend-ui": 15,
                "frontend-pad": 2,
                "frontend-lifecycle": 9,
                "c4-core": 4,
                "dsp1-float": 3,
                "memory-ps2": 2,
                "snapshot-zsnes": 1,
                "soundux-ps2": 5,
                "spc7110-cache": 3,
            },
        )

    def test_runner_keeps_fail_closed_and_source_level_constraints(self) -> None:
        runner = (
            ROOT / "tools" / "history" / "research" / "hunt1041_v78_c4bit.py"
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
            "READ_WAVE_WORD_REG",
            'register int j __asm__("$16")',
            'READ_WAVE_WORD_REG(dst+bmpdata[i], "$14")',
            'READ_WAVE_WORD_REG(dst+bmpdata[i], "$24")',
            'READ_WAVE_WORD_REG(Memory.C4RAM+0xa00+height*2, "$15")',
            'READ_WAVE_WORD_REG(Memory.C4RAM+0xa10+height*2, "$25")',
            "build_profile(cxx)",
        ):
            self.assertIn(marker, runner)
        self.assertNotIn('lwl %0', runner)
        self.assertNotIn('lwr %0', runner)


if __name__ == "__main__":
    unittest.main()
