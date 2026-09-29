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
        try:
            start = int(fields[0], 16)
            end = int(fields[1], 16)
        except ValueError:
            continue
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
    built: dict[str, Path] = {}
    expected_by_name: dict[str, bytes] = {}
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

        built[name] = obj
        expected_by_name[name] = expected
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

    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")

    common_provides = """
PROVIDE(_SifLoadModule = 0x0019f7e8);
PROVIDE(_SifLoadModuleBuffer = 0x0019f8f4);
PROVIDE(SifLoadFileInit = 0x0019fd20);
PROVIDE(SifCallRpc = 0x0019c7b0);
PROVIDE(SifInitRpc = 0x0019cc0c);
PROVIDE(SifBindRpc = 0x0019c688);
PROVIDE(memset = 0x0019c39c);
PROVIDE(memcpy = 0x0019c364);
PROVIDE(strncpy = 0x0019c550);
"""

    raw_failed = False
    for name, _define, address, _expected_size in SPECS:
        obj = built[name]
        expected = expected_by_name[name]
        stem = obj.stem
        script = BUILD / f"{stem}.target.ld"
        linked = BUILD / f"{stem}.target-linked.elf"
        linked_text = BUILD / f"{stem}.target-linked.text.bin"

        if name == "SifLoadFileInit.o":
            data_layout = """
  . = 0x00425ab8;
  .bss : { *(.bss) *(.sbss) }
  . = 0x00450bf0;
  .common : { *(COMMON) *(.scommon) }
"""
            extra_provides = """
PROVIDE(_lf_init = 0x00425ab8);
"""
        else:
            data_layout = ""
            extra_provides = """
PROVIDE(_lf_init = 0x00425ab8);
PROVIDE(_lf_cd = 0x00450bf0);
"""

        script.write_text(
            common_provides
            + extra_provides
            + f"""
SECTIONS
{{
  . = 0x{address:08x};
  .text : {{ *(.text) }}
{data_layout}
  /DISCARD/ : {{
    *(.data) *(.rodata*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*)
  }}
}}
""",
            encoding="utf-8",
        )

        cp = subprocess.run(
            [str(ld), "-T", str(script), str(obj), "-o", str(linked)],
            cwd=ROOT, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        )
        if cp.returncode:
            raise SystemExit(f"raw link failed for {name}\n{cp.stdout[-6000:]}")

        cp = subprocess.run(
            [str(objcopy), "-j", ".text", "-O", "binary", str(linked), str(linked_text)],
            cwd=ROOT, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        )
        if cp.returncode:
            raise SystemExit(f"objcopy failed for {name}\n{cp.stdout[-6000:]}")

        raw = linked_text.read_bytes()
        ok = raw == expected
        if ok:
            print(f"RAW   {name:26} bytes={len(raw)}/{len(expected)} MATCH")
        else:
            first = next(
                (i for i, (actual, want) in enumerate(zip(raw, expected)) if actual != want),
                None,
            )
            print(
                f"RAW   {name:26} bytes={len(raw)}/{len(expected)} DIFF "
                f"first={('-' if first is None else hex(first))}"
            )
            raw_failed = True

    if raw_failed:
        raise SystemExit("LOADFILE raw linked gate: FAIL")

    print()
    print("LOADFILE raw linked gate: 5/5 MATCH")


if __name__ == "__main__":
    main()
