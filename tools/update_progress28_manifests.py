#!/usr/bin/env python3
"""
Script to update build system manifests for new canonical modules from progress28_structural_lift.c.
"""

import os
import re

# All the new canonical snes_* modules from progress28 that need to be added
PROGRESS28_MODULES = [
    'snes_startup.c',
    'snes_frontend_menu.c',
    'snes_frontend_splash.c',
    'snes_frontend_gui.c',
    'snes_frontend_options.c',
    'snes_config.c',
    'snes_frontend_main.c',
    'snes_sdd1.c',
    'snes_file_browser.c',
    'snes_hdd.c',
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


def update_translation_units_tsv():
    """Update translation_units.tsv to replace progress* entries with new snes_* modules."""
    tsv_path = '/root/SNESstation-Decomp/analysis/source_tree/translation_units.tsv'

    with open(tsv_path, 'r') as f:
        lines = f.readlines()

    header = lines[0]
    new_lines = [header]

    # Filter out progress* entries
    progress_count = 0
    for line in lines[1:]:
        if 'progress' in line and '_recovered.o' in line:
            progress_count += 1
            continue
        new_lines.append(line)

    print(f"Removed {progress_count} progress* entries")

    # Find the last PS2 entry to insert new snes modules before SNES9X entries
    # SNES9X entries start with 'src/snes9x/'
    insert_idx = len(new_lines)
    for i, line in enumerate(new_lines):
        if line.startswith('src/snes9x/'):
            insert_idx = i
            break

    # Add new snes modules
    # Determine next order number
    last_order = 0
    for line in new_lines[1:]:
        parts = line.split('\t')
        if parts[0].isdigit():
            last_order = max(last_order, int(parts[0]))

    for module in PROGRESS28_MODULES:
        last_order += 1
        obj_name = module.replace('.c', '.o')
        new_line = f"{last_order}\tsrc/ps2/{module}\tps2\tc\tcanonical\tps2/{obj_name}\t\tCanonical recovered translation unit from progress28.\n"
        new_lines.insert(insert_idx, new_line)
        insert_idx += 1

    # Renumber all entries
    for i, line in enumerate(new_lines[1:], 1):
        parts = line.split('\t')
        parts[0] = str(i)
        new_lines[i] = '\t'.join(parts)

    with open(tsv_path, 'w') as f:
        f.writelines(new_lines)

    print(f"Added {len(PROGRESS28_MODULES)} progress28 modules to translation_units.tsv")
    print(f"Total entries: {len(new_lines) - 1}")


def update_build_source_tree():
    """Update build_source_tree.py SOURCE_FIXED_FLAGS with snes_* modules."""
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
        for module in PROGRESS28_MODULES:
            entry = f"    'src/ps2/{module}': {PS2_C_FLAGS},"
            new_entries.append(entry)

        # Insert new entries before the closing brace
        new_lines = lines[:dict_end] + [''] + new_entries + [''] + lines[dict_end:]
        content = '\n'.join(new_lines)

        with open(build_path, 'w') as f:
            f.write(content)

        print(f"Added {len(PROGRESS28_MODULES)} progress28 modules to build_source_tree.py SOURCE_FIXED_FLAGS")
    else:
        print("ERROR: Could not find SOURCE_FIXED_FLAGS dict boundaries")

    # Also remove progress* entries from SOURCE_FIXED_FLAGS
    progress_pattern = r"^\s*'src/ps2/progress[^']+'\s*:.*?,$"
    content = re.sub(progress_pattern, '', content, flags=re.MULTILINE)
    # Clean up empty lines
    content = re.sub(r'\n\s*\n\s*\n', '\n\n', content)

    with open(build_path, 'w') as f:
        f.write(content)

    print("Removed progress* entries from build_source_tree.py")


def main():
    print("Updating build system manifests for progress28 modules...")
    update_translation_units_tsv()
    update_build_source_tree()
    print("Done!")


if __name__ == '__main__':
    main()