#!/usr/bin/env python3
from __future__ import annotations

import csv
import unittest
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v75-validated-c4-5.tsv"
COMPANION = ROOT / "analysis" / "matching" / "hunt1041-v75-c4-companion-1.tsv"
FRONTIER = ROOT / "analysis" / "matching" / "hunt1041-v75-frontier-map-47.tsv"
ADDRESSES = {
    "0x0010b8a4",
    "0x0010bbcc",
    "0x0010becc",
    "0x0010c094",
    "0x0010c1f8",
}
SPAN_HASHES = {
    "0x0010b8a4": "836a03bef39c640f2f73345444026688456ae3e64678907e41517fe9736efc3a",
    "0x0010bbcc": "979b7761e73d9bb38d98c8a2a7e092098a81f2d985c0b4634bc6895f3d6fba26",
    "0x0010becc": "fe5df2ee28aad2cb5f3098f332af85f7e08ba6eecb9c15b5fd60639425446f30",
    "0x0010c094": "4f960ff15e772d288e9469159b2f9b27928aed85916794192aefb05980166bd9",
    "0x0010c1f8": "e7e60c411845301df556805b1d6d058698fd0d2aaaef6683690f9bbb581c9cf8",
}


def rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V75C4Tests(unittest.TestCase):
    def test_formal_evidence_is_strict_and_hash_gated(self) -> None:
        evidence = rows(EVIDENCE, "\t")
        self.assertEqual(len(evidence), 5)
        self.assertEqual({row["address"] for row in evidence}, ADDRESSES)
        for row in evidence:
            self.assertEqual(row["result"], "MATCH")
            self.assertEqual(row["differing_bytes"], "0")
            self.assertEqual(row["normalized_equal"], "True")
            self.assertEqual(row["unknown_relocations"], "")
            self.assertEqual(row["promotion_scope"], "formal-manifest")
            self.assertEqual(row["target_span_sha256"], SPAN_HASHES[row["address"]])
            self.assertTrue(row["target_gate"].startswith("formal-unpacked-elf:"))
            self.assertIn("c4.cpp", row["source"])
            self.assertIn("normal-double", row["profile"])

    def test_sizes_identities_and_boundaries_are_explicit(self) -> None:
        evidence = {row["address"]: row for row in rows(EVIDENCE, "\t")}
        expected = {
            "0x0010b8a4": ("C4TransfWireFrame", "808"),
            "0x0010bbcc": ("C4TransfWireFrame2", "768"),
            "0x0010becc": ("C4CalcWireFrame", "456"),
            "0x0010c094": ("C4Op1F", "224"),
            "0x0010c1f8": ("C4Op0D", "264"),
        }
        for address, (identity, size) in expected.items():
            self.assertEqual(evidence[address]["historical_identity"], identity)
            self.assertEqual(evidence[address]["object_size"], size)
        self.assertEqual(
            evidence["0x0010c094"]["boundary"],
            "exact-object-symbol-boundary+exact-companion",
        )
        self.assertEqual(evidence["0x0010c1f8"]["boundary"], "exact-next-boundary")

    def test_auxiliary_c4op15_pins_the_only_manifest_gap(self) -> None:
        companion = rows(COMPANION, "\t")
        self.assertEqual(len(companion), 1)
        row = companion[0]
        self.assertEqual(row["address"], "0x0010c174")
        self.assertEqual(row["end_address"], "0x0010c1f8")
        self.assertEqual(row["historical_identity"], "C4Op15")
        self.assertEqual(row["object_size"], "132")
        self.assertEqual(row["promotion_scope"], "auxiliary-companion")
        self.assertEqual(row["differing_bytes"], "0")
        self.assertEqual(
            row["target_span_sha256"],
            "bc97f7fe4eb3a27670fdfc05d693ff570fc6580bb6c0b1e2be43a1f28927cbfe",
        )

    def test_manifests_reference_v75_evidence(self) -> None:
        for filename in ("progress_targets.csv", "symbols.csv"):
            selected = [
                row for row in rows(ROOT / "analysis" / filename, ",")
                if row["address"] in ADDRESSES
            ]
            self.assertEqual(len(selected), 5)
            for row in selected:
                self.assertEqual(row["status"], "MATCHING")
                self.assertEqual(row["confidence"], "very-high")
                self.assertIn("HUNT1041 V75 strict MATCH", row["notes"])
                self.assertIn("hunt1041-v75-validated-c4-5.tsv", row["notes"])

    def test_frozen_frontier_map_records_the_v75_remaining_47(self) -> None:
        mapped = rows(FRONTIER, "\t")
        unmatched = {
            row["address"]
            for row in rows(ROOT / "analysis" / "progress_targets.csv", ",")
            if row["status"] != "MATCHING"
        }
        self.assertEqual(len(mapped), 47)
        mapped_addresses = {row["address"] for row in mapped}
        self.assertEqual(
            mapped_addresses - unmatched,
            {"0x0010cdcc", "0x0010d4f0"},
        )
        self.assertEqual(unmatched - mapped_addresses, set())
        self.assertEqual(
            Counter(row["track"] for row in mapped),
            {"frontend-ownership": 26, "historical-source": 21},
        )
        self.assertEqual(
            Counter(row["work_packet"] for row in mapped),
            {
                "frontend-ui": 15,
                "frontend-pad": 2,
                "frontend-lifecycle": 9,
                "c4-core": 7,
                "dsp1-float": 3,
                "memory-ps2": 2,
                "snapshot-zsnes": 1,
                "soundux-ps2": 5,
                "spc7110-cache": 3,
            },
        )
        self.assertTrue(ADDRESSES.isdisjoint({row["address"] for row in mapped}))

    def test_runner_keeps_fail_closed_gates(self) -> None:
        runner = (
            ROOT / "tools" / "history" / "research" / "hunt1041_v75_c4.py"
        ).read_text(encoding="utf-8")
        for marker in (
            "TARGET_SHA256",
            "PATCHED_SOURCE_SHA256",
            "strict comparison failed",
            "unknown relocation types",
            "exact boundary changed",
            "C4Op1F/C4Op15/C4Op0D corridor gate failed",
            'flag != "-fshort-double"',
            '"-fno-builtin"',
            '"__builtin_sqrtf ("',
        ):
            self.assertIn(marker, runner)


if __name__ == "__main__":
    unittest.main()
