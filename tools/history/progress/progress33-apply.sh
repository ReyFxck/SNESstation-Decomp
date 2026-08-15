#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS32.md ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 32." >&2
  exit 2
fi

python3 - <<'PY'
from pathlib import Path
p = Path("Makefile")
text = p.read_text(encoding="utf-8")
old = "EE_SOURCE_SCAN_FLAGS += -w\n"
new = "EE_SOURCE_SCAN_FLAGS += -Iinclude/ee_stage1_compat -w\n"
count = text.count(old)
if count != 1:
    raise SystemExit(f"Makefile: expected exactly one {old!r}, found {count}")
p.write_text(text.replace(old, new), encoding="utf-8")
PY

mkdir -p include/ee_stage1_compat

cat > include/ee_stage1_compat/stdint.h <<'EOF'
#ifndef SNESSTATION_EE_STAGE1_STDINT_H
#define SNESSTATION_EE_STAGE1_STDINT_H

/*
 * Scan-only C99 integer compatibility for the header-less EE GCC 3.2.2
 * bootstrap. Do not add this directory to EE_CFLAGS used by matching.
 *
 * Project target contract: char=8, short=16, int=32, pointer=32.
 */
typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef signed short       int16_t;
typedef unsigned short     uint16_t;
typedef signed int         int32_t;
typedef unsigned int       uint32_t;
typedef signed long long   int64_t;
typedef unsigned long long uint64_t;

typedef signed int         intptr_t;
typedef unsigned int       uintptr_t;

#define INT8_C(v)   v
#define UINT8_C(v)  v##U
#define INT16_C(v)  v
#define UINT16_C(v) v##U
#define INT32_C(v)  v
#define UINT32_C(v) v##U
#define INT64_C(v)  v##LL
#define UINT64_C(v) v##ULL

#define INT8_MIN    (-128)
#define INT8_MAX    127
#define UINT8_MAX   255U
#define INT16_MIN   (-32767 - 1)
#define INT16_MAX   32767
#define UINT16_MAX  65535U
#define INT32_MIN   (-2147483647 - 1)
#define INT32_MAX   2147483647
#define UINT32_MAX  4294967295U

#endif
EOF

cat > include/ee_stage1_compat/string.h <<'EOF'
#ifndef SNESSTATION_EE_STAGE1_STRING_H
#define SNESSTATION_EE_STAGE1_STRING_H

#include <stddef.h>

void *memcpy(void *dst, const void *src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *dst, int c, size_t n);
int memcmp(const void *a, const void *b, size_t n);

size_t strlen(const char *s);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t n);
char *strcat(char *dst, const char *src);
char *strncat(char *dst, const char *src, size_t n);
int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, size_t n);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *s, const char *delim);

#endif
EOF

cat > tools/test_progress33_ee_compat.py <<'EOF'
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
EOF

cat > docs/PROGRESS33.md <<'EOF'
# Progress 33 — scan-only EE stage-one libc compatibility

Progress 32 established that all 95 non-passing translation units reach the
historical EE front end but are blocked by missing standard headers and parser
cascades.

The bootstrapped GCC 3.2.2 stage one is deliberately header-less. Newlib
1.10.0 also predates the generic Newlib `stdint.h`, while the recovered source
models use modern fixed-width spelling for clarity.

Progress 33 therefore adds a deliberately narrow scan-only compatibility
directory:

- `include/ee_stage1_compat/stdint.h`
- `include/ee_stage1_compat/string.h`

Only `EE_SOURCE_SCAN_FLAGS` sees this directory. `EE_CFLAGS`, matching objects,
and the already-closed 7/7 unwind corridor remain unchanged.

These headers are parser scaffolding, not a claim that they were part of the
original 2004 build.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"
make ee-source-scan EE_CC="$EE_CC"
python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv --top 30
```
EOF

echo "Progress 33 applied."
