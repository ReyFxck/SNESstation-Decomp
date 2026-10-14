#!/usr/bin/env python3
"""Check two DSP profile/boundary recoveries against complete frozen targets.

Atan uses the public PS2 float variant with normal-width double constants.
DSP2GetByte's actual caller entry precedes the old residual label by 12 bytes;
adjacent authenticated captures cover its complete body. Frozen old windows
are preserved. No fresh private ELF or complete replacement image is claimed.
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
from build_source_tree import SOURCE_FIXED_FLAGS, set_instruction_section_alignment

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/matching/native-dsp-profiles-source-recovery'
LEDGER = ROOT / 'analysis/functions/native_dsp_profiles_exact_308.tsv'
ATAN_SOURCE = 'src/snes9x/native_dsp_atan.c'
GET_SOURCE = 'src/snes9x/native_dsp2_get.cpp'
PUBLIC_ATAN = 'matching/candidates/hunt1041_v51_dsp.c'
CAPTURES = 'matching/candidates/stage3p_code_residual_exact.S'
CALLER = 'analysis/functions/progress16_r5900_pseudocode.c.txt'
PINS = {
    PUBLIC_ATAN: '93a258580b65eced6f689f1b3c0757c5d24089e2613288f51dfe20a0af31e1ee',
    CAPTURES: '665251d6c4d55da8987ac64463f35e9fae4c7a918f7fd52089a5572bfdb7646b',
    CALLER: '95a2682a13d4ccac248daf8c7933cb7b68e0b5cd5756f3a079f784ab58762e85',
}
ATAN_TARGET = 'ac7d64687bffe8e4865e1f0c45fabc1b82ca6c9ec68e27bf9a2809a8e722df56'
ATAN_RAW = 'a36a0745eaf555ddd7739d5852c355fbf2bb58553ccc7a70cd10f1890ef62cf5'
GET_TARGET = 'd54cf4ce9e39b50a786d83481e479c96f0e820d38bebc45c6891b678716c342e'
RUNTIME = dict(fptodp=0x1a3340, dpmul=0x1a3680, dpadd=0x1a35b0,
               dpdiv=0x1a3950, dpsub=0x1a3610, dptofp=0x1a3cc0)


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def load(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'tools' / ('run-' + name + '-source-recovery.py'))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def readonly(elf):
    section = next((s for s in elf.sections if s.name == '.rodata'), None)
    return elf.data[section.offset:section.offset + section.size] if section else b''


def selected_bindings(elf, name, known):
    """Reject placeholders for every relocation in the selected function."""
    symbol = elf.find_symbol(name)
    section = elf.sections[symbol.section_index]
    rel = next((s for s in elf.sections if s.name == '.rel' + section.name), None)
    if rel:
        for position in range(rel.offset, rel.offset + rel.size, rel.entry_size):
            offset, info = struct.unpack_from('<II', elf.data, position)
            target = elf.symbols[info >> 8]
            if symbol.value <= offset < symbol.value + symbol.size and not target.section_index:
                if not known.get(target.name):
                    raise SystemExit('unproved selected DSP relocation: ' + target.name)


def link(cpu, tag, path, name, address, known, rodata=0):
    elf = ELFFile(path)
    selected_bindings(elf, name, known)
    # Zero placeholders occur only in discarded original functions/storage.
    imports = {s.name for s in elf.symbols if s.name and not s.section_index}
    text = ''.join(f'PROVIDE({n} = {known.get(n, 0):#x});\n' for n in sorted(imports))
    symbol = elf.find_symbol(name)
    text += 'SECTIONS {\n' + f'.body {address:#x} : {{ *({elf.sections[symbol.section_index].name}) }}\n'
    if rodata:
        text += f'.rodata {rodata:#x} : {{ *(.rodata*) }}\n'
    text += '/DISCARD/ : { *(.text*) *(.data*) *(.bss*) *(.rodata*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n'
    script, output = BUILD / (tag + '.ld'), BUILD / (tag + '.elf')
    script.write_text(text)
    cpu.run([cpu.CXX.with_name('ee-ld'), '-EL', '-T', script, path, '-o', output])
    linked = ELFFile(output)
    if linked.find_symbol(name).value != address:
        raise SystemExit('linked DSP entry drift: ' + name)
    return linked


def capture(source, label):
    body = source.split(label + ':', 1)[1].split('    .size ' + label, 1)[0]
    return b''.join(struct.pack('<I', int(w, 16)) for w in re.findall(r'\.word 0x([0-9a-f]+)', body))


def main(record=False):
    for name, sha in PINS.items():
        if digest((ROOT / name).read_bytes()) != sha:
            raise SystemExit('DSP provenance drift: ' + name)
    cpu, fx = load('native-cpu-interrupts'), load('native-fxemu')
    cpu.main()
    fx.main()
    BUILD.mkdir(parents=True, exist_ok=True)
    windows = json.loads((ROOT / 'analysis/link_identity/code_windows.json').read_text())['result']['selected_sources']
    evidence = list(csv.DictReader((ROOT / 'analysis/matching/hunt1041-v51-validated-16.tsv').open(), delimiter='\t'))
    witness = next(r for r in evidence if r['address'] == '0x0012bf5c')
    if (witness['object_size'], witness['normalized_equal'], witness['differing_bytes'], witness['unknown_relocations'],
            witness['source'], witness['target_span_sha256']) != ('208', 'True', '0', '', PUBLIC_ATAN, ATAN_TARGET):
        raise SystemExit('complete historical Atan target witness drift')
    window = next(w for w in windows if w['name'] == 'tsv_0012bf5c_snes_p28_0012bf5c')
    if (window['address'], window['size'], window['raw_sha256'], window['relocations']) != (0x12bf5c, 208, ATAN_RAW, 14):
        raise SystemExit('Atan frozen source window drift')
    import sys
    sys.path.insert(0, str(ROOT / 'tools/history/research'))
    import hunt1041_v52_closure as recipe
    atan_flags = [x for x in recipe.v47.COMMON_FLAGS if x != '-fshort-double']
    atan_flags += ['-O2', '-falign-functions=4', '-ffunction-sections', *recipe.v47.PS2_DEFINES]
    if tuple(atan_flags) != SOURCE_FIXED_FLAGS[ATAN_SOURCE] or SOURCE_FIXED_FLAGS[GET_SOURCE] != cpu.FLAGS:
        raise SystemExit('native DSP compiler profile drift')
    # The C driver belongs to the separately pinned C stage1 installation.
    cc = ROOT / 'build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc'
    reference_path, native_path = BUILD / 'atan.reference.o', BUILD / 'atan.native.o'
    cpu.run([cc, *atan_flags, '-c', ROOT / PUBLIC_ATAN, '-o', reference_path])
    cpu.run([cc, *atan_flags, '-c', ROOT / ATAN_SOURCE, '-o', native_path])
    reference, native = ELFFile(reference_path), ELFFile(native_path)
    old_name, new_name = 'snes_p28_0012bf5c', 'S9xDSPAtan'
    if digest(reference.symbol_bytes(reference.find_symbol(old_name), 208)) != ATAN_RAW:
        raise SystemExit('original Atan raw instructions drift')
    ro_witness = next(r for r in json.loads((ROOT / 'analysis/link_identity/window11_rodata.json').read_text())['source_sections'] if r['name'] == 'dsp_normal_constants')
    if (ro_witness['address'], ro_witness['size'], ro_witness['source_offset']) != (0x1b20b8, 32, 0):
        raise SystemExit('Atan readonly placement witness drift')
    if digest(readonly(reference)) != '582fa1a78b32c4bd50337b7fc7580a8ff9efdcdf901bd882a67e16ed7b6a4761':
        raise SystemExit('complete original DSP normal-double constants drift')
    if len(readonly(native)) != 16 or readonly(native) != readonly(reference)[:16]:
        raise SystemExit('native Atan readonly slice mismatch')
    # Both canonical compilation and this proof use the same metadata-only
    # correction. The helper verifies every section payload is unchanged.
    set_instruction_section_alignment(reference_path, '.text.' + old_name, 4)
    set_instruction_section_alignment(native_path, '.text.' + new_name, 4)
    original_link = link(cpu, 'atan.reference', reference_path, old_name, 0x12bf5c, RUNTIME, 0x1b20b8)
    native_link = link(cpu, 'atan.native', native_path, new_name, 0x12bf5c,
                       {'snes_' + n: v for n, v in RUNTIME.items()}, 0x1b20b8)
    if readonly(native_link) != readonly(original_link)[:16]:
        raise SystemExit('linked Atan readonly slice mismatch')
    rows = []

    def compare(reference, native, original_link, native_link, old_name, new_name, address, size, target_sha, source, boundary):
        old, new = reference.find_symbol(old_name), native.find_symbol(new_name)
        linked_old, linked_new = original_link.find_symbol(old_name), native_link.find_symbol(new_name)
        raw = native_link.symbol_bytes(linked_new, linked_new.size)
        if (old.size != size or new.size != size or linked_old.size != size or linked_new.size != size
                or cpu.normalized(reference, old) != cpu.normalized(native, new)
                or original_link.symbol_bytes(linked_old, size) != raw or digest(raw) != target_sha):
            raise SystemExit('complete linked DSP target mismatch: ' + new_name)
        functions = {(s.name, s.size) for s in native.symbols if s.info & 15 == 2 and s.size}
        if functions != {(new_name, size)}:
            raise SystemExit('unexpected native DSP function inventory')
        if any(s.size and s.name.startswith('.rodata') and s.name != '.rodata' for s in native.sections):
            raise SystemExit('unexpected native DSP readonly section')
        if any(s.type == 8 and s.size for s in native.sections) or any(s.size and s.info & 15 == 1 and s.section_index for s in native.symbols):
            raise SystemExit('unexpected owned native DSP storage')
        rows.append(dict(address=f'0x{address:08x}', symbol=new_name, historical_symbol=old_name, size=str(size),
            source_file=source, source_sha256=digest((ROOT / source).read_bytes()),
            historical_raw_sha256=digest(reference.symbol_bytes(old, size)),
            normalized_sha256=digest(cpu.normalized(native, new)), linked_sha256=target_sha,
            boundary_evidence=boundary, proof_level='complete-frozen-linked-target'))

    compare(reference, native, original_link, native_link, old_name, new_name, 0x12bf5c, 208,
            ATAN_TARGET, ATAN_SOURCE, 'hunt1041-v51-validated-16.tsv:exact-next-boundary;PS2-float-variant')

    layout = fx.BUILD / 'profile/historical/snes9x-target-layout'
    archive = ROOT / 'build/upstream/hunt1000plus-v47-snes/source-1.41-1/snes9x-1.41-1-src'
    newlib = recipe.v47.PS2DEV / 'ps2toolchain/soft/newlib-1.10.0/newlib/libc/include'
    includes = ['-I' + str(p) for p in (fx.BUILD / 'compat', newlib, layout, layout / 'unzip', archive / 'zlib')]
    reference_path, native_path = BUILD / 'get.reference.o', BUILD / 'get.native.o'
    cpu.run([cpu.CXX, *cpu.FLAGS, '-DZLIB', *includes, '-x', 'c++', '-c', layout / 'DSP1.CPP', '-o', reference_path])
    cpu.run([cpu.CXX, *cpu.FLAGS, '-c', ROOT / GET_SOURCE, '-o', native_path])
    reference, native = ELFFile(reference_path), ELFFile(native_path)
    globals_elf = ELFFile(cpu.BUILD / 'GLOBALS.historical.o')
    if globals_elf.find_symbol('DSP1').value + 0x345060 != 0x345628:
        raise SystemExit('original DSP1 shared-state address drift')
    name = '_Z11DSP2GetBytet'
    original_link = link(cpu, 'get.reference', reference_path, name, 0x12fb78, {'DSP1': 0x345628})
    native_link = link(cpu, 'get.native', native_path, name, 0x12fb78, {'DAT_00345628': 0x345628})
    captures = (ROOT / CAPTURES).read_text()
    spans = [('stage3p_dsp2_set_byte', 0x12f8a8, 732, '88ad630d2863a33b2edafbe084f63f812db12f097891329e4b315448070299b4'),
             ('stage3p_dsp2_get_byte', 0x12fb84, 100, '37fd29a2296ae3baa7e4b2f73a64657497ed17a24087bd76b08246bfdd74644d')]
    raw_spans = []
    for label, address, size, sha in spans:
        window = next(w for w in windows if w['name'] == label)
        raw = capture(captures, label)
        if (window['address'], window['size'], window['relocations'], window['raw_sha256']) != (address, size, 0, sha) or len(raw) != size or digest(raw) != sha:
            raise SystemExit('frozen adjacent DSP2 capture drift')
        raw_spans.append(raw)
    caller = (ROOT / CALLER).read_text()
    if 'PTR_FUN_0034165c = &LAB_0012fb78;' not in caller:
        raise SystemExit('actual DSP2 byte-reader caller entry witness drift')
    target = raw_spans[0][720:] + raw_spans[1][:88]
    if spans[0][1] + 720 != 0x12fb78 or spans[0][1] + spans[0][2] != spans[1][1] or len(target) != 100 or digest(target) != GET_TARGET:
        raise SystemExit('complete DSP2 reader boundary reconstruction drift')
    if readonly(native) or readonly(native_link):
        raise SystemExit('unexpected native DSP2 reader readonly storage')
    compare(reference, native, original_link, native_link, name, name, 0x12fb78, 100,
            GET_TARGET, GET_SOURCE, 'progress16:PTR_FUN_0034165c;adjacent-frozen-captures:last12+first88')
    if record:
        with LEDGER.open('w', newline='') as stream:
            writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter='\t', lineterminator='\n')
            writer.writeheader()
            writer.writerows(rows)
    elif list(csv.DictReader(LEDGER.open(), delimiter='\t')) != rows:
        raise SystemExit('native DSP profile ledger drift')
    if not record:
        ownership = list(csv.DictReader((ROOT / 'analysis/source_tree/defined_symbol_ownership.tsv').open(), delimiter='\t'))
        for row in rows:
            if not any(r['symbol'] == row['symbol'] and r['source'] == row['source_file'] and r['binding'] == 'global' and r['size_hex'] == hex(int(row['size'])) for r in ownership):
                raise SystemExit('canonical native DSP ownership drift')
    (BUILD / 'report.json').write_text(json.dumps(dict(functions=2, instruction_bytes=308,
        separately_checked_readonly_bytes=16, atan_code_alignment=4, original_dsp2_window_entry=0x12fb84,
        reviewed_dsp2_function_entry=0x12fb78, fresh_private_elf=False, replacement_image=False), indent=2) + '\n')
    print('native DSP profiles: MATCH 2/2 complete functions, 308/308 linked target instruction bytes; readonly checked separately')


if __name__ == '__main__':
    main()
