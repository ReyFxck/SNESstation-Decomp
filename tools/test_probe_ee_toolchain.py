#!/usr/bin/env python3
"""Self-tests for the historical EE compiler probe."""
from __future__ import annotations

import struct
import unittest

from probe_ee_toolchain import base_version_matches, parse_elf_identity, target_matches


class ToolchainProbeTest(unittest.TestCase):
    def test_parses_expected_ee_object_identity(self) -> None:
        header = bytearray(52)
        header[:4] = b"\x7fELF"
        header[4] = 1
        header[5] = 1
        struct.pack_into("<HH", header, 16, 1, 8)
        struct.pack_into("<I", header, 36, 0x20924001)

        identity = parse_elf_identity(bytes(header))

        self.assertTrue(identity.is_ee_relocatable)
        self.assertEqual(identity.flags, 0x20924001)

    def test_rejects_wrong_endianness_for_ee_contract(self) -> None:
        header = bytearray(52)
        header[:4] = b"\x7fELF"
        header[4] = 1
        header[5] = 2
        struct.pack_into(">HH", header, 16, 1, 8)

        self.assertFalse(parse_elf_identity(bytes(header)).is_ee_relocatable)

    def test_version_and_target_contracts_are_deliberately_strict(self) -> None:
        self.assertTrue(base_version_matches("3.2.2\n"))
        self.assertFalse(base_version_matches("3.2.3"))
        self.assertTrue(target_matches("ee"))
        self.assertTrue(target_matches("mips64r5900el-scei-elf"))
        self.assertFalse(target_matches("mips64el-unknown-elf"))


if __name__ == "__main__":
    unittest.main()
