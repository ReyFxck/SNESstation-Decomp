#!/usr/bin/env python3
"""Rebuild and integrate the exact GCC 3.2.2 frontend unwind metadata.

The historical EE compiler emitted CIE/FDE records into writable data.  This
gate assembles those records from explicit DWARF semantics and public exact
function extents, applies R_MIPS_32 relocations at final addresses, and adds
the three non-overlapping sections to the Stage-3G executable diagnostic.
It never copies target bytes and does not claim a replacement ELF.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import shlex
import struct
import subprocess
from pathlib import Path
from typing import Sequence

import data_backing
import link_layout_probe
import startup_integration as startup
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool, sibling_tool

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE = ROOT / "matching/candidates/stage3h_frontend_eh_frames.S"
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP_OBJECT = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_BUILD = ROOT / "build/frontend-eh-frames"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_STARTUP_MANIFEST = ROOT / "analysis/link_identity/startup_integration.json"
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/frontend_eh_frames.json"

FORMAT = "snesstation-stage3h-frontend-eh-frame-integration"
SCHEMA = 1
SOURCE_SHA256 = "8d7d7d809e897666a3d62e27d6302df2d228c298302d5e98138f517bff5c6409"
COMPILE_FLAGS = (
    "-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
)

GROUPS = (
    {
        "section": ".data.stage3h.frontend_eh_1",
        "address": 0x001EBAF0,
        "size": 0x220,
        "relocations": 13,
        "unrelocated_sha256": "264888dcb98f386e48d07e5efd5b598ae73f84faca725e5e3c159792423da7e6",
        "linked_sha256": "7df126bdea3534f63ead51338f0691f78bf13b2fe983e44f5d866f5769345dc3",
        "cie_augmentation": "zPL",
    },
    {
        "section": ".data.stage3h.frontend_eh_2",
        "address": 0x001EBD60,
        "size": 0x8C,
        "relocations": 4,
        "unrelocated_sha256": "aec592da90a927aea302192f960ff668f97b498c0bed43b674551e3091242569",
        "linked_sha256": "282e3ddd331b982d5a590d09c855a80eabb8786fe403bcdb6ecaf6e8d65b4277",
        "cie_augmentation": "zPL",
    },
    {
        "section": ".data.stage3h.frontend_eh_3",
        "address": 0x001EC1CC,
        "size": 0x104,
        "relocations": 6,
        "unrelocated_sha256": "f0f11fa62568aa1550bfa23cf6086c2b7e6b0994d7757734f37b9e907b873e74",
        "linked_sha256": "6f770a317db9f05a09a3d4801d851d026d9c40ae099e3011406ac1d77c1a0099",
        "cie_augmentation": "zP",
    },
)

FDES = (
    (0x00104E58, 0x24, "frontend_shutdown_00104e58", 1),
    (0x00104E7C, 0x9C, "loadModuleBuffer", 1),
    (0x00104F18, 0x798, "main", 1),
    (0x001056C0, 0x20, "snes_dispatch_001056c0", 1),
    (0x00105750, 0xAC, "build_sram_path_00105750", 1),
    (0x001057FC, 0x9C, "snes_p16_001057fc", 1),
    (0x00105898, 0x134, "snes_p16_00105898", 1),
    (0x00105D30, 0x48, "snes_dispatch_00105d30", 1),
    (0x00105D78, 0x8C, "snes_dispatch_00105d78", 1),
    (0x00105E04, 0x34, "snes_dispatch_00105e04", 1),
    (0x00105E48, 0x20C, "snes_p17_00105e48", 1),
    (0x001060DC, 0x3E4, "snes_p16_001060dc", 2),
    (0x001064C0, 0x344, "snes_p16_001064c0", 2),
    (0x00106824, 0x78, "snes_p12_00106824", 3),
    (0x0010689C, 0x330, "snes_p16_0010689c", 3),
    (0x00106BCC, 0x3C, "is_sram_extension_00106bcc", 3),
    (0x00106CA0, 0x6B8, "snes_p16_00106ca0", 3),
    (0x00107358, 0x220, "snes_p16_00107358", 3),
)

EH_LINK_SYMBOLS = {
    "stage3h_gxx_personality": 0x001A9728,
    "stage3h_lsda_00426b71": 0x00426B71,
    "stage3h_lsda_00426b83": 0x00426B83,
    **{f"stage3h_pc_{address:08x}": address for address, _size, _name, _group in FDES},
}

EXPECTED = {
    "target_entry_address": 0x00100008,
    "integrated_entry_address": 0x00100008,
    "frontend_eh_groups": 3,
    "frontend_fdes": 18,
    "frontend_eh_bytes": 944,
    "frontend_eh_relocations": 23,
    "target_initialized_size": 3_304_936,
    "integrated_unpadded_size": 3_304_836,
    "terminal_zero_padding": 100,
    "chunk_count": 51,
    "exact_chunks": 13,
    "mismatching_chunks": 38,
    "equal_bytes": 1_421_301,
    "differing_bytes": 1_883_635,
    "first_differing_address": 0x00100114,
    "integrated_unpadded_sha256": "fa45d9cdb1569f55bc8a6108f822c938150c3aa1d66d406798181794b60c22aa",
    "integrated_padded_sha256": "9c88e92f2651a141233ec505135d75c23bb56d69662cf88c8fb82027a11a11b2",
    "target_sha256": "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b",
    "prior_exact_chunks": 12,
    "prior_differing_bytes": 1_884_142,
}
EXACT_CHUNKS = [12, 13, 14, *range(37, 47)]


class FrontendEhError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise FrontendEhError(message)


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def file_digest(path: Path) -> str:
    return digest(path.read_bytes())


def run(command: Sequence[str | Path], *, binary: bool = False) -> str | bytes:
    result = subprocess.run(
        [str(item) for item in command], cwd=ROOT, capture_output=True,
        text=not binary, check=False,
    )
    if result.returncode:
        details = result.stderr or result.stdout
        if isinstance(details, bytes):
            details = details.decode(errors="replace")
        fail(f"command failed: {shlex.join(map(str, command))}\n{details[-4000:]}")
    return result.stdout


def one_section(elf: ELFFile, name: str):
    found = [item for item in elf.sections if item.name == name]
    if len(found) != 1:
        fail(f"missing/duplicate section: {name}")
    return found[0]


def payload(elf: ELFFile, section) -> bytes:
    return elf.data[section.offset:section.offset + section.size]


def relocation_count(elf: ELFFile, section) -> int:
    return sum(
        item.size // item.entry_size for item in elf.sections
        if item.type in (4, 9) and item.info == section.index and item.entry_size
    )


def source_contract(args: argparse.Namespace) -> dict:
    return {
        "source": str(args.source.relative_to(ROOT)),
        "source_sha256": SOURCE_SHA256,
        "compiler_base_version": "3.2.2",
        "compiler_target": "ee",
        "compile_flags": list(COMPILE_FLAGS),
        "progress_targets_sha256": file_digest(ROOT / "analysis/progress_targets.csv"),
        "startup_manifest_sha256": file_digest(args.startup_manifest),
        "construction": "semantic DWARF CIE/FDE directives with R_MIPS_32 link relocations",
        "private_byte_fallback": False,
    }


def claims() -> dict[str, bool]:
    return {
        "frontend_unwind_semantics_reconstructed": True,
        "frontend_unwind_relocations_applied": True,
        "frontend_unwind_ranges_raw_exact": True,
        "chunk_14_raw_exact": True,
        "private_target_bytes_stored": False,
        "exact_implementation_selection_complete": False,
        "historical_link_order_proved": False,
        "replacement_elf": False,
        "sjcrunch2_packing_reproduced": False,
        "unpacked_hash_matched": False,
        "packed_hash_matched": False,
    }


def fde_document() -> list[dict]:
    return [
        {"address": address, "size": size, "function": name, "group": group}
        for address, size, name, group in FDES
    ]


def group_document() -> list[dict]:
    return [dict(row) for row in GROUPS]


def frozen_document(args: argparse.Namespace, result: dict) -> dict:
    return {
        "format": FORMAT,
        "schema_version": SCHEMA,
        "source_contract": source_contract(args),
        "groups": group_document(),
        "fdes": fde_document(),
        "link_symbols": dict(sorted(EH_LINK_SYMBOLS.items())),
        "result": result,
        "claims": claims(),
    }


def validate_progress_targets() -> None:
    with (ROOT / "analysis/progress_targets.csv").open(newline="", encoding="utf-8") as handle:
        rows = {int(row["address"], 0): row for row in csv.DictReader(handle)}
    for address, _size, name, _group in FDES:
        row = rows.get(address)
        if row is None or row["name"] != name or row["status"] != "MATCHING":
            fail(f"FDE function evidence drift at 0x{address:08x}")


def validate(args: argparse.Namespace) -> dict:
    if file_digest(args.source) != SOURCE_SHA256:
        fail("frontend unwind source SHA-256 drift")
    text = args.source.read_text(encoding="utf-8")
    if ".incbin" in text or "SNES_EMU" in text:
        fail("frontend unwind source must not embed or name a private target")
    startup.validate(startup.parse_args(["validate", "--manifest", str(args.startup_manifest)]))
    validate_progress_targets()
    try:
        document = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read frontend unwind manifest: {exc}")
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("frontend unwind manifest identity drift")
    if document.get("source_contract") != source_contract(args):
        fail("frontend unwind source contract drift")
    if document.get("groups") != group_document() or document.get("fdes") != fde_document():
        fail("frontend unwind roster drift")
    if document.get("link_symbols") != dict(sorted(EH_LINK_SYMBOLS.items())):
        fail("frontend unwind linker-symbol roster drift")
    result = document.get("result", {})
    for key, value in EXPECTED.items():
        if result.get(key) != value:
            fail(f"frozen frontend unwind metric drift: {key}")
    if result.get("exact_chunk_indices") != EXACT_CHUNKS:
        fail("frontend unwind exact-chunk roster drift")
    if sorted(result.get("exact_chunk_indices", []) + result.get("mismatching_chunk_indices", [])) != list(range(51)):
        fail("frontend unwind chunk accounting drift")
    if result["equal_bytes"] + result["differing_bytes"] != result["target_initialized_size"]:
        fail("frontend unwind byte accounting drift")
    if document.get("claims") != claims():
        fail("frontend unwind claim boundary drift")
    return document


def verify_relocatable(path: Path) -> None:
    elf = ELFFile(path)
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 1):
        fail("frontend unwind object is not little-endian MIPS ELF32 ET_REL")
    seen = 0
    for row in GROUPS:
        section = one_section(elf, row["section"])
        if section.type != 1 or section.size != row["size"]:
            fail(f"unwind section geometry drift: {row['section']}")
        if digest(payload(elf, section)) != row["unrelocated_sha256"]:
            fail(f"unrelocated unwind payload drift: {row['section']}")
        count = relocation_count(elf, section)
        if count != row["relocations"]:
            fail(f"unwind relocation count drift: {row['section']}")
        seen += count
    if seen != EXPECTED["frontend_eh_relocations"]:
        fail("aggregate unwind relocation count drift")


def render_script(sections: Sequence[dict[str, str]]) -> str:
    lines = ['OUTPUT_FORMAT("elf32-littlemips")', "OUTPUT_ARCH(mips)", "ENTRY(_start)"]
    symbols = {**startup.LINK_SYMBOLS, **EH_LINK_SYMBOLS}
    lines.extend(f"{name} = 0x{address:08x};" for name, address in symbols.items())
    lines.extend([
        "SECTIONS {",
        "  .startup 0x00100000 : { *(.text.stage3g.crt0) }",
        "  .text 0x00100114 : { *(.text) }",
        "  .rodata : { *(.rodata) }",
        "  .data : { *(.data) }",
        "  .bss (NOLOAD) : { *(.bss) *(.bss.stage3.compatibility) }",
    ])
    placements = [
        (int(row["target_address"], 0), "fixed", row)
        for row in sections if row["section"] not in startup.ABSORBED_ZERO_FILL
    ]
    placements.extend((row["address"], "eh", row) for row in GROUPS)
    for _address, kind, row in sorted(placements):
        if kind == "eh":
            lines.append(
                f"  {row['section']} 0x{row['address']:08x} : "
                f"{{ KEEP(*({row['section']})) }}"
            )
        else:
            suffix = " (NOLOAD)" if row["region"] == "zero-fill" else ""
            lines.append(
                f"  {row['section']} {row['target_address']}{suffix} : "
                f"{{ KEEP(*({row['section']})) }}"
            )
    lines.append("  .bss.stage3g.crt0 0x00426e80 (NOLOAD) : { KEEP(*(.bss.stage3g.crt0)) }")
    discarded = " ".join(f"*({name})" for name in startup.ABSORBED_ZERO_FILL)
    lines.append(
        "  /DISCARD/ : { " + discarded
        + " *(.data.stage3g.crt0) *(.reginfo) *(.mdebug*) *(.comment)"
        + " *(.pdr) *(.gnu.attributes) }"
    )
    lines.append("}")
    return "\n".join(lines) + "\n"


def probe(args: argparse.Namespace) -> dict:
    document = startup.validate(startup.parse_args([
        "validate", "--manifest", str(args.startup_manifest),
        "--sections", str(args.sections), "--layout", str(args.layout),
    ]))
    sections, layout = startup.load_inputs(startup.parse_args([
        "validate", "--sections", str(args.sections), "--layout", str(args.layout),
    ]))
    reference = args.reference.read_bytes()
    if digest(reference) != EXPECTED["target_sha256"]:
        fail("private unpacked reference SHA-256 drift")
    if not args.input.is_file() or not args.startup_object.is_file():
        fail("missing Stage-3F/startup build output; run make startup-integration first")
    data_backing.check_sections(args.input, sections)
    startup.verify_relocatable(args.startup_object)
    compiler = resolve_tool(args.compiler)
    version = str(run([compiler, "-dumpversion"])).strip()
    target = str(run([compiler, "-dumpmachine"])).strip()
    if (version, target) != ("3.2.2", "ee"):
        fail("frontend unwind integration requires EE GCC 3.2.2")
    linker = sibling_tool(compiler, args.ld, "ld")
    objcopy = sibling_tool(compiler, args.objcopy, "objcopy")
    args.build_dir.mkdir(parents=True, exist_ok=True)
    unwind_object = args.build_dir / "frontend-eh-frames.o"
    linker_script = args.build_dir / "frontend-eh-integration.ld"
    output = args.build_dir / "stage3h-frontend-eh-integrated.elf"
    raw_path = args.build_dir / "stage3h-frontend-eh-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3h-frontend-eh-integrated.padded.bin"
    run([compiler, *COMPILE_FLAGS, "-c", args.source, "-o", unwind_object])
    verify_relocatable(unwind_object)
    linker_script.write_text(render_script(sections), encoding="utf-8")
    run([linker, "-EL", "-T", linker_script, "-o", output,
         args.startup_object, args.input, unwind_object])
    elf = ELFFile(output)
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 2):
        fail("frontend unwind output is not little-endian MIPS ELF32 ET_EXEC")
    if any(item.type in (4, 9) and item.size for item in elf.sections):
        fail("frontend unwind output retains relocations")
    if struct.unpack_from("<I", elf.data, 24)[0] != EXPECTED["integrated_entry_address"]:
        fail("frontend unwind output entry drift")
    startup.verify_symbols(elf)
    startup.verify_fixed_output(elf, sections)
    startup_section = one_section(elf, ".startup")
    if payload(elf, startup_section) != reference[:startup.STARTUP_SIZE]:
        fail("startup corridor regressed")
    linked_bytes = 0
    for row in GROUPS:
        section = one_section(elf, row["section"])
        expected = reference[
            row["address"] - startup.TARGET_BASE:
            row["address"] - startup.TARGET_BASE + row["size"]
        ]
        actual = payload(elf, section)
        if (
            section.address != row["address"] or section.size != row["size"]
            or digest(actual) != row["linked_sha256"] or actual != expected
        ):
            fail(f"linked frontend unwind range differs: {row['section']}")
        linked_bytes += len(actual)
    run([objcopy, "-O", "binary", output, raw_path])
    unpadded = raw_path.read_bytes()
    if len(unpadded) > len(reference):
        fail("frontend unwind image exceeds target initialized size")
    padded = unpadded + bytes(len(reference) - len(unpadded))
    padded_path.write_bytes(padded)
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [index for index, (left, right) in enumerate(zip(padded, reference)) if left != right]
    if not differences:
        fail("unexpected complete match; this gate is not a replacement-ELF claim")
    result = {
        "target_entry_address": layout["entry_address"],
        "integrated_entry_address": struct.unpack_from("<I", elf.data, 24)[0],
        "frontend_eh_groups": len(GROUPS),
        "frontend_fdes": len(FDES),
        "frontend_eh_bytes": linked_bytes,
        "frontend_eh_relocations": sum(row["relocations"] for row in GROUPS),
        "target_initialized_size": len(reference),
        "integrated_unpadded_size": len(unpadded),
        "terminal_zero_padding": len(reference) - len(unpadded),
        "chunk_count": len(exact) + len(different),
        "exact_chunks": len(exact),
        "mismatching_chunks": len(different),
        "exact_chunk_indices": exact,
        "mismatching_chunk_indices": different,
        "equal_bytes": len(reference) - len(differences),
        "differing_bytes": len(differences),
        "first_differing_address": startup.TARGET_BASE + differences[0],
        "integrated_unpadded_sha256": digest(unpadded),
        "integrated_padded_sha256": digest(padded),
        "target_sha256": digest(reference),
        "prior_exact_chunks": document["result"]["exact_chunks"],
        "prior_differing_bytes": document["result"]["differing_bytes"],
    }
    return result


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "probe", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--startup-object", type=Path, default=DEFAULT_STARTUP_OBJECT)
    parser.add_argument("--startup-manifest", type=Path, default=DEFAULT_STARTUP_MANIFEST)
    parser.add_argument("--sections", type=Path, default=DEFAULT_SECTIONS)
    parser.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--compiler", default="ee-gcc")
    parser.add_argument("--ld")
    parser.add_argument("--objcopy")
    args = parser.parse_args(argv)
    for name, value in vars(args).items():
        if isinstance(value, Path):
            setattr(args, name, value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "validate":
            document = validate(args)
        else:
            result = probe(args)
            document = frozen_document(args, result)
            if args.command == "capture":
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(
                    json.dumps(document, indent=2, sort_keys=True) + "\n",
                    encoding="utf-8",
                )
            elif validate(args) != document:
                fail("private frontend unwind result differs from frozen manifest")
        result = document["result"]
        print(
            "verified frontend C++ unwind integration: "
            f"groups={result['frontend_eh_groups']} fdes={result['frontend_fdes']} "
            f"bytes={result['frontend_eh_bytes']} relocations={result['frontend_eh_relocations']}"
        )
        print(
            f"whole-image chunks={result['exact_chunks']}/{result['chunk_count']} "
            f"remaining={result['mismatching_chunks']} differing_bytes={result['differing_bytes']}; "
            "complete ELF verified separately"
        )
        return 0
    except (
        FrontendEhError, startup.StartupIntegrationError,
        data_backing.DataBackingError, link_layout_probe.LinkLayoutProbeError,
        OSError, ValueError, KeyError, RuntimeError,
    ) as exc:
        print(f"frontend unwind integration: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
