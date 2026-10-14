#!/usr/bin/env python3
"""Exact source gate for SNES Station's PS2LIB xprintf lineage."""
from __future__ import annotations

import csv
import hashlib
import re
import struct
import subprocess
from pathlib import Path
from types import SimpleNamespace

import runtime_members as rm
import runtime_overrides as ro
from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "xprintf-source-recovery"
SOURCE = ROOT / "src" / "ps2" / "xprintf.c"
CORE_LISTING = ROOT / "analysis" / "functions" / "ps2lib_vsnprintf_0019d84c.asm"
VPRINTF_LISTING = ROOT / "analysis" / "functions" / "vprintf_0019faa8.asm"
WRAPPER_ASM = ROOT / "matching" / "candidates" / "stage3p_code_residual_exact.S"
OVERRIDES = ROOT / "analysis" / "link_identity" / "runtime_overrides.tsv"

CORE_ADDR = 0x0019D84C
CORE_SIZE = 0x0B18
RODATA_ADDR = 0x001BA478

WRAPPERS = (
    ("vsprintf", "F_vsprintf", 0x0019E364, 0x24, {"vsnprintf": 0x0019E2E0}),
    ("printf", "F_printf", 0x0019E388, 0x48, {"vprintf": 0x0019FAA8}),
    ("sprintf", "F_sprintf", 0x0019E3D0, 0x44, {"vsprintf": 0x0019E364}),
)


def listing_bytes(path: Path, start: int, size: int) -> bytes:
    byte_map: dict[int, int] = {}
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        stripped = line.strip()
        if ":" not in stripped:
            continue
        addr_text, rest = stripped.split(":", 1)
        if re.fullmatch(r"[0-9A-Fa-f]+", addr_text) is None:
            continue
        fields = rest.split()
        if len(fields) < 4 or any(
            re.fullmatch(r"[0-9A-Fa-f]{2}", field) is None for field in fields[:4]
        ):
            continue
        address = int(addr_text, 16)
        raw = bytes(int(field, 16) for field in fields[:4])
        for offset, value in enumerate(raw):
            byte_map[address + offset] = value
    missing = [a for a in range(start, start + size) if a not in byte_map]
    if missing:
        raise SystemExit(
            f"{path.name}: missing {len(missing)} target bytes; first=0x{missing[0]:08x}"
        )
    return bytes(byte_map[a] for a in range(start, start + size))


def exact_wrapper_span() -> bytes:
    text = WRAPPER_ASM.read_text(encoding="utf-8")
    start = text.index("stage3p_tail_gap_0019e364:")
    end = text.index(".size stage3p_tail_gap_0019e364", start)
    words = []
    for line in text[start:end].splitlines():
        m = re.search(r"\.word\s+0x([0-9A-Fa-f]{8})", line)
        if m:
            words.append(int(m.group(1), 16))
    raw = b"".join(struct.pack("<I", word) for word in words)
    if len(raw) != 0xB0:
        raise SystemExit(f"wrapper exact span drift: {len(raw)} != 176")
    return raw


