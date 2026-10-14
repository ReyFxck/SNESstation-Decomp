#!/usr/bin/env python3
"""Prove five original SPC7110 helpers and 1784 provider-linked bytes.

Pinned V72/V74 source rebuilds establish historical instruction identity.
RTC update, ROM base lookup, reset and both FIO persistence functions then
match frozen target hashes or complete public listings after linking. Private
image captures and whole-image claims are not regenerated.
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
BUILD = ROOT / "build/matching/spc7110-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/spc7110_exact_1784.tsv"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900", "-Os",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES", "-DCPU_SHUTDOWN",
    "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE", "-DSPC700_C",
    "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
PROVIDERS = {
    "DAT_0034e29c": 0x0034E29C, "DAT_00423548": 0x00423548,
    "g_s7r_blob": 0x00413508, "__divdi3": 0x001A1DB8,
    "_Z17S9xRTCDaysInMonthii": 0x001825E4, "memset": 0x0019C39C,
    "snes_p12_get_filename": 0x00101924,
    "fioOpen": 0x0019CFC0, "fioClose": 0x0019D090,
    "fioRead": 0x0019D120, "fioWrite": 0x0019D244,
}
ORIGINAL_HASHES = {
    "v72": "7686d49c6d7cfdad4ed23c7f53ce3f06b909a5d3451b098327d4517ea2811b28",
    "v74": "b7d8e4a8a926eacc4031d0bbf16c0ae491f7c60fc4aded2fb9d5f368fe1713fd",
}
LINE = re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} ){3}[0-9a-fA-F]{2})\s")


SPECS = (('0x00182638',
  '_Z12S9xUpdateRTCv',
  'src/snes9x/spc7110_helpers.cpp',
  '728',
  '12',
  '82b5da31a2897ff9de72259891732c7a7a5838e98c82ffef7464ffb41e18b905',
  '82b5da31a2897ff9de72259891732c7a7a5838e98c82ffef7464ffb41e18b905',
  '82b5da31a2897ff9de72259891732c7a7a5838e98c82ffef7464ffb41e18b905',
  '5cadf3ebda02080fc9f54580fb56d2ceb5dd60d8ae09633be82cc9a6bbd7ca59',
  'v72',
  'analysis/matching/hunt1041-v72-validated-v53-6.tsv'),
 ('0x00182910',
  'Get7110BasePtr',
  'src/snes9x/spc7110_helpers.cpp',
  '116',
  '8',
  'dea0718788d2b82f0006d1e8d31194d143a5bf6e8c79ce89d7cbcfe0fd9d7164',
  'dea0718788d2b82f0006d1e8d31194d143a5bf6e8c79ce89d7cbcfe0fd9d7164',
  'e4765f7529ab2cec612127d1ee338e72e8a58f3f70b22f9e8055ad3d57f06d0d',
  '225c9367cc7853814bd5dd1572f08f4ee686c36cfa792ad7fcb1f397cf49ec12',
  'v72',
  'analysis/functions/progress11_short_targets.asm'),
 ('0x001832a4',
  '_Z15S9xSpc7110Resetv',
  'src/snes9x/spc7110_helpers.cpp',
  '248',
  '4',
  '77c6f359ffb700d2f9fffb86b03a792e0eb63d0b2dfecb3a97b39557a08ed225',
  '77c6f359ffb700d2f9fffb86b03a792e0eb63d0b2dfecb3a97b39557a08ed225',
  '77c6f359ffb700d2f9fffb86b03a792e0eb63d0b2dfecb3a97b39557a08ed225',
  '2bfa20a83d92742640fabe2b8b5d4c780094bda1aa8b735f5720d2d048031434',
  'v72',
  'analysis/functions/progress13_targets.asm'),
 ('0x001833a4',
  '_Z17S9xSaveSPC7110RTCP10SPC7110RTC',
  'src/snes9x/spc7110_rtc_io.cpp',
  '332',
  '14',
  '575be965f1c51d33072a0f6d6d726e3b02eaa144082084c8e605f1d58046a7fc',
  'cf786ac0defb70500065250c55405ec435790e2d93b3d50b8ece9154696e2f08',
  'cf786ac0defb70500065250c55405ec435790e2d93b3d50b8ece9154696e2f08',
  '9b2d2cd4b69daf19b42e337d4680d1a84cca6703bbfd5d5121a522df9eb772b5',
  'v74',
  'analysis/matching/hunt1041-v74-validated-spc7110-rtc-2.tsv'),
 ('0x001834f0',
  '_Z17S9xLoadSPC7110RTCP10SPC7110RTC',
  'src/snes9x/spc7110_rtc_io.cpp',
  '360',
  '14',
  '7c35d321b0a6575f8a997d1dba6d7132df25665d4c3b9695519c5f8f6b18c9a2',
  '63947efdcf58c623c8428ca36bd15ef185e67c59bca9c7708a6a60bb05444bea',
  '63947efdcf58c623c8428ca36bd15ef185e67c59bca9c7708a6a60bb05444bea',
  'f6722672e6d348d673fe96239e99cf1f1e70e2db435ad7dd5bdc966bd4c046f0',
  'v74',
  'analysis/matching/hunt1041-v74-validated-spc7110-rtc-2.tsv'))

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


def listing_bytes(path, address, size):
    words = {}
    for line in path.read_text().splitlines():
        match = LINE.match(line)
        if match:
            start = int(match[1], 16)
            if address <= start < address + size:
                if start in words:
                    raise SystemExit(f"duplicate target instruction: {start:#x}")
                words[start] = bytes.fromhex(match[2])
    if set(words) != set(range(address, address + size, 4)):
        raise SystemExit(f"incomplete target listing: {address:#x}")
    return b"".join(words[start] for start in range(address, address + size, 4))


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v72_promote_v53 as v72
    import hunt1041_v74_spc7110_rtc as v74

    originals = {}
    for key, builder in (("v72", v72), ("v74", v74)):
        path = builder.build_objects(CXX)["spc7110"].path
        if digest(path.read_bytes()) != ORIGINAL_HASHES[key]:
            raise SystemExit(f"complete historical SPC7110 object drift: {key}")
        originals[key] = ELFFile(path)
    BUILD.mkdir(parents=True, exist_ok=True)
    objects, finals = {}, {}
    for unit in ("spc7110_helpers", "spc7110_rtc_io"):
        obj = BUILD / f"{unit}.o"
        split = ("-ffunction-sections",) if unit == "spc7110_helpers" else ()
        run([CXX, *FLAGS, *split, "-c", ROOT / f"src/snes9x/{unit}.cpp", "-o", obj])
        elf = objects[unit] = ELFFile(obj)
        specs = [r for r in SPECS if r[2] == f"src/snes9x/{unit}.cpp"]
        functions = [s for s in elf.symbols if s.info & 15 == 2 and s.size]
        sections = [s for s in elf.sections if s.name == ".text" or s.name.startswith(".text.")]
        if (len(functions) != len(specs) or {s.name for s in functions} != {r[1] for r in specs}
                or sum(s.size for s in sections) != sum(int(r[3]) for r in specs)):
            raise SystemExit(f"unproved SPC7110 functions/instructions: {unit}")
        script = BUILD / f"{unit}.target.ld"
        placements = ("".join(f" .text.{i} {int(r[0], 0):#x} : {{ *(.text.{r[1]}) }}\n"
                              for i, r in enumerate(specs)) if split
                      else " .text 0x001833a4 : { *(.text) }\n .rodata 0x001b8870 : { *(.rodata*) }\n")
        script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
                          + "SECTIONS {\n" + placements
                          + " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr)"
                          + " *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
        output = BUILD / f"{unit}.target.elf"
        run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", output])
        finals[unit] = ELFFile(output)
    selected = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    rows = []
    witnesses = {}
    for spec in SPECS:
        (address_text, name, source, size_text, relocation_text, historical_sha,
         isolated_sha, normalized_sha, linked_sha, key, evidence) = spec
        address, size, relocations = int(address_text, 0), int(size_text), int(relocation_text)
        captures = [s for s in selected if s.get("address") == address and s.get("symbol") == name]
        expected = {"size": size, "new_bytes": size, "raw_sha256": historical_sha,
                    "relocations": relocations}
        if len(captures) != 1 or any(captures[0].get(k) != v for k, v in expected.items()):
            raise SystemExit(f"frozen SPC7110 code-window drift: {name}")
        unit = Path(source).stem
        old, local, final = originals[key], objects[unit], finals[unit]
        original_symbol, symbol, placed = (e.find_symbol(name) for e in (old, local, final))
        if (original_symbol.size != size or symbol.size != size or placed.size != size
                or len(old.relocation_ranges(original_symbol, size)) != relocations
                or len(local.relocation_ranges(symbol, size)) != relocations
                or digest(old.symbol_bytes(original_symbol, size)) != historical_sha
                or digest(local.symbol_bytes(symbol, size)) != isolated_sha
                or digest(normalized(old, original_symbol)) != normalized_sha
                or digest(normalized(local, symbol)) != normalized_sha
                or placed.value != address or digest(final.symbol_bytes(placed, size)) != linked_sha):
            raise SystemExit(f"SPC7110 instruction/provider-linked bytes drift: {name}")
        if evidence.endswith(".asm"):
            target = listing_bytes(ROOT / evidence, address, size)
            if target != final.symbol_bytes(placed, size) or digest(target) != linked_sha:
                raise SystemExit(f"SPC7110 target listing drift: {name}")
        else:
            if evidence not in witnesses:
                with (ROOT / evidence).open(newline="") as stream:
                    witnesses[evidence] = list(csv.DictReader(stream, delimiter="\t"))
            witness = [r for r in witnesses[evidence] if r["address"] == address_text and r["object_symbol"] == name]
            if (len(witness) != 1 or witness[0]["result"] != "MATCH"
                    or witness[0]["differing_bytes"] != "0" or witness[0]["unknown_relocations"]
                    or witness[0]["object_size"] != size_text
                    or witness[0]["target_span_sha256"] != linked_sha):
                raise SystemExit(f"frozen SPC7110 target witness drift: {name}")
        rows.append(dict(zip(("address", "symbol", "source_file", "size", "relocations",
                              "historical_raw_sha256", "isolated_raw_sha256", "normalized_sha256",
                              "linked_sha256", "historical_build", "evidence"), spec)))
        print(f"SPC7110 provider-linked: MATCH {name} bytes={size}/{size} relocations={relocations}")
    with EVIDENCE.open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen:
        raise SystemExit("SPC7110 ledger drift")
    (BUILD / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
    print("SPC7110: MATCH functions=5/5 bytes=1784/1784 relocations=52")


if __name__ == "__main__":
    main()
