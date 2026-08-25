#!/usr/bin/env python3
from __future__ import annotations

import unittest
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import build_ee_gcc_regalloc_profile as profile  # noqa: E402


class BuildEeGccRegallocProfileTests(unittest.TestCase):
    def test_patch_swaps_only_t4_and_t5_local_order(self) -> None:
        source = "prefix\n" + profile.PATCH_CONTEXT + "suffix\n"
        patched = profile.patch_mips_source(source)
        self.assertNotIn(profile.PATCH_CONTEXT, patched)
        self.assertEqual(patched.count(profile.PATCH_REPLACEMENT), 1)
        self.assertIn("reg_alloc_order[12] = 13;", patched)
        self.assertIn("reg_alloc_order[13] = 12;", patched)
        self.assertTrue(patched.startswith("prefix\n"))
        self.assertTrue(patched.endswith("suffix\n"))

    def test_patch_fails_closed_when_context_is_not_unique(self) -> None:
        with self.assertRaises(RuntimeError):
            profile.patch_mips_source("context absent")
        with self.assertRaises(RuntimeError):
            profile.patch_mips_source(profile.PATCH_CONTEXT * 2)

    def test_cc1plus_link_parser_requires_one_backend_link(self) -> None:
        command = profile.find_cc1plus_link_command(
            "gcc -o cc1plus cp/call.o libbackend.a -lm\n"
        )
        self.assertEqual(
            command,
            ["gcc", "-o", "cc1plus", "cp/call.o", "libbackend.a", "-lm"],
        )
        with self.assertRaises(RuntimeError):
            profile.find_cc1plus_link_command("gcc -o cc1 cp/call.o libbackend.a\n")


if __name__ == "__main__":
    unittest.main()
