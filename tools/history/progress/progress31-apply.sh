#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f matching/candidates/libgcc_unwind_leaves.c || ! -f docs/PROGRESS30.md ]]; then
    echo "Run this from the SNESstation-Decomp repository root after Progress 30." >&2
    exit 2
fi

python3 - <<'PY'
from pathlib import Path


def replace_exact(path: str, old: str, new: str, expected: int = 1) -> None:
    p = Path(path)
    text = p.read_text(encoding="utf-8")
    count = text.count(old)
    if count != expected:
        raise SystemExit(
            f"{path}: expected {expected} occurrence(s) of {old!r}, found {count}; "
            "refusing to patch an unexpected tree"
        )
    p.write_text(text.replace(old, new), encoding="utf-8")


source = "matching/candidates/libgcc_unwind_leaves.c"

# The target uses signed slti in the GCC switch decision trees.  Keeping the
# mask expression as int (not unsigned int) reproduces that old front-end shape.
replace_exact(source, "switch (encoding & 7u)", "switch (encoding & 7)")
replace_exact(source, "switch (encoding & 0x70u)", "switch (encoding & 0x70)")

# GCC 3.2.2 evaluates (byte & 0x7f) as int before folding it into the 64-bit
# _Unwind_Word result.  Casting before << incorrectly forces EE dsllv; the
# reference uses 32-bit sllv in both LEB128 loops and 64-bit dsllv only for the
# SLEB sign extension.
replace_exact(
    source,
    "result |= ((p29_unwind_word)byte & 0x7fu) << shift;",
    "result |= (byte & 0x7f) << shift;",
    expected=2,
)

# The committed disassembly has 8-byte function alignment padding between two
# helpers.  compare_elf_functions compares an ELF symbol's st_size, which does
# not include the following .p2align bytes, so the local manifest must stop at
# the true function end rather than the next function address.
manifest = "analysis/matching/libgcc_unwind_listing.csv"
replace_exact(
    manifest,
    "0x001a3dc0,0x001a3e30,size_of_encoded_value,",
    "0x001a3dc0,0x001a3e2c,size_of_encoded_value,",
)
replace_exact(
    manifest,
    "0x001a3ee8,0x001a3f28,read_uleb128,",
    "0x001a3ee8,0x001a3f24,read_uleb128,",
)

Path("tools/test_progress31_matching.py").write_text(
    '''from __future__ import annotations

import csv
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class Progress31Tests(unittest.TestCase):
    def test_signed_switch_masks_match_reference_decision_tree(self):
        source = (ROOT / "matching" / "candidates" / "libgcc_unwind_leaves.c").read_text(
            encoding="utf-8"
        )
        self.assertIn("switch (encoding & 7)", source)
        self.assertIn("switch (encoding & 0x70)", source)
        self.assertNotIn("switch (encoding & 7u)", source)
        self.assertNotIn("switch (encoding & 0x70u)", source)

    def test_leb_loop_preserves_int_width_before_unwind_word_or(self):
        source = (ROOT / "matching" / "candidates" / "libgcc_unwind_leaves.c").read_text(
            encoding="utf-8"
        )
        self.assertEqual(source.count("result |= (byte & 0x7f) << shift;"), 2)
        self.assertNotIn("((p29_unwind_word)byte & 0x7fu) << shift", source)

    def test_listing_manifest_excludes_alignment_padding(self):
        path = ROOT / "analysis" / "matching" / "libgcc_unwind_listing.csv"
        with path.open(encoding="utf-8") as stream:
            rows = {row["name"]: row for row in csv.DictReader(stream)}
        self.assertEqual(rows["size_of_encoded_value"]["end"], "0x001a3e2c")
        self.assertEqual(rows["read_uleb128"]["end"], "0x001a3f24")
        self.assertEqual(rows["base_of_encoded_value"]["end"], "0x001a3ee8")
        self.assertEqual(rows["read_sleb128"]["end"], "0x001a3f88")


if __name__ == "__main__":
    unittest.main()
''',
    encoding="utf-8",
)

