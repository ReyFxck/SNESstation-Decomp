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


echo
echo "=== strict relocation-normalized 8/8 gate ==="
python3 tools/compare_elf_functions.py \
  --target "$TARGET" \
  --base-address 0x0019be70 \
  --object "$OBJECT" \
  --manifest "$MANIFEST" \
  --report "$BUILD/report.md" \
  --require-all-matching

LD="$(dirname "$EE_CC")/ee-ld"
OBJCOPY="$(dirname "$EE_CC")/ee-objcopy"
LINKER_SCRIPT="$BUILD/cdvd_rpc_target.ld"
LINKED_ELF="$BUILD/cdvd_rpc.target-linked.elf"
LINKED_TEXT="$BUILD/cdvd_rpc.target-linked.text.bin"

cat >"$LINKER_SCRIPT" <<'LDS'
ENTRY(CDVD_Init)

PROVIDE(memcpy = 0x0019c364);
PROVIDE(strncpy = 0x0019c550);
PROVIDE(SifBindRpc = 0x0019c688);
PROVIDE(SifCallRpc = 0x0019c7b0);
PROVIDE(SifWriteBackDCache = 0x0019cf10);

SECTIONS
{
  . = 0x0019be70;
  .text : { *(.text) }

  . = 0x00425a38;
  .data : { *(.data) }

  . = 0x0043ed00;
  .bss (NOLOAD) : { *(.bss) *(COMMON) }

  /DISCARD/ : {
    *(.comment)
    *(.mdebug*)
    *(.pdr)
  }
}
LDS

"$LD" -T "$LINKER_SCRIPT" "$OBJECT" -o "$LINKED_ELF"
"$OBJCOPY" -j .text -O binary "$LINKED_ELF" "$LINKED_TEXT"

echo
echo "=== raw linked .text byte-for-byte gate ==="
if ! cmp -s "$TARGET" "$LINKED_TEXT"; then
  echo "raw linked .text differs from target" >&2
  cmp -l "$TARGET" "$LINKED_TEXT" | head -40 >&2 || true
  exit 1
fi
echo "RAW MATCH: $(wc -c < "$LINKED_TEXT")/$(wc -c < "$TARGET") bytes"
echo "target sha256: $(sha256sum "$TARGET" | awk '{print $1}')"
echo "linked sha256: $(sha256sum "$LINKED_TEXT" | awk '{print $1}')"

OBJDUMP="$(dirname "$EE_CC")/ee-objdump"
if [[ -x "$OBJDUMP" ]]; then
  echo
  echo "=== candidate disassembly ==="
  "$OBJDUMP" -dr "$OBJECT"
fi

