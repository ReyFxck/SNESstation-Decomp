from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
COMPAT = ROOT / "include" / "ee_stage1_compat"


class Progress34EEStage1HeadersTests(unittest.TestCase):
    def test_required_scan_headers_exist(self):
        for name in ("stdint.h", "string.h", "stdlib.h", "stdio.h", "errno.h", "math.h"):
            self.assertTrue((COMPAT / name).is_file(), name)

    def test_uint64_max_and_static_assert_bridge(self):
        text = (COMPAT / "stdint.h").read_text(encoding="utf-8")
        self.assertIn("#define UINT64_MAX", text)
        self.assertIn("#define _Static_assert(condition, message)", text)

    def test_stdio_seek_constants(self):
        text = (COMPAT / "stdio.h").read_text(encoding="utf-8")
        self.assertIn("#define SEEK_CUR 1", text)
        self.assertIn("typedef struct __snesstation_scan_FILE FILE;", text)

    def test_compat_remains_scan_only(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("-Iinclude/ee_stage1_compat", makefile)
        ee_cflags = next(
            line for line in makefile.splitlines()
            if line.startswith("EE_CFLAGS ?=")
        )
        self.assertNotIn("ee_stage1_compat", ee_cflags)


if __name__ == "__main__":
    unittest.main()
