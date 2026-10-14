#!/usr/bin/env python3
"""Prove all six historical C4 math bodies and 2652 provider-linked bytes.

The complete pinned V75 upstream module establishes the object fingerprint.
The canonical source aliases existing data and runtime providers; only known
relocation fields may differ before linking. Five formal rows and the exact
132-byte C4Op15 companion cover every instruction in the module. No private
image is needed and no whole-image capture is changed.
"""
from __future__ import annotations

import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/c4-math-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/c4_math_exact_2652.tsv"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-mlong64", "-mhard-float",
    "-mno-abicalls", "-march=r5900", "-mtune=r5900", "-Os", "-fno-builtin",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES", "-DCPU_SHUTDOWN",
    "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE", "-DSPC700_C",
    "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)


SPECS = (('0x0010b8a4',
  'C4TransfWireFrame',
  'src/snes9x/c4_math.cpp',
  '808',
  '89',
  '11117a3990fb8299ce45d425d72fa804ea32ab81dddede0937d04e65284ae255',
  '65718fcd4587b6711fcd553b502430384558ec48b7b6b0d976b46476f8ffed5c',
  '65718fcd4587b6711fcd553b502430384558ec48b7b6b0d976b46476f8ffed5c',
  '836a03bef39c640f2f73345444026688456ae3e64678907e41517fe9736efc3a',
  'formal-manifest',
  'analysis/matching/hunt1041-v75-validated-c4-5.tsv'),
 ('0x0010bbcc',
  'C4TransfWireFrame2',
  'src/snes9x/c4_math.cpp',
  '768',
  '88',
  '9c320ef5888d856e0958c0999117c2199ffffaa25e1a4ee7595f5c29d735a912',
  '175523cf86d1d141733c652e692ef2c7fc79abbedbaac106062de69031c7ef12',
  '13b100fe60bdd88a1ca8a669a7bba135ed8baa72b6ae6b9585a239cadfe7d1e5',
  '979b7761e73d9bb38d98c8a2a7e092098a81f2d985c0b4634bc6895f3d6fba26',
  'formal-manifest',
  'analysis/matching/hunt1041-v75-validated-c4-5.tsv'),
 ('0x0010becc',
  'C4CalcWireFrame',
  'src/snes9x/c4_math.cpp',
  '456',
  '35',
  'e212ccf7f2dd9cc6de63612684f190f410ddad71007cdf0dce3534d9fdd10707',
  'e212ccf7f2dd9cc6de63612684f190f410ddad71007cdf0dce3534d9fdd10707',
  'e212ccf7f2dd9cc6de63612684f190f410ddad71007cdf0dce3534d9fdd10707',
  'fe5df2ee28aad2cb5f3098f332af85f7e08ba6eecb9c15b5fd60639425446f30',
  'formal-manifest',
  'analysis/matching/hunt1041-v75-validated-c4-5.tsv'),
 ('0x0010c094',
  'C4Op1F',
  'src/snes9x/c4_math.cpp',
  '224',
  '23',
  'e49a33672b24ca94a28e00abdea6afa05cc15115f4a78467ad2a8963b8521dce',
  'e49a33672b24ca94a28e00abdea6afa05cc15115f4a78467ad2a8963b8521dce',
  '726e9a09fbcd87ccfcb6d77c4cc6ff4188461d70a1628fc2a8d5234fe86c6ba6',
  '4f960ff15e772d288e9469159b2f9b27928aed85916794192aefb05980166bd9',
  'formal-manifest',
  'analysis/matching/hunt1041-v75-validated-c4-5.tsv'),
 ('0x0010c174',
  'C4Op15',
  'src/snes9x/c4_math.cpp',
  '132',
  '9',
  '687190a8145552875da9bf91e518c916eae011b9c33a9698eabd6e44bc1adbdf',
  '687190a8145552875da9bf91e518c916eae011b9c33a9698eabd6e44bc1adbdf',
  '687190a8145552875da9bf91e518c916eae011b9c33a9698eabd6e44bc1adbdf',
  'bc97f7fe4eb3a27670fdfc05d693ff570fc6580bb6c0b1e2be43a1f28927cbfe',
  'auxiliary-companion',
  'analysis/matching/hunt1041-v75-c4-companion-1.tsv'),
 ('0x0010c1f8',
  'C4Op0D',
  'src/snes9x/c4_math.cpp',
  '264',
  '24',
  '00bfea6edd83a6276205a9810f099b920f38e10425d631e38bd5f4059275c243',
  '00bfea6edd83a6276205a9810f099b920f38e10425d631e38bd5f4059275c243',
  'aaa5dc96448ac37c0cbf718010894ef7910a3b4f2f2e485e6152be99266641b7',
  'e7e60c411845301df556805b1d6d058698fd0d2aaaef6683690f9bbb581c9cf8',
  'formal-manifest',
  'analysis/matching/hunt1041-v75-validated-c4-5.tsv'))
