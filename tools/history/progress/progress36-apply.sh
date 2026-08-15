#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS35.md || ! -f include/ee_stage1_compat/stdint.h ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 35." >&2
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

old = """#ifndef __SIZEOF_INT128__
typedef unsigned int __uint128_t __attribute__((mode(TI)));
typedef signed int __int128_t __attribute__((mode(TI)));
#define __SIZEOF_INT128__ 16
#endif
"""
new = """#ifndef __SIZEOF_INT128__
#define __SIZEOF_INT128__ 16
#endif
"""
replace_once("include/ee_stage1_compat/stdint.h", old, new)

old_test = """    def test_scan_has_legacy_ti_mode_bridge(self):
        text = (ROOT / "include/ee_stage1_compat/stdint.h").read_text(encoding="utf-8")
        self.assertIn("__uint128_t __attribute__((mode(TI)))", text)
        self.assertIn("#define __SIZEOF_INT128__ 16", text)
"""
new_test = """    def test_scan_exposes_int128_feature_without_redeclaring_builtin_type(self):
        text = (ROOT / "include/ee_stage1_compat/stdint.h").read_text(encoding="utf-8")
        self.assertIn("#define __SIZEOF_INT128__ 16", text)
        self.assertNotIn("typedef unsigned int __uint128_t", text)
        self.assertNotIn("typedef signed int __int128_t", text)
"""
replace_once("tools/test_progress35_ee_final_frontend.py", old_test, new_test)

Path("tools/test_progress36_int128_probe.py").write_text("""from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress36Int128ProbeTests(unittest.TestCase):
    def test_scan_shim_only_supplies_missing_size_macro(self):
        text = (ROOT / "include/ee_stage1_compat/stdint.h").read_text(
            encoding="utf-8"
        )
        self.assertIn("#define __SIZEOF_INT128__ 16", text)
        self.assertNotIn("__attribute__((mode(TI)))", text)

    def test_progress28_still_uses_compiler_owned_uint128_name(self):
        text = (ROOT / "src/ps2/progress28_structural_lift_recovered.c").read_text(
            encoding="utf-8"
        )
        self.assertIn("typedef __uint128_t p28_u128;", text)
        self.assertIn("#if defined(__SIZEOF_INT128__)", text)


if __name__ == "__main__":
    unittest.main()
""", encoding="utf-8")

Path("docs/PROGRESS36.md").write_text("""# Progress 36 — correct the EE int128 compatibility probe

Progress 35 accidentally redeclared GCC's internal `__uint128_t` and
`__int128_t` names in the scan-only `stdint.h` shim. The historical EE
compiler immediately proved that those type names already exist: 94
translation units failed with `conflicting types for __uint128_t`.

The real compatibility gap is narrower. GCC 3.2.2 accepts the internal
128-bit type name used by the Progress-28 structural lift, but predates the
modern `__SIZEOF_INT128__` predefined macro used as its feature test.

Progress 36 therefore removes both typedefs and supplies only the missing
`__SIZEOF_INT128__` feature macro. Matching flags remain unchanged.
""", encoding="utf-8")

print("Progress 36 applied.")
PY

echo
echo 'Run:'
echo '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"'
echo '  make check'
echo '  make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"'
echo '  make ee-source-scan EE_CC="$EE_CC"'
echo '  python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv --top 30'