def run(command: list[str | Path]) -> None:
    cp = subprocess.run(
        [str(x) for x in command],
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    if cp.returncode:
        raise SystemExit(
            f"command failed: {' '.join(map(str, command))}\n{cp.stdout[-8000:]}"
        )


def compile_obj(compiler: Path, include_flags: list[str], define: str, name: str,
                flags: tuple[str, ...] = rm.FLAGS) -> Path:
    obj = BUILD / name
    run([compiler, *flags, *include_flags, "-D" + define, "-c", SOURCE, "-o", obj])
    return obj


def raw_link(compiler: Path, obj: Path, section: str, address: int,
             provides: dict[str, int], expected: bytes,
             *, rodata_address: int | None = None,
             bss_address: int | None = None) -> bytes:
    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")
    stem = obj.stem
    script = BUILD / f"{stem}.target.ld"
    linked = BUILD / f"{stem}.target-linked.elf"
    raw_path = BUILD / f"{stem}.target-linked.bin"
    provide_text = "\n".join(
        f"PROVIDE({name} = 0x{value:08x});" for name, value in provides.items()
    )
    rodata = (
        f"  . = 0x{rodata_address:08x};\n  .rodata : {{ *(.rodata*) }}\n"
        if rodata_address is not None else ""
    )
    bss = (
        f"  . = 0x{bss_address:08x};\n  .bss : {{ *(.bss*) *(COMMON) }}\n"
        if bss_address is not None else ""
    )
    script.write_text(
        provide_text + f"""
SECTIONS
{{
  . = 0x{address:08x};
  {section} : {{ *({section}) }}
{rodata}{bss}
  /DISCARD/ : {{
    *(.data*) *(.sdata*) *(.sbss*) *(.reginfo) *(.pdr) *(.mdebug*)
    *(.comment) *(.note*)
  }}
}}
""",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", script, obj, "-o", linked])
    run([objcopy, "-j", section, "-O", "binary", linked, raw_path])
    raw = raw_path.read_bytes()
    if raw != expected:
        first = next(
            (i for i, (actual, want) in enumerate(zip(raw, expected)) if actual != want),
            None,
        )
        raise SystemExit(
            f"{stem} raw gate: FAIL bytes={len(raw)}/{len(expected)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )
    return raw


def override_row(symbol: str) -> dict[str, str]:
    with OVERRIDES.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    matches = [row for row in rows if row["symbol"] == symbol]
    if len(matches) != 1:
        raise SystemExit(f"expected one override row for {symbol}, found {len(matches)}")
    return matches[0]


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    compiler = rm.libgcc.resolve_tool(
        str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
    )
    version = subprocess.check_output([str(compiler), "-dumpversion"], text=True).strip()
    machine = subprocess.check_output([str(compiler), "-dumpmachine"], text=True).strip()
    if (version, machine) != ("3.2.2", "ee"):
        raise SystemExit(f"unexpected compiler: {machine} gcc {version}")

    args = SimpleNamespace(
        inputs=(ROOT / "analysis/link_identity/runtime_member_inputs.tsv").resolve(),
        source_cache=None,
        build_dir=(BUILD / "runtime-inputs").resolve(),
    )
    rm.materialize_inputs(args, compiler)

    include_flags: list[str] = ["-I", str(ROOT / "include")]
    for revision, relative in rm.INCLUDE_DIRS:
        include_flags += ["-I", str(args.build_dir / "inputs" / revision / relative)]
    gcc_include = Path(
        subprocess.check_output([str(compiler), "-print-file-name=include"], text=True).strip()
    ).resolve()
    include_flags += ["-I", str(gcc_include)]

    print("source: src/ps2/xprintf.c (promoted target source)")
    print(f"lineage: ps2dev/ps2sdk@{rm.APR15} ee/libc/src/xprintf.c + target puts override")
    print(f"compiler: {machine} gcc {version}")
    print()

    # Historical F_vsnprintf emits the complete contiguous formatter core.
    core_expected = listing_bytes(CORE_LISTING, CORE_ADDR, CORE_SIZE)
    core = compile_obj(compiler, include_flags, "F_vsnprintf", "xprintf-vsnprintf.o")
    image, masks, normalized = rm.libgcc.text_image(core)
    differing = rm.libgcc.differing_unmasked(core_expected, image, masks)
    core_ok = (
        len(image) == CORE_SIZE
        and len(masks) == 27
        and differing == 0
        and normalized == rm.libgcc.normalize_target(core_expected, masks)
    )
    print(
        f"{'MATCH' if core_ok else 'DIFF':5} F_vsnprintf core "
        f"bytes={len(image)}/{CORE_SIZE} relocs={len(masks)} diff={differing}"
    )
    if not core_ok:
        raise SystemExit("XPRINTF historical formatter object gate: FAIL")

    raw = raw_link(
        compiler, core, ".text", CORE_ADDR,
        {
            "strlen": 0x0019C5E8,
            "__udivdi3": 0x001A25B0,
            "__umoddi3": 0x001A2C78,
        },
        core_expected,
        rodata_address=RODATA_ADDR,
    )
    print(f"RAW   F_vsnprintf core bytes={len(raw)}/{CORE_SIZE} MATCH")

    # Three historical wrappers are contiguous in the public exact span.
    wrapper_span = exact_wrapper_span()
    wrapper_base = 0x0019E364
    for name, define, address, size, provides in WRAPPERS:
        expected = wrapper_span[address - wrapper_base:address - wrapper_base + size]
        obj = compile_obj(compiler, include_flags, define, f"xprintf-{name}.o")
        image, masks, normalized = rm.libgcc.text_image(obj)
        differing = rm.libgcc.differing_unmasked(expected, image, masks)
        ok = (
            len(image) == size
            and differing == 0
            and normalized == rm.libgcc.normalize_target(expected, masks)
        )
        print(
            f"{'MATCH' if ok else 'DIFF':5} {define:12} "
            f"bytes={len(image)}/{size} relocs={len(masks)} diff={differing}"
        )
        if not ok:
            raise SystemExit(f"XPRINTF {define} object gate: FAIL")
        raw = raw_link(compiler, obj, ".text", address, provides, expected)
        print(f"RAW   {define:12} bytes={len(raw)}/{size} MATCH")

    # Late-linked vprintf is historical too; its BSS buffer has target VA 0x4454c0.
    vprintf_expected = listing_bytes(VPRINTF_LISTING, 0x0019FAA8, 0x58)
    vprintf_obj = compile_obj(compiler, include_flags, "F_vprintf", "xprintf-vprintf.o")
    image, masks, normalized = rm.libgcc.text_image(vprintf_obj)
    differing = rm.libgcc.differing_unmasked(vprintf_expected, image, masks)
    ok = (
        len(image) == 0x58
        and differing == 0
        and normalized == rm.libgcc.normalize_target(vprintf_expected, masks)
    )
    print(
        f"{'MATCH' if ok else 'DIFF':5} F_vprintf    "
        f"bytes={len(image)}/88 relocs={len(masks)} diff={differing}"
    )
    if not ok:
        raise SystemExit("XPRINTF F_vprintf object gate: FAIL")
    raw = raw_link(
        compiler, vprintf_obj, ".text", 0x0019FAA8,
        {"vsnprintf": 0x0019E2E0, "fioWrite": 0x0019D244},
        vprintf_expected,
        bss_address=0x004454C0,
    )
    print(f"RAW   F_vprintf    bytes={len(raw)}/88 MATCH")

    # SNES Station deliberately overrides PS2LIB puts semantics.
    puts_obj = compile_obj(
        compiler, include_flags, "F_puts", "xprintf-puts.o", flags=ro.FLAGS
    )
    elf = ELFFile(puts_obj)
    symbols = [s for s in elf.symbols if s.name == "puts_like_recovered" and s.section_index != 0]
    if len(symbols) != 1:
        raise SystemExit("XPRINTF target puts symbol missing/ambiguous")
    symbol = symbols[0]
    image = elf.symbol_bytes(symbol, symbol.size)
    masks = elf.relocation_masks(symbol, 4)
    row = override_row("puts")
    normalized_sha = hashlib.sha256(rm.libgcc.normalize_target(image, masks)).hexdigest()
    if (
        symbol.size != int(row["extent_hex"], 0)
        or normalized_sha != row["normalized_sha256"]
    ):
        raise SystemExit(
            f"XPRINTF target puts object gate: FAIL size={symbol.size} "
            f"relocs={len(masks)}"
        )

    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")
    script = BUILD / "xprintf-puts.target.ld"
    linked = BUILD / "xprintf-puts.target-linked.elf"
    raw_path = BUILD / "xprintf-puts.target-linked.bin"
    script.write_text(
        """fioWrite = 0x0019d244;
SECTIONS {
  .text.puts_like_recovered 0x0019e414 : { *(.text.puts_like_recovered) }
  /DISCARD/ : {
    *(.text) *(.data*) *(.bss*) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*)
  }
}
""",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", script, "-o", linked, puts_obj])
    run([objcopy, "-j", ".text.puts_like_recovered", "-O", "binary", linked, raw_path])
    puts_raw = raw_path.read_bytes()
    puts_sha = hashlib.sha256(puts_raw).hexdigest()
    if len(puts_raw) != 0x60 or puts_sha != row["linked_sha256"]:
        raise SystemExit(
            f"XPRINTF target puts raw gate: FAIL bytes={len(puts_raw)}/96 sha={puts_sha}"
        )
    print(
        f"MATCH F_puts target override bytes={len(image)}/96 "
        f"relocs={len(masks)} raw=96/96"
    )

    print()
    print("XPRINTF source gate: formatter core + 5 wrappers + target puts MATCH")


if __name__ == "__main__":
    main()
