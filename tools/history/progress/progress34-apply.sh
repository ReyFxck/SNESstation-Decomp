#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS33.md || ! -f include/ee_stage1_compat/stdint.h ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 33." >&2
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

replace_once(
    "tools/test_progress32_ee_scan.py",
    '        self.assertIn("EE_SOURCE_SCAN_FLAGS += -w", makefile)\n',
    '        scan_lines = [\n'
    '            line for line in makefile.splitlines()\n'
    '            if line.startswith("EE_SOURCE_SCAN_FLAGS")\n'
    '        ]\n'
    '        self.assertTrue(any("-w" in line.split() for line in scan_lines))\n',
)

replace_once(
    "include/ee_stage1_compat/stdint.h",
    '#define INT32_MIN   (-2147483647 - 1)\n'
    '#define INT32_MAX   2147483647\n'
    '#define UINT32_MAX  4294967295U\n\n'
    '#endif\n',
    '#define INT32_MIN   (-2147483647 - 1)\n'
    '#define INT32_MAX   2147483647\n'
    '#define UINT32_MAX  4294967295U\n'
    '#define INT64_MIN   (-9223372036854775807LL - 1LL)\n'
    '#define INT64_MAX   9223372036854775807LL\n'
    '#define UINT64_MAX  18446744073709551615ULL\n'
    '#define UINTPTR_MAX 4294967295U\n\n'
    '/* GCC 3.2.2 predates C11 _Static_assert. Host syntax still validates it. */\n'
    '#ifndef _Static_assert\n'
    '#define _Static_assert(condition, message)\n'
    '#endif\n\n'
    '#endif\n',
)
PY

cat > include/ee_stage1_compat/stdlib.h <<'EOF'
#ifndef SNESSTATION_EE_STAGE1_STDLIB_H
#define SNESSTATION_EE_STAGE1_STDLIB_H

#include <stddef.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void *malloc(size_t size);
void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void *memalign(size_t alignment, size_t size);

int abs(int value);
long labs(long value);
int atoi(const char *s);
long atol(const char *s);
long strtol(const char *s, char **end, int base);
unsigned long strtoul(const char *s, char **end, int base);

void abort(void) __attribute__((noreturn));
void exit(int status) __attribute__((noreturn));

void qsort(void *base, size_t count, size_t size,
           int (*compare)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t count, size_t size,
              int (*compare)(const void *, const void *));

#endif
EOF

cat > include/ee_stage1_compat/stdio.h <<'EOF'
#ifndef SNESSTATION_EE_STAGE1_STDIO_H
#define SNESSTATION_EE_STAGE1_STDIO_H

#include <stddef.h>
#include <stdarg.h>

typedef struct __snesstation_scan_FILE FILE;

#define EOF      (-1)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int printf(const char *fmt, ...);
int fprintf(FILE *stream, const char *fmt, ...);
int sprintf(char *dst, const char *fmt, ...);
int snprintf(char *dst, size_t size, const char *fmt, ...);
int vprintf(const char *fmt, va_list ap);
int vfprintf(FILE *stream, const char *fmt, va_list ap);
int vsprintf(char *dst, const char *fmt, va_list ap);
int vsnprintf(char *dst, size_t size, const char *fmt, va_list ap);
int puts(const char *s);
int putchar(int c);

FILE *fopen(const char *path, const char *mode);
int fclose(FILE *stream);
size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
int fflush(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
int remove(const char *path);
int rename(const char *oldpath, const char *newpath);

#endif
EOF

cat > include/ee_stage1_compat/errno.h <<'EOF'
#ifndef SNESSTATION_EE_STAGE1_ERRNO_H
#define SNESSTATION_EE_STAGE1_ERRNO_H

extern int errno;

#define EDOM   33
#define ERANGE 34

#endif
EOF

cat > include/ee_stage1_compat/math.h <<'EOF'
#ifndef SNESSTATION_EE_STAGE1_MATH_H
#define SNESSTATION_EE_STAGE1_MATH_H

double fabs(double x);
float fabsf(float x);
double sqrt(double x);
float sqrtf(float x);
double sin(double x);
float sinf(float x);
double cos(double x);
float cosf(float x);
double tan(double x);
float tanf(float x);
double atan(double x);
float atanf(float x);
double atan2(double y, double x);
float atan2f(float y, float x);
double floor(double x);
float floorf(float x);
double ceil(double x);
float ceilf(float x);

#define HUGE_VAL  (__builtin_huge_val())
#define HUGE_VALF (__builtin_huge_valf())

#endif
EOF

cat > tools/test_progress34_ee_stage1_headers.py <<'EOF'
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
EOF

cat > docs/PROGRESS34.md <<'EOF'
# Progress 34 — second EE stage-one compatibility batch

Progress 33 moved the historical EE scan from 6/101 to 69/101 passing
translation units while preserving the 7/7 local libgcc-unwind matching gate.

The remaining diagnostics exposed two environment/compiler-era classes:

1. the header-less stage-one compiler still lacks libc interfaces used by the
   source models (`stdio.h`, `stdlib.h`, `errno.h`, `math.h`);
2. GCC 3.2.2 predates C11 `_Static_assert`, while the recovered source uses
   static assertions to prove target layouts during modern host checks.

This batch extends only `include/ee_stage1_compat`. The `_Static_assert`
bridge is intentionally a no-op in the historical scan because `make check`
continues to compile the same sources as C11 on the host and therefore still
evaluates the real assertions.

It also adds `UINT64_MAX` and the stdio seek constants observed in the scan.

No matching `EE_CFLAGS` are changed. The 7/7 unwind object remains isolated
from these scan-only headers.
EOF

echo "Progress 34 applied."
