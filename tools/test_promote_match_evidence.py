#!/usr/bin/env python3
"""Unit tests for strict evidence boundary validation."""
from __future__ import annotations

import unittest

from promote_match_evidence import validate_boundary


class PromoteMatchEvidenceTests(unittest.TestCase):
    def test_accepts_supported_boundary_proofs(self) -> None:
        for value in (
            "exact-next-boundary",
            "terminal-control-flow-boundary",
            "historical-symbol+target-zero-gap:0x4",
            "historical-symbol+target-zero-gap:0x40",
        ):
            with self.subTest(value=value):
                validate_boundary(value)

    def test_rejects_unbounded_or_unaligned_gap_proofs(self) -> None:
        for value in (
            "",
            "target-zero-gap:0x4",
            "historical-symbol+target-zero-gap:0x0",
            "historical-symbol+target-zero-gap:0x2",
            "historical-symbol+target-zero-gap:0x44",
        ):
            with self.subTest(value=value), self.assertRaises(ValueError):
                validate_boundary(value)


if __name__ == "__main__":
    unittest.main()
