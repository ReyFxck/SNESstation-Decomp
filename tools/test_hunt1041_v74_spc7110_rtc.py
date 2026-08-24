#!/usr/bin/env python3
from __future__ import annotations

import csv
import unittest
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = (
    ROOT / "analysis" / "matching"
    / "hunt1041-v74-validated-spc7110-rtc-2.tsv"
)
FRONTIER = (
    ROOT / "analysis" / "matching" / "hunt1041-v74-frontier-map-52.tsv"
)
ADDRESSES = {"0x001833a4", "0x001834f0"}
SPAN_HASHES = {
    "0x001833a4": "9b2d2cd4b69daf19b42e337d4680d1a84cca6703bbfd5d5121a522df9eb772b5",
    "0x001834f0": "f6722672e6d348d673fe96239e99cf1f1e70e2db435ad7dd5bdc966bd4c046f0",
}


def rows(path: Path, delimiter: str) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


class Hunt1041V74Spc7110RtcTests(unittest.TestCase):
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
            self.assertIn("spc7110.cpp", row["source"])
            self.assertIn("rtc32", row["profile"])

    def test_sizes_and_boundary_modes_are_explicit(self) -> None:
        evidence = {row["address"]: row for row in rows(EVIDENCE, "\t")}
        save = evidence["0x001833a4"]
        load = evidence["0x001834f0"]
        self.assertEqual(save["historical_identity"], "S9xSaveSPC7110RTC")
        self.assertEqual(save["object_size"], "332")
        self.assertEqual(save["end_address"], save["manifest_next"])
        self.assertEqual(save["boundary"], "exact-next-boundary")
        self.assertEqual(load["historical_identity"], "S9xLoadSPC7110RTC")
        self.assertEqual(load["object_size"], "360")
        self.assertEqual(load["end_address"], "0x00183658")
        self.assertEqual(load["manifest_next"], "0x00183660")
        self.assertEqual(load["boundary"], "terminal-control-flow-boundary")

    def test_manifests_reference_v74_evidence(self) -> None:
        for filename in ("progress_targets.csv", "symbols.csv"):
            selected = [
                row for row in rows(ROOT / "analysis" / filename, ",")
                if row["address"] in ADDRESSES
            ]
            self.assertEqual(len(selected), 2)
            for row in selected:
                self.assertEqual(row["status"], "MATCHING")
                self.assertEqual(row["confidence"], "very-high")
                self.assertIn("HUNT1041 V74 strict MATCH", row["notes"])
                self.assertIn("hunt1041-v74-validated-spc7110-rtc-2.tsv", row["notes"])

    def test_v74_frontier_map_remains_frozen_at_52(self) -> None:
        mapped = rows(FRONTIER, "\t")
        self.assertEqual(len(mapped), 52)
        self.assertEqual(len({row["address"] for row in mapped}), 52)
        self.assertEqual(
            Counter(row["track"] for row in mapped),
            {"frontend-ownership": 26, "historical-source": 26},
        )
        self.assertEqual(
            Counter(row["work_packet"] for row in mapped),
            {
                "frontend-ui": 15,
                "frontend-pad": 2,
                "frontend-lifecycle": 9,
                "c4-core": 12,
                "dsp1-float": 3,
                "memory-ps2": 2,
                "snapshot-zsnes": 1,
                "soundux-ps2": 5,
                "spc7110-cache": 3,
            },
        )

    def test_v73_frontier_misclassifications_are_superseded(self) -> None:
        mapped = {row["address"]: row for row in rows(FRONTIER, "\t")}
        for address in ("0x00104a54", "0x00104bbc"):
            self.assertEqual(mapped[address]["work_packet"], "frontend-pad")
            self.assertNotEqual(mapped[address]["working_translation_unit"],
                                "src/ps2/libmtap_recovered.c")
        identities = {
            "0x0010b8a4": "C4TransfWireFrame",
            "0x0010d7dc": "S9xSetC4",
        }
        for address, identity in identities.items():
            self.assertEqual(mapped[address]["work_packet"], "c4-core")
            self.assertEqual(mapped[address]["historical_identity"], identity)
        self.assertTrue(ADDRESSES.isdisjoint(mapped))

    def test_runner_keeps_fail_closed_gates(self) -> None:
        runner = (
            ROOT / "tools" / "history" / "research"
            / "hunt1041_v74_spc7110_rtc.py"
        ).read_text(encoding="utf-8")
        for marker in (
            "TARGET_SHA256",
            "strict comparison failed",
            "unknown relocation types",
            "exact boundary changed",
            "terminal jr-ra boundary changed",
            "interstitial empty leaf changed",
            "time_t last_used",
            "int last_used",
        ):
            self.assertIn(marker, runner)


if __name__ == "__main__":
    unittest.main()
