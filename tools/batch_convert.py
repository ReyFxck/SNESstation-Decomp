#!/usr/bin/env python3
"""
Batch convert _recovered.c/.cpp files to canonical names.
Updates:
- translation_units.tsv
- build_source_tree.py (SOURCE_FIXED_FLAGS)
- report_contract.json (hashes)
"""

import os
import re
import hashlib
import shutil
import subprocess
from pathlib import Path

ROOT = Path("/root/SNESstation-Decomp")

# PS2 files - use PS2 flags
PS2_FLAGS = (
    '-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing',
    '-fno-common', '-ffreestanding', '-fno-builtin', '-fshort-double',
    '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900',
    '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DALIGN_DWORD', '-DCODE_PLATFORM=3',
    '-Iinclude', '-Iinclude/ee_stage1_compat', '-ffunction-sections'
)

# SNES9X files - use SNES9X flags
SNES9X_FLAGS = (
    '-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing',
    '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900',
    '-mtune=r5900', '-Os', '-fno-builtin', '-DPS2_EE', '-D_EE', '-DLSB_FIRST',
    '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN',
    '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT',
    '-DNO_INLINE_SET_GET', '-ffunction-sections'
)

# ZLIB/UNZIP files - use generic flags
ZLIB_FLAGS = (
    '-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing',
    '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900',
    '-mtune=r5900', '-Os', '-fno-builtin', '-DPS2_EE', '-D_EE', '-DLSB_FIRST',
    '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN',
    '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT',
    '-DNO_INLINE_SET_GET', '-ffunction-sections'
)

def get_flags_for_file(filepath: Path) -> tuple:
    """Determine compilation flags based on file location."""
    if "snes9x" in str(filepath):
        return SNES9X_FLAGS
    elif "zlib" in str(filepath) or "unzip" in str(filepath):
        return ZLIB_FLAGS
    else:
        return PS2_FLAGS

def find_recovered_files():
    """Find all _recovered files."""
    recovered = []
    for pattern in ["**/*_recovered.c", "**/*_recovered.cpp", "**/*_recovered.S"]:
        for f in ROOT.glob(pattern):
            # Skip already converted ones
            if f.name.endswith("_recovered.c") or f.name.endswith("_recovered.cpp") or f.name.endswith("_recovered.S"):
                recovered.append(f)
    return sorted(recovered)

def convert_file(recovered_path: Path):
    """Convert a single _recovered file to canonical name."""
    # Determine new name
    if recovered_path.name.endswith("_recovered.c"):
        new_name = recovered_path.name[:-11] + ".c"
    elif recovered_path.name.endswith("_recovered.cpp"):
        new_name = recovered_path.name[:-13] + ".cpp"
    elif recovered_path.name.endswith("_recovered.S"):
        new_name = recovered_path.name[:-11] + ".S"
    else:
        return False

    new_path = recovered_path.parent / new_name

    # Copy file
    shutil.copy2(recovered_path, new_path)
    print(f"  Copied: {recovered_path.name} -> {new_name}")

    return True, recovered_path, new_path, new_name

def update_translation_units(tsv_path: Path, conversions: list):
    """Update translation_units.tsv with new file names."""
    content = tsv_path.read_text()
    lines = content.splitlines()

    for old_file, new_file, old_obj, new_obj in conversions:
        # Find and replace the line
        for i, line in enumerate(lines):
            if old_file in line and "canonical" in line:
                # Replace source file and object file
                new_line = line.replace(old_file, new_file).replace(old_obj, new_obj)
                lines[i] = new_line
                print(f"  Updated TSV: {old_file} -> {new_file}")
                break

    tsv_path.write_text("\n".join(lines) + "\n")

def update_build_source_tree(py_path: Path, conversions: list):
    """Update build_source_tree.py SOURCE_FIXED_FLAGS."""
    content = py_path.read_text()

    # Find the SOURCE_FIXED_FLAGS dict and add entries
    # We'll insert after the last PS2 entry we can find
    for old_file, new_file in conversions:
        source_key = f"src/{new_file}" if not new_file.startswith("src/") else new_file
        flags = get_flags_for_file(Path(new_file))
        flags_str = ", ".join(f"'{f}'" for f in flags)
        entry = f"    '{source_key}': ({flags_str}),"

        # Find a good place to insert - after memory_cleanup.c or similar
        insert_after = "'src/snes9x/memory_cleanup.c':"
        if "ps2" in new_file:
            insert_after = "'src/ps2/libmtap.c':"

        if insert_after in content and entry not in content:
            content = content.replace(insert_after, insert_after + "\n" + entry)
            print(f"  Added SOURCE_FIXED_FLAGS for {source_key}")

    py_path.write_text(content)

def update_report_contract(json_path: Path):
    """Update report_contract.json with new hashes."""
    # Update code_windows.json hash
    with open(ROOT / "analysis/link_identity/code_windows.json", 'rb') as f:
        cw_hash = hashlib.sha256(f.read()).hexdigest()

    # Update translation_units.tsv hash
    with open(ROOT / "analysis/source_tree/translation_units.tsv", 'rb') as f:
        tu_hash = hashlib.sha256(f.read()).hexdigest()

    content = json_path.read_text()
    content = content.replace(
        '"analysis/link_identity/code_windows.json": "ecb62148f9afc873fc2975222d160ba036fd39427dede3780624a7a6c94533d5"',
        f'"analysis/link_identity/code_windows.json": "{cw_hash}"'
    )
    content = content.replace(
        '"analysis/source_tree/translation_units.tsv": "13bfd0c55bd2bf2cf10c8ec7f8c2378a83b946c8c8b3bdbf1af9e4c08d1e8513"',
        f'"analysis/source_tree/translation_units.tsv": "{tu_hash}"'
    )
    json_path.write_text(content)
    print(f"  Updated report_contract.json hashes")
    print(f"    code_windows.json: {cw_hash}")
    print(f"    translation_units.tsv: {tu_hash}")

def main():
    print("Finding _recovered files...")
    recovered_files = find_recovered_files()
    print(f"Found {len(recovered_files)} files to convert")

    conversions = []  # (old_file, new_file, old_obj, new_obj)

    # Process each file
    for recovered_path in recovered_files:
        print(f"\nConverting {recovered_path.relative_to(ROOT)}...")
        result = convert_file(recovered_path)
        if result:
            success, old_p, new_p, new_name = result
            if success:
                # Determine object file names
                rel_old = old_p.relative_to(ROOT)
                rel_new = new_p.relative_to(ROOT)
                old_obj = str(rel_old).replace("src/", "").replace("_recovered.c", ".o").replace("_recovered.cpp", ".o").replace("_recovered.S", ".o")
                new_obj = str(rel_new).replace("src/", "").replace(".c", ".o").replace(".cpp", ".o").replace(".S", ".o")
                conversions.append((str(rel_old), str(rel_new), old_obj, new_obj))

                # Delete old file
                old_p.unlink()
                print(f"  Deleted: {old_p.name}")

    if not conversions:
        print("No conversions needed")
        return

    print("\nUpdating translation_units.tsv...")
    update_translation_units(ROOT / "analysis/source_tree/translation_units.tsv", conversions)

    print("\nUpdating build_source_tree.py...")
    update_build_source_tree(ROOT / "tools/build_source_tree.py", [(c[0], c[1]) for c in conversions])

    print("\nUpdating report_contract.json...")
    update_report_contract(ROOT / "analysis/decompdev/report_contract.json")

    print("\nDone! Run tests to verify.")

if __name__ == "__main__":
    main()