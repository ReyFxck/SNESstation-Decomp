#!/usr/bin/env python3
"""
Script to update build system manifests for ALL new canonical modules.
"""

import os
import re

# All the new canonical PS2 modules (snes_*)
PS2_MODULES = [
    'snes_cdfs.c',
    'snes_write.c',
    'snes_sdd1_io.c',
    'snes_transfer.c',
    'snes_softdouble_add.c',
    'snes_state_helpers.c',
    'snes_softdiv.c',
    'snes_file_buffer.c',
    'snes_core_blend.c',
    'snes_register_read.c',
    'snes_frontend_init.c',
    'snes_frontend_panel.c',
    'snes_channel_state.c',
    'snes_ppu_bounds.c',
]

# All the new canonical SNES9X modules (snes9x_*)
SNES9X_MODULES = [
    'snes9x_io.c',
    'snes9x_map.c',
    'snes9x_cheat.c',
    'snes9x_memory.c',
    'snes9x_code_stream.c',
    'snes9x_table.c',
    'snes9x_palette.c',
    'snes9x_state.c',
    'snes9x_transform.c',
    'snes9x_slot_runtime.c',
    'snes9x_apu_io.c',
    'snes9x_gate.c',
    'snes9x_palette_expand.c',
    'snes9x_ppu_reset.c',
    'snes9x_map_window.c',
    'snes9x_rotate.c',
    'snes9x_runtime_block.c',
    'snes9x_path.c',
    'snes9x_bitplane.c',
    'snes9x_frontend_table.c',
    'snes9x_multibyte.c',
    'snes9x_ppu_frontend.c',
    'snes9x_record.c',
    'snes9x_floatdidf.c',
    'snes9x_days_in_month.c',
    'snes9x_stage.c',
    'snes9x_record_ram.c',
    'snes9x_color_blend.c',
    'snes9x_map_cursor.c',
    'snes9x_controller_mask.c',
    'snes9x_weekday.c',
]

# Standard PS2 C flags (from existing entries)
PS2_C_FLAGS = (
    '-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing',
    '-fno-common', '-ffreestanding', '-fno-builtin', '-fshort-double',
    '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900',
    '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST',
    '-DALIGN_DWORD', '-DCODE_PLATFORM=3', '-Iinclude',
    '-Iinclude/ee_stage1_compat', '-ffunction-sections'
)

# Standard SNES9X C flags (from existing entries)
SNES9X_C_FLAGS = (
    '-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing',
    '-fno-common', '-ffreestanding', '-fno-builtin', '-fshort-double',
    '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900',
    '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST',
    '-DALIGN_DWORD', '-DCODE_PLATFORM=3', '-Iinclude',
    '-Iinclude/ee_stage1_compat', '-ffunction-sections'
)


def update_translation_units_tsv():
    """Update translation_units.tsv to replace progress* entries with new modules."""
    tsv_path = '/root/SNESstation-Decomp/analysis/source_tree/translation_units.tsv'

    with open(tsv_path, 'r') as f:
        lines = f.readlines()

    header = lines[0]
    new_lines = [header]

    # Filter out progress* entries
    progress_count = 0
    for line in lines[1:]:
        if 'progress' in line and ('_recovered.o' in line or 'canonical' in line) and 'progress28' not in line:
            progress_count += 1
            continue
        new_lines.append(line)

    print(f"Removed {progress_count} progress* entries")

    # Find the last PS2 entry to insert new snes modules before SNES9X entries
    insert_idx = len(new_lines)
    for i, line in enumerate(new_lines):
        if line.startswith('src/snes9x/'):
            insert_idx = i
            break

    # Determine next order number
    last_order = 0
    for line in new_lines[1:]:
        parts = line.split('\t')
        if parts[0].isdigit():
            last_order = max(last_order, int(parts[0]))

    # Add new PS2 snes modules
    for module in PS2_MODULES:
        last_order += 1
        obj_name = module.replace('.c', '.o')
        new_line = f"{last_order}\tsrc/ps2/{module}\tps2\tc\tcanonical\tps2/{obj_name}\t\tCanonical recovered translation unit.\n"
        new_lines.insert(insert_idx, new_line)
        insert_idx += 1

    # Add new SNES9X modules
    for module in SNES9X_MODULES:
        last_order += 1
        obj_name = module.replace('.c', '.o')
        new_line = f"{last_order}\tsrc/snes9x/{module}\tsnes9x\tc\tcanonical\tsnes9x/{obj_name}\t\tCanonical recovered translation unit.\n"
        new_lines.insert(insert_idx, new_line)
        insert_idx += 1

    # Renumber all entries
    for i, line in enumerate(new_lines[1:], 1):
        parts = line.split('\t')
        parts[0] = str(i)
        new_lines[i] = '\t'.join(parts)

    with open(tsv_path, 'w') as f:
        f.writelines(new_lines)

    print(f"Added {len(PS2_MODULES)} PS2 modules to translation_units.tsv")
    print(f"Added {len(SNES9X_MODULES)} SNES9X modules to translation_units.tsv")
    print(f"Total entries: {len(new_lines) - 1}")


def update_build_source_tree():
    """Update build_source_tree.py SOURCE_FIXED_FLAGS with new modules."""
    build_path = '/root/SNESstation-Decomp/tools/build_source_tree.py'

    with open(build_path, 'r') as f:
        content = f.read()

    # Find SOURCE_FIXED_FLAGS dict
    lines = content.split('\n')
    in_dict = False
    dict_start = -1
    dict_end = -1
    brace_count = 0

    for i, line in enumerate(lines):
        if 'SOURCE_FIXED_FLAGS = {' in line:
            in_dict = True
            dict_start = i
            brace_count = 1
        elif in_dict:
            brace_count += line.count('{')
            brace_count -= line.count('}')
            if brace_count == 0:
                dict_end = i
                break

    if dict_start >= 0 and dict_end >= 0:
        # Build new entries
        new_entries = []
        for module in PS2_MODULES:
            entry = f"    'src/ps2/{module}': {PS2_C_FLAGS},"
            new_entries.append(entry)
        for module in SNES9X_MODULES:
            entry = f"    'src/snes9x/{module}': {SNES9X_C_FLAGS},"
            new_entries.append(entry)

        # Insert new entries before the closing brace
        new_lines = lines[:dict_end] + [''] + new_entries + [''] + lines[dict_end:]
        content = '\n'.join(new_lines)

        # Also remove progress* entries from SOURCE_FIXED_FLAGS
        progress_pattern = r"^\s*'src/(ps2|snes9x)/progress[^']+'\s*:.*?,$"
        content = re.sub(progress_pattern, '', content, flags=re.MULTILINE)
        # Clean up empty lines
        content = re.sub(r'\n\s*\n\s*\n', '\n\n', content)

        with open(build_path, 'w') as f:
            f.write(content)

        print(f"Added {len(PS2_MODULES)} PS2 + {len(SNES9X_MODULES)} SNES9X modules to build_source_tree.py SOURCE_FIXED_FLAGS")
        print("Removed progress* entries from build_source_tree.py")
    else:
        print("ERROR: Could not find SOURCE_FIXED_FLAGS dict boundaries")


def main():
    print("Updating build system manifests for all new canonical modules...")
    update_translation_units_tsv()
    update_build_source_tree()
    print("Done!")


if __name__ == '__main__':
    main()