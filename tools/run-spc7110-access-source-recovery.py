#!/usr/bin/env python3
"""Prove all three historical SPC7110 register/ROM access bodies.

The isolated source reproduces all 4596 historical instruction bytes. After
linking, the setter and direct ROM read match 2616 frozen/public target bytes;
the register reader matches all 1980 bytes of a freshly linked historical
reference with the recorded core/data layout. That reference comparison checks
every relocation value, including state-field and jump-table addends, without
claiming a new private-target digest for the register reader.
"""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/spc7110-access-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/spc7110_access_exact_4596.tsv"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900", "-Os",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES", "-DCPU_SHUTDOWN",
    "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE", "-DSPC700_C",
    "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET", "-ffunction-sections",
)
LINE = re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} ){3}[0-9a-fA-F]{2})\s")


SPECS = (('0x001813f0',
  'S9xGetSPC7110',
  'src/snes9x/spc7110_access.cpp',
  '1980',
  '121',
  '680ec1a6e959485a60a34d09dcd3e3f0480176a7ac803508df28b97430b4a3c8',
  'eb3071e5f1c807887175725b0efe553de869eeab8032c03f734c73f03173e518',
  'de7e4c0c5f45ad83e72100ac03a1f6c5ab14e02fb4ade70ccc83c6c9f6c152d8',
  'd8eb4b903f2f803f2d57cd5a6afc7aaca868cbff9cf186960efa985b5f3b2c02',
  'linked-historical-reference',
  'analysis/link_identity/code_windows.json'),
 ('0x00181bac',
  '_Z13S9xSetSPC7110ht',
  'src/snes9x/spc7110_access.cpp',
  '2480',
  '129',
  'd71ce49bd549518ac8151b78037e21db032d4639a84eb73f992c7a232ddc27e4',
  '5003039f872a45c124ecc2bf85298cbc73f88f7ad0034d9ce3f3ad106ff84917',
  '914022fa4744757a55ef796608ab32f55ea941d91d75c7e5a8fd8c1582b3d90c',
  'b0e0b1b5782b26b6ad8962e30a58704ab2eee75bfe0145ec6f938384166b108a',
  'provider-linked-target',
  'analysis/matching/hunt1041-v72-validated-v53-6.tsv'),
 ('0x0018255c',
  'S9xGetSPC7110Byte',
  'src/snes9x/spc7110_access.cpp',
  '136',
  '10',
  'd4049c9d08117d1041bd2f9c9f5aaa0f9c6d98f7220ed2044e8426932caf41db',
  'd4049c9d08117d1041bd2f9c9f5aaa0f9c6d98f7220ed2044e8426932caf41db',
  '6e569a3dfe9f8e1489919dca5ae4923aeb079f203e16e9e772cc0f801e3ba9aa',
  '73ba1ebd8e9b5aeecc1fa31493bf9dff4a796883c80d822bdf54747ed944c4c1',
  'provider-linked-target',
  'analysis/functions/progress11_short_targets.asm'))
