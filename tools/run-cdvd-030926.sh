#!/usr/bin/env bash
# Run the CDVD frontier with the preserved pre-target EE GCC 3.2 build 030926.
# This is a fingerprint experiment, not a declaration that the compiler is exact.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ARCHIVE="${EE_030926_ARCHIVE:-$ROOT/third_party/toolchain/archive/ee-gcc3.2-030926.tar.gz}"
WORK="${EE_030926_WORK:-$ROOT/build/toolchains/ee-gcc3.2-030926}"
EXPECTED_SHA256="6b92b61e40f80835b165d14fadd57d4046dbee82195599f791626180fe79b8e9"
STAMP="$WORK/.archive-sha256"

if [[ ! -f "$ARCHIVE" ]]; then
    echo "missing preserved compiler bundle: $ARCHIVE" >&2
    exit 2
fi

actual_sha="$(sha256sum "$ARCHIVE" | awk '{print $1}')"
if [[ "$actual_sha" != "$EXPECTED_SHA256" ]]; then
    echo "historical compiler SHA-256 mismatch" >&2
    echo "expected: $EXPECTED_SHA256" >&2
    echo "actual:   $actual_sha" >&2
    exit 2
fi

if [[ ! -f "$STAMP" || "$(cat "$STAMP" 2>/dev/null || true)" != "$EXPECTED_SHA256" ]]; then
    rm -rf "$WORK"
    mkdir -p "$WORK"
    # Historical bundles retain the uploader's numeric UID/GID.  Restoring
    # those owners fails in rootless/containerized environments (including
    # DroidSpaces), even though the archive contents are otherwise usable.
    tar --no-same-owner -xzf "$ARCHIVE" -C "$WORK"
    printf '%s\n' "$EXPECTED_SHA256" > "$STAMP"
fi

compiler="$WORK/bin/ee-gcc"
if [[ ! -f "$compiler" ]]; then
    compiler="$(find "$WORK" -type f -path '*/bin/ee-gcc' -print -quit)"
fi
if [[ -z "${compiler:-}" || ! -f "$compiler" ]]; then
    echo "ee-gcc not found after extracting $ARCHIVE" >&2
    exit 2
fi
chmod +x "$compiler" 2>/dev/null || true

# The preserved bundle is identified by its archive hash. These runtime probes
# are sanity checks only; byte comparison remains the matching gate.
if ! version="$($compiler -dumpversion 2>&1)"; then
    echo "preserved ee-gcc 030926 cannot execute on this host" >&2
    command -v file >/dev/null 2>&1 && file "$compiler" >&2 || true
    exit 2
fi
machine="$($compiler -dumpmachine 2>/dev/null || true)"
printf 'EE 030926 compiler: %s\n' "$compiler"
printf 'base version: %s\n' "$version"
printf 'target: %s\n' "$machine"
case "$version" in
    3.2|3.2-*) ;;
    *)
        echo "unexpected base version for 3.2-ee-030926: $version" >&2
        exit 2
        ;;
esac
case "${machine,,}" in
    ee|*r5900*) ;;
    *)
        echo "unexpected target for EE compiler: $machine" >&2
        exit 2
        ;;
esac

# run-cdvd-frontier.sh already performs the source/flag matrix, scores each
# compiled object against the committed target corridor and invokes the strict
# gate only if every row matches.  Supplying EE_CC changes only the compiler.
EE_CC="$compiler" bash "$ROOT/tools/run-cdvd-frontier.sh"
