#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
IN="$ROOT/original/SNES_EMU.ELF"
RAW="$ROOT/build/SNES_EMU.unpacked.bin"
WRAP="$ROOT/build/SNES_EMU.analysis.elf"

if [[ ! -f "$IN" ]]; then
  echo "Missing $IN" >&2
  exit 1
fi

sha256sum "$IN" | tee "$ROOT/notes/SHA256SUMS.txt"
file "$IN" | tee "$ROOT/notes/file.txt"
readelf -h "$IN" > "$ROOT/notes/elf_header.txt"
readelf -S "$IN" > "$ROOT/notes/elf_sections.txt"
readelf -l "$IN" > "$ROOT/notes/elf_segments.txt"

ENTRY=$(python3 "$ROOT/tools/sjuncrunch.py" "$IN" "$RAW" | tee "$ROOT/notes/unpack.txt" | awk -F= '/^entry=/{print $2}')
python3 "$ROOT/tools/wrap_raw_elf.py" "$RAW" "$WRAP" --entry "$ENTRY"
python3 "$ROOT/tools/scan_calls.py" "$RAW" "$ROOT/notes/jal_candidates.csv"
strings -a -t x "$RAW" > "$ROOT/notes/strings.txt"

OBJDUMP=""
for CAND in mips64r5900el-ps2-elf-objdump llvm-objdump /usr/local/swift/usr/bin/llvm-objdump; do
  if command -v "$CAND" >/dev/null 2>&1; then OBJDUMP=$(command -v "$CAND"); break; fi
done
if [[ -z "$OBJDUMP" ]]; then
  echo "No suitable objdump found; skipping disassembly" >&2
else
  "$OBJDUMP" -d "$WRAP" > "$ROOT/asm/full.asm" || true
  echo "disassembler=$OBJDUMP" | tee "$ROOT/notes/disassembler.txt"
fi

echo "Done. Entry: $ENTRY"
