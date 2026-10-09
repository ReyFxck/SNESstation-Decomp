#!/usr/bin/env python3
"""Check the complete eight-module native batch against linked public references.

Frozen source windows and prior matching ledgers authenticate the historical
functions. Selected functions are linked at their reviewed entries and checked
without relocation masking. The reference retains excluded bodies at explicitly
unclaimed comparison locations. Owned storage and retained readonly slices are
checked separately; this is not a fresh private-ELF or replacement-image run.
"""
from __future__ import annotations
import csv
import importlib.util
import json
import struct
import re
from pathlib import Path
from compare_elf_functions import ELFFile
from build_source_tree import SOURCE_FIXED_FLAGS
ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/matching/native-bulk-source-recovery'
CONFIG = ROOT / 'analysis/functions/native_bulk_84756_config.json'
CONFIG_SHA256 = '4bdfe75e10df4cf5885b23c24a75e4ea484a502b76c76f6f5d95334df511931f'


def load_proof(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'tools' / ('run-' + name + '-source-recovery.py'))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def imports(elf):
    return {s.name for s in elf.symbols if s.name and not s.section_index}


def section_bytes(elf, name):
    section = next((s for s in elf.sections if s.name == name), None)
    if section is None:
        return b''
    if section.type == 8:
        return bytes(section.size)
    return elf.data[section.offset:section.offset + section.size]


def relocations(elf, section_name):
    section = next((s for s in elf.sections if s.name == '.rel' + section_name), None)
    if section:
        for offset in range(section.offset, section.offset + section.size, section.entry_size):
            position, info = struct.unpack_from('<II', elf.data, offset)
            yield position, elf.symbols[info >> 8]


def script(module, reference=False):
    providers = module['reference_providers'] if reference else module['native_providers']
    sections = module['reference_functions'] if reference else module['functions']
    assignments = module['reference_assignments'] if reference else {}
    body = ''.join(f'PROVIDE({name} = {value:#x});\n' for name, value in providers.items())
    body += ''.join(f'{name} = {value:#x};\n' for name, value in assignments.items())
    body += 'SECTIONS {\n'
    for index, row in enumerate(sections):
        body += f".fn{index} {row['address']:#x} : {{ *({row['section']}) }}\n"
    body += f".data {module['data']:#x} : {{ *(.data*) }}\n"
    if not reference and module.get('readonly_sections'):
        for row in module['readonly_sections']:
            body += f"{row['section']} {row['address']:#x} : {{ *({row['section']}) }}\n"
    elif module['rodata']:
        address = module['rodata'] + (0 if reference else module['rodata_shift'])
        body += f'.rodata {address:#x} : {{ *(.rodata*) }}\n'
    if module['bss']:
        body += f".bss {module['bss']:#x} : {{ *(.bss*) }}\n"
    body += '/DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n'
    return body


