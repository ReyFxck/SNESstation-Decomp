#!/usr/bin/env python3
"""Prove canonical historical Snes9x 1.40 S9xGetByte against target evidence."""
from __future__ import annotations

import csv
import hashlib
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "src/snes9x/s9xgetbyte.cpp"
BUILD = ROOT / "build/matching/s9xgetbyte-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/matching/hunt500plus-v11-validated-4.tsv"
CODE_WINDOWS = ROOT / "analysis/link_identity/code_windows.json"

SYMBOL = "_Z10S9xGetBytej"
ADDRESS = 0x001AB63C
SIZE = 708
RELOCS = 49
RAW_SHA = "0e803c2aaafecb8bfc19887f9177e909d011156393483cb46d247f148b6e31e4"
LINKED_SHA = "06f6009e7bbc5f4695525a78dba798ffb3af2ffd849e73d260d78fa11b9002af"


def run(cmd):
    cp = subprocess.run(
        [str(x) for x in cmd],
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    if cp.returncode:
        raise SystemExit(
            "command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-16000:]
        )
    return cp.stdout


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def validate_public_evidence() -> None:
    with EVIDENCE.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    matches = [
        row for row in rows
        if row["address"].lower() == "0x001ab63c"
        and row["object_symbol"] == SYMBOL
    ]
    if len(matches) != 1:
        raise SystemExit(f"missing/duplicate historical evidence row: {matches}")
    row = matches[0]
    if (
        row["object_size"] != str(SIZE)
        or row["result"] != "MATCH"
        or row["differing_bytes"] != "0"
        or row["normalized_equal"] != "True"
    ):
        raise SystemExit(f"historical target evidence drift: {row}")

    document = json.loads(CODE_WINDOWS.read_text(encoding="utf-8"))
    selected = [
        item for item in document["result"]["selected_sources"]
        if item.get("address") == ADDRESS and item.get("symbol") == SYMBOL
    ]
    if len(selected) != 1:
        raise SystemExit(f"missing/duplicate code-window slice: {selected}")
    item = selected[0]
    if (
        item.get("new_bytes") != SIZE
        or item.get("size") != SIZE
        or item.get("relocations") != RELOCS
        or item.get("raw_sha256") != RAW_SHA
    ):
        raise SystemExit(f"frozen historical slice drift: {item}")


def main():
    validate_public_evidence()

    if (
        run([CXX, "-dumpmachine"]).strip() != "ee"
        or run([CXX, "-dumpversion"]).strip() != "3.2.2"
    ):
        raise SystemExit("wrong historical C++ compiler")

    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "s9xgetbyte.o"
    raw_object = BUILD / "s9xgetbyte.raw-text.bin"
    linked = BUILD / "s9xgetbyte.target.elf"
    linked_binary = BUILD / "s9xgetbyte.target.bin"

    flags = [
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET", "-x", "c++",
    ]
    run([CXX, *flags, "-c", SOURCE, "-o", obj])

    nm = CXX.with_name("ee-nm")
    rows = run([nm, "-S", "--size-sort", obj]).splitlines()
    matches = [row for row in rows if row.rstrip().endswith(" " + SYMBOL)]
    if len(matches) != 1:
        raise SystemExit(f"missing/duplicate {SYMBOL}: {matches}")
    actual_size = int(matches[0].split()[1], 16)
    if actual_size != SIZE:
        raise SystemExit(f"S9xGetByte size mismatch: {actual_size}/{SIZE}")

    readelf = CXX.with_name("ee-readelf")
    reltext = run([readelf, "-r", obj])
    lines = reltext.splitlines()
    start = next(
        (i for i, line in enumerate(lines) if "Relocation section '.rel.text'" in line),
        -1,
    )
    if start < 0:
        raise SystemExit("missing .rel.text relocation section")
    end = next(
        (i for i in range(start + 1, len(lines)) if lines[i].startswith("Relocation section '")),
        len(lines),
    )
    reloc_count = sum(1 for line in lines[start:end] if "R_MIPS_" in line)
    if reloc_count != RELOCS:
        raise SystemExit(
            f"S9xGetByte relocation count mismatch: {reloc_count}/{RELOCS}\n{reltext}"
        )

    objcopy = CXX.with_name("ee-objcopy")
    run([objcopy, "-j", ".text", "-O", "binary", obj, raw_object])
    raw_sha = sha256(raw_object)
    if raw_object.stat().st_size != SIZE or raw_sha != RAW_SHA:
        raise SystemExit(
            f"S9xGetByte raw historical-slice mismatch "
            f"bytes={raw_object.stat().st_size}/{SIZE} "
            f"relocations={reloc_count}/{RELOCS} sha256={raw_sha} expected={RAW_SHA}"
        )

    ld = CXX.with_name("ee-ld")
    script = BUILD / "s9xgetbyte.target.ld"
    script.write_text(
        """PROVIDE(g_p12_memory = 0x0034e2b0);
PROVIDE(g_CPU_blob = 0x00345340);
PROVIDE(g_OpenBus_byte = 0x0035b768);
PROVIDE(S9xGetPPU = 0x0015a5f0);
PROVIDE(S9xGetCPU = 0x0015bc70);
PROVIDE(S9xGetDSP = 0x0012e704);
PROVIDE(S9xGetC4 = 0x0010c328);
PROVIDE(S9xGetSPC7110Byte = 0x0018255c);
PROVIDE(S9xGetSPC7110 = 0x001813f0);
PROVIDE(GetOBC1 = 0x00158b5c);
PROVIDE(S9xGetSetaDSP = 0x0016fc48);
PROVIDE(S9xGetST018 = 0x001701fc);
SECTIONS {
  . = 0x001ab63c;
  .text : { *(.text) }
  . = 0x001b1150;
  .rodata : { *(.rodata*) }
  /DISCARD/ : {
    *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*)
  }
}
""",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", script, obj, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, linked_binary])
    linked_sha = sha256(linked_binary)
    if linked_binary.stat().st_size != SIZE or linked_sha != LINKED_SHA:
        raise SystemExit(
            f"S9xGetByte provider-linked target mismatch "
            f"bytes={linked_binary.stat().st_size}/{SIZE} "
            f"sha256={linked_sha} expected={LINKED_SHA}"
        )

    print(
        f"S9XGETBYTE historical-slice: MATCH bytes={SIZE}/{SIZE} "
        f"relocations={RELOCS}/{RELOCS} sha256={raw_sha}"
    )
    print(
        f"S9XGETBYTE provider-linked: MATCH bytes={SIZE}/{SIZE} "
        f"sha256={linked_sha} target=0x{ADDRESS:08x}"
    )
    print(
        "S9XGETBYTE proof chain: canonical source == frozen historical slice "
        "== relocation-normalized target span; target providers linked at proved addresses"
    )


if __name__ == "__main__":
    main()
