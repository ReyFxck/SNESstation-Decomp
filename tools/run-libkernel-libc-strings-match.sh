#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
EE_CC="${EE_CC:-$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc}"
[[ -x "$EE_CC" ]] || { echo "Missing EE compiler: $EE_CC" >&2; exit 2; }
BUILD="$ROOT/build/matching/libkernel_libc_strings"
mkdir -p "$BUILD"; rm -f "$BUILD"/*.o "$BUILD"/*.bin
CFLAGS=(-G0 -O2 -EL -pipe -Wall -Werror -Wa,-al -fomit-frame-pointer -fstrict-aliasing -fno-common -ffreestanding -fno-builtin -fshort-double -mlong64 -mhard-float -mno-abicalls -march=r5900 -mtune=r5900 -DPS2_EE -D_EE -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3 -Iinclude)
"$EE_CC" "${CFLAGS[@]}" -c matching/candidates/libkernel_libc_strings.S -o "$BUILD/libkernel_libc_strings.o"
python3 tools/objdump_listing_to_binary.py --input analysis/functions/libkernel_libc_strings_0019c3d4.asm --output "$BUILD/listing.bin" --base-address 0x0019c3d4 --end-address 0x0019c688
python3 tools/compare_elf_functions.py --target "$BUILD/listing.bin" --base-address 0x0019c3d4 --object "$BUILD/libkernel_libc_strings.o" --manifest analysis/matching/libkernel_libc_strings.csv --report analysis/matching/libkernel-libc-strings-listing-report.md --require-all-matching
python3 tools/summarize_matching_report.py analysis/matching/libkernel-libc-strings-listing-report.md
grep -F 'Result: **7/7 relocation-normalized matches**' analysis/matching/libkernel-libc-strings-listing-report.md >/dev/null
echo "libkernel historical libc-string committed-listing strict gate: OK (7/7)"
if [[ -f original/SNES_EMU.ELF ]]; then
  echo "=== formal original-ELF gate ==="
  make reference >/dev/null
  python3 tools/compare_elf_functions.py --target build/SNES_EMU.unpacked.bin --base-address 0x00100000 --object "$BUILD/libkernel_libc_strings.o" --manifest analysis/matching/libkernel_libc_strings.csv --report "$BUILD/formal-original-elf-report.md" --require-all-matching
  echo "libkernel historical libc-string formal original-ELF gate: OK (7/7)"
fi
