#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

DEFAULT_EE_CC="$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
EE_CC="${EE_CC:-$DEFAULT_EE_CC}"

if [[ ! -x "$EE_CC" ]]; then
    echo "Missing historical EE compiler: $EE_CC" >&2
    exit 2
fi

BUILD="$ROOT/build/matching/cdvd_rpc"
SOURCE="$ROOT/matching/candidates/cdvd_rpc.c"
MANIFEST="$ROOT/analysis/matching/cdvd_rpc_listing.csv"
RAW="$BUILD/listing.bin"

mkdir -p "$BUILD"
rm -f "$BUILD"/shape-*.o "$BUILD"/shape-*.compile.log
rm -f "$BUILD"/shape-scores.tsv "$BUILD"/best-profile.txt
rm -f "$BUILD"/best-objdump.txt "$RAW"

python3 tools/objdump_listing_to_binary.py \
  --input analysis/functions/cdvd_rpc_0019be70.asm \
  --output "$RAW" \
  --base-address 0x0019be70 \
  --end-address 0x0019c364 >/dev/null

COMMON=(
  -G0 -EL -pipe -Wall
  -fshort-double -mlong64 -mhard-float -mno-abicalls
  -march=r5900 -mtune=r5900
  -DPS2_EE -D_EE -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3
  -Imatching/ee_abi_compat
)

try_profile() {
    local name="$1"
    shift
    local obj="$BUILD/shape-${name}.o"
    local log="$BUILD/shape-${name}.compile.log"

    if ! "$EE_CC" "$@" "${COMMON[@]}" -c "$SOURCE" -o "$obj" >"$log" 2>&1; then
        printf '%s\tFAIL\t0\t999999\t999999\n' "$name" >>"$BUILD/shape-scores.tsv"
        return 0
    fi

    python3 tools/score_cdvd_candidate.py \
      --target "$RAW" \
      --base-address 0x0019be70 \
      --object "$obj" \
      --manifest "$MANIFEST" \
      --label "$name" \
      --tsv >>"$BUILD/shape-scores.tsv"
}

# Faithful historical baselines.
try_profile o2-strict \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common

try_profile o2-default-alias \
  -O2 -fomit-frame-pointer -fno-common

try_profile o2-no-strict \
  -O2 -fomit-frame-pointer -fno-strict-aliasing -fno-common

# Scheduler fingerprints.
try_profile o2-no-sched1 \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-schedule-insns

try_profile o2-no-sched2 \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-schedule-insns2

try_profile o2-no-sched12 \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-schedule-insns -fno-schedule-insns2

try_profile o2-no-delayed \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-delayed-branch

try_profile o2-no-sched1-no-delayed \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-schedule-insns -fno-delayed-branch

# Individual O2 passes that can move prologues/branches/register lifetimes.
try_profile o2-no-gcse \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-gcse

try_profile o2-no-crossjump \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-crossjumping

try_profile o2-no-cse-follow \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-cse-follow-jumps

try_profile o2-no-expensive \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-expensive-optimizations

try_profile o2-no-peephole2 \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-peephole2

try_profile o2-no-reorder-blocks \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-reorder-blocks

try_profile o2-no-ifconv \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-if-conversion

try_profile o2-no-ifconv2 \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -fno-if-conversion2

# Optimization-level controls.
try_profile o1 \
  -O1 -fomit-frame-pointer -fstrict-aliasing -fno-common

try_profile o3 \
  -O3 -fomit-frame-pointer -fstrict-aliasing -fno-common

# Surviving SNESticle Makefile profile, kept as a negative/positive control.
try_profile snesticle-freestanding \
  -O2 -fomit-frame-pointer -fstrict-aliasing -fno-common \
  -ffreestanding -fno-builtin

BEST="$(python3 - "$BUILD/shape-scores.tsv" <<'PY'
import sys
from pathlib import Path

rows = []
for line in Path(sys.argv[1]).read_text(encoding="utf-8").splitlines():
    p = line.split("\t")
    if len(p) != 5 or p[1] == "FAIL":
        continue
    name = p[0]
    matches = int(p[1])
    same_size = int(p[2])
    size_delta = int(p[3])
    diff_bytes = int(p[4])
    # Exact matches dominate; then actual relocation-normalized differing
    # bytes; then shape/size proximity.
    rows.append((matches, -diff_bytes, same_size, -size_delta, name))

if not rows:
    raise SystemExit("no profile compiled")
rows.sort(reverse=True)
print(rows[0][4])
PY
)"

printf '%s\n' "$BEST" >"$BUILD/best-profile.txt"

echo "=== CDVD source/header + codegen matrix ==="
printf '%-28s %7s %10s %10s %11s\n' \
  "profile" "match" "same-size" "size-delta" "diff-bytes"

python3 - "$BUILD/shape-scores.tsv" "$BEST" <<'PY'
import sys
from pathlib import Path

path = Path(sys.argv[1])
best = sys.argv[2]
rows = []
for line in path.read_text(encoding="utf-8").splitlines():
    p = line.split("\t")
    if len(p) != 5:
        continue
    if p[1] == "FAIL":
        rows.append((10**9, p[0], p))
        continue
    rows.append((int(p[4]), p[0], p))

rows.sort(key=lambda x: x[0])
for _, name, p in rows:
    marker = "*" if name == best else " "
    if p[1] == "FAIL":
        print(f"{marker} {name:<26} COMPILE_FAIL")
    else:
        print(
            f"{marker} {name:<26} {p[1]:>4}/8 "
            f"{p[2]:>8}/8 {p[3]:>10} {p[4]:>11}"
        )
PY

echo
echo "=== best profile: $BEST ==="
python3 tools/score_cdvd_candidate.py \
  --target "$RAW" \
  --base-address 0x0019be70 \
  --object "$BUILD/shape-${BEST}.o" \
  --manifest "$MANIFEST"

OBJDUMP="$(dirname "$EE_CC")/ee-objdump"
if [[ -x "$OBJDUMP" ]]; then
    "$OBJDUMP" -dr "$BUILD/shape-${BEST}.o" >"$BUILD/best-objdump.txt" 2>/dev/null || true
fi

# Keep the canonical comparator report for the best candidate.
python3 tools/compare_elf_functions.py \
  --target "$RAW" \
  --base-address 0x0019be70 \
  --object "$BUILD/shape-${BEST}.o" \
  --manifest "$MANIFEST" \
  --report analysis/matching/cdvd-rpc-listing-report.md >/dev/null

if python3 tools/score_cdvd_candidate.py \
  --target "$RAW" \
  --base-address 0x0019be70 \
  --object "$BUILD/shape-${BEST}.o" \
  --manifest "$MANIFEST" \
  --label best --tsv | grep -q $'^best\t8\t'; then
    echo
    echo "=== strict gate ==="
    python3 tools/compare_elf_functions.py \
      --target "$RAW" \
      --base-address 0x0019be70 \
      --object "$BUILD/shape-${BEST}.o" \
      --manifest "$MANIFEST" \
      --report analysis/matching/cdvd-rpc-listing-report.md \
      --require-all-matching
    echo "CDVD RPC strict listing gate: OK"
fi

echo
echo "Best profile:    build/matching/cdvd_rpc/best-profile.txt"
echo "Best disassembly: build/matching/cdvd_rpc/best-objdump.txt"
