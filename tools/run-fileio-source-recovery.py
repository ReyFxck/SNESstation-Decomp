#!/usr/bin/env python3
"""Historical-source gate for SNES Station's old EE fileio.c."""
from __future__ import annotations

import csv
import hashlib
import re
import subprocess
from pathlib import Path
from types import SimpleNamespace

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "fileio-source-recovery"
CORRIDOR_BASE = 0x0019CFC0
CORRIDOR_END = 0x0019D600
CORRIDOR_ASM = ROOT / "analysis" / "functions" / "fileio_0019cfc0.asm"
RUNTIME_OBJECTS = ROOT / "analysis" / "link_identity" / "runtime_member_objects.tsv"

SPECS = (
    ("fio_open.o", "F_fio_open", 0x0019CFC0, 0xD0),
    ("fio_close.o", "F_fio_close", 0x0019D090, 0x90),
    ("fio_read.o", "F_fio_read", 0x0019D120, 0x124),
    ("fio_write.o", "F_fio_write", 0x0019D244, 0x11C),
    ("fio_lseek.o", "F_fio_lseek", 0x0019D360, 0xB0),
    ("fio_mkdir.o", "F_fio_mkdir", 0x0019D410, 0xA0),
    ("_fio_read_intr.o", "F__fio_read_intr", 0x0019D4B0, 0x84),
    ("fio_putc.o", "F_fio_putc", 0x0019D534, 0x24),
    ("fio_gets.o", "F_fio_gets", 0x0019D558, 0xA8),
)
MAIN = ("fio_main.o", "F_fio_main", 0x0019F600, 0x1E8)

INSN = re.compile(
    r"^\s*([0-9A-Fa-f]+):\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
)


def target_corridor() -> bytes:
    byte_map: dict[int, int] = {}
    for line in CORRIDOR_ASM.read_text(encoding="utf-8", errors="replace").splitlines():
        m = INSN.match(line)
        if not m:
            continue
        address = int(m.group(1), 16)
        raw = bytes(int(m.group(i), 16) for i in range(2, 6))
        for offset, value in enumerate(raw):
            byte_map[address + offset] = value
    missing = [a for a in range(CORRIDOR_BASE, CORRIDOR_END) if a not in byte_map]
    if missing:
        raise SystemExit(
            f"fileio target corridor has {len(missing)} missing bytes; "
            f"first=0x{missing[0]:08x}"
        )
    return bytes(byte_map[a] for a in range(CORRIDOR_BASE, CORRIDOR_END))


def runtime_object_row(member: str) -> dict[str, str]:
    with RUNTIME_OBJECTS.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    matches = [row for row in rows if row["member"] == member]
    if len(matches) != 1:
        raise SystemExit(f"expected one frozen runtime row for {member}, found {len(matches)}")
    return matches[0]


