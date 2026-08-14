from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress33EECompatTests(unittest.TestCase):
    def test_compat_is_scan_only(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn(
            "EE_SOURCE_SCAN_FLAGS += -Iinclude/ee_stage1_compat -w",
            makefile,
        )
        ee_cflags_line = next(
            line for line in makefile.splitlines()
            if line.startswith("EE_CFLAGS ?=")
        )
        self.assertNotIn("ee_stage1_compat", ee_cflags_line)

    def test_stdint_has_ee_width_contract(self):
        header = (ROOT / "include/ee_stage1_compat/stdint.h").read_text(
            encoding="utf-8"
        )
        for token in (
            "uint8_t",
            "uint16_t",
            "uint32_t",
            "uint64_t",
            "uintptr_t",
            "UINT32_C",
            "UINT64_C",
        ):
            self.assertIn(token, header)

    def test_string_shim_uses_compiler_stddef(self):
        header = (ROOT / "include/ee_stage1_compat/string.h").read_text(
            encoding="utf-8"
        )
        self.assertIn("#include <stddef.h>", header)
        self.assertIn("strncpy", header)


if __name__ == "__main__":
    unittest.main()
