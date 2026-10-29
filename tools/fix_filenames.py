#!/usr/bin/env python3
"""
Fix the incorrectly named files from batch conversion.
Files ended up with _ suffix instead of proper names.
"""

import os
import hashlib
from pathlib import Path

ROOT = Path("/root/SNESstation-Decomp")

def fix_filenames():
    """Rename files from *__.c to *.c etc."""
    fixes = []

    # Find all files with _ suffix pattern
    for pattern in ["**/*_.c", "**/*_.cpp", "**/*_.S"]:
        for f in ROOT.glob(pattern):
            old_name = f.name
            # Remove the trailing _ before extension
            if old_name.endswith("_.c"):
                new_name = old_name[:-3] + ".c"
            elif old_name.endswith("_.cpp"):
                new_name = old_name[:-5] + ".cpp"
            elif old_name.endswith("_.S"):
                new_name = old_name[:-3] + ".S"
            else:
                continue

            new_path = f.parent / new_name
            f.rename(new_path)
            fixes.append((old_name, new_name, str(f.relative_to(ROOT)), str(new_path.relative_to(ROOT))))
            print(f"  Renamed: {old_name} -> {new_name}")

    return fixes

def update_translation_units(fixes):
    """Update translation_units.tsv with correct names."""
    tsv_path = ROOT / "analysis/source_tree/translation_units.tsv"
    content = tsv_path.read_text()

    for old_name, new_name, old_rel, new_rel in fixes:
        # Replace in TSV
        old_file = f"src/{old_rel}"
        new_file = f"src/{new_rel}"
        content = content.replace(old_file, new_file)
        print(f"  Updated TSV: {old_file} -> {new_file}")

    tsv_path.write_text(content)

def update_build_source_tree(fixes):
    """Update build_source_tree.py SOURCE_FIXED_FLAGS."""
    py_path = ROOT / "tools/build_source_tree.py"
    content = py_path.read_text()

    for old_name, new_name, old_rel, new_rel in fixes:
        # Fix SOURCE_FIXED_FLAGS keys
        old_key = f"'src/{old_rel}'"
        new_key = f"'src/{new_rel}'"
        if old_key in content:
            content = content.replace(old_key, new_key)
            print(f"  Fixed SOURCE_FIXED_FLAGS key: {old_key} -> {new_key}")

    py_path.write_text(content)

def update_report_contract():
    """Update report_contract.json with new hashes."""
    # Update code_windows.json hash
    with open(ROOT / "analysis/link_identity/code_windows.json", 'rb') as f:
        cw_hash = hashlib.sha256(f.read()).hexdigest()

    # Update translation_units.tsv hash
    with open(ROOT / "analysis/source_tree/translation_units.tsv", 'rb') as f:
        tu_hash = hashlib.sha256(f.read()).hexdigest()

    json_path = ROOT / "analysis/decompdev/report_contract.json"
    content = json_path.read_text()
    content = content.replace(
        '"analysis/link_identity/code_windows.json": "f0b406e92b6a3d3ea2494d691371bc432748772fd36e9323db200d05019e47ca"',
        f'"analysis/link_identity/code_windows.json": "{cw_hash}"'
    )
    content = content.replace(
        '"analysis/source_tree/translation_units.tsv": "ceac0edb9afcb976b5b5779277c149cc27059d9722d4cac30ddc15a02d70563b"',
        f'"analysis/source_tree/translation_units.tsv": "{tu_hash}"'
    )
    json_path.write_text(content)
    print(f"  Updated report_contract.json hashes")
    print(f"    code_windows.json: {cw_hash}")
    print(f"    translation_units.tsv: {tu_hash}")

def main():
    print("Fixing filenames...")
    fixes = fix_filenames()

    if not fixes:
        print("No fixes needed")
        return

    print("\nUpdating translation_units.tsv...")
    update_translation_units(fixes)

    print("\nUpdating build_source_tree.py...")
    update_build_source_tree(fixes)

    print("\nUpdating report_contract.json...")
    update_report_contract()

    print("\nDone!")

if __name__ == "__main__":
    main()