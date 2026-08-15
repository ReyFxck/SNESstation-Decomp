#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

EE_CC="${EE_CC:-$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc}"
BUILD="$ROOT/build/matching/cpp_runtime_small"
SOURCE="$ROOT/matching/candidates/cpp_runtime_small.S"
OBJECT="$BUILD/cpp_runtime_small.o"

EH_LISTING="$ROOT/analysis/functions/cpp_eh_personality_001a90f8.asm"
EH_BIN="$BUILD/eh-listing.bin"
EH_MANIFEST="$ROOT/analysis/matching/cpp_eh_runtime_small.csv"
EH_REPORT="$ROOT/analysis/matching/cpp-eh-runtime-small-listing-report.md"

RTTI_LISTING="$ROOT/analysis/functions/libsupcxx_rtti_001a9fa8.asm"
RTTI_BIN="$BUILD/rtti-listing.bin"
RTTI_MANIFEST="$ROOT/analysis/matching/libsupcxx_rtti_small.csv"
RTTI_REPORT="$ROOT/analysis/matching/libsupcxx-rtti-small-listing-report.md"

REFERENCE="$ROOT/build/SNES_EMU.unpacked.bin"
EXPECTED_REFERENCE_SHA="739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

[[ -x "$EE_CC" ]] || {
  echo "Missing historical EE compiler: $EE_CC" >&2
  exit 2
}

mkdir -p "$BUILD"

python3 tools/objdump_listing_to_binary.py \
  --input "$EH_LISTING" \
  --output "$EH_BIN" \
  --base-address 0x001a90f8 \
  --end-address 0x001a9d58

python3 tools/objdump_listing_to_binary.py \
  --input "$RTTI_LISTING" \
  --output "$RTTI_BIN" \
  --base-address 0x001a9fa8 \
  --end-address 0x001ab3c0

"$EE_CC" \
  -G0 -EL -mno-abicalls -march=r5900 -mtune=r5900 \
  -c "$SOURCE" \
  -o "$OBJECT"

python3 tools/compare_elf_functions.py \
  --target "$EH_BIN" \
  --base-address 0x001a90f8 \
  --object "$OBJECT" \
  --manifest "$EH_MANIFEST" \
  --report "$EH_REPORT" \
  --require-all-matching

python3 tools/compare_elf_functions.py \
  --target "$RTTI_BIN" \
  --base-address 0x001a9fa8 \
  --object "$OBJECT" \
  --manifest "$RTTI_MANIFEST" \
  --report "$RTTI_REPORT" \
  --require-all-matching

python3 tools/summarize_matching_report.py "$EH_REPORT"
python3 tools/summarize_matching_report.py "$RTTI_REPORT"

# Optional stronger original-target gate. The repository's established
# checkpoint policy allows committed-listing strict matches; this formal
# comparison is run automatically when the verified unpacked target exists.
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
    --manifest "$EH_MANIFEST" \
    --report "$ROOT/analysis/matching/cpp-eh-runtime-small-formal-report.md" \
    --require-all-matching

  python3 tools/compare_elf_functions.py \
    --target "$REFERENCE" \
    --base-address 0x00100000 \
    --object "$OBJECT" \
    --manifest "$RTTI_MANIFEST" \
    --report "$ROOT/analysis/matching/libsupcxx-rtti-small-formal-report.md" \
    --require-all-matching

  echo "formal original-target gate: OK (48/48)"
else
  echo "formal original-target gate: skipped (build/SNES_EMU.unpacked.bin not present)"
fi

echo "cpp runtime strict gate: OK (48/48)"
