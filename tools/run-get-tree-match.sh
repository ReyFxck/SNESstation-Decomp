#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

DEFAULT_EE_CC="$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
EE_CC="${EE_CC:-$DEFAULT_EE_CC}"
REPORT="analysis/matching/get-tree-listing-report.md"

if [[ ! -x "$EE_CC" ]]; then
    echo "Missing historical EE compiler/assembler driver: $EE_CC" >&2
    echo "Set EE_CC=/path/to/ee-gcc and rerun." >&2
    exit 2
fi

rm -f build/matching/get_tree/get_tree.o
rm -f build/matching/get_tree/listing.bin
rm -f "$REPORT"

make match-get-tree-listing-strict EE_CC="$EE_CC"

grep -F 'Result: **1/1 relocation-normalized matches**' "$REPORT" >/dev/null

echo
echo "get_tree: 1/1 relocation-normalized MATCH"
echo "get_tree committed-listing strict gate: OK"

if [[ -f original/SNES_EMU.ELF ]]; then
    echo
    echo "Reference ELF present; running the formal function gate too."
    make match-get-tree-strict EE_CC="$EE_CC"
fi
