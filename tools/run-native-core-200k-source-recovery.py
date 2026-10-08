#!/usr/bin/env python3
"""Prove 206104 native instruction bytes in CPU, Super FX and rendering.

Each original module is rebuilt from the pinned public archive and checked
against the frozen complete code window. Every selected native function is
linked at its historical address and compared byte for byte, including all
relocated calls and shared-state operands. Existing interrupt/weak-helper
owners and unknown frontend callbacks are excluded. No private ELF rerun or
complete replacement-image claim is made.
"""
from __future__ import annotations
import csv
import importlib.util
import json
import re
import struct
from pathlib import Path
from build_source_tree import SOURCE_FIXED_FLAGS
from compare_elf_functions import ELFFile
ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/matching/native-core-200k-source-recovery'
CONFIG = ROOT / 'analysis/functions/native_core_206104_config.json'
CONFIG_SHA256 = '50eb8dd7ca70e8a5f5bbdf2550237239d6457ac88bc6483538326ea470bcd431'
CODE_SHA256 = '2036b0de9609af7473f6ee1b37258b4e4bff242f53f28b9bffcaf821481fca75'


def load_proof(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'tools' / ('run-' + name + '-source-recovery.py'))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def imports(elf):
    return {s.name for s in elf.symbols if s.name and s.section_index == 0}


