#!/usr/bin/env python3
"""Unit tests for the deterministic address-anchored match miner."""
from __future__ import annotations

import argparse
import unittest

from run_match_miner import (
    DEFAULT_PROFILES,
    PROFILES,
    has_terminal_control_flow,
    identity_candidates,
    select_profiles,
)


class MatchMinerTests(unittest.TestCase):
    def test_identity_candidates_accept_exact_name_and_known_address_suffix(self) -> None:
        by_name = {"ExactLeaf": 0x00123450}
        known = {0x00123450, 0x001ABCDE}
        self.assertEqual(
            identity_candidates("ExactLeaf", by_name, known),
            {0x00123450: "name-strict"},
        )
        self.assertEqual(
            identity_candidates("recovered_leaf_001abcde", by_name, known),
            {0x001ABCDE: "address-anchored-strict"},
        )
        self.assertEqual(identity_candidates("leaf_00111110", by_name, known), {})

    def test_terminal_control_flow_accepts_return_and_closed_loop(self) -> None:
        little_return = (0x03E00008).to_bytes(4, "little") + b"\0" * 4
        little_loop = (0x1000FFFF).to_bytes(4, "little") + b"\0" * 4
        big_return = (0x03E00008).to_bytes(4, "big") + b"\0" * 4
        self.assertTrue(has_terminal_control_flow(little_return, "<"))
        self.assertTrue(has_terminal_control_flow(little_loop, "<"))
        self.assertTrue(has_terminal_control_flow(big_return, ">"))

    def test_terminal_control_flow_rejects_open_or_malformed_tail(self) -> None:
        ordinary = (0x24020001).to_bytes(4, "little") + b"\0" * 4
        open_loop = (0x10000008).to_bytes(4, "little") + b"\0" * 4
        self.assertFalse(has_terminal_control_flow(ordinary, "<"))
        self.assertFalse(has_terminal_control_flow(open_loop, "<"))
        self.assertFalse(has_terminal_control_flow(b"\0" * 7, "<"))

    def test_profile_selection_is_narrow_by_default_and_deterministic(self) -> None:
        args = argparse.Namespace(full=False, profiles=",".join(DEFAULT_PROFILES))
        self.assertEqual(select_profiles(args), DEFAULT_PROFILES)
        full = argparse.Namespace(full=True, profiles="ignored")
        self.assertEqual(select_profiles(full), tuple(PROFILES))

    def test_profile_selection_rejects_unknown_or_empty_names(self) -> None:
        with self.assertRaises(SystemExit):
            select_profiles(argparse.Namespace(full=False, profiles="o2,not-a-profile"))
        with self.assertRaises(SystemExit):
            select_profiles(argparse.Namespace(full=False, profiles=""))


if __name__ == "__main__":
    unittest.main()
