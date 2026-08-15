#!/usr/bin/env bash
set -euo pipefail

# SNESstation-Decomp historical asset pinning + GitHub sync.
# Designed for the user's DroidSpaces Debian checkout.
#
# What this DOES:
#   - verifies the already-vendored BETA 3 source recipe
#   - archives ee-gcc 3.2 Beta2 (2003-02-10) and 3.2-030926
#   - vendors Newlib 1.10.0 + the pinned PS2DEV patch
#   - vendors immutable historical libcdvd references from SNESticle and PGEN
#   - writes DECOMP_PLAYBOOK.md and DECOMP_STATE.md
#   - records SHA-256/size provenance
#   - commits only these paths and pushes main
#
# What this DOES NOT do:
#   - distribute the original SNES_EMU.ELF or embedded IRX blobs
#   - claim the exact libcdvd/PS2LIB revision is proven
#   - claim 030926 is proven until byte comparisons say so

REPO="${1:-$HOME/SNESstation-Decomp}"
cd "$REPO"

die() {
    echo "ERROR: $*" >&2
    exit 1
}

[[ -d .git ]] || die "Not a git repository: $REPO"
[[ -f Makefile ]] || die "Makefile not found; wrong repository root?"

BRANCH="$(git branch --show-current)"
[[ "$BRANCH" == "main" ]] || die "Expected branch main, found: $BRANCH"

# Do not silently mix a pre-existing staged commit with this provenance commit.
if ! git diff --cached --quiet; then
    echo "There are already staged changes. Commit/unstage them first so this script"
    echo "does not mix unrelated work into the historical-assets commit."
    git status --short
    exit 2
fi

echo "=== Sync main safely ==="
git pull --rebase --autostash origin main

need_cmd() {
    command -v "$1" >/dev/null 2>&1 || die "Missing command: $1"
}
need_cmd git
need_cmd python3
need_cmd sha256sum
need_cmd stat

if command -v curl >/dev/null 2>&1; then
    DOWNLOAD_TOOL="curl"
elif command -v wget >/dev/null 2>&1; then
    DOWNLOAD_TOOL="wget"
else
    die "Install curl or wget first."
fi

download() {
    local url="$1"
    local dest="$2"
    mkdir -p "$(dirname "$dest")"

    if [[ -s "$dest" ]]; then
        echo "[keep] $dest"
        return 0
    fi

    local tmp="${dest}.part"
    rm -f "$tmp"
    echo "[get ] $dest"

    if [[ "$DOWNLOAD_TOOL" == "curl" ]]; then
        curl -L --fail --retry 4 --retry-delay 2 \
            --connect-timeout 20 \
            -o "$tmp" "$url"
    else
        wget --tries=4 --timeout=30 -O "$tmp" "$url"
    fi

    mv "$tmp" "$dest"
}

verify_sha256() {
    local expected="$1"
    local file="$2"
    [[ -f "$file" ]] || die "Missing file for SHA-256 check: $file"
    local got
    got="$(sha256sum "$file" | awk '{print $1}')"
    [[ "$got" == "$expected" ]] || {
        echo "SHA-256 mismatch: $file" >&2
        echo "expected: $expected" >&2
        echo "got:      $got" >&2
        exit 3
    }
    echo "[sha ] OK  $file"
}

verify_size() {
    local expected="$1"
    local file="$2"
    [[ -f "$file" ]] || die "Missing file for size check: $file"
    local got
    got="$(stat -c %s "$file")"
    [[ "$got" == "$expected" ]] || {
        echo "Size mismatch: $file" >&2
        echo "expected: $expected" >&2
        echo "got:      $got" >&2
        exit 4
    }
    echo "[size] OK  $file ($got bytes)"
}

TOOLCHAIN="third_party/toolchain"
ARCHIVE="$TOOLCHAIN/archive"
NEWLIB="$TOOLCHAIN/newlib-1.10.0"
REFS="third_party/historical_refs"
SNESTICLE_REF="$REFS/snesticle-9590ebf3"
PGEN_REF="$REFS/pgen-f7226813"

mkdir -p "$ARCHIVE" "$NEWLIB" "$SNESTICLE_REF" "$PGEN_REF" docs

echo
echo "=== Verify already-pinned public BETA 3 source recipe ==="

# GNU release archives already present in this repository.
verify_sha256 \
  "ba91202a1aefca79f5eeb534e6c4235c874b220a2975725296712e42e6b91df1" \
  "$TOOLCHAIN/binutils-2.14.tar.gz"

verify_sha256 \
  "a0a626b10be8f793349a5309dd054a224c00772c83207d0354499c17e8deb187" \
  "$TOOLCHAIN/gcc-3.2.2.tar.gz"

# PS2DEV patches from immutable root recipe 16a47184...
verify_sha256 \
  "f63a6d656d51e9ed74bd26d7cde4fb237d6552569980f00c979ef73f52ff3cda" \
  "$TOOLCHAIN/binutils-2.14.patch"

verify_sha256 \
  "803395ac6345d71ebdcdf6bd4a8981863e64b0cfb36cfb48c607c59d433c5b9a" \
  "$TOOLCHAIN/gcc-3.2.2.patch"

echo
echo "=== Archive historical EE GCC candidates ==="

# These are immutable GitHub release assets preserved by decomp.me.
# Their release metadata does not publish SHA-256 for these two old assets,
# so we pin exact asset URL + exact byte size and record our local SHA-256
# into HISTORICAL_ASSETS.sha256 after download.

BETA2_URL="https://github.com/decompme/compilers/releases/download/compilers/ee-gcc3.2-030210-beta2.tar.gz"
GCC030926_URL="https://github.com/decompme/compilers/releases/download/compilers/ee-gcc3.2-030926.tar.gz"

download "$BETA2_URL" "$ARCHIVE/ee-gcc3.2-030210-beta2.tar.gz"
verify_size 7870231 "$ARCHIVE/ee-gcc3.2-030210-beta2.tar.gz"

download "$GCC030926_URL" "$ARCHIVE/ee-gcc3.2-030926.tar.gz"
verify_size 11747681 "$ARCHIVE/ee-gcc3.2-030926.tar.gz"

echo
echo "=== Pin Newlib 1.10.0 + PS2DEV historical patch ==="

NEWLIB_URL="https://sourceware.org/pub/newlib/newlib-1.10.0.tar.gz"
NEWLIB_PATCH_URL="https://raw.githubusercontent.com/ps2dev/ps2toolchain/16a47184b3a5fdf4aea45fcc8fee082d3c4d4183e/newlib-1.10.0.patch"

download "$NEWLIB_URL" "$NEWLIB/newlib-1.10.0.tar.gz"
verify_sha256 \
  "69b62ad4c746a9acaf4f898772549f6da49f228f83a95efce7e88ae1d88c5a84" \
  "$NEWLIB/newlib-1.10.0.tar.gz"

download "$NEWLIB_PATCH_URL" "$NEWLIB/newlib-1.10.0-ps2dev-16a47184.patch"
verify_sha256 \
  "618634ff422e17aa517445308a2acbf8bfba95f8fee8b0bd5cbc123a0d69db38" \
  "$NEWLIB/newlib-1.10.0-ps2dev-16a47184.patch"

echo
echo "=== Vendor immutable libcdvd historical references ==="

SNESTICLE_COMMIT="9590ebf3bf768424ebd6cb018f322e724a7aade3"
SNESTICLE_RAW="https://raw.githubusercontent.com/iaddis/SNESticle/$SNESTICLE_COMMIT"

download \
  "$SNESTICLE_RAW/SNESticle/Modules/libcdvd/ee/cdvd_rpc.c" \
  "$SNESTICLE_REF/cdvd_rpc.c"
download \
  "$SNESTICLE_RAW/SNESticle/Modules/libcdvd/ee/cdvd_rpc.h" \
  "$SNESTICLE_REF/cdvd_rpc.h"
download \
  "$SNESTICLE_RAW/SNESticle/Project/ps2/release_EE3.2.2-b1/cdvd_rpc.lst" \
  "$SNESTICLE_REF/cdvd_rpc.release.lst"
download \
  "$SNESTICLE_RAW/SNESticle/Project/ps2/release_EE3.2.2-b1/cdvd_rpc.o" \
  "$SNESTICLE_REF/cdvd_rpc.release.o"
download \
  "$SNESTICLE_RAW/SNESticle/Project/ps2/Makefile" \
  "$SNESTICLE_REF/ps2.Makefile"

PGEN_COMMIT="f722681391fb6a1cc64a1260027a33862685e585"
PGEN_RAW="https://raw.githubusercontent.com/ps2homebrew/pgen/$PGEN_COMMIT"

download "$PGEN_RAW/ps2/lib/cdvd_rpc.c" "$PGEN_REF/cdvd_rpc.c"
download "$PGEN_RAW/ps2/lib/cdvd_rpc.h" "$PGEN_REF/cdvd_rpc.h"
download "$PGEN_RAW/ps2/lib/cdvd.h" "$PGEN_REF/cdvd.h"

cat > "$SNESTICLE_REF/PROVENANCE.txt" <<EOF
repository=https://github.com/iaddis/SNESticle
commit=$SNESTICLE_COMMIT
role=historical reference for the old EE libcdvd family and EE3.2.2-b1 listing/object
status=REFERENCE, not proof that SNES Station used this exact libcdvd revision
EOF

cat > "$PGEN_REF/PROVENANCE.txt" <<EOF
repository=https://github.com/ps2homebrew/pgen
commit=$PGEN_COMMIT
role=historical libcdvd revision preserving CDVD_GetSize / command 0x08
status=REFERENCE, not proof that SNES Station used this exact PGEN source revision
EOF

echo
echo "=== Write permanent decompilation playbook ==="

cat > docs/DECOMP_PLAYBOOK.md <<'EOF'
# SNES Station decompilation playbook

This file is the persistent methodology for continuing SNESstation-Decomp in a
new ChatGPT session or on another machine.

## Non-negotiable evidence rules

1. Search historical source first. Do not begin from invented pseudocode when
   a contemporaneous library/application source snapshot can be found.
2. Freeze every historical reference by immutable commit, release asset, hash,
   or exact target fingerprint.
3. Distinguish:
   - STRUCTURAL: boundaries/symbols/control flow identified.
   - RECOVERED: readable behavior/source reconstructed.
   - MATCHING: compiled candidate matches the target listing after relocation
     normalization.
   - STRICT: every function in the selected listing gate matches.
   - FORMAL ELF: the legally obtained original ELF itself is the comparison
     target after final linking/layout.
4. Same function size is not a MATCH.
5. Compiler acceptance is not a MATCH.
6. A historical-looking source file is not a MATCH.
7. Never promote WIP to MATCHING until the byte comparator says so.

## Matching workflow

1. Identify the exact target corridor and function boundaries.
2. Find the closest historical source and record provenance.
3. Identify the compiler family and, where possible, its exact build date.
4. Compile isolated candidates with the historically plausible compiler.
5. Force a fresh object before every experiment; stale `.o` files invalidate
   compiler-fingerprint conclusions.
6. Compare target vs candidate with `tools/compare_elf_functions.py`.
7. Mask only relocation-controlled bytes. All other bytes must agree.
8. When every row matches, run the strict gate.
9. Commit the source, manifest, report, provenance and compiler contract.
10. Only then move to the next corridor.

## Compiler fingerprint rule

Do not mutilate recovered C merely to compensate for the wrong compiler.
When source structure and function sizes line up but register allocation,
prologue order or scheduling differs, test historical compiler builds first.

For the January 24, 2004 SNES Station target, maintain this matrix:

- EE GCC 3.2 Beta2 / 2003-02-10: historical control candidate.
- EE GCC 3.2 / 2003-09-26: strongest pre-target candidate currently preserved.
- GCC 3.2.2 BETA 3 / 2004-02-14: reproducible public PS2DEV source recipe,
  useful and already proven for several gates, but newer than the target.

The 2003-09-26 build is a candidate, not a proven global answer. Bytes decide.

## User/assistant workflow

Prefer doing compiler/source permutations in the analysis environment. Do not
turn the user into a manual CI runner for a long sequence of speculative
variants. Ask for a device-side run only when the required compiler or target
artifact genuinely cannot be executed/accessed elsewhere.

Deliver consolidated checkpoints:
- MATCHED/closed work;
- WIP separately;
- exact commands for commit/push when needed.

## Legal/source hygiene

Do not commit or redistribute the original SNES_EMU.ELF or carved proprietary
IRX blobs. Keep hashes, sizes and offsets in the repo so a legally obtained
local copy can be verified.

Open-source historical compiler/library source and explicitly redistributable
archives may be pinned for reproducibility subject to their licenses.
EOF

cat > docs/DECOMP_STATE.md <<'EOF'
# SNESstation-Decomp state

Last methodology checkpoint: 2026-08-14.

## Target

SNES Station v0.23 WIP, January 24, 2004.

Packed ELF SHA-256:
`4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487`

Unpacked image SHA-256:
`739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`

## Closed local listing gates

- Newlib mathfp corridor: 7/7 relocation-normalized matches.
- libgcc unwind compact gate: 7/7 relocation-normalized matches.
- GSLIB hardware corridor: 7/7 relocation-normalized matches; strict gate OK.

These are function/listing claims, not a claim that the complete ELF links
byte-identically yet.

## Current WIP: EE libcdvd RPC

Historical source family recovered for eight functions:

- CDVD_Init
- CDVD_DiskReady
- CDVD_FindFile
- CDVD_Stop
- CDVD_TrayReq
- CDVD_getdir
- CDVD_FlushCache
- CDVD_GetSize

The historical source/layout is structurally strong, but the corridor is not
yet an 8/8 byte match. Do not mark it MATCHING.

Important references:
- SNESticle commit 9590ebf3bf768424ebd6cb018f322e724a7aade3:
  old libcdvd source plus EE3.2.2-b1 listing/object.
- PGEN commit f722681391fb6a1cc64a1260027a33862685e585:
  same historical libcdvd family with CDVD_GetSize / command 0x08.

## Compiler state

Already reproducible from repository source:
- binutils 2.14
- GCC 3.2.2 PS2/R5900 BETA 3 patch snapshot dated 2004-02-14
- stage-one C compiler

Archived fingerprint candidates:
- ee-gcc3.2-030210-beta2.tar.gz
- ee-gcc3.2-030926.tar.gz

The 030926 compiler is the strongest pre-target fingerprint candidate for the
remaining CDVD register-allocation/scheduling differences, but it must be
validated by byte comparisons.

## C library

Pinned:
- Newlib 1.10.0 official source archive
- PS2DEV newlib-1.10.0 patch from immutable ps2toolchain commit
  16a47184b3a5fdf4aea45fcc8fee082d3c4d4183e

## Still not globally pinned

- exact old PS2LIB snapshot used by SNES Station
- exact EE libcdvd source revision
- exact binutils patch level used by the target
- full historical libgcc/libsupc++ archive set and archive ordering
- exact application linker script/archive order
- SJCRUNCH2 packer revision

Do not silently substitute modern PS2SDK for those unknowns.
EOF

cat > "$TOOLCHAIN/HISTORICAL_COMPILER_MATRIX.md" <<'EOF'
# Historical EE compiler matrix

| Candidate | Date | Repository role | Confidence |
|---|---|---|---|
| `archive/ee-gcc3.2-030210-beta2.tar.gz` | 2003-02-10 | older control/fingerprint | historical candidate |
| `archive/ee-gcc3.2-030926.tar.gz` | 2003-09-26 | strongest pre-target compiler fingerprint candidate | candidate, bytes must decide |
| GCC 3.2.2 + vendored PS2DEV patch | patch labels itself BETA 3, 2004-02-14 | reproducible source build used by current stage-one bootstrap | proven useful, globally newer than target |

Target date: January 24, 2004.

The archived decomp.me compiler bundles are preserved as historical compiler
artifacts. They may be host-platform binaries and are not assumed to execute
natively on ARM64 DroidSpaces. Their primary purpose in this repository is
provenance/fingerprint preservation; execution compatibility is a separate
question.

No compiler is considered the exact global SNES Station compiler solely from
its date. Function bytes are the gate.
EOF

cat > "$NEWLIB/PROVENANCE.txt" <<'EOF'
newlib_source=newlib-1.10.0.tar.gz
source_origin=https://sourceware.org/pub/newlib/newlib-1.10.0.tar.gz
source_sha256=69b62ad4c746a9acaf4f898772549f6da49f228f83a95efce7e88ae1d88c5a84

ps2_patch=newlib-1.10.0-ps2dev-16a47184.patch
patch_origin=https://github.com/ps2dev/ps2toolchain
patch_commit=16a47184b3a5fdf4aea45fcc8fee082d3c4d4183e
patch_sha256=618634ff422e17aa517445308a2acbf8bfba95f8fee8b0bd5cbc123a0d69db38
EOF

echo
echo "=== Record immutable local hashes ==="

MANIFEST="$TOOLCHAIN/HISTORICAL_ASSETS.sha256"
{
    echo "# Generated on $(date -u +%Y-%m-%dT%H:%M:%SZ)"
    echo "# Exact local SHA-256 values for preserved historical assets."
    find "$ARCHIVE" "$NEWLIB" "$REFS" -type f -print0 \
      | sort -z \
      | xargs -0 sha256sum
} > "$MANIFEST"

SIZE_MANIFEST="$TOOLCHAIN/HISTORICAL_ASSETS.sizes"
{
    echo "# bytes  path"
    find "$ARCHIVE" "$NEWLIB" "$REFS" -type f -print0 \
      | sort -z \
      | while IFS= read -r -d '' f; do
            printf '%s  %s\n' "$(stat -c %s "$f")" "$f"
        done
} > "$SIZE_MANIFEST"

# Mark archives/objects as binary for Git's diff machinery, without requiring LFS.
touch .gitattributes
for rule in \
    "third_party/toolchain/archive/*.tar.gz binary" \
    "third_party/toolchain/newlib-1.10.0/*.tar.gz binary" \
    "third_party/historical_refs/**/*.o binary" \
    "analysis/matching/cdvd_rpc_target.bin binary"
do
    grep -Fqx "$rule" .gitattributes || echo "$rule" >> .gitattributes
done

echo
echo "=== Review payload ==="
git status --short -- \
    .gitattributes \
    docs/DECOMP_PLAYBOOK.md \
    docs/DECOMP_STATE.md \
    third_party/toolchain \
    third_party/historical_refs

echo
echo "Disk footprint:"
du -sh "$ARCHIVE" "$NEWLIB" "$REFS" 2>/dev/null || true

echo
echo "=== Restore the complete recovered checkpoint package ==="

CHECKPOINT_TMP="$(mktemp -d)"
trap 'rm -rf "$CHECKPOINT_TMP"' EXIT

python3 - "$CHECKPOINT_TMP/recovered-pack.zip" <<'PY'
import base64
import sys
from pathlib import Path

