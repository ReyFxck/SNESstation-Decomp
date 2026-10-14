#!/usr/bin/env python3
"""Prove canonical C4DrawWireFrame source against the target-proved V77 object."""
from __future__ import annotations

import csv
import hashlib
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

from compare_elf_functions import ELFFile  # noqa: E402
import hunt1000plus_v47_closure as v47  # noqa: E402
import hunt1041_v77_c4draw as v77  # noqa: E402

LOCAL = ROOT / "src/snes9x/c4drawwireframe.cpp"
BUILD = ROOT / "build/matching/c4drawwireframe-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/matching/hunt1041-v77-validated-c4draw-1.tsv"

SYMBOL = "_Z15C4DrawWireFramev"
SIZE = 472
RELOCS = 15
TARGET_SHA = "80ff14ccbe949105572ee52bb8f3cc4749586645b9965e3b030cd022c59ef77f"


def run(cmd):
    cp = subprocess.run([str(x) for x in cmd], cwd=ROOT, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-12000:])
    return cp.stdout


def masks(elf: ELFFile, symbol):
    return elf.relocation_masks(symbol, 4)


def normalized_diff(left: bytes, right: bytes, relocation_masks) -> list[int]:
    ignored = bytearray(len(left))
    for item in relocation_masks:
        for i, mask in enumerate(item.mask_bytes):
            off = item.start + i
            if off < len(ignored):
                ignored[off] |= mask
    return [
        i for i, (a, b) in enumerate(zip(left, right))
        if ((a ^ b) & (~ignored[i] & 0xff)) != 0
    ]


def mask_signature(items):
    return [
        (x.start, x.end, x.relocation_type, bytes(x.mask_bytes), x.known)
        for x in items
    ]


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical C++ compiler")

    with EVIDENCE.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    if len(rows) != 1:
        raise SystemExit("V77 evidence cardinality drift")
    row = rows[0]
    checks = {
        "historical_identity": "C4DrawWireFrame",
        "object_size": str(SIZE),
        "result": "MATCH",
        "differing_bytes": "0",
        "normalized_equal": "True",
        "relocation_count": str(RELOCS),
        "target_span_sha256": TARGET_SHA,
    }
    for key, expected in checks.items():
        if row.get(key) != expected:
            raise SystemExit(f"V77 frozen evidence drift: {key}={row.get(key)!r}")

    historical = v77.build_object(CXX)
    hist_elf = ELFFile(historical.path)
    hist_sym = hist_elf.find_symbol(SYMBOL)
    hist_raw = hist_elf.symbol_bytes(hist_sym, SIZE)
    hist_masks = masks(hist_elf, hist_sym)

    BUILD.mkdir(parents=True, exist_ok=True)
    local_obj = BUILD / "c4drawwireframe.o"
    flags = [
        *[flag for flag in v47.COMMON_FLAGS if flag != "-fshort-double"],
        "-Os", "-fno-builtin", *v47.SNES_DEFINES, "-x", "c++",
    ]
    run([CXX, *flags, "-c", LOCAL, "-o", local_obj])

    local_elf = ELFFile(local_obj)
    local_sym = local_elf.find_symbol(SYMBOL)
    local_raw = local_elf.symbol_bytes(local_sym, SIZE)
    local_masks = masks(local_elf, local_sym)

    if hist_sym.size != SIZE or local_sym.size != SIZE:
        raise SystemExit(f"size drift historical={hist_sym.size} local={local_sym.size}")
    if len(hist_masks) != RELOCS or len(local_masks) != RELOCS:
        raise SystemExit(f"relocation count drift historical={len(hist_masks)} local={len(local_masks)}")
    if mask_signature(hist_masks) != mask_signature(local_masks):
        raise SystemExit(
            "relocation layout drift\n"
            f"historical={mask_signature(hist_masks)}\nlocal={mask_signature(local_masks)}"
        )

    diff = normalized_diff(hist_raw, local_raw, hist_masks)
    if diff:
        raise SystemExit(
            f"C4DrawWireFrame standalone differs outside relocations: "
            f"{len(diff)} bytes; first=" + ",".join(f"+0x{x:x}" for x in diff[:16])
        )

    # Resolve every live relocation to the target-proved address and hash the
    # final 472-byte .text image. This turns the normalized object proof into a
    # literal byte-for-byte target-span proof without publishing the private ELF.
    ld = CXX.with_name("ee-ld")
    objcopy = CXX.with_name("ee-objcopy")
    script = BUILD / "c4drawwireframe.target.ld"
    linked = BUILD / "c4drawwireframe.target.elf"
    linked_text = BUILD / "c4drawwireframe.target.bin"
    script.write_text(
        """PROVIDE(g_p12_memory = 0x0034e2b0);
PROVIDE(_Z16S9xGetMemPointerj = 0x001ab4e8);
PROVIDE(_Z10C4DrawLineiisiish = 0x0010cbb0);
SECTIONS {
  . = 0x0010cdcc;
  .text : { *(.text) }
  /DISCARD/ : {
    *(.data) *(.rodata*) *(.bss) *(COMMON) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*)
  }
}
""",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", script, local_obj, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, linked_text])
    raw_linked = linked_text.read_bytes()
    raw_sha = hashlib.sha256(raw_linked).hexdigest()
    if len(raw_linked) != SIZE or raw_sha != TARGET_SHA:
        raise SystemExit(
            f"C4DrawWireFrame raw-linked target mismatch "
            f"bytes={len(raw_linked)}/{SIZE} sha256={raw_sha}"
        )

    print(
        f"C4DRAWWIREFRAME historical-chain: MATCH bytes={SIZE}/{SIZE} "
        f"relocations={RELOCS} target_span_sha256={TARGET_SHA}"
    )
    print("chain: canonical src C++ == V77 historical object == target ELF (relocation-normalized)")
    print(
        f"C4DRAWWIREFRAME raw-linked: MATCH bytes={SIZE}/{SIZE} "
        f"sha256={raw_sha} target=0x0010cdcc"
    )


if __name__ == "__main__":
    main()
