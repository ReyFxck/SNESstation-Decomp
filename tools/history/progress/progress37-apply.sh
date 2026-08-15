#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS36.md ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 36." >&2
  exit 2
fi

python3 - <<'PY'
from pathlib import Path

p = Path('Makefile')
text = p.read_text(encoding='utf-8')

old = '\tee-source-scan ee-source-scan-strict \\\n'
new = '\tee-source-scan ee-source-scan-strict historical-ee-gate \\\n'
if text.count(old) != 1:
    raise SystemExit('Makefile: PHONY insertion point not found exactly once')
text = text.replace(old, new, 1)

old = '\t@echo "  make ee-source-scan  baseline every C TU against the historical EE front end"\n'
new = old + '\t@echo "  make historical-ee-gate  strict 101/101 EE scan + strict 7/7 unwind listing gate"\n'
if text.count(old) != 1:
    raise SystemExit('Makefile: help insertion point not found exactly once')
text = text.replace(old, new, 1)

anchor = '\n$(MATHFP_CORE_OBJECT):'
gate = '\n# Local historical EE regression gate. The original ELF remains the formal byte gate.\n' + \
       'historical-ee-gate: check match-libgcc-unwind-listing-strict ee-source-scan-strict\n' + \
       '\t@echo "historical EE gate: OK (repository checks + 7/7 unwind + 101/101 C TUs)"\n\n'
if text.count(anchor) != 1:
    raise SystemExit('Makefile: target insertion anchor not found exactly once')
text = text.replace(anchor, gate + anchor, 1)
p.write_text(text, encoding='utf-8')

Path('tools/test_progress37_historical_gate.py').write_text(
    "from __future__ import annotations\n\n"
    "import unittest\n"
    "from pathlib import Path\n\n"
    "ROOT = Path(__file__).resolve().parents[1]\n\n"
    "class Progress37HistoricalGateTests(unittest.TestCase):\n"
    "    def test_gate_combines_strict_checks(self):\n"
    "        makefile = (ROOT / 'Makefile').read_text(encoding='utf-8')\n"
    "        self.assertIn('historical-ee-gate: check match-libgcc-unwind-listing-strict ee-source-scan-strict', makefile)\n"
    "        self.assertIn('historical EE gate: OK', makefile)\n\n"
    "if __name__ == '__main__':\n"
    "    unittest.main()\n",
    encoding='utf-8',
)

Path('docs/PROGRESS37.md').write_text(
    "# Progress 37 — close the local historical EE front-end gate\n\n"
    "Progress 36 closes the historical EE C-front-end compatibility baseline:\n\n"
    "- **101/101** C translation units pass GCC 3.2.2 syntax checking;\n"
    "- **0** missing headers, **0** EE C errors, **0** compiler crashes;\n"
    "- committed libgcc-unwind listing remains **7/7**;\n"
    "- repository host checks and tool tests remain green.\n\n"
    "Progress 37 adds one strict regression target:\n\n"
    "```bash\nmake historical-ee-gate EE_CC=\"$EE_CC\"\n```\n\n"
    "This local gate does not replace formal comparison against the user's legally obtained original ELF.\n",
    encoding='utf-8',
)

print('Progress 37 applied.')
PY

echo
echo 'Run:'
echo '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"'
echo '  make historical-ee-gate EE_CC="$EE_CC"'
echo '  git status --short'