payload = "UEsDBBQAAAAIALEGD13APc6q+AEAAFkDAABAAAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvZ3NsaWJfaHcvUFJPR1JFU1M0Mi5tZHWSza7TMBCF93mKkVjSNCVNW93LDukKrsQCCfbEsSexkeOJ/NPQHQ/BE/IkjJNUXJBYRLH8c+ac+eYVfPI0eAwBmhp+/fgJ2oRI3khh4f3nj8/vIFDyEuE1hCgsltR9QxnLyRP14JNz6Ivii0aYPF4NJRaqLjCiCMnjiC6CMgocRQiaZhAOnp5A0jgZi/k/Cqcgf1Gjx548Fh5TwLwJ+J3dGDdAOwRruq963lO7B3iOMIuwqIq/arEn1gGHs72xjqQri6qXoVrWkO2Wap+tm8De1yY8FkUJvODLGBYlVr6aa7bwv8YYZWgMwM7XB3hFV/bJyWjIFcAZvTeK/Nt/pTffCrgFpucT6Cg5JbzBsOPo0iaVCwtrBrekm4TKO1mJVwHaSGRDxRTKpT9l78lFg34fdLuDWRup+fksbgEUWoxbZcn9NkpEhJUmu+yW1m9gthqTNy4GIGdvq2ERpc6OQmJs/pYLMESPURiX9/OlPll75+sru/LjApTilCIYB22XjFXVXa26o63u7ksOtLc0tEvQFImv5r6zDT7ZuERGEcFS5rFVgSFHmjU6uPAMMlePQmpUjPnDH3obN55WhU7iY9FOodY0YudxXt1U651qmZVdkcfURGjVA57rozie3pzrC56OTX3om6Y7HFTz0Bybs0Q8qZOo233xG1BLAwQUAAAACACxBg9ddewi2B4BAACuAQAAPAAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9NQVRDSEVEL2dzbGliX2h3L1NUQVRVUy5tZG2RwU4CMRRF9/2Km7ibhKEOGgZ2OiAYRRMgupTSedCaoU3aDhBXfoRf6JfYARNn4fbek/veu+8Ck8Xj/S2UcOVBOML35xdmN8tiOh4xNiPha0clDjooBEVQ2gfrtBQVxmNMigK9NEsz1F6bLQQ21snIbxx5Bbt+JxlSxpKk3+3DUWWlCNqajrFuJyr9EdGdCFKRTxIgcj7E8IAqjmkCtyLQEM8PSRKXOYExuzayCfFD1gE/cn45WJe9HC9zH4QLb0qYsiLXMq85XoUOd9Y90TH8ci2/n2NJ/h8jz1FUJNzLXNratJ1BjtFOzMlTS6WMY0GmjA7PWvIVx0lqlmBs+leht3WsC7TXJRkZL41t7/W+ufz8ltWZ6KpDKlcp+wFQSwMEFAAAAAgAsQYPXVWWyPfWAAAAfAIAAFkAAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9nc2xpYl9ody9hbmFseXNpcy9tYXRjaGluZy9nc2xpYl9od19saXN0aW5nLmNzdo3RTU/DMAwG4Pt+i7VmJajteRNHDgWNo5Um1j7UNsjOYPx7BgIHEJpyex29eRIlLgQmEaA5wOwmgjgcySeUt2mII0g8saeFORuz6oZw08J3tB62vSTHCfduDiPx3xl1j7CvnqWudjIeBty/IpOPL8QUll7tW6N2Y+HJHdJd5Hs6py/2nyXUnUUnNPn2rYVHEqV/ZNRukdlms7OwHsnxtvfxNKdfA2q7SO1UpZWHzeR6EkoaUFslGtX6smQNPFy++gKZOifUXpFns9cY+BQ+/iYn1N5V7x1QSwMEFAAAAAgAsQYPXXb+pJimAgAAiQgAAE8AAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9nc2xpYl9ody9zcmMvcHMyL2dzbGliX2h3X3JlY292ZXJlZC5jjVVNb9pAEL37V4wiDkAILP4gfLSHNm3TSE1ahbS9VLLW9gAr2bto15i0Vf97dzGGAHawT8C8ee/N8+zSa1vQhs9MpUKykMZwO/1y9x4W624IoZCSRUKCxFBkKDGCmf42ffg4hWlKUyY4ZKRrO13D8UTlHNNd0xjIMyH9URA5w253+xkHocEeSWLGIuQhjmGp7IVIMJC47s1VzAJQYiVD7Bk/pisUScJSiEY4sB3qeP2BfY2e49pk5roBIZE7ch13ECJ6kUftQuybRIUyQ1jsVXNmYBETiRpDukD4cP8OFhgvUSpY63nh9uE7BFSxEBiPGUfDRZXCJIh/d4CLFG4gE7GOIsar+/u7r7CWdGn6jXLPspSJKYQVV2zOdX6Mp/DjMRQrnvr7VN8CmVhWr/0iMtOdCRZptOaQqb+gPIpR+gWgaaot668F+jmhvLycbAoS05XkE+vfAb1HdvQ/KUs/CfmAz2mhVGCaxixfJZlUlTpb66a2XugQoHkKebMjge2z7Sg3d72Z3Wg/oToydX04dk5w6uqYcriP8yZGKouGovp6lvmM1YZHe/YPCX3Um1Ywjw6ZfZ+qxPebF79StQZokI7m6BNCKBmSi9bkHIjUATl1QP06IK8OyC0DxUyDbAOazfqzKha7YME6frDO+EjsOiCnDsirAyof/2i6UuP6FoKG09G4fkmVi2WVunOGeNtavbBo74//FHmkl5bYflHabCy09Y9PdF6yuTF7YaHYyRKjDbezEXRIs+G0yofJkyT6pVVgdJJ5kFqnCrMJMgf1iVchdELySjwueXmeiW2uSL8oVR1oGkVspXVGHbhyK0wMtYmmhrROI9sJdZcijscVUQzP3AXlW6P/ONimN+/XKZX1Bhz/bBCHVs4sWInB3YTVCbklb+A/UEsDBBQAAAAIALEGD10zTxIIdQIAAFUFAABOAAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvZ3NsaWJfaHcvdG9vbHMvcnVuLWdzbGliLWZyb250aWVyLnNonVRbb9owFH73rzhNK9pOclLYQ6dMVGpZetFASNA+MYZMOCRekzizndKq9L/PiRPo6DpNywNy7ONzvlvY3/MKJb05zzzMHmDOVEwUaqBYCMh5jkvGE0JGw+Ft1zk4ChdgfhdcZixFs3y+OB9fz8bDu1EvmJxMX5xjz3UdaLUgXy2OHVLVl5cdQr4El+d3/dtZEMx6va7d9uYFTxaeFiIJY8Yz5SHSKAzpR7fjdqjSLMK2l0tc8kcLsjp2SNPkuVr49OC37i9mHF/CZAJ7QB8NhGrXgen0M+gYMwLmwTAW4Ay4UjyLIOZKC8lDlkAQQCjSnCcofWiunrU6r26NisyHlN0jzIXQSkuWUwPN4n1V/Mg1dMiSE5LeG9WA5mAZp0yHsZnrRSrh81m8Iv3hVdd559BbSpFpjpLKInMTERl++9CTXJeAfcjwASVILBQCA4MiQRDzHxhqWMWGB3CNkumSZ9MblChkiC6RKdDle6g2C1fUhSxjyZPiaqeWxiuaGAnNBpWYC6nddFGZsGd1qsrpbi00PjYqOwdGBwc6Z632G6u63S5cjfs3F9DI4VvcUIYUF2AKnKpem3egGXw6Adtw60a7cuO9hrZFZBgADeBwJYXGf6e83opbpCmTT/76+2Tiq5yF6E+nH44G57e96/XgZjw+frV/WIOEbxXM9QZ+5wTW5k0WWClpYf2Ewzdj4NQ7bbrsyLajn4kqN6mImOG1kcv0/otHtL7zR6tce/jGsfL5bxkNm3VflJ9i5U7NbDNqK1B7K1Az1FK1rtbAm6yVpH0Yfq3TkKjtrd3ANLO2bW12yqXJTxOhOkeXRZKA+SrNv0UVtl9QSwMEFAAAAAgAxgYPXR5HrUHXAAAAWQEAAEEAAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9saWJnY2NfdW53aW5kL1NUQVRVUy5tZH2QMU4DMRBF+z3FSHQrLTgJUtISgkIBTQK1NTuedUby2tF6FhAVh+CEnASDkLJQUP83T//PGQRpPRGM8Vmig4+3d7i/eri+vdlUVV0vL5YwcEiEKik2MQ09BnllBz0qHTjXNUgEPTBkHYQUKPW9qLJrgmSV6MGj8nlVNWBejJnhwpGBXBw2dZYjJcfOPmEY+YTwwkCL+V+EV6UZOjsGbmfz1Snp5j9J/pNcmnJjH7+H2i3rHUY/ouf9kUk6oQ0qTuDOTOEd+/KBveKgU+aX8Euw47AuzatPUEsDBBQAAAAIAMYGD11BkX+aqAMAAMcNAABiAAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvbGliZ2NjX3Vud2luZC9tYXRjaGluZy9jYW5kaWRhdGVzL2xpYmdjY191bndpbmRfbGVhdmVzLmPVVt9v2zYQfvdfcUOBQJKdWFHUWquiBy9xuwBZYqQuNmwYCFmibQKyJEhUnK7t/74jRbm2fjUFloflwVB4H7+7+8g73tiAq2Sb+gGH91dXcHFmnVmny3Mo4h2Lw9NwZ8G4+ielsPV5sGHxGgI/Dlnoc5qfgTEe8E8pDekKkTlbxzSEYONnkFo/k8Jxm1YW89J4YbVYowQdyB+JeWO3YKRFhkV2SRYCIT7nGVsWnBKiaYRsk1B8EWkmRNf1byxNjvz5JK/YCpCEIQMaPtz8Obt/R+b3N3eL2QNC4OQEmsvgeeB0ZCrDSHmG1DTKaY9aCoTKrwb0idMshseEheAvk4xr4lOv5xAnGeVFFsvIy7BhjlzXv5PZr2Q+I9NfPswXD2A+mWbRjvh4PV1MLYGw+hC2QFz0IRyBsDsQ86uH2S0CzrvCWMz+WJQQqwsinJSQiy7Iu493ypHdBZne3ry/m10j5HUX5P63mwXaV6tioK6xqhGyppxEeeiTfYFoQRLnvDwoA785npwOn6E8FzCUveIxdO1wxcEFtQmGQh7b0V342uY2o2uWxCTnfsZfzP1ru8s9OvMxhogs/Zy+nH+zy7/AvLx/O5D+j2qz6iHsH0qSFaFxgG0jJI9+VBwGogilGVuoPvg8APzDdqJVa6JNNC6aXsVqunJDvmPYhA82ncAEM5K2AHNvq++3FYcIMllppRy627GpLPn9JqsXZ+9xdi/O2eOcEvdV/pbNC0NpnKo8yOcLOoK2o/4PVTafJuZ3he6wyvbWZVQd520thCZQ9cA98HnXX+rQRal6ZhtlT0X3Uqoe20bZ3aMOGBv34rggFV1G/ZAUEV2eW06j3CtsOmpMCoa8RtW9OCrkfMNW3DPdavvyE6dugyCjeREJmCQIE2wmAugZ6XDoKusXTxNrJ3hlVvrlpSR2S/6hN8EOArsNiyhUKMcsVPJleF5J41YSpv0y5D8kQ/5/0EEUrMTBJTiGalslqZyxqh34jOs/eaauHMIXD041TasFq5/fVu6Pddbquuhtyo8NmIYh43h3/QgtQfJIMzG/Uf+R5iNICp6zkALfUGxC2y3jHK2T8QTWeBhyQlaj7GExrLP+Z2p0fCq4jT6Jp6v2YuVRwnOv8Y4pEne/5Ich5pZ7Ev+XZPvbbX0HMUx8B6uRU1cbxbsn46vkqqfw3eBHe/ry7mEu9Sx+OH5D6wnZk346BgaWvsCUMBQzQjkj1bU68lfTqiLe62Joh746vHxL719QSwMEFAAAAAgAxgYPXXpFx5lEAQAAAQIAADoAAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9tYXRoZnAvU1RBVFVTLm1kVVFBTgMxDLznFSNxq9R2gQJCnKCAuCAh0Tv1Jt5uRDYpsbcCTjyCF/IS0i6gcrPG4xmPfYB70vb2AV8fn7i/XMzvbq6NGY3OpmfIHJIl9SmOY8odBf/ODh2pbVlGI9CKfBSFtgybus6rln7woj6uJsaMUb1W1eF545wtBGl+EaqqoxnExz3k6GQGpX1kdmJB/6FTqiAvWfehukJD9b74qa0Q+05ZCtEshu3WgZXRluVS9pbCmF/XmUVKOsxhKTrvqDAyd9tU8HGXK/M6iS8zbyAxy134km76NyDTgrXNemKXF1gO9dOP++RxCS87nfpNuViSVQSmBr2wM03Ku6bwhqO2aPpot+dGzZYKY2j2eeM3xRNXN4tLHKMm+8zRwSUWxKTgcvkt1aTguChSXrFi+5rcD3rCgYeqyakrochRHRjzifkGUEsDBBQAAAAIANMGD11f8ldKbgUAAHwQAABNAAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvbWF0aGZwL21hdGNoaW5nL2NhbmRpZGF0ZXMvbWF0aGZwLmPNV21zm0YQ/q5fceNOMmAZ+Y4Tb6HuTJo6Lx/iZDJp+yHjYbB0yLQIZEAxqsf/vbtwwCEZJe2nKJMR7D27t7f77KPz+emEnJJfd6Uw1mG5uI3TFVmE6TJehqUoSJTlpLwV5ErcJ/ENYTNGZ5QA8jbakEWW5/Eyy2cY4m1clFkeL8KEiGqTi6KIs5SUuw1EgXhEfA2TbViiMcuXIq9DX16SN69eET4z4R9EOZ/8tBRRnAry/uXnt68/Ble/vyf8wPjyipj7xndXrwnbN17+9gH8DwJcfnp59eaS8Pn+wsd3kAubM8szbYtbrud43OTu3D4EBh/+uPwUfP7zAxTFcqjj2dy0HW/uerbNPJMzbk7w9OAHhUqLkmxTPPwDfBfxKhVLkmRQ7MQnUZKFJYl88igrG2xrkz8RVSnylMRpSUSep1lnGeDIP0GalWG6XQdRh2hX8iwrxaZQVjAaYKG9ZRR0vdZ0f9L4FHf5YKGxVjqk3jxCc7dJ6ZMgCIt1EGgn6DEryDN69oydkBfk5CI60RqUjq/wVum6D47lFhJo/R/lhlF4U4xsKD2C4GYbJ2WcBjUWoqF3UQKfFrK6MnfoUXAbJlGwicmF2hp/FJ7h/68ib1zojDOXU891j7jkX64B+kAMOmM2fizbds/A1+WcU9Ni3BMGNc9qgOdSZ85cxwQTR5BpU+ZRTrktDIs8dtsEQZwmGD7A0pZlHt9sS3jTtDC5D3eFXNb1iZLKSqQiD2H0Dgt4Vrd6kSFOnzxMCHzQgvTzGxq05Nu1D1VnWrUPmyzplmXr6lCNZR1W8Xq7hnKYjLqm67D5jDaA4j4GRSHaE2QDOpAmoUVYqIP9oiE6hFNGuGNO5R/4wNwf91GGYxY1/o9NKSKiyeLIXPCzgzCHdCPTAbX8Di5nAL6/Zgl0McF+ndRDMAXa74D2HRbrjqSUSRCRwDH6nTGfivwMBKFIfYk2mF/nZFS+zBs/jWsfsYZ0iP58O/JL2yGMuV+nWgeV6g48ZSa4gQZk0cF0ejgxBqAsecg6q2/gpwoe90nJc8L07rQ1Nw/aU9VBa8LpabOlsuNgtbGPdNFA7Gkv4v1OhqKUcPIdef6cYAkUsy7ZD6F3yu59A1e4AuF3fctxdjA5TZO6wa8BsGrplH8xr/Whge0baGPoY/ZZAAj3qwdUabwEnF6QvppD6Z200gsFPq68oxoD6sL0XsMB9/8DUSUQjOpTgaR8PSHI5X02kG+b4w+w45jcH3PZ9PLtWS5lCBYGQyke8bjrPeamx7jlOA5Kuecw27VcbgnDbN33FDU6ENQKxKh7Xop0T1ylMv/YEgrXmhCSfUFuchH+rbLv6dHzB5LEqDVnc6hiI3XHZWmnylJ1KEsVjIBKgiOKtA9VxShCBQXngUB0t7xRpYhQKaJ9pXiomwwRofvY4vouQtuDSPVGvYhgN8RINLxpGxh5nHj80endtTuQBqkMd60owLMSVpHTbn+jIVunGXW0c0lBJZsegH7nDS+PKUf4nwcVy8PrQjjcpBaFa9HovIXpKhFy5qDdOGqWyS3PdRzrTL3W4QudOzDwlvU9A0zxem9aFK5kVEyPeSgiMXeoy+FWN2f1jc6iHgXdMC1VNMaGPce/eZ66SkGBs3Wcohq2pnD517Yo1yItfwhJkMtjw1DjaQej7bVvXBqib0hDhNLQSELUTAwwEUqK/DcHdEULHTpqJsCNhmW6eqPrqwpODQmNemw6RNT8RCtAGEqpI+0XzKIO2WhNAHzr/dPpdP9i+V0q0Q1d5I/oQnt/2PTDv+luBCqFANQqg99wDiy19/mQaf2eeAiIIwmqSsgvKCEd0FAv/dI6vZAjml77h9o85ronJP8CUEsDBBQAAAAIAMYGD12Wfv9nMwEAAF0DAABVAAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvbWF0aGZwL21hdGNoaW5nL2NhbmRpZGF0ZXMvbWF0aGZwX251bXRlc3QuU51Sy27DIBC89yv24FNEHMBSW+VUVcp3VBiWiNaGBEjr5uuLjWs5squqXQlpPTOeHR67DTx/RtxiJ2SEwwE8SmdD9BcZjbPgNNhLGzFEDU9AO0qZoPeSlrDZ3UGqMmAE6zw6r9DPoVZI7zIQsYu5E405WuD549i4upn8X6SwyigRMbNo4xq3hPZZr71oEYpwIpQUFctgK8LbEDsXoaN2HW+jZFBc0TtSaDZafKSdQeZliecyQOL4uqCWTAPTC3zev3oovBhaoZSB4j3lHWbealneV6v7TIL2A/nCTAXfJJaRXsGrmavg367dg37MhFWjtl/ZrMbziHHC/xu8utHyHLy5mMG5nz8AzptpPO10qlkqSvJis1STvf5Dltszqva/yJd3mB6e+vFRBnPFFZKU25U/vgBQSwMEFAAAAAgAxgYPXXjuwUyLAQAA8AIAAFUAAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9tYXRoZnAvbWF0Y2hpbmcvY2FuZGlkYXRlcy9tYXRoZnBfbnVtdGVzdC5jdVJba9swGH33rzgw2OI+uG4D28CkkI2VDdYwuvTZKNbnWEyRgi6JR8l/3yfbXei66kmc23d0ubzIcIF7ElJsNGFDnTgo64TGzkrSaK1DEG5LAWVflleifN+UBbDuWPw7EHYiNB05bKLS0qcwRrp2X5u4C+RD8ZNDGxE9IbDHR3dQB2W2+PRlvcQcG9H8IiMhLXkYG+Bo76yMzaBPeeP4dx5WSx6kjA8uNkFZA0+axl3r7A6fC9ZfZm8ktcoQ7pbrr7c/6tXDHeYvwOUK1/+C31a3uMqUCZjKt3UjjFRSBJq12oqAPs8eM/CKJo19xAgfhI5UMejV1pBEyjhaJyuc4Duxp2owacsHT5Ka+sCnJlkPqilxMg+qY/8/lPr9CKsWsx6LBcqibHO+tBCdQTmSw8Ri6IQFpqCXY5mbpdR8xnXz0XSuc+wT/2x8/lp1rpXEbLm5wfU8x1v+LR/ajw/fz20HzeIJzzFe4xPL1tGTVqKnE52fq/qrJ82f6TnPLzfyp+wVBf+CKjtlfwBQSwMEFAAAAAgAxgYPXYvsw96kAgAATQQAACsAAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svUkVBRE1FLm1kTVPbTuMwEH3PV4zEW9WmN7ZdeIO2XCTYrVoEj9Sxp8kIx87aTtnyxD/sfiFfsuOklJWiJPY4Z+ZccgLrH4u1DyKQNb05SltW4PixQ4cKvK2dRJAFypfKkglJ8lCQB75CwfvWeKtJifB1thLyReTIdRHAF7bWCgqxQ8gQDSjU1EKjcJrQpUlycgL3Fw+zm8UcPt7/wmgI29rIOJBPkh50OvciFFfLc5j2p51Oc0Zav+2CJ8P3IOJdtA//ywV+bEUWD5i6DOjDNm1gNGW5lFCbVzLqfzSPOx4tUhcyHOpQoK7Q+fbT6/Xd7SXTcOpVOGy+BR8cyXCAeFyxhi48F8Ioja4LT4LClXU/8Hc41LrwwLMcFzPNCjyupK0Nr+alWKFHflujUbwajJrNwSgCpVF2LgM3B22l0HHakgLr3tPkA5kc+uxbrDVOGutKoemNhS5FYP98Cp/OGRtgjwEESC2obJ1q7SwrjQHBOsrJcJfF3RVoMi8esn3AHik0gbi93re+Pd0uG/qz+eMcVstZkiwoLwJwp8AgcdCmdDS0YfCVr9oodLBhmL5UO/XsKtnfdOGVQgEXl7dsglBRzVIY2rJ6wPKy4S7n8eNEvn9gH9nhvkGPEcvQcSb1vuGqRYaam32GLEMpao9HysSGwZZB0FWOMx41YlCtwVZomOg6OMtVbn88LnmQJveNHWzDOWwQe5yvcTrqDcaDs9Ek5UHT/I35XFO4qbPoD4roomenYTMaTc+mk/Hk2yZG+Q1hOOxOT6fdyfdhS67VeC0dVYF/hY1v3/qiqvS+1/qqUl9sIO4QerCGOUdelbMx021uv37fRj/DcXfxRGm5SWsQO5DCTx3dWDqbO/R+PPh4/3M6gUPXRlsyUteKtdxax3QYQ9WSMtIUOBH/AFBLAwQUAAAACADUBg9dPK5jYD4HAACQDgAAMAAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9TSEEyNTZTVU1TLnR4dJ1Xy25cyQ3d6yv0A5bqRRZraWSMJItBBvYEWQoskiU3ILeE7rYH/vucq8m4O572IlkJ6nvBKvI8eK7MnqIWV+raM4U20sykIrmESERaPDyTLZt4kcusNXorjWUVSX57+/PbX//yt3c/3T8en3bz4eNv97+8/8df37/78KGVu09+Q83WLDk4MtfZ1lyTeozunJJZ7tw8ZfFcSzPrRXqLlTxLD2fvI1854MOvb3/954et+JC6OpGWtazNMgoumjlHspWqplkisjtNLlSr5VksCdFMnZNyLnaluO716etxd7z/pCf7uNs/fnv08LQ7nvDDnR2/3Og2MSmpqZSCuXAqtRNGaNlLnjqop5Za0MhNNWnqqmusQsmcq12b2/Fg9y/Hcj7vEPb8JQ7hd3YzVqu92xSTVa27Jys1lz69R9asyzzWnM1phNSSVNqYPHKeXEVGunLe6fn56Xh/+Lx/8/rTm3V43p92cbg7fryZ3VW56SgzVxZaPoYSrZ68rpzTbIN4tLBozB6De+BYtDlG5UwX7aHyo9nD5/1vu71fQKfquXlrKTc2I0q+KurPqa35mDWW9aa1LipcB6ZbWiqNPOdSutT8oxO+wWa6953rKY7//cbDU+iXOGKmHW1Vi6JePIMUqxFGm2ShPFPL6HipVOkTx7ZiOfecbSOnVUB8cQUc+nG9XHRXRw2fs8zVV3SqQG6Em+K0NKj5HEw+kg2rdebuw1rhzIt9KonYn0pfa+v3R2hEkpEXM0tReLkkwrCIIYraUmodnCnWu5eg2UfKFWB20MlZpOqQ/+G0h/3nT/jvdPfhJgI96WsLBA7IzIvcO89eIIoqCmGkaqMD5uwS6E0SYGVag1f8X6faDQ6bowRGmOEiUQq0XqqKLVmURlpgbm8NtiObRssCj3NKAr42xdxvb9+/e/vTz+82lGB8AYZDxgGqD/FaNGcNaa0XVsmwPqorjBh0DYwyrQbXa1GNJPT29l9//+Xe/Is/HF7sbHz0WlxZvRfjlg0XdlAhhgr3YR1/YIgOjtVQ8AzKtslS2iqjpxm0on1X/NL0lA0KR2etWRFtymShdZUJE1fQuhP4xzFMJGnNaRRZmNYAZLS+K/zN8NbnvZ12z/vjt2cPKeUxo6c7PX66gdFsdciJzAdPF4b/gHvA3F0GXKIIuJzQY4ULAYtimNLy3qHe+qNzz7D/ceyl0ZoOWEKHm4K0vLRFDzCXmral1KLNpanmnEXnTHOCWcGMG/GG2fp+jNc49sfDTUku1hnnTAwSmwn1h0zAbsCugSbbdgRc0+G3sSrEBl9Q7IGRZi38o9MiHnTuHuz504uefn/+FI9qX//z093HG/jdgFJMN3MtvixZsBGWCo6eBM+BWanSxHpcRnjJBAa8nGBX5PLd0YdYWB17i/NYT3p4jNPd3O1vqALaTT0TJLIxYB5hmfrcksA27xIscKwMyB0U6+i+w+QVu1XyBJZHO+xeTsd7fXl5+vrmtVEsKqwOWngBWnFU1AAfWm9MmWZtNDSmwhJ1LsEgDZYPGLGgsS07LAwSiHPpl8Pz4yGOx5revJ7yuphwFxhIpJKRIMQHVaJuTgYiis3WB4dSKr3CaCcj09TBzLApVgjhSvV8rg62CzwEyl0jAV/cuWX4xwxIh5sU2ix9sU1D2sA+r2NRzRgejBWWc6V6OVeXXpcWuCbm4YogUkCt5ob9ALFuuDp5A5cxHkwecYlBa3mdHE+7dvd6rj7acPHpWDNBGasL8x/cFnislWuRUdeo26pYrUtGMwHJAs+uFaq6Vr2dqxusCuRo3BXVMRwhSWuB/xBIxUwqY6d5dccyLSAUCFTgApYtQNx8pTpdoJowWJVODDdHihnTm26BsGEEicdmYamg/JABFBNMqNIYAxBzB8pXqvMFqjVllbHAHYJwKaU+YY5lYVzIj3mMjCU1fcLzF7pKuDm6dC1bPLB0pXo/V/c1EUuQNgNuo6/QJo+yhQva0meM0UEgr4w8XXvG4dV5M3roKkivVJdzdSjfkDEnImbCAl1YnZRA7RbFgAMMnlNeW5yOMsFVKUAW5CzTqk7pV6qPc/VQN8gFwUBlYXOrbndHDFeY/MLic7UcnjhRr9QXWkFSR+bsSGVT2p+rtwutbhqBvlNC4FVbY/uigFkNHp1AbzAUpgJ+syA8ICssLN+FuJp4+8CodKX6hVY35aBVjigirZYihPVWEM3w0dIX0j6pNUTHgQ4oEWGrrozAvrm7XEO1XWh14HZAsNVahoD3zSYIGdqRrpyU0ILJRtkBMNAHjkAqRVyAe0PbcqX6hVZhIDNKIMG0vJCXmk4AMKRzcTgKNjU+afA54whp2gOk7RvtGemTYQfX7n6hVbeZcXcosYtjcUJESO3MNaISYjpyrEBkSEkLawU0zAXfUEGbPac6rnCmXWi1b/sBjo3oL2QTexbbFt8EoUMdhgWZIprYrBmBhLevjyb4EpoVQ3dc4Ur1C63+G1BLAwQUAAAACACxBg9dsDKc+BQDAAChBQAAPAAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9XSVAvY2R2ZF9ycGMvUFJPR1JFU1M0NS5tZF1UwXLTMBS8+yveDEeapJQyw8CJKTDDhXZa7o0sPceisuTRk5KYEx/BF/IlrGQ3bbnEsay32rf7Vq/oJoZdZBG6fEd/f/+hL0elE/VWUohWK0fOttrsDUnIUfOmZ2U4Umf9juMYrU9N84RxSdKHAxtKvUp0/ZaskA8Jr0yDFUEVPfC0JvqBlZYFmy5et9m6ZL2Q8gZFzRhDZx0LtSH1hDeTNTB/cQzEld+gku55LijYogamFBL4iv3FZNgltW6aH2ikAMz0Ig9hX6uAiybUiE9HCzAbPHUxDBWMjyNHO7BPFYEBwHsbspDGedaoxJQFhBSNyhj8kYQPK+spTSPToQ/CFLybaMxxLC8HJaDXjE5ppu2d7W5HfeUsjviskrpPa+G457glqPb6/HhxCYU+keS4t/ui2TM/7r59pdubq2YxQgff2ThIZa7DMDoGPV2xyakp5PRSpQpP83kUuk4YbZYun5rz4VAalNnFZ2d3ll3pNmadcuSzxY2dCy2+esBXSxrD2qk4yxoiaJ6dOABpZ33Byl7XDbVsM49X0b5YVdalVyPLYsHy+YkjfFVOAhoZy0kw4eRfZA2bI5Zg708Gwbn6Q9OsaFtHB5puTliyKQN+H0e91tsXW5jvVWvvi6wqzbsc75SelqV1v53pDcrbrkzzGJAIKTbOk/RfmGQa2uAemwJjnyxsiNl7PIruUfmHZ4OGDExoyAVd1Vz5EAflMOKGjO06zKnfNe2EjWc1aHXqUHJSt8Shxg2CDeoBgIU60vUiw89mXpqB0Y3fddkR79ljnrH9qZ9VlQcwGXWIlCrJPd+8R1Pf0DZUkKo0xsTqtDHcqezSxodlZQX+qtwEH7EJsyvzgApcwwNgdbwqd2TcZFAVLBseGT8+uakUIuFqYrNqIRjYLFsXVAHvCLlRhfyYjL/XFwirYKjL9+s3G9xNOC9FuFFWajxOcbv7/uUuWe2KRcw13WV5uZfQ6Pfw+ELzBTOEMoIDpg76dyHSwfoi4nzzKTR+hAvXxZ16VFWCinHNrowzDEdoy1Ux5zhGa0JcN/8AUEsDBBQAAAAIALEGD10X+EeecAEAAB0CAAA4AAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL1dJUC9jZHZkX3JwYy9TVEFUVVMubWRlUctu20AMvO9XEMjNiNTYTu2mt0LO65AmsI32WNG7lERE3lV3qSDKKR+RL8yXlAqgXHojORySMzyBYvNrA9uHAt5f3+D37QN8gYaThMgWW0ihj5Ygkg1PFMkZc8l1I1D13goHn6DBJ/qfkVV45HYAduSFK0XG7u8m+9j359azTPGG0+OW0A1T4Yq9u+KWpnwnoZvifcRhS3+ntCZxHD+JbZ+aAm3zSb0m2fELGVOEqBqEHBxC7x1GpgQ16+nSENBzN4FugKSMBFgJRcCWa39UDdChc+xr4KTttu0dudzsVThYnc0uxBGazX7e7+EwCGVHFL3EwUAymwF6N+KOWj5QRCE155E6gdBLUpfg7se+uLnc5MbsJAZfUxKdfOzUiAiVbqbYRdZDqlGA2gwVxtHQkiirrV3mi+xseXaxWOWCMa9fSsWuWW76g76vJUwEmBKJ/gTKxWJ9sV4tV1/HrlEwlPP56fp8fbr6Ni8/BCTzD1BLAwQUAAAACAC6uw5df1retKkKAACpNQAAVwAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9XSVAvY2R2ZF9ycGMvYW5hbHlzaXMvZnVuY3Rpb25zL2NkdmRfcnBjXzAwMTliZTcwLmFzba1bTY/cNhI99/4KAZmDg00G/JJEDRa+JLkvcti7RImAFxOP07Y3Rn79VpGqItlSt9hGB0kQZd6UisWqV49F+Yfml1//82vz22/N7//+pXFv5/OH+e380ohvQshhWnrx/Bz/2+lO/6Np4v98aRbReN9Mc6P6pjmN8/zh6+np86efmvCvn8U3JZp/PX9Zvn35p/jm+a8F/xHvyZB5aaRohGgmj+aa0+f59HQef4L3S/EOTP1ISPuCMESKhJQdIkuge2mMCVjVaAfA168fTk8KgcasICuitakRbQZqETQJQSgTUHpoWtEok5aJr40W9ZDQ4KHuG6mbEWwi+u0cbUa7UuueoOCjgsiJ1c3m9Mfb/5bTkwGs7FbQAD6OqullI7pGoI//HV9PYGdwnbUptuGRAjqYYFmLNVhkGR3+ezm/EQx8lSFIBmDo6/T65e81SghYXPYCfOQXgOs+7JXQRUz0+oaw9/La1ntPhkZYnqJ9sgtuwV+rA8q8gzDQho64C/Pqqwy+flxyX+fS15l9HTFnZHiFznZZh18kDK+n265H1y5mEhwVVVhR1VE5nZ6fn1dzDp0aglOKlxxd4lVPplj1ZMiXWTxiRTNEfRnDigSWKLhwii+2onix5WKebUXlzS7bk/38SZuzwFKMumJREQg81bZpx8ZA6SHo81/rJrejtu+eFGXSYkNxSHtRHLosjsVlpDQj1bxeISUvokHRdIVB3HXiSm8y4mJrO8TlMVuDa0sIDtT7GV9LP0bOiHaucC5SbgR7AY7NRwStrxL0THvqhTneAS8su7ZhcZWv0YtIfBAtWwRMYsTWFPFScPw3VF/E38u09cAhLueQcuu9xOB210nE66Kc4JEiIB9EeF6JLEoXaVVGSZnDtPLKZkG6SKsySMrdyiuvwS19kFea8kobKnF4d0qH+Fqqca8TEZgMZUqQC3WxALP3jerSe/vQBGNUtbiSo8BynKNGUJu2ptghbKem7NLeGEoE6Pw5uM34pyOwremm3uBS4nptYdNmNmndbeQMY8N2JJsDYHvCGDY4lsJD7FmMXraXtCZl6WabIu6acVmJEktvDXXWdX2HEs/tqo9+yjsAPtJOdMx1I/zt1zeQv0VWduA0eLLtL14V/QUe2XryXyfFoHfdR4XqjgjQXCVAxy/tawiwTwQoM8bqt6XdP4pNbCrbDefq4pXW1NJpULBVdIoCFsr2Op2OhSaDR/J7iOkPAnYskxXLviWQyRZ3QZXl4gZ7TJWDy3YnUeXO7oyillSDKL1Oqqg8zQGpGqKkseLE4qcYN2UvOmebNc7JUIFcOa9UU+q0nrYMVn9uqcv5h0l1ghW0Zpcs2rYgC3ikdzhxfAjyrp5RXE3XcUl+Xqf/VX76+Z7GMps6Fp7resoclZK2F9Hp8+gsgrfpRuNhFxdDrae0ORQ27XcQP0rmITpyq2EN5IlPjuui+eu1+eduo4JWLRam0tHtt3Os8EAaVOaon220aQqbJtm0bDMRmMK9APDHORZdLLzYip1AJY2vjUwX+tTyZ2I6J4YiJvD4nn7TUMWqItptcJxAzF92w1+GMOxrZ5rOBlZ6XRO7f/ekf1xxUlCtIC4UwHxecSLHYU2Fvtt1mb0QIZ/j+ECCOLYXcDbH4Xkp+tdn9gK/ytxBxfSKQDbYE70mILas6KHNLNoAzF0MKtgSkC1GYO6j4hiOBskdm2AWw5ZwmmOIOBNwWQwTjmM4dpk9imHCcQwRx/YohgnHMRz7zB7HkIGGY4hANsgxTECO4WgzixzDBOQYIpAtcgwTkPvo/mAhnD/TGMMJFJzLdGuMgcldlI9wVD4dH5fGLVO32WkXoIYLRGXpoi4LpOMZIuI4W9RlgQSh6fcLLs8+FJlrgWwKLk++nv0bVbYV6jL50owTcbwT6jL5evZvm8z5zlr2b5vM+cZacyhAAATOObUn2Ify6DwY2kLLgt3opCdzwa7IhUFkWzO7VXltdmYwGSUxbMNIg82IhmFbnkFlyPTBuC17jCLbGO9WCb3ZlyQKodgZtqn10WYlzLBtBY8uK0zGbetyEnfWJSovr0Ndmou6NIdtDRVZZV2iIlu6bcpgxRbWO04Z5yjSpI4+vn1afzY/5tYBDB2f6QBk664mAJn0e5+Z64vyWaUa1MLSF2FDCdNXS3MwZA5lLoAqT3KAdNUa14kg28yBeiYffM1VBMCiAjKXMHsBc0djEydFPFG0l6ZYhBKOxyu7p1cnZXG9Ao/v6TerRi2Aqxwhg21x4yAJPzbVU1/0827pDr/kKk9YTqJ+89NOPcMqyoCxdWVqA4FS7gEjazDkjqtbalE3sgZk5fgEkLVFJ8Pss7uRgG2ZgC0noBEPmR+BIVM3jQbk4YgFMK5ucAJLuZ3vrameRmNYjqfRgHLHhCk78ZhpNFgy9YQqgyJtDwi1JbCrIVTZi+ppNIDNMa0GWVo5jQa0q6NIK76DrKypJivUq8uyR1amEB/wyNarB8xOomC1R3Rlr9KV5SUNFWJEoqCN1Te1iYSUDJcVxbIfdD/vJGre2Enh5JBeKUJGFq8cYw6NcdAhE0Hgjg8EShNRndHoEEc9hbmYQLDBruQbzLZuBU1p/K0yc/ZyQgzI6N0Q8zF5h69+sgSqHOADMno3iIuCwVdzxaDA7q90l74w56q7i6vuLkFDTze6iy/O3PBIez4fD+cBZLLFXXSOcnFzRedA8cyZzdb2MnvJEzIhdxIySGTKtdSNdnIt6GRKo4TcSaOl8hIBwilqe6E3N3shDTRv9UJLncQfXyI4JQ4vEQDzoEsEsGRfVh66cYmgvSf4/ZcITsnjSwQAVUxiFYrnPjSLG9725CyqZhXuEcCkVDwWjgmBiLHoMvBILqs0+746pyaBoFA/3x0UZa8eopVaJ/U3FcdAr0eZvCosmSus4myrgkS2GN+uXeP79fXL6WnEBvAZG88IvPtD83s7oDL6+e3Tch4/zvTblhIOKjGPR0g4ecfZWKGujlcAQHt8wx0pwti8fyvDiWO2iWMJBAuzYZ9gi9ONuY32TGEPx2OwUXq7Ub48HXk+HSmUzUC2aF9n9iNRWZvbb0Wt8lFtxYRAoXYW6iAPFIGrlKfq6u/LAGwOL7gAZA9vrABUpzgVCmNonAe3ZdYRfJ0SHNxpCYpo/x2nb9XzpyNyL/m7TFkrK+7iU8tErmVSs1k9QXr1lDPWUjmMm3JQkkDorF5zMCO92K0RMRTKAh5pnQN3HrGlYNrI4XvIbrhBdiiG3bx3BpjLM8DM7DwK+kaw7BU67xWoeBe5YxfotQiA4gCM6UZdpFLP7mjzYkcJ/IhJp5qOP6AEUOVH2IC0x5NONbnHTDqVExU85io/AQSkvYOdHN8X3qBHSodVNh/R4/qFwMGkU8328Eiu1q8DjiadahG3J53KlyrFcx0spo5UF1s54FPLre8Q4dWiftKpUDnfzbXeVrdQVNO7k061lAVOH8Y7LURlIDQq7EdMOjUK7KPq1sJVnjC1rKg3LU3l4FTLmumclg/6DNRpdYf60KqWNrTiG/wbTGAJXCWUtBY1TKD18XBO63uGczoMntvrfKDbgg/gkaK76uUjPtDVc2VtKr9gBuRN5tBt/RfMuKL7mUO31cyhW/6jGxfMocuxo+axo85mz+WfhCnGjv8HUEsDBBQAAAAIALEGD13zCLBS0QAAAJwCAABVAAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL1dJUC9jZHZkX3JwYy9hbmFseXNpcy9tYXRjaGluZy9jZHZkX3JwY19saXN0aW5nLmNzdo3Qza6CMBCG4b3X0hzrT/SwlmDcqnFLhplRqlC0rUa8esOxGNDF6e79Fn0yKRAZtlawJqGhZFFlR0aX2rrMqkLY6mqQB/Iu5SjKeC6Fz72UYhHv4nSlletUCQ5zpQ9DBE2KwLEdIt0oNWf8wUHndZtzD8XKntYMVH/OQPJ9G0ryZKI0JargjxUE/ik+R+PfF7Fx1blTYVDzus3IX7Y1UK/50h+BXPS+awz4Eg7sSJleh2GN4HMip/6fiqvNF4A5f+0wtJHanHl0yW6jHtwf/3BPUEsDBBQAAAAIALEGD10XouBSvAMAAMUKAABNAAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL1dJUC9jZHZkX3JwYy9tYXRjaGluZy9jYW5kaWRhdGVzL2NkdmRfcnBjLmO1Vm1v2kgQ/s6vmPakyiYmMZTmKqV8SMG0qLm0xfTu2gpZm/U4XsXYdHdN4ar+9+56bbAJTS7qHZEImp2XZ555duyTdgva4K0JlR2R5ZxiR8RkiUBJGrKQSIQo4yBjhJgJmXFGSQIJu6LhKgTPA5owTOWxyqITjfOUSpalAoajP0fBJGVSxfIsv46NZZzkIh4SqvIRjrDkKJCvMASW6iI6h3/p+ZLRBMEAAskRj8HEv0Lps39UcBqWBm/mTz55A3ftPgeR8xVbYS3Zu1feJdBsuYEsKroQZIFb/BFZsGRTgX+DuIRUnQsHQqQJ4UT3AhkPkRcVca0BC20sWBJ1UmLkqFOdtH5jKU3yEOGxrhIkeE3oJqDZYknkcfy41WKphJtAUPRZNFoQXxJp5alg12nBhAQW2mctocyMwvZAXOVR9Nldd5+67hyCgEjJ2VUuMQgsiyTGyTrt2/YuWBWYLumwGNKISBJIoKF7ZiAU6JiakYobQGXdTs6yW99aoD4FInWqf3+NWYJW1zYnxWkElirzkqWhKmXBE1XAKbNM/3bAteGF/uIoc55Cp3vWCFXex4UGODwaaL8rjuSm5qOhrbuu+uyMBgXrdGxj+27ANRvqlpDLwir8e63DERM3UyThxtKmRRbitt3IelTL1EBeOJSDmKsaOq60Kg6GJEk0BwUFpsrEfzP1zkcfHdexVhkL27b1pIq3nf5ho+u4dhN8ddrsYaw4H2smqLpyap4x4W2ItIQdEJLnVMIso14q+aYNsvz179uUPKXLjWUVeW2D0DHpu26vb9/V+HhyORpPLrzDfevwQ3ah7nYWWXvY7aPCv8bKAhcaWdWSA2WO3rPTuWr9cJa7KNVQDKe+zJbWPRzd1bg/e/vucNM/Md4adnPGM042U/zycJUeRDebnn+ceu//FzVeowwZb2hRLbzYyLFuxbXEVG9RtWgxzRfb8D9Uf1D+v6XgrYA/qxE3tiXHL4Hab5zpzW1KpPg1qIpXjDViVNkqpuztoffBXIdth0qiT+1tKrB2TcJgAJcfLi7s4my3OA2LWton/blZwGaX6W9MBO77H7qPVbxTI7Xb+725F0vP7rOeqVRSfLZ3empOtdTsiuw9n1PX+NQo38ntL66Ie0nozah4wteuZ829fc/tPChb9ZQfTaY/3yVH3d7zo77+OyjhW/ujNn3VzE7O2+nV9aOfS7v51SdR93L2xjJvKKLUklUrbO8tnt2r0a+sn/HFB//18Hz42vsPllDjxtTfv34FYfnG9gB4962gH1BLAwQUAAAACACxBg9dC0pErzcDAABFBwAAWgAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9XSVAvY2R2ZF9ycGMvbWF0Y2hpbmcvZWVfYWJpX2NvbXBhdC9jZHZkX2xlZ2FjeV9jb21wYXQuaJ1V247aSBB991eUNPtgCCIGsglaNA/G9gQrBFhMdncSRVaP3R5awRe127Mh0fx7qi9czEx2pfgFuurUqSrX6fIVy4qUZhAtgijauJtwuYg9/y8/ngdvXe829pbvV+4mnllXiGIF/X+gJfYVlZRNUbP7gqaQbAmHZjx56mGFgGY0/ImnZt9oLCaWdWWKXHyYz4+FyAPY9kPJUuh2nI51RYuUZaf8ymN3I5atqyQo0pumSGLROUQgby14kwgQscZElD9Q7hNBJieWC8yMkpRy67sF+Gim6ouISZryibJhO8CrJGapPqtGaE6OBgnIy5ROrEc451Sdvuxa0IUZq0XJWUJ2sIqGEIU3sF55kOwYRbYd2ZeN6AMsCxBbCkEA7jSEpsbXtqWc/iEpQL29MrPbKTpwfQ3O14GjMWWW1VQgqlatd0A/CjN81eLRyQ+IA2aMGAnbbLUyIBJEsBILI/yeCuw+pRXOBUN3e0jKImM8r0GnAyLghUyEMEmi5BQWTIC9+xf6/X5PlWH/VjudTh8RL382FU8VJydnJvPs1GB7PqSkzHNSmKHoSd41WdaDbiJ/tf1CPICtxBn+lT2eR0p7RTjJJ8+lPwkLurr10/BPpSsBmMhNiUkF35t2ZL0Z29H51J20DBEOx1jGyrDiZUW5YLQ+misUJyvuB59Gn7VN3UcJLkhOPw2G4xeDz5fooUI/QhwTITi7awSNY9uuSPKFph15eWjR5HpkOOj3KGjQtSrT22AT34TzIIqXi/ktXMOg13b64froG/aeC3QXvkIhYGQ9yi1gLr6WyfqfgxKdr1PHGYxGb9qIm3DhSyaFcAZtJ6ZBbhPuDNvOaLNcHbmdV23nZu3eroM/jfP3ttMPo3frwPVvlfP1RUHzD9HMc71ZIJ1vnhQUhR8DQzu2LLk3UCFTvEGoEvupWFCn6tRTK0auHBzIHeX6LDcMTsmweGS3+1UWPZyzRyu+RslrUC03RM+YOU0oe6CGTnku4//rTvUu7xP2oCwY9Ddngk5RgL5Hki01e7wSplaZS+pSqbuL96hIqr2tTymtsUNcP7UA4+dJz3xgoDgk6eY0l0H6cB5ker4IOnxyfgBQSwMEFAAAAAgAw7sOXYpDG4WoAgAA9AQAAEwAAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svV0lQL2NkdmRfcnBjL3JlZmVyZW5jZS9jZHZkX3JwY190YXJnZXQuYmlupZS/axNhGMe/ee9NLrSVnm2g2XrS1zTLCxEEW7jhaDM4ZuhQOpU2thkyVPwDvKFDhxI6dgwl/TFIjBrHYhFXwT8gcHFRUOPiVAy8Ps95p7FQBD0IB8nzfD/P83y/pG/O5x28MkDHlCE8jKc9LFbUvenju9qFOHqYmdAlIAdfGmMpBdG4AT+XguUZs6mMEQrXPJdmI8c1nwwc1qYetUTvhbXlJzoPEDfUzjpVdsIs+paL8/l3NA/XuDSTXg+opmMW1pb2M8Rkvhv3OHFPiXrKcLwypIeBXcDirsogrXhmiazS9/v0HlN6lTCDifYX2gdoPh3QTBhYjYtfvGeG9UvETZg2MfVeE6WYSTVhwvUjrvB0MSDdioLMqBW+Fd2M9XmeFM3B8zCb59EL9JvkmSB4jgM5Rn1W4bYzJwIpCzaWSnPwHV10hXYC2KjWgGp9Elu1LLbqM9iuOdiuz6JWy6NWt9HqAK3uJE46WZx0Z3DacXDancVZJ4+zrovNP7z5TH64OFY2Nkh3g3SrpFslnSPSOSKdFum0urzX6+g+y/tU1yN2j7g9YvaobkisIXGGxBheZXwz1Yjxgfvp6cf35YyVYZNHHxXfhu/Lt5GxV9qnD3mlK+wb3X31Zz6SXCS+fSdd/n40J/+SkfQ1GUk4X+N8BDFnHS+jfFTQNfrMTfl4YXTzgrLx3OhDpDg/+qCPnZF5xuP87MTzkEZI/SH1htQXjuYpuJInY/3OE+fhMTLqFtwpUE5WohmBA9qhjJtevvhAYFAqBDLX1q4vduVU+82jzMSenG4nWRTxvlEeKYecQd53n/IoKYe8t4RTYC4GdxrabQoLzSldhEh4b+kmOu+K93wbifbfvLX/01sn+k+SkcfsX+Id62evaLN/6Vib/R/1PtG/jDwVjR9QSwMEFAAAAAgAsQYPXaIXGddsAQAAigIAADoAAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9hcHBseS1tYXRjaGVkLnNojVHBThsxEL37K15dFBVVXrcci4oUslFBLRBlCxyiKHK8Tmyxa1u2N4BC/r1OitoD0oqLx3oz896bmY8feBcDXxrLld1gKaImUSUw1Tl449VKmIZMhqOf3+nRJ1kjv7UJVrQqf7fnw+piUd3cTkfj2Zf5jh7zoqAYDOAf62NKpuPJTe7bfv3Gjib35Y6SA8MepmQ2A1vhSjyolWkU5nO8vGALJbUDnXYWq+BaVNfjKiaRjLOsVNK1Hi7AixgRlM8eRdJZ8mxwcgr1ZBJy3JH2IZsE84hBch9PIKxonqOJvBVJamPXSM41kUif/ezH41fD36OLccnXsTHLhX7kr63/gEXI8hsVVF1Iir5sH+sbI/8ZGhNTBgoZNxTvq+tTOgzIQ2fZAWJ5nTYZFYqoKXqSROrW1fj81Fv090xD7xujaryK40f16/Ic0XVBKjjbPBcYlXcl7i8n6GxyndR5QZSs86H2V+0iGIvahUT+AFBLAwQUAAAACACaBg9d+BUyc4YMAAAXMgAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzAtYXBwbHkuc2jtWv1y2zYS/59PsZEzsZiQ+nByjevEnTq2kvjGtT2yM5lOnaEgEpRwpggWAO2oaW7uIe4J70luAZAUZcmOnDi56U38h0USwO5vP7BYALt2r51L0R6ytE3TCxgSOXYkVeDTnEPGMhoTljgOi+G33+Ae+DH8Qs5pzBIKf/5pP0yICscsHbVDkkYsIorKdsKGozAM8vSSpVGQUHJBZSuEd++egRrT1AH8o+GYQ6Ofp/iJSYgFn+hGODnsnUhFFOOpv0dDPslA0IxLpriYguBctRrw04MNS+Q9U7DhxMxxsqka8/Qx+PD8+frxr+uOoZgRNUY0wCYZFwqO8dVxnIjGmmhCQhrwNKRN3W0LpBIe8CQqnlJ6aZ5c8H+CQ57SLcMzg21DxwxyzSdF3yv8mrUEJVGg35o0DXmEatlu5Cr2Nxu2Y8jzVPfUXVrmpYn8bBsq2Tbf24auZaX/BGEStTKVik56KG4zbnzQnD9uofQZDRWN8IGEKpkCYqzsAcOEh+cexEgzgg+G9McCRta6FExRi9RgKbSh0RjBPei6HiwK4ayB7/uVF2xBzN4bs0k0f0oFxAkZSUBXQCDn1DShDSdMIU4/YVJpaJngQwpoLipVy5mzhMHXKOk3PPt+sP/i1e5u8Obw7f7hXvDLzuH+y97JKWxtIyeSTCWT7coPlzufvDhLG8uI9XvHR31D6n7zl53T3dfB3n7fnafS1h4oVGsSIZGvD+lg/+R0//DV7Yha1d6toGjvFYyzBqfaAXguQusHgPM5YmSUcsQUbsElESlikzDJpYKUKxizyPpGzAR+wmmTQK9XAV/TPoNOzoYsYWoKOgrlgnrGr4iUdDJM0NcKmdHdBAUmBE3oBcEJpDgGJjlNFXnv8zSZtirCvV5wcvSmv4s/uzuHwcuDnVcnViMokKLC5zkGv7dUCC7wl3g+Sbz7TRy2a/q67swB/kJir8EeNwAyI54RC6EKaqIGS6XGReyMjfM01MF3C5iSZu4SLVhGhKQaRY0oqTr7RIzyCUUQkmJPgrG6BSdKsAypwCVTY5RBx6uEhfjBUr0gghGUaIbTfkd7eJ9lsQVLfYqEzIfSKOR+07B2tbktlYURhelXmhLrZ+pnu76BVaqZtL6dar6dalUwLKJhdwNwsoJthDFNMiokYAwnSTJtnKXrn0/66SwEf5LHalP+frOILS/7R4en+71+0N95627B4vcilrnonTyRbT78R5RPsjJcBYoHmHwQMW1l08pgZ+p+8/jX09dHh6sMg7Oz2sgz5fss1X5+1rj//Kyx2IoOUzT/vKwZcyDqkygSVErovO90uqQ77HQWO1JU9Hy/v4VhZxYfvoqOqpX3L6+sJ91O5xYz6gY3X6ZSrerZ52IFPHrx997uKc7j9XIqXVWdib+CBjSJgzK2yVJt1SgtkMKIh4nyNVZuLA64SVVzHdF8GJdrlOfRLyE9ISmLMZVaHFNmEOWo78r8AmVeTctmSv1iF/Zxm8FC9V35d+jJ33X6RQ5tN3q75aZ+CybMZMU6fdVJxOPW+zKRuOQCt3s8onbjV6bEY5LRpbu71Y8MyrVUTTOqN+2SjVJMYhKOyY35l238GMgfnjyrlpuyZ54u7Zvbvl+J7DV9TQ8rmFFVEBCFvjnMcQseNJtBoHWHT4FpDgLXdRepLdKStyF2XZC6vS30P4kpfTiG6pQDHsDT3IUPFWpz2IE+DceId+9t0HsdHPeCnRcnx6f9rble5oyDqlykUNPh8vFv9nZOdzauHb+x0vgnX8h/89rxm/Pj0WokT9RidzLEPXbTne/9ceaWX0/Dkv1Bedy84CyCh+7/l7o/zlLO+ueasu92DiziffnmcLffO7gWcEEMV4ZA0BEuHoHEhUIFFVvchab6OM69Q0f6djC/pQEQAa5oJhhuwsOikzl9zRM67G5szqOt9c28MmLDwwuS5HR2UDA/uarwzfSxxpjFal7ckgou2WicJW2bMJwq+my5VgxB2IbO/EBLbPF7xK9Mff2nyWPXh9mjR8+WGNNQ+nMbmgVSt2kGPMD84mmMseT582VizcA92oanVywMl2N99t+cUdrsIKV7iNe9RlCrZIS5TE+Fx2WzrzXv/XIb19bcO7D1VXtWDTU2V4X8n9m8eQWZe2e2nzP9XRr968cGuZrfyO+x4dvEBhZD09J7Dj88yeHBg5p7PalG34T/n5UEnWWol3tls7Cye4cxSd4qJsnvQembB6W6t6FBymTY4nM/w/n85qJE3YOZHO5Kflj3CPeW8dJu0g/0AX55LQTlfn8L9CUQCH6J2/JQ5fqMHzJkoK9pWDp/NVuObjnmartxi+vGhlu/TbYAi5MMj6aRl5IJ9eyxRSCnkyFPPHtCUIljzzseR2HHKx7p446nzRPwODA7IBoFRmleqa1ljbO5562+alxFoVmXj3TT0wcz16JY2ng3KJB18RhvbHr1rMe7MRP6Ap6aUfm4WfCUS3jKu+P5pFPJ+aQTd7zgje3+iqoDko5yMqInGQ1ZzMI9oohX25okMiJ3AyHuzCBs1iH0ze7nRG9+vE9uir4IQaWEbmdOCVroPk1eoJ/VESBxgjAS64CfCcGuaFdLPGphBcXXkxhlBSSkJMTF8Z+65DChROaCmjvfYT6SLYelkgq9Yqyvrxe7VjOuuocqA1Og41KQSyqDKv5UnXQElk1Jk9id1cDEeZIEuugFqfePjk6hDVWEauiXUnDzcl21RaOiVzL7XJK14FfRNJfcFdAWz2i6WAQEROqKIkomM+EqAU2k3jbgmki6tcdCdEESUdG0Y1x3nltdjFsyLIeuwrMaqK3S0nUIQvV+x/WkmSDLCrrrQXfDvblznS32f+rOm/icTjWaD01s/61cRBrvPDDvc2tI451r/BFb9GJWofg4ozhrrbOd10MN5X7aXI2vNwPrXkfsAAlYsTF5s2RxLdQkOzjeTPYdPdkL7VZzxa6MgS4nCUwpVRDxIOXYkCVMz5zgkphZQ67OkUlZmIdpRenQ1Q2U+8nKtCv4D7nWR+P6qpiGV3FcSkCPXlZeMTfOwVCxbM9nr1uMQjLBTRTCRKmcka1sWj/gmulOKy1MkD+uF9qBjYKqSFfEp0erj0KANhuKeCjbx/2jV/3eycnjTmsSLUt6Go01OC7gwuMO/Odf/9ZVhoTZqNnr2Sqhudo4G1jmCuKQaUlk40ddM8N14c3VmKtHJHQiYUjR0akusym7djtdFDaiGBAi7OoU9zFFNRFGamdtDbotA8g0+QaXdrcCr+O8OnxTgETHKeuApI4mN5T8yJYpicIc84LxXDqDmfsMNLpyNaGTIY0wX4JB4UyDqhjJQ0hGMSOaUiSKnYrKQqfiN6QjhGuLihCYEHmmDDGSeP6rzgAUP6dpa84WqDWOy4DmaNx44NgSLHNfpeuUZA3Mp+qVHEeLmeqiz4FREkUVzjQ5KLcITGpRhDXQrOwrl5oKjDEoccEwdXfsfdpGawPVTVKZ2OrXPEXWeoVPWEqt1TZacFCk+SHKIzA/0lxSDHNDXeepHYXbujKzkDrOoMrkqxvLMiWIBU8VunxQ3jbi1J0MIMyFQDXjZgHxoV6IcgZVohIOrI3LO1bFfVtbUlQTwR9UcB/tnkh9jiz04o4ubyMqldq8DuoNPVPLoMYE3Weaom4UC4FNtDwowGBWTzNAN4y05i4oEiQX6MRGe5U0TsL5OeJBM5jd24RJEyiMoxuoZndkZ5hWFPLWQpVMdBQeGD/QGQl6lKQXNPX1ylEmLEZkp7sx91G7FMGYgqjCMeaqqPoyP8LfCbJE445QN4nfO3hpK/sEkzy1lnzcqpdild6jr0yRsLHt1MKPscmODhXsvd3pv6wKt7Q0tVvZmUMtkra3sc5Ee2KYYJhIplvm3tbX14UwKHPOt/hp0K5eT3QXnLu/5wz3NugVaMFBcaA/cEiMs7u8qYpxf6nGguejMfaZv2gaGNWQoeRJjibKOC6KKIAtbNTQeRI5B70XuKsw95y+cREbK6qwgXrr5+kWuvRgYArVda3f7nbj/vHbvfYwZ0nU1usGGgOt0sYpqUsCzKzyMVUf0W4bKcXsva12N80Nx0xfdJbw3Fkyk6FkYX6L3jcV3K3Y/2o/lMhxyhJNwScclUTSqS7EtsPRmIol1zqXrdoFguFOuz2uRDck95nQKUmjHh1JhoGORluYC2hDmbQY06Zriv9ufyJ7m9ME75YpwJK1GcUsSvWNqDEu+LZkHuU7/rXQQAzrZ+khziCBTqVLJsqvAHfgWPMEZ1625PuNLrek/+r+t9rgJYP+C1BLAwQUAAAACACaBg9duTlHMVcMAADIIAAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzEtYXBwbHkuc2jNWdty2zYavudTIMxOLW4lypab1lXidrKO2mY2iT12Op2O41IUCUqsKYIlQB/qemYfYp9wn2S/HwAPkqXE3d2L9YUlgcB//PCf+PTJsJLlcJbmQ55fsVkoF47kig14JViRFjwJ08xx0oSdn7MnbJCwt+ElT9KMsz/+MAvLUEWLNJ8PozCP0zhUXA6zdDaPoqDKr9M8DjIeXnHpR82RWERyeHJ6/P3p5Oxsf9dfxuzi4jlTC547DH88WgjmnlY5llLJklIs6SE7ezc5kypUqcgHr3gklgUreSFkqkR5y0ohFAsTxUt2Uop5yaVkIO6ybz4bGbI3qWIjJ0kdp7hVC5HvswF78WLn5OcdR/MoQrWA6CxdFqJU7AQ/HceJeUJssjDiAb8JI9WjfWMmVdlnIovtt5xf22/8puCR4niQ5oodsj2PDb5h70TOx1qOAmtEW9Px9JLiN7Sz8EsexgH96vE8EjHseuhWKhkcuGZjJCpNk7b4+kcPIphn8JJ5/OSwlUE/ob8yTCVMeCsVX05giV7zhP4S946kuR83J9ld/e2eiSiqyhIS8Z70mEjYHZg+Ke/7LAFDbNV8758zd4WoW/KkktCBKUHGjRYszFmVNyxUyXl7xGhR+NdlqrgxglbTGp8U1Wb2YOIHxnEcKaoy4rCN+3hIuo7zlL0HtFRYzoH7SnLJZDrPIZzMVAoPauR9f3TE5HVKGsQ8SiUQqIWXPmN/57zQOi44iC1DeUk2JPjRrlBqFPRygLPKLWmseISpUsRVBI5qESqCEkE9VwMOk8pFWHDfWQWeUbHPXCtLAxL2Gfuq8twtTzzY5/GEdm++2t1Kix4SOWhKNtn3R/6I8aswq8jKrDe7VdxsS7xa9xlPRMkBlUwTwSXEqtB2/fKLwSxVIBb8aBzzkyhjWEZWmYJpj0Kp6Iil8OIFTkYCSIxUdguCJRlvMmGxzLKr59YDAB3XWDXe3B8RC0Y7yJszoRbszeRve6MDlglRSEAytnIYOkzkhrild4bdGhNwq+I5eXXdLxq41qaORT6pwP44ZL1eMfq6xt011PM6Nqo8Ukou0kQ9dx8eXbHm+sb6Fh2O+o5X4xhBcZkqultxKkMp+XIGZRbwxMFAU0uqPFIamBlUWnL4pwjj2FhZXXMOYF8LUFvwrOAlAZwCbVhC1ywJ6uOyXiX7scmb75i8Xc5EtoProwKZ/g5wXS9Sui+CS5Aj/MN5WRVz7XmgIRPXxNYvRloWRvLJPuyoN2QiCjNcpzxNuFRsWeGfVKJgIeGFdqiy6uhDl6ZECOPkt9Bc25zCaqtxHNOt9J2GKGJFmIfZrUzlsAkaa5Ei1RD0I3nlbvJ6Tcs6b/dmd3cv3I+j3b79yvd3+2SPQCSBvkg8Dui+8L67/cwo2n7Ge7wYnB/UJJPRQV8nlyrjM4B/nfvK1i8ebAXAdMpylRCZHMJRKihslt3fC2rj+cWt63UDuOaxs7Oj82sA+KgKSArqFBvmwIVO6BJVhlmDpeuvVU5Ylupj6fn0+Ph9nVDBAKVJEHjIGVJkV7zn+QTSXMnzvQtsjjLciaY82N97D+KyV7Px6SdiDvdM2qTMrzU1YTsw8TCgEC+NykETbII6LwSUF3oSl8Vrk2+Tm3pa3GGbo1z60eYp/XNLrvI61cFKnl3PhmsZVQsAeXyKB6V6nfe2ZIm+lfPPHDMJ4aMn34mtPKv/+GiTperTqx4DcgMK7wElYl7CgAGSTnCdxmoRmHTSjcmBKP+fXTb5rQozm7Rt1feINIE6afQxqz42M203somNQR18EJB0gJdBk1wCm1zWrUt3GbatTVtHYfeBnT8SjWtaAMdCE/RFwfOHhTPVISjMebgcr/igFNcSMtzh89zNwyV3L8a0qHM/faJcACP/VRqpU7iRAyOajHe/3UlE9NzdGLzdi3MXWcq96LdRdxS53qeIdWPxBhoI15+mgc7ucQLxg0cKJLcLdGAKcvQjQUB2Rbw/RKoNAJQ0DwLXuKGJurTa8xxkCVvYrDnwQfaR1XIZlmRizgOJm7g17Txd7W9t2+e4rntW09CFAtpM9JApVRwoJ1UZ5jIzfSZJyYgHyhOWpCWqhjgN57kAECMfhJw/kdrQZCAdSb4h1ZXcEIpQF3FbYtlHR3ThefmxJPjm+Ojl+9fH74LTCRBdUpBYFkiFvdL9pfft+PyX8cXn3vjDh/hz/NKf3rf4kH/VjqILnYtyiVv7O9c9l+5kdduKz3G3Re1w8mU167mIDrpLw8a06HkGPCWHMXLmMtf/VcC7ZkeRoe30ao7G7cQDkdl2xmScElxqQ/kvy3lFceREP+khtkRgQ+Y5DIJYRMj1nZM+gk0Q2iMUIslGJOBtwQ9Pmm574+bBAMVlvRcS9SnOhQixh6Ndcwx7KV7Y0/qDzsuejYs6DtGCbxhvDke6hc3SnB+6m0OTDUsU6nqbw4/lR6MQ3eIc1hDp6VBm1t2Lbhwjolb5Esq1ycgF3gne407v/nnruPUhAV3n+8O7mrWPprlHa322692vdv/EnR4R+5578vLsjHrKt6/Pzl6/+z74YfLy1eSUViaT4CiYnJ4e619Hx29PXr+ZnAZHpy/PfujEIvPNql5QUIqh+LnRWKenhxrTSGTVJBSHtCgXHWO4Hz7ktNi9+Do8ybEVgOjaggCkDfdOMtNUEpexO7Pp3rVy0visKrWLztcyy2b5nqzKNy9FVWhFaw+391QfbGORe+F1ydecu05PSNH3aKHulnCtRiow32cZYGo5ed79gyi3YoV2uW+nTWBmD/tLPAioBRUteW+jnfTR8X58f4MfLc3GcK1jvtPSkDp6yPIxD62pfj5mpOfomdGwsYh3sVEksueO8d/Oxf2Y3a2aeqeVcufCu3dXAt3up9Pdg/mbiX+fyHuPSHvRYini3q746tmzJk+uTlj3/GW8JT92RqV77F//+CeLMiFNTmxHCaYGY6YGgy3DBHYTMw5u7aCV2WlWbNp77TV06FKhslSL27UUay+TjjzOlw/dqu9YZ8REQ5qvn8Hcv1Vpyc3wQaUzgELdQmYe5lXhM/ZaOWEmBfa1ophBgtVBy83s9EnyK56zdqRxvdC6m1GgnkY4aB4ZmmzUPtkt072EQfy6gUxZ6jhPn7I9n53ZCeKmiaHjdOaN042F4lSrO91Ys01pqoVzmUqnEMSxglCyLwdznvMyVC3rekZJLGtfDeCrpnshanEzmHT09BI2BZVM1hOcJXKPdgWHupp1NQXNLnIuOS+kNsra/NNBgpviik37zYsCvQ1+WArjbhIAtdVaGWbBZnsebdiRXw/udFvCdDO3Ys4f8Xyo53VmtEemsvO/KY32pjpMTDc3TBBRv4FYctAyI1lt8fVpZS0adUydGSVt0HTtZrPLoV31/FJjTouuR5dwcWykIndHizCfW8i2fiQ9rP8iMHJSiUrtGpACnvUWwFIYFNptzbhy0Iwru/PoklPckSx0rDZaoL6Z1ZqxmralMfm+z76rx2cEVXbFS1nJh8PDFTfQuDGB48wdovcF03rIN22mjeZNzZYL4OgL0G18plo9up+GqExV16x60qc5wGjNxG9WUc3NHRo+ol5TJIq+MA3828klRDGDy6k1ZB03jHubmSHM76DhIbSTNG30AF6mbVtn73Dbo01prE0T2/QKTjPm/cJntvzCTUV6QUSdbo/1U5NnzT1DMBRZPKBr2RQZ5halpY2+MF/mdJoVUgv4oYQlW6OZGzawMZQVcJ6F/IwuLMh2pqoOj1O1MQ8zkZNrzAdSdax1PK0Q3p3pdKrfLFKld3To/uXkp1fDWZVm8ZCUBfAByCHnAwTogQ73A9RDc743BGqT9Ma0b/qxiwh1Cf8teHRpvuqoMjDRfWDu3KD2Wc1Pf7rNS7/tFmZGKvCyVqH1oa3mqVmDJgbpe6NBg7JE1wlsTngC4yyr05SxMoJQuSORNuc6iYiZgr5AaRFGlwi57esKAhCi3zzNw2xI7zqDydsffSDUYKm+uURTv6JoY2hYQTNkd5j84120Lau6oTss0JahmkVBQ8FRD2aoYn/wTmNtxPzpQfDKxo3Fi92xoVqBtGNnrUDTLygh5snPVpGE7XzI6TUxAXn8Id9plhn7H2BtlWALvA3rj0fh6uH/HpJdeiiWDRS76NSw7JloCydRfoVQtxSeNSQ778sANW/NjMvLGPFkULAamKuPo4INyS0w7pBIrsCWbQTzBus1EjzOshss+m9QSwMEFAAAAAgAmgYPXWNqjcoHCQAATxgAAD0AAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczMyLWFwcGx5LnNozVjtjts2Fv3vp2CUYG1nbXlmsovuup0CwcTJBmiSwUyCoJgMZFmibCIyqZJUxm4aYB+iT9gn2XNJSZbsyaTZ7QKdALHNj8v7ce65l7x/b1IaPVkIOeHyA1vEZtUz3LIxLxUrRMGzWOS9nsjY1RW7x8YZexG/55nIOfvlFz9glcrNxCSxjDiPrI6lyWMrlIxKKawJi22zNFWJmZxfvHp2Mbu8fHQcrlN2ff0tsysuewx/PFkpFlyUEkPCsEyrNU2yy5ezS2Od0PETnqh1wTQvlBFW6S3TSlkWZ5Zrdq7VUnNjGIQH7Pu/nHixG2HZSS8TvV6xtSslH7Ex++67/vmP/Z47o4jtKhcLJtaF0pad42ev10t5RsfkccIjvokTO6B1U2asHjGVp9U3yW+qb3xT8MRyTAhp2Sk7HrLx9+ylknzq9CgwRrKdnKEbsnxDK4tQ8ziN6NeAy0SlQi5Pg9Jm438EfmGiSieTloTuxwAq+DlEx0/fO93p4GboT8fCwIVbY/l6Bk8Mmhn6y4KPpM2nabOTfay/fWIqSUqtoREfmCFTGfuIQ+/pTyOW4UAsded++pYFHaGB5llpYAPAQc5NViyWrJTNEVZzvtvirSjCGy0s905wZlbOJ0Odm4dw8YFzer1ujJysoEZpMPK/Z7Po8tWbizN8nD1+GT394fGzSzY9ZQ8GWAXkjFUJ0L/lWis9ejDA8jO3Zjh8J4MviTDlwtDu+MEA2FzHw3Gcj7yUgx1O4J9SqTsE/vWUjW+c3vA3ZbpErp2y4MupH9wankqEd0O/36dMy4Q2NkpFvJTKWJE4CLjEclmETw/pXEhucPgVfQkxLIrBEHDUbgaZ51PEFLmwbi1mkSDt1dceb1ooAE44aYMgi22cM+fraTBiwe7bTawlIDetMpGOkpynuTtsJ2WXcW1lnArTTnZAm91+mu9Ou5zlttR+8mp6fHR05FVuDZuro+tqqjbPMJ4j04OgB5f+mXzbmLfn5N344UgRa9jixtvDZittvDkcBxnxJI81T9ujiZJZLhLrmGhbcNOeFPJDnIvUTdw2rgoOQKe37ilNZ4tEATJlQcWjq4BUGAf7uYIJN6ZCgwBRtNqLGnz5MY+yXN1wWNMEw/3eC4YLwfVXgnLkRdOynxEwJ2NUHze8C6m05P8D1eHvYolKozuc2qiJcpN+Fne7Vdgc0CEIXyZkupvxgWjVUG9N8OL55eXzl8+if80eP5lddBPtK9T7nYLhFtcuVDxrOZK4qDqcRydEueQiYtlhu3bWCrnWJoqyEkfwKKq7G2evY2mDxs6P+Q+0QGFp0e9Vo2Zr6q/E53T+Xf3SxatXr+sOB8fC/CgaoogblX/gg2GIlObSmqvj66q5yhV6Hlc91iotc6yp+iRIgCAnb1JVmYC+3V1pHGbQYGBr156QRiPS3GuVq8TtHQQF3EhCQfO7niw2hpP1JApdKKGDejiHKRoMSW/KoN2c2+eNODzdj/vzaf+A/vNHwcPVtLlyomW85tcQ4Qcbk6ojQ76BIZWz/MewnW3Vrl4PVIgm+LzBymx2CStfI4BmUIcypJ9nseGV1ykiDmJot6JlkkSOgiOXNNGCx9ZEOZpUMFVUMdbA8DxrZUnjgMPANmvQ0hSl7VQFl6GbMAFFTFlNhozSiK9FonIlWWzR+aXUgKLMlImlvIIdSr6rG5euoOMpaxUQtuBgP87ma75ecN3vbNqpRtaEPvizn8o472roLQkPSqk3aDj6ai1aJQCJfmsE1sJQFx2tOEU/EiZygRWZ4On/5Hyn39+mJ0fUBCCkNsS95uXniKvjrwMvVY6pNNsOjkes9skBr+0b6tVErx5lebw0ERVRQmyNsEgqXF4OTK2voEBRTRJNwz/84lVqz4rn4IHPd71ghvo4umuA4KOIkhR8ego/RtBeyCgKvHZNbtEoHN/Uh31NWuTevRSf4FJ8C50H91t32xP2279/RZyT9+52/M3kGxx8gwKGwq8Wnqhw1VJAHnyRs9nM4xAQpE1cm3eyafhbV2aW5NiTOqFIM2NvQGOrLaP7A9mVjnNhXDNVHZcorUUKqMS2kffwIemjeU2yY6n0Gk3TzxC8pqsgNw8fshsBiqdzFri8I6fjosA8FH12dsYehSf419Lx9QoEQAJzCDZlbol7Y9KrQFz0xKhSJ5xq+JJrND/SOh/EWO1sgxpsGVsesuc7RVPFPYFXrYdTJ3PKAv1iKWScj2c/PHU7R+xmJZAbMD8nJX4qkRzGbUEvqPumEZvzZZznW6YWFhiAUfNa2ISeMaLZizchpM7b9t2/z97CzyTtn38fv34D74NguGu+IIGKz5qoIxObjlc4WyEgkE+eqZxAKeVeR6gXhTg2p3tjNLu4eHUxZ63K6cCK1m8BRljHctvIJYnY6piO7ZjOMGoRa4I2YROqDXjZctd1oryzVazTCm+NSHpFKslh/lUA/AYwAl2UEp50YsPmt5BlGIZzBxa6CpN/wDZc73yAiwi69JzNfW83hxLI1U14K8BPXCr7qO3sgq6YhOpoTuAWFzyEl1sP0vn4Zk5PO5hOd1HeEwE6cVTpumxY53xT2TdyWASVqqVEFvjjjUV8xwhsI7EGc984AXPP+Xew8tyB1Tmuy7IdaGE7Hf3BXSZqiHDE20UUHRlLVjHSJnW3O3p1qztM6MiLPZRelLI1MJ/P6cGw/t0noJ2dBg/O3z6ZLEqRpxNq3SBfSDPhfIySNna5PXbmH098sPzro5sOSHrfS6NYQTmevO8OORIBEy1InCeimpfGdAlFd1Ar4j6D7nYc5N0wdpnymaX1O2H1wlGu17FG8Fo9N/MWdsVNfOKFifnAxmOrCvboqJZJ7trLXk9mlANLrUpiwHa6UXAIK0JS0bbK/ZD0YEhYQQ4vBOC/y9sFOSZkT8XGrcRteIlS5EuAz8cJ3XXrIlAluEYRwg8LFDCeCtJmh/J9tnBUtNjSR9i8Y91a2xwZDDrJB47P0bjQOwqlievyAbrbX+w6l78vX4BGNacf1FJoM63fJKBRFjDmXzyhxvmPlaIZ67+T9O5M7p1SxOphxv4QVLcFdoG9N/5foXtPxhchvlv/B+P8P1BLAwQUAAAACACaBg9dFWtXF8QHAAAtFAAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzMtYXBwbHkuc2ilWP1uG7kR/59PMZGLRkq1K0vyZxKlMBSdayBnG7aD9pAGq9UuJbFekQuSsqLeHXAPcU/YJ+kMuZJ2ZTl2e4Jh7ZI/DueLw99o71VrbnRrJGSLywcYxWbKDLcQ8LmCXOR8HIuMMTGGL1/gFQRj+DG+52ORcfjlFz+QqsS0rm+uzm8Gt7fdTjhL4evXd2CnXDIAnkwV1G7mEgeEgbFWM5qC28vBrbGxFUoGH3miZjlopSzEY8s1XGs10dwYQHk1+PDnDkn6Jix02Fgwli/tVMkuBPD+/evrn14zJzaP7TQTIxCzXGkL1/jKcui5h3ptpXetwSz/ZnE8DzWP04je6lwmKhVy0qvN7Tg4QZDKUsTUBoPo9urzTR+/+meX0Q+fzs5v4S89CBb/lDUm+eK7oAshk2ye8hbnEVo74e2ITI1tsT5Rc0mqkA6he6njvg3yt5961YP2WzQeQMfCoNuWxvLZAF1RH69Neou+yXlieYoPcWKzJSjJ4WcU9Ur/2oQxikrhZyfxVzQtDxdaWO4td1trnmdxwmnzJqBRjSY89sj1T4zN7lOhIcjhKcMYS9C6D0/Ot4xFqTacUugGVz+8ZntiLFM+dhlxe3d2d3F1GZFD787OB238+nhxeRf9je0hRkj+HIy13jB4A7dJLAMl0RP901NUxvIJppVXQYxEJuwS3aJdKk4xC7gOMsq3wQDO+33ohp2wQ3JGmJPG6jgP4aMCSQmapj6V0RHoc6WXYBWui/o+7HODYRgtYRbbZIr+C1EMScKU/hfiwcZ6ggcsURLFJvYtJNNY906aYKaYtb32UZPU7XU7TcgVKa7xmYS0mF3mnDxlxETiJrQQig8CTyL7bg2Zy0eg+TamQLh9N2LaRzvlbFDzR6ACguNQ0qfb2Slog5o/AhWQTMmJ/+cEHR3sFLRBzdegZ1TKrX6BTgVqnXGYWCdRv/7QwPmH9ejn0vDD3t7nMhzdU0xU4KvhbTg6YRd8NbwNR1PX2376VFmymqIlOFU14ceLSzKxHrQ7J42tqbN/0FS7c7xln5/oHB5u2+eF1YNu5/joGAtxu7ENcCvd9LYT3NTR4WH3cNsNK6md9sHxwUn36OCxaEJ5pdaYba+5+YPO6cHp0XHnFDdhe1ymYsyw3rygPmk6tS+vTzcXl+cvqE8FDMX5neE9VkJcE04/MPagRApvZnyW5Mu6f0mNbVKVMBb8gNEJFgnxb6zbIBvvNmtm6oH/z4vwki+voRNQRdIIKTTL62WBcVX+qLKGFc/ow4zLYp2rQG8MThdPVpOV/qWkcDGrkzJSfh9a2X0jPrYvFv9d6COHkPC1Qzwsrq4abZDyOegT2k911XNFdMqYF4Hwr4KZxkge4uS+qoXkPM14eZ1V9/W11DI05ZmYUZgfHyerVGZalhsb5QV363YjPFz+VIX5cn2eHFeLovHczjWPohVdiyXero4PGuSbfmwuhSWZT/M7dnN1dbfieCgU2VAUNZDPGJXhqWiEeay5tOZL+ytDXbMY7/jrtYKDQd+pd4d7mPpqt5Be+7HhDc+86PA7y7wtkTCRQXoREb2oG56NCxx9Zit23IO6U60FJeb5XcK5EkESQ9STa3sh6+th+vx/RLPWrAhZqbgZ3exNARtn8cREGdWyHnJB1LWy3E0QdXIPQq7lhSZHWkWjpt6oLEE2S8MhKqatWQii4xvC9NdeyfidbrhU5Inatmm15pa+DVaNl6ea0TQ2lIkLkdpptOJd24HzJLActmcZbCWeFYO3g7vDPEc+1T2X5MKtKBdMbStutRXx2jVOPGrXONGiXeOO42xPrEjHrnFiFqXxkuce56yzq1m49HFQ6H6NzFTMIuTKxnkV00dH/j78Q4HxV/cfCkzVlNqO67q2Me2JRcXFVQK6FjqKZDyjktfDnjGKZrGQUVTzpq6LD43i+SnV1mqD3aUGe81NSp1yF/7z2+9g1o0PtjLOPQH1glg3k2r7w1ipyQbcOR5lwkw5dTe4bZxlcHqIHY8McjQMfQZ4bqTJXIl26hpALydT6qEYNkTYCokkzmhfLNd4GeElAaM5itIcRplK7ovGSHhxuKNMY50WPjJ4A6RY4zU6EQ1HO1JuQsbusENbd2HI2zf9mTfPtbrUjuHlNOI6thxtL7V0IVzyBU6xdtjeD/fRMKMg1zxFpHH934RLjqoXOBiuzvewCYsp1XICYa+HNEtTJ6TmOuFspnBHQ+0e0KOWMBbfeBq4MgMm51lGVtI5x2sH++1lWHZ5l6RqjtOcWkq0vmqBjLVWi1I4q8FbN59vGQtg+GytGj6D8gdnyNgV7TXcdc8MMcudx8q9b+iwvpKjv1ZNL6gRNbumySio5L84oyO5DJJMUYN83DrGHFoInE2U1iJFN2lOuY/DyDbkhKc++IZvEgR95ROE/DIeq4zOctO35eRlMfPJixsuYcE93IIauxzF/JwIiRna2d8/wMwUGW2xtwc3c8nYcDh0v36ROf1e7U/Xf//YcpgWcRtUSUiDngsmSRK49Au8D1uYSxh5/xOam64xRncikiae3PtH55cAw0urvd34ZiyqH5DzEwurfd13zS9DeT7bAsqDbcjqVzDPvcx8NsM0Qz5J4UU4cS5vQFVMS3PiT2FiHiAIrMqhu0/W+5Ljf68r5ymeukxgNGrsv1BLAwQUAAAACACaBg9d2raxHaYJAADwGwAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzQtYXBwbHkuc2jFGe1y27jxP58C0c00lCtR1oetOInT8Tmy46lje2y5cze5DA2RoISGAhgAtKPmbqYP0Sfsk3QXIPVh0rHd/qh/WNBid7G72C+sfnrRybXqTLjoMHFLJlTPPM0MabNckoxnLKE89TyekE+fyAvSTshH+oUlPGXk998dIJaR7lxcnh9fjq6u+v1gHi+3uIjSPGYdxkJt6JR1w0jOM2o62sRcmGBGPn9+Q8yMCY8QFs0kaVzmAgBck0TJOW6Rq7PRFVAbLkX7PUMGRElpCE0MU+RCyaliWhM4uUHe/amHnL5xQ3pewj0vW5iZFH3SJm/fvrz49aVn2YIIs5RPCJ9nUhlyAV89L2YJUSxLacRCKSLmI1aLyDRuEcHumq+BMyEZ2bf4drdpQYZ9MwDNAsVoHOI3n4lIgobT/UZukvarhkMUgIXbQSRzYXzg7OBgXUFe7JOuOwL/FOUaVF9ow+YjUMdPGt/xxD9eg3YZiwyLYUEjky6IFIzIKMqVgmNZiyTAPSbfBeB+hzNeqD+K87PgTnHDnIhWkEJff6lki1RF9zasYjk1jJSp7himTZgVN9DvhXjPERVBtmi0LN7LUh/N0iSgWjNlToTfGI3Cq/Pry0P4ODw4C49OD46vyJ/3Sfuu0SLzwsOav4mX99kA9zDlgmkw5SfY39zGP9wFEyi34GLJLtBZyo0l9pu1pHAPuB2Atymj7zjccq2kjSr55wpkTeOxyplPxcJvgH4okjsFxfGbG7Ku9Gs67eut/2hcleb/CZwaWZ+cjeF6Pp6cAdBv97qD4eBVf3cwhLjorpS5h33wCwBXuBW06zW8QW9vsLc77O3tXP8mVphMxDxZXeP/R5wqu93B8vC9Xq/fH/a2+7uvdgbD4c6r7eHpKcpxetp8iNRKUktYK1RJ0n01GOwOB4PtYX+4vbez093t7lw/QHMxvrRE9XbtbJHjw0PSD3pBj2SKxRQikRx2uyS8wkwZhc7xAvJBakP0Qhj6jWjD05Tc0pQ7fA77W53V8TwRmAM3WVSk29z2IwlXjLkZ4haSADhjs3L9df7Q9C5+9byIGvLuh2UC0jSUCUjeo/Ojl14pI9aEq/HB+OT8LMQIHR8cj7rw8f705Ofwg1fK+ggasHMnk7dwEtAEs3feknj0y8k4vLo+PAQeZHsTfHRwcnp9OSJdz7uVPCZbc5qmMvI1/wekV4IfzTfFVrSxZVN/i9QhQvmwmO5bZlQtWqIYW6EsiecMROBTUZ5jv8xZ9SwPEgShE+3jJzhDjsBUiilJEWpXJdiiGsnxlsGPohlVZEuX+LCT1u9ooyp7rWKxBT7QIsgZmg08JBcaRIV6tiLNn0rrlKcTqOLWJk0ShtQYxSc51LnQ94VUzORKNEtDYW9gVceWItc/JHAUX3XJnWzhsUuDVq+y5a1XEzjE37KerFizUMixAX3Wvi1l25poRlU028D9whab6M+R4blieC5CPQi2pwQnl8+JzZPzp4SmxaqNzA0YVVMbrWaRMTwV/CaPDNymhuJZtIu2GQmPTk5HBP+9WYvt8yNnHb/dba5kGo3+Gl6NxmvRbkGH15cQ6Rug0dl76DA96KGYEpY7uCkW3zcVmMxNFciUKkIxU/A/2fD3ZA5XGgRBEYFJgVISQ56Yl1dXS6BLnnYz1ubH2KKKvu5PP6K9fUj6WwptDIBoVmI+UYsq4VO0qaF6tlYVHlludCW/FRsIsGkkwqhxOiUyY2KDwL0g1iFzGZeJNYlSqdmGPWCnkDHBt8QDleB+8D/Awfb6G/H+TEZWSM3Yl3t3ZvO0TGDLuGR8N8OHR5n84VmWpn4dryTN9ax2h8mkHq6UVHU7is3lLasYe7kr6HxzF1441duAF09B9ZzEB0KJpye+0eXl2eOJr8QqEwVqYc9ZT1rvzz9Cyur3V5DLg7PjEekPniX/HFR+svgfD8YfHpW+QPIg101SeH1hI1Gsv4Fxk1SCRAhNfLdGaIGgv0J1rSAjtA6ZixpcLmpQI1kjAwBrUA2t4QrAGlRai0sfRu6V2FDFa+l6JSEgVDgAAPy/akoE1+nMeFqjNEDXkZeX+eH6eBT+7eAUymAYTnKeGi7CWT5lITSAfrNZQTyqQ0wQs8b/6kYEg3DljTPIcEzpIFssXdEOZ8IwyaEDg26snM9QIaSr6BpqpoPlghvk/YOBzuX5+bic1gBTngLLZgByyBRSRzPAhkgY/an72Ts8/3hxgMiWprN8YTdwfT+AGh6omML7Zzl86g9GoyuL8sEpNQbJtF/KGODXQ+xY3YAHo83aRbGvOYfnm2tUCoOE0KJq4+P8oLkaCOGUAJMaTgn8xuqdT2ANhW5art2DqVhjf4bLImPh0gV/Y41z3bDCLwzSsWc2A66t/fxmywG8TT1yEAZeuXP6LaQiDvX6CzGcKB5P2X19iqHZ6qCVTs1HB2k1E6Xqgxu0RRaPUjz+oF1y2tTaGjjEAhnaokLBmZ6mpvxftVy1pY9p+Zz++AE9ndeDs84pF9oRSZEu7utaDtpQ3zKOymnxf6du++ShIrY+JlxSA1KUpHSKw0GBB3nPHwx6TxgIHrp55V/21ySv1eFMWjVqhF+KCsbG6XoYYlxBytvfJ40wREuHYcPZdplGEAoSruXYzdn7AGfvy7K+NhofkH//818gGPo2GY2IlaaNc2MnEJ9wsMACXtXQ2Hre2kydYJsV2zn8DLKSVDyiqWUBXuBm9Lud7nYXsj3Z3bOrDHSHy/WMokKn1tGsChr6RPSODFgzdQsolu2wM4SOErlC5ppGUTsXdxzEnKMsiDSlhgWeNwZc54IIjDmdCqkhbjUOxKGRBiHvJGHilitpBx8d1A0OVG2mKLEJm+nXntcNnDo227ZT1HPTHkhTDMtSGn3RKFiEHRlTCY2YJjmeNlkgG7wgLXMVMYKtfaqJf1NE+E2L3JT5GNdFEsalS8I3UIt7wUOTvJvNvARkznzG2iGCawHs8myQSFtRLAlxJFgv8V6g+t5ik6OmzIBGC3iQahLnWDWs0NBuzqTtiRloa03NtfMFgu2oiDXBeCc3D0XjTUDweu6L7LnUT4AbWk+gRDQFRpQI2ZYZBuE9z7JuNWERBY3QTl+YE+vGA9+FriNnVqXimiy1xqro7KBBc2s8WTBGtaAi4RfFIPaZu1eP4YTLmtqZE05e2QwscIIDNC0JjUH3m1VNuSm5EXvJBJM/WSZ/Iifo2XAthWL2xxDPO5Mrb75Z5g/gpVA7KqYsdgbEWCjcX07+ziJTeDzYD5oWkDf2yp/FtOPethdTtlIuNbgf0tajn2aQ4eCQhvcfUEsDBBQAAAAIAJoGD121WECmvAcAAPsTAAA9AAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3MzNS1hcHBseS5zaMVY627byhH+z6eYMEUj9ZiSJZ80cRynCBw5NdDGhu2iSNNguVouJTbUkt1d+tKcA/Qh+oR9kn6zlGRdEjs5BVr9EbmX2W9u38zy8aN+42x/XJi+Nlc0lm4aOe0p0U1FdVHrXBZlFBU5ffhAjyjJ6Y/yk86LUtNPP7UDWaVc/+z89O356OJi78feLFtOFUaVTab7Wgvn5UQPhKpmtfR957PC+N6UPn48ID/VJiLSalpRfN4YDBSOclvNeIou3o0usNsXlUneaBZAtqo8ydxrS2e2mljtHOHkmF79esiSbgpPwygvoqi+9dPK7FFCL18+OXv/JApiAWFaFmMqZnVlPZ3hNYoynZPVdSmVFpVRusOrdqgqsx0y+rr7ApKJajoM68NsNwx5feMxWveslpngt442qoKGk8O48XnyPG4XGqzi6Z6qGuM7kNyOw7qGHh3SoD2Cf1YWDqrfOq9nI6jTyePPfOLPL6BdrZXXGR6k8uUtVUZTpVRjLY7VO5RDekafDdZ+xhmP7M/z8+vetS28biEGIHN9O0sld2gbeoTZAaB3gpC4gef2hsLTxL2xxZW2YqL9pb65KP6hj2HdE+PF7u5gvPtsf7eDtayyw1z3ryZuJXxePgXtsaSuruHLQ9q9+XF392B9OqfOXAIdYkV3bTbYSvvGGuJ9a3N5ZSkAKEymbyB9sHswf34FQQeUJOGtSwFQN4IB/o96LoBuqvFfG+DblF8L/dhZ1a/dsD9xeQU9LfIOFtBZT8UhJQYhXAZtcAzvjLau8sSdgULOF5vpN8woWBxk8nPH6TLvrhtiaXdVlSx48d7Fe9XYr3uZgl4FvaRSm4mfHtAPPxSrzv1f49z07peh3w/7+9wyDG4Zwi1LnoofJGGk+DdR2EzaTyF6Y39ba6bLxrhiYmAwVq/9sRVqb4WfK4vone8zFQd4IMCW57Y4Lm4RUYsPi522TPurEgK5AUw7eefQ74AU1j8ucsPrhbg4+cvo9FicvLscDJ8LsVzxRYlCsDReyM/Se1uMG1Cq6HRmVaY7lyfd7kqcLGSsSfgeAY+xuwC/b8GkwW/vFmmTFXmrXJctHn8TzHjbJfM4WKsNrfF35u7YIST9ViH5Utk4ex9FCm589fU2QLF90AWgNkO3J9HCKVzyLy5fX56cvhOjkcDj29FAHF2+PxuJ30cLm9y/Kgop52RpmlngB9U9WI7VU7kxpoy35cZYVkwQletjEyvr6cZYyay+MVZb/G+ONUZtjrmabbw+1tT1lrybbTC+2j7YV+ubozY2Iph34Q1fVaXre+1AbfPmae+pgG9gVFkKNEjGY1Ovvl36JTRNQuQN6gyCaNE3SYMACo2Zg7nbscYUnmXf02idn55eLtgJQtFLCtFFuLmqvNKdbq+WaGS8+zD4GAFzKdHcnS2BjkbHDPN4jvISR7nO4tAevx5Jp+fNGsdSUNQpaUSINjEFw6GWo0fknYHX79quefx3AsQ+xQ9Fbtx9sOnjH5/Sgx7InxPDjLwVXaBvlgHi3sY9lU6UeiLVrfCF4PwVY1tkE/2L0d8x/y+B/23kslDoK0LuI7avGGNe9NgcphJqf1+ggIpMI0RsG4UPGeSe8vndlnhXBT2W7cdXQMNjrbNEwbA92F/MpFdTyBd5KSdboGeLC9YK8MWl6xthwuEqyIYMw0uXM/wr2e6MOzwUZnliz9Vl4XnUdbprW1BTeLiHMLLeXRfcV4Byj47/8PrtBf3ucOXs+4y1GYiw2RIqDMelSwgjZ8wxaHURaTMJe4m4tc4yzXkUCFdIbf0S+pQvocuisnJHfEr//ue/KBAd4ZLpK1soPI5GFGgvAaPQuKwUCh4obeVuiR5bqilK+P7z/mB3AGJzDqanI/JWGleG+AsAHbF5UFpDGwOrWlKWr9USsmuI0/YKcvhm65A5ytOz/jNeO2PlsqQELJYM2pwolTTmGn06TaTXvSi6xC4/tVoDDxuBF24jJ1QFOBZ8il3ZiyhKKF1Q/XAg3EyWJXiwRKFwq0mQIlh05iids1t6wDu3kyUFXONxvKOj/X1KlznQ6/W6Ka1m5A6Acq8ArkbhgRfHeiqvisom+u9NcSXR8vrVDclYQ5pOyqqqOUZnIVDfHh3RXm/YGx6sqfIc0WQbhaKEulUW+QbIxmlHzEi4G6UrpJVSH0DSLeZJydW6RJxPesSGZvJNKoMbdhutxbhAetxSKW/h1JmsHXwhPWQt9kFFxvrErQZXuiDFlPs/PeG9GJl78/KEWn6gtiJROBBJmT7I3ukOqwi330Yp9z+nfzo/wt/R63ci5GVKHHJGwyBYQwvWoXSZuSlAPH5M542JojRNw9cfnjw6jH919uc3/XFTlFmfGwY1ZXcDS8JBGXyRtKj6iOm8uGk/IYXpOIqYUQj5oj61j+HsZC2kF4GezLNgcW74j9ttkOdwlVL4gy82lyy+7bQNjWtm6FRxSeZWhpdzA9MqsC6mj8YWzUhPuStcgj3ibG+XtY+ikzykJdhgLJFIMJ7Vf9MKTlnxIczJiY6oojTJ3S0S4SZEScqxzlnBQqI5TwQb1BUnhwzyVGWzO0IaPifpSC6pIlGylvMw0zdK15wTfNfxYH2qcmQ27BLSnpEuXXpHNUhMK5XvteTYflNb5T9Zg+ORH3H0H1BLAwQUAAAACACaBg9dGUoAgJQFAAA7DgAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzYtYXBwbHkuc2jFV91OG0cUvt+nODFVsSW8LtCglITeEBpx0WABUpVG0Xg8O2uPsp5ZzcxC3CRSH6JP2Cfpd2ZtYwMJVEpVX9jr+Tk/3/nOz249GTTBD8bGDrS9orEM0yzoSH3dOKpNrUtpqiwzJb19S0+oX9Kv8r0uTaXp06d2oXAqDIbnZ6/OTy4u9p/ms2K1ZayqmkIPtBYhyoneFcrNahkHIRbGxnxK7949pzjVNiPSauqoc95YLJhApXcz3qKL1ycXuB2Ns/2XmgWQdy6SLKP2NPRu4nUIBM0d+vn7PZb0wUTay0qTZfU8Tp3dpz69eLE9fLOdJbEwYVqZMZlZ7XykIf5mWaFL8rqupNLCWaW7fGqHXFXskNXXvUNIJqrpKJ1Pu720FPWHiNU691oWgv91tVUOHk6OOk0s+8867UGLU7ydK9fY2IXkdh3oWnpyRLutCv54aQJcn4eoZydwp1t2PrLGz4fwrtYq6gIPUsVqTs5qcko13kOt3qES0gv6aHH2I3Q88Z8X+uv82puoWxOTIQt/uysnd+iu6Rl2YXmn09kypWWYhLg4/f3k7Bdx+vpyd++ZEFmc15p3GhvMxMI4hBfHGvzwAX6WMXozbqBfdLszV+ju5Wmv13u+urtx8zEXt3DLwPk75tDuQbalbWHKDFZncOwh+x8raoMgnQf53VnjT8IR2IfYGsMRYWN4RQQlrZjKICo9kWouohHsqRh7U0x0N+iq7N2wY8G47vnZ2SUN6BF29B4kJ39YTS5D0D6e2m7nUfGDiyz1i0K+huzq7iJKX0UHvHdBhyUzSi1j47W4NsjwJgoPDqlKejgmxo2porGCmfU/Yfcot++5+9ql6w/l078QcW9abSC/SeroXBUGCfd6UVz3nwrAA39kJVBAbURC5PW8ZXcKWqJ4egLPU328V87B0gCsjDWL6K3XJBiT6rMQZZOCK5YlWlrr2h4Q0I3atcaayOK/UtNTkBcFG0LRtoTgcAZXXeluL68lamYMb3ffZVkG9qCPDFe2niZTh2zpJdSE7lJhzn+PZdALWm3yNEzNDFBWcxGauq4MKDszITAvg/lDi5lU3n1bVq7E8Oc2RVeb/xVXHy4Nmygt2QANIZqqEg2nNfuH+HjhrkHXFdOtnD2YwsGrQR32BhuCfaNAIRC2MiVXB+WuNGpErr4lcsscW6+UNb4bPD5/sDKi77fIF9070K+Bx9OXSEAgIkeojgIcQnUTnRaSFS15tZtS+t423qbl5rh2gHHtTg5urU1VB/T3n3+RchguVEzz2MkJtb5SS0kzNpWJc0opDS03AxlJpUyBBJMVxpRlgUYtenV8vB1YivYoKTRag2+EVC+y0U2lGhF7zqeTdk6xPmcXjZapMCJOuZwusY3JMTpvFKSenGRLTqEozHRhZNRVsvMKNsSpZH/QUoijuNAiK+bGnEfIEA/ppx+z6KUNVSo9CWlMphiJIYAbD42Us2VlFLrNJMnBtvPrfBjlWcaWQW51C7GJrAmTrpXeu2vtc8aF9vO9fI+R0zV0sctLnDLI648x2q7sJWROQeN5OrYEvo/I3NCfmP47hOSE47oABEloxlnqLUN/i3ijdK6lJaVa1WqRCAE737bdlMn5erQPWKzX8J59nQHjQGMHiBY5EjiwtCyJlELIZi9qY3afJUtlyYwcrx5RTRnospKTwFpAeARFTaWdILW/yPwaUwGYvW6sTHYUObaHb7KMXz7SF23jDeRwe/FMYJE4Pj7qfDf87eWAx4piwH0NGo0NqM79iVL9FLJ+W6dRhIDdh/aNKm13boTN8O5EaqrV+1trM/asj/bF4hp7bWyBf4FZBbngc1wZkn5vy4Sm4Bqv8IP8+OLZ5dtQ25pDM5thXkJL4i6De+jH1Pq4KW+AAQE9NVfhivr96Gra/2E7+wdQSwMEFAAAAAgAmgYPXeWniF7fBAAA4AsAAD0AAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczM3LWFwcGx5LnNotVbhbts2EP6vp7g6xWS7lt0kwDKkzbDNddNtbW0kHoaiLlRaomyiMimQVBsD/bGH2BPuSfaRkm05cYsF2wxEkUje8e77jt/x6MGgNHowF3LA5UeaM7MMDLcU8VJRIQqeMZEHgcjo7Vt6QFFGr9gHnomc0+fP1UCqEjOYXI0vr0bX16ff9lcpvXv3hOySy4CIJ0tFratSYkAYyrRauSm6fj26NpZZoWT0jCdqVZBWyhLLLNc00WqhuTEEfy36/psT5+lGWDoJMhEExdoulTyliJ4+DSdvwsC7LZhd5mJOYlUobWmCTyylC//WDjeBh53A8huL8aKvOUtj99XmMlGpkIuLsLRZ9B0WBSpPsSicWc4jo0qd4F/CJO1/RsZqkViazWYyDCT/dA8bIGIV3lgeYcGCWV67Ad4uqn6iSmnbCKRDDy7o+BwwEGkmDABcG8tXI4CyS+2cJi/Gr9+QkIZrBy0VSkhLEsBmcJUCRJbYfE1KJg0g/F6aFzlLuNutR8ijR8dNEH6omCRaYbNb+ZArHJ4LiYmPXK9pSNPfiC0YArGe7l2mNBq5KkBUXKatHWZuo0d3NzoAEdXoHT8+HuDPOfRBPNpMnA3OqJSfBPLNYQ5WyRm2/hWyS54X/yWwTCJN7bGVD9uvfpy+eD6Jh+OrUTz+6ZfRcNo5DwOfrl9xRC+VA28fR839KXHxuKV9mgJqTC+EdAtePseKlSPBc5ApvcLwfA2nfjnwAGgznzp+4V2szylZ8uQDqLDJMsLhWiRJVEEb1dBuavlghd/eYcvufh7VVuNfqQ2slBGYWlc7G5g36Hy0Jd1VmOmA0rukVsj+U14t0wsI3o7Zmph7UVvZ9HwiCHLzCZ6L/ictLK9Exj16dEBqKoWySuVmYLmxcVEL4OlZvIMq9qwV67DTdOozbHkFjOOstKXmcbwRQSaRh1dZ46BqVYvryVIK63bbjn9JRhumV+PxdCOp2A4QxnEHOBiVf+TtTr9gmktr3h6/axglOYOWT7Y5vdimdImMpgjBtDfB9N3nEHLSOd/au0fKHcmAxoEQo2GgZ3ETV3UWV8XShghlt+zcb7XpWRfU9gkMqNEOvtYF7rhyO/SRDYrlZ9n+H45M2NuGe4/dm6do38OO9Az1IdnKVccFRCWOnTTEcbiP15YHN9t2IfT85G1oesG2bvdvAGe4ARyq0KNGVz+jv/74k5Jc4Vg6acoPiJtvEhGahE+skUjjclC5MAdazDDa2bvbBc7AXOTCrret6rzhMqJut1aWbhfaYjWTJvfnxgNicCqw4eVwSKf9k/4JmbW07KaiGqA82fP0GD5WArqMxrNEbXFtevWwi4y41mo35KIDVZoSjdsXN01fmFs5OlwjaxTStq1t5L3bhUh2u03ThpIuFbpwLacM1k5n/FkytQMCnlz2D2J8RixNDcSPb9pro+tU4tlE8v379+4SOZNf6t6jUTwcXsxaD/3LDAoOk4aDqbsmVvXg16cK/DoxrrV208Y8qVoYp9iNi0aJsxHCAV+w3Gn23GIO+DW7Yv/rRV1otPZ2uAdBUeSCp31o9eRNELge5h8U4maLRl29U51c6+Hk92eDeSnydOCwTpYuvoFDABT6AsJhZwt+PCg0zulNdf/2062ds68jWAPYWL/A/dhdqUtDUWTQgGwY/A1QSwMEFAAAAAgAmgYPXWDBzB49CQAA0xkAAD0AAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczM4LWFwcGx5LnNo7Vj9ThtJEv878xSVCTrbWo9tSE4QNqySgAOcCCCbJIpCNG7PtPEsM9Oz3T0Q72alfYh7wnuSq+qeL4PxstLefxcpuKc/quvzV1X97Gk/V7I/jdI+T29gytTcUVyDx3MBWZTxGYtix4lm8OULPAVvBu/ZNZ9FMYfv3+1EKALVPx+dHY6G4/Hz7V4SVktKBv1MbfWvVBxN/fmtL3kgbrjkYS+Ar19/BD3nqQPAg7kAd5SnOBEpmEmR0BKMT4djpZmOROod4NEkAymEBjbTXMK5FFeSKwV4qQs//WOLKH2LNGw5s8hxsoWei/Q5ePDqVev8c8sxZDOm58gMREkmpIZz/MStsGdG7VYpXavjaP5N43zWk5yFPn21eRqIMEqv9lq5nnk7uMkJ+Qwkz2IWcF+kAW+LOOxCym+7ELMpjzu7yBXAVSymLAaiYr4DkadEnSZ65oMOdswaKtsuP92DTXuc/kkWKVTJQmmeDFHM9qzidhd+M5f9vosKyHigeQgi5RClikvSHmQiSnUXZkg3hN8M+d9b9rpCTsNJIUlDiE2U8Rl8ZDJi05irXbjmPLNmCoSUUSgkXhPyjOMf5NkoOUFdzrI+6vkqCHoOS9G+Eu9oDYf++OzDaB9/9t+c+gfHI9jdg4322w/HJwf02elz7imRywB/ApZepi3nhknlT2MRXCOJtuG5dTg+OX7rH30qyBGVdc5GZJbPnb3913D/wt7+/s3F/pG9vTxcDXri/tn3b06P3w3H5jRLWbxQkeqjzMEcfaO+P46UxoleoG7u0zg5Hl8cnx4ukZjlaUDWUjWNwWDz5TR8vtNjKnmQiD968+lhSUo2MMbXUBien43WCeTNb72CkodegrGDgU70Os6S+1tbd6FhtB+gnGzdFH4EZqVlfOsco3QBmskrrlXPQdcjT7nU5n7P+pCXp7foZSUHsGbNU1pGgYbLy8v0UvN45hGC5ApwCEHMWeqVkhH76OR/43V2412Fwerpv8jpAyjTypr6syo94nEGGI1y0dDna4uygNxc83UiAWRSTDlsY4QnSaQJTQ7398HugjkS51IBWpDF8cKttUhX/fDQVfeUUt1SxSkYt4T57co7HpJ/XglrhX+bR8iHzBGtCgBEylM+E5LD4fDCvxgNhyCmPyNMml1NfNpolzsKgOjsQhUHCEdhFDLNMT659rXkHNNY40iJCx34DsGcB9ceghllLYRoaUQwTO2BbLXoqmUswqsacxbXOs2pteSdJ6+T6zCS4GXgbrRptPG64zpPNtrI2f4+UaLBu5M3h+MOeMj4K/AEbnKcxh0NRFnmp1jogBYiVn3UX5gnWYlxvhY+4gtDh8sWVYlAl59/vjg6O33UsUvnyRPPi9Is1yjCK7eYELm2M6/LGaxSuMfC0CT/wbcSI4tVTER3F/n2wHFQ56uhylqlgVLT2oOsS70nFyhD7J5T1ZHbdKV6dhdYHka6TGvGdOQNJT3rD6ujZJUNjHHgvv/cVzc5B5PcR178KrnUqrYMkLusvOJx6i4iyb3PT3k+YWk042p5T+XM5S6bVFYzY5KTdeZlAVWeJJhSfuV+GaV+kZxQyPWkCow6IYSpgacEJ4tNpL+Ya/4jCBldoaPGMDx5h4CVMHQCU6KiByQ4fYWw0HMdZy3S/9+YDQtUO3/JI4ksxXGV7dbEahkzjWhNmsFJ8Zr1bmWkuS3Z6U8X7hfu558dJ2AafnpkBUdNxPDs3VbLKZTXRZzppizhXas0Xy2SqYi7NsqdWrXdcvgi6H4cISRI7c8xlSBo3/2uyr3uumK2ov3PQUV7e6f7iUX6nZCnKHBBdsWUX5181A3bNfc7L7oXaPeSdGPsV3sfRXOnpvnyRXcf6xz5cWQ6kqUPv9r9KKovK6p8a9A9SNiIYx9bDfxq12OoEYly+GLQHaOpkdBgqx751b5H0XtR09s23A22yDb1yK/2raVHHlj6rUUFrEa0nxWd8PMdvzpmEIzgoXJc05r5iB06RxjxyxaYpamwPbbCRt/OocOXwzylClDpNe3z6OzsouygkT5mft/vYC+pRHzD250ewVaq1ZfNrw4yHzNEnvOKYwMQR5/OiV1yKtUub+zR5z7iVdFDU6ttxC0xyDeKUb7iNzytMbGtECI7deNMPCN7hss+uI+Ld7c6fhvhcaLRE9jktrHojKOU77luA1ZcAytuB5iCWX2zadvFLaV4It1Gur0DTAQjzkIu27NOp9pKPGOTR5XF8Jecxe3tLsR4HR1ft82tgQYZot1fBl+/uAVIuV8fcRRdsjzqbeJZ9HE6d1fntqZDqFK+Kcuth91VdvGa0C61XdaCbmfVO0qpt1VMHqdt94GiCNklOg8eezDN/fnB1dVjdW5ZKUYDflGCqzpSfZsEfKzCMTPEi/+tkv7iW0gtCz3r+T7lMESDvT1wfZ8KG993LatVINJsu7OEPsuvfjv06lcBzbPG09wO/OePf8MsklgypCL1JEJ7lHDb2VW9FT1bsVRh9O5WtZjjXGB9NUeTY/FFm7FtQwxKNdnIFFxAj1CxUFiKI0+bg80+/gctkVJsAM1IoBDhQqrVnLqdXep7C14MxbK02+5v9wCIgZRsVfFp68IEdakcbExUFHI4JUSYFq9d2B1oYR4uYWxRtaUaHS7mt3gBR5Fc5KWgTIa3CJDYDkRxzwpdM1pUdWVZqkhLMKkduternXRiBA1QQ0YEg4qOwuo0LvvpXcfxYHKn6JjQ3P1KwUw3srz5biZoM1HmV/NRZcdyyea2iRXKqi7kqCcuUdeohzJyYLLOYSdQxhHguTn2uhpZhzDPYvQLo5eI8hRCKkxWdesTY0p0Fnq7tGV77VYO+ox38aHhWqZBMw+mgJLTa5VxIMpNcDvnxIGhUVvVhjtyQZFJhasUYR5we1VtzIQRa9Szh7x6PUWT24dvbuKjeBOyyjJhg0abTCbmZd508nvuxvmng75pUvtUBARzsjc9nJJPP+9t9bao87zim/1MIq58s8/7Ztl1zJOMEdEOa03Qc4IJgvIa81uceOAR585WZNRxDgRKYkRIBFJbfivG4GB1OGExECVAkBAXKjVlBR5IOFM5md/4TtV+5Qh6nsozND0qdF0zVqCV7fKaeMTsYezWaM1uaKEJdlvFGOBv0HNNrFb4nbk/1fyd/Y8yQcv5L1BLAwQUAAAACACaBg9dPloGIjYHAACgEgAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzktYXBwbHkuc2jFWO1y27oR/c+n2MieWrojSrX9J3Hi9DqOEqtzY3ssdTK3SYaCSFDENQVwANCOOvnRh+gT9kl6AIiS/J20mWlmkpAg9uvsYs9CW8/6tdH9qZB9Lq9oykwRGW4p5rWiSlQ8Z6KMIpHTp0/0jOKcPrBLnouS07dvYSFTqemfX5y9vxiMRvvPe/Ns9cnotF+Zvf7MlGKaFNeJ5qm64ppnvZS+fHlJtuAyIuJpoah1UUssCEO5VnP3iUang5GxzAol47cQnVeklbLEcss1nWs109wYgtEWvf7TntP0VVjai3IRRdXCFkruU0yvXu2c/74TebUVswWcITGvlLZ0jldspUP/1G410bU6keVfLdarnuYsS9xbm8tUZULODlu1zePn2BSpMkuumDbY2YZ9otb70W/DN8nJx2R09reL4wEdHD6Kw2fZuiV39uavg+Oxk9tufzgaH58kb4cXnZXw6qGnnGwnkvz6/+vCTdnBIDl+99vR+1EQX712KB7OmU0LANjnPGFTkbiUMhvCQI05kHupqqVtN7h26Nkh7R54G5oJg6JYGMvnAyR6na4D8ubp5CNBSLAp6nNaqvSSJMolh8YMpcFSWy5IyXQjvd6k5lXJUr4y2qUG0y7tLpOsa+hcIbzdvoVV54A21gLunc2lD0enw3eD0bhD3ygteHoZcx67+OG+blDc+Wx/nV9mQlNcOSPuafvXTuuz3Al2P9uA6HHnFrQpbb+iWGH3uih+nsv3561vLE6D7RU/L6S7VXRPbHcrxUX6w5Wipn/w1JJH6YfLxEmFMglPrkyq3rUWlode4f7p0t2Ocf57FK3wuB/XKEqZpddPoo6+Njh7txNtiVxmPPftcjQ+Gg/PTpMAZDi7o/Hb4ek4OYm2sEtI/vTGqP9LRL+g1wcHYiWByNGbIU21yGYcUGnfoIVRJbM8I3TpGcc2Tu+Pj2laizLrQYNTMsa+9eemPKhgBqjTKb927bhAj8WikNhZlr7jd+m6cDzj7KyalVMYEmhUrVNOteGGvHs1QNnfS8AOyKJ7qazGm+/6rxrMXveIzqTXORg4ZZbpGfhuqmxBTHPa34unoJBaGjGTCAxSfAbP7KLiBsJjR1BLb01dVaWAfcnm3HSdOldHoJaSz7kMxIWIM9SHcwqSTp10y4hyQehQrCSrvD8hrEqrKXfQ9SNn0qV105dVlC8f/uzjfhlFW1xmIo9QIk1BWaVK07fc2KRakuf+i2RFCSEs06sWq9Ly6CVJXtta8yRpaJNJBOrDMxgOwlothXWqH+HZi7OzcUO1UIrsJkkHxwpVdMXbnV6FDEhrPu1+ieByycDt5ys/PT4nH0+Ck2NYMu3GZs+9HjPDO+H8O1x8mM0RWsaWCJPMhRRzVrYNL/PldvdnedDb3sc+tZ44fK3Oo3NBo9UZ6SEOru1QtluPp7TV9V78uHTI+GPip8prSEYua2kSFr9HIBkN/45KSNAXdveeJ8lK5ibOoYZ88SbuSCa+tO/A7w7qU8CvR7D/AuHV8v86k7S6N1T5br5aeTBD389kD+FYqukmbnnJZgaz2pwJmdQyLZic8ewnQ4jAU28IOqTbeiP00jGGa/j+QcjAhQadz7oV0+7c2A5ydss9NHJtzbVwU/Ua/r8cbth9rPBuJWPt4xNCnml275Xz15gkcc0anezwkFDdHtekFaBcdRO3irA2OufNS84Ld8lZse/GReQF/fuf/wJvX/KNll5ch67uHcZ8BGIAQFkUOWbMhTZ2pSHef455RIIvVYWzTlMO4DHKLmxgTsy2BpQy5SnDKbtBwdGaYweDNc3eJJwl6S5pFgaW9Dtp2tqkF9wCy1mlhaOn3T/v9vGXzAKOfyWTMvjnGnYG+wDHpZgYJgPFMv/VTwtRyICYCtTJAhbTss64pwWwaDNYkClUXWaBNmXBMT4tlRYK1J8JNpPKoGNFphDzLuyGG2LDXsSyDP+AgLVW1wgXM0oYVhqKrngqchcH1pZTS7TBtAdRFNOk6cATil/fNwE0e3yffXBTdKpuc3+OI+tZskvHu7vL6Qkv2I5uSjlnjldRMKlWWIZ/SIzMmM4ipEUzvUCuC3Yl8MVNJ0K6kRV3tig6c1FO7naZCc1QXoYmD3W2ib92U+g00WQt2HUDZ5FXXRzg6SxNw9ByTyXMUG8UehKtehJc2toi3OGjaDKZ+B8SfC88bG2ff3zb9yNh3w0g2A//4FMMG/F+b6+3F4dT26802tbX8GuE/9zCrOwOk79dhMe1P+624V1p7Pj/W2Gbjz32tBQX13EJKVdvt7bC01DvmDWsOzTz5ZyWcQhyDe1AWSoJD2HTonSH1iwPLU5gXpcAwtSldUKutnB5qAGVrOcQJ5Xjs6NC/xOGVBqDh/gH6sW7x41vp24+5ldcxk21rPoGDpFGySjdC70o/E6y2W6Yn0CzXst/Cxt2kISDneUz0U/IwlrZOhu31p5My63935Wfneg/UEsDBBQAAAAIAJoGD12vAhVTzwQAAJYLAAA9AAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3M0MC1hcHBseS5zaK1W227bRhB951eM6aCUAJGSL0VrNSrQOopjILUN20UQWAa1IofiIuQusbuUbCAP/Yh+Yb+ks0tRkhM7cOvowebeZs+cOTM7uzv9Wqv+jIs+igXMmM49jQZCrCVUvMKM8cLzeAY3N7ADYQZ/sE+Y8QLh8+dmIpWJ7l9cnp9cjq+uDo6iMoXb21/A5Cg8AExyCf5lLWiCa8iULO0SXJ2Nr7RhhksRvsFElhUoKQ2wzKCCCyXnCrUGsufDrz/sW0t33MC+l3Gvuje5FAcQwuvXwcXHwHNWK2bygs+Al5VUBi5o6HkVjNxXJ2hxB13P4J2h+SpSyNLYjjooEplyMR8FtcnCn2mTLFLaE5xcvT/9PX73IR6P4+O37387uYLhCF511sMuhKclM0lOp/uIMZvx2LrDzEQEnsDlE1b+gxFi34KMElkL0yFgXdgZwd6QSAFQjGui814bLMdE0cbTIVFWYWIw3eYTmNZ8LkoUhtZZYop7kCLZ4sXdpbAqWIL2th6QFz3Y63pVtFTcYEOZ/dODr4nzzIZ0I2Wh+wa1iasVhIOjeK4pTnG+jHPiH5WOqnt7u7v720Fp9mzQvSQ8Qe8lcbFgHtLxOBlP8nA4WPGQFWweU6SY0JQMjottu4ETdxxntakVxnGrbyaEbPJHT8RErGZrwY29ZiKeygm7+fL8/LqNEVkmrcRxl0jVslhgpxtVTJE89M3erd09EUlBolmL6HDgaHtLuMct7Gu6U3fa2yM7PGYau8OJsCJN0UqYvN+4rONaY6wwqZXmC4xLUu2Gho7GImtP21/Zlp0RdBz8Pvit0P3uY6LxXQz87saGtRmRJ6jMqehs5u3P//9S8HsPTbVQt6YfB3Emn4dj+F2BOLVkpCjBSqun0Qj8mOjnIo79FePrQNrpDh0KviHuBw/A4YAegC8UvLupP4cD+OevvyHjd+AchdYTsJqAdfwtyGt6JjKutAGKbrHaX3Bt7HZKoxnSgtU1FbhB/6cezGoD3GiwbJDbyn6UTKQToXO5pF0mZ8a9Ps3pnKVQSN1M0etkpOIJ3TQeb0w4qZLvhiss7ocW13Q6tX5NBGI4T5InwwFhAlEUuQOtPwkj1cOSaTg5+9MFCHDBitolMkhF5TCC6dcamAKdS2E6HE1hhplUSGa3Vq1FSjIukMq1ljDdFkyz7LhNyQgvS0w5M+QPGEmFBLCszD2xZBQ5EVms2wFLcibmqBv2tl4POrtOX5gSsHX0WpqsfxPxksdvRd2ZXMK2u9RJrGgjf5bUabgQEhrqWEDVQvdWjYSVg+OFO4Kph6A6FbXRqBQuuKy1lQ+d1nVhrGkuyDZPyVdgayWEDiKUyDTVYUuAM7O7C9TcrPy17dNEWJzHI//VxYc3/VnNi7Rvyz+xyIXuN5oJD6L9aD+kBmiOe32CQQnRNGFu2bf2nDiSHJNPE6FK22k1xtZUtc/o+iOSq1NuS+imw3wZtjnTAnP//Ye6bLyk9BJ0vKacWHDNZ6SQRApDyB3BWV0U24nikqNHfCVFbUsDmQxPBhCe7xNcWXITZoqKTFhJLmw0iEOV5CP149GAdpWmFtgMptETJaYiRVIF2ZYjq6qCYxrR6sVHz/NchxlQEIbB6hvgO4RgY2wTiM3ccwPyhZVnBSbw/gVQSwMEFAAAAAgAmgYPXUuSpWsTCQAA9hoAAD0AAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczQxLWFwcGx5LnNoxVnrbts6Ev7vp5iqxVrO8T3JnjRpDrCbZtsC2zZIilMcxIFMS5RNVCJ1SMpJ0BbYh9gn3CfZGepiOXHinAt2+yNVyOHc5+MM8/zZIDd6MBNywOUSZswsWoZb6PFcQSYyHjORtFoihstLeAa9GN6zLzwWCYdv34qFSIVmcHb+8c356cXF3rCfRvWW0eEgM+PB3CRiFiyuA81DteSaR/0Qrq6OwC64bAHwcKHAO88lLggDsVYpbcHFh9MLY5kVSvZe49E0A62UBRZbruFMq7nmxgAK9eCnv4yJ042wMG7FotXKbu1CyV3owatX7bNf2q2W45sxu0BtQKSZ0hbO8NdWK+IxaJ4lLOSBkiH3iaoLKom6IPl15xBZA2Rw7OjdbsctWX5jcTXra86igH7zuQxVJOT82Mtt3DvwCkKJVLTdD1UurY+ci3X0rIRnxzAqRNA/zYRB22+N5ekp2uPH3leS+P0Qzct4aHmEHyy0yS0oyUGFYa41iuVdiJF7BF/l91Js1r/WwvJCMye/NNOvbevCfY1ba85wnLzHgul1HU273V4qEcFnJuw/lP6A8n4+xwBqGwyHo5ezaH/oC2lB5ulSm07rqzv187nzyYofump4VHnHL4jhJxh24GvtpFhp8I+Omkv0b7ADKG6OGTy8IYF/HQ7Kj71D+PDxDH6AHK2VkaCkYklPyF6m0Zci5JAolcHOoGb4vVX8/N5Cw55uYS6NmEs04zeZer2govLvU7yqeDRMRTPfCmOVFiFLwKhco/oojkt7BBKrZakSLBtkWPIDKisMOfKb3cKbk5N+ZWht3h+JuivSEDVIBCak81CaCrU7DkhJ7ueoW2Z1gKUbRVSzXaAl3LewZEnOKwft+LXmNcFOpzyEznLER6hya11kTVyKTRSL7kutpGhucy0fF3Z0J+zPESNI0puLf777e/D28+7Yr7jCZOKo/M0MV3p06iOd/6EBf7icX6fsnOOtUKb4ywOflitd1gKNxTYcDtnwYJh3sWCPHqYYbqXY3Uox2kqxv5Vir6J4iISXYm7ieBTnD7PiW03iw/FWit2tFPtbKfa2UjhNm5nW2OjANxiRnRtg7/FMaJRGIw86K4zbQDDcRrC7jWC0jWB/G8FeTbCZgtcyihw4eohsizEu/o8T7G4j2N9GsLeNoFTyoR0K/p8HHRdcRpg0w3GRMXw89PH2Nba4I3ailAWWzasEKrZWqBcuxs7rdapsSGmi+QFpXOH4FQw2QbcS8sjhh+uyorhXMbhBziLl9h+olt9m/AYod/bXl/62y2XNT4U/F+NL5xkYwF5+hb58zEFHjTPjxplhY4N+Lz8q25vJklYdssewubo1wgxSZsMFNperbEmwc8GFfmiW2GqWHXT6aAeNnUtwjS0XEnoUi6LT6lafPx507zdkXa+F7e2jx/Y2HyNpmmCuee7lQXUO49itgLAU8hj1KGxSY0e7PgY4BTtuBADsae9sOsadxoBwbzjwnF8hZVLE3NjGjFDPR7sHMKO5gGnBDbaHthgTVs6/Oxg4ndx0UH6NOmvbTqtiv/wcdVrp3VFj42BRZIdVKjEDi/oGWanl3igoMqToZgOzYBnvZ7dep8m37Wa4IIhzbIB4EFRjHJNolhsUzUROZLmKGVZ/51JYEjiRD42BdPD848dPVQ6jFCy2ICDbjUqW3O/0M4aDljWXoyuinsgwYejgs9oGh6oXzoILMuATSjR+JbtPv54wwzuHE0nRpLnTeYHcHOSGm6CaHgKXBFwHNPZi5IySvuFJXB1tTJ++03qwBZg7j1XYiifJ6KNRXNt30l+tO+T/XbOP113n4nJjtfSgbG/7SOR1HbcOhWLNnwRpLjULp0YCD9uA8DtI3Axh/g/O9B5rkFamPCkSv/Ni3x6LO46scCXgN2GSR+hLlmCEU6yCIMNJA+2+58lrYRcrPz71JuirjN81ExGGpqNjz7sPJg2tgeF0e7h+VKtrGhi/4v+XnmQp964OadG9HND/QhI+9F+L0J5jLLn24873jd4//TVnie+trg3UhthfevdLwbu69PDa9646T2CFd0PNqrojmgwclMUId6Q/gt0x3jABhkTIIPBKe2tsoWUfD7U34W4Bu+tPdaN+Gt1B1+eNR7UR/Odf/wYHw+4xLhYaWxeXXbBgOrpGKIRUoFkOcj8hSai0Xr97entDQISfcfoZ5SFu7ezsDn7c2YFE0dtFmQLgkoObQ2LVg/d/+3TyFqYVvCyYjBKup40tQtJyu7l8knCmS8Ao1t9dXMD0fpwam5XnG0t157ZONhwTo2llbuERu9CcV/qTmcvCX4vVAw1BOEKZHsQJmxu8rCKowWQip5W+kKqIJ0B+XQo2Q+yjXHVPouRZESndJ9nNGKXsC0rlSxHRA2Av0mLJZfki1CuCV0aF7sbSvZv8AQSUqBrUEF48HJGyZA6aMhfSGePuJcq+1dXUBZYk6poi+ebkBHb74/4YrKqi7jiUD3NTk9h82pvhNUjXxBIZMGmJnXt/K9526a2KmNG5tfeseaJmLOk7K+rAFboXIL/qo9+/f/cRSrRHQdgxsQhUXL24uP4eFjzJuDYOAyuAamg7aaNPZnj755bDNMkFvBhNEdWm5nqKdf1rTl4vtFnlDHzhPDPubTbE1JU86Y3paR2LQNEjne4+JMrgUsJJFyLvaT7HJEJ3rEl6X0IylnlkXIpsjCcFruEiyqpVfVpFQkpMhxrS6YnUgD9dQV3JZwVYU0xdQx0mJlpy23EJ2agNemgsHiLd+2pyCwmPMYsS8kadzog42nVrR/iJZrMl6jR9cYClVeRsym7pQZ6ucNSqUUpl8AhN0xk9gGtgWDdoHCklYhHWz6GRUOlEkvpmofIkAkQhzFXMDhRWpFnKmcl1GQeMj2ExrjYKxpn3/Dmc5w6Mp9Mp/YlkIk9Pg5OTY+/F2efXg1kukmhALS1GG/NswHlvHoY9VwQ9jMacjwZoSixuij+0uG2P+FH1Yo7w8MtE6pT+ZFIwu3dV1h99VZ5yJD233Ftc9yocrRRz/3tO4weuhAztxvGhCSYsyxKB/Y1r1H9ptejvMu4HtNEBh+3yG+BPMH/FbOWE1dpTnXGHy5Oc0m79F1BLAwQUAAAACACaBg9dPRB4m+cJAAD1HQAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzNDItYXBwbHkuc2jFWetS20oS/q+n6KNQwU5084UATjincgi51IZAYbKnTmFWlqWxPRtpRjsj2bAhVfsQ+4T7JNujiyV8w5s/SxVga3r6Pt1fj579YqdS2CPKbMJmMPLkVJMkAZOkHGIak7FHQ80PQN/73uqZe5d/vPuha9rNDZhjOPe+kTENCTx/rr5K4duxbNsTGdKRO527gvh8RgQJLB9ub+HhAb5rAMSfctCvUgZjwSNIpgT6X876MvESypn5DjdFMQgSc0kTLu5BcJ5YOvz6vK1239EE2toPTZsgCZj/gP3IS/ypmUk1p3MzpDKhbLJfqfdI8If+50+/Q0EEseAjAlRCRKVUDwqd8Mmci2/qSSIIWSM+vk+mnHXAhDdv9i//3NeynbGXTFEPoFHMRQKX+FXT1N+Gvs09etOaC5oQNyF3SWPffjFg8AI+opJcUN8LIdd6OkdP+lwIGnABi/0wxm/Kh9DPnQgzx2p3rIzJtScmGNByVw+cO8dpHY+CzpFlFZ/JKz8jXpZKZjQgzCc9QMWnPCIjQea5ASB5KnxiK5WybRi1CH0THJNX7Y7XOWi9ah+Sg0637Yy73ZHjBN3jbqf7yifkIDjw2pW8S0EkETMC00pwzhxoQHkke1mOvDt/C1MSxkRgaNBq+PDlq8pX6gNlIWUkY+ZJSaJReG8A4wmcwoyH6JGQmOfnny5gLrxYMciE2wM2YFnW+ZAySScMPUlZAn+98nnKkio+cALOa0Vtv6h5L+Mw4zTADchGJO7UY0FIhFtSNNRqc8C+ozj8WeH78uXrfEWQJBUMv/xYknLgVFL+8GjynosvmCGlwJKoodRmaTQTcou4hRlqdT5VJ6OxSvSmYgTFz2LTRj0Pc28oNa6JXNLvcNkROZtVBdcwPqq5+TQknig3lctP+bgweqv2xzUh7yLvCvOxFHC8LMB1PRm5bkMfDBI5B9hzDOTTchzHc44cvfn6aTJnN7LObmSt3cgOdiPrricLKZK1Fdl43Bpv5tQuOZHd9CK7OYM47d3IOruRHexGtskZS5ZuMAFLGex1DKRsrV1nPN6sRedJ9uX27blN2rUK0icswPx22m65liU3vMCH195kfZKHtKbNInnXqr3XNTK5Haex12luMi73sIMh3UiFHs4djOI2U2UOzslazsFGcauMnnBZ13lUDpy2qrxuubalHnhBQFMUeGyA2d2ozxHq00Ci5jpHLuRZMQ/D3kbvHD1ZTjalF/Yomu3PeaDr1u8fMfLPjOaxSjvk4jpdK4O3OK27ITr7BiAK4QGisRM9Tcbmkd4sQJXHvPBeUmlnIBAJKnhVQDzLl7MleIUiEXNIA4+DwbyIGHz0d+InrryPRjw0cvAxYFWrN8qPXd9YavbGpuZvbMN7FfMDZ8H8sGus9nhjS9vfUcRhpf9R16j1Z2Ndr96R6VHF9Lhr1FuzsbZP78j2eMGWtHyjbMbGSlfejR1WOKM61cai/hmrlXBHht2K4aFjLI6GsVonnmC4JacTzkNpi5QVMw0OFiyhRFhyupTIz1anN4S0y+ObOlJXFxfXJ/peI5vkGgEVKu/VUPf72/5Ht3/x9er07Ma5/aE3bQuHHRzo4nnQ1Acs26B264rNu7P3b79+vnbPztzT05N8wR6lNAxspbU/9SiTNiHmxPfNjtW22iZm14S07FjgLHaXq5otI7+Sy/fsA06Xj9j/yCTSMeCw+QuYd6hH9lzHWfK1GgmKSpGPdefF/FYbIs7O1FASI87Fqafci2NcfRuOoT2IcFCEEU6ZMsEBwUT9cqXr1NncN2BjqpSKvqEHwYwhN32l9gzY54sPJ/qGVbsMqIkxtkI+yQx9BqcYWaV4DxjBPMESmEoCHqAyCNXzGlXgdkwB4WUDbMm9GJlwshGRGsY3yS4/WLyk3FBCa6O0qYZxkVhRUETkl9xl6wdvKMNaelzfQ2/o0P71eWs1bicnJ8VwW3qll+sOKnUVfj850fMNCT4Ak8GRAznLWmhaZWg2cS345JcGZzDYnwuekN2Nf6gcnUaRJ+57D3+7uenJ2MPx+Pb2ReP87fXpx4fzT/1+s/Z8sF/oCoNBru7Dwg6EZg/4TaSkcGt5ozHYXxEGh/bhgteyF5f9iWlMMVUmHhpYuQ8FbImaWWxaGzwrX1yNofr5eZeiTQ+f+eJ6o7RvIa1yVavmqlJu/TqnUL7MQGV5Dy7+UmZIKGv7ltOoFFfjXGSU+qyyqsqsMr/ep2EIeHKxruSJ+GQ5T7DVurHgE4U8um23OIeLauViLWBY4+P7pRq/6VoJMyb/kjKaKO5lmceJNxPsuuryy3WbFork4Yw0mlbsCcISedO6VdRY3UNPSrhcqPVBaVVdAF1lOimUIBulGEt9PfUkafZyDwVkDJl1NWOCyHPzW5kM6VGJpmP/ZklDknBc7syigUaixo1McxueuiUTxAtyx6y4u+KpZFjqJkgkn1hjsF+DmxundgygYrszl7Vj0f/KZGVg2MrgC1c89CiivNN2la+Jvjt9yL2gIl8K3Vwhl7i4iJP1SJI79VhSzv4PodOfvqXaaFJ+ntwxx64o3TEaMXXzHvqkIdsB2M/YsWtP1p9IIH1r8a77ImsorqtgnutiFwDddSNEZ66rF5YvDrR63GhurWEB96V9eXXx4eqs3++2EQksI9FFEYFuG/7zr3/XgVhRo/M73Zc5njHzWJhYEvkY8mApra+nBDDjZpSnyMo+hIh4MhUkwuIBAU7k6l5XTvkc+0wN4WXXzziBgfrF/oRoE48HQhyFodRTVdTz5jCs3D20AD4hpvJkxtZ7JAzVUvfOjMzDe8WoTL2aYUN1+z2soJdSn0ooy3xPPTERx2VnVWbskP2MzpaQ6iMH5Zfe2Z1+tgGxIDPHKfPVzb4KXXmV/3qFeaF+gD0ew48riGpTFniCEonBvfPDVEUXvJBOWGZkrEoQm2Ss8KOE4ZbkHxoKfvpT3D/37iWetZAkhWhf3SsECnHkgVWKjrIgFCEqpcSCYgcCzsL7XOclqFMgXJz/MS2z9y9INFb9tgTzdpH1SgRPkzhNgDIY7oq2h7mxacIjL8PbqAguFeEpgESGSupwAtUiTKEw9bIIS4A/xbKm4vtx5a3F4q3JgA1X3pvYtfcmQwM7cP7aZLjre5OhteGgahm4K3q/Xr77woWiuqnlbbVL46G6LNcz4Ae/nUAxpml4AB4932tgZdlrzNHbvicC+Lnxr2nsNbyRVMDmpzkU/3UNFVLaYxYo23parairf2h2HCIebyCRoQ60Aa2mRkLcpoxTR//RVuFRHLz69zIh0RliwcqdPaya2BExv/GQ5R7B2lwcJvRh5uRaXVR/1lbVPzXNn0Y8gJd3sOXEaVqON+vF1YvjkGLy6Vr1JlWdpl75AMCyt7Uw7b9QSwMEFAAAAAgAmgYPXaY7+qUmEQAAYjUAAD0AAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczQzLWFwcGx5LnNozTtpc9s6kt/9K/CUVCzlkbp9RHlKjS3Jjuc5sUdyJpOxvBREghYnFMnHw7YmcdX+iP2F+0u2GwdJUZKvqZ0aVVkmgUaj0TfQ0KtfakkU1qaOV2PeDZnSaLYVsZjoLPFJ4ATMpo67ZVqk9PpHo6O/Pv/avy9tbV1eEt0mn+h3ZjsuI2/e4Cv1qLuInKhmJ54ZO74X1UzrxjLCwDTq9ca7KdurV2k0J1dX5OdP8mOLEGbOfFIaJh6xQ39O4hkjo8+DURRTHK/3menPAxKywI+c2A8XJPT9uFoiH940cfSdE5Pm1v3WVrCIZ77XIjr57bft82/bWxxdQOOZ60yJMw/8MCbn8Lq1hd/lUhSatSBqZgTOnAgmcEzqGiHMesNCZlXNUqV6GzoxM2J2F5e3a2/HHnlLPqawZDAgMANiIcPzHjH9MHQsPyQpDmLDG66JjMSiyE292mxVOaILGl6zuEPqdyl/quLZbO0KGA53HjpzCqvPiCSRn4Qm6/BuQhxqWcB4nCd2TJdpJH2sffKtxGVRTdJZYyxdddWU44HNc+Dlu513dTa1W1N7b3e/3WyzqbVrTuuNfbvVbLK9ZpvuUWqxVkpYr//XvnHM4pHzT1augAZYsmlwMTr5+6ALi9knURLeODeMOB4XsOn6EXMXeshcGgODzo8Hnzky0w8WxLc5UETnLOWsTeeOu1BrBbHN/Dmbhuy2FlwzT8MWXN3GZdl7zebufqP1rmFPd2nDNHfbtNHcrdebe7TV2t+Fzh22s7+TLusCGE3ikHqRK2SWeIAG2hwvZh62UNddEFiGrZu+F1PHk5LOJCSXNA/AQEIyp7E5c7zrKgHkjLcD5qnjOvGCWMx0achnisiUuf4tCUIWsRCYBszgmFpNfQo0gL4dHJ4Qly78JCYeYxZMPF0AGFDnu1ILXQeoJLcOGAVACfWHyQnlqOa+xUKPnI+ao/7vXMlqY2/sxYsAsNmw1si5xvWYMxqSZP/9mi7gA0lazU1dEaiDEb/PY73xHYuU344cexiYA886Ah9hxJUyb39b4bA1xX2W13S5GrnkJMotGNwWNwsakQlnVzghNCa/1u+abWlhM3hnd9SMQbNs9GxcihYLGHx5MUjxxomcqcvVk+vuCcoaRimjpHXFI7WWKA4TQCjW0uPk9WlMM8dBfsAI+CT7xJgy0AtmCPIukbKr96JXLF10QNP9KkLBQjndhQ98i2G5CnmrSdD7nh4evF9uQWt8n1KALeehHzDQARZl7QH6DO+6cdlSBHGBI7gH1nfZaO6TX0njamVEU4y4J4ZB4zh0pgk4SKNcDqj5nVkVIUvmJXPBT/Bw4IKYIlv5B+Po5HQwMs4+n34jXdLQCr39k2Ha2dTWDj343OdgAAEe6Z5P+wrkA8YoJTn8G5EfEOYhiLPV2ivAHJ187iM6AdMo9MJkMIPC0Cz0ji7OzjP87ULvxfDg23DwF9m7U+jtn4x+Hw4O+t94726RqtMvo4+9g97HAfburVKF3lVi3ufsvovRptH4QIsOQcNBk8qrCkXeCnPSOCgGPpDTlIXiHT0Dim8ZWw+c3UuxScmtfpTye5YAjtBnaLIZDIlBxJBoec8mPAWHQgChofIPhQ7bAnCw89zaeA8M/orh/RA0t9+j5oxJfxTEcg04d24Ut5C3YJCeGSzK4s1iETAAwkAUq/7Q1KQPJF5xzrdzNsfB4iU/WLKkOHjsISHfjcgEw7b7c4qJRHnJ3zpWRXoKCCJm5oujaWLbmVsC79No1etXRcOlLgcv77Yr3HolmjXyNq16hi6ljcddB9wms3I+sEvq0qvn8hvuR3FM6mrT1JBzpDL2pJ/g6+II8O12hnlmuVFRboSD2KSc0/Y3S9RpqQ/QSL1CfoOvZR0KWZyAUPRGOonCuYSnKhw0+aW7ggFSEPp9eTSu+q5Rh8/7rFUS7+h6AYGEuVcoNjEyI1ESjdjvl5lr1wvM7TvR9yGj1sJQ/eXUxjMuw3J/WTttjtRVTq0o1hVQiZhTiJzfWCuX1P+BdFaNW6UF5TerM1U00n7BCJgGBFhk5CrwGs4W1fYIFO4IRGqo7nLe+m2MnhopRu23sXz615kv3Y/0P5XiijUiSWjUm+3KkyWiIuFLBIIzPXfQCjg6Pd8uFxhXwSQE0RcEKN2o4qpGViZp7uxeaZuwvkQTzLolNIGvS6QAsR8Yqq/owZ4u3icLCVOOlwjoJSPWWswqUzBPXDKPi5AuhuwPQ/WucTuCU0sO44XG8BjDZBb2H+9lzMa7gpeBvNlyQkN1LvkYPNkQNp5vxTQDQj+0bEyX4LOamcv/Dw0qejNldpdXD41aSlJCUAnIJELYhGiSYI/dGmopmXIsjQJi1ah/VVWKfnPFbWZcBYfTqizNl/GWdDETyOchRfGiuyI10r6SORCP8YS5EVsa9Ygbz9BoJCdZ0mjuVYqJw8rQxk5TUSCl+34j5K6CREOtKMluhN+tK/icRPP2WEypMxedGwB780c982O2LXZnL41XGFf4Fret/p5v7xtiU05pgU1rrT/TrLwRFLNMpSN5GI1s1piroupKK1i2ooLnaVKzENWO3CSaceEZCuLfENuyTe9/WIRr1ducQUuOKX/0aSiwApf4gRZ+ls6MxdEtzMYPEfmBFiVTGjIykRRMyAzIJLA5DIC58gxQ4ElpkJLFM6kqIefqvNCJO/wANeZHy0Apha0wtKP2ENf5zm6dKMPmMnoDOvr6po6IA2Y6tgNbHtVf+/8TtjzD+PdI+rlxeRs8rmf6eNjVLSWxre+XKrJwkBY51IluVkJwQcR4xGtGN4W6AbUskE+k4WkHN2F/+g9mxka0mE99VxMKMfaybbKWbeq0dKesre6ZtSfWMVLciDDb1mjLG0Vtw77x+bOkK8C0WFvaNGnrt1DPnYMjzjJQLc3GtdW8/Nm4EWGWlmn5rFZbm+I+f4Z3KfXgXLVcwqetS/6ejR+RZt5LK3h1bZOXf/Y8iDytWMl5pFPU1nrIp8/wgBHGvu9GtTDxdMSi26HvxQ4Lq9GsYHivVsubY2+lvoleYXh2dtEtvS7zUmcZeM/Dcen1j8OD0UdjdPZl2BuAf7gvVWrVagkrnsGtVSmNPT4AR5cQTX9wdPDl9MIYDIxerys6atPEca0aEg2B2/GiGmP6tWnqrWqz2tSjmF6zRi0Ime3cCVJ5N+BTWH7wh47+egn9PZ8RnPPlJfmF6HdAB28vkaur9xgEpLMTRdZPThRhDWi2VLxUBaoOUWM/vGnmhw0Tr0Pm9DsjU9+PIR+hgQ70CaLz0LwWO/ZsB4k6/HJy2l9e/oq/BOrn34HRRA+Acj6CL2h48LUr32vKpQJXoO/s8M9pz8Pq4wP0cHB+NrxQRKz32zpg0OUkOpaZw7g6t2Dw6dlxOpVSLx0Urur615zKV6JIqEqUF1/IPIGNWMBCrDge93rj7YiAQJmlY54rjyuATQHlibwsBIowjeiURySyoBjxMA5hI3F57a5+966uTxcxFqpcLAVguVSDZIJYPvF8XhkTmcKrXMFcv3Vgg6fbdsgYyMxDa6rptufrKBVADIkBiABLl7yipgrYkAJw80WlODo9OB51yyhn/bhO9LMm0QenIDYwH6J/hSiP35rOa54wlw8cAJsE+9EDHyumIGIbNMcxY4BxKFdDTgPWZ31PjIpmwHzd8pOpy/MTfe763vVuGx4g2wUjd30KdjuHYXSK4nYjAUZDc9YNd97VgbR5nHhMvPDO/vmoCWuAB/F9Ojo0jk6Gowt4Pjg9Of5s9L+eDfvw1jvrD4zz04OLo7Php25r7FVQyuEcLzaUXoPmldDMuUrxp4Ov+A/URP6rigVy3VC3EYSbglhvJfNApQdG7BuiblkFfRiPOZ264wVJ/PQbFGqYn8Q4TtKjWsHNMV3mHPmzd9UPaUihG503+SDX0/zwpoHLSN0JOKBlZbj8EzhChc4kT3TpaoCvGPqhOOMy43iRPGQGc+20pBPlmSZz3GeuXuReigbVOqfgScFEyNMSvHSccBo53XhkUVEyB43FiKjwG9LvwMIewCK8cbfb5ekzL/Mrv0SgFfTuGvCAYZLx9m3og5t4ssf7qQCIIG7R+flfl5edKKAm61xdvS1/Orjoffz56WQ0quTax9vKAAQvfoIzc8AVeAT21j/hLUyYDFCCtD+AtJWpyH5tP8VUjFr5QIRLFzZGriksTywa+5+lNfhZrzmi52HtETDrNEj0vEyLxNgVTVru/CNxQqDMdfWUhx+WPI9UFYR/uSrkpJHiTWWZCSPVQSkRiYVLpkPOfi+pREBIUQw6SiBSQPzskA0ZwbpI+2gWGAO7jSD0r1Fi7ZbBkWUOCCRfyAk3XQ0DTRUvIJj0GS/+4AwqRSRdDls2DLwnYRiVKkzruzesXKmi2nlxdNm4QmjIDF0KOnSekpYdAfSAxgvAGpUV/iq+9kD3Kh3B5z/x0XMGqm2JFn7zhMVfgh72lE03UrC8dOhGVXmw0CVlTmqNPOOaWwg7PsGg5c11kflZb0VtppEwLgVQToM517M4P01qgwZaJb/X5MVlvDqVJx8TEJ5xQ0ZSoKC0uu0taetAVjeu6+FW9p7rwZa2kOtBitvA9VCFvdwGolY3Y+sBixuqPFSeo/z4ELgM+ULEwvjEK8u6JLYJTVmVYF49QFt4cYGCThsOaHNiMVwLprMr8lueqfT4NZxSkZKnYCpcfCk9shpxOUVevQKDZa4loi6LDBEBDHEp7LHlbLrH9dRFLN30eozq2Df5We3CmNEoL5FGu23gBsCIZjR4VAQbLnM9leTNt7seo1/FQIO7l5TTamexQjdeU8w81pPP1/yAeQVP4bFb3Bl1SyXtIb9FKOzMCpYS+rd4VI9TlAF/tQ9RDX0JC8t2ZT2XBn8k1C3vawRYXMbxD8KVslwCqEPwy/rVZUlmGqWrJ4zFJF2N1RswGPJ4PrAgAAieHuq7D+KJDNj5RTPDxK2fBdF5hf3o8/Mh48GzlSxKrATlTZo03s5vosYQz3H8ZsVbn+uUHhr22Rcjl3a5TxuRbYQzeJm0GgZajmFgva9kgFo7nmGUOqpGKeM2NpcrD+Yplm9GtfPh2fFwMBq1W3jAsJSRlF6lSQJpt8j//vf/5KsF6oQB9q/5G+ZIJD+BUKkPQYMQhwbpLXRnPmeWA1J3FxBhXde/xSQNQYQbtGCicJGQ49HpyeHYw332LdYhZswNwHQ7ZLL+ZvqkitOvljTYjWMBFyCK491arCVEBI8HeFJA0kygg6MbVTJJI/tk7DXVexrGobGlGlXMhra2asMADe876l1GY2jaVU0i9ELLXooojbPQuq9aZVCdCK4y8JshJPERu2Fe8fp6erE+lYxcPI3lffOxN3nqRfpJlRQo4HfoEQ/8B0RLYa/Lo97kKRfqxWX6PElPvQQvZHuRKyHFaDh4LAVcmQsNw5vm8l72weFJJy9JrGtZAgr0FbNgBNvO3dBWh0I0vZM94Te2xcwFeefZnv6EQlKWnpRRTx2KZSdugEmcuU3UbwqyE7SJKjZPNLBkrIqJHx1wvqKJ5IXMg87Yu5055ozwcyeiMKe3+dWvPrjnRVwhzxbwTA6og00PGkn+twHpDwPAfPlPaNRl/cmyF5vUJnknBfRSZcbHvR7hJ8hjL/bFwaIFUwNXc8eOqAacSvAUtkuvieXYNlDn8QLknHNPUAc4+Okh39oBNhY6c5QvYrV8WAaeMVI3FutbOWLkZ8nAUmrGXIx4bszFOZmIU/dqbXNk4WAI/dknKCDh9gneQ00ivLEPmyGgxgJhxY4rfRxu82nMf2uDWzWAw5lLa/3wt60tczb3LfLrHdlMxtaW2KjmvTENAhdLoqWt7LdKvucuOqqBkIdWVtr6P1BLAwQUAAAACACaBg9dWbPFxh8NAADaIwAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzNDQtYXBwbHkuc2jFWv1W20YW/99PMVXZYCdIBkPTlNbZEj6ybCFwMNmcHswRsjTG2kgadWYEuDTn7EPsE+6T7O/OSLJswMn2dHd9EseamXvnfn8pX3/VLZTsjuKsy7MbNgrUpKW4Zi4vBMvjnI+DOGmFEXNW7je23ZXTD3ufnFbr4oK5Y6aFSFRXFpkbRjeRO5Yi0zGXnpqwZ8/ogJJhN1e9Lm37Mg/9Say0kHEYJL7kobjhkkdeyC4v2W+/sfsWYzycCObs5HkyZadSXEuuFNvaZONYKs2CLGK4juGmlOkJZ5LnQsVAOWVSCO057PWzHqG5izXrtT61WvlUT0S2yVz2ww+rpz+vtgxsHuhJEo9YnOZCanaKxxZ9tZ2neXI63q2MNfc1v9Pt1a8fSm6YPRDdMBtmZycn531npW2k2I5imQUpJ4G+2Rn8xR+cvD/b3b9Yv/zkdLoeOIDo8tuo4wwzA0DQDqHZ2z/YeX907u/v+7u7fbvRHRVxEnWJ6HASxJnqcu5eh6G76fW8nqt0cM03urnk4/jOkmq2ga/Ccm9+QLNz6D/ZEydv/rr3/vi0P0e2OQBagUqM/h4VaW7Ii8cMVvEVc+/qM9Dr96Qm7DJW6fY4VirOrtnMFNj+PgtFmscJl9usgoUiSzCjy2E2jumaN+8Pj/bmuU8DHU6AsjYz0JN+BMHMzUGLgTAknu186JfP3QTXA8aDULBnlVCh/UKzBdzxzrvDg/3BeQUZZEEyVbF6SJNfXRiqG0OMTMlDKvKgIzGGANznnliyR/YutZdGS86oIk0DOfX0nV5yqhS4l4jrJ0+pUMD/PK1uHhwZcaXd8tyjN5kDpX3MH4AWjAAqz7QeVx6txORr4UM1xEc+ZcMhmYLrxlleUBQohTwuslDHAlZfS3l9feO7Ef923QtUWoOJQhOcvbpehcdyN4giE2LW7yrIep9n0cJ2uPlyi73uRvymmxWJ8e3dk+Pjk3f9tgF5u87c/SPmfgiSBLyqCXTlRqIYJZy5aSKy65db5mA6CSRiSyIChIs0E24wIuNKFJ4CGU768pvv1oEs1UXG7YOB2zsd9OCg+GG/jwZv/IPDs8E5fu8cHb595+99ODnbw9Puyd6+f3q0c35wcnbc3xxmHaK2VLpfKq7dYffWxxJBfkgODkvecOyimsRj3dyHjmr/qWxk5Z6gPnnCaZ60dvrk4dqM54BKw30SqmHYc3AhbPhJoIadG6sjMISqr2ZByln5kb7urSovfkQgZm6IFRsU8ICotQLeEZOcFbrMYb3XzzaaoY0+uYwzPWbD1T+p4VAD2+nh0b5/sHN4hMf18u935gMLG64CKZEIrK8XiZ85njPDL7kuZMbW7YqNhubeOT8ifgPJfZ6M/do/Zj5EH9fVgbzmCw5hd5Y7hT0DUfBQVzJp7qQB0gAcH3tVZJw/YBWPbfvDWfCmh+xYnce/cr+KqH5pPGCpgcZZKa3DWcTj1nKenVkuclQKQ5QKw1UkNVsfSF7/VFOFVPREDUF3myzZp3MepHxzsXE5zKhiwJqpMOqN3mUHjhBEtp7gcM4I7PWdQo/dV47x1xRAknuKU1BoS6eSQeUq28Ohet4eDqMXna79x1ljhK5j8nEmNEu3S3GQdbbHjnULWOLpztngCfOky43FBbHibDBVmqf7yMKGJCKBKxBG+FLvWooib290AKKFhjfOb/Rog/9SIHMqaJHgSgwolkitEU90gNV1Qj4WkiVxxoHCsOGpPIk1rah2p2QkXRTK8eFgADm88J7/uSEP+tWdPUIuhKZTBwAjm0o49Amp0MsKbldKDymZmWOTtmH5i3u9zgx5Bd2ng407moJ40WcbZZydyQGLwUi1S3iXwI0hPNDeyU/4ui9FSQv3DdzmeYb1E+kT9px9IlRfs40O278L4MGDd/sDHYfITrZcHG0wJQoZctcUVkzpKSLng7TBFPRh4FzRq9LlSQ/VFopeyn5rLuygXB+LNNYoo0G3mwtwwSWlRo1qSuNYHJhi0B0jCeKaVGQ14FhyXIOSvz5AVEFHlotehw3I1XAxqU4SQyNk+ZxL3MjKs8pj7BzlG8MfahiqnsJFT0GxjoyCsJHKJ8jGYDEqQh6Zw1YNLkkyQjDceLnujqaas929v+35B3EWHcSPy0cHFHYhnYpk9YeLyQphs8NO3+6/G66qZj0tch2n8a+GCJbwG56w21hPWMDSGHEMJ8ZJQEFEjoPwMQbyaw7qN93qeEX8pr11y97KTjZZnhRWsPwOrhpC8GdUrbhcBlBLxIGITabXMYdTBwmFuykjrY5Q5014VIqeJ4rfTlBVG8cndUXc0mR4WEIhiW1G3h8q22867JxyGQgDg9KaT8m2dRM2DtIYjeptoKDzGEVfSWbEmsa7hPw5G/+j2fhST3oJT0IYiYqE9sf44tIEHPIG1PHGhTgyAeoKFsXBdSbI/ZnIkukjvDWs3sVJwvxfjBJYUZZ4jvZAZar3SKHLIOqnadr839D0xvaJ7WZh8h/UIJ8tPKS4pfR6cTmfR+eLjo3lRcdjCRflpCbEtGr32w4SjDNLdwnP2uZUh33VZ98w3G4ecRstOCc/OU8m2rJcsufXL8scP1dk2D2US+U8YK6amB3YrA5UJcVsa6vaIhl5QZ6jt2u3y1vWmhjXmGvA1wxhnU452aBqgWC3Hy+M2k4mTFZglcHVgUAVYYh8M0aFO7VFnaFBQXNtidAsFe+fy4IqE5vkaRuCsOxQ1u44dQVQdRe2e3hjyuu6kl1syQ2Ynbn0UYkY8hpJoqIUUpDxHWqVvtO8xe29VOxP3+LvxgZ9rVfXlnAop0v54ZciHyH50W/8Y2UIfLcTuuLwYNBfgSkPNRCQ9TFXWsVTpkQCqfTdVK1B8T2LRG1mFxdUwhsIh2orsquFEZM1HvmRyz5Ima0ZUNsFEGApustLGrZV5587j/ZyDKLYMqLovjLCMP9U4pg1N/RxViwyp9FzzKS00mCPHishVRnwketZdbuabxlrOcyawUjA339Y2klac2gaBdlMZQnbzIiltIQw0OxhT00HFhvxavRXDf7KoeGiaub33Eg+iV4sGnVjjEStd90y0tBYS4okthsGHSjucub+AtE96JXYq+4r6zefZWpxatmcX5LYbBJg1wGKQSuu/28j/qQcf19z/ji62dBmAeqXIpagNUncSuZNeZm4c3a6WwmtHPAZ4W0zuHClvaZtvmnYJVPBDRXkYps9Me99LPQ18ESxCpTi6SiBEdjPUkRNcxtmq8gPi2ly7l2BBgwVGqav2NryDbKSGN9GV6h94e3B04m8fCiyWBPm6gVCNT7wfYPXpzSuRHLD2x2P7Cwz6XZoSrowAb91q7O1tQuKTi1Bx4YeqmtVu7rCo8dd2FuV8X80CFKUvSKqUuqYKa7f57u00w4T1Wlm9ER5ssgyFEh91jbUdtnyNymzGqQZP9kDUc92O9Vgh2gxMqcv1ZzN131pPeBEgzduUkrPHhmD1IdZ21nW2Dpr9rRlrPMkjiXl9QMUjzJAvYAvNn8vzQu92peSvayh+jK6qxJX+o2W4XPUP9YcfLmkHxbWnyMVmwiRyh+VTmqch5phUWhf8oJqeN/GUfUZ2oerS17ZrH4hD4/k5S+EfBDjHnJuMqDvU4ng+6bQ8RGA4sz3q/q7dnlabneWRrdIhKp7enby9mx/MNjaosC/8Aa08YZ2i/3rH/98UGNWb/Xc+WKTKG2+3G0MBOwUxr7aLV+0LTTbMcrEOy15yvEQJgKFuEbeIBCaZ7GBHb5YWiBlGUdCrpnREJ2xL5JNY2v69vXuK4/oOad78UD4qdoPKF8VoS6keZ1BBgKs23TUBZIbYKqSej3YmCBRgbgg1CBtNkAyA77vCe7KTI4OoYQr8y77am6SdGUa7ObQyQARoigejyGLDK0ES+gALhjxa7R3+ANmEnFdIHzNuni0YBL7ippWZMI45DWukihqw+iNcUnEW64HuPJqjaFmDyc0HLoBl8pOZMrJBxqWWJFwxZjsidap9GciiZDXRxTnSzVZmd6KpjXYoWJpCsowm4lb9hFflWDrieQ2e7u7W48l19iVe9IDbc1QixZN1GO+tYUBlEfYiOZtAt207+aWvZe7Kq2AszKT0cyHo5Guezk147fBVGmcRRYBBkaDEJysofy4iSNSF+pcSDFb7LbMf2QIso9qJo7R1Mhgw7MWVJtX1RxBfz2PZUU6wkVi3DA/M9srzYZu1ZYwa3SbHrNz+eq8sSzbXRmWD2HriRLQc5gUEQi5eizOXjUHPzeBjAMUHDDBMCjggIoa2SBBb8spsJABNi02MIM9609mLipJWI2soUrhxzRHQwEfCW69ENJJBYrsIJvWItRzRgWk4H6EuKBNOGQMDAFPUA+BpNscacFMuIxTSMmOtSC9zBBca6hAfLDTvJzKKCh9mJmxr63oA9xs9NdoArynAunPrVY4SUXEXtwt+d8yrZatVpvhNMhhzjzynHLvrMjMwG27WmDM6y4psVr/BlBLAwQUAAAACACaBg9d9LDU0gIXAACcSwAAPQAAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzNDUtYXBwbHkuc2jNPOtW20iT/3mKHiVfkIglXyA3iHOGi0nYIcBistksZjWy3MYaZEmjlgEPwzn7EPuE+yRbVd26+gLJR84uObGlvlTXvauru/3sl/pExPW+F9R5cM36jhitCJ4wk09CFnkRHzqev+IOmPb8rrlpPj/5unevraycnzNzyJzA8afCE/XhJHATLwxE3R1cD+w4cu1Go/muz980LEeM2cUF+/tvdrfCGHdHIdNOJwEbxuGYdY86XZE42Nfc4244jljMo1B4SRhPWRyGiaWxDy9a2PPWS1hr5X5lZXw18GJmRmzsJO7ICy7rrhMMvIGTcJGXcW47fc9GmE7CkjD0BRuErsixTpuurETTZBQG68xk79+vnnxbXSHkoN/I9/rMG0dhnLATeF3BD12bP4gk3ueXjjtVRdZIM6yb2Eu4nfDbRF995g2DAR8S4d2z7bOD4yN7d+/f9uzDzsft3W/27vHnk+0z+1MveAbNvIA/omUvSKYRR6iTQHiXAR8wd+TEbPJ2a06VFyRsst5aVCW8vwDXLYSa4nr05fAwxwffmK5fh96ArRkNA2o4cH9YxIMq9bWuNzyN3E4w2Af9sBMj7UTQRRJPXBCMLVt1eXzN4z0ncbaKkCqtPnFnwONecNcLGPxJeNFVYjuDQbwlC4E4hhroDVQBkcXHTl6CTcbhgMPrPStCVpTX16DdGvvkCdBDz3V8dtJtse7BPjs92WWu73EA6TvTcJJYjB0HLBlx1umw7Z0DNhHAyRGP+SbBYMTRcKiXhzFYu80at82GahQOh2B20EwQHwwm/6hRa6MMSY6fNkkbvcVG1PBsJLWGdaVlscSJL8GmPZBmBLKCzv6UuWEw9OKxYHJEBkbyEseCZgSFdO0gAKPT/RtmWVaNUNGfi4ZhWNikvlBMu4QhCjMT1VxBslFJamAzY7DkraJw+5PhsMbWXPxWFRW1YkCSnXqgUl+siJzYGW/NxSFXObYmmVDQh5wEpROq81kIIyfxNCMMER96Pj/c2d4ql3RBXGnRWyo5icOIx4nHRV4egeqCK2mer1+oQjJebB44Y37ebL192byYad+S7e+ZbTtJEnv9CfgYW9cjx73iA0PaGA8mYylIkP9nUHimkKayj50ze//gsNO1j48Ov7E2a9YqtXsHp1llqza36/bRHjWDFuuAjnQcylVIFTr991RRG7c7MC2sr7+pNNk/ONpDaNSk0azUwlgwgALQaFVqu2fHJxn4xkal9ux0+9tp519V7atK7d5B97fTzvbeN6p9XcXq8Ev30+727qcO1r6Zxap78B8dBfktko1+BpRnB+wMFEif1SNQY3qrkUtCHwXy6fNYvqNDQrEpMLuO7/8oGCWqwp80CAEWIVsJ9CU1VRxzl3vXXMGjmhkAy4yuVrU3JIOKoNdXnP12QCv3dh13xNUcECUKXRxNKiup/RqYWeBGU12+DbgAMsFViYSp+titqUmKBdk4a2M+xl7ypdhLEV7tlc9aqzWgxQ3RptraJBmabzWjOs3n8UUW4FhuZWZXc0bn1nETU4ST2OWmGDkRZ1lvNgxjmipG+cQCAQaCxNlDStXK/Ph+GlUVfHEyisPJ5UhpqD8RI+Iqc2LOopiTG8OJHIchIDgRJJ7rcyZxYknMuZWaMU/QSUFINChpdRtVmolJfA1qUYR28rFzBIyNpjBhESUCXFRGw9AZe/40J+A3ziOGTkzU2IC7PqgGzUdhjL4fB+W3iLTAQuKVKLIGJ9F0onnmBa4/AQemzQ2yUvO7soULfne4N3Zw8tNLoY03QMlTuOnmQY/AmeUcpuL1RuOi6k0dXzbSX28YRqH3HKN0B42tFA3C0QN5QVeYnLPyTI66kc0fhBm1wLebEfh9vWnc5QboDZlecCvsBYxUy3wrzMoGe48fMU8mccDM5la5LzS31Bz/Sxsb9mPuXBUbMRmKwF+hVGLimaahCu9THMvUNTPcFQII5L5E8Z4nrk5hwp/qmZ/LyR/qvxQAlslQ07aU0AUMpoK2LA5InSTxpOzTa40auYM1Q3+RQjBqG/MLG7WGUaUjrZ8hZx8ksY/Myf3SGhuintdYJUpYg4WHfPo+ipUTJC+4ZkhUa3KIZqO1YSznQTqfzmcBAphXrqLLCgXGS2pfYpBytilpNaagtF69vqixBXAe5C95ajmnJ2GkP8iw5TzAuGA+/QsK56jAjOTPYmd6yv/8MTWei6YKUH6eskLgB6vlkqriulZqa7EUJjEeoCcWOCFW40b1PaPgmX6fg+RL3jbmf9rgHWMPvb8cIuA3djp4zrpSLxg47ZWR+P0GI+0loxM0eN0ogGN6TiyunnBJa8jKgt+VLEXtr29cKDeu/CB+cV/w2T7zDDeFUSuwuNl6M+NXVePmq5YcUPF8a6b+taxHNTRS/s+0et2QrQpyKKpiNSzLjbnQYe1BW56r1DJiX+x9XuKaZgP/zVXwOR6noBZAUkHZc5EWlQsnuoJQi5IpNqtVxHRR0RSlZ3phdGPWW+VR2D/rs/IVx1N5rpJtFUO+fxZVFSh+B56P8VtLYvGZrF2eavQhZIQCyxXXlZgcs0IQXtZw2UMCD/t/cDexxXTcD/2ajIh7AUS7KltZU4/DhqITg7XC0/IVQQYJu6ePb9LAJA2Cqq+PhZlh5zYyhVGRSOXtcRAJjHoEg6xlU2/h6ZGQsHv6+C6d2eRcWX55LLx3GWYtx60VJrLS8yOhIQj1uN7YqFWMdub9kVARVPr4WkFVxlV+eQjeEqWnrHVduGHMbeqSgbCiaTWzXM7kq5R2L6BUtg1rdbA3WNOk2WwnCEKZHhQUL6jS+BIW8DixqQKwqexZfsFiz5oknq8gz0mSI7weZY790BnIRVrMdczmb1IDY1P6AOyLMxRUsDqT1Gr4pLrY3B9mSQaBJKspLuIu9CvjY2GpjTjZmDuz/dAl8vT50Kje12qEhJFNIwTaE+woDDisU+ndQjJg9aSKN/NJJXY8wVl3KhI+7tzCsk5zia9EOFtIhRoOIsgJrMtnCJHlkhQcX8cPIydd4WPxWyBYNtbll1HyrbIsl8bY8QLdYOaHIhkk7hiQSEVvbceXkzFMdydUoxvFdhZ4VNtRDXTNNGVqWaOoYeLFfNA+iycQJmJiuH2Ss3ZB974juKm89AIgvjPuDxx2u4lzmH6L693lMKWP/2GUxk7gDbn4cQC+0+eoWMByZ+InbU17gIUwbdWYQ+rR1jD7ASYNA6bdoC0GPao3fWF/gZJR8kYDapNp6bbUftuwgKehfw0zPfaBsQQEOGoBP45AN6DHjH1mINWWAamFsOQbQITm/Sk4sFQrQLWhjYRndQ73aVlMXaQUUoUMb5AEnKh18CjWnucmp7QDIBunPLfCiAd61RvWcOHgewFHVho51VwAexHuuaIL02swFCascMSCoSoVQ1sDpkPluZZq3QVqVN4SQoViK3ittpDj5mSnZp6auF7OnUreVRKqKT6m5C+agZ0GK+WWiI/JFtT5w0oJ4VwKcgD7vMkMFcJyItwS0pHamirMWUxTlwy5J2M9ttKpjDhtQ3titYSUOigIs2xcNmSd+C34LAgsVSn4eyubxmTZEmiYsh1wP3EUOKcvZkCaMxCNJSAH3nAolTjDEIt4DHSp8vmdszlCmoS4LuhXFKPGlGUx1O6oJfmD+14vuVPspOeMT/Ito5NecxzvtQXSQw+f4lQdfahRyLCZyq+djVy/88HAUprumVbqBCiZiEm7gN3yHtDCJLTbRRKIxybh355Di1Gy1jKfN4srNieZoJC0z9tnu580ZH1BBXH9DVUH3W4JJy8WaJvgNDTrjxAmvKH2snF7d7t5e6/RkLc0mkUNbSl5cDiwssPpXjO1h4QK632J2T27Q3PrpauM3upFVoSLDXzfLOKmlKKivsDhqvre12a7FVk6o7HQgehp39FXRWkovhtCDIhYQQQIJqjZNkYDtq1tppaOscEjgtF4EpgYipoQngSJBzOSGC2PRPFMSS+YOVSCWJ0eH5+1tec6nS/RIaSnxbv2/G5nu/vJ7h5/Od3twOLwXjPqlqWxFy9YdDMwgDrqgL0p87/X2d/+cnhmdzr27m5bVtT7E88f1BFpWOR7gahzbl66rrlutayWCSK85M16FPOhdytRpWqAl0K5o4dN83kJ/L2m2Hl+zn5h5i3gQeUau7jYwi0RZZPyoMtnTwhU1sKuBm70wIwBE2W8ydK+H160VDc65QKxtYfD7Hw5ONwrEzSz/AV8JKPSdg/sWvWCz9tHB/ud7lna4ZFLaxDX9lfoQ0jV0xpgHXEkO5SjGkBhPMZDQul7nTZ4zDUrnC1SDIHA9nJBN3Jo5HXzmj5EDGYUhxjwWMltMtOVGsBsOJiMI2yAOrP9ldBNT/xIpVZtUnLtJLSBLieeQrTOej0UjWl6QTRJHn/mKe0WThLsJ4fOSouBL8vzD1k9zMmValxlsg/1Ab+uBxOfzGf3+PPn46M2uSjzY4OZnUOQANgXM7862ASKh2IEawtzEE76sNYwx34YXAIgczxyYjBiiADBLsdBaDp9VE9fUK+xE7ujdvzqXQOgjpMJxF70QpV7J90WGAM8yM/D7o69f3DaPYPn7cODj0f23tfj0z142z3e69gnh9tn+8enn9vr1Ptg/uGpXkCeKomnthIpLFVUhoqWarSRCOrXTBeBI2+YFOtBiJl2SqV5fod97q1QK7YDJVvQrqCGWmG+/yW3ce35r/hxJzl//iu4Jma6UCJNEF5QuwETMGntOcJhrQ8vmkXHkM0sQ9Zb/YeA6X5/++AQvhrw/x39FR6C3ioARPQA4ocy2rlNaNXIgDXUHOtlMUJJ4RekFJT64V+6tCsrrqxZrryyjQxCU2YUa9JgH+pST1RuQAFTRnUJJTD/pVygpOcztu94yWg48Yt+F5HGRYSwSlrGQpwMoEmS2t5xC6wmHHsJTHEYEUUQRSSwRAZTonam43sOeXVzCHaDh5bCoKK7CFWt/2Tzh4EvAwV1j8Uxa7oQzWesC7EgpgbwlFFwyWNSxzl8QVjYtPlPsSbtPEzBwcjgTIVMPC0YsvWThmwtGbP5swb9DkQggHamfPBkiCh4Zj92Ane0jPifMfo8Nsxi9IwdgAu69gYTMFMYM3KEgFVYMoKpCbwTG4fXeLglBHc6gVhGdoSHmF+CeQM+PviTxBvz+Sp86cJC4akoQmDzuejGoRB/QBTxZGNlEKHNgjHBDw9D3w9vnm7QDKSJQy+wUVzBBAIPBz3VuBlEM4xAlt5feW56dviI82gU+vzpDDaDOH/AmNOZJbMPIcSVeLJRy2DnD+0N3RBWUU81pDc0ER6PcR962YitnzNkS1r8cUHIps+vYcKHNgkY+YwR57NP8ykm5vUM3PoPgoMJlE7GYVV+vO6zc8VpBDVSjV3xCNYLgjks4JdA6TWv09UGNBtFbIVWAeEJQQOUODxiaAaDPIkgShCpAS4oE49I2pHLQT2/CLEwyGLv3/dWT771VrN9ITEVyzeDVPoX07SYgcEoDJMwlFaAzhYEmtfnzQtDJpgpiTCThLBE5HsJBXB6tn9EOeWAyzpdg7hZy7dyMG8VGXgU4BWmdiIYgVIfGG5rhTwTisILJuowB2Ug2tC6cVHNgWI2CKHMyXTKqtbFvLSlrFu/mJd/lHUbad0zeXw0G3MA0g4gNJfrB9wjwCky5uneFlhrPAap/wVzdpYSSkHRGKonSbFOyIKu3YLpJXROU+XmsyywGrjGzBzNWk4plOek1YhXMlkM7MZtr0LWfXZrLAhT00gTIAOUFo0vQF30mKOP4LTbAhUy/YbVIAvJI1C7wJDr9+IKSi6Sdmghka0OqtkB6iazMm1QA9yiVUdi6yN5GeEl4DXgl8AuYEPs3YK2tLXiUGbrrWD/eAP/m43so6kQkHamqRFhlUi8hO8srYrPWcIUXvKUXiklscz2Uiq/3wbVXuuMzfUCZBSmwtPC1sVCi0UgP81IF5okNFtovCXtbTbW1t7VyHbh0zAWASx1ykyw1LGol1d8WtiFvMXTLJIttjQB6FLZcho78RVtqmprlLaWTqXNiNUycZ1meJcRJy1gqN1JePeM8hSb71uv7xle/zo47NjUx8iPpD28KTEH2B2isPlh477+dk7CGh3b5oe3WHmHjmzzQ7NBfTbwsVnJNqONSkMrmhtRrmxjk5EWp+b1HYmJ+UmJh7NpWTKinPVBLDA7lLWbl5pAao53/mXvy+eTdilJLVNCBiaOVf5QSxPDlBZWnaqJ4UKFOYgX41R2ZIUsJqaUsjQg3qvEzeI0Y/xMHrrHo/nAvzCg1IfcoXQS3HDh5BXSiwgklpzRVWksOsHwfyiOrFJRMj95bcaRa6psrmppjQeV9Cma3v9z5csqZU6MxCXTYH+zS6CLmX+y573V/8QKcKxv4X9vtboNUdyQQFtUqaRLvJEibXA2QfiA5J80Q7iUL4/MGf6INpT701EPCKF9M+1b5BuFC3j5UzFPQSMmbrLj37TUAIuOb6fk9OBvwRbOvFilAGHgCcyIjPv+dHM5iKKXeMReHkQeCa49LlFMG6+k5nOMQG2Sxuwhs8URhnqZBF6CYNPdvcceS6ELYD6QyU4yfHYBHYqHu4jMGUAVegrfwtdd0LI0zPiVeo85KPFABdp4L5UnX6JdrNFdXxjFEAMWmvnFrDbTCds6e/jSVx7/VE9qlHk9d+OehpVX0mHMMoAZDB5xu7wwyLLArIxAPAkCCk8yopfu7f4QwbkMSMukVuXpePvSD/vwRbfDbOC0TZkQXYDDKYoJ3y3U/jg5BKWoYEC1uavGC863urb0hpdm1L4Dxvx7XiUYxpMg+4iBFsOYc+WsiuJcccgLh7Zc/kiTXyaAgwDRXXSNW6sp7Eg1jYUQqlezH9vv4cvbM5AqRMNcRatXWGd6mIqyPWHTnUmIkh8ge9nV38cSsOQsfgYilews8nJJasvFoC0zAXblGMhDVCw/TqxwkM5hMRnF7ajv7EP7DN/XR+0WzHR6zJmWbK54xOEW/GWQ+snp8cfTTre78QrCg+qhlmxiYhuv2P/813+rVM2cq7zlrEJhiw2RzqFsMDEKb0AfaKPjeB3PF2MSBePysTo3AktPi9EPOlDod9x6qZJ2gq7PHq9jeoLCBsH6Iazw4W0wcQHoXzwOGS9lk7BHdmc3CRNAmTJClI+wELkzoIalAQGsFXDnBfsBZLywG8nckbzES6EAgsPMfeyN5dVlhEE3ka+9cCIKV5/pZzEc+vUCdMmYiDTxYvE04uxmFMKSOAz8KYsmcYQvN44AFIE633E5+33WPaorrb9nv1sBfNpWV5YrJ27U73b0AiWR7JcvaJUEVutzQLD0qx5lXsnfxVB3aOUvdViS0Jy+ILxBGtW2VWH0ocf9gbo2N4l5TQlFzn/ydjSOhke0KzekaxkWAOrSCxCYCsVlv7q60j1zizoThGqQownydXwRAjG0IgRRZHKMuQvijqEIxEyhuey9icBM9vvywOj3cpuHQ5ffUxyz2J7y2QIFKtWqYl7yPGtOWxqkMBXOoARiJ7gqaB0YxfThdKnKcqLtkQ5Cn4zPaCBkgR7+tNAVQEwPbxUNu2ADoheMOVAUXOL5A37NA3mTOqdJri8AzoRuSaJzgjVS/S0RdpCQuxeK6dLN1tVRgnoQskqqfwtbyUOPqCwCZBjgDCP1jShIN/tF+QdgqGd5MzZtm8IVmJAFzntztmepwXGzDn4r3bihIjKaOdsjpe2HdKmDRB7laWHpfMYhquUYNBFkgYmKGy9AfkrHSNMg/egO1NJgckGGQuwFtKbFPWPXRy8iDTyOvUEYW4v8/7eVFXeE5+Ff3j6QB1gcJa+syPVacY4AdwkuZWBpK/mPXqF+baYFjFn1JYH3yv8CUEsDBBQAAAAIAJkGD13Qw4/U+AIAAHwHAAA9AAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3M0Ni1hcHBseS5zaMVVXU/bMBR996+4C2htpSYVsE1TB3uCV1QNTRMCFLnJTWPNsSPboY2A/75rJ13LyirEHpaHpHbv1/G59/jg3aSxZjIXaoLqHubclsyigxgbDbWoseBCsiyH6PDhaBofzn6cP0WM3dxAXIDTWtqJzbTBNMvv8zTjKhc5d5jULdzdweMjPDAAzEoN0bdGQWF0Ba5EuLq8uLKOO6FVfI6ZrmowWGsrnDYtGK0d8MKhgZnRC4PWwoePSQRf3x/7eCvh4Jg9MVa3rtTqBGI4PR3MrgcsJKi5K6WYg6hqbRzMaEmm9IazsBhGeyuPRszhypGx90kM8jz1G0NUmc6FWpxFjSviz2THtMzJbpBjAVKTnUfCDQ49gmlINpreKqCnzx+gTaCrIPK/epcUZZEWjcr8mVhfRudna8zIr8NCqJLGCZn43dSDTQshMZU6C2c5fDla+F9G41DEqIsrii60sHCpFYI2YZ14GHTu/XZfvH8MF5aIa63D6oIYoFxcKULjPeCvKPp0lc4bibtAuv0Ois8/9K/RBnpfT4IrAtwZD7tPb2TQNUb18W/VgClcvpKSvj9sa2/Vv5HUo1C88hD3kvAWVrfC/y8O38Zi53UAszCncJIcnQBNGc8ktxYtkWe1vEcIawilBMSWRMLoZlF6cvrodh1sWdLhQE6qYTjJhQXfGw2NLnzDhbBeNVxJ2J3hygpUbl02rnjmZAv8dyilTcXluhEqnpVCIQnQUjc02HMsSCBgu/VGSc/fpqybLXbu6GjWnejNnGm3Tv9V/YyrDGsHF+FDR7Htv0ma1Lp+3hae6dEfTK8J2JkRRp3jpUso8Mo2ZevmT5ZGOOzkzr9I/WrJMxyS9RhotMZwNBrDrhKGAEYoaqmNZH8CXtdSYJ6QBUpK6ofzedIXfKSX3Pa5r8XOfreDvytc0cE6zPdfSF+gH4s5zdVP8F1f6EaF+LPrrbuELpA2qIZvs70hGfNXG9vcb1rJdhr1GwDJpHM3jYq9c0wDopwg+m0ZsV9QSwECFAMUAAAACACxBg9dwD3OqvgBAABZAwAAQAAAAAAAAAAAAAAApIEAAAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvZ3NsaWJfaHcvUFJPR1JFU1M0Mi5tZFBLAQIUAxQAAAAIALEGD1117CLYHgEAAK4BAAA8AAAAAAAAAAAAAACkgVYCAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9nc2xpYl9ody9TVEFUVVMubWRQSwECFAMUAAAACACxBg9dVZbI99YAAAB8AgAAWQAAAAAAAAAAAAAApIHOAwAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvZ3NsaWJfaHcvYW5hbHlzaXMvbWF0Y2hpbmcvZ3NsaWJfaHdfbGlzdGluZy5jc3ZQSwECFAMUAAAACACxBg9ddv6kmKYCAACJCAAATwAAAAAAAAAAAAAApIEbBQAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvZ3NsaWJfaHcvc3JjL3BzMi9nc2xpYl9od19yZWNvdmVyZWQuY1BLAQIUAxQAAAAIALEGD10zTxIIdQIAAFUFAABOAAAAAAAAAAAAAACkgS4IAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9nc2xpYl9ody90b29scy9ydW4tZ3NsaWItZnJvbnRpZXIuc2hQSwECFAMUAAAACADGBg9dHketQdcAAABZAQAAQQAAAAAAAAAAAAAApIEPCwAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvbGliZ2NjX3Vud2luZC9TVEFUVVMubWRQSwECFAMUAAAACADGBg9dQZF/mqgDAADHDQAAYgAAAAAAAAAAAAAApIFFDAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvbGliZ2NjX3Vud2luZC9tYXRjaGluZy9jYW5kaWRhdGVzL2xpYmdjY191bndpbmRfbGVhdmVzLmNQSwECFAMUAAAACADGBg9dekXHmUQBAAABAgAAOgAAAAAAAAAAAAAApIFtEAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL01BVENIRUQvbWF0aGZwL1NUQVRVUy5tZFBLAQIUAxQAAAAIANMGD11f8ldKbgUAAHwQAABNAAAAAAAAAAAAAACkgQkSAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9tYXRoZnAvbWF0Y2hpbmcvY2FuZGlkYXRlcy9tYXRoZnAuY1BLAQIUAxQAAAAIAMYGD12Wfv9nMwEAAF0DAABVAAAAAAAAAAAAAACkgeIXAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svTUFUQ0hFRC9tYXRoZnAvbWF0Y2hpbmcvY2FuZGlkYXRlcy9tYXRoZnBfbnVtdGVzdC5TUEsBAhQDFAAAAAgAxgYPXXjuwUyLAQAA8AIAAFUAAAAAAAAAAAAAAKSBiBkAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9NQVRDSEVEL21hdGhmcC9tYXRjaGluZy9jYW5kaWRhdGVzL21hdGhmcF9udW10ZXN0LmNQSwECFAMUAAAACADGBg9di+zD3qQCAABNBAAAKwAAAAAAAAAAAAAApIGGGwAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL1JFQURNRS5tZFBLAQIUAxQAAAAIANQGD108rmNgPgcAAJAOAAAwAAAAAAAAAAAAAACkgXMeAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svU0hBMjU2U1VNUy50eHRQSwECFAMUAAAACACxBg9dsDKc+BQDAAChBQAAPAAAAAAAAAAAAAAApIH/JQAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL1dJUC9jZHZkX3JwYy9QUk9HUkVTUzQ1Lm1kUEsBAhQDFAAAAAgAsQYPXRf4R55wAQAAHQIAADgAAAAAAAAAAAAAAKSBbSkAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9XSVAvY2R2ZF9ycGMvU1RBVFVTLm1kUEsBAhQDFAAAAAgAursOXX9a3rSpCgAAqTUAAFcAAAAAAAAAAAAAAKSBMysAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9XSVAvY2R2ZF9ycGMvYW5hbHlzaXMvZnVuY3Rpb25zL2NkdmRfcnBjXzAwMTliZTcwLmFzbVBLAQIUAxQAAAAIALEGD13zCLBS0QAAAJwCAABVAAAAAAAAAAAAAACkgVE2AABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svV0lQL2NkdmRfcnBjL2FuYWx5c2lzL21hdGNoaW5nL2NkdmRfcnBjX2xpc3RpbmcuY3N2UEsBAhQDFAAAAAgAsQYPXRei4FK8AwAAxQoAAE0AAAAAAAAAAAAAAKSBlTcAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9XSVAvY2R2ZF9ycGMvbWF0Y2hpbmcvY2FuZGlkYXRlcy9jZHZkX3JwYy5jUEsBAhQDFAAAAAgAsQYPXQtKRK83AwAARQcAAFoAAAAAAAAAAAAAAKSBvDsAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9XSVAvY2R2ZF9ycGMvbWF0Y2hpbmcvZWVfYWJpX2NvbXBhdC9jZHZkX2xlZ2FjeV9jb21wYXQuaFBLAQIUAxQAAAAIAMO7Dl2KQxuFqAIAAPQEAABMAAAAAAAAAAAAAACkgWs/AABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svV0lQL2NkdmRfcnBjL3JlZmVyZW5jZS9jZHZkX3JwY190YXJnZXQuYmluUEsBAhQDFAAAAAgAsQYPXaIXGddsAQAAigIAADoAAAAAAAAAAAAAAO2BfUIAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL2FwcGx5LW1hdGNoZWQuc2hQSwECFAMUAAAACACaBg9d+BUyc4YMAAAXMgAAPQAAAAAAAAAAAAAApIFBRAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3MzMC1hcHBseS5zaFBLAQIUAxQAAAAIAJoGD125OUcxVwwAAMggAAA9AAAAAAAAAAAAAACkgSJRAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczMxLWFwcGx5LnNoUEsBAhQDFAAAAAgAmgYPXWNqjcoHCQAATxgAAD0AAAAAAAAAAAAAAKSB1F0AAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzItYXBwbHkuc2hQSwECFAMUAAAACACaBg9dFWtXF8QHAAAtFAAAPQAAAAAAAAAAAAAApIE2ZwAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3MzMy1hcHBseS5zaFBLAQIUAxQAAAAIAJoGD13atrEdpgkAAPAbAAA9AAAAAAAAAAAAAACkgVVvAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczM0LWFwcGx5LnNoUEsBAhQDFAAAAAgAmgYPXbVYQKa8BwAA+xMAAD0AAAAAAAAAAAAAAKSBVnkAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzUtYXBwbHkuc2hQSwECFAMUAAAACACaBg9dGUoAgJQFAAA7DgAAPQAAAAAAAAAAAAAApIFtgQAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3MzNi1hcHBseS5zaFBLAQIUAxQAAAAIAJoGD13lp4he3wQAAOALAAA9AAAAAAAAAAAAAACkgVyHAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczM3LWFwcGx5LnNoUEsBAhQDFAAAAAgAmgYPXWDBzB49CQAA0xkAAD0AAAAAAAAAAAAAAKSBlowAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzMzgtYXBwbHkuc2hQSwECFAMUAAAACACaBg9dPloGIjYHAACgEgAAPQAAAAAAAAAAAAAA7YEulgAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3MzOS1hcHBseS5zaFBLAQIUAxQAAAAIAJoGD12vAhVTzwQAAJYLAAA9AAAAAAAAAAAAAADtgb+dAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczQwLWFwcGx5LnNoUEsBAhQDFAAAAAgAmgYPXUuSpWsTCQAA9hoAAD0AAAAAAAAAAAAAAO2B6aIAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzNDEtYXBwbHkuc2hQSwECFAMUAAAACACaBg9dPRB4m+cJAAD1HQAAPQAAAAAAAAAAAAAA7YFXrAAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3M0Mi1hcHBseS5zaFBLAQIUAxQAAAAIAJoGD12mO/qlJhEAAGI1AAA9AAAAAAAAAAAAAADtgZm2AABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczQzLWFwcGx5LnNoUEsBAhQDFAAAAAgAmgYPXVmzxcYfDQAA2iMAAD0AAAAAAAAAAAAAAO2BGsgAAFNORVNzdGF0aW9uLURlY29tcC1SZWNvdmVyZWQtUGFjay9zY3JpcHRzL3Byb2dyZXNzNDQtYXBwbHkuc2hQSwECFAMUAAAACACaBg9d9LDU0gIXAACcSwAAPQAAAAAAAAAAAAAA7YGU1QAAU05FU3N0YXRpb24tRGVjb21wLVJlY292ZXJlZC1QYWNrL3NjcmlwdHMvcHJvZ3Jlc3M0NS1hcHBseS5zaFBLAQIUAxQAAAAIAJkGD13Qw4/U+AIAAHwHAAA9AAAAAAAAAAAAAADtgfHsAABTTkVTc3RhdGlvbi1EZWNvbXAtUmVjb3ZlcmVkLVBhY2svc2NyaXB0cy9wcm9ncmVzczQ2LWFwcGx5LnNoUEsFBgAAAAAmACYAzxAAAETwAAAAAA=="
Path(sys.argv[1]).write_bytes(base64.b64decode(payload))
PY

