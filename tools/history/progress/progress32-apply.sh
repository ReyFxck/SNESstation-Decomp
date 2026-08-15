#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f tools/scan_ee_translation_units.py || ! -f docs/PROGRESS31.md ]]; then
    echo "Run this from the SNESstation-Decomp repository root after Progress 31." >&2
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


replace_exact(
    "Makefile",
    "EE_SOURCE_SCAN_FLAGS := $(filter-out -Werror,$(EE_CFLAGS))\n"
    "EE_SOURCE_SCAN_FLAGS := $(subst -Wa$(comma)-al,,$(EE_SOURCE_SCAN_FLAGS))\n",
    "EE_SOURCE_SCAN_FLAGS := $(filter-out -Werror,$(EE_CFLAGS))\n"
    "EE_SOURCE_SCAN_FLAGS := $(subst -Wa$(comma)-al,,$(EE_SOURCE_SCAN_FLAGS))\n"
    "EE_SOURCE_SCAN_FLAGS += -w\n",
)

scanner = "tools/scan_ee_translation_units.py"

replace_exact(
    scanner,
    '''def first_diagnostic(text: str) -> str:
    lines = [line.strip() for line in text.splitlines() if line.strip()]
    priorities = ("fatal error:", "error:", "warning:")
    for needle in priorities:
        for line in lines:
            if needle in line:
                return line[:1000]
    return lines[0][:1000] if lines else ""
''',
    '''def first_diagnostic(text: str) -> str:
    lines = [line.strip() for line in text.splitlines() if line.strip()]
    priorities = (
        "fatal error:",
        "error:",
        "parse error",
        "syntax error",
        "undeclared",
        "conflicting types",
        "invalid type",
        "invalid operands",
        "invalid use",
        "not supported",
        "no such file or directory",
        "warning:",
    )
    lowered = [line.lower() for line in lines]
    for needle in priorities:
        for line, lower in zip(lines, lowered):
            if needle in lower:
                return line[:1000]
    return lines[0][:1000] if lines else ""
''',
)

replace_exact(
    scanner,
    '''    if "no such file or directory" in lower and (
        "fatal error:" in lower or "cannot find" in lower
    ):
        return "MISSING_HEADER"
''',
    '''    if "no such file or directory" in lower:
        return "MISSING_HEADER"
''',
)

Path("tools/test_progress32_ee_scan.py").write_text(
    '''from __future__ import annotations

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
            "x.c:10: warning: no semicolon at end of struct or union\\n"
            "x.c:11: parse error before `member'\\n"
        )
        self.assertEqual(
            module.first_diagnostic(output),
            "x.c:11: parse error before `member'",
        )

    def test_old_gcc_missing_header_is_classified(self):
        module = load_scan_module()
        output = "x.c:4:20: stdint.h: No such file or directory\\n"
        self.assertEqual(module.classify(1, output), "MISSING_HEADER")

    def test_scan_make_flags_suppress_warning_noise(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("EE_SOURCE_SCAN_FLAGS += -w", makefile)


if __name__ == "__main__":
    unittest.main()
''',
    encoding="utf-8",
)

Path("docs/PROGRESS32.md").write_text(
    "# Progress 32 — lock the 7/7 unwind probe and expose real EE parser blockers\n\n"
    "Progress 31 closed the trustworthy committed-listing unwind corridor at\n"
    "**7/7 relocation-normalized matches** with the bootstrapped EE GCC 3.2.2.\n\n"
    "That local result is a compiler/source fingerprint and a regression gate. It\n"
    "does not replace the formal original-ELF gate, which still requires the user's\n"
    "legally obtained `original/SNES_EMU.ELF`.\n\n"
    "## Why the 95-TU triage needed one more fix\n\n"
    "The historical source scan reported 95 `EE_C_ERROR` translation units, but many\n"
    "stored first diagnostics were warnings. GCC 3.2.x often prints hard parser\n"
    "failures using older wording such as `parse error before ...` without the modern\n"
    "literal `error:` prefix.\n\n"
    "Progress 32 makes the diagnostic pass intentionally quiet with `-w`, extends\n"
    "the diagnostic selector for old GCC wording, and recognizes the stage-one\n"
    "compiler's old `header: No such file or directory` form as `MISSING_HEADER`.\n\n"
    "No recovered source semantics are changed in this progress step.\n\n"
    "## Run\n\n"
    "```bash\n"
    'EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"\n\n'
    'make check\n'
    'make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"\n'
    'make ee-source-scan EE_CC="$EE_CC"\n'
    'python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv --top 30\n'
    "```\n\n"
    "The resulting grouped diagnostics are the input to the next compatibility\n"
    "batch. Fix the largest real parser/type blocker first rather than editing\n"
    "translation units one by one.\n",
    encoding="utf-8",
)

print("Progress 32 applied:")
for path in (
    "Makefile",
    scanner,
    "tools/test_progress32_ee_scan.py",
    "docs/PROGRESS32.md",
):
    print(f"  {path}")
PY

printf '\nRun next:\n'
printf '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"\n'
printf '  make check\n'
printf '  make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"\n'
printf '  make ee-source-scan EE_CC="$EE_CC"\n'
printf '  python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv --top 30\n'
