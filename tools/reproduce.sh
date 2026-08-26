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
    printf '  - zero-byte link contracts: 1336/1594 resolved; 258-provider frontier\n'
    printf '  - private embedded assets: 10/10 providers; 258 -> 248 frontier\n'
    printf '  - source-link provider namespace: 248/248 resolved; 0 externals\n'
    printf '  - original Stage-3C named data: CLOSED 54/54 (50 ranges + 4 refactors)\n'
    printf '\nExact replacement ELF still requires:\n'
    printf '  - Stage 3D historical runtime/archive identities; remaining exact data\n'
    printf '  - exact initializers replacing 9 compatibility stores; sections and relocations\n'
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
