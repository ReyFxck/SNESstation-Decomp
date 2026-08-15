#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS39.md ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 39." >&2
  exit 2
fi
python3 - <<'PY'
from pathlib import Path

p = Path('Makefile')
text = p.read_text(encoding='utf-8')
old = 'GSLIB_HW_EE_CFLAGS := $(EE_CFLAGS) -Imatching/ee_abi_compat\n'
new = 'GSLIB_HW_EE_CFLAGS = $(EE_CFLAGS) -Imatching/ee_abi_compat\n'
if text.count(old) != 1:
    raise SystemExit('Makefile: expected Progress 39 assignment exactly once')
text = text.replace(old, new, 1)
p.write_text(text, encoding='utf-8')

tp = Path('tools/test_progress39_gslib_hw_headers.py')
tt = tp.read_text(encoding='utf-8')
tt = tt.replace('GSLIB_HW_EE_CFLAGS := $(EE_CFLAGS) -Imatching/ee_abi_compat', 'GSLIB_HW_EE_CFLAGS = $(EE_CFLAGS) -Imatching/ee_abi_compat')
tp.write_text(tt, encoding='utf-8')

Path('tools/test_progress40_gslib_flag_expansion.py').write_text('from __future__ import annotations\n\nimport unittest\nfrom pathlib import Path\n\nROOT = Path(__file__).resolve().parents[1]\n\n\nclass Progress40GSLIBFlagExpansionTests(unittest.TestCase):\n    def test_gslib_flags_use_recursive_make_expansion(self):\n        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")\n        self.assertIn(\n            "GSLIB_HW_EE_CFLAGS = $(EE_CFLAGS) -Imatching/ee_abi_compat",\n            makefile,\n        )\n        self.assertNotIn(\n            "GSLIB_HW_EE_CFLAGS := $(EE_CFLAGS) -Imatching/ee_abi_compat",\n            makefile,\n        )\n\n\nif __name__ == "__main__":\n    unittest.main()\n', encoding='utf-8')

Path('docs/PROGRESS40.md').write_text('# Progress 40 — fix GSLIB matching flag expansion\n\nThe first real GSLIB listing probe reported 0/7, but its compiler command\nshowed that the probe had lost the historical EE compiler flags entirely:\n\n```text\nee-gcc -Imatching/ee_abi_compat -c ...\n```\n\nThe cause was GNU make evaluation order. `GSLIB_HW_EE_CFLAGS` used `:=` before\n`EE_CFLAGS` was defined, so `$(EE_CFLAGS)` was expanded immediately to an empty\nstring.\n\nProgress 40 changes that assignment to recursive `=` expansion:\n\n```make\nGSLIB_HW_EE_CFLAGS = $(EE_CFLAGS) -Imatching/ee_abi_compat\n```\n\nNow `EE_CFLAGS` is evaluated when the recipe runs, after its definition exists.\n\nThe previous 0/7 result is invalid as a compiler-match measurement.\n\n## Run\n\n```bash\nEE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"\n\nmake check\nrm -f build/matching/gslib_hw/gslib_hw.o\nmake match-gslib-hw-listing EE_CC="$EE_CC"\n```\n\nThe compile line must visibly contain the full historical flags, including\n`-G0 -O2 -fomit-frame-pointer -march=r5900 -mtune=r5900`.\n', encoding='utf-8')
print('Progress 40 applied.')
PY


echo 'Run:'
echo '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"'
echo '  make check'
echo '  rm -f build/matching/gslib_hw/gslib_hw.o'
echo '  make match-gslib-hw-listing EE_CC="$EE_CC"'
