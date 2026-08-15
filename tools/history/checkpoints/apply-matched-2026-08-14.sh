#!/usr/bin/env bash
set -euo pipefail
PACK="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
REPO="${1:-$PWD}"
cd "$REPO"
[[ -f Makefile ]] || { echo "Run from SNESstation-Decomp or pass repo path." >&2; exit 2; }
mkdir -p src/ps2 analysis/matching tools
cp "$PACK/MATCHED/gslib_hw/src/ps2/gslib_hw_recovered.c" src/ps2/gslib_hw_recovered.c
cp "$PACK/MATCHED/gslib_hw/analysis/matching/gslib_hw_listing.csv" analysis/matching/gslib_hw_listing.csv
cp "$PACK/MATCHED/gslib_hw/tools/run-gslib-frontier.sh" tools/run-gslib-frontier.sh
chmod +x tools/run-gslib-frontier.sh
echo "Applied MATCHED GSLIB source only. CDVD WIP untouched."
git status --short