python3 - "$CHECKPOINT_TMP/recovered-pack.zip" "$REPO" <<'PY'
from pathlib import Path
from zipfile import ZipFile
import sys

archive = Path(sys.argv[1])
repo = Path(sys.argv[2])
prefix = "SNESstation-Decomp-Recovered-Pack/"
mapping = {'MATCHED/gslib_hw/PROGRESS42.md': 'docs/PROGRESS42.md', 'MATCHED/gslib_hw/STATUS.md': 'docs/checkpoints/recovered-pack-2026-08-14/gslib_hw_STATUS.md', 'MATCHED/gslib_hw/analysis/matching/gslib_hw_listing.csv': 'analysis/matching/gslib_hw_listing.csv', 'MATCHED/gslib_hw/src/ps2/gslib_hw_recovered.c': 'src/ps2/gslib_hw_recovered.c', 'MATCHED/gslib_hw/tools/run-gslib-frontier.sh': 'tools/run-gslib-frontier.sh', 'MATCHED/libgcc_unwind/STATUS.md': 'docs/checkpoints/recovered-pack-2026-08-14/libgcc_unwind_STATUS.md', 'MATCHED/libgcc_unwind/matching/candidates/libgcc_unwind_leaves.c': 'matching/candidates/libgcc_unwind_leaves.c', 'MATCHED/mathfp/STATUS.md': 'docs/checkpoints/recovered-pack-2026-08-14/mathfp_STATUS.md', 'MATCHED/mathfp/matching/candidates/mathfp.c': 'matching/candidates/mathfp.c', 'MATCHED/mathfp/matching/candidates/mathfp_numtest.S': 'matching/candidates/mathfp_numtest.S', 'MATCHED/mathfp/matching/candidates/mathfp_numtest.c': 'matching/candidates/mathfp_numtest.c', 'README.md': 'docs/checkpoints/RECOVERED_PACK_2026-08-14.md', 'SHA256SUMS.txt': 'docs/checkpoints/recovered-pack-2026-08-14/SHA256SUMS.txt', 'WIP/cdvd_rpc/PROGRESS45.md': 'docs/PROGRESS45.md', 'WIP/cdvd_rpc/STATUS.md': 'docs/CDVD_RPC_STATUS.md', 'WIP/cdvd_rpc/analysis/functions/cdvd_rpc_0019be70.asm': 'analysis/functions/cdvd_rpc_0019be70.asm', 'WIP/cdvd_rpc/analysis/matching/cdvd_rpc_listing.csv': 'analysis/matching/cdvd_rpc_listing.csv', 'WIP/cdvd_rpc/matching/candidates/cdvd_rpc.c': 'matching/candidates/cdvd_rpc.c', 'WIP/cdvd_rpc/matching/ee_abi_compat/cdvd_legacy_compat.h': 'matching/ee_abi_compat/cdvd_legacy_compat.h', 'WIP/cdvd_rpc/reference/cdvd_rpc_target.bin': 'analysis/matching/cdvd_rpc_target.bin', 'scripts/apply-matched.sh': 'tools/history/checkpoints/apply-matched-2026-08-14.sh', 'scripts/progress30-apply.sh': 'tools/history/progress/progress30-apply.sh', 'scripts/progress31-apply.sh': 'tools/history/progress/progress31-apply.sh', 'scripts/progress32-apply.sh': 'tools/history/progress/progress32-apply.sh', 'scripts/progress33-apply.sh': 'tools/history/progress/progress33-apply.sh', 'scripts/progress34-apply.sh': 'tools/history/progress/progress34-apply.sh', 'scripts/progress35-apply.sh': 'tools/history/progress/progress35-apply.sh', 'scripts/progress36-apply.sh': 'tools/history/progress/progress36-apply.sh', 'scripts/progress37-apply.sh': 'tools/history/progress/progress37-apply.sh', 'scripts/progress38-apply.sh': 'tools/history/progress/progress38-apply.sh', 'scripts/progress39-apply.sh': 'tools/history/progress/progress39-apply.sh', 'scripts/progress40-apply.sh': 'tools/history/progress/progress40-apply.sh', 'scripts/progress41-apply.sh': 'tools/history/progress/progress41-apply.sh', 'scripts/progress42-apply.sh': 'tools/history/progress/progress42-apply.sh', 'scripts/progress43-apply.sh': 'tools/history/progress/progress43-apply.sh', 'scripts/progress44-apply.sh': 'tools/history/progress/progress44-apply.sh', 'scripts/progress45-apply.sh': 'tools/history/progress/progress45-apply.sh', 'scripts/progress46-apply.sh': 'tools/history/progress/progress46-apply.sh'}

