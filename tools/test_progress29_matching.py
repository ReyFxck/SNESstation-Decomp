from __future__ import annotations

import csv
import importlib.util
import tempfile
import unittest
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def load_module(path: Path, name: str):
    spec = importlib.util.spec_from_file_location(name, path)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


class Progress29Tests(unittest.TestCase):
    def test_manifest_has_twelve_non_overlapping_rows(self):
        path = ROOT / "analysis" / "matching" / "libgcc_unwind_leaves.csv"
        with path.open(encoding="utf-8") as stream:
            rows = list(csv.DictReader(stream))
        self.assertEqual(len(rows), 12)
        ranges = []
        source = (ROOT / "matching" / "candidates" / "libgcc_unwind_leaves.c").read_text(
            encoding="utf-8"
        )
        for row in rows:
            start = int(row["address"], 0)
            end = int(row["end"], 0)
            self.assertLess(start, end)
            ranges.append((start, end))
            self.assertIn(row["object_symbol"] + "(", source)
        self.assertEqual(ranges, sorted(ranges))
        for left, right in zip(ranges, ranges[1:]):
            self.assertLessEqual(left[1], right[0])

    def test_listing_manifest_only_uses_committed_listing_bytes(self):
        full_path = ROOT / "analysis" / "matching" / "libgcc_unwind_leaves.csv"
        listing_path = ROOT / "analysis" / "matching" / "libgcc_unwind_listing.csv"
        with full_path.open(encoding="utf-8") as stream:
            full_rows = list(csv.DictReader(stream))
        with listing_path.open(encoding="utf-8") as stream:
            listing_rows = list(csv.DictReader(stream))

        self.assertEqual(len(full_rows), 12)
        self.assertEqual(len(listing_rows), 7)
        full_keys = {(row["address"], row["object_symbol"]) for row in full_rows}
        for row in listing_rows:
            self.assertIn((row["address"], row["object_symbol"]), full_keys)
            self.assertLessEqual(int(row["end"], 0), 0x001A4100)

    def test_source_scan_flags_do_not_split_on_wa_comma(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertNotIn("filter-out -Werror -Wa,-al", makefile)
        self.assertIn("subst -Wa$(comma)-al", makefile)

    def test_scan_classifier(self):
        module = load_module(ROOT / "tools" / "scan_ee_translation_units.py", "p29scan")
        self.assertEqual(module.classify(0, ""), "PASS")
        self.assertEqual(
            module.classify(1, "x.c:1: fatal error: stdint.h: No such file or directory"),
            "MISSING_HEADER",
        )
        self.assertEqual(module.classify(1, "x.c:2: error: broken"), "EE_C_ERROR")
        self.assertEqual(
            module.classify(1, "internal compiler error: Segmentation fault"),
            "COMPILER_CRASH",
        )

    def test_summary_parser(self):
        module = load_module(ROOT / "tools" / "summarize_matching_report.py", "p29summary")
        text = """# ELF function comparison report

- Result: **1/2 relocation-normalized matches**

| Address | Target | Object symbol | Target / object bytes | Relocations | Result | First non-relocation difference |
|---:|---|---|---:|---:|---|---|
| `0x00100000` | `a` | `a_candidate` | 8 / 8 | 0 | **MATCHING** | — |
| `0x00100008` | `b` | `b_candidate` | 8 / 12 | 0 | **SIZE_MISMATCH** | offset 0x0 |
"""
        matched, total, rows = module.parse_report(text)
        self.assertEqual((matched, total), (1, 2))
        self.assertEqual(rows[0]["status"], "MATCHING")
        self.assertEqual(rows[1]["status"], "SIZE_MISMATCH")


if __name__ == "__main__":
    unittest.main()
