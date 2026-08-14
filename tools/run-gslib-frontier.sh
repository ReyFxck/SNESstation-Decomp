#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

DEFAULT_EE_CC="$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
EE_CC="${EE_CC:-$DEFAULT_EE_CC}"

if [[ ! -x "$EE_CC" ]]; then
    echo "Missing historical EE compiler: $EE_CC" >&2
    echo "Run: make bootstrap-ee-stage1" >&2
    exit 2
fi

mkdir -p build/matching/gslib_hw
LOG="build/matching/gslib_hw/frontier-run.log"

# Critical: never reuse a stale object while iterating matching source.
rm -f build/matching/gslib_hw/gslib_hw.o
rm -f analysis/matching/gslib-hw-listing-report.md

if ! make match-gslib-hw-listing EE_CC="$EE_CC" >"$LOG" 2>&1; then
    echo "=== GSLIB frontier: build failed ==="
    tail -n 80 "$LOG"
    exit 1
fi

echo "=== GSLIB frontier ==="
grep -E 'wrote analysis/matching/gslib-hw-listing-report|matching summary:|^[[:space:]]*(MATCH|MISS)[[:space:]]' "$LOG" \
    | tail -n 20 || true

if grep -q 'matching summary: 7/7' "$LOG"; then
    echo
    echo "=== strict gate ==="
    if make match-gslib-hw-listing-strict EE_CC="$EE_CC" >"$LOG.strict" 2>&1; then
        grep -E 'wrote analysis/matching/gslib-hw-listing-report|7/7|Local GSLIB' "$LOG.strict" | tail -n 10 || true
        echo "GSLIB strict listing gate: OK"
    else
        tail -n 80 "$LOG.strict"
        exit 1
    fi
fi

echo
echo "Full log: $LOG"
