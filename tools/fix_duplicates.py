#!/usr/bin/env python3
"""Fix duplicate entries in translation_units.tsv and build_source_tree.py"""

import re

# Fix translation_units.tsv
tsv_path = '/root/SNESstation-Decomp/analysis/source_tree/translation_units.tsv'

with open(tsv_path, 'r') as f:
    lines = f.readlines()

header = lines[0]
seen = set()
new_lines = [header]

for line in lines[1:]:
    parts = line.split('\t')
    if len(parts) >= 2:
        source = parts[1].strip()
        # Check if this is a duplicate of the snes9x modules that already existed
        key = source
        if key in seen and key in [
            'src/snes9x/snes9x_apu_io.c',
            'src/snes9x/snes9x_bitplane.c',
            'src/snes9x/snes9x_palette.c',
            'src/snes9x/snes9x_path.c',
        ]:
            print(f"Removing duplicate: {line.strip()}")
            continue
        seen.add(key)
    new_lines.append(line)

# Renumber all entries
for i, line in enumerate(new_lines[1:], 1):
    parts = line.split('\t')
    parts[0] = str(i)
    new_lines[i] = '\t'.join(parts)

with open(tsv_path, 'w') as f:
    f.writelines(new_lines)

print(f"Fixed translation_units.tsv - now {len(new_lines) - 1} entries")

# Fix build_source_tree.py
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
    # Process the dict entries to remove duplicates
    dict_lines = lines[dict_start:dict_end+1]

    # Track seen entries within the dict
    seen_in_dict = set()
    new_dict_lines = []

    for line in dict_lines:
        # Check if this line contains one of the duplicate snes9x modules
        duplicate_found = False
        for dup in [
            "src/snes9x/snes9x_apu_io.c",
            "src/snes9x/snes9x_bitplane.c",
            "src/snes9x/snes9x_palette.c",
            "src/snes9x/snes9x_path.c",
        ]:
            if dup in line:
                if dup in seen_in_dict:
                    print(f"Removing duplicate in build_source_tree.py: {line.strip()}")
                    duplicate_found = True
                    break
                seen_in_dict.add(dup)

        if not duplicate_found:
            new_dict_lines.append(line)

    # Also remove progress* entries
    progress_pattern = re.compile(r"^\s*'src/(ps2|snes9x)/progress[^']+'\s*:.*?,$")
    new_dict_lines = [l for l in new_dict_lines if not progress_pattern.match(l)]

    # Clean up empty lines
    cleaned = []
    prev_empty = False
    for l in new_dict_lines:
        if l.strip() == '':
            if not prev_empty:
                cleaned.append(l)
                prev_empty = True
        else:
            cleaned.append(l)
            prev_empty = False

    new_lines = lines[:dict_start] + cleaned + lines[dict_end+1:]
    content = '\n'.join(new_lines)

    with open(build_path, 'w') as f:
        f.write(content)

    print("Fixed build_source_tree.py - removed duplicates and progress entries")
else:
    print("ERROR: Could not find SOURCE_FIXED_FLAGS dict boundaries")