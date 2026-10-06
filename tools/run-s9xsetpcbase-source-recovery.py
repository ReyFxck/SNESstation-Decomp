#!/usr/bin/env python3
"""Prove isolated historical Snes9x 1.40 S9xSetPCBase against target listing."""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "src/snes9x/s9xsetpcbase.cpp"
BUILD = ROOT / "build/matching/s9xsetpcbase-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/matching/hunt500plus-v11-validated-4.tsv"
CODE_WINDOWS = ROOT / "analysis/link_identity/code_windows.json"
TARGET_LISTING = ROOT / "analysis/functions/core_getset_apumem_001ab3c0.asm"

SYMBOL = "_Z12S9xSetPCBasej"
ADDRESS = 0x001AC024
SIZE = 364
RELOCS = 29
HISTORICAL_RAW_SHA = "a49edabe9071590beacb8a0b54e0d0ed48fd848f41d166906165e3540c8149a8"
RODATA_ADDRESS = 0x001B1080

LINE_RE = re.compile(
    r"^\s*([0-9a-fA-F]+):\s+"
    r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+"
    r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s"
)


def run(cmd):
    cp = subprocess.run(
        [str(x) for x in cmd], cwd=ROOT, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    if cp.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-24000:])
    return cp.stdout


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def target_bytes_from_listing() -> bytes:
    words = {}
    for line in TARGET_LISTING.read_text(encoding="utf-8").splitlines():
        match = LINE_RE.match(line)
        if not match:
            continue
        address = int(match.group(1), 16)
        if ADDRESS <= address < ADDRESS + SIZE:
            words[address] = bytes(int(match.group(i), 16) for i in range(2, 6))
    expected = list(range(ADDRESS, ADDRESS + SIZE, 4))
    missing = [address for address in expected if address not in words]
    if missing:
        raise SystemExit("target listing incomplete: " + ", ".join(f"0x{x:08x}" for x in missing[:8]))
    target = b"".join(words[address] for address in expected)
    if len(target) != SIZE:
        raise SystemExit(f"target listing span size mismatch: {len(target)}/{SIZE}")
    return target


def validate_public_evidence() -> None:
    with EVIDENCE.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    matches = [
        row for row in rows
        if row["address"].lower() == "0x001ac024"
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
        or "snes9x-1.40" not in row["provenance"]
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
        or item.get("raw_sha256") != HISTORICAL_RAW_SHA
    ):
        raise SystemExit(f"frozen historical slice drift: {item}")


def main():
    validate_public_evidence()
    target = target_bytes_from_listing()
    target_sha = sha256_bytes(target)

    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical C++ compiler")

    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "s9xsetpcbase.o"
    linked = BUILD / "s9xsetpcbase.target.elf"
    linked_binary = BUILD / "s9xsetpcbase.target.bin"
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
        raise SystemExit(
            f"S9xSetPCBase size mismatch: {actual_size}/{SIZE}\n"
            + run([CXX.with_name("ee-objdump"), "-dr", obj])[-24000:]
        )

    readelf = CXX.with_name("ee-readelf")
    reltext = run([readelf, "-r", obj])
    lines = reltext.splitlines()
    start = next((i for i, line in enumerate(lines) if "Relocation section '.rel.text'" in line), -1)
    if start < 0:
        raise SystemExit("missing .rel.text relocation section")
    end = next((i for i in range(start + 1, len(lines)) if lines[i].startswith("Relocation section '")), len(lines))
    reloc_count = sum(1 for line in lines[start:end] if "R_MIPS_" in line)
    if reloc_count != RELOCS:
        raise SystemExit(f"S9xSetPCBase relocation count mismatch: {reloc_count}/{RELOCS}\n{reltext}")

    ld = CXX.with_name("ee-ld")
    objcopy = CXX.with_name("ee-objcopy")
    script = BUILD / "s9xsetpcbase.target.ld"
    script.write_text(
        """PROVIDE(g_p12_memory = 0x0034e2b0);
PROVIDE(g_CPU_blob = 0x00345340);
SECTIONS {
  . = 0x001ac024;
  .text : { *(.text) }
  . = 0x001b1080;
  .rodata : { *(.rodata*) }
  /DISCARD/ : {
    *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*)
  }
}
""", encoding="utf-8")
    run([ld, "-EL", "-T", script, obj, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, linked_binary])
    actual = linked_binary.read_bytes()
    if len(actual) != SIZE or actual != target:
        first = next((i for i, (a, b) in enumerate(zip(actual, target)) if a != b), min(len(actual), len(target)))
        lo, hi = max(0, first - 24), min(max(len(actual), len(target)), first + 48)
        raise SystemExit(
            f"S9xSetPCBase provider-linked target mismatch bytes={len(actual)}/{SIZE} "
            f"first_offset=0x{first:x} target_address=0x{ADDRESS + first:08x}\n"
            f"actual[{lo:#x}:{hi:#x}]={actual[lo:hi].hex()}\n"
            f"target[{lo:#x}:{hi:#x}]={target[lo:hi].hex()}\n"
            f"actual_sha256={sha256_bytes(actual)}\n"
            f"target_sha256={target_sha}\n"
            + run([CXX.with_name("ee-objdump"), "-dr", linked])[-26000:]
        )

    print(f"S9XSETPCBASE isolated-source: MATCH bytes={SIZE}/{SIZE} relocations={RELOCS}/{RELOCS}; historical_raw_sha256={HISTORICAL_RAW_SHA}")
    print(f"S9XSETPCBASE provider-linked: MATCH bytes={SIZE}/{SIZE} sha256={target_sha} target=0x{ADDRESS:08x}")
    print("S9XSETPCBASE proof chain: exact Snes9x 1.40 body + EE GCC 3.2.2 == target listing byte-for-byte")


if __name__ == "__main__":
    main()
