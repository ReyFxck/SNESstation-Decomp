#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS34.md || ! -f include/ee_stage1_compat/stdint.h ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 34." >&2
  exit 2
fi

python3 - <<'PY'
from pathlib import Path

def replace_once(path, old, new):
    p = Path(path)
    text = p.read_text(encoding="utf-8")
    n = text.count(old)
    if n != 1:
        raise SystemExit(f"{path}: expected exactly one occurrence, found {n}: {old!r}")
    p.write_text(text.replace(old, new), encoding="utf-8")

old1 = (
    "uint32_t gsDriver_getTexSizeFromInt_001b0790(int texsize)\n"
    "{\n"
    "    int power = 0x400;\n"
    "    if (texsize == 0)\n"
    "        return 0;\n\n"
    "    for (int index = 10; index >= 0; --index) {\n"
)
new1 = (
    "uint32_t gsDriver_getTexSizeFromInt_001b0790(int texsize)\n"
    "{\n"
    "    int power = 0x400;\n"
    "    int index;\n\n"
    "    if (texsize == 0)\n"
    "        return 0;\n\n"
    "    for (index = 10; index >= 0; --index) {\n"
)
replace_once("src/ps2/gsfont_recovered.c", old1, new1)

old2 = (
    "{\n"
    "    gsPipeRecovered *pipe = font_pipe(self);\n"
    "    uint32_t col = (uint32_t)colour;\n\n"
    "    for (int i = 0; i < length; ++i) {\n"
)
new2 = (
    "{\n"
    "    gsPipeRecovered *pipe = font_pipe(self);\n"
    "    uint32_t col = (uint32_t)colour;\n"
    "    int i;\n\n"
    "    for (i = 0; i < length; ++i) {\n"
)
replace_once("src/ps2/gsfont_recovered.c", old2, new2)

p = Path("include/ee_stage1_compat/stdint.h")
text = p.read_text(encoding="utf-8")
marker = "typedef unsigned int       uintptr_t;\n\n"
if marker not in text:
    raise SystemExit("stdint compat insertion marker not found")
insert = (
    "typedef unsigned int       uintptr_t;\n\n"
    "#ifndef __SIZEOF_INT128__\n"
    "typedef unsigned int __uint128_t __attribute__((mode(TI)));\n"
    "typedef signed int __int128_t __attribute__((mode(TI)));\n"
    "#define __SIZEOF_INT128__ 16\n"
    "#endif\n\n"
)
if "__uint128_t __attribute__((mode(TI)))" not in text:
    text = text.replace(marker, insert, 1)
p.write_text(text, encoding="utf-8")
PY

cat > include/ee_stage1_compat/ctype.h <<'EOF'
#ifndef SNESSTATION_EE_STAGE1_CTYPE_H
#define SNESSTATION_EE_STAGE1_CTYPE_H

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);

#endif
EOF

cat > tools/test_progress35_ee_final_frontend.py <<'EOF'
from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress35EEFinalFrontendTests(unittest.TestCase):
    def test_scan_ctype_header_exists(self):
        text = (ROOT / "include/ee_stage1_compat/ctype.h").read_text(encoding="utf-8")
        self.assertIn("int isdigit(int c);", text)

    def test_scan_has_legacy_ti_mode_bridge(self):
        text = (ROOT / "include/ee_stage1_compat/stdint.h").read_text(encoding="utf-8")
        self.assertIn("__uint128_t __attribute__((mode(TI)))", text)
        self.assertIn("#define __SIZEOF_INT128__ 16", text)

    def test_gsfont_has_no_c99_for_declarations(self):
        text = (ROOT / "src/ps2/gsfont_recovered.c").read_text(encoding="utf-8")
        self.assertNotIn("for (int ", text)

    def test_ti_bridge_is_not_in_matching_flags(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        ee_cflags = next(
            line for line in makefile.splitlines()
            if line.startswith("EE_CFLAGS ?=")
        )
        self.assertNotIn("ee_stage1_compat", ee_cflags)


if __name__ == "__main__":
    unittest.main()
EOF

cat > docs/PROGRESS35.md <<'EOF'
# Progress 35 — final historical EE front-end blockers

Progress 34 reached 98/101 passing C translation units with no compiler crash
and preserved the strict 7/7 committed-listing libgcc-unwind gate.

The three remaining front-end blockers were isolated:

- `progress21_small_helpers_recovered.c` needs `ctype.h`;
- `gsfont_recovered.c` contains C99 `for (int ...)` declarations, rewritten to
  behavior-equivalent declaration-before-loop form for GCC 3.2.2;
- `progress28_structural_lift_recovered.c` uses modern `__uint128_t` /
  `__SIZEOF_INT128__` spelling. The scan-only compatibility layer maps that
  spelling to GCC's historical `mode(TI)` integer mode.

The TI bridge exists only in `include/ee_stage1_compat/stdint.h`, used by
`EE_SOURCE_SCAN_FLAGS` and never by matching `EE_CFLAGS`.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"
make ee-source-scan EE_CC="$EE_CC"
python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv --top 30
```

If the EE backend rejects `mode(TI)` even under `-fsyntax-only`, retain the
98/101 checkpoint and record Progress 28 as a compiler-capability exception
instead of weakening the matching compiler contract.
EOF

echo "Progress 35 applied."
