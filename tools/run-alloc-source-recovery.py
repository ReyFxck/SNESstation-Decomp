#!/usr/bin/env python3
"""Historical-source gate for SNES Station's old PS2LIB alloc.c."""
from __future__ import annotations

import hashlib
import re
import subprocess
from pathlib import Path
from types import SimpleNamespace

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "alloc-source-recovery"
TARGET_ASM = ROOT / "analysis" / "functions" / "libkernel_heap_0019e474.asm"
TARGET_BASE = 0x0019E474
TARGET_END = 0x0019E860

SPECS = (
    ("malloc.o", "F_malloc", 0x0019E474, 0x1D4),
    ("calloc.o", "F_calloc", 0x0019E648, 0x50),
    ("memalign.o", "F_memalign", 0x0019E698, 0xEC),
    ("free.o", "F_free", 0x0019E784, 0xDC),
)


def target_bytes() -> bytes:
    byte_map: dict[int, int] = {}
    for line in TARGET_ASM.read_text(encoding="utf-8", errors="replace").splitlines():
        stripped = line.strip()
        if ":" not in stripped:
            continue
        address_text, rest = stripped.split(":", 1)
        if re.fullmatch(r"[0-9A-Fa-f]+", address_text) is None:
            continue
        fields = rest.split()
        if len(fields) < 4:
            continue
        if any(re.fullmatch(r"[0-9A-Fa-f]{2}", x) is None for x in fields[:4]):
            continue
        address = int(address_text, 16)
        raw = bytes(int(x, 16) for x in fields[:4])
        for i, value in enumerate(raw):
            byte_map[address + i] = value

    missing = [a for a in range(TARGET_BASE, TARGET_END) if a not in byte_map]
    if missing:
        raise SystemExit(
            f"ALLOC target corridor has {len(missing)} missing bytes; "
            f"first=0x{missing[0]:08x}"
        )
    return bytes(byte_map[a] for a in range(TARGET_BASE, TARGET_END))


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


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    compiler = rm.libgcc.resolve_tool(
        str(
            ROOT
            / "build"
            / "toolchains"
            / "ee-gcc-3.2.2-stage1"
            / "prefix"
            / "bin"
            / "ee-gcc"
        )
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

    source = args.build_dir / "inputs" / rm.APR15 / "ee/libc/src/alloc.c"
    target = target_bytes()

    print(f"source: ps2dev/ps2sdk@{rm.APR15} ee/libc/src/alloc.c")
    print(f"compiler: {machine} gcc {version}")
    print(f"target corridor: 0x{TARGET_BASE:08x}..0x{TARGET_END - 1:08x}")
    print()

    objects: list[Path] = []
    failed = False
    for name, define, address, expected_size in SPECS:
        obj = BUILD / name
        run([
            compiler,
            *rm.FLAGS,
            *include_flags,
            "-D" + define,
            "-c",
            source,
            "-o",
            obj,
        ])
        objects.append(obj)
        image, masks, normalized = rm.libgcc.text_image(obj)
        start = address - TARGET_BASE
        expected = target[start : start + expected_size]
        differing = rm.libgcc.differing_unmasked(expected, image, masks)
        ok = (
            len(image) == expected_size
            and differing == 0
            and rm.libgcc.normalize_target(expected, masks) == normalized
        )
        print(
            f"{'MATCH' if ok else 'DIFF':5} {name:14} {define:12} "
            f"bytes={len(image)}/{expected_size} relocs={len(masks)} diff={differing}"
        )
        failed |= not ok

    if failed:
        raise SystemExit("ALLOC historical-source object gate: FAIL")

    print()
    print("ALLOC historical-source object gate: 4/4 MATCH")

    # Resolve the small external ABI so the linked .text can be compared raw
    # against the exact target corridor.
    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")
    linker = BUILD / "alloc_target.ld"
    linked = BUILD / "alloc.target-linked.elf"
    linked_text = BUILD / "alloc.target-linked.text.bin"
    linker.write_text(
        """ENTRY(_heap_mem_fit)

PROVIDE(ps2_sbrk = 0x0019f078);
PROVIDE(memset = 0x0019c39c);

SECTIONS
{
  . = 0x0019e474;
  .text : { *(.text) }

  /* F_malloc owns the three allocator globals in this exact target order. */
  . = 0x00425a74;
  .alloc_globals (NOLOAD) :
  {
    *(.data)
    *(.sdata)
    *(.bss)
    *(.sbss)
    *(COMMON)
  }

  /DISCARD/ : { *(.comment) *(.mdebug*) *(.pdr) }
}
""",
        encoding="utf-8",
    )
    run([ld, "-T", linker, *objects, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, linked_text])
    raw = linked_text.read_bytes()

    if raw != target:
        limit = min(len(raw), len(target))
        first = next((i for i in range(limit) if raw[i] != target[i]), None)
        if first is None and len(raw) != len(target):
            first = limit
        raise SystemExit(
            f"ALLOC raw linked gate: FAIL bytes={len(raw)}/{len(target)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )

    print(f"ALLOC raw linked gate: MATCH {len(raw)}/{len(target)} bytes")
    print(f"raw sha256: {hashlib.sha256(raw).hexdigest()}")


if __name__ == "__main__":
    main()
