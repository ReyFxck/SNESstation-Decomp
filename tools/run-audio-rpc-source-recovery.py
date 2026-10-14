#!/usr/bin/env python3
"""Rebuild the target-proven PGEN RPC suffixes and SNES Station variants.

The frozen code-window hashes reproduce the historical objects. Three small
SjPCM wrappers additionally have complete public target listings. Both log
functions are checked against committed instruction witnesses after resolving
their providers, including the historical 32-bit memcpy length ABI.
"""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile, Symbol

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/audio-rpc-source-recovery"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-gcc"
WINDOWS = ROOT / "analysis/link_identity/code_windows.json"
EVIDENCE = ROOT / "analysis/functions/audio_rpc_exact_2892.tsv"
LISTING = ROOT / "analysis/functions/progress11_short_targets.asm"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD",
    "-DCODE_PLATFORM=3", "-Iinclude", "-Iinclude/ee_stage1_compat",
)
# name, TU, actual source offset, address, bytes, relocations, historical hash
SLICES = (
    ("sjpcm_exact_suffix", "sjpcm", 268, 0x0010768C, 1264, 93,
     "11a86d45402fa2eb34bae5fcfd68c5859d52a8a6528a41d22291e50303f04e81"),
    ("amigamod_exact_suffix", "amigamod", 456, 0x00107D44, 576, 39,
     "f6dc17cdeba0606b04886f81e20f1e097f6b4d11eb93bf29d4cb75822022d72d"),
    ("tsv_00107b7c_amigaModInit", "amigamod", 0, 0x00107B7C, 232, 12,
     "9548581129fd897af2fc8e5a1e496207cbf3af1d8d61a1f614771afa8fbdb400"),
    ("tsv_00107c64_amigaModLoad", "amigamod", 232, 0x00107C64, 224, 18,
     "59278b471336cb04bf4f9b4b5f8599328bf40229f8305a8accd50164d62046ce"),
)
LINE = re.compile(r"^\s*([0-9a-fA-F]+):\s+"
                  r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+"
                  r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s")
LINKED_SYMBOLS = ("SjPCM_Play", "SjPCM_Pause", "SjPCM_Setvol")
LOG_WITNESS = ROOT / "matching/candidates/stage3p_code_residual_exact.S"
LOG_SPECS = (
    ("SjPCM_Puts", "sjpcm", "stage3p_sjpcm_puts_bridge_00107578",
     0x00107580, 268, 12, 8,
     "79a5e12c2ea51cf89b11efa08e4c088030058e746e4a58ee19d7b400f5490a82"),
    ("ModPuts", "amigamod", "stage3p_amigamod_log_tail_00107f84",
     0x00107F84, 328, 10, 0,
     "ac7c8b1b548b93e324f22e8735696136a610151adb72041da47da8ae35ca1a61"),
)


def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command))
                         + "\n" + result.stdout[-12000:])
    return result.stdout


