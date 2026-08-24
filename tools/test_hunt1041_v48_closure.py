#!/usr/bin/env python3
"""Structural tests for the HUNT1041 V48 strict-evidence runner."""
from __future__ import annotations

import sys
import unittest
from collections import defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

from hunt1041_v48_closure import (  # noqa: E402
    BOUNDED_ZERO_GAPS,
    CANDIDATES,
    listing_bytes,
)


class Hunt1041V48ClosureTests(unittest.TestCase):
    def test_candidate_universe_is_fixed_and_unique(self) -> None:
        self.assertEqual(len(CANDIDATES), 25)
        self.assertEqual(len({item.address for item in CANDIDATES}), 25)
        self.assertTrue(all(item.size > 0 and item.size % 4 == 0 for item in CANDIDATES))

    def test_split_historical_symbols_are_complete_partitions(self) -> None:
        groups: dict[tuple[str, str, int], list[tuple[int, int]]] = defaultdict(list)
        for item in CANDIDATES:
            if item.detail.startswith("historical-symbol-slice-strict"):
                groups[(item.object_key, item.symbol, item.symbol_size)].append(
                    (item.object_offset, item.object_offset + item.size)
                )
        self.assertEqual(len(groups), 3)
        for (_key, _symbol, symbol_size), ranges in groups.items():
            cursor = 0
            for start, end in sorted(ranges):
                self.assertEqual(start, cursor)
                self.assertGreater(end, start)
                cursor = end
            self.assertEqual(cursor, symbol_size)

    def test_only_two_bounded_zero_gaps_are_admitted(self) -> None:
        self.assertEqual(
            [(start, end) for _source, start, end in BOUNDED_ZERO_GAPS],
            [(0x00195F1C, 0x00195F24), (0x00195F5C, 0x00195F64)],
        )
        values, sources = listing_bytes()
        for source, start, end in BOUNDED_ZERO_GAPS:
            self.assertEqual(bytes(values[address] for address in range(start, end)), bytes(end - start))
            for address in range(start, end):
                self.assertIn(source + "#bounded-zero-gap", sources[address])


if __name__ == "__main__":
    unittest.main()