PROVIDERS = {'_DAT_00335938': 3365176,
 '_DAT_0033593a': 3365178,
 'DAT_0033593c': 3365180,
 'DAT_0033593e': 3365182,
 'DAT_00335940': 3365184,
 'DAT_00335942': 3365186,
 'DAT_00335944': 3365188,
 '_DAT_00335946': 3365190,
 '_DAT_00335948': 3365192,
 '_DAT_0033594a': 3365194,
 'DAT_0033594c': 3365196,
 'PTR_LAB_0033594e': 3365198,
 'atanf_001a045c': 1705052,
 'cosf_0019fddc': 1703388,
 'fRam0042c804': 4376580,
 'fRam0042c808': 4376584,
 'fRam0042c80c': 4376588,
 'fRam0042c810': 4376592,
 'fRam0042c814': 4376596,
 'fRam0042c818': 4376600,
 'fRam0042c81c': 4376604,
 'fabsf_001a06b0': 1705648,
 'sinf_001a0024': 1703972,
 'snes_dpadd': 1717680,
 'snes_dpdiv': 1718608,
 'snes_dpmul': 1717888,
 'snes_dptofp': 1719488,
 'snes_dptoli': 1719280,
 'snes_fptodp': 1717056,
 'sqrtf_001a06a0': 1705632}

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
    import hunt1041_v75_c4 as historical

    original_path = historical.build_objects(CXX)["c4"].path
    if digest(original_path.read_bytes()) != "7e4c9f0dc58d7d4f4d6d6d56e9e91df6e0ca2989ad0fdf90fb0936df021e7f7e":
        raise SystemExit("complete historical C4 object fingerprint drift")
    original = ELFFile(original_path)
    section = next(s for s in original.sections if s.name == ".text")
    selected = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    captures = [s for s in selected if s.get("name") == "c4"]
    expected = {"address": 0x0010B8A4, "size": 2652, "new_bytes": 2652,
                "relocations": 268, "source_offset": 0,
                "raw_sha256": "fdb3aec04aa7ac3fd1c9fcef0a36449fab9505bd3ff0741c84e507d1f54fef7d"}
    if (len(captures) != 1 or any(captures[0].get(k) != v for k, v in expected.items())
            or section.size != 2652
            or digest(original.data[section.offset:section.offset + section.size]) != expected["raw_sha256"]):
        raise SystemExit("frozen complete C4 code-window drift")
    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "c4_math.o"
    run([CXX, *FLAGS, "-c", ROOT / "src/snes9x/c4_math.cpp", "-o", obj])
    isolated = ELFFile(obj)
    functions = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    text = next(s for s in isolated.sections if s.name == ".text")
    if (len(functions) != 6 or {s.name for s in functions} != {r[1] for r in SPECS}
            or text.size != 2652 or sum(s.size for s in functions) != text.size):
        raise SystemExit("unproved C4 functions/instructions")
    script = BUILD / "c4_math.target.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
                      + "SECTIONS { .text 0x0010b8a4 : { *(.text) }\n"
                      + " .rodata 0x001b18d0 : { *(.rodata*) }\n"
                      + " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)"
                      + " *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked = BUILD / "c4_math.target.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked])
    final = ELFFile(linked)
    rows = []
    witnesses = {}
    for spec in SPECS:
        (address, name, source, size_text, relocation_text, historical_sha,
         isolated_sha, normalized_sha, linked_sha, scope, evidence) = spec
        size, relocations = int(size_text), int(relocation_text)
        if evidence not in witnesses:
            with (ROOT / evidence).open(newline="") as stream:
                witnesses[evidence] = list(csv.DictReader(stream, delimiter="\t"))
        witness = [r for r in witnesses[evidence] if r["address"] == address and r["object_symbol"] == name]
        if (len(witness) != 1 or witness[0]["object_size"] != size_text
                or witness[0]["relocation_count"] != relocation_text
                or witness[0]["result"] != "MATCH" or witness[0]["differing_bytes"] != "0"
                or witness[0]["unknown_relocations"] or witness[0]["promotion_scope"] != scope
                or witness[0]["target_span_sha256"] != linked_sha):
            raise SystemExit(f"frozen C4 witness drift: {name}")
        old, local, placed = (e.find_symbol(name) for e in (original, isolated, final))
        if (old.size != size or local.size != size or placed.size != size
                or len(original.relocation_ranges(old, size)) != relocations
                or len(isolated.relocation_ranges(local, size)) != relocations
                or digest(original.symbol_bytes(old, size)) != historical_sha
                or digest(isolated.symbol_bytes(local, size)) != isolated_sha
                or digest(normalized(original, old)) != normalized_sha
                or digest(normalized(isolated, local)) != normalized_sha
                or placed.value != int(address, 0)
                or digest(final.symbol_bytes(placed, size)) != linked_sha):
            raise SystemExit(f"C4 object/provider-linked bytes drift: {name}")
        rows.append(dict(zip(("address", "symbol", "source_file", "size", "relocations",
                              "historical_raw_sha256", "isolated_raw_sha256", "normalized_sha256",
                              "linked_sha256", "promotion_scope", "evidence"), spec)))
        print(f"C4 math provider-linked: MATCH {name} bytes={size}/{size} relocations={relocations}")
    with EVIDENCE.open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen:
        raise SystemExit("C4 math ledger drift")
    (BUILD / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
    print("C4 math: MATCH functions=6/6 bytes=2652/2652 relocations=268 (5 formal + 1 companion)")


if __name__ == "__main__":
    main()
