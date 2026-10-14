#!/usr/bin/env python3
"""Focused historical-source proof for the SNES Station SIF command layer."""
from __future__ import annotations

import hashlib
import re
import subprocess
from pathlib import Path
from types import SimpleNamespace

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "sifcmd-source-recovery"
MAIN_LISTING = ROOT / "analysis" / "functions" / "sifcmd_0019f138.asm"
IRQ_LISTING = ROOT / "analysis" / "functions" / "sifcmd_irq_0019fb00.asm"

MAIN_BASE = 0x0019F138
MAIN_END = 0x0019F594
IRQ_BASE = 0x0019FBF0
IRQ_END = 0x0019FCD0

SPECS = (
    ("sif_cmd_send.o", "F_sif_cmd_send", 0x0019F138, 0x1A4, "main"),
    ("sif_cmd_main.o", "F_sif_cmd_main", 0x0019F2DC, 0x268, "main"),
    ("sif_cmd_addhandler.o", "F_sif_cmd_addhandler", 0x0019F544, 0x38, "main"),
    ("sif_sreg_get.o", "F_sif_sreg_get", 0x0019F57C, 0x18, "main"),
    ("_sif_cmd_int_handler.o", "F__sif_cmd_int_handler", 0x0019FBF0, 0xE0, "irq"),
)

INSN = re.compile(
    r"^\s*([0-9A-Fa-f]+):\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
)


def listing_map(path: Path) -> dict[int, int]:
    out: dict[int, int] = {}
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        match = INSN.match(line)
        if not match:
            continue
        address = int(match.group(1), 16)
        raw = bytes(int(match.group(i), 16) for i in range(2, 6))
        for offset, value in enumerate(raw):
            out[address + offset] = value
    return out


def target_span(byte_map: dict[int, int], start: int, end: int) -> bytes:
    missing = [address for address in range(start, end) if address not in byte_map]
    if missing:
        raise SystemExit(
            f"target listing missing {len(missing)} byte(s) in "
            f"0x{start:08x}..0x{end - 1:08x}; first=0x{missing[0]:08x}"
        )
    return bytes(byte_map[address] for address in range(start, end))


