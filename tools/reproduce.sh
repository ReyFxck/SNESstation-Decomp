#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
MODE="${1:-full}"
cd "$PROJECT_ROOT"

show_status() {
    python3 tools/project_status.py
    printf '\nImplemented whole-program gates:\n'
    printf '  - unpacked layout oracle: 1 section / 13 blocks / 51 hash windows\n'
    printf '  - source-address alias tranche: 323/337 proved; 14 explicit blockers\n'
    printf '  - zero-byte link contracts: 1337/1598 resolved; 261-provider frontier\n'
    printf '  - private embedded assets: 10/10 providers; 261 -> 251 frontier\n'
    printf '  - source-link provider namespace: 251/251 resolved; 0 externals\n'
    printf '  - original Stage-3C named data: 49/54 fingerprinted; 33 exact providers\n'
    printf '\nExact replacement ELF still requires:\n'
    printf '  - 3 Stage-3C extents and 2 source-layout refactors; remaining exact data\n'
    printf '  - exact initializers replacing compatibility storage; sections and relocations\n'
    printf '  - exact historical members replacing shims; linker script and link order\n'
    printf '  - reproduced SJCRUNCH2 packing\n'
}

require_reference() {
    if [[ ! -f original/SNES_EMU.ELF ]]; then
        printf 'Missing private reference: original/SNES_EMU.ELF\n' >&2
        printf 'Copy the legally obtained packed ELF there and rerun.\n' >&2
        exit 2
    fi
}

case "$MODE" in
    status)
        show_status
        ;;
    verify)
        make check
        require_reference
        make named-data
        make layout-oracle-check
        make elf-status
        ;;
    full)
        make check
        require_reference
        make named-data
        make layout-oracle-check
        make elf
        ;;
    *)
        printf 'Usage: %s [status|verify|full]\n' "$0" >&2
        exit 2
        ;;
esac
