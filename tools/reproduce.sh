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
    printf '  - zero-byte link contracts: 1336/1573 resolved; 237-provider frontier\n'
    printf '  - private embedded assets: 10/10 providers; 237 -> 227 frontier\n'
    printf '  - source-link provider namespace: 227/227 resolved; 0 externals\n'
    printf '  - original Stage-3C named data: CLOSED 54/54 (50 ranges + 4 refactors)\n'
    printf '  - original Stage-3E named contracts: CLOSED 212/212 (165 fingerprints)\n'
    printf '\nExact replacement ELF still requires:\n'
    printf '  - Stage 3D historical runtime/archive identities; replace 4 shims\n'
    printf '  - Stage 3F ranges/bytes for 1,265 unnamed address contracts\n'
    printf '  - exact sections, relocations, linker script and link order\n'
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
        make named-contracts
        make layout-oracle-check
        make elf-status
        ;;
    full)
        make check
        require_reference
        make named-contracts
        make layout-oracle-check
        make elf
        ;;
    *)
        printf 'Usage: %s [status|verify|full]\n' "$0" >&2
        exit 2
        ;;
esac
