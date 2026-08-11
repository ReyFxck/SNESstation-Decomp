#!/usr/bin/env python3
"""Extract the auditable Progress 16 R5900 structural-source checkpoint.

The input is the combined output produced by DecompileCandidates.java with the
Ghidra Emotion Engine processor.  We keep every warning-free function and only
nine short functions whose exact warning sets were individually reviewed and
pinned below. Larger ambiguous functions stay out of the checkpoint.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_OUTPUT = ROOT / "analysis" / "functions" / "progress16_r5900_pseudocode.c.txt"
DEFAULT_MANIFEST = ROOT / "analysis" / "progress16_recovered_targets.csv"
EXPECTED_GHIDRA_CANDIDATES = 214

REVIEWED_WARNINGS = {
    0x001029C4: (
        "Globals starting with '_' overlap smaller symbols at the same address",
    ),
    0x0010C094: (
        "Globals starting with '_' overlap smaller symbols at the same address",
    ),
    0x0010C1F8: (
        "Removing unreachable block (ram,0x0010c248)",
        "Globals starting with '_' overlap smaller symbols at the same address",
    ),
    0x0010D734: (
        "Could not recover jumptable at 0x0010d764. Too many branches",
        "Treating indirect jump as call",
    ),
    0x0012C444: (
        "Globals starting with '_' overlap smaller symbols at the same address",
    ),
    0x0012C4A8: (
        "Globals starting with '_' overlap smaller symbols at the same address",
    ),
    0x0012E04C: (
        "Removing unreachable block (ram,0x0012e098)",
        "Globals starting with '_' overlap smaller symbols at the same address",
    ),
    0x001A5228: ("Subroutine does not return",),
    0x001A54C8: ("Subroutine does not return",),
}

WARNING_EVIDENCE = {
    0x001029C4: "absolute-global label overlap",
    0x0010C094: "absolute-global label overlap",
    0x0010C1F8: "unreachable compiler block and global-label overlap",
    0x0010D734: "indirect jump-table lowering",
    0x0012C444: "absolute-global label overlap",
    0x0012C4A8: "absolute-global label overlap",
    0x0012E04C: "unreachable compiler block and global-label overlap",
    0x001A5228: "known non-returning sink annotation",
    0x001A54C8: "known non-returning sink annotation",
}

KNOWN_NAMES = {
    0x0014311C: "S9xStartScreenRefresh",
    0x00143390: "RenderLine",
    0x001434AC: "S9xEndScreenRefresh",
    0x0014368C: "S9xSetupOBJ",
    0x001437D8: "DrawOBJS",
    0x00143E7C: "DrawBackgroundMosaic",
    0x001443EC: "DrawBackgroundOffset",
    0x00144AD8: "DrawBackgroundMode5",
    0x001452B0: "DrawBackground",
    0x00145C08: "DrawBGMode7Background",
    0x001461A8: "DrawBGMode7Background16",
    0x00146720: "DrawBGMode7Background16Add",
    0x00146E6C: "DrawBGMode7Background16Add1_2",
    0x00147578: "DrawBGMode7Background16Sub",
    0x00147C68: "DrawBGMode7Background16Sub1_2",
    0x00148394: "DrawBGMode7Background16_i",
    0x001493A8: "DrawBGMode7Background16Add_i",
    0x0014A784: "DrawBGMode7Background16Add1_2_i",
    0x0014BABC: "DrawBGMode7Background16Sub_i",
    0x0014CE24: "DrawBGMode7Background16Sub1_2_i",
    0x0014E140: "RenderScreen",
    0x0014E828: "DisplayChar",
    0x0014EAA4: "S9xDisplayString",
    0x0014EC54: "S9xUpdateScreen",
    0x00150B1C: "ForceInterleave1OverrideSnes9x",
    0x00150CCC: "CMemory_ScoreHiROM",
    0x00150E18: "CMemory_ScoreLoROM",
    0x00150F54: "CMemory_Safe",
    0x001522D8: "CMemory_InitROM",
    0x001538B0: "CMemory_LoROMMap",
    0x00153DA4: "CMemory_BSLoROMMap",
    0x001541E0: "CMemory_HiROMMap",
    0x0015458C: "CMemory_TalesROMMap",
    0x00154A24: "CMemory_AlphaROMMap",
    0x00154C2C: "CMemory_SuperFXROMMap",
    0x00154F9C: "CMemory_SA1ROMMap",
    0x00155334: "CMemory_LoROM24MBSMap",
    0x0015568C: "CMemory_SufamiTurboLoROMMap",
    0x00155A44: "CMemory_SRAM512KLoROMMap",
    0x00155CA0: "CMemory_BSHiROMMap",
    0x00156118: "CMemory_JumboLoROMMap",
    0x00156514: "CMemory_SPC7110HiROMMap",
    0x00156C60: "CMemory_ApplyROMFixes",
    0x001584D0: "CMemory_CheckForIPSPatch",
}


def area_for(address: int) -> str:
    if address < 0x00114000:
        return "frontend-core"
    if address < 0x00131000:
        return "snes-core-dsp"
    if address < 0x00150B1C:
        return "renderer"
    if address < 0x00160000:
        return "memory-ppu"
    if address < 0x00184000:
        return "cpu-audio-runtime"
    if address < 0x001A0000:
        return "legacy-zip-zlib"
    return "gcc-runtime"


def parse_sections(text: str) -> dict[int, str]:
    marker = re.compile(r"/\* ===== (0x[0-9a-fA-F]+) ===== \*/\n")
    matches = list(marker.finditer(text))
    sections: dict[int, str] = {}
    for index, match in enumerate(matches):
        end = matches[index + 1].start() if index + 1 < len(matches) else len(text)
        body = text[match.end():end]
        body = body.split("/* attempted=", 1)[0].rstrip() + "\n"
        address = int(match.group(1), 16)
        if address in sections:
            raise SystemExit(f"duplicate Ghidra section: 0x{address:08x}")
        sections[address] = body
    return sections


def read_tracked() -> set[int]:
    with (ROOT / "analysis" / "progress_targets.csv").open(
        newline="", encoding="utf-8"
    ) as handle:
        return {
            int(row["address"], 16)
            for row in csv.DictReader(handle)
            if not row["notes"].startswith("Progress 16:")
        }


def read_candidates() -> dict[int, dict[str, str]]:
    with (ROOT / "analysis" / "jal_candidates.csv").open(
        newline="", encoding="utf-8"
    ) as handle:
        return {int(row["target"], 16): row for row in csv.DictReader(handle)}


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path, help="combined Ghidra decompiler output")
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    args = parser.parse_args()

    sections = parse_sections(args.input.read_text(encoding="utf-8"))
    if len(sections) != EXPECTED_GHIDRA_CANDIDATES:
        raise SystemExit(
            f"expected {EXPECTED_GHIDRA_CANDIDATES} Ghidra candidates, "
            f"found {len(sections)}"
        )
    for address, body in sections.items():
        if ("/* decompiler failed:" in body or
                "/* no function could be created */" in body):
            raise SystemExit(f"0x{address:08x}: incomplete Ghidra decompile")

    tracked = read_tracked()
    candidates = read_candidates()
    selected: list[int] = []
    for address, body in sorted(sections.items()):
        candidate = candidates.get(address)
        if candidate is None or address in tracked:
            continue
        caller = int(candidate["first_call_site"], 16)
        if not (0x00100000 <= address < 0x001B0800 and
                0x00100000 <= caller < 0x001B0800):
            continue
        if "WARNING:" not in body or address in REVIEWED_WARNINGS:
            selected.append(address)

    if len(selected) != 165:
        raise SystemExit(f"expected 165 conservative targets, found {len(selected)}")
    missing_allowlisted = set(REVIEWED_WARNINGS).difference(selected)
    if missing_allowlisted:
        raise SystemExit(
            "missing warning-reviewed targets: " +
            ", ".join(f"0x{x:08x}" for x in sorted(missing_allowlisted))
        )

    args.output.parent.mkdir(parents=True, exist_ok=True)
    header = """/*
 * Progress 16 structural source recovered from SNES Station v0.23 WIP.
 *
 * Generated from the exact unpacked target with Ghidra 10.4 and
 * ghidra-emotionengine-reloaded 2.1.10 (r5900:LE:32:default).
 * This is address-labelled analysis pseudocode, not build-ready C and not a
 * claim of compiler matching. Absolute DAT_* names intentionally preserve the
 * target memory evidence until final project types replace them.
 */