with ZipFile(archive) as zf:
    actual = {
        n[len(prefix):]
        for n in zf.namelist()
        if n.startswith(prefix) and not n.endswith("/")
    }
    expected = set(mapping)
    if actual != expected:
        raise SystemExit(
            "checkpoint mapping incomplete: "
            f"unmapped={sorted(actual-expected)} "
            f"missing_from_zip={sorted(expected-actual)}"
        )

    for source_rel, dest_rel in mapping.items():
        data = zf.read(prefix + source_rel)
        dest = repo / dest_rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(data)

for path in (repo / "tools/history/progress").glob("*.sh"):
    path.chmod(path.stat().st_mode | 0o111)

for path in [
    repo / "tools/history/checkpoints/apply-matched-2026-08-14.sh",
    repo / "tools/run-gslib-frontier.sh",
]:
    if path.exists():
        path.chmod(path.stat().st_mode | 0o111)

print(f"checkpoint restored: {len(mapping)} files mapped to canonical paths")
PY

# Recreate the latest canonical CDVD WIP runner/scorer state from the archived
# Progress45 + Progress46 scripts. CDVD remains WIP; this is not a MATCH claim.
bash tools/history/progress/progress45-apply.sh "$REPO"
bash tools/history/progress/progress46-apply.sh "$REPO"

