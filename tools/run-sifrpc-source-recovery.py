#!/usr/bin/env python3
"""Focused historical-source proof for the SNES Station SIF RPC corridor."""
from __future__ import annotations

import re
import subprocess
from pathlib import Path
from types import SimpleNamespace

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "sifrpc-source-recovery"
BASE = 0x0019C688
END = 0x0019CE5C
LISTING = ROOT / "analysis" / "functions" / "sifrpc_0019c688.asm"

SPECS = (
    ("SifBindRpc.o", "F_SifBindRpc", 0x0019C688, 0x128),
    ("SifCallRpc.o", "F_SifCallRpc", 0x0019C7B0, 0x1B0),
    ("SifRpcMain.o", "F_SifRpcMain", 0x0019C960, 0x410),
    ("_rpc_get_packet.o", "F__rpc_get_packet", 0x0019CD70, 0xBC),
    ("_rpc_get_fpacket.o", "F__rpc_get_fpacket", 0x0019CE2C, 0x30),
)

INSN = re.compile(
    r"^\s*([0-9A-Fa-f]+):\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
)


def target_bytes() -> bytes:
    byte_map: dict[int, int] = {}
    for line in LISTING.read_text(encoding="utf-8", errors="replace").splitlines():
        m = INSN.match(line)
        if not m:
            continue
        address = int(m.group(1), 16)
        raw = bytes(int(m.group(i), 16) for i in range(2, 6))
        for offset, value in enumerate(raw):
            byte_map[address + offset] = value

    missing = [address for address in range(BASE, END) if address not in byte_map]
    if missing:
        raise SystemExit(f"SIF RPC target listing has {len(missing)} missing bytes; first=0x{missing[0]:08x}")
    return bytes(byte_map[address] for address in range(BASE, END))


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

    include_flags: list[str] = []
    for revision, relative in rm.INCLUDE_DIRS:
        include_flags += ["-I", str(args.build_dir / "inputs" / revision / relative)]
    gcc_include = Path(
        subprocess.check_output([str(compiler), "-print-file-name=include"], text=True).strip()
    ).resolve()
    include_flags += ["-I", str(gcc_include)]

    source = args.build_dir / "inputs" / rm.APR18 / "ee/kernel/src/sifrpc.c"
    target = target_bytes()

    print(f"source: ps2dev/ps2sdk@{rm.APR18} ee/kernel/src/sifrpc.c")
    print(f"compiler: {machine} gcc {version}")
    print(f"target corridor checked here: 0x{BASE:08x}..0x{END - 1:08x}")
    print()

    failed = False
    for name, define, address, expected_size in SPECS:
        obj = BUILD / name
        command = [
            str(compiler), *rm.FLAGS, *include_flags, "-D" + define,
            "-c", str(source), "-o", str(obj),
        ]
        run(command)

        image, masks, normalized = rm.libgcc.text_image(obj)
        expected = target[address - BASE: address - BASE + expected_size]
        differing = rm.libgcc.differing_unmasked(expected, image, masks)
        reloc_count = len(masks)
        ok = (
            len(image) == expected_size
            and differing == 0
            and rm.libgcc.normalize_target(expected, masks) == normalized
        )
        status = "MATCH" if ok else "DIFF"
        print(
            f"{status:5} {name:22} {define:20} "
            f"bytes={len(image)}/{expected_size} relocs={reloc_count} diff={differing}"
        )
        if not ok:
            failed = True

    if failed:
        raise SystemExit("SIF RPC historical-source gate: FAIL")
    print()
    print("SIF RPC historical-source gate: 5/5 MATCH")

    # Stronger gate: resolve the five historical objects at their target text,
    # data and BSS addresses, then require raw equality for the whole corridor.
    ld = compiler.with_name("ee-ld")
    objcopy = compiler.with_name("ee-objcopy")
    linker = BUILD / "sifrpc_target.ld"
    linked = BUILD / "sifrpc.target-linked.elf"
    linked_text = BUILD / "sifrpc.target-linked.text.bin"
    linker.write_text(
        """ENTRY(SifBindRpc)

PROVIDE(iWakeupThread = 0x0019ce60);
PROVIDE(CreateSema = 0x0019ce70);
PROVIDE(DeleteSema = 0x0019ce80);
PROVIDE(iSignalSema = 0x0019ce90);
PROVIDE(WaitSema = 0x0019cea0);
PROVIDE(SifSetReg = 0x0019cef0);
PROVIDE(SifGetReg = 0x0019cf00);
PROVIDE(SifWriteBackDCache = 0x0019cf10);
PROVIDE(DIntr = 0x0019f018);
PROVIDE(EIntr = 0x0019f060);
PROVIDE(SifSendCmd = 0x0019f264);
PROVIDE(iSifSendCmd = 0x0019f2a0);
PROVIDE(SifInitCmd = 0x0019f304);
PROVIDE(SifExitCmd = 0x0019f510);
PROVIDE(SifAddCmdHandler = 0x0019f544);
PROVIDE(SifGetSreg = 0x0019f57c);

SECTIONS
{
  . = 0x0019c688;
  .text : { *(.text) }

  . = 0x00425a40;
  .data : { *(.data) }

  . = 0x00443940;
  .bss (NOLOAD) : { *(.bss) *(COMMON) }

  /DISCARD/ : {
    *(.comment)
    *(.mdebug*)
    *(.pdr)
  }
}
""",
        encoding="utf-8",
    )

    objects = [BUILD / spec[0] for spec in SPECS]
    run([ld, "-T", linker, *objects, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, linked_text])
    raw = linked_text.read_bytes()
    if raw != target:
        first = next((i for i, (a, b) in enumerate(zip(raw, target)) if a != b), None)
        raise SystemExit(
            f"SIF RPC raw linked gate: FAIL bytes={len(raw)}/{len(target)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )

    import hashlib
    digest = hashlib.sha256(raw).hexdigest()
    print(f"SIF RPC raw linked gate: MATCH {len(raw)}/{len(target)} bytes")
    print(f"raw sha256: {digest}")


if __name__ == "__main__":
    main()
