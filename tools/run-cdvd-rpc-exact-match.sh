#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

EE_CC="${EE_CC:-$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc}"
BUILD="$ROOT/build/matching/cdvd_rpc_exact"
SOURCE="$ROOT/matching/candidates/cdvd_rpc_exact.S"
OBJECT="$BUILD/cdvd_rpc_exact.o"
LISTING="$ROOT/analysis/functions/cdvd_rpc_0019be70.asm"
LISTING_BIN="$BUILD/listing.bin"
MANIFEST="$ROOT/analysis/matching/cdvd_rpc_exact.csv"
REPORT="$ROOT/analysis/matching/cdvd-rpc-exact-listing-report.md"
REFERENCE="$ROOT/build/SNES_EMU.unpacked.bin"
FORMAL_REPORT="$ROOT/analysis/matching/cdvd-rpc-exact-formal-report.md"
EXPECTED_REFERENCE_SHA="739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

[[ -x "$EE_CC" ]] || {
  echo "Missing historical EE compiler/assembler: $EE_CC" >&2
  echo "Build it with: make bootstrap-ee-stage1" >&2
  exit 2
}

mkdir -p "$BUILD"

python3 tools/objdump_listing_to_binary.py \
  --input "$LISTING" \
  --output "$LISTING_BIN" \
  --base-address 0x0019be70 \
  --end-address 0x0019c364

"$EE_CC" \
  -G0 -EL -mno-abicalls -march=r5900 -mtune=r5900 \
  -c "$SOURCE" \
  -o "$OBJECT"

python3 tools/compare_elf_functions.py \
  --target "$LISTING_BIN" \
  --base-address 0x0019be70 \
  --object "$OBJECT" \
  --manifest "$MANIFEST" \
  --report "$REPORT" \
  --require-all-matching

python3 tools/summarize_matching_report.py "$REPORT"

# Stronger original-target gate when the verified unpacked ELF is available.
if [[ -f "$REFERENCE" ]]; then
  actual="$(sha256sum "$REFERENCE" | awk '{print $1}')"
  if [[ "$actual" != "$EXPECTED_REFERENCE_SHA" ]]; then
    echo "Original target hash mismatch: $actual" >&2
    exit 2
  fi

  python3 tools/compare_elf_functions.py \
    --target "$REFERENCE" \
    --base-address 0x00100000 \
    --object "$OBJECT" \
    --manifest "$MANIFEST" \
    --report "$FORMAL_REPORT" \
    --require-all-matching

  echo "formal original-target gate: OK (2/2)"
else
  echo "formal original-target gate: skipped (build/SNES_EMU.unpacked.bin not present)"
fi

echo "CDVD exact strict gate: OK (2/2)"
