#!/usr/bin/env bash
set -euo pipefail
if [ "$#" -lt 3 ]; then
  echo "usage: $0 START_HEX STOP_HEX OUTPUT" >&2
  exit 2
fi
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
OBJDUMP=${OBJDUMP:-$(command -v llvm-objdump || true)}
if [ -z "$OBJDUMP" ]; then
  echo "llvm-objdump not found" >&2
  exit 1
fi
"$OBJDUMP" -d --start-address="$1" --stop-address="$2" \
  "$ROOT/build/SNES_EMU.analysis.elf" > "$3"