mkdir -p docs/checkpoints/recovered-pack-2026-08-14

cat > docs/checkpoints/recovered-pack-2026-08-14/PACKAGE_MAPPING.md <<'EOF'
# Recovered pack → canonical repository mapping

The master checkpoint script embeds the complete
`SNESstation-Decomp-Recovered-Pack-2026-08-14.zip`.

Every regular file from that ZIP is mapped to a canonical repository path.
No package file is silently dropped.

- MATCHED source goes to normal `matching/`, `src/`, `analysis/` and `tools/`.
- CDVD remains WIP under its canonical candidate/header/analysis paths.
- Progress 30-46 are archived under `tools/history/progress/`.
- The old `apply-matched.sh` helper is archived under `tools/history/checkpoints/`.
- Package README/status/hash records are kept under `docs/checkpoints/`.
EOF

python3 - <<'PY'
from pathlib import Path
mapping = {'MATCHED/gslib_hw/PROGRESS42.md': 'docs/PROGRESS42.md', 'MATCHED/gslib_hw/STATUS.md': 'docs/checkpoints/recovered-pack-2026-08-14/gslib_hw_STATUS.md', 'MATCHED/gslib_hw/analysis/matching/gslib_hw_listing.csv': 'analysis/matching/gslib_hw_listing.csv', 'MATCHED/gslib_hw/src/ps2/gslib_hw_recovered.c': 'src/ps2/gslib_hw_recovered.c', 'MATCHED/gslib_hw/tools/run-gslib-frontier.sh': 'tools/run-gslib-frontier.sh', 'MATCHED/libgcc_unwind/STATUS.md': 'docs/checkpoints/recovered-pack-2026-08-14/libgcc_unwind_STATUS.md', 'MATCHED/libgcc_unwind/matching/candidates/libgcc_unwind_leaves.c': 'matching/candidates/libgcc_unwind_leaves.c', 'MATCHED/mathfp/STATUS.md': 'docs/checkpoints/recovered-pack-2026-08-14/mathfp_STATUS.md', 'MATCHED/mathfp/matching/candidates/mathfp.c': 'matching/candidates/mathfp.c', 'MATCHED/mathfp/matching/candidates/mathfp_numtest.S': 'matching/candidates/mathfp_numtest.S', 'MATCHED/mathfp/matching/candidates/mathfp_numtest.c': 'matching/candidates/mathfp_numtest.c', 'README.md': 'docs/checkpoints/RECOVERED_PACK_2026-08-14.md', 'SHA256SUMS.txt': 'docs/checkpoints/recovered-pack-2026-08-14/SHA256SUMS.txt', 'WIP/cdvd_rpc/PROGRESS45.md': 'docs/PROGRESS45.md', 'WIP/cdvd_rpc/STATUS.md': 'docs/CDVD_RPC_STATUS.md', 'WIP/cdvd_rpc/analysis/functions/cdvd_rpc_0019be70.asm': 'analysis/functions/cdvd_rpc_0019be70.asm', 'WIP/cdvd_rpc/analysis/matching/cdvd_rpc_listing.csv': 'analysis/matching/cdvd_rpc_listing.csv', 'WIP/cdvd_rpc/matching/candidates/cdvd_rpc.c': 'matching/candidates/cdvd_rpc.c', 'WIP/cdvd_rpc/matching/ee_abi_compat/cdvd_legacy_compat.h': 'matching/ee_abi_compat/cdvd_legacy_compat.h', 'WIP/cdvd_rpc/reference/cdvd_rpc_target.bin': 'analysis/matching/cdvd_rpc_target.bin', 'scripts/apply-matched.sh': 'tools/history/checkpoints/apply-matched-2026-08-14.sh', 'scripts/progress30-apply.sh': 'tools/history/progress/progress30-apply.sh', 'scripts/progress31-apply.sh': 'tools/history/progress/progress31-apply.sh', 'scripts/progress32-apply.sh': 'tools/history/progress/progress32-apply.sh', 'scripts/progress33-apply.sh': 'tools/history/progress/progress33-apply.sh', 'scripts/progress34-apply.sh': 'tools/history/progress/progress34-apply.sh', 'scripts/progress35-apply.sh': 'tools/history/progress/progress35-apply.sh', 'scripts/progress36-apply.sh': 'tools/history/progress/progress36-apply.sh', 'scripts/progress37-apply.sh': 'tools/history/progress/progress37-apply.sh', 'scripts/progress38-apply.sh': 'tools/history/progress/progress38-apply.sh', 'scripts/progress39-apply.sh': 'tools/history/progress/progress39-apply.sh', 'scripts/progress40-apply.sh': 'tools/history/progress/progress40-apply.sh', 'scripts/progress41-apply.sh': 'tools/history/progress/progress41-apply.sh', 'scripts/progress42-apply.sh': 'tools/history/progress/progress42-apply.sh', 'scripts/progress43-apply.sh': 'tools/history/progress/progress43-apply.sh', 'scripts/progress44-apply.sh': 'tools/history/progress/progress44-apply.sh', 'scripts/progress45-apply.sh': 'tools/history/progress/progress45-apply.sh', 'scripts/progress46-apply.sh': 'tools/history/progress/progress46-apply.sh'}
path = Path("docs/checkpoints/recovered-pack-2026-08-14/PACKAGE_MAPPING.md")
with path.open("a", encoding="utf-8") as f:
    f.write("\n| Package path | Canonical repo path |\n")
    f.write("|---|---|\n")
    for src, dst in sorted(mapping.items()):
        f.write(f"| `{src}` | `{dst}` |\n")
