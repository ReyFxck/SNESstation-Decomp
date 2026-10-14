#!/usr/bin/env python3
"""Exact historical-source gate for SNES Station's EE iopcontrol.c member."""
from __future__ import annotations

import re
import subprocess
from pathlib import Path
from types import SimpleNamespace

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "iopcontrol-source-recovery"
SOURCE = ROOT / "src" / "ps2" / "iopcontrol.c"
LISTING = ROOT / "analysis" / "functions" / "loadfile_iop_0019d600.asm"
ADDRESS = 0x0019D740
SIZE = 0x10C
DEFINE = "F_SifIopReset"


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
            re.fullmatch(r"[0-9A-Fa-f]{2}", field) is None
            for field in fields[:4]
        ):
            continue
        address = int(addr_text, 16)
        raw = bytes(int(field, 16) for field in fields[:4])
        for offset, value in enumerate(raw):
            byte_map[address + offset] = value

    missing = [a for a in range(start, start + size) if a not in byte_map]
    if missing:
        raise SystemExit(
            f"{path.name}: missing {len(missing)} target bytes; "
            f"first=0x{missing[0]:08x}"
        )
    return bytes(byte_map[a] for a in range(start, start + size))


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
            f"command failed: {' '.join(map(str, command))}\n{cp.stdout[-6000:]}"
        )


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

    expected = listing_bytes(LISTING, ADDRESS, SIZE)
    obj = BUILD / "SifIopReset.o"
    run([
        compiler, *rm.FLAGS, *include_flags, "-D" + DEFINE,
        "-c", SOURCE, "-o", obj,
    ])

    image, masks, normalized = rm.libgcc.text_image(obj)
    differing = rm.libgcc.differing_unmasked(expected, image, masks)
    normalized_target = rm.libgcc.normalize_target(expected, masks)
    object_ok = (
        len(image) == SIZE
        and differing == 0
        and normalized == normalized_target
        and len(masks) == 11
    )

    print("source: src/ps2/iopcontrol.c (historical source candidate)")
    print(f"lineage: ps2dev/ps2sdk@{rm.APR18} ee/kernel/src/iopcontrol.c")
    print(f"compiler: {machine} gcc {version}")
    print(
        f"{'MATCH' if object_ok else 'DIFF':5} SifIopReset.o "
        f"{DEFINE} bytes={len(image)}/{SIZE} relocs={len(masks)} diff={differing}"
    )
    if not object_ok:
        raise SystemExit("IOPCONTROL historical-source object gate: FAIL")

    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")
    script = BUILD / "SifIopReset.target.ld"
    linked = BUILD / "SifIopReset.target-linked.elf"
    linked_text = BUILD / "SifIopReset.target-linked.text.bin"

    script.write_text(
        """PROVIDE(SifStopDma = 0x0019f5d0);
PROVIDE(memset = 0x0019c39c);
PROVIDE(strlen = 0x0019c5e8);
PROVIDE(strncpy = 0x0019c550);
PROVIDE(SifSetDma = 0x0019cee0);
PROVIDE(SifSetReg = 0x0019cef0);
PROVIDE(SifGetReg = 0x0019cf00);
PROVIDE(SifWriteBackDCache = 0x0019cf10);

SECTIONS
{
  . = 0x0019d740;
  .text : { *(.text) }
  /DISCARD/ : {
    *(.data) *(.bss) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*)
  }
}
""",
        encoding="utf-8",
    )
    run([ld, "-T", script, obj, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, linked_text])
    raw = linked_text.read_bytes()

    if raw != expected:
        first = next(
            (i for i, (actual, want) in enumerate(zip(raw, expected)) if actual != want),
            None,
        )
        raise SystemExit(
            f"IOPCONTROL raw linked gate: FAIL bytes={len(raw)}/{len(expected)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )

    print("IOPCONTROL historical-source object gate: 1/1 MATCH")
    print(f"IOPCONTROL raw linked gate: MATCH {len(raw)}/{len(expected)} bytes")


if __name__ == "__main__":
    main()
