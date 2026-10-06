#!/usr/bin/env python3
"""Prove eleven historical memory-map and ROM metadata methods.

Full-TU historical fingerprints are frozen by the public code-window gate.
Isolation changes string/table/buffer addends, so compare only the 98 known
relocation fields while retaining all opcode and register bits. Four methods
also retain complete historical raw object hashes. This does not claim a new
complete provider-linked target-byte comparison or private image rerun.
"""
from __future__ import annotations

import csv
import hashlib
import json
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/memmap-methods-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
WINDOWS = ROOT / "analysis/link_identity/code_windows.json"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
    "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
    "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
SPECS = ((1390016,
  '_ZN7CMemory11FixROMSpeedEv',
  72,
  2,
  '57945116a897d8dcf0dd1d03d9a32851389cdcfec5f1b2d140e00da2b085b08f',
  '57945116a897d8dcf0dd1d03d9a32851389cdcfec5f1b2d140e00da2b085b08f',
  '82bb38839907dad3cf9edec8c037567db196d905724fa11cdf6fdfa4b0aa0edb',
  'tsv_001535c0__ZN7CMemory11FixROMSpeedEv'),
 (1390088,
  '_ZN7CMemory15WriteProtectROMEv',
  108,
  1,
  'ef10f882dd5d6ab9346abadb32d143dc2cc09066bd8e9ace03c855a0e4912bce',
  'ef10f882dd5d6ab9346abadb32d143dc2cc09066bd8e9ace03c855a0e4912bce',
  'ef10f882dd5d6ab9346abadb32d143dc2cc09066bd8e9ace03c855a0e4912bce',
  'tsv_00153608__ZN7CMemory15WriteProtectROMEv'),
 (1403012,
  '_ZN7CMemory11SPC7110SramEh',
  64,
  2,
  '0199ac7acab62356ef3a4f0c13a3928fda7579256fd1d54bfeba65c18020173f',
  '0199ac7acab62356ef3a4f0c13a3928fda7579256fd1d54bfeba65c18020173f',
  '0199ac7acab62356ef3a4f0c13a3928fda7579256fd1d54bfeba65c18020173f',
  'tsv_00156884__ZN7CMemory11SPC7110SramEh'),
 (1403076,
  '_ZN7CMemory10TVStandardEv',
  36,
  6,
  '1b763faebc2fa2aa1c6d681929d2697ea29793cee96cf20aa9598820d8bf3e61',
  '3da93ab33173831e5f59528bbc84b52df66ba8d9f5cb3b9891de036486e0f6b8',
  '544c3337ed1a896f297ed2e95dda0b70eb5f904e9a51e330eaf193f8ab473bce',
  'tsv_001568c4__ZN7CMemory10TVStandardEv'),
 (1403112,
  '_ZN7CMemory5SpeedEv',
  44,
  4,
  'ded906bef9d54969d103e3f1d76bd517e6917a84a83247ffdb256e6e897e7e09',
  'c1c636a3b8d81915ca93d4b6f59bbb394f8cb286e1e28d1b6dd10b35529a73d7',
  '6952fd5b4c932dadd5219163c35e0bdf67c9b7c625c84da299b691ebcf002b1d',
  'memory_speed'),
 (1403156,
  '_ZN7CMemory7MapTypeEv',
  32,
  4,
  '7434979cb95c11e15eabde28dc1a813e934b9eafdc79577d08e6eb97d45e2664',
  '7459a3616f02614d3b3334b929e740b07c9ecb5afc753d73378866995bd9f3bc',
  '86c58f191a4d6b8dc9384e65f2fae8770bbc346b5100f5195691f25ab7a54faa',
  'tsv_00156914__ZN7CMemory7MapTypeEv'),
 (1403188,
  '_ZN7CMemory13StaticRAMSizeEv',
  96,
  9,
  '7354fdc1528645073f3e5eb9f72f3a154a9f0e62673a1ef7cedd3bd6f8c4fce3',
  'bb2fb42f3b0d80f2b5b61660b08d4c8f23db142366646a2611ca8a9240913af0',
  '62a7f244a8587ff3e0424d93f09e6e1996849c8073262d62d6b957647ec363ad',
  'tsv_00156934__ZN7CMemory13StaticRAMSizeEv'),
 (1403284,
  '_ZN7CMemory4SizeEv',
  112,
  7,
  'a52ad1a49570f090b04d14d73bd1546f27d4e8b286d6b6f3a8bb168291c01e44',
  '7eec30f007f18907dcd52c1a92f018cd524f9031a8dc42a2d00ce9b9a067d211',
  '7178909a3cf51c22cc0e5d8a56c5b4c71c2e538f71766b378c755693a3ae6440',
  'tsv_00156994__ZN7CMemory4SizeEv'),
 (1403396,
  '_ZN7CMemory12KartContentsEv',
  520,
  58,
  'c2453e1d700eb24662c48737bfa2182be8982ba5b5f98d98fda6c6540d9a6083',
  '0b41794aa97dae7d8625a5652b33197bad29f610a9fe24d1ccde3c51f57e5ada',
  '1565d8ccfe8864bcd5d59ef3ef76f037c3227fe0ecbf9845957ed4d51e6d02f4',
  'tsv_00156a04__ZN7CMemory12KartContentsEv'),
 (1403916,
  '_ZN7CMemory7MapModeEv',
  72,
  5,
  '7e4a85fdbdb5bff6e779011cfee8adf7fe74a1cf4e6eaa95ee6d165c2b74e2b7',
  'bca703771ad8187144803b0ae411306c96a2c582955881c48adf991db69a18ea',
  '7e6c5c565a4c7f150530249d57e6f646f07ffc3547067dbc7676f99ce56e3cb7',
  'tsv_00156c0c__ZN7CMemory7MapModeEv'),
 (1403988,
  '_ZN7CMemory5ROMIDEv',
  12,
  0,
  '79a41ff2b9fa79947e43a6b96d6c09a0158705f99dbec432afd701063ac9ec6f',
  '79a41ff2b9fa79947e43a6b96d6c09a0158705f99dbec432afd701063ac9ec6f',
  '79a41ff2b9fa79947e43a6b96d6c09a0158705f99dbec432afd701063ac9ec6f',
  'memory_romid'))