PY

cat > tools/history/progress/README.md <<'EOF'
# Historical progress scripts

All Progress 30-46 scripts from the 2026-08-14 recovered checkpoint are
preserved here.

Archival presence is **not** a matching claim.

- Progress 42: closed GSLIB hardware 7/7 strict checkpoint.
- Progress 43-46: CDVD WIP/compiler-fingerprint experiments.
- Progress 30-41: earlier development scripts preserved for reproducibility;
  individual scripts are not automatically strict byte-match claims.

Current authoritative matching status:
`docs/MATCHED_CHECKPOINT.md`.
EOF

MASTER_SOURCE="/storage/emulated/0/Download/pin-everything-decomp-complete-and-push.sh"
if [[ -f "$MASTER_SOURCE" ]]; then
    mkdir -p tools/history/checkpoints
    cp -f "$MASTER_SOURCE" tools/history/checkpoints/pin-everything-decomp-complete-and-push.sh
    chmod +x tools/history/checkpoints/pin-everything-decomp-complete-and-push.sh
fi

echo
echo "Checkpoint package coverage:"
python3 - <<'PY'
from pathlib import Path
p = Path("docs/checkpoints/recovered-pack-2026-08-14/PACKAGE_MAPPING.md")
rows = sum(
    1 for line in p.read_text(encoding="utf-8").splitlines()
    if line.startswith("| `")
)
print(f"  mapped package files: {rows}/38")
if rows != 38:
    raise SystemExit("package mapping coverage is not 38/38")