Path("tools/summarize_ee_scan.py").write_text(
    '''#!/usr/bin/env python3
"""Summarize the historical EE translation-unit scan by first diagnostic."""
from __future__ import annotations

import argparse
import csv
import re
from collections import Counter
from pathlib import Path


LOCATION_RE = re.compile(r"^(?:[^:]+):\\d+(?::\\d+)?:\\s*")


def normalize(text: str) -> str:
    text = LOCATION_RE.sub("", text.strip())
    return " ".join(text.split())


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", type=Path)
    parser.add_argument("--top", type=int, default=20)
    args = parser.parse_args()

    with args.report.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream))

    statuses = Counter(row["status"] for row in rows)
    print(
        "EE scan: "
        + " ".join(
            f"{name}={statuses.get(name, 0)}"
            for name in ("PASS", "MISSING_HEADER", "EE_C_ERROR", "COMPILER_CRASH")
        )
    )

    passed = [row["source"] for row in rows if row["status"] == "PASS"]
    print("\\nPASS translation units:")
    for source in passed:
        print(f"  {source}")

    failures = [row for row in rows if row["status"] != "PASS"]
    grouped = Counter(normalize(row["diagnostic"]) for row in failures)
    print(f"\\nTop {min(args.top, len(grouped))} first diagnostics:")
    for diagnostic, count in grouped.most_common(args.top):
        print(f"  {count:3d}x  {diagnostic}")

    print("\\nFirst failing translation units:")
    for row in failures[: min(25, len(failures))]:
        print(f"  {row['source']}: {normalize(row['diagnostic'])}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
''',
    encoding="utf-8",
)
Path("tools/summarize_ee_scan.py").chmod(0o755)

Path("docs/PROGRESS31.md").write_text(
    '''# Progress 31 — close the committed libgcc unwind leaf probe

Progress 30 produced the first trustworthy historical EE source scan:
6 translation units pass GCC 3.2.2 and 95 require compatibility cleanup.  It
also reduced the local libgcc probe to the seven functions whose target bytes
are actually present in the committed listing.

## 1. Signed switch decision trees

The target `size_of_encoded_value` and `base_of_encoded_value` use `slti` in
the compiler-generated switch trees.  The Progress-30 candidate used unsigned
mask literals, which made GCC emit `sltu`.  Progress 31 keeps the mask expression
as `int`, matching the promotion used by the historical unwind source.

## 2. LEB128 shift width

The target ULEB/SLEB loops use 32-bit `sllv` for `(byte & 0x7f) << shift`, then
merge that value into the 64-bit unwind word.  Casting the byte to the unwind
word before the shift forced `dsllv` and changed the generated loop.  The cast
is now deliberately omitted.  The SLEB sign-extension expression remains a
64-bit shift, as in the target.

## 3. Function size versus alignment padding

The target has four bytes of `.p2align` padding after `size_of_encoded_value`
and `read_uleb128`.  Those bytes sit before the next aligned function but are
not part of the candidate ELF symbol `st_size`.  The local listing manifest now
ends those functions at `0x001a3e2c` and `0x001a3f24` respectively.

## 4. EE scan triage

`tools/summarize_ee_scan.py` groups the 95 old-GCC failures by their first real
diagnostic.  This turns the next source-cleanup phase into batches rather than
editing translation units one by one blindly.

Run:

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make check
make match-libgcc-unwind-listing EE_CC="$EE_CC"
python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv
```

The 12-function formal gate still requires the user's legally obtained packed
reference at `original/SNES_EMU.ELF` and remains the only promotion authority.
''',
    encoding="utf-8",
)

print("Progress 31 applied:")
for path in (
    source,
    manifest,
    "tools/test_progress31_matching.py",
    "tools/summarize_ee_scan.py",
    "docs/PROGRESS31.md",
):
    print(f"  {path}")
PY

printf '\nRun next:\n'
printf '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"\n'
printf '  make check\n'
printf '  make match-libgcc-unwind-listing EE_CC="$EE_CC"\n'
printf '  python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv\n'
printf '\nFormal 12-function gate (after restoring your legal reference ELF):\n'
printf '  mkdir -p original\n'
printf '  cp /path/to/your/SNES_EMU.ELF original/SNES_EMU.ELF\n'
printf '  make reference\n'
printf '  make match-libgcc-unwind EE_CC="$EE_CC"\n'
