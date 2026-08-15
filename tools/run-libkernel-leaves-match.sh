#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

EE_CC="${EE_CC:-$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc}"
if [[ ! -x "$EE_CC" ]]; then
    echo "Missing EE compiler: $EE_CC" >&2
    exit 2
fi

BUILD="$ROOT/build/matching/libkernel_leaves"
mkdir -p "$BUILD"
rm -f "$BUILD"/*.o "$BUILD"/*.bin

CFLAGS=(
  -G0 -O2 -EL -pipe -Wall -Werror -Wa,-al
  -fomit-frame-pointer -fstrict-aliasing -fno-common
  -ffreestanding -fno-builtin -fshort-double
  -mlong64 -mhard-float -mno-abicalls
  -march=r5900 -mtune=r5900
  -DPS2_EE -D_EE -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3
  -Iinclude
)

"$EE_CC" "${CFLAGS[@]}" -c matching/candidates/libkernel_syscall_wrappers.S -o "$BUILD/syscalls.o"
"$EE_CC" "${CFLAGS[@]}" -c matching/candidates/libkernel_intr.S -o "$BUILD/intr.o"
"$EE_CC" -nostdlib -Wl,-r -o "$BUILD/libkernel_leaves.o" "$BUILD/syscalls.o" "$BUILD/intr.o"

python3 tools/objdump_listing_to_binary.py \
  --input analysis/functions/libkernel_syscalls_0019ce60.asm \
  --output "$BUILD/syscalls-core.bin" \
  --base-address 0x0019ce60 \
  --end-address 0x0019cf10

python3 tools/objdump_listing_to_binary.py \
  --input analysis/functions/libkernel_intr_0019f018.asm \
  --output "$BUILD/intr.bin" \
  --base-address 0x0019f018 \
  --end-address 0x0019f078

python3 tools/objdump_listing_to_binary.py \
  --input analysis/functions/libkernel_syscalls_0019f5d0.asm \
  --output "$BUILD/syscalls-late.bin" \
  --base-address 0x0019f5d0 \
  --end-address 0x0019f600

python3 tools/objdump_listing_to_binary.py \
  --input analysis/functions/libkernel_syscalls_0019fcd0.asm \
  --output "$BUILD/syscalls-irq-tail.bin" \
  --base-address 0x0019fcd0 \
  --end-address 0x0019fd20

python3 tools/compare_elf_functions.py \
  --target "$BUILD/syscalls-core.bin" \
  --base-address 0x0019ce60 \
  --object "$BUILD/libkernel_leaves.o" \
  --manifest analysis/matching/libkernel_syscalls_core.csv \
  --report analysis/matching/libkernel-syscalls-core-listing-report.md \
  --require-all-matching

python3 tools/compare_elf_functions.py \
  --target "$BUILD/intr.bin" \
  --base-address 0x0019f018 \
  --object "$BUILD/libkernel_leaves.o" \
  --manifest analysis/matching/libkernel_intr.csv \
  --report analysis/matching/libkernel-intr-listing-report.md \
  --require-all-matching

python3 tools/compare_elf_functions.py \
  --target "$BUILD/syscalls-late.bin" \
  --base-address 0x0019f5d0 \
  --object "$BUILD/libkernel_leaves.o" \
  --manifest analysis/matching/libkernel_syscalls_late.csv \
  --report analysis/matching/libkernel-syscalls-late-listing-report.md \
  --require-all-matching

python3 tools/compare_elf_functions.py \
  --target "$BUILD/syscalls-irq-tail.bin" \
  --base-address 0x0019fcd0 \
  --object "$BUILD/libkernel_leaves.o" \
  --manifest analysis/matching/libkernel_syscalls_irq_tail.csv \
  --report analysis/matching/libkernel-syscalls-irq-tail-listing-report.md \
  --require-all-matching

python3 tools/summarize_matching_report.py analysis/matching/libkernel-syscalls-core-listing-report.md
python3 tools/summarize_matching_report.py analysis/matching/libkernel-intr-listing-report.md
python3 tools/summarize_matching_report.py analysis/matching/libkernel-syscalls-late-listing-report.md
python3 tools/summarize_matching_report.py analysis/matching/libkernel-syscalls-irq-tail-listing-report.md

grep -F 'Result: **11/11 relocation-normalized matches**' analysis/matching/libkernel-syscalls-core-listing-report.md >/dev/null
grep -F 'Result: **2/2 relocation-normalized matches**' analysis/matching/libkernel-intr-listing-report.md >/dev/null
grep -F 'Result: **3/3 relocation-normalized matches**' analysis/matching/libkernel-syscalls-late-listing-report.md >/dev/null
grep -F 'Result: **5/5 relocation-normalized matches**' analysis/matching/libkernel-syscalls-irq-tail-listing-report.md >/dev/null

echo "libkernel leaves committed-listing strict gate: OK (21/21)"

# Optional formal gate against the user's legally obtained reference ELF.
if [[ -f original/SNES_EMU.ELF ]]; then
    echo "=== formal original-ELF gate ==="
    make reference >/dev/null
    python3 tools/compare_elf_functions.py \
      --target build/SNES_EMU.unpacked.bin \
      --base-address 0x00100000 \
      --object "$BUILD/libkernel_leaves.o" \
      --manifest analysis/matching/libkernel_leaves.csv \
      --report "$BUILD/formal-original-elf-report.md" \
      --require-all-matching
    echo "libkernel leaves formal original-ELF gate: OK (21/21)"
fi
