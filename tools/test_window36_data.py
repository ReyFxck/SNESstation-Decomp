#!/usr/bin/env python3
"""Public regression guards for the Stage-3L window-36 gate."""
from __future__ import annotations

import unittest

import window36_data as gate


class Window36DataTests(unittest.TestCase):
    def test_source_ranges_are_disjoint_and_inside_window_36(self):
        ranges = sorted((row["address"], row["address"] + row["size"])
                        for row in gate.SOURCE_SECTIONS)
        self.assertTrue(all(left[1] <= right[0] for left, right in zip(ranges, ranges[1:])))
        self.assertTrue(all(0x00340000 <= end and start < 0x00350000 for start, end in ranges))
        self.assertEqual(gate.EXPECTED["source_bytes"], sum(end-start for start, end in ranges))

    def test_semantic_fdes_have_unique_function_extents(self):
        fdes = [fde for group in gate.SEMANTIC_SECTIONS for fde in group["fdes"]]
        self.assertEqual(gate.EXPECTED["semantic_fdes"], len(fdes))
        self.assertEqual(len(fdes), len({(fde["pc"], fde["size"]) for fde in fdes}))
        self.assertTrue(all(op[0] in {"advance_loc4", "def_cfa_offset", "offset_extended_sf"}
                            for fde in fdes for op in fde["ops"]))

    def test_private_payload_is_not_embedded(self):
        text = gate.ROOT.joinpath("tools/window36_data.py").read_text(encoding="utf-8")
        self.assertNotIn(".incbin \"build/SNES_EMU", text)
        self.assertFalse(gate.claims()["private_target_bytes_stored"])
        self.assertFalse(gate.claims()["replacement_elf"])

    def test_frozen_window_roster(self):
        self.assertEqual([12, 13, 14, 36, *range(37, 51)], gate.EXACT_CHUNKS)
        self.assertEqual(0, gate.EXPECTED["window36_differing_bytes"])
        self.assertEqual(18, gate.EXPECTED["exact_chunks"])


if __name__ == "__main__":
    unittest.main()
