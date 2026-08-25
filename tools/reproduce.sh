#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
MODE="${1:-full}"
cd "$PROJECT_ROOT"

show_status() {
    python3 tools/project_status.py
    printf '\nExact replacement ELF still requires:\n'
    printf '  - exact global data, sections, relocations and object boundaries\n'
    printf '  - exact historical archives, linker script and link order\n'
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
        make source-tree
        require_reference
        make reference
        make elf-status
        ;;
    full)
        make check
        make source-tree
        require_reference
        make reference
        make elf
        ;;
    *)
        printf 'Usage: %s [status|verify|full]\n' "$0" >&2
        exit 2
        ;;
esac
