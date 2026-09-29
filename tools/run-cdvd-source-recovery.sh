#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

EE_CC="${EE_CC:-$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc}"
[[ -x "$EE_CC" ]] || {
  echo "missing EE compiler: $EE_CC" >&2
  exit 2
}

BUILD="$ROOT/build/matching/cdvd-source-recovery"
TARGET="$BUILD/cdvd_rpc_target.bin"
OBJECT="$BUILD/cdvd_rpc.os.o"
SOURCE="$ROOT/matching/candidates/cdvd_rpc.c"
MANIFEST="$ROOT/analysis/matching/cdvd_rpc_listing.csv"

mkdir -p "$BUILD"

python3 tools/objdump_listing_to_binary.py \
  --input analysis/functions/cdvd_rpc_0019be70.asm \
  --output "$TARGET" \
  --base-address 0x0019be70 \
  --end-address 0x0019c364 >/dev/null

"$EE_CC" \
  -Imatching/ee_abi_compat -I- \
  -G0 -Os -EL -pipe \
  -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fshort-double -mlong64 -mhard-float -mno-abicalls \
  -march=r5900 -mtune=r5900 \
  -DPS2_EE -D_EE -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3 \
  -Iinclude/ee_stage1_compat -Iinclude -w \
  -c "$SOURCE" -o "$OBJECT"

echo "=== CDVD C source recovery: ee-gcc -Os ==="
python3 tools/score_cdvd_candidate.py \
  --target "$TARGET" \
  --base-address 0x0019be70 \
  --object "$OBJECT" \
  --manifest "$MANIFEST"

OBJDUMP="$(dirname "$EE_CC")/ee-objdump"
if [[ -x "$OBJDUMP" ]]; then
  echo
  echo "=== candidate disassembly ==="
  "$OBJDUMP" -dr "$OBJECT"
fi

if [[ "${REQUIRE_ALL:-0}" == "1" ]]; then
  python3 tools/compare_elf_functions.py \
    --target "$TARGET" \
    --base-address 0x0019be70 \
    --object "$OBJECT" \
    --manifest "$MANIFEST" \
    --report "$BUILD/report.md" \
    --require-all-matching
fi
