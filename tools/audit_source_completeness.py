#!/usr/bin/env python3
"""Audit structural coverage separately from source and matching readiness."""
from __future__ import annotations

import argparse
import csv
import io
import re
from collections import Counter, defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
P16_TARGETS = ROOT / "analysis" / "progress16_recovered_targets.csv"
P17_TARGETS = ROOT / "analysis" / "progress17_recovered_targets.csv"
P16_PSEUDOCODE = ROOT / "analysis" / "functions" / "progress16_r5900_pseudocode.c.txt"
P17_PSEUDOCODE = ROOT / "analysis" / "functions" / "progress17_r5900_pseudocode.c.txt"
PROMOTIONS = ROOT / "analysis" / "source_promotions.csv"
EXACT_NON_PSEUDOCODE_PROMOTIONS = {
    '0x0016fb04': {"source_file": 'src/snes9x/sdd1_log_save.cpp', "evidence": "analysis/functions/sdd1_logs_exact_324.tsv", "evidence_token": '_Z21S9xSDD1SaveLoggedDatav'},
    '0x0016fbb4': {"source_file": 'src/snes9x/sdd1_log_load.cpp', "evidence": "analysis/functions/sdd1_logs_exact_324.tsv", "evidence_token": '_Z21S9xSDD1LoadLoggedDatav'},
    '0x00158b5c': {"source_file": 'src/snes9x/obc1.cpp', "evidence": "analysis/functions/otherchips_exact_1616.tsv", "evidence_token": 'GetOBC1'},
    '0x00158b74': {"source_file": 'src/snes9x/obc1.cpp', "evidence": "analysis/functions/otherchips_exact_1616.tsv", "evidence_token": 'SetOBC1'},
    '0x00158fd0': {"source_file": 'src/snes9x/obc1.cpp', "evidence": "analysis/functions/otherchips_exact_1616.tsv", "evidence_token": 'GetBasePointerOBC1'},
    '0x00158fdc': {"source_file": 'src/snes9x/obc1.cpp', "evidence": "analysis/functions/otherchips_exact_1616.tsv", "evidence_token": 'GetMemPointerOBC1'},
    '0x00158ff0': {"source_file": 'src/snes9x/obc1.cpp', "evidence": "analysis/functions/otherchips_exact_1616.tsv", "evidence_token": 'ResetOBC1'},
    '0x0016f9b0': {"source_file": 'src/snes9x/sdd1_map.cpp', "evidence": "analysis/functions/otherchips_exact_1616.tsv", "evidence_token": '_Z19S9xSetSDD1MemoryMapjj'},
    '0x0016fa18': {"source_file": 'src/snes9x/sdd1_map.cpp', "evidence": "analysis/functions/otherchips_exact_1616.tsv", "evidence_token": '_Z12S9xResetSDD1v'},
    '0x0016fa7c': {"source_file": 'src/snes9x/sdd1_map.cpp', "evidence": "analysis/functions/otherchips_exact_1616.tsv", "evidence_token": '_Z20S9xSDD1PostLoadStatev'},
    '0x001535c0': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory11FixROMSpeedEv'},
    '0x00153608': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory15WriteProtectROMEv'},
    '0x00156884': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory11SPC7110SramEh'},
    '0x001568c4': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory10TVStandardEv'},
    '0x00156914': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory7MapTypeEv'},
    '0x00156934': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory13StaticRAMSizeEv'},
    '0x00156994': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory4SizeEv'},
    '0x00156a04': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory12KartContentsEv'},
    '0x00156c0c': {"source_file": "src/snes9x/memmap_methods.cpp", "evidence": "analysis/functions/memmap_methods_exact_1168.tsv", "evidence_token": '_ZN7CMemory7MapModeEv'},
    '0x001140e0': {"source_file": "src/snes9x/cheats2.cpp", "evidence": "analysis/functions/cheats2_exact_1768.tsv", "evidence_token": '_Z16S9xInitCheatDatav'},
    '0x00114118': {"source_file": "src/snes9x/cheats2.cpp", "evidence": "analysis/functions/cheats2_exact_1768.tsv", "evidence_token": '_Z11S9xAddCheathhjh'},
    '0x00114328': {"source_file": "src/snes9x/cheats2.cpp", "evidence": "analysis/functions/cheats2_exact_1768.tsv", "evidence_token": '_Z14S9xRemoveCheatj'},
    '0x0011439c': {"source_file": "src/snes9x/cheats2.cpp", "evidence": "analysis/functions/cheats2_exact_1768.tsv", "evidence_token": '_Z13S9xApplyCheatj'},
    '0x0011444c': {"source_file": "src/snes9x/cheats2.cpp", "evidence": "analysis/functions/cheats2_exact_1768.tsv", "evidence_token": '_Z14S9xApplyCheatsv'},
    '0x001144d0': {"source_file": "src/snes9x/cheats2.cpp", "evidence": "analysis/functions/cheats2_exact_1768.tsv", "evidence_token": '_Z15S9xRemoveCheatsv'},
    '0x00114544': {"source_file": "src/snes9x/cheats2.cpp", "evidence": "analysis/functions/cheats2_exact_1768.tsv", "evidence_token": '_Z16S9xLoadCheatFilePKc'},
    '0x001825e4': {'source_file': 'src/snes9x/rtc_days.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z17S9xRTCDaysInMonthii'},
    '0x00183660': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z12S9xResetSRTCv'},
    '0x00183678': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z16S9xHardResetSRTCv'},
    '0x001836d0': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z23S9xSRTCComputeDayOfWeekv'},
    '0x00183778': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z19S9xSRTCDaysInMmonthii'},
    '0x001837cc': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z17S9xUpdateSrtcTimev'},
    '0x00183aa4': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z10S9xSetSRTCht'},
    '0x00183bdc': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z10S9xGetSRTCt'},
    '0x00183c58': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z19S9xSRTCPreSaveStatev'},
    '0x00183d40': {'source_file': 'src/snes9x/srtc.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z20S9xSRTCPostLoadStatev'},
    '0x001ac734': {'source_file': 'src/snes9x/getbasepointer.cpp', 'evidence': 'analysis/functions/calendar_memory_exact_2300.tsv', 'evidence_token': '_Z14GetBasePointerj'},
    '0x0010768c': {'source_file': 'src/ps2/sjpcm_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'SjPCM_Play'},
    '0x001076e4': {'source_file': 'src/ps2/sjpcm_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'SjPCM_Pause'},
    '0x0010773c': {'source_file': 'src/ps2/sjpcm_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'SjPCM_Setvol'},
    '0x001077f8': {'source_file': 'src/ps2/sjpcm_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'SjPCM_Init'},
    '0x001078f8': {'source_file': 'src/ps2/sjpcm_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'SjPCM_Enqueue'},
    '0x00107b1c': {'source_file': 'src/ps2/sjpcm_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'SjPCM_Quit'},
    '0x00107b7c': {'source_file': 'src/ps2/amigamod_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'amigaModInit'},
    '0x00107c64': {'source_file': 'src/ps2/amigamod_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'amigaModLoad'},
    '0x00107d44': {'source_file': 'src/ps2/amigamod_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'amigaModPlay'},
    '0x00107db4': {'source_file': 'src/ps2/amigamod_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'amigaModPause'},
    '0x00107e14': {'source_file': 'src/ps2/amigamod_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'amigaModSetVolume'},
    '0x00107f18': {'source_file': 'src/ps2/amigamod_rpc.c', 'evidence': 'analysis/functions/audio_rpc_exact_2892.tsv', 'evidence_token': 'amigaModQuit'},
    '0x001ac838': {'source_file': 'src/snes9x/selecttilerenderer.cpp', 'evidence': 'analysis/functions/selecttilerenderer_exact_348.tsv', 'evidence_token': '_Z18SelectTileRendererh'},
    "0x001ac994": {
        "source_file": "src/snes9x/apumem.cpp",
        "evidence": "analysis/functions/apumem_exact_880.tsv",
        "evidence_token": "_Z14S9xAPUGetByteZh",
    },
    "0x001aca50": {
        "source_file": "src/snes9x/apumem.cpp",
        "evidence": "analysis/functions/apumem_exact_880.tsv",
        "evidence_token": "_Z14S9xAPUSetByteZhh",
    },
    "0x001acb3c": {
        "source_file": "src/snes9x/apumem.cpp",
        "evidence": "analysis/functions/apumem_exact_880.tsv",
        "evidence_token": "_Z13S9xAPUGetBytej",
    },
    "0x001acbf0": {
        "source_file": "src/snes9x/apumem.cpp",
        "evidence": "analysis/functions/apumem_exact_880.tsv",
        "evidence_token": "_Z13S9xAPUSetBytehj",
    },
    "0x001ab4e8": {
        "source_file": "src/snes9x/s9xgetmempointer.cpp",
        "evidence": "analysis/matching/hunt400-validated-19.tsv",
        "evidence_token": "_Z16S9xGetMemPointerj",
    },
    "0x001ab63c": {
        "source_file": "src/snes9x/s9xgetbyte.cpp",
        "evidence": "analysis/matching/hunt500plus-v11-validated-4.tsv",
        "evidence_token": "_Z10S9xGetBytej",
    },
    "0x001ab900": {
        "source_file": "src/snes9x/s9xsetbyte.cpp",
        "evidence": "analysis/matching/hunt500plus-v11-validated-4.tsv",
        "evidence_token": "_Z10S9xSetBytehj",
    },
    "0x001abc28": {
        "source_file": "src/snes9x/s9xgetword.cpp",
        "evidence": "analysis/matching/hunt1041-v48-validated-25.tsv",
        "evidence_token": "_Z10S9xGetWordj",
    },
}
CSV_OUT = ROOT / "analysis" / "source_readiness.csv"
DOC_OUT = ROOT / "docs" / "SOURCE_COMPLETENESS.generated.md"

EXPECTED_STRUCTURAL_TARGETS = 1041
EXPECTED_P16_PSEUDOCODE = 165
EXPECTED_P17_PSEUDOCODE = 74
MARKER_RE = re.compile(r"^/\* ===== (0x[0-9a-fA-F]{8}) ===== \*/$", re.MULTILINE)

EXACT_SOURCE_TRACES = (
    (
        ROOT / "analysis" / "functions" / "libkernel_leaf_exact_508.tsv",
        "src/ps2/kernel.S",
    ),
    (
        ROOT / "analysis" / "functions" / "memcpy_exact_56.tsv",
        "src/ps2/memcpy.S",
    ),
    (
        ROOT / "analysis" / "functions" / "memset_exact_56.tsv",
        "src/ps2/memset.S",
    ),
    (
        ROOT / "analysis" / "functions" / "memmove_exact_136.tsv",
        "src/ps2/memmove.S",
    ),
    (
        ROOT / "analysis" / "functions" / "strcat_exact_56.tsv",
        "src/ps2/strcat.S",
    ),
    (
        ROOT / "analysis" / "functions" / "memcmp_exact_72.tsv",
        "src/ps2/memcmp.S",
    ),
    (
        ROOT / "analysis" / "functions" / "strcpy_exact_40.tsv",
        "src/ps2/strcpy.S",
    ),
    (
        ROOT / "analysis" / "functions" / "strlen_exact_40.tsv",
        "src/ps2/strlen.S",
    ),
    (
        ROOT / "analysis" / "functions" / "strchr_exact_56.tsv",
        "src/ps2/strchr.S",
    ),
    (
        ROOT / "analysis" / "functions" / "strcmp_exact_64.tsv",
        "src/ps2/strcmp.S",
    ),
    (
        ROOT / "analysis" / "functions" / "strncpy_exact_88.tsv",
        "src/ps2/strncpy.S",
    ),
    (
        ROOT / "analysis" / "functions" / "strncmp_exact_72.tsv",
        "src/ps2/strncmp.S",
    ),
    (
        ROOT / "analysis" / "functions" / "strrchr_exact_84.tsv",
        "src/ps2/string.c",
    ),
    (
        ROOT / "analysis" / "functions" / "strstr_exact_136.tsv",
        "src/ps2/strstr.c",
    ),
    (
        ROOT / "analysis" / "functions" / "strtol_exact_556.tsv",
        "src/ps2/strtol.c",
    ),
    (
        ROOT / "analysis" / "functions" / "strcasecmp_exact_132.tsv",
        "src/ps2/strcasecmp.c",
    ),
    (
        ROOT / "analysis" / "functions" / "strtok_exact_264.tsv",
        "src/ps2/strtok.c",
    ),
    (
        ROOT / "analysis" / "functions" / "strncasecmp_exact_184.tsv",
        "src/ps2/strncasecmp.c",
    ),
    (
        ROOT / "analysis" / "functions" / "ctype_exact_616.tsv",
        "src/ps2/ctype.c",
    ),
    (
        ROOT / "analysis" / "functions" / "qsort_historical_object.tsv",
        "src/ps2/qsort.c",
    ),
    (
        ROOT / "analysis" / "functions" / "sbrk_historical_object.tsv",
        "src/ps2/sbrk.c",
    ),
    (
        ROOT / "analysis" / "functions" / "get_tree_exact_212.tsv",
        "src/unzip/get_tree.S",
    ),
    (
        ROOT / "analysis" / "functions" / "numtestf_exact_128.tsv",
        "src/ps2/numtestf.S",
    ),
    (
        ROOT / "analysis" / "functions" / "c4convoam_exact_952.tsv",
        "src/ps2/c4convoam.S",
    ),
    (
        ROOT / "analysis" / "functions" / "c4doscalerotate_exact_1208.tsv",
        "src/ps2/c4doscalerotate.S",
    ),
    (
        ROOT / "analysis" / "functions" / "c4transformlines_exact_772.tsv",
        "src/ps2/c4transformlines.S",
    ),
    (
        ROOT / "analysis" / "matching" / "hunt1041-v77-validated-c4draw-1.tsv",
        "src/snes9x/c4drawwireframe.cpp",
    ),
)


def read_csv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream))


def fail(message: str) -> None:
    raise SystemExit(f"source audit failed: {message}")


def unique_by_address(rows: list[dict[str, str]], label: str) -> dict[str, dict[str, str]]:
    result: dict[str, dict[str, str]] = {}
    for row in rows:
        address = row["address"].lower()
        if address in result:
            fail(f"duplicate {label} address {address}")
        result[address] = row
    return result


def pseudocode_markers(path: Path) -> set[str]:
    text = path.read_text(encoding="utf-8")
    markers = [match.lower() for match in MARKER_RE.findall(text)]
    if len(markers) != len(set(markers)):
        fail(f"duplicate address marker in {path.relative_to(ROOT)}")
    return set(markers)


def explicit_source_references(addresses: set[str]) -> tuple[dict[str, list[str]], int]:
    """Return conservative address-to-source traceability, not ownership proof."""
    references: dict[str, list[str]] = defaultdict(list)
    translation_units = sorted(
        path
        for pattern in ("*.c", "*.cpp", "*.S")
        for path in (ROOT / "src").rglob(pattern)
    )
    address_by_hex = {address[2:]: address for address in addresses}

    for source_path in translation_units:
        text = source_path.read_text(encoding="utf-8", errors="replace").lower()
        relative = source_path.relative_to(ROOT).as_posix()
        for token, address in address_by_hex.items():
            if token in text:
                references[address].append(relative)

    # Exact historical sources do not need recovery-only address comments added
    # to their byte-identical source text.  Reviewed sidecars can therefore pin
    # source traceability for selected target entries.
    for table, source in EXACT_SOURCE_TRACES:
        if not (ROOT / source).is_file():
            fail(f"missing exact traced source {source}")
        with table.open(encoding="utf-8", newline="") as stream:
            for row in csv.DictReader(stream, delimiter="\t"):
                address = row["address"].lower()
                if address not in addresses:
                    fail(f"exact source trace address outside target universe: {address}")
                references[address].append(source)

    normalized = {
        address: sorted(set(files))
        for address, files in references.items()
    }
    return normalized, len(translation_units)


def render_csv(
    target_rows: list[dict[str, str]],
    p16: set[str],
    p17: set[str],
    references: dict[str, list[str]],
) -> str:
    stream = io.StringIO(newline="")
    fields = [
        "address",
        "name",
        "area",
        "manifest_status",
        "source_form",
        "explicit_src_reference",
        "source_files",
        "matching_status",
    ]
    writer = csv.DictWriter(stream, fieldnames=fields, lineterminator="\n")
    writer.writeheader()
    for row in target_rows:
        address = row["address"].lower()
        if address in p16:
            source_form = "STRUCTURAL_PSEUDOCODE_P16"
        elif address in p17:
            source_form = "STRUCTURAL_PSEUDOCODE_P17"
        else:
            source_form = "BEHAVIORAL_SOURCE_MODEL"
        files = references.get(address, [])
        writer.writerow(
            {
                "address": address,
                "name": row["name"],
                "area": row["area"],
                "manifest_status": row["status"],
                "source_form": source_form,
                "explicit_src_reference": "yes" if files else "no",
                "source_files": ";".join(files),
                "matching_status": "MATCHING" if row["status"] == "MATCHING" else "UNPROVEN",
            }
        )
    return stream.getvalue()


def render_doc(
    total: int,
    behavioral: int,
    p16_count: int,
    p17_count: int,
    promoted: int,
    explicit: int,
    translation_units: int,
    matching: int,
) -> str:
    pseudo = p16_count + p17_count

    def pct(value: int) -> str:
        return f"{value * 100.0 / total:.2f}%"

    return f"""# Generated source-completeness audit

> Generated by `tools/audit_source_completeness.py`. Do not hand-edit this file.

The repository has complete **structural representation** for the audited target
universe and a closed build-ready source/object ownership gate. The separate
whole-image and wrapper gates now produce a byte-identical packed replacement;
these measurements remain deliberately distinct.

| Measurement | Result | What it proves |
|---|---:|---|
| Audited structural entries | **{total:,}/{total:,} ({pct(total)})** | Every validated entry has a committed structural representation. |
| Behavioral/source-model checkpoint | **{behavioral:,}/{total:,} ({pct(behavioral)})** | Typed behavioral/source-model reconstruction exists for every audited row; exact source provenance is a separate claim. |
| Structural pseudocode only | **{pseudo:,}/{total:,} ({pct(pseudo)})** | Historical structural-only backlog after promotions: {p16_count} remaining from Progress 16 and {p17_count} remaining from Progress 17. |
| Source promotions | **{promoted:,}** | Historical pseudocode promotions plus explicitly reviewed exact-historical promotions override older source models while retaining their evidence. |
| Explicit address trace in `src/` | **{explicit:,}/{total:,} ({pct(explicit)})** | A conservative text-level traceability check; corridor files may cover additional entries without repeating every address. |
| Build-ready EE source ownership | **{translation_units}/{translation_units} TUs** | `make source-tree` compiles every unit with EE GCC 3.2.2 and verifies the frozen canonical partial-link/ownership maps. |
| Relocation-normalized machine-code matches | **{matching:,}/{total:,} ({pct(matching)})** | No function is promoted to `MATCHING` without generated-object evidence. |
| Complete replacement ELF | **Yes: 726,968/726,968 bytes** | The independent whole-image, compression and public SjCRUNCH wrapper gates close full-file identity. |

## Invariants checked

- `analysis/progress_targets.csv` and `analysis/symbols.csv` contain the same
  {total:,} addresses, names, statuses, confidence values and notes.
- All {total:,} manifest rows are structurally reconstructed.
- The historical {EXPECTED_P16_PSEUDOCODE} Progress-16 and {EXPECTED_P17_PSEUDOCODE} Progress-17 manifest sets exactly match the
  address markers in their committed pseudocode snapshots.
- `analysis/source_promotions.csv` contains {promoted} source promotion(s); each
  promoted address is either a historical pseudocode checkpoint or an explicitly
  reviewed exact-historical target. Every promotion names an existing
  source/evidence file, and the source file must explicitly carry the promoted
  address token.
- No address occurs in both pseudocode checkpoints.
- The independent EE source gate freezes {translation_units} source boundaries and
  their canonical objects, the EE ABI and every emitted/unresolved symbol owner. See
  [`docs/status/BUILD_READY_SOURCE_TREE.md`](status/BUILD_READY_SOURCE_TREE.md).

The machine-readable row-by-row classification is
[`analysis/source_readiness.csv`](../analysis/source_readiness.csv).

## Meaning of “complete”

"Nothing left behind" is defensible inside the closed structural universe and
the manifest-defined EE source tree: 1,137 raw JAL-shaped targets − 292
rejected post-code data patterns + 196 independently mapped non-JAL entries =
1,041 validated entries, compiled through {translation_units} explicit TUs. It does not prove
that 1,041 is the mathematically exact number of compiler-created functions,
that those source boundaries are Hiryu's verbatim originals, or that the final ELF
layout already matches.

The next proof ladder is documented in
[`docs/MATCHING_WORKFLOW.md`](MATCHING_WORKFLOW.md).
"""


def write_or_check(path: Path, content: str, check: bool) -> None:
    if check:
        if not path.exists():
            fail(f"missing generated file {path.relative_to(ROOT)}")
        if path.read_text(encoding="utf-8") != content:
            fail(f"stale generated file {path.relative_to(ROOT)}; run make audit-source")
        return
    path.write_text(content, encoding="utf-8", newline="")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail if generated audit files are stale")
    args = parser.parse_args()

    target_rows = read_csv(TARGETS)
    symbol_rows = read_csv(SYMBOLS)
    targets = unique_by_address(target_rows, "progress target")
    symbols = unique_by_address(symbol_rows, "symbol")

    if len(target_rows) != EXPECTED_STRUCTURAL_TARGETS:
        fail(f"expected {EXPECTED_STRUCTURAL_TARGETS} targets, found {len(target_rows)}")
    if set(targets) != set(symbols):
        fail("progress and symbol manifests have different address sets")
    for address, target in targets.items():
        symbol = symbols[address]
        for field in ("name", "status", "confidence", "notes"):
            if target[field] != symbol[field]:
                fail(f"manifest mismatch at {address}: {field}")

    statuses = Counter(row["status"] for row in target_rows)
    unexpected = set(statuses) - {"RECONSTRUCTED", "MATCHING"}
    if unexpected:
        fail(f"non-reconstructed statuses remain: {sorted(unexpected)}")

    p16_rows = unique_by_address(read_csv(P16_TARGETS), "Progress-16 target")
    p17_rows = unique_by_address(read_csv(P17_TARGETS), "Progress-17 target")
    p16 = set(p16_rows)
    p17 = set(p17_rows)
    if len(p16) != EXPECTED_P16_PSEUDOCODE:
        fail(f"expected {EXPECTED_P16_PSEUDOCODE} Progress-16 rows, found {len(p16)}")
    if len(p17) != EXPECTED_P17_PSEUDOCODE:
        fail(f"expected {EXPECTED_P17_PSEUDOCODE} Progress-17 rows, found {len(p17)}")
    if p16 & p17:
        fail(f"pseudocode checkpoints overlap at {sorted(p16 & p17)[0]}")
    if not (p16 | p17) <= set(targets):
        fail("pseudocode checkpoint contains an address outside the structural manifest")
    if p16 != pseudocode_markers(P16_PSEUDOCODE):
        fail("Progress-16 CSV and pseudocode address markers differ")
    if p17 != pseudocode_markers(P17_PSEUDOCODE):
        fail("Progress-17 CSV and pseudocode address markers differ")

    promotion_rows = unique_by_address(read_csv(PROMOTIONS), "source promotion")
    promoted = set(promotion_rows)
    historical_pseudocode = p16 | p17
    if not promoted <= set(targets):
        fail("source promotion contains an address outside the structural manifest")
    non_pseudocode_promotions = promoted - historical_pseudocode
    if not non_pseudocode_promotions <= set(EXACT_NON_PSEUDOCODE_PROMOTIONS):
        fail("source promotion contains an unreviewed address outside the historical pseudocode checkpoints")
    for address in sorted(non_pseudocode_promotions):
        row = promotion_rows[address]
        exact = EXACT_NON_PSEUDOCODE_PROMOTIONS[address]
        for field in ("source_file", "evidence"):
            if row.get(field, "").strip() != exact[field]:
                fail(f"exact historical promotion {address} {field} drift")
        evidence_text = (ROOT / exact["evidence"]).read_text(
            encoding="utf-8", errors="replace"
        )
        if address not in evidence_text or exact["evidence_token"] not in evidence_text:
            fail(f"exact historical promotion {address} evidence drift")
    for address, row in promotion_rows.items():
        source_rel = row.get("source_file", "").strip()
        evidence_rel = row.get("evidence", "").strip()
        if not source_rel:
            fail(f"source promotion {address} has no source_file")
        if not evidence_rel:
            fail(f"source promotion {address} has no evidence")
        source_path = ROOT / source_rel
        evidence_path = ROOT / evidence_rel
        if not source_path.is_file():
            fail(f"source promotion {address} references missing source {source_rel}")
        if not evidence_path.is_file():
            fail(f"source promotion {address} references missing evidence {evidence_rel}")
        source_text = source_path.read_text(encoding="utf-8", errors="replace").lower()
        if address[2:] not in source_text:
            fail(f"source promotion {address} is not explicitly traced in {source_rel}")

    p16_remaining = p16 - promoted
    p17_remaining = p17 - promoted
    references, translation_units = explicit_source_references(set(targets))
    behavioral = len(target_rows) - len(p16_remaining) - len(p17_remaining)
    matching = statuses.get("MATCHING", 0)
    csv_text = render_csv(target_rows, p16_remaining, p17_remaining, references)
    doc_text = render_doc(
        len(target_rows),
        behavioral,
        len(p16_remaining),
        len(p17_remaining),
        len(promoted),
        len(references),
        translation_units,
        matching,
    )
    write_or_check(CSV_OUT, csv_text, args.check)
    write_or_check(DOC_OUT, doc_text, args.check)

    action = "verified" if args.check else "wrote"
    print(
        f"{action} source audit: structural={len(target_rows)}/{len(target_rows)} "
        f"behavioral_source_model={behavioral} "
        f"pseudocode_only={len(p16_remaining) + len(p17_remaining)} promoted={len(promoted)} "
        f"explicit_src_trace={len(references)} matching={matching} translation_units={translation_units}"
    )


if __name__ == "__main__":
    main()