def main():
    cpu = load_proof('native-cpu-interrupts')
    fx = load_proof('native-fxemu')
    # Rebuild original CPU/GLOBALS and Super FX objects, checking their pinned
    # archive/profile, full raw windows, state geometry and data witnesses.
    cpu.main()
    fx.main()
    run, digest, normalized, section_bytes = cpu.run, cpu.digest, cpu.normalized, cpu.section_bytes
    config_bytes = CONFIG.read_bytes()
    if digest(config_bytes) != CONFIG_SHA256:
        raise SystemExit('native core module/provider config drift')
    modules = json.loads(config_bytes)
    import sys
    sys.path.insert(0, str(ROOT / 'tools/history/research'))
    import hunt1041_v52_closure as recipe
    layout = fx.BUILD / 'profile/historical/snes9x-target-layout'
    archive = ROOT / 'build/upstream/hunt1000plus-v47-snes/source-1.41-1/snes9x-1.41-1-src'
    newlib = recipe.v47.PS2DEV / 'ps2toolchain/soft/newlib-1.10.0/newlib/libc/include'
    includes = ['-I' + str(p) for p in (fx.BUILD / 'compat', newlib, layout, layout / 'unzip', archive / 'zlib')]
    gfx_path = BUILD / 'gfx.historical.o'
    run([cpu.CXX, *cpu.FLAGS[:-1], '-DZLIB', *includes, '-x', 'c++', '-c', layout / 'GFX.CPP', '-o', gfx_path])
    windows = json.loads((ROOT / 'analysis/link_identity/code_windows.json').read_text())['result']['selected_sources']
    paths = {'cpu_opcodes': cpu.BUILD / 'CPUOPS.historical.o', 'fx_opcodes': fx.BUILD / 'fxinst.historical.o', 'gfx': gfx_path}
    results = []
    all_code = bytearray()
    for module in modules:
        key, source = module['key'], module['source']
        if SOURCE_FIXED_FLAGS[source] != cpu.FLAGS:
            raise SystemExit('native core compiler profile drift: ' + key)
        original = ELFFile(paths[key])
        text = section_bytes(original, '.text')
        rel = next(s for s in original.sections if s.name == '.rel.text')
        witness = [r for r in windows if r['address'] == module['base'] and r['size'] == len(text)]
        if (digest(text) != module['text_sha256'] or len(witness) != 1
                or witness[0]['raw_sha256'] != module['text_sha256']
                or witness[0]['relocations'] != module['text_relocations']
                or rel.size // rel.entry_size != module['text_relocations']):
            raise SystemExit('frozen complete original code window drift: ' + key)
        obj = BUILD / (key + '.native.o')
        run([cpu.CXX, *cpu.FLAGS, '-c', ROOT / source, '-o', obj])
        isolated = ELFFile(obj)
        if any(section.size for section in isolated.sections if section.name.startswith('.bss')):
            raise SystemExit('native core unexpectedly allocates zero-fill/shared state: ' + key)
        functions = sorted((s for s in original.symbols if s.info & 15 == 2 and s.size
                            and original.sections[s.section_index].name == '.text'
                            and s.name not in module['excluded']), key=lambda s: s.value)
        native = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
        if (len(functions) != module['count'] or sum(s.size for s in functions) != module['instruction_bytes']
                or {s.name for s in native} != {s.name for s in functions}
                or sum(s.size for s in native) != module['instruction_bytes']):
            raise SystemExit('native core exact function inventory drift: ' + key)
        providers = module['native_providers']
        if imports(isolated) != set(providers) or imports(original) != set(module['reference_providers']) | set(module['unselected_comparison_placeholders']):
            raise SystemExit('native core provider/import roster drift: ' + key)
        # Zero bindings exist only to link the excluded frontend functions in
        # the comparison reference. Enforce that selected instructions and
        # selected rodata never reference those names; their bytes are not claimed.
        missing = set(module['unselected_comparison_placeholders'])
        for section in original.sections:
            if section.name not in ('.rel.text', '.rel.rodata'):
                continue
            for offset in range(section.offset, section.offset + section.size, section.entry_size):
                position, info = struct.unpack_from('<II', original.data, offset)
                if original.symbols[info >> 8].name in missing:
                    if section.name == '.rel.rodata' or any(old.value <= position < old.value + old.size for old in functions):
                        raise SystemExit('unselected callback leaks into native proof')
        reference_providers = {**module['reference_providers'], **{name: 0 for name in missing}}
        reference_script = BUILD / (key + '.historical.ld')
        weak = module['weak']
        reference_script.write_text(''.join(f'PROVIDE({name} = {address:#x});\n' for name, address in reference_providers.items())
            + 'SECTIONS {\n' + f".text {module['base']:#x} : {{ *(.text) }}\n"
            + (f".weak {weak[1]:#x} : {{ *(.gnu.linkonce.t.{weak[0]}) }}\n" if weak else '')
            + f".data {module['data']:#x} : {{ *(.data*) }}\n"
            + (f".rodata {module['rodata']:#x} : {{ *(.rodata*) }}\n" if module['rodata'] else '')
            + '/DISCARD/ : { *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n')
        reference_path = BUILD / (key + '.historical.elf')
        run([cpu.CXX.with_name('ee-ld'), '-EL', '-T', reference_script, paths[key], '-o', reference_path])
        script = BUILD / (key + '.native.ld')
        script.write_text(''.join(f'PROVIDE({name} = {address:#x});\n' for name, address in providers.items())
            + 'SECTIONS {\n' + ''.join(f".text.{i} {module['base'] + old.value:#x} : {{ *(.text.{old.name}) }}\n" for i, old in enumerate(functions))
            + f".data {module['data']:#x} : {{ *(.data*) }}\n"
            + (f".rodata {module['rodata']:#x} : {{ *(.rodata*) }}\n" if module['rodata'] else '')
            + '/DISCARD/ : { *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n')
        linked_path = BUILD / (key + '.native.elf')
        run([cpu.CXX.with_name('ee-ld'), '-EL', '-T', script, obj, '-o', linked_path])
        reference, linked = ELFFile(reference_path), ELFFile(linked_path)
        rows, aggregate = [], bytearray()
        for old in functions:
            local, placed, ref = (e.find_symbol(old.name) for e in (isolated, linked, reference))
            raw = linked.symbol_bytes(placed, placed.size)
            if (local.size != old.size or placed.size != old.size or ref.size != old.size
                    or normalized(original, old) != normalized(isolated, local)
                    or placed.value != old.value + module['base'] or ref.value != placed.value
                    or old.info >> 4 != local.info >> 4
                    or raw != reference.symbol_bytes(ref, ref.size)):
                raise SystemExit('full linked native instruction mismatch: ' + key + ':' + old.name)
            rows.append(dict(address=f'0x{placed.value:08x}', symbol=old.name, size=str(old.size),
                source_file=source, binding='global' if old.info >> 4 == 1 else 'local',
                historical_raw_sha256=digest(original.symbol_bytes(old, old.size)),
                isolated_raw_sha256=digest(isolated.symbol_bytes(local, local.size)),
                normalized_sha256=digest(normalized(isolated, local)), linked_sha256=digest(raw),
                relocations=str(len(isolated.relocation_ranges(local, local.size))), proof_level='linked-historical-reference'))
            aggregate.extend(raw)
        ledger = ROOT / module['ledger']
        with ledger.open(newline='') as stream:
            captured = list(csv.DictReader(stream, delimiter='\t'))
        if rows != captured or digest(ledger.read_bytes()) != module['ledger_sha256'] or digest(aggregate) != module['linked_code_sha256']:
            raise SystemExit('native core function ledger/digest drift: ' + key)
        objects = [s for s in isolated.symbols if s.info & 15 == 1 and s.size]
        wanted = module['objects']
        if {(s.name, s.size, isolated.sections[s.section_index].name) for s in objects} != {(r['symbol'], r['size'], r['section']) for r in wanted}:
            raise SystemExit('native core owned storage inventory drift: ' + key)
        for item in wanted:
            name = item['symbol']
            old, placed = reference.find_symbol(name), linked.find_symbol(name)
            raw = linked.symbol_bytes(placed, placed.size)
            if (placed.value != item['address'] or placed.size != item['size']
                    or raw != reference.symbol_bytes(old, old.size) or digest(raw) != item['linked_sha256']):
                raise SystemExit('native core dispatch/font/static data mismatch: ' + name)
        if module['rodata']:
            raw = section_bytes(linked, '.rodata')
            if raw != section_bytes(reference, '.rodata') or digest(raw) != module['rodata_linked_sha256']:
                raise SystemExit('native core complete rodata mismatch: ' + key)
        if key in ('gfx', 'fx_opcodes'):
            frozen = json.loads((ROOT / 'analysis/link_identity/window11_rodata.json').read_text())['source_sections']
            name = 'gfx' if key == 'gfx' else 'fxinst'
            target = next(row for row in frozen if row['name'] == name)
            historical_rodata = section_bytes(original, '.rodata')
            if (target['address'] != module['rodata'] or target['size'] != len(historical_rodata)
                    or digest(historical_rodata) != target['raw_sha256']
                    or module['rodata_linked_sha256'] != target['linked_sha256']):
                raise SystemExit('frozen complete target-linked rodata witness drift: ' + key)
        if key == 'fx_opcodes':
            raw = section_bytes(linked, '.data')
            frozen = json.loads((ROOT / 'analysis/link_identity/window36_data.json').read_text())['source_sections']
            target = next(r for r in frozen if r['name'] == 'fxinst_data')
            if len(raw) != 6672 or digest(raw) != target['linked_sha256']:
                raise SystemExit('complete Super FX frozen target-linked data mismatch')
        with (ROOT / 'analysis/source_tree/defined_symbol_ownership.tsv').open(newline='') as stream:
            owned = [r for r in csv.DictReader(stream, delimiter='\t') if r['source'] == source and int(r['size_hex'], 0)]
        expected = {(r['symbol'], r['binding'], 'text', int(r['size'])) for r in rows}
        expected |= {(s.name, 'global' if s.info >> 4 == 1 else 'local', 'rodata' if isolated.sections[s.section_index].name.startswith('.rodata') else 'data', s.size) for s in objects}
        if {(r['symbol'], r['binding'], r['section_class'], int(r['size_hex'], 0)) for r in owned} != expected:
            raise SystemExit('canonical native core ownership drift: ' + key)
        all_code.extend(aggregate)
        results.append(dict(module=key, routines=len(rows), instruction_bytes=len(aggregate), linked_code_sha256=digest(aggregate)))
    if len(all_code) != 206104 or digest(all_code) != CODE_SHA256:
        raise SystemExit('complete 206104-byte native core aggregate drift')
    result = dict(modules=results, routines=1094, instruction_bytes=206104, linked_code_sha256=digest(all_code), proof_level='linked-historical-reference', fresh_private_elf=False)
    (BUILD / 'report.json').write_text(json.dumps(result, indent=2) + '\n')
    print('native core: MATCH 1094/1094 routines, 206104/206104 complete linked historical instruction bytes; dispatch/font/rodata checked separately')


if __name__ == '__main__':
    main()
