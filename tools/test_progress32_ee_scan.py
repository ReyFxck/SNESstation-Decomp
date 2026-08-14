from __future__ import annotations

import importlib.util
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def load_scan_module():
    path = ROOT / "tools" / "scan_ee_translation_units.py"
    spec = importlib.util.spec_from_file_location("p32_scan", path)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


class Progress32EEScanTests(unittest.TestCase):
    def test_old_gcc_parse_error_beats_leading_warning(self):
        module = load_scan_module()
        output = (
            "x.c:10: warning: no semicolon at end of struct or union\n"
            "x.c:11: parse error before `member'\n"
        )
        self.assertEqual(
            module.first_diagnostic(output),
            "x.c:11: parse error before `member'",
        )

    def test_old_gcc_missing_header_is_classified(self):
        module = load_scan_module()
        output = "x.c:4:20: stdint.h: No such file or directory\n"
        self.assertEqual(module.classify(1, output), "MISSING_HEADER")

    def test_scan_make_flags_suppress_warning_noise(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        scan_lines = [
            line for line in makefile.splitlines()
            if line.startswith("EE_SOURCE_SCAN_FLAGS")
        ]
        self.assertTrue(any("-w" in line.split() for line in scan_lines))


if __name__ == "__main__":
    unittest.main()
