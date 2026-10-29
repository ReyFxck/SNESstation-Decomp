#!/usr/bin/env python3
"""
Script to update build system manifests for new SNES9X canonical modules.
"""

import os
import re

# All the new canonical snes9x_* modules that need to be added
SNES9X_MODULES = [
    'snes9x_apu_io.c',
    'snes9x_bitplane.c',
    'snes9x_controller.c',
    'snes9x_datetime.c',
    'snes9x_frontend_init.c',
    'snes9x_matrix.c',
    'snes9x_memory_map.c',
    'snes9x_palette.c',
    'snes9x_path.c',
    'snes9x_renderer_gate.c',
    'snes9x_renderer_leaf.c',
    'snes9x_softfloat.c',
    'snes9x_state_machine.c',
]

# Standard SNES9X C flags (from existing entries)
SNES9X_C_FLAGS = (
    '-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing',
    '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900',
    '-mtune=r5900', '-Os', '-fno-builtin', '-DPS2_EE', '-D_EE', '-DLSB_FIRST',
    '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN',
    '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT',
    '-DNO_INLINE_SET_GET', '-ffunction-sections'
)

# SNES9X C++ flags
SNES9X_CPP_FLAGS = (
    '-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing',
    '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls',
    '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST',
    '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN',
    '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT',
    '-DNO_INLINE_SET_GET', '-ffunction-sections'
)


def update_translation_units_tsv():
    """Update translation_units.tsv to replace progress* entries with snes9x_* modules."""
    tsv_path = '/root/SNESstation-Decomp/analysis/source_tree/translation_units.tsv'

    with open(tsv_path, 'r') as f:
        lines = f.readlines()

    header = lines[0]
    new_lines = [header]

    # Filter out SNES9X progress* entries and underscore-suffixed recovered entries
    progress_count = 0
    underscore_count = 0
    for line in lines[1:]:
        if 'progress' in line and 'snes9x' in line and '_recovered.o' in line:
            progress_count += 1
            continue
        if 'snes9x/' in line and '_.c' in line and '_recovered.o' in line:
            underscore_count += 1
            continue
        new_lines.append(line)

    print(f"Removed {progress_count} SNES9X progress* entries")
    print(f"Removed {underscore_count} SNES9X underscore-suffixed entries")

    # Find the last SNES9X entry to insert new modules
    # SNES9X entries end before 'src/unzip/' or 'src/zlib/'
    insert_idx = len(new_lines)
    for i, line in enumerate(new_lines):
        if line.startswith('src/unzip/') or line.startswith('src/zlib/'):
            insert_idx = i
            break

    # Determine next order number
    last_order = 0
    for line in new_lines[1:]:
        parts = line.split('\t')
        if parts[0].isdigit():
            last_order = max(last_order, int(parts[0]))

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

    print(f"Added {len(SNES9X_MODULES)} snes9x modules to translation_units.tsv")
    print(f"Total entries: {len(new_lines) - 1}")


def update_build_source_tree():
    """Update build_source_tree.py SOURCE_FIXED_FLAGS with snes9x_* modules."""
    build_path = '/root/SNESstation-Decomp/tools/build_source_tree.py'

    with open(build_path, 'r') as f:
        content = f.read()

    # Find SOURCE_FIXED_FLAGS dict boundaries
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
        for module in SNES9X_MODULES:
            entry = f"    'src/snes9x/{module}': {SNES9X_C_FLAGS},"
            new_entries.append(entry)

        # Insert new entries before the closing brace
        new_lines = lines[:dict_end] + [''] + new_entries + [''] + lines[dict_end:]
        content = '\n'.join(new_lines)

        with open(build_path, 'w') as f:
            f.write(content)

        print(f"Added {len(SNES9X_MODULES)} snes9x modules to build_source_tree.py SOURCE_FIXED_FLAGS")
    else:
        print("ERROR: Could not find SOURCE_FIXED_FLAGS dict boundaries")

    # Remove SNES9X progress* entries from SOURCE_FIXED_FLAGS
    progress_pattern = r"^\s*'src/snes9x/progress[^']+'\s*:.*?,$"
    content = re.sub(progress_pattern, '', content, flags=re.MULTILINE)

    # Remove SNES9X underscore-suffixed entries
    underscore_pattern = r"^\s*'src/snes9x/[^']+_\.c'\s*:.*?,$"
    content = re.sub(underscore_pattern, '', content, flags=re.MULTILINE)

    # Clean up empty lines
    content = re.sub(r'\n\s*\n\s*\n', '\n\n', content)

    with open(build_path, 'w') as f:
        f.write(content)

    print("Removed SNES9X progress* and underscore-suffixed entries from build_source_tree.py")


def main():
    print("Updating SNES9X build system manifests...")
    update_translation_units_tsv()
    update_build_source_tree()
    print("Done!")


if __name__ == '__main__':
    main()