def run_batch(record=False):
    cpu, fx = load_proof('native-cpu-interrupts'), load_proof('native-fxemu')
    cpu.main()
    fx.main()
    run, digest, normalized = cpu.run, cpu.digest, cpu.normalized
    config_raw = CONFIG.read_bytes()
    if not record and digest(config_raw) != CONFIG_SHA256:
        raise SystemExit('native batch config drift')
    modules = json.loads(config_raw)
    import sys
    sys.path.insert(0, str(ROOT / 'tools/history/research'))
    import hunt1041_v52_closure as recipe
    layout = fx.BUILD / 'profile/historical/snes9x-target-layout'
    archive = ROOT / 'build/upstream/hunt1000plus-v47-snes/source-1.41-1/snes9x-1.41-1-src'
    newlib = recipe.v47.PS2DEV / 'ps2toolchain/soft/newlib-1.10.0/newlib/libc/include'
    includes = ['-I' + str(p) for p in (fx.BUILD / 'compat', newlib, layout, layout / 'unzip', archive / 'zlib')]
    windows = json.loads((ROOT / 'analysis/link_identity/code_windows.json').read_text())['result']['selected_sources']
    BUILD.mkdir(parents=True, exist_ok=True)
    aggregate = bytearray()
    reports = []
    for module in modules:
        key, source = module['key'], module['source']
        if not record and tuple(SOURCE_FIXED_FLAGS[source]) != tuple(cpu.FLAGS):
            raise SystemExit('native batch compiler profile drift: ' + key)
        flags = [*cpu.FLAGS[:-1], *module['original_extra_flags']]
        original_path, ff_path, native_path = (BUILD / (key + suffix) for suffix in ('.historical.o', '.historical-ff.o', '.native.o'))
        run([cpu.CXX, *flags, '-DZLIB', *includes, '-x', 'c++', '-c', layout / module['original_source'], '-o', original_path])
        run([cpu.CXX, *flags, '-ffunction-sections', '-DZLIB', *includes, '-x', 'c++', '-c', layout / module['original_source'], '-o', ff_path])
        run([cpu.CXX, *cpu.FLAGS, '-c', ROOT / source, '-o', native_path])
        original, ff, native = map(ELFFile, (original_path, ff_path, native_path))
        witness_originals = {}
        if any(row.get('witness_profile') for row in module['functions']):
            stdio_path = BUILD / (key + '.stdio.o')
            run([cpu.CXX, *flags, *includes, '-x', 'c++', '-c', layout / module['original_source'], '-o', stdio_path])
            witness_originals['stdio'] = ELFFile(stdio_path)
        actual_functions = [s for s in native.symbols if s.info & 15 == 2 and s.size]
        if {(s.name, s.size, s.info >> 4) for s in actual_functions} != {(s['symbol'], s['size'], s['binding']) for s in module['functions']}:
            raise SystemExit('native batch function inventory drift: ' + key)
        if imports(native) != set(module['native_providers']) or imports(ff) != set(module['reference_providers']):
            raise SystemExit('native batch import inventory drift: ' + key)
        if any(value == 0 for value in module['native_providers'].values()):
            raise SystemExit('unproved comparison placeholder in native providers: ' + key)
        for row in module['functions']:
            old_original = witness_originals[row['witness_profile']] if row.get('witness_profile') else original
            old = old_original.find_symbol(row['symbol'])
            witness = row['witness']
            raw = old_original.symbol_bytes(old, old.size)
            if witness.get('symbol') != row['symbol']:
                if row.get('historical_symbol_alias') != witness.get('symbol'):
                    raise SystemExit('undeclared inherited symbol alias')
                if not row.get('target_listing') and row.get('abi_alias') != 'original STREAM void* and legacy FILE* descriptor functions have identical complete code and pointer calling ABI':
                    raise SystemExit('unreviewed historical symbol alias')
            if digest(raw) != witness['raw_sha256'] or old.size != row['size']:
                raise SystemExit('original source function drift: ' + row['symbol'])
            if witness.get('derived_from_whole_window'):
                whole = next(w for w in windows if w['name'] == witness['derived_from_whole_window'])
                raw_section = section_bytes(original, '.text')
                if digest(raw_section) != whole['raw_sha256'] or len(raw_section) != whole['size'] or row['address'] != whole['address'] + old.value:
                    raise SystemExit('frozen complete module window drift: ' + key)
            elif witness.get('matching_ledger'):
                path = ROOT / witness['matching_ledger']
                rows = list(csv.DictReader(path.open(), delimiter='\t'))
                matches = [r for r in rows if r.get('object_symbol') == old.name and r.get('result') == 'MATCH' and int(r['address'], 0) == row['address'] and int(r['object_size']) == row['size']]
                if len(matches) != 1 or digest(path.read_bytes()) != row['witness_ledger_sha256'] or normalized(old_original, old) != normalized(native, native.find_symbol(old.name)):
                    raise SystemExit('inherited weak-helper matching witness drift')
            elif not any(w == witness for w in windows):
                raise SystemExit('frozen complete function window drift: ' + row['symbol'])
        placeholders = {name for name, value in module['reference_providers'].items() if value == 0}
        for row in module['functions']:
            old = ff.find_symbol(row['symbol'])
            if any(symbol.name in placeholders for _, symbol in relocations(ff, ff.sections[old.section_index].name)):
                raise SystemExit('comparison placeholder referenced by selected code')
        reference_ld, native_ld = BUILD / (key + '.reference.ld'), BUILD / (key + '.native.ld')
        reference_ld.write_text(script(module, True))
        native_ld.write_text(script(module))
        reference_path, linked_path = BUILD / (key + '.reference.elf'), BUILD / (key + '.linked.elf')
        run([cpu.CXX.with_name('ee-ld'), '-EL', '-T', reference_ld, ff_path, '-o', reference_path])
        run([cpu.CXX.with_name('ee-ld'), '-EL', '-T', native_ld, native_path, '-o', linked_path])
        reference, linked = ELFFile(reference_path), ELFFile(linked_path)
        rows = []
        for row in module['functions']:
            name = row['symbol']
            old_original = witness_originals[row['witness_profile']] if row.get('witness_profile') else original
            old, local, ref, placed = (e.find_symbol(name) for e in (old_original, native, reference, linked))
            raw = linked.symbol_bytes(placed, placed.size)
            if (placed.value != row['address'] or ref.value != row['address']
                    or placed.size != row['size'] or ref.size != row['size']
                    or normalized(old_original, old) != normalized(native, local)
                    or raw != reference.symbol_bytes(ref, ref.size)):
                raise SystemExit('complete linked native function mismatch: ' + name)
            captured = dict(address=f'0x{placed.value:08x}', symbol=name, size=str(placed.size), source_file=source,
                binding=str(local.info >> 4), historical_raw_sha256=digest(old_original.symbol_bytes(old, old.size)),
                isolated_raw_sha256=digest(native.symbol_bytes(local, local.size)), normalized_sha256=digest(normalized(native, local)),
                linked_sha256=digest(raw), proof_level='linked-historical-reference')
            if row.get('target_listing'):
                listing = ROOT / row['target_listing']
                if digest(listing.read_bytes()) != row['target_listing_sha256']:
                    raise SystemExit('complete target instruction listing drift')
                words = {int(m[1], 16): bytes.fromhex(m[2]) for m in re.finditer(r'^\s*([0-9a-f]+): ((?:[0-9a-f]{2} ){3}[0-9a-f]{2})', listing.read_text(), re.M)}
                target = b''.join(words[row['address'] + offset] for offset in range(0, row['size'], 4))
                if raw != target:
                    raise SystemExit('complete linked ambiguous DSP target mismatch')
            rows.append(captured)
            aggregate.extend(raw)
        ledger = ROOT / module['ledger']
        if record:
            with ledger.open('w', newline='') as stream:
                writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter='\t', lineterminator='\n')
                writer.writeheader()
                writer.writerows(rows)
            module['ledger_sha256'] = digest(ledger.read_bytes())
        elif rows != list(csv.DictReader(ledger.open(), delimiter='\t')) or digest(ledger.read_bytes()) != module['ledger_sha256']:
            raise SystemExit('native batch exact ledger drift: ' + key)
        objects = [s for s in native.symbols if s.info & 15 == 1 and s.size and s.section_index]
        storage = []
        for obj in objects:
            ref, placed = reference.find_symbol(obj.name), linked.find_symbol(obj.name)
            if obj.size != ref.size or placed.value != ref.value or linked.symbol_bytes(placed, placed.size) != reference.symbol_bytes(ref, ref.size):
                raise SystemExit('original owned storage mismatch: ' + obj.name)
            storage.append(dict(symbol=obj.name, size=obj.size, address=placed.value, section=native.sections[obj.section_index].name,
                binding=obj.info >> 4, linked_sha256=digest(linked.symbol_bytes(placed, placed.size))))
        readonly_slices = []
        specifications = module.get('readonly_sections', [dict(section='.rodata', address=module['rodata'] + module['rodata_shift'], reference_offset=module['rodata_shift'])])
        for specification in specifications:
            raw = section_bytes(linked, specification['section'])
            offset = specification['reference_offset']
            actual_section = next((s for s in linked.sections if s.name == specification['section']), None)
            if raw and (actual_section.address != specification['address'] or raw != section_bytes(reference, '.rodata')[offset:offset + len(raw)]):
                raise SystemExit('retained complete readonly slice mismatch: ' + key)
            readonly_slices.append(dict(**specification, size=len(raw), sha256=digest(raw)))
        actual_ro_sections = {s.name for s in native.sections if s.size and s.name.startswith('.rodata')}
        claimed_ro_sections = {r['section'] for r in readonly_slices if r['size']}
        if actual_ro_sections != claimed_ro_sections:
            raise SystemExit('unclaimed native readonly section')
        raw_rodata = b''.join(section_bytes(linked, r['section']) for r in readonly_slices)
        if record:
            module['readonly_slices'] = readonly_slices
        elif readonly_slices != module['readonly_slices']:
            raise SystemExit('native readonly placement/slice inventory drift')
        geometry = {s.name: s.size for s in native.sections if s.name.startswith('.bss')}
        if record:
            module['objects'], module['bss_geometry'] = storage, geometry
            module['rodata_size'], module['rodata_sha256'] = len(raw_rodata), digest(raw_rodata)
        elif storage != module['objects'] or geometry != module['bss_geometry'] or len(raw_rodata) != module['rodata_size'] or digest(raw_rodata) != module['rodata_sha256']:
            raise SystemExit('native batch owned storage/readonly geometry drift: ' + key)
        if not record:
            owned = list(csv.DictReader((ROOT / 'analysis/source_tree/defined_symbol_ownership.tsv').open(), delimiter='\t'))
            actual = {(r['symbol'], r['binding'], r['section_class'], int(r['size_hex'], 0)) for r in owned if r['source'] == source and int(r['size_hex'], 0)}
            wanted = {(s.name, 'global' if s.info >> 4 in (1, 2) else 'local', 'weak-text' if s.info >> 4 == 2 else 'text', s.size) for s in actual_functions}
            wanted |= {(s.name, 'global' if s.info >> 4 in (1, 2) else 'local', 'rodata' if native.sections[s.section_index].name.startswith('.rodata') else 'data', s.size) for s in objects}
            if actual != wanted:
                raise SystemExit('canonical native batch ownership drift: ' + key)
        reports.append(dict(module=key, routines=len(rows), instruction_bytes=sum(int(r['size']) for r in rows), owned_storage_bytes=sum(s.size for s in objects), readonly_bytes=len(raw_rodata)))
    if len(aggregate) != 84756 or sum(r['routines'] for r in reports) != 151:
        raise SystemExit('complete native batch inventory drift')
    result = dict(modules=reports, routines=151, instruction_bytes=len(aggregate), linked_code_sha256=digest(aggregate),
        proof_level='linked-historical-reference', fresh_private_elf=False, replacement_image=False)
    if record:
        CONFIG.write_text(json.dumps(modules, indent=2) + '\n')
        path = Path(__file__)
        path.write_text(re.sub(r"CONFIG_SHA256 = '[0-9a-f]{64}'", "CONFIG_SHA256 = " + repr(digest(CONFIG.read_bytes())), path.read_text(), count=1))
    (BUILD / 'report.json').write_text(json.dumps(result, indent=2) + '\n')
    print('native batch: MATCH 151/151 routines, 84756/84756 complete linked historical instruction bytes; owned data and readonly slices checked separately')


if __name__ == '__main__':
    run_batch()
