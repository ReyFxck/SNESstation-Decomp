#!/usr/bin/env python3
"""Rebuild, integrate and verify the exact historical EE startup corridor.

The gate selects the pinned PS2SDK/PS2LIB ``crt0.s`` ahead of the real
Stage-3F aggregate and proves only 0x00100000..0x00100114 plus the ELF entry.
It does not claim that the following application order or replacement ELF is
already exact.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import shlex
import struct
import subprocess
from pathlib import Path
from typing import Sequence

import data_backing
import link_layout_probe
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool, sibling_tool

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_BUILD = ROOT / "build/startup-integration"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/startup_integration.json"

FORMAT = "snesstation-stage3g-exact-startup-integration"
SCHEMA = 1
SOURCE_REPOSITORY = "https://github.com/ps2dev/ps2sdk.git"
SOURCE_REVISION = "694100b78ad5bc8f8248a1138143860af4f8435f"
SOURCE_PATH = "ee/startup/src/crt0.s"
SOURCE_SHA256 = "13ab436418c8b8e815173fc99a5a40f889e7e58c80ad15e6ffc83096bcd15999"
SOURCE_TEXT_SHA256 = "d41e96c4c471147a3c8a1e7ddb18b0c51c577e6b1be9ca944c14e6eef423aae9"
RENAMED_OBJECT_SHA256 = "891f520abc8b0fa2ea214418b629232f57b1511e5c0d7fcdc3b1e9aaa9d0d640"

TARGET_BASE = 0x00100000
STARTUP_END = 0x00100114
STARTUP_SIZE = STARTUP_END - TARGET_BASE
STARTUP_BSS = 0x00426E80
STARTUP_BSS_SIZE = 0x180
ABSORBED_ZERO_FILL = {
    ".bss.stage3ce.va_00426e80": ("iRam00426e80", 0x00426E80),
    ".bss.stage3ce.va_00426fa4": ("uRam00426fa4", 0x00426FA4),
    ".bss.stage3ce.va_00426fb4": ("uRam00426fb4", 0x00426FB4),
    ".bss.stage3ce.va_00426fc4": ("puRam00426fc4", 0x00426FC4),
}
LINK_SYMBOLS = {
    "_fbss": 0x00426E00,
    "_end": 0x00450C18,
    "_gp": 0x0042EDF0,
    "_stack": 0xFFFFFFFF,
    "_stack_size": 0x00020000,
    "_heap_size": 0xFFFFFFFF,
    "main": 0x00104F18,
    **{symbol: address for symbol, address in ABSORBED_ZERO_FILL.values()},
}
STARTUP_SYMBOLS = {
    "_start": (0x00100008, 0xD8),
    "_exit": (0x001000E0, 0x2C),
    "_root": (0x0010010C, 0x08),
    "_args": (0x00426E80, 0),
    "_args_ptr": (0x00426FC4, 0),
}
COMPILE_FLAGS = (
    "-G0", "-O2", "-EL", "-pipe", "-Wall", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-ffreestanding", "-fno-builtin",
    "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls",
    "-march=r5900", "-mtune=r5900", "-DPS2_EE", "-D_EE",
    "-DLSB_FIRST", "-DALIGN_DWORD", "-DCODE_PLATFORM=3", "-w",
)
EXPECTED = {
    "target_entry_address": 0x00100008,
    "integrated_entry_address": 0x00100008,
    "startup_base_address": TARGET_BASE,
    "startup_end_address": STARTUP_END,
    "startup_exact_bytes": STARTUP_SIZE,
    "startup_text_sha256": "ecdea86dc1a457ea1951ae338815041e1a5a5f7eecd80324240aaba9874d092f",
    "startup_functions_exact": 3,
    "startup_relocations_applied": 27,
    "startup_bss_address": STARTUP_BSS,
    "startup_bss_size": STARTUP_BSS_SIZE,
    "absorbed_zero_fill_sections": 4,
    "preserved_fixed_sections": 175,
    "fixed_sections_accounted": 179,
    "first_differing_address": STARTUP_END,
    "target_initialized_size": 3_304_936,
    "integrated_unpadded_size": 3_304_836,
    "terminal_zero_padding": 100,
    "chunk_count": 51,
    "exact_chunks": 12,
    "mismatching_chunks": 39,
    "equal_bytes": 1_420_794,
    "differing_bytes": 1_884_142,
    "integrated_unpadded_sha256": "a753d3043db427ad9d4dac80dfe057bfe8a7f191ee3735e464ac463ac2fc2a89",
    "integrated_padded_sha256": "0a6f0c0b8c84367e5e5d8ea44e40ae7e502a4bcba1967bbc5c45d20356ae5512",
    "target_sha256": "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b",
}
EXACT_CHUNKS = [12, 13, *range(37, 47)]


class StartupIntegrationError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise StartupIntegrationError(message)


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
        rendered = shlex.join(str(item) for item in command)
        fail(f"command failed ({result.returncode}): {rendered}\n{details[-4000:]}")
    return result.stdout


def claims() -> dict[str, bool]:
    return {
        "historical_startup_source_pinned": True,
        "startup_relocations_applied": True,
        "startup_corridor_raw_exact": True,
        "elf_entry_exact": True,
        "startup_bss_layout_integrated": True,
        "behavioral_startup_lift_removed": False,
        "exact_implementation_selection_complete": False,
        "historical_link_order_proved": False,
        "replacement_elf": False,
        "sjcrunch2_packing_reproduced": False,
        "packed_hash_matched": False,
        "unpacked_hash_matched": False,
    }


def source_contract(args: argparse.Namespace, sections: Sequence[dict[str, str]], layout: dict) -> dict:
    return {
        "repository": SOURCE_REPOSITORY,
        "revision": SOURCE_REVISION,
        "path": SOURCE_PATH,
        "source_sha256": SOURCE_SHA256,
        "unrelocated_text_sha256": SOURCE_TEXT_SHA256,
        "renamed_relocatable_sha256": RENAMED_OBJECT_SHA256,
        "compiler_base_version": "3.2.2",
        "compiler_target": "ee",
        "compile_flags": list(COMPILE_FLAGS),
        "data_backing_sections_sha256": file_digest(args.sections),
        "unpacked_layout_sha256": file_digest(args.layout),
        "fixed_section_count": len(sections),
        "target_image_base": layout["image"]["base_address"],
        "target_initialized_size": layout["image"]["initialized_size"],
    }


def frozen_document(args: argparse.Namespace, sections, layout, result) -> dict:
    return {
        "format": FORMAT,
        "schema_version": SCHEMA,
        "source_contract": source_contract(args, sections, layout),
        "link_symbols": dict(sorted(LINK_SYMBOLS.items())),
        "startup_symbols": {
            name: {"address": address, "size": size}
            for name, (address, size) in sorted(STARTUP_SYMBOLS.items())
        },
        "result": result,
        "claims": claims(),
    }


def load_inputs(args: argparse.Namespace) -> tuple[list[dict[str, str]], dict]:
    sections = link_layout_probe.load_sections(args.sections)
    layout = link_layout_probe.load_layout(args.layout)
    if len(sections) != EXPECTED["fixed_sections_accounted"]:
        fail("Stage-3F section count drift")
    absorbed = {row["section"]: row for row in sections if row["section"] in ABSORBED_ZERO_FILL}
    if set(absorbed) != set(ABSORBED_ZERO_FILL):
        fail("startup BSS overlap roster drift")
    for name, row in absorbed.items():
        _symbol, address = ABSORBED_ZERO_FILL[name]
        if (
            row["region"] != "zero-fill"
            or int(row["target_address"], 0) != address
            or int(row["extent_hex"], 0) != 4
            or not (STARTUP_BSS <= address < STARTUP_BSS + STARTUP_BSS_SIZE)
        ):
            fail(f"startup BSS overlap geometry drift: {name}")
    return sections, layout


def validate_evidence() -> None:
    checks = (
        (ROOT / "analysis/matching/hunt500plus-v33-validated-204.tsv",
         ("0x00100008", "_start", "\t216\t", "\tMATCH\t0\t")),
        (ROOT / "analysis/matching/hunt1041-v49-validated-20.tsv",
         ("0x001000e0", "_exit+_root", "\t44+8\t", "\tMATCH\t0\t", SOURCE_REVISION[:7])),
    )
    for path, tokens in checks:
        text = path.read_text(encoding="utf-8")
        if len([line for line in text.splitlines() if all(token in line for token in tokens)]) != 1:
            fail(f"startup match evidence drift: {path.relative_to(ROOT)}")


def validate(args: argparse.Namespace) -> dict:
    sections, layout = load_inputs(args)
    validate_evidence()
    try:
        document = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read startup manifest {args.manifest}: {exc}")
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("startup manifest identity drift")
    if document.get("source_contract") != source_contract(args, sections, layout):
        fail("startup source contract drift")
    if document.get("link_symbols") != dict(sorted(LINK_SYMBOLS.items())):
        fail("startup link-symbol contract drift")
    expected_symbols = {
        name: {"address": address, "size": size}
        for name, (address, size) in sorted(STARTUP_SYMBOLS.items())
    }
    if document.get("startup_symbols") != expected_symbols:
        fail("startup symbol contract drift")
    result = document.get("result", {})
    for key, expected in EXPECTED.items():
        if result.get(key) != expected:
            fail(f"frozen startup metric drift: {key}")
    if result.get("exact_chunk_indices") != EXACT_CHUNKS:
        fail("startup exact-chunk roster drift")
    if sorted(result["exact_chunk_indices"] + result.get("mismatching_chunk_indices", [])) != list(range(51)):
        fail("startup chunk accounting drift")
    if result["equal_bytes"] + result["differing_bytes"] != result["target_initialized_size"]:
        fail("startup byte accounting drift")
    if document.get("claims") != claims():
        fail("startup claim boundary drift")
    return document


def materialize_source(args: argparse.Namespace) -> Path:
    if args.source:
        if not args.source.is_file() or file_digest(args.source) != SOURCE_SHA256:
            fail("explicit startup source is missing or has the wrong SHA-256")
        return args.source
    candidates = (
        ROOT / "build/upstream/ps2sdk-20040415" / SOURCE_PATH,
        args.build_dir / "inputs" / SOURCE_REVISION / SOURCE_PATH,
    )
    for source in candidates:
        if source.is_file():
            if file_digest(source) != SOURCE_SHA256:
                fail(f"cached startup source SHA-256 drift: {source}")
            return source
    cache = args.source_cache or args.build_dir / "source-cache.git"
    if not cache.exists():
        cache.parent.mkdir(parents=True, exist_ok=True)
        run(["git", "init", "--bare", "--quiet", cache])
    if subprocess.run(
        ["git", "-C", str(cache), "cat-file", "-e", f"{SOURCE_REVISION}^{{commit}}"],
        capture_output=True,
    ).returncode:
        print(f"startup source: fetch ps2sdk@{SOURCE_REVISION[:8]}", flush=True)
        run(["git", "-C", cache, "fetch", "--quiet", "--depth=1", SOURCE_REPOSITORY, SOURCE_REVISION])
    actual = str(run(["git", "-C", cache, "rev-parse", f"{SOURCE_REVISION}^{{commit}}"])).strip()
    if actual != SOURCE_REVISION:
        fail("startup source revision mismatch")
    payload = run(["git", "-C", cache, "show", f"{SOURCE_REVISION}:{SOURCE_PATH}"], binary=True)
    assert isinstance(payload, bytes)
    if digest(payload) != SOURCE_SHA256:
        fail("fetched startup source SHA-256 mismatch")
    source = candidates[1]
    source.parent.mkdir(parents=True, exist_ok=True)
    source.write_bytes(payload)
    return source


def rename_script() -> str:
    return """SECTIONS
{
  .text.stage3g.crt0 : { *(.text) }
  .data.stage3g.crt0 : { *(.data) }
  .bss.stage3g.crt0 : { *(.bss) }
  /DISCARD/ : { *(.reginfo) *(.mdebug*) *(.comment) *(.pdr) *(.gnu.attributes) }
}
"""


def render_script(sections: Sequence[dict[str, str]]) -> str:
    lines = ['OUTPUT_FORMAT("elf32-littlemips")', "OUTPUT_ARCH(mips)", "ENTRY(_start)"]
    lines.extend(f"{name} = 0x{address:08x};" for name, address in LINK_SYMBOLS.items())
    lines.extend([
        "SECTIONS {",
        "  .startup 0x00100000 : { *(.text.stage3g.crt0) }",
        "  .text 0x00100114 : { *(.text) }",
        "  .rodata : { *(.rodata) }",
        "  .data : { *(.data) }",
        "  .bss (NOLOAD) : { *(.bss) *(.bss.stage3.compatibility) }",
    ])
    for row in sections:
        if row["section"] in ABSORBED_ZERO_FILL:
            continue
        suffix = " (NOLOAD)" if row["region"] == "zero-fill" else ""
        lines.append(f"  {row['section']} {row['target_address']}{suffix} : {{ KEEP(*({row['section']})) }}")
    lines.append("  .bss.stage3g.crt0 0x00426e80 (NOLOAD) : { KEEP(*(.bss.stage3g.crt0)) }")
    discarded = " ".join(f"*({name})" for name in ABSORBED_ZERO_FILL)
    lines.append("  /DISCARD/ : { " + discarded + " *(.data.stage3g.crt0) *(.reginfo) *(.mdebug*) *(.comment) *(.pdr) *(.gnu.attributes) }")
    lines.append("}")
    return "\n".join(lines) + "\n"


def one_section(elf: ELFFile, name: str):
    rows = [item for item in elf.sections if item.name == name]
    if len(rows) != 1:
        fail(f"missing/duplicate output section: {name}")
    return rows[0]


def section_payload(elf: ELFFile, item) -> bytes:
    return bytes(item.size) if item.type == 8 else elf.data[item.offset:item.offset + item.size]


def relocation_count(elf: ELFFile, target_name: str) -> int:
    target = one_section(elf, target_name)
    return sum(
        item.size // item.entry_size for item in elf.sections
        if item.type in (4, 9) and item.info == target.index and item.entry_size
    )


def verify_relocatable(path: Path) -> None:
    elf = ELFFile(path)
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 1):
        fail("startup object is not little-endian MIPS ELF32 ET_REL")
    text = one_section(elf, ".text.stage3g.crt0")
    bss = one_section(elf, ".bss.stage3g.crt0")
    if text.size != STARTUP_SIZE or digest(section_payload(elf, text)) != SOURCE_TEXT_SHA256:
        fail("startup relocatable text drift")
    if bss.size != STARTUP_BSS_SIZE or bss.type != 8:
        fail("startup relocatable BSS drift")
    if relocation_count(elf, text.name) != EXPECTED["startup_relocations_applied"]:
        fail("startup relocation roster drift")
    relative = {
        "_start": (0x08, 0xD8), "_exit": (0xE0, 0x2C), "_root": (0x10C, 0x08),
        "_args": (0, 0), "_args_ptr": (0x144, 0),
    }
    for name, expected in relative.items():
        symbol = elf.find_symbol(name)
        if (symbol.value, symbol.size) != expected:
            fail(f"startup relocatable symbol drift: {name}")


def symbol_map(elf: ELFFile) -> dict[str, object]:
    result = {}
    for symbol in elf.symbols:
        if symbol.name and symbol.section_index != 0:
            result.setdefault(symbol.name, symbol)
    return result


def verify_symbols(elf: ELFFile) -> None:
    symbols = symbol_map(elf)
    for name, (address, size) in STARTUP_SYMBOLS.items():
        symbol = symbols.get(name)
        if symbol is None or symbol.value != address or symbol.size != size:
            fail(f"linked startup symbol drift: {name}")
    for name, address in LINK_SYMBOLS.items():
        symbol = symbols.get(name)
        if symbol is None or symbol.value != address:
            fail(f"linked startup absolute symbol drift: {name}")


def verify_fixed_output(elf: ELFFile, sections: Sequence[dict[str, str]]) -> None:
    by_name = {item.name: item for item in elf.sections if item.name}
    startup_bss = one_section(elf, ".bss.stage3g.crt0")
    if startup_bss.address != STARTUP_BSS or startup_bss.size != STARTUP_BSS_SIZE or startup_bss.type != 8:
        fail("linked startup BSS geometry drift")
    preserved = 0
    for row in sections:
        name = row["section"]
        if name in ABSORBED_ZERO_FILL:
            if name in by_name:
                fail(f"startup BSS alias was not absorbed: {name}")
            continue
        item = by_name.get(name)
        expected_type = 8 if row["region"] == "zero-fill" else 1
        if item is None or item.address != int(row["target_address"], 0) or item.size != int(row["extent_hex"], 0) or item.type != expected_type:
            fail(f"fixed Stage-3F output geometry drift: {name}")
        if row["region"] == "initialized" and digest(section_payload(elf, item)) != row["sha256"]:
            fail(f"fixed Stage-3F output payload drift: {name}")
        preserved += 1
    if preserved != EXPECTED["preserved_fixed_sections"]:
        fail("preserved fixed-section count drift")


def elf_entry(elf: ELFFile) -> int:
    return struct.unpack_from(elf.endian + "I", elf.data, 24)[0]


def probe(args: argparse.Namespace, sections: Sequence[dict[str, str]], layout: dict) -> dict:
    if not args.input.is_file():
        fail(f"missing Stage-3F aggregate: {args.input}; run make data-backing-check")
    reference = args.reference.read_bytes()
    if digest(reference) != EXPECTED["target_sha256"]:
        fail("private unpacked reference SHA-256 drift")
    data_backing.check_sections(args.input, sections)
    source = materialize_source(args)
    compiler = resolve_tool(args.compiler)
    if (str(run([compiler, "-dumpversion"])).strip(), str(run([compiler, "-dumpmachine"])).strip()) != ("3.2.2", "ee"):
        fail("startup integration requires historical EE GCC 3.2.2")
    linker = sibling_tool(compiler, args.ld, "ld")
    objcopy = sibling_tool(compiler, args.objcopy, "objcopy")
    args.build_dir.mkdir(parents=True, exist_ok=True)
    source_object = args.build_dir / "crt0-20040415.o"
    renamed_object = args.build_dir / "crt0-stage3g.o"
    rename_path = args.build_dir / "rename-crt0.ld"
    script_path = args.build_dir / "startup-integration.ld"
    output = args.build_dir / "stage3g-startup-integrated.elf"
    raw_path = args.build_dir / "stage3g-startup-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3g-startup-integrated.padded.bin"
    run([compiler, *COMPILE_FLAGS, "-c", source, "-o", source_object])
    rename_path.write_text(rename_script(), encoding="utf-8")
    run([linker, "-r", "-EL", "-T", rename_path, "-o", renamed_object, source_object])
    if file_digest(renamed_object) != RENAMED_OBJECT_SHA256:
        fail("renamed startup relocatable SHA-256 drift")
    verify_relocatable(renamed_object)
    script_path.write_text(render_script(sections), encoding="utf-8")
    run([linker, "-EL", "-T", script_path, "-o", output, renamed_object, args.input])
    elf = ELFFile(output)
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 2):
        fail("startup integration output is not little-endian MIPS ELF32 ET_EXEC")
    if any(item.type in (4, 9) and item.size for item in elf.sections):
        fail("startup integration output retains relocations")
    if elf_entry(elf) != EXPECTED["integrated_entry_address"]:
        fail("startup integration entry drift")
    verify_symbols(elf)
    verify_fixed_output(elf, sections)
    startup = one_section(elf, ".startup")
    if startup.address != TARGET_BASE or startup.size != STARTUP_SIZE:
        fail("linked startup section geometry drift")
    startup_payload = section_payload(elf, startup)
    if startup_payload != reference[:STARTUP_SIZE] or digest(startup_payload) != EXPECTED["startup_text_sha256"]:
        fail("linked startup corridor differs from private target")
    run([objcopy, "-O", "binary", output, raw_path])
    unpadded = raw_path.read_bytes()
    if len(unpadded) > len(reference):
        fail("startup-integrated image exceeds target initialized size")
    padded = unpadded + bytes(len(reference) - len(unpadded))
    padded_path.write_bytes(padded)
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [index for index, (left, right) in enumerate(zip(padded, reference)) if left != right]
    if not differences:
        fail("unexpected complete image match; this is not a replacement-ELF gate")
    result = {
        "target_entry_address": layout["entry_address"],
        "integrated_entry_address": elf_entry(elf),
        "startup_base_address": TARGET_BASE,
        "startup_end_address": STARTUP_END,
        "startup_exact_bytes": len(startup_payload),
        "startup_text_sha256": digest(startup_payload),
        "startup_functions_exact": 3,
        "startup_relocations_applied": relocation_count(ELFFile(renamed_object), ".text.stage3g.crt0"),
        "startup_bss_address": one_section(elf, ".bss.stage3g.crt0").address,
        "startup_bss_size": one_section(elf, ".bss.stage3g.crt0").size,
        "absorbed_zero_fill_sections": len(ABSORBED_ZERO_FILL),
        "preserved_fixed_sections": len(sections) - len(ABSORBED_ZERO_FILL),
        "fixed_sections_accounted": len(sections),
        "first_differing_address": TARGET_BASE + differences[0],
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
        "integrated_unpadded_sha256": digest(unpadded),
        "integrated_padded_sha256": digest(padded),
        "target_sha256": digest(reference),
    }
    for key, expected in EXPECTED.items():
        if result.get(key) != expected:
            fail(f"frozen startup result drift for {key}: {result.get(key)!r} != {expected!r}")
    return result


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "probe", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--sections", type=Path, default=DEFAULT_SECTIONS)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--source", type=Path)
    parser.add_argument("--source-cache", type=Path)
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
            sections, layout = load_inputs(args)
            validate_evidence()
            result = probe(args, sections, layout)
            document = frozen_document(args, sections, layout, result)
            if args.command == "capture":
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n", encoding="utf-8")
            elif validate(args) != document:
                fail("private startup integration differs from frozen manifest")
        result = document["result"]
        print(
            "verified exact EE startup integration: "
            f"entry=0x{result['integrated_entry_address']:08x} "
            f"startup={result['startup_exact_bytes']}/{result['startup_exact_bytes']} bytes "
            f"functions={result['startup_functions_exact']}/3 relocations={result['startup_relocations_applied']}"
        )
        print(
            f"first remaining difference=0x{result['first_differing_address']:08x}; "
            f"chunks={result['exact_chunks']}/{result['chunk_count']}; replacement ELF: not yet"
        )
        return 0
    except (StartupIntegrationError, data_backing.DataBackingError,
            link_layout_probe.LinkLayoutProbeError, OSError, ValueError,
            KeyError, RuntimeError) as exc:
        print(f"startup integration: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
