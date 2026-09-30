#!/usr/bin/env python3
"""Exact historical-source gate for SNES Station's original PS2LIB F_strstr member."""
from __future__ import annotations

import hashlib
import re
import subprocess
from pathlib import Path
from types import SimpleNamespace

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "strstr-source-recovery"
SOURCE = ROOT / "src" / "ps2" / "strstr.c"
LISTING = ROOT / "analysis" / "functions" / "libc_text_0019e860.asm"
ADDRESS = 0x0019EAF8
SIZE = 0x88
DEFINE = "F_strstr"


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

    original_source = args.build_dir / "inputs" / rm.APR15 / "ee/libc/src/string.c"
    if hashlib.sha256(original_source.read_bytes()).hexdigest() != "54ffd1b2845d412e30c934622bb57c8e85dfe99985e04a93bd634c9c1a696095":
        raise SystemExit("historical ps2sdk string.c source hash drift")
    marker = "#ifdef F_strstr\\n".replace("\\n", "\n")
    original_body = original_source.read_text(encoding="utf-8").split(marker, 1)[1].split("#endif", 1)[0]
    recovered_body = SOURCE.read_text(encoding="utf-8").split(marker, 1)[1].split("#endif", 1)[0]
    if original_body != recovered_body:
        raise SystemExit("historical F_strstr source body drift")
    expected = listing_bytes(LISTING, ADDRESS, SIZE)
    if hashlib.sha256(expected).hexdigest() != "29dc825569d23603c48a4498cb4143ef205af4ae3b58621468aa356aa8806879":
        raise SystemExit("STRSTR original linked target listing hash drift")
    obj = BUILD / "strstr.o"
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
        and len(masks) == 2
    )

    print("source: src/ps2/strstr.c (historical source candidate)")
    print(f"lineage: ps2dev/ps2sdk@{rm.APR15} ee/libc/src/string.c")
    print(f"compiler: {machine} gcc {version}")
    print(
        f"{'MATCH' if object_ok else 'DIFF':5} strstr.o "
        f"{DEFINE} bytes={len(image)}/{SIZE} relocs={len(masks)} diff={differing}"
    )
    if not object_ok:
        raise SystemExit("STRSTR historical-source object gate: FAIL")

    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")
    script = BUILD / "strstr.target.ld"
    linked = BUILD / "strstr.target-linked.elf"
    linked_text = BUILD / "strstr.target-linked.text.bin"

    script.write_text(
        """PROVIDE(strlen = 0x0019c5e8);
PROVIDE(strncmp = 0x0019c410);
SECTIONS
{
  . = 0x0019eaf8;
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
            f"STRSTR raw linked gate: FAIL bytes={len(raw)}/{len(expected)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )

    print("STRSTR historical-source object gate: 1/1 MATCH")
    print(f"STRSTR raw linked gate: MATCH {len(raw)}/{len(expected)} bytes")
    print('raw sha256:', hashlib.sha256(raw).hexdigest())


if __name__ == "__main__":
    main()
