#!/usr/bin/env python3
"""Check native clipping against the complete frozen linked target function.

The two original local comparators are rebuilt and compared with the pinned
public source. Their target-body bytes were not captured and are excluded from
the 4,572-byte target claim. qsort uses its independently proved signed int ABI.
"""
from __future__ import annotations
import csv
import hashlib
import importlib.util
import json
import re
import struct
from pathlib import Path
from compare_elf_functions import ELFFile
from build_source_tree import SOURCE_FIXED_FLAGS

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/matching/native-clip-source-recovery'
SOURCE = 'src/snes9x/native_clip.cpp'
LEDGER = ROOT / 'analysis/functions/native_clip_exact_4572.tsv'
NAME = '_Z18ComputeClipWindowsv'
ADDRESS, SIZE = 0x114818, 4572
TARGET_SHA256 = '6e8218f0344f7df2d2002cdda3a01bab68e06dec7fa3230659f08d7249b50a56'
ADDRESSES = {'_Z10IntComparePKvS0_': 0x1147c8, '_Z11BandComparePKvS0_': 0x1147f0, NAME: ADDRESS}


def load(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'tools' / ('run-' + name + '-source-recovery.py'))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def script(elf, providers):
    body = ''.join(f'PROVIDE({name} = {value:#x});\n' for name, value in providers.items())
    body += 'SECTIONS {\n'
    for index, (name, address) in enumerate(ADDRESSES.items()):
        symbol = elf.find_symbol(name)
        body += f'.fn{index} {address:#x} : {{ *({elf.sections[symbol.section_index].name}) }}\n'
    body += '/DISCARD/ : { *(.data*) *(.rodata*) *(.bss*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n'
    return body