def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command))
                         + "\n" + result.stdout[-12000:])
    return result.stdout


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def normalized(elf, symbol):
    raw = bytearray(elf.symbol_bytes(symbol, symbol.size))
    for relocation in elf.relocation_masks(symbol, symbol.size):
        if not relocation.known:
            raise SystemExit(f"unknown relocation: {symbol.name}")
        for index, mask in enumerate(relocation.mask_bytes):
            raw[relocation.start + index] &= 255 ^ mask
    return bytes(raw)


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "memmap_methods.o"
    run([CXX, *FLAGS, "-c", ROOT / "src/snes9x/memmap_methods.cpp", "-o", obj])
    elf = ELFFile(obj)
    functions = [s for s in elf.symbols if s.info & 15 == 2 and s.size]
    section = next(s for s in elf.sections if s.name == ".text")
    if ({s.name for s in functions} != {s[1] for s in SPECS}
            or len(functions) != 11 or section.size != 1168):
        raise SystemExit("unproved memory-map instruction bytes")
    selected = json.loads(WINDOWS.read_text())["result"]["selected_sources"]
    rows = []
    cursor = 0
    for address, name, size, relocations, historical_sha, isolated_sha, normalized_sha, witness in SPECS:
        matches = [s for s in selected if s.get("address") == address and s.get("symbol") == name]
        expected = {"name": witness, "size": size, "new_bytes": size,
                    "relocations": relocations, "raw_sha256": historical_sha}
        if len(matches) != 1 or any(matches[0].get(k) != v for k, v in expected.items()):
            raise SystemExit(f"frozen historical slice drift: {name}")
        symbol = elf.find_symbol(name)
        if (symbol.value != cursor or symbol.size != size
                or len(elf.relocation_ranges(symbol, size)) != relocations
                or digest(elf.symbol_bytes(symbol, size)) != isolated_sha
                or digest(normalized(elf, symbol)) != normalized_sha):
            raise SystemExit(f"historical memory-map code drift: {name}")
        rows.append({"address": f"0x{address:08x}", "symbol": name,
                     "source_file": "src/snes9x/memmap_methods.cpp", "size": str(size),
                     "relocations": str(relocations), "historical_raw_sha256": historical_sha,
                     "isolated_raw_sha256": isolated_sha, "normalized_sha256": normalized_sha,
                     "evidence": "analysis/link_identity/code_windows.json#" + witness})
        cursor += size
        print(f"memory-map normalized: MATCH {name} bytes={size}/{size} relocations={relocations}")
    with (ROOT / "analysis/functions/memmap_methods_exact_1168.tsv").open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen or cursor != 1168 or sum(int(r["relocations"]) for r in rows) != 98:
        raise SystemExit("memory-map evidence ledger drift")
    (BUILD / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
    print("memory-map source: MATCH functions=11/11 normalized bytes=1168/1168 relocations=98")


if __name__ == "__main__":
    main()