def checked(command: list[str | Path], *, cwd: Path = ROOT) -> str:
    result = subprocess.run(
        [str(item) for item in command],
        cwd=cwd,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    if result.returncode:
        raise SystemExit(
            f"command failed ({result.returncode}): {' '.join(map(str, command))}\n"
            f"{result.stdout[-8000:]}"
        )
    return result.stdout


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    compiler = rm.libgcc.resolve_tool(
        str(ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1" / "prefix" / "bin" / "ee-gcc")
    )
    version = subprocess.check_output([str(compiler), "-dumpversion"], text=True).strip()
    machine = subprocess.check_output([str(compiler), "-dumpmachine"], text=True).strip()
    if (version, machine) != ("3.2.2", "ee"):
        raise SystemExit(f"unexpected compiler: {machine} gcc {version}")

    args = SimpleNamespace(
        inputs=(ROOT / "analysis" / "link_identity" / "runtime_member_inputs.tsv").resolve(),
        source_cache=None,
        build_dir=(BUILD / "runtime-inputs").resolve(),
    )
    rm.materialize_inputs(args, compiler)

    include_flags: list[str] = []
    for revision, relative in rm.INCLUDE_DIRS:
        include_flags += ["-I", str(args.build_dir / "inputs" / revision / relative)]
    gcc_include = Path(
        subprocess.check_output([str(compiler), "-print-file-name=include"], text=True).strip()
    ).resolve()
    include_flags += ["-I", str(gcc_include)]

    historical = args.build_dir / "inputs" / rm.APR18 / "ee/kernel/src/sifcmd.c"
    source = ROOT / "src" / "ps2" / "sifcmd.c"
    using_local = source.is_file()
    if not using_local:
        source = historical

    main_target = target_span(listing_map(MAIN_LISTING), MAIN_BASE, MAIN_END)
    irq_target = target_span(listing_map(IRQ_LISTING), IRQ_BASE, IRQ_END)
    targets = {"main": (MAIN_BASE, main_target), "irq": (IRQ_BASE, irq_target)}

    print(
        "source: "
        + (
            "src/ps2/sifcmd.c (promoted historical source)"
            if using_local
            else f"ps2dev/ps2sdk@{rm.APR18} ee/kernel/src/sifcmd.c"
        )
    )
    print(f"compiler: {machine} gcc {version}")
    print(f"main target: 0x{MAIN_BASE:08x}..0x{MAIN_END - 1:08x}")
    print(f"irq target:  0x{IRQ_BASE:08x}..0x{IRQ_END - 1:08x}")
    print()

    built: list[Path] = []
    failed = False
    for name, define, address, expected_size, target_key in SPECS:
        obj = BUILD / name
        command = [str(compiler), *rm.FLAGS]
        if using_local:
            command += ["-I", str(ROOT / "include")]
        else:
            command += include_flags
        command += ["-D" + define, "-c", str(source), "-o", str(obj)]
        checked(command)
        built.append(obj)

        image, masks, normalized = rm.libgcc.text_image(obj)
        base, whole = targets[target_key]
        expected = whole[address - base: address - base + expected_size]
        differing = rm.libgcc.differing_unmasked(expected, image, masks)
        ok = (
            len(image) == expected_size
            and differing == 0
            and rm.libgcc.normalize_target(expected, masks) == normalized
        )
        print(
            f"{'MATCH' if ok else 'DIFF':5} {name:26} {define:24} "
            f"bytes={len(image)}/{expected_size} relocs={len(masks)} diff={differing}"
        )
        failed |= not ok

    if failed:
        raise SystemExit("SIF CMD historical-source gate: FAIL")
    print()
    print("SIF CMD historical-source gate: 5/5 MATCH")

    # Stronger proof: link the historical members at the addresses observed in
    # SNES Station, including the original cmd_data .data/.bss placement.
    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")
    linker = BUILD / "sifcmd_target.ld"
    linked = BUILD / "sifcmd.target-linked.elf"
    main_bin = BUILD / "sifcmd.target-linked.main.bin"
    irq_bin = BUILD / "sifcmd.target-linked.irq.bin"

    linker.write_text(
        """ENTRY(_SifSendCmd)

PROVIDE(FlushCache = 0x0019ceb0);
PROVIDE(SifSetDma = 0x0019cee0);
PROVIDE(SifSetReg = 0x0019cef0);
PROVIDE(SifGetReg = 0x0019cf00);
PROVIDE(SifWriteBackDCache = 0x0019cf10);
PROVIDE(DIntr = 0x0019f018);
PROVIDE(EIntr = 0x0019f060);
PROVIDE(AddDmacHandler = 0x0019f5a0);
PROVIDE(RemoveDmacHandler = 0x0019f5b0);
PROVIDE(iSifSetDma = 0x0019f5e0);
PROVIDE(SifSetDChain = 0x0019f5f0);
PROVIDE(EnableDmac = 0x0019fb00);
PROVIDE(DisableDmac = 0x0019fb78);
PROVIDE(iSifSetDChain = 0x0019fd10);

SECTIONS
{
  .text.cmd 0x0019f138 : {
    sif_cmd_send.o(.text)
    sif_cmd_main.o(.text)
    sif_cmd_addhandler.o(.text)
    sif_sreg_get.o(.text)
  }

  .text.irq 0x0019fbf0 : {
    _sif_cmd_int_handler.o(.text)
  }

  .data 0x00425a88 : {
    sif_cmd_main.o(.data)
  }

  .bss 0x00445180 (NOLOAD) : {
    sif_cmd_main.o(.bss)
    sif_cmd_main.o(COMMON)
  }

  /DISCARD/ : {
    *(.comment)
    *(.mdebug*)
    *(.pdr)
    *(.reginfo)
  }
}
""",
        encoding="utf-8",
    )

    checked(
        [
            ld,
            "-T",
            linker.name,
            *(path.name for path in built),
            "-o",
            linked.name,
        ],
        cwd=BUILD,
    )
    checked([objcopy, "-j", ".text.cmd", "-O", "binary", linked.name, main_bin.name], cwd=BUILD)
    checked([objcopy, "-j", ".text.irq", "-O", "binary", linked.name, irq_bin.name], cwd=BUILD)

    main_raw = main_bin.read_bytes()
    irq_raw = irq_bin.read_bytes()
    if main_raw != main_target:
        first = next((i for i, pair in enumerate(zip(main_raw, main_target)) if pair[0] != pair[1]), None)
        raise SystemExit(
            f"SIF CMD raw main gate: FAIL bytes={len(main_raw)}/{len(main_target)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )
    if irq_raw != irq_target:
        first = next((i for i, pair in enumerate(zip(irq_raw, irq_target)) if pair[0] != pair[1]), None)
        raise SystemExit(
            f"SIF CMD raw irq gate: FAIL bytes={len(irq_raw)}/{len(irq_target)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )

    combined = main_raw + irq_raw
    print(f"SIF CMD raw main gate: MATCH {len(main_raw)}/{len(main_target)} bytes")
    print(f"SIF CMD raw irq gate:  MATCH {len(irq_raw)}/{len(irq_target)} bytes")
    print(f"SIF CMD combined raw bytes: {len(combined)}")
    print(f"combined sha256: {hashlib.sha256(combined).hexdigest()}")


if __name__ == "__main__":
    main()
