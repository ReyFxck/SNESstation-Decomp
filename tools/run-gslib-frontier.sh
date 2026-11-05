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

    echo
    echo "=== whole-corridor raw-link gate ==="
    EE_BINDIR="$(dirname "$EE_CC")"
    EE_LD="$EE_BINDIR/ee-ld"
    EE_OBJCOPY="$EE_BINDIR/ee-objcopy"
    EE_OBJDUMP="$EE_BINDIR/ee-objdump"
    LINKER="build/matching/gslib_hw/gslib_hw.target.ld"
    LINKED="build/matching/gslib_hw/gslib_hw.target-linked.elf"
    LINKED_TEXT="build/matching/gslib_hw/gslib_hw.target-linked.text.bin"
    cat >"$LINKER" <<'LDS'
SECTIONS
{
  . = 0x0019bd38;
  .text : { *(.text) }

  . = 0x00425a30;
  .bss (NOLOAD) : { *(.bss) *(COMMON) }

  /DISCARD/ : {
    *(.comment)
    *(.mdebug*)
    *(.pdr)
    *(.reginfo)
  }
}
LDS
    "$EE_LD" -EL -T "$LINKER" build/matching/gslib_hw/gslib_hw.o -o "$LINKED"
    "$EE_OBJCOPY" -j .text -O binary "$LINKED" "$LINKED_TEXT"
    if ! cmp -s build/matching/gslib_hw/listing.bin "$LINKED_TEXT"; then
        echo "GSLIB whole-corridor raw gate: FAIL" >&2
        cmp -l build/matching/gslib_hw/listing.bin "$LINKED_TEXT" | head -40 >&2 || true
        exit 1
    fi
    BYTES="$(wc -c < "$LINKED_TEXT" | tr -d ' ')"
    [[ "$BYTES" == "312" ]] || { echo "unexpected GSLIB .text size: $BYTES" >&2; exit 1; }
    "$EE_OBJDUMP" -h build/matching/gslib_hw/gslib_hw.o | grep -E '(^|[[:space:]])\.text|(^|[[:space:]])\.bss' || true
    echo "GSLIB whole-corridor raw gate: MATCH 312/312 bytes"
fi

echo
echo "Full log: $LOG"
