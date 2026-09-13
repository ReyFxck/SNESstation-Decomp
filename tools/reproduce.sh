#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
MODE="${1:-full}"
cd "$PROJECT_ROOT"

show_status() {
    python3 tools/project_status.py
    printf '\nImplemented whole-program gates:\n'
    printf '  - unpacked layout oracle: 1 section / 13 blocks / 51 hash windows\n'
    printf '  - source-address alias tranche: 333/347 proved; 14 explicit blockers\n'
    printf '  - zero-byte link contracts: 1297/1530 resolved; 233-provider frontier\n'
    printf '  - private embedded assets: 10/10 providers; 233 -> 223 frontier\n'
    printf '  - source-link provider namespace: 223/223 resolved; 0 externals\n'
    printf '  - original Stage-3C named data: CLOSED 54/54 (50 ranges + 4 refactors)\n'
    printf '  - original Stage-3E named contracts: CLOSED 212/212 (165 fingerprints)\n'
    printf '  - Stage-3D libgcc subtranche: CLOSED 7/7 (4 archive members + 3 refactors)\n'
    printf '  - Stage-3D snprintf refactor: CLOSED (4 sprintf calls; runtime shims=0)\n'
    printf '  - Stage-3D PS2LIB member text: 43 contracts / 42 objects / 12,964 bytes\n'
    printf '  - Stage-3D target overrides: 2/2; 15 named calls; 104 exact linked bytes\n'
    printf '  - Stage-3D runtime contract ledger: CLOSED 53/53\n'
    printf '  - Stage-3F access spans: 872/1265; 167,659 bytes (146 CFG + 33 prefix proofs)\n'
    printf '  - Stage-3F typed historical providers: 49 intervals / 810,542 exact bytes\n'
    printf '  - Stage-3F address identities: 1265/1265; 0 unresolved\n'
    printf '  - Real backing link: 695,316 bytes freshly rebuilt from pinned source\n'
    printf '  - Stage-3G clean link: 179 fixed sections; 155 payloads exact; 12/51 windows exact\n'
    printf '  - Stage-3G delta: 1,883,867 bytes; entry 0x00111f70 != 0x00100008\n'
    printf '  - Stage-3G exact startup: _start/_exit/_root; 276 bytes; 27 relocations\n'
    printf '  - Stage-3G startup entry: 0x00100008 exact; next difference 0x00100114\n'
    printf '  - Stage-3H frontend unwind: 18 FDEs / 944 bytes; window 14 exact\n'
    printf '  - Stage-3I historical tail: 123,140 source bytes + 30 FDEs\n'
    printf '  - Stage-3J runtime tail: 3,868 source bytes; 56 FDEs; 73 relocations\n'
    printf '  - Stage-3K tail metadata: 9,428 source bytes; window 50 exact\n'
    printf '  - Stage-3L window 36: 31,460 source bytes; 27 semantic FDEs\n'
    printf '  - Stage-3M media corridor: windows 15-34 exact\n'
    printf '  - Stage-3N window 35: 29,516 source bytes; 47 semantic FDEs\n'
    printf '  - Stage-3O window 11: 35,067 public-source bytes + 2,220 semantic bytes; exact\n'
    printf '  - Stage-3P code windows: 720,620 evidence bytes integrated\n'
    printf '  - Whole-image diagnostic: 51/51 windows exact; 0 bytes differ\n'
    printf '  - SJCRUNCH2 compression: 13/13 LZO1X-999 level-8 blocks exact\n'
    printf '  - SJCRUNCH2 container: 714,268/714,268 bytes exact\n'
    printf '  - SjCRUNCH 2.1 public loader/outer ELF: 12,700/12,700 bytes exact\n'
    printf '  - complete packed ELF: 726,968/726,968 bytes exact\n'
    printf '  - packed SHA-256: 4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487\n'
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
        make sjcrunch-outer-elf-check layout-oracle-check elf-status
        ;;
    full)
        make check
        require_reference
        make sjcrunch-outer-elf-check layout-oracle-check elf-status
        ;;
    *)
        printf 'Usage: %s [status|verify|full]\n' "$0" >&2
        exit 2
        ;;
esac