def main():
    if run([CC, "-dumpmachine"]).strip() != "ee" or run([CC, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    BUILD.mkdir(parents=True, exist_ok=True)
    objects = {}
    for name in ("sjpcm", "amigamod"):
        obj = BUILD / f"{name}.o"
        run([CC, *FLAGS, "-c", ROOT / f"src/ps2/{name}_rpc.c", "-o", obj])
        objects[name] = ELFFile(obj)
    selected = json.loads(WINDOWS.read_text())["result"]["selected_sources"]
    report = []
    rows = []
    for name, unit, offset, address, size, relocations, digest in SLICES:
        matches = [s for s in selected if s.get("name") == name]
        expected = {"address": address, "size": size, "new_bytes": size,
                    "relocations": relocations, "raw_sha256": digest}
        if len(matches) != 1 or any(matches[0].get(k) != v for k, v in expected.items()):
            raise SystemExit(f"frozen target-proven slice drift: {name}")
        elf = objects[unit]
        section = next(s for s in elf.sections if s.name == ".text")
        span = Symbol(name, offset, size, section.index, 2)
        raw = elf.symbol_bytes(span, size)
        if (len(elf.relocation_ranges(span, size)) != relocations
                or hashlib.sha256(raw).hexdigest() != digest
                or any(not r.known for r in elf.relocation_masks(span, size))):
            raise SystemExit(f"historical object bytes/relocations drift: {name}")
        functions = sorted((s for s in elf.symbols if s.info & 15 == 2 and s.size
                            and offset <= s.value < offset + size), key=lambda s: s.value)
        cursor = offset
        for symbol in functions:
            if symbol.value != cursor or symbol.value + symbol.size > offset + size:
                raise SystemExit(f"incomplete function boundaries: {name}/{symbol.name}")
            rows.append({"address": f"0x{address + symbol.value - offset:08x}",
                         "symbol": symbol.name, "source_file": f"src/ps2/{unit}_rpc.c",
                         "size": str(symbol.size),
                         "relocations": str(len(elf.relocation_ranges(symbol, symbol.size))),
                         "raw_sha256": hashlib.sha256(elf.symbol_bytes(symbol, symbol.size)).hexdigest(),
                         "evidence": "analysis/link_identity/code_windows.json#" + name})
            cursor += symbol.size
        if cursor != offset + size:
            raise SystemExit(f"function coverage drift: {name}")
        report.append({"slice": name, **expected, "historical_object_match": True,
                       "functions": len(functions)})
        print(f"audio RPC historical slice: MATCH {name} bytes={size}/{size} functions={len(functions)}")
    script = BUILD / "sjpcm.target.ld"
    script.write_text("""PROVIDE(SifCallRpc = 0x0019c7b0);
PROVIDE(SifBindRpc = 0x0019c688);
PROVIDE(FlushCache = 0x0019ceb0);
PROVIDE(SifSetDma = 0x0019cee0);
PROVIDE(SifDmaStat = 0x0019ced0);
PROVIDE(vsnprintf = 0x0019e2e0);
PROVIDE(memcpy = 0x0019c364);
SECTIONS {
  .text 0x00107580 : { *(.text) }
  .data 0x001f6100 : { *(.data) }
  .bss 0x0042b400 : { *(.bss) }
  /DISCARD/ : { *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment)
               *(.note*) *(.eh_frame*) *(.rodata*) }
}
""")
    linked = BUILD / "sjpcm.target.elf"
    run([CC.with_name("ee-ld"), "-EL", "-T", script, BUILD / "sjpcm.o", "-o", linked])
    final = ELFFile(linked)
    words = {}
    for line in LISTING.read_text().splitlines():
        match = LINE.match(line)
        if match:
            address = int(match[1], 16)
            if address in words:
                raise SystemExit(f"duplicate target instruction: {address:#x}")
            words[address] = bytes(int(match[i], 16) for i in range(2, 6))
    for name in LINKED_SYMBOLS:
        row = next(r for r in rows if r["symbol"] == name)
        address, size = int(row["address"], 16), int(row["size"])
        try:
            target = b"".join(words[a] for a in range(address, address + size, 4))
        except KeyError as missing:
            raise SystemExit(f"incomplete target wrapper listing: {missing}")
        symbol = final.find_symbol(name)
        actual = final.symbol_bytes(symbol, symbol.size)
        if symbol.value != address or actual != target:
            raise SystemExit(f"provider-linked wrapper mismatch: {name}")
        report.append({"symbol": name, "address": address, "size": size,
                       "provider_linked_match": True,
                       "linked_sha256": hashlib.sha256(actual).hexdigest()})
        print(f"audio RPC provider-linked: MATCH {name} bytes={size}/{size}")

    script = BUILD / "amigamod.target.ld"
    script.write_text("""PROVIDE(SifCallRpc = 0x0019c7b0);
PROVIDE(SifBindRpc = 0x0019c688);
PROVIDE(SifSetDma = 0x0019cee0);
PROVIDE(SifDmaStat = 0x0019ced0);
PROVIDE(SifAllocIopHeap = 0x0019d63c);
PROVIDE(SifFreeIopHeap = 0x0019d6b8);
SECTIONS {
  .text 0x00107b7c : { *(.text) }
  .data 0x001fafc0 : { *(.data) }
  .bss 0x0042c540 : { *(.bss) }
  .rodata 0x001b1500 : { *(.rodata*) }
  /DISCARD/ : { *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment)
               *(.note*) *(.eh_frame*) }
}
""")
    amiga_linked = BUILD / "amigamod.target.elf"
    run([CC.with_name("ee-ld"), "-EL", "-T", script, BUILD / "amigamod.o", "-o", amiga_linked])
    linked_objects = {"sjpcm": final, "amigamod": ELFFile(amiga_linked)}
    witness = LOG_WITNESS.read_text()
    for name, unit, label, address, size, relocations, prefix, witness_sha in LOG_SPECS:
        match = re.search(r"(?ms)^" + re.escape(label) + r":\n(.*?)^\s*\.size "
                          + re.escape(label) + r",", witness)
        if not match:
            raise SystemExit(f"missing log witness: {label}")
        span = b"".join(int(word, 16).to_bytes(4, "little")
                        for word in re.findall(r"^\s*\.word (0x[0-9a-fA-F]+)", match[1], re.M))
        slices = [s for s in selected if s.get("symbol") == label]
        if (len(span) != prefix + size or hashlib.sha256(span).hexdigest() != witness_sha
                or len(slices) != 1 or slices[0].get("size") != len(span)
                or slices[0].get("raw_sha256") != witness_sha
                or slices[0].get("address") != address - prefix):
            raise SystemExit(f"frozen log witness drift: {label}")
        elf = objects[unit]
        symbol = elf.find_symbol(name)
        actual_elf = linked_objects[unit]
        actual_symbol = actual_elf.find_symbol(name)
        actual = actual_elf.symbol_bytes(actual_symbol, actual_symbol.size)
        if (symbol.size != size or len(elf.relocation_ranges(symbol, size)) != relocations
                or any(not r.known for r in elf.relocation_masks(symbol, size))
                or actual_symbol.value != address or actual != span[prefix:]):
            raise SystemExit(f"provider-linked log mismatch: {name}")
        rows.append({"address": f"0x{address:08x}", "symbol": name,
                     "source_file": f"src/ps2/{unit}_rpc.c", "size": str(size),
                     "relocations": str(relocations),
                     "raw_sha256": hashlib.sha256(elf.symbol_bytes(symbol, size)).hexdigest(),
                     "evidence": "matching/candidates/stage3p_code_residual_exact.S#" + label})
        report.append({"symbol": name, "address": address, "size": size,
                       "provider_linked_match": True,
                       "linked_sha256": hashlib.sha256(actual).hexdigest()})
        print(f"audio RPC provider-linked: MATCH {name} bytes={size}/{size}")
    rows.sort(key=lambda r: int(r["address"], 16))
    with EVIDENCE.open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen or len(rows) != 18:
        raise SystemExit("audio RPC evidence ledger drift")
    (BUILD / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print("audio RPC source: MATCH functions=18/18 bytes=2892/2892; provider-linked bodies=5/5 bytes=872/872")


if __name__ == "__main__":
    main()