def main(record=False):
    cpu, fx = load('native-cpu-interrupts'), load('native-fxemu')
    cpu.main()
    fx.main()
    if tuple(SOURCE_FIXED_FLAGS[SOURCE]) != tuple(cpu.FLAGS):
        raise SystemExit('native clipping compiler profile drift')
    import sys
    sys.path.insert(0, str(ROOT / 'tools/history/research'))
    import hunt1041_v52_closure as recipe
    layout = fx.BUILD / 'profile/historical/snes9x-target-layout'
    archive = ROOT / 'build/upstream/hunt1000plus-v47-snes/source-1.41-1/snes9x-1.41-1-src'
    newlib = recipe.v47.PS2DEV / 'ps2toolchain/soft/newlib-1.10.0/newlib/libc/include'
    includes = ['-I' + str(p) for p in (fx.BUILD / 'compat', newlib, layout, layout / 'unzip', archive / 'zlib')]
    BUILD.mkdir(parents=True, exist_ok=True)
    expanded = cpu.run([cpu.CXX, *cpu.FLAGS, *includes, '-x', 'c++', '-E', '-P', layout / 'CLIP.CPP'])
    declaration = 'size_t __nmemb, size_t __size'
    if expanded.count(declaration) != 3:
        raise SystemExit('historical qsort declaration drift')
    # The original target qsort source proof pins size_t=int. Keep the rest of
    # the complete Snes9x layout/header profile unchanged.
    expanded = expanded.replace(declaration, 'int __nmemb, int __size')
    original_path = BUILD / 'historical.cpp'
    original_path.write_text(expanded)
    historical, native = BUILD / 'historical.o', BUILD / 'native.o'
    cpu.run([cpu.CXX, *cpu.FLAGS, '-c', original_path, '-o', historical])
    cpu.run([cpu.CXX, *cpu.FLAGS, '-c', ROOT / SOURCE, '-o', native])
    reference, candidate = ELFFile(historical), ELFFile(native)
    for elf in (reference, candidate):
        functions = {(s.name, s.size, s.info >> 4) for s in elf.symbols if s.info & 15 == 2 and s.size}
        if functions != {(NAME, SIZE, 1), ('_Z10IntComparePKvS0_', 40, 0), ('_Z11BandComparePKvS0_', 40, 0)}:
            raise SystemExit('native clipping function inventory drift')
        if any(s.size and s.type == 8 for s in elf.sections) or any(s.size and s.info & 15 == 1 and s.section_index for s in elf.symbols):
            raise SystemExit('unexpected owned native clipping storage')
        # Unsized compiler unwind metadata in .data is discarded by the link
        # and is outside the instruction and target-body claims.
    globals_elf = ELFFile(cpu.BUILD / 'GLOBALS.historical.o')
    states = {s.name: s.value + 0x345060 for s in globals_elf.symbols if s.info & 15 == 1 and s.size}
    # Addresses and the signed qsort ABI are authenticated by these maintained
    # source ledgers; pointer-width compatibility is explicit in the source.
    qsort_proof = load('qsort')
    qsort_proof.check_evidence()
    providers = {'qsort': 0x1080cc, 'memmove': 0x19c4a0, '__gxx_personality_v0': 0x1a9768, **states}
    linked = []
    for tag, elf, obj in [('reference', reference, historical), ('native', candidate, native)]:
        imports = {s.name for s in elf.symbols if s.name and not s.section_index}
        bindings = {}
        for name in imports:
            if name in providers:
                bindings[name] = providers[name]
            elif re.fullmatch(r'DAT_[0-9a-f]{8}', name):
                bindings[name] = int(name[4:], 16)
                if bindings[name] not in states.values():
                    raise SystemExit('native clipping state address lacks original GLOBALS witness')
            else:
                raise SystemExit('unproved native clipping import: ' + name)
        ld, output = BUILD / (tag + '.ld'), BUILD / (tag + '.elf')
        ld.write_text(script(elf, bindings))
        cpu.run([cpu.CXX.with_name('ee-ld'), '-EL', '-T', ld, obj, '-o', output])
        linked.append(ELFFile(output))
    windows = json.loads((ROOT / 'analysis/link_identity/code_windows.json').read_text())['result']['selected_sources']
    witness = next(w for w in windows if w['name'] == 'stage3p_compute_clip_windows')
    if (witness['address'], witness['size'], witness['relocations'], witness['raw_sha256']) != (ADDRESS, SIZE, 0, TARGET_SHA256):
        raise SystemExit('frozen complete clipping witness drift')
    source = (ROOT / witness['object']).read_text()
    body = source.split('stage3p_compute_clip_windows:\n', 1)[1].split('.size stage3p_compute_clip_windows', 1)[0]
    target = b''.join(struct.pack('<I', int(m[1], 16)) for m in re.finditer(r'\.word (0x[0-9a-f]+)', body))
    if len(target) != SIZE or hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise SystemExit('frozen clipping instruction bytes drift')
    for name in ADDRESSES:
        a, z = (e.find_symbol(name) for e in linked)
        raw = linked[1].symbol_bytes(z, z.size)
        if (a.value != ADDRESSES[name] or z.value != ADDRESSES[name]
                or cpu.normalized(reference, reference.find_symbol(name)) != cpu.normalized(candidate, candidate.find_symbol(name))
                or linked[0].symbol_bytes(a, a.size) != raw):
            raise SystemExit('linked native clipping/public helper mismatch')
        if name == NAME and raw != target:
            raise SystemExit('complete linked clipping target mismatch')
    row = dict(address=f'0x{ADDRESS:08x}', symbol=NAME, size=str(SIZE), source_file=SOURCE,
               historical_raw_sha256=cpu.digest(reference.symbol_bytes(reference.find_symbol(NAME), SIZE)),
               normalized_sha256=cpu.digest(cpu.normalized(candidate, candidate.find_symbol(NAME))),
               linked_sha256=TARGET_SHA256, proof_level='complete-frozen-linked-target')
    if record:
        with LEDGER.open('w', newline='') as stream:
            writer = csv.DictWriter(stream, fieldnames=list(row), delimiter='\t', lineterminator='\n')
            writer.writeheader()
            writer.writerow(row)
    elif list(csv.DictReader(LEDGER.open(), delimiter='\t')) != [row]:
        raise SystemExit('native clipping ledger drift')
    ownership = list(csv.DictReader((ROOT / 'analysis/source_tree/defined_symbol_ownership.tsv').open(), delimiter='\t'))
    if not record and not any(r['symbol'] == NAME and r['source'] == SOURCE and r['binding'] == 'global' and r['size_hex'] == hex(SIZE) for r in ownership):
        raise SystemExit('canonical native clipping ownership drift')
    (BUILD / 'report.json').write_text(json.dumps(dict(target_routines=1, instruction_bytes=SIZE,
        upstream_helpers=2, unclaimed_helper_bytes=80, target_sha256=TARGET_SHA256,
        fresh_private_elf=False, replacement_image=False), indent=2) + '\n')
    print('native clipping: MATCH 1/1 complete target function, 4572/4572 linked target bytes; 80 public helper bytes excluded from target claim')


if __name__ == '__main__':
    main()