PY

echo
echo "=== Force fresh objects and verify closed listing gates ==="

EE_CC="${EE_CC:-$REPO/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc}"
[[ -x "$EE_CC" ]] || die "Historical EE compiler missing: $EE_CC. Run make bootstrap-ee-stage1 EE_BUILD_JOBS=2 first."

# Never let stale matching objects produce a false checkpoint.
rm -rf \
    build/matching/mathfp \
    build/matching/libgcc_unwind \
    build/matching/gslib_hw

echo "[gate] MathFP 7/7"
make match-mathfp-listing-strict EE_CC="$EE_CC"

echo
echo "[gate] libgcc unwind 7/7"
make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"

echo
echo "[gate] GSLIB hardware 7/7"
if [[ -x tools/run-gslib-frontier.sh ]]; then
    gslib_log="$(mktemp)"
    trap 'rm -f "$gslib_log"' EXIT
    ./tools/run-gslib-frontier.sh | tee "$gslib_log"
    grep -q 'matching summary: 7/7' "$gslib_log" \
        || die "GSLIB runner did not report 7/7"
    grep -q 'GSLIB strict listing gate: OK' "$gslib_log" \
        || die "GSLIB strict gate did not pass"
    rm -f "$gslib_log"
    trap - EXIT
else
    make match-gslib-hw-listing-strict EE_CC="$EE_CC"
