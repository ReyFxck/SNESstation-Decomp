#!/usr/bin/env python3
"""Historical-source gate for SNES Station's old EE loadfile.c."""
from __future__ import annotations

import subprocess
from pathlib import Path
from types import SimpleNamespace

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "loadfile-source-recovery"
LISTINGS = (
    ROOT / "analysis" / "functions" / "loadfile_iop_0019d600.asm",
    ROOT / "analysis" / "functions" / "libkernel_client_init_0019f5d0.asm",
)
ZERO_RANGES = ROOT / "analysis" / "matching" / "hunt400-inferred-zero-ranges.tsv"

SPECS = (
    ("SifLoadModule.o", "F_SifLoadModule", 0x0019D600, 0x20),
    ("SifLoadModuleBuffer.o", "F_SifLoadModuleBuffer", 0x0019D620, 0x1C),
    ("_SifLoadModule.o", "F__SifLoadModule", 0x0019F7E8, 0x10C),
    ("_SifLoadModuleBuffer.o", "F__SifLoadModuleBuffer", 0x0019F8F4, 0xF4),
    ("SifLoadFileInit.o", "F_SifLoadFileInit", 0x0019FD20, 0xBC),
)


def target_bytes() -> dict[int, int]:
    out: dict[int, int] = {}
    for path in LISTINGS:
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            stripped = line.strip()
            if ":" not in stripped:
                continue
            address_text, rest = stripped.split(":", 1)
            fields = rest.split()
            try:
                address = int(address_text, 16)
            except ValueError:
                continue
            if len(fields) < 4:
                continue
            try:
                raw = bytes(int(field, 16) for field in fields[:4])
            except ValueError:
                continue
            for offset, value in enumerate(raw):
                out[address + offset] = value
    for line in ZERO_RANGES.read_text(encoding="utf-8").splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        fields = line.split("\t")
        if len(fields) < 4:
            continue
        start = int(fields[0], 16)
        end = int(fields[1], 16)
        source = fields[3]
        if source != "analysis/functions/libkernel_client_init_0019f5d0.asm":
            continue
        for address in range(start, end):
            out.setdefault(address, 0)
    return out


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

    source = args.build_dir / "inputs" / rm.APR18 / "ee/kernel/src/loadfile.c"
    target = target_bytes()

    print(f"source: ps2dev/ps2sdk@{rm.APR18} ee/kernel/src/loadfile.c")
    print(f"compiler: {machine} gcc {version}")
    print()

    failed = False
    for name, define, address, expected_size in SPECS:
        missing = [a for a in range(address, address + expected_size) if a not in target]
        if missing:
            raise SystemExit(
                f"target listing missing {len(missing)} bytes for {name}; "
                f"first=0x{missing[0]:08x}"
            )
        expected = bytes(target[a] for a in range(address, address + expected_size))

        obj = BUILD / name
        cp = subprocess.run(
            [
                str(compiler), *rm.FLAGS, *include_flags, "-D" + define,
                "-c", str(source), "-o", str(obj),
            ],
            cwd=ROOT, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        )
        if cp.returncode:
            raise SystemExit(f"compile failed for {name}\n{cp.stdout[-6000:]}")

        image, masks, normalized = rm.libgcc.text_image(obj)
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
        raise SystemExit("LOADFILE historical-source object gate: FAIL")
    print()
    print("LOADFILE historical-source object gate: 5/5 MATCH")


if __name__ == "__main__":
    main()
