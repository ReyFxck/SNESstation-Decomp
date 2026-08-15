#!/usr/bin/env python3
"""Regression test: MIPS relocations must not hide CDVD register-allocation differences."""
from __future__ import annotations

import unittest
from pathlib import Path

from compare_elf_functions import ELFFile, compare_function


class CdvdRelocationPrecisionTest(unittest.TestCase):
    def test_historical_stop_keeps_register_bits_visible(self) -> None:
        root = Path(__file__).resolve().parents[1]
        target_path = root / "analysis" / "matching" / "cdvd_rpc_target.bin"
        object_path = (
            root
            / "third_party"
            / "historical_refs"
            / "snesticle-9590ebf3"
            / "cdvd_rpc.release.o"
        )
        if not target_path.is_file() or not object_path.is_file():
            self.skipTest("preserved CDVD target/reference artifacts are unavailable")

        target = target_path.read_bytes()
        elf = ELFFile(object_path)
        result = compare_function(
            target,
            0x0019C0D0 - 0x0019BE70,
            0x0019C128 - 0x0019C0D0,
            elf,
            "CDVD_Stop",
        )

        self.assertFalse(result.matching)
        self.assertEqual(result.expected_size, result.candidate_size)
        # +0x1a is the rt field difference in a relocated lw; the legacy
        # whole-word relocation mask incorrectly hid it. +0x36 is the
        # corresponding branch-register difference outside a relocation.
        self.assertIn(0x1A, result.first_differences)
        self.assertIn(0x36, result.first_differences)

    def test_historical_flushcache_has_the_same_ra_fingerprint(self) -> None:
        root = Path(__file__).resolve().parents[1]
        target_path = root / "analysis" / "matching" / "cdvd_rpc_target.bin"
        object_path = (
            root
            / "third_party"
            / "historical_refs"
            / "snesticle-9590ebf3"
            / "cdvd_rpc.release.o"
        )
        if not target_path.is_file() or not object_path.is_file():
            self.skipTest("preserved CDVD target/reference artifacts are unavailable")

        target = target_path.read_bytes()
        elf = ELFFile(object_path)
        result = compare_function(
            target,
            0x0019C2AC - 0x0019BE70,
            0x0019C304 - 0x0019C2AC,
            elf,
            "CDVD_FlushCache",
        )
        self.assertFalse(result.matching)
        self.assertEqual(result.expected_size, result.candidate_size)
        self.assertIn(0x1A, result.first_differences)
        self.assertIn(0x36, result.first_differences)


if __name__ == "__main__":
    unittest.main()
