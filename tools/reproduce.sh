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
    printf '  - zero-byte link contracts: 1336/1569 resolved; 233-provider frontier\n'
    printf '  - private embedded assets: 10/10 providers; 233 -> 223 frontier\n'
    printf '  - source-link provider namespace: 223/223 resolved; 0 externals\n'
    printf '  - original Stage-3C named data: CLOSED 54/54 (50 ranges + 4 refactors)\n'
    printf '  - original Stage-3E named contracts: CLOSED 212/212 (165 fingerprints)\n'
    printf '  - Stage-3D libgcc subtranche: CLOSED 7/7 (4 archive members + 3 refactors)\n'
    printf '  - Stage-3D snprintf refactor: CLOSED (4 sprintf calls; runtime shims=0)\n'
    printf '  - Stage-3D PS2LIB member text: 43 contracts / 42 objects / 12,964 bytes\n'
    printf '  - Stage-3D target overrides: 2/2; 15 named calls; 104 exact linked bytes\n'
    printf '  - Stage-3D runtime contract ledger: CLOSED 53/53\n'
    printf '  - Stage-3F access spans: 824/1265; 167,521 unique consumed bytes (131 CFG proofs)\n'
    printf '  - Stage-3F section backing: 961/1265 addresses; 165,946 materialized access bytes\n'
    printf '\nExact replacement ELF still requires:\n'
    printf '  - final source selection/integration of exact function implementations\n'
    printf '  - complete Stage-3F object/array extents; 441 lack access witnesses\n'
    printf '  - backing for 304 remaining addresses; 137 backed interiors do not prove bounds\n'
    printf '  - historical member data and original whole-archive composition\n'
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
        make runtime-overrides
        make unnamed-data
        make data-backing
        make layout-oracle-check
        make elf-status
        ;;
    full)
        make check
        require_reference
        make runtime-overrides
        make unnamed-data
        make data-backing
        make layout-oracle-check
        make elf
        ;;
    *)
        printf 'Usage: %s [status|verify|full]\n' "$0" >&2
        exit 2
        ;;
esac