def run(command: list[str | Path]) -> None:
    cp = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit(f"command failed: {' '.join(map(str, command))}\n{cp.stdout[-6000:]}")


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

    include_flags: list[str] = ["-I", str(ROOT / "include")]
    for revision, relative in rm.INCLUDE_DIRS:
        include_flags += ["-I", str(args.build_dir / "inputs" / revision / relative)]
    gcc_include = Path(
        subprocess.check_output([str(compiler), "-print-file-name=include"], text=True).strip()
    ).resolve()
    include_flags += ["-I", str(gcc_include)]

    source = ROOT / "src" / "ps2" / "fileio.c"

    target = target_corridor()

    print("source: src/ps2/fileio.c (promoted historical source)")
    print(f"compiler: {machine} gcc {version}")
    print(f"main target corridor: 0x{CORRIDOR_BASE:08x}..0x{CORRIDOR_END - 1:08x}")
    print()

    failed = False
    objects: list[Path] = []
    for name, define, address, expected_size in SPECS:
        obj = BUILD / name
        run([
            compiler, *rm.FLAGS, *include_flags, "-D" + define,
            "-c", source, "-o", obj,
        ])
        objects.append(obj)
        image, masks, normalized = rm.libgcc.text_image(obj)
        expected = target[address - CORRIDOR_BASE: address - CORRIDOR_BASE + expected_size]
        differing = rm.libgcc.differing_unmasked(expected, image, masks)
        ok = (
            len(image) == expected_size
            and differing == 0
            and rm.libgcc.normalize_target(expected, masks) == normalized
        )
        print(
            f"{'MATCH' if ok else 'DIFF':5} {name:20} {define:18} "
            f"bytes={len(image)}/{expected_size} relocs={len(masks)} diff={differing}"
        )
        if not ok and name == "fio_gets.o":
            objdump = compiler.with_name("ee-objdump")
            dis = subprocess.run(
                [str(objdump), "-dr", str(obj)],
                cwd=ROOT, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            )
            print("--- fio_gets candidate disassembly ---")
            print(dis.stdout)
            print("--- end fio_gets candidate disassembly ---")
        failed |= not ok

    main_name, main_define, _main_address, main_size = MAIN
    main_obj = BUILD / main_name
    run([
        compiler, *rm.FLAGS, *include_flags, "-D" + main_define,
        "-c", source, "-o", main_obj,
    ])
    image, masks, normalized = rm.libgcc.text_image(main_obj)
    frozen = runtime_object_row("kernel/fio_main.o")
    main_ok = (
        frozen["status"] == "MEMBER_TEXT_EXACT"
        and frozen["source_revision"] == rm.APR18
        and frozen["source"] == "ee/kernel/src/fileio.c"
        and frozen["define"] == main_define
        and int(frozen["text_size_hex"], 0) == main_size == len(image)
        and frozen["normalized_sha256"] == hashlib.sha256(normalized).hexdigest()
        and int(frozen["relocation_count"]) == len(masks)
    )
    print(
        f"{'MATCH' if main_ok else 'DIFF':5} {main_name:20} {main_define:18} "
        f"bytes={len(image)}/{main_size} relocs={len(masks)} frozen-target=MEMBER_TEXT_EXACT"
    )
    failed |= not main_ok

    if failed:
        raise SystemExit("FILEIO historical-source object gate: FAIL")
    print()
    print("FILEIO historical-source object gate: 10/10 MATCH")

    # Strong raw gate for the contiguous file-I/O wrapper corridor.  The nine
    # historical objects are linked in target order with the exact target
    # addresses of their external code/data dependencies.
    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")
    linker = BUILD / "fileio_target.ld"
    linked = BUILD / "fileio.target-linked.elf"
    linked_text = BUILD / "fileio.target-linked.text.bin"
    linker.write_text(
        """ENTRY(fioOpen)

PROVIDE(fioInit = 0x0019f600);
PROVIDE(_fio_intr = 0x0019f6e8);
PROVIDE(WaitSema = 0x0019cea0);
PROVIDE(iSignalSema = 0x0019ce90);
PROVIDE(SifCallRpc = 0x0019c7b0);
PROVIDE(SifWriteBackDCache = 0x0019cf10);
PROVIDE(memcpy = 0x0019c364);
PROVIDE(strncpy = 0x0019c550);

PROVIDE(_fio_init = 0x00425ab0);
PROVIDE(_fio_completion_sema = 0x00426e44);
PROVIDE(_fio_block_mode = 0x00426e48);
PROVIDE(_fio_cd = 0x00450310);
PROVIDE(_fio_intr_data = 0x00450340);
PROVIDE(_fio_recv_data = 0x004503c0);

SECTIONS
{
  . = 0x0019cfc0;
  .text : { *(.text) }
  /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.comment) *(.mdebug*) *(.pdr) }
}
""",
        encoding="utf-8",
    )
    run([ld, "-T", linker, *objects, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, linked_text])
    raw = linked_text.read_bytes()
    if raw != target:
        first = next((i for i, (a, b) in enumerate(zip(raw, target)) if a != b), None)
        raise SystemExit(
            f"FILEIO raw linked gate: FAIL bytes={len(raw)}/{len(target)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )

    print(f"FILEIO raw linked gate: MATCH {len(raw)}/{len(target)} bytes")
    print(f"raw sha256: {hashlib.sha256(raw).hexdigest()}")


if __name__ == "__main__":
    main()