fi

# Require the committed reports to actually contain the expected result.
grep -q '7/7 relocation-normalized matches' analysis/matching/mathfp-listing-report.md \
    || die "MathFP report does not contain 7/7"
grep -q '7/7 relocation-normalized matches' analysis/matching/libgcc-unwind-leaves-listing-report.md \
    || die "libgcc unwind report does not contain 7/7"
grep -q '7/7 relocation-normalized matches' analysis/matching/gslib-hw-listing-report.md \
    || die "GSLIB report does not contain 7/7"

cat > docs/MATCHED_CHECKPOINT.md <<'EOF'
# Matched source checkpoint

Checkpoint date: 2026-08-14.

## Closed committed-listing gates: 21 functions

### Newlib mathfp — 7/7

- `0x0019fddc` `cosf`
- `0x001a0024` `sinf`
- `0x001a0254` `tanf`
- `0x001a045c` `atanf`
- `0x001a06a0` `sqrtf`
- `0x001a06b0` `fabsf`
- `0x001a06c0` `numtestf`

Evidence:
`analysis/matching/mathfp-listing-report.md`

### libgcc unwind — 7/7

- `0x001a3dc0` `size_of_encoded_value`
- `0x001a3e30` `base_of_encoded_value`
- `0x001a3ee8` `read_uleb128`
- `0x001a3f28` `read_sleb128`
- `0x001a40e8` `_Unwind_GetLanguageSpecificData`
- `0x001a40f0` `_Unwind_GetRegionStart`
- `0x001a40f8` `_Unwind_GetDataRelBase`

Evidence:
`analysis/matching/libgcc-unwind-leaves-listing-report.md`

### GSLIB hardware — 7/7 strict

- `0x0019bd38` `VRstart_handler`
- `0x0019bd50` `WaitForNextVRstart`
- `0x0019bd78` `TestVRstart`
- `0x0019bd88` `ClearVRcount`
- `0x0019bd98` `DmaReset`
- `0x0019be20` `SendDma02`
- `0x0019be40` `Dma02Wait`

Evidence:
`analysis/matching/gslib-hw-listing-report.md`

## Important scope

These are **relocation-normalized committed-listing matches**. They are strong
function-level evidence, but they do not claim that the complete original ELF
has already been linked and reproduced byte-for-byte.

## Current WIP

EE CDVD RPC corridor:
eight historical functions recovered structurally/source-wise, but **not yet
8/8 byte matching**. Keep CDVD experiments separate from this matched list.
EOF

# Refresh DECOMP_STATE so a future chat sees the closed gates and the archived
# experiment scripts in addition to the historical assets.
cat >> docs/DECOMP_STATE.md <<'EOF'

## Preserved matching checkpoint and scripts

The repository also stores:
- `docs/MATCHED_CHECKPOINT.md` — the 21 closed committed-listing matches.
- `tools/history/progress/` — archived Progress 42-46 scripts.

Script status is explicit in `tools/history/progress/README.md`:
Progress 42 is the closed GSLIB gate; Progress 43-46 remain CDVD WIP/fingerprint
experiments and must never be counted as matching progress.
EOF

echo
echo "=== Stage complete reproducibility checkpoint ==="

git add \
    .gitattributes \
    docs/DECOMP_PLAYBOOK.md \
    docs/DECOMP_STATE.md \
    docs/MATCHED_CHECKPOINT.md \
    third_party/toolchain \
    third_party/historical_refs \
    matching/candidates/mathfp.c \
    matching/candidates/mathfp_numtest.c \
    matching/candidates/mathfp_numtest.S \
    matching/candidates/libgcc_unwind_leaves.c \
    src/ps2/gslib_hw_recovered.c \
    analysis/matching/mathfp.csv \
    analysis/matching/mathfp-listing-report.md \
    analysis/matching/libgcc_unwind_listing.csv \
    analysis/matching/libgcc-unwind-leaves-listing-report.md \
    analysis/matching/gslib_hw_listing.csv \
    analysis/matching/gslib-hw-listing-report.md \
    tools/run-gslib-frontier.sh \
    matching/candidates/cdvd_rpc.c \
    matching/ee_abi_compat/cdvd_legacy_compat.h \
    analysis/functions/cdvd_rpc_0019be70.asm \
    analysis/matching/cdvd_rpc_listing.csv \
    analysis/matching/cdvd_rpc_target.bin \
    tools/run-cdvd-frontier.sh \
    tools/score_cdvd_candidate.py \
    tools/test_progress45_cdvd_exact_shape.py \
    docs/PROGRESS45.md \
    docs/CDVD_RPC_STATUS.md \
    docs/checkpoints \
    tools/history

# Include the Progress42 test/docs if they exist in this checkout.
for optional in \
    tools/test_progress42_gslib_historical_runner.py \
    docs/PROGRESS42.md
do
    [[ -e "$optional" ]] && git add "$optional"
done

echo
echo "=== Final staged payload ==="
git status --short
git diff --cached --check

if git diff --cached --quiet; then
    echo "Nothing new to commit; repository already contains this checkpoint."
else
    git commit -m "Pin historical toolchains and matched decomp checkpoint"
fi

echo
echo "=== Push main ==="
git push origin main

echo
echo "=== DONE ==="
echo "Closed listing matches preserved: 21 functions"
echo "  MathFP:       7/7"
echo "  libgcc unwind: 7/7"
echo "  GSLIB HW:     7/7 strict"
echo "CDVD RPC remains WIP and is NOT counted as matched."
echo
echo "Historical scripts:"
ls -lh tools/history/progress || true
echo
git status --short