"""
    chunks = [header]
    rows: list[dict[str, str]] = []
    for address in selected:
        body = sections[address]
        actual_warnings = tuple(re.findall(r"/\* WARNING: (.*?) \*/", body))
        expected_warnings = REVIEWED_WARNINGS.get(address, ())
        if actual_warnings != expected_warnings:
            raise SystemExit(
                f"0x{address:08x}: warning set changed: {actual_warnings!r}"
            )
        section = f"/* ===== 0x{address:08x} ===== */\n{body}\n"
        chunks.append(section)
        digest = hashlib.sha256(body.encode("utf-8")).hexdigest()
        warning_count = body.count("WARNING:")
        candidate = candidates[address]
        name = KNOWN_NAMES.get(address, f"snes_p16_{address:08x}")
        confidence = "medium-high" if warning_count == 0 else "medium"
        evidence = (
            "complete R5900 structural decompile"
            if warning_count == 0
            else "complete short R5900 decompile; "
                 + WARNING_EVIDENCE[address] + " reviewed"
        )
        rows.append({
            "address": f"0x{address:08x}",
            "name": name,
            "area": area_for(address),
            "confidence": confidence,
            "first_call_site": candidate["first_call_site"],
            "call_count": candidate["call_count"],
            "pseudocode_lines": str(body.count("\n")),
            "warning_count": str(warning_count),
            "pseudocode_sha256": digest,
            "evidence": evidence,
        })

    args.output.write_text("".join(chunks), encoding="utf-8")
    fieldnames = list(rows[0])
    with args.manifest.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fieldnames, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)

    def display_path(path: Path) -> Path:
        try:
            return path.relative_to(ROOT)
        except ValueError:
            return path

    print(f"wrote {display_path(args.output)} ({len(selected)} functions)")
    print(f"wrote {display_path(args.manifest)}")


if __name__ == "__main__":
    main()
