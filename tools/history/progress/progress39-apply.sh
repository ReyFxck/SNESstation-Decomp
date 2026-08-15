#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS38.md || ! -f src/ps2/gslib_hw_recovered.c ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 38." >&2
  exit 2
fi

python3 - <<'PY'
from pathlib import Path

p = Path("Makefile")
text = p.read_text(encoding="utf-8")

old_vars = (
    "GSLIB_HW_SOURCE := src/ps2/gslib_hw_recovered.c\n"
    "GSLIB_HW_OBJECT := $(MATCH_DIR)/gslib_hw/gslib_hw.o\n"
)
new_vars = (
    "GSLIB_HW_SOURCE := src/ps2/gslib_hw_recovered.c\n"
    "GSLIB_HW_OBJECT := $(MATCH_DIR)/gslib_hw/gslib_hw.o\n"
    "GSLIB_HW_EE_CFLAGS := $(EE_CFLAGS) -Imatching/ee_abi_compat\n"
)
if text.count(old_vars) != 1:
    raise SystemExit("Makefile: GSLIB HW variable block not found exactly once")
text = text.replace(old_vars, new_vars, 1)

old_rule = (
    "$(GSLIB_HW_OBJECT): $(GSLIB_HW_SOURCE) $(GSLIB_HW_MANIFEST) | check-ee-compiler\n"
    '\t@mkdir -p "$(dir $@)"\n'
    "\t$(EE_CC) $(EE_CFLAGS) -c $< -o $@\n"
)
new_rule = (
    "$(GSLIB_HW_OBJECT): $(GSLIB_HW_SOURCE) $(GSLIB_HW_MANIFEST) matching/ee_abi_compat/stdint.h | check-ee-compiler\n"
    '\t@mkdir -p "$(dir $@)"\n'
    "\t$(EE_CC) $(GSLIB_HW_EE_CFLAGS) -c $< -o $@\n"
)
if text.count(old_rule) != 1:
    raise SystemExit("Makefile: GSLIB HW object rule not found exactly once")
text = text.replace(old_rule, new_rule, 1)

p.write_text(text, encoding="utf-8")
PY

mkdir -p matching/ee_abi_compat

cat > matching/ee_abi_compat/stdint.h <<'EOF'
#ifndef SNESSTATION_GSLIB_MATCH_STDINT_H
#define SNESSTATION_GSLIB_MATCH_STDINT_H

/*
 * Matching-only ABI bridge for the isolated stage-one GCC build.
 *
 * The stage-one compiler has no Newlib header installation, while the recovered
 * GSLIB source uses only uint32_t and uintptr_t from <stdint.h>.  On the EE
 * target both are 32-bit unsigned integer types.  This header supplies names,
 * not implementation code, and is intentionally local to the GSLIB probe.
 */
typedef unsigned int uint32_t;
typedef unsigned int uintptr_t;

#endif
EOF

cat > tools/test_progress39_gslib_hw_headers.py <<'EOF'
from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress39GSLIBHWHeadersTests(unittest.TestCase):
    def test_matching_header_is_minimal(self):
        text = (ROOT / "matching/ee_abi_compat/stdint.h").read_text(encoding="utf-8")
        self.assertIn("typedef unsigned int uint32_t;", text)
        self.assertIn("typedef unsigned int uintptr_t;", text)
        self.assertNotIn("_Static_assert", text)
        self.assertNotIn("__SIZEOF_INT128__", text)

    def test_gslib_probe_uses_local_matching_header_only(self):
        text = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn(
            "GSLIB_HW_EE_CFLAGS := $(EE_CFLAGS) -Imatching/ee_abi_compat",
            text,
        )
        self.assertIn("$(EE_CC) $(GSLIB_HW_EE_CFLAGS) -c $< -o $@", text)

    def test_global_matching_flags_remain_unchanged(self):
        text = (ROOT / "Makefile").read_text(encoding="utf-8")
        ee_cflags = next(
            line for line in text.splitlines()
            if line.startswith("EE_CFLAGS ?=")
        )
        self.assertNotIn("ee_abi_compat", ee_cflags)
        self.assertNotIn("ee_stage1_compat", ee_cflags)


if __name__ == "__main__":
    unittest.main()
EOF

cat > docs/PROGRESS39.md <<'EOF'
# Progress 39 — make the GSLIB hw probe self-contained

The first Progress-38 run stopped before byte comparison because the isolated
stage-one EE compiler intentionally has no installed Newlib `stdint.h`.

The historical 101/101 syntax scan solved that with a broad scan-only
compatibility include path.  Matching should not inherit that whole diagnostic
shim, so this progress adds a narrower ABI-only header specifically for the
GSLIB probe:

- `uint32_t` -> 32-bit unsigned int
- `uintptr_t` -> 32-bit unsigned int

No implementation functions, C11 bridges, int128 feature macros, or standard
library behavior are injected.

Only `GSLIB_HW_EE_CFLAGS` gains `-Imatching/ee_abi_compat`; the global
`EE_CFLAGS`, mathfp, libgcc, and historical 101/101 gate remain unchanged.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
make historical-ee-gate EE_CC="$EE_CC"
make match-gslib-hw-listing EE_CC="$EE_CC"
```

The last command is deliberately non-strict.  Its first useful result is the
actual number of relocation-normalized matches in the seven-function GSLIB hw
corridor.
EOF

echo "Progress 39 applied."
echo
echo 'Run:'
echo '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"'
echo '  make check'
echo '  make historical-ee-gate EE_CC="$EE_CC"'
echo '  make match-gslib-hw-listing EE_CC="$EE_CC"'
