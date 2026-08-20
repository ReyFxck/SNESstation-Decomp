#!/usr/bin/env python3
"""Structural tests for the HUNT1041 V49 formal-ELF evidence runner."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools" / "research"))

from hunt1041_v49_closure import (  # noqa: E402
    CANDIDATES,
    EXACT,
    TARGET_SHA256,
    TERMINAL,
)


class Hunt1041V49ClosureTests(unittest.TestCase):
    def test_candidate_universe_is_fixed_and_unique(self) -> None:
        self.assertEqual(len(CANDIDATES), 20)
        self.assertEqual(len({item.address for item in CANDIDATES}), 20)
        self.assertEqual(sum(item.boundary == EXACT for item in CANDIDATES), 11)
        self.assertEqual(sum(item.boundary == TERMINAL for item in CANDIDATES), 9)

    def test_every_row_has_a_contiguous_word_aligned_piece_partition(self) -> None:
        for candidate in CANDIDATES:
            cursor = 0
            for piece in sorted(candidate.pieces, key=lambda item: item.target_offset):
                self.assertEqual(piece.target_offset, cursor)
                self.assertGreater(piece.size, 0)
                self.assertEqual(piece.size % 4, 0)
                self.assertLessEqual(piece.object_offset + piece.size, piece.symbol_size)
                if piece.full_target is not None:
                    self.assertEqual(
                        piece.full_target + piece.object_offset,
                        candidate.address + piece.target_offset,
                    )
                cursor += piece.size

    def test_crt0_row_is_the_exact_exit_root_partition(self) -> None:
        candidate = next(item for item in CANDIDATES if item.address == 0x001000E0)
        self.assertEqual(candidate.boundary, EXACT)
        self.assertEqual(
            [(item.symbol, item.target_offset, item.size) for item in candidate.pieces],
            [("_exit", 0, 44), ("_root", 44, 8)],
        )

    def test_only_one_non_tail_terminal_slice_is_admitted(self) -> None:
        non_tail = []
        for candidate in CANDIDATES:
            if candidate.boundary != TERMINAL:
                continue
            self.assertEqual(len(candidate.pieces), 1)
            piece = candidate.pieces[0]
            if piece.object_offset + piece.size != piece.symbol_size:
                non_tail.append(candidate)
        self.assertEqual([item.address for item in non_tail], [0x0013FC10])
        self.assertIn("closed-terminal-prefix", non_tail[0].detail)

    def test_private_reference_hash_is_frozen(self) -> None:
        self.assertEqual(
            TARGET_SHA256,
            "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b",
        )


if __name__ == "__main__":
    unittest.main()