PROVIDERS = {'DAT_003454e0': 3429600,
 'DAT_0034e29c': 3465884,
 'DAT_0034e2b0': 3465904,
 'DAT_004134f4': 4273396,
 'DAT_00423548': 4339016,
 '_Z12S9xUpdateRTCv': 1582648,
 '_ZN7CMemory11SPC7110SramEh': 1403012,
 'g_s7r_blob': 4273416}

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
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v72_promote_v53 as historical

    original_path = historical.build_objects(CXX)["spc7110"].path
    if digest(original_path.read_bytes()) != "7686d49c6d7cfdad4ed23c7f53ce3f06b909a5d3451b098327d4517ea2811b28":
        raise SystemExit("complete historical SPC7110 V72 fingerprint drift")
    original = ELFFile(original_path)
    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "spc7110_access.o"
    run([CXX, *FLAGS, "-c", ROOT / "src/snes9x/spc7110_access.cpp", "-o", obj])
    isolated = ELFFile(obj)
    functions = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    text = [s for s in isolated.sections if s.name == ".text" or s.name.startswith(".text.")]
    if (len(functions) != 3 or {s.name for s in functions} != {r[1] for r in SPECS}
            or sum(s.size for s in text) != 4596):
        raise SystemExit("unproved SPC7110 access functions/instructions")
    readonly = next(s for s in isolated.sections if s.name == ".rodata")
    if readonly.size != 528:
        raise SystemExit("SPC7110 access jump-table extent drift")
    script = BUILD / "spc7110_access.target.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
                      + "SECTIONS {\n"
                      + "".join(f" .text.{i} {int(r[0], 0):#x} : {{ *(.text.{r[1]}) }}\n"
                                for i, r in enumerate(SPECS))
                      + " .rodata 0x001b85dc : { *(.rodata*) }\n"
                      + " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)"
                      + " *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked = BUILD / "spc7110_access.target.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked])
    final = ELFFile(linked)

    # Preserve original complete sections and their exact relocation addends.
    # Calls outside the selected core are never executed or claimed; only
    # those unused external dependencies receive zero addresses in this
    # comparison artifact. Every dependency of the three compared bodies
    # is bound to its recorded nonzero address or original placed definition.
    legacy = {"Settings": 0x003454E0, "ROM": 0x0034E29C,
              "Memory": 0x0034E2B0, "_ZN7CMemory11SPC7110SramEh": 0x00156884}
    for symbol in original.symbols:
        if symbol.name and symbol.section_index == 0:
            legacy.setdefault(symbol.name, 0)
    reference_script = BUILD / "historical-core.reference.ld"
    reference_script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in legacy.items())
                               + "SECTIONS { .text 0x0018076c : { *(.text) }\n"
                               + " .data 0x004134e8 : { *(.data) }\n"
                               + " .bss 0x00423644 : { *(.bss) }\n"
                               + " .rodata 0x001b8580 : { *(.rodata) }\n"
                               + " /DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment)"
                               + " *(.note*) *(.eh_frame*) } }\n")
    reference_path = BUILD / "historical-core.reference.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", reference_script, original_path, "-o", reference_path])
    reference = ELFFile(reference_path)
    for name, address in (("s7r", 0x00413508), ("rtc_f9", 0x00423548),
                          ("Copy7110", 0x004134F4), ("_Z12S9xUpdateRTCv", 0x00182638)):
        if reference.find_symbol(name).value != address:
            raise SystemExit(f"historical core state/provider placement drift: {name}")
    selected = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    rows = []
    for spec in SPECS:
        (address_text, name, source, size_text, relocation_text, historical_sha,
         isolated_sha, normalized_sha, linked_sha, level, evidence) = spec
        address, size, relocations = int(address_text, 0), int(size_text), int(relocation_text)
        captures = [s for s in selected if s.get("address") == address and s.get("symbol") == name]
        expected = {"size": size, "new_bytes": size, "raw_sha256": historical_sha,
                    "relocations": relocations}
        if len(captures) != 1 or any(captures[0].get(k) != v for k, v in expected.items()):
            raise SystemExit(f"frozen SPC7110 access window drift: {name}")
        old, local, placed, ref = (e.find_symbol(name) for e in (original, isolated, final, reference))
        if (any(s.size != size for s in (old, local, placed, ref))
                or len(original.relocation_ranges(old, size)) != relocations
                or len(isolated.relocation_ranges(local, size)) != relocations
                or digest(original.symbol_bytes(old, size)) != historical_sha
                or digest(isolated.symbol_bytes(local, size)) != isolated_sha
                or digest(normalized(original, old)) != normalized_sha
                or digest(normalized(isolated, local)) != normalized_sha
                or placed.value != address or ref.value != address
                or digest(final.symbol_bytes(placed, size)) != linked_sha
                or final.symbol_bytes(placed, size) != reference.symbol_bytes(ref, size)):
            raise SystemExit(f"SPC7110 access instructions/provider values drift: {name}")
        if name == "_Z13S9xSetSPC7110ht":
            with (ROOT / evidence).open(newline="") as stream:
                witness = [r for r in csv.DictReader(stream, delimiter="\t")
                           if r["address"] == address_text and r["object_symbol"] == name]
            if (len(witness) != 1 or witness[0]["result"] != "MATCH"
                    or witness[0]["differing_bytes"] != "0" or witness[0]["unknown_relocations"]
                    or witness[0]["object_size"] != size_text or witness[0]["target_span_sha256"] != linked_sha):
                raise SystemExit("frozen SPC7110 setter target proof drift")
        elif name == "S9xGetSPC7110Byte":
            words = {}
            for line in (ROOT / evidence).read_text().splitlines():
                match = LINE.match(line)
                if match and address <= int(match[1], 16) < address + size:
                    start = int(match[1], 16)
                    if start in words:
                        raise SystemExit("duplicate target instruction")
                    words[start] = bytes.fromhex(match[2])
            if set(words) != set(range(address, address + size, 4)):
                raise SystemExit("incomplete SPC7110 direct ROM read listing")
            target = b"".join(words[a] for a in range(address, address + size, 4))
            if target != final.symbol_bytes(placed, size):
                raise SystemExit("provider-linked SPC7110 direct ROM read mismatch")
        rows.append(dict(zip(("address", "symbol", "source_file", "size", "relocations",
                              "historical_raw_sha256", "isolated_raw_sha256", "normalized_sha256",
                              "linked_sha256", "proof_level", "evidence"), spec)))
        print(f"SPC7110 access: MATCH {name} bytes={size}/{size} proof={level}")
    with EVIDENCE.open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen:
        raise SystemExit("SPC7110 access ledger drift")
    (BUILD / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
    print("SPC7110 access: MATCH functions=3/3 historical_bytes=4596/4596 target_bytes=2616/2616 linked_reference_bytes=1980/1980")


if __name__ == "__main__":
    main()
