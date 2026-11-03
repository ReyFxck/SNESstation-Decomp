#!/usr/bin/env python3
"""
Fix readiness file by replacing _recovered source file names with canonical names.
"""

import csv
import sys

def load_defined_ownership(defined_map_path):
    """Load defined symbol ownership map to get canonical source for each symbol."""
    symbol_to_source = {}
    with open(defined_map_path, 'r') as f:
        reader = csv.DictReader(f, delimiter='\t')
        for row in reader:
            symbol = row['symbol']
            source = row['source']
            if symbol not in symbol_to_source:
                symbol_to_source[symbol] = source
    return symbol_to_source

def load_canonical_sources(manifest_path):
    """Load set of canonical source files from manifest."""
    canonical = set()
    with open(manifest_path, 'r') as f:
        for line in f:
            parts = line.strip().split('\t')
            if len(parts) >= 2:
                canonical.add(parts[1])
    return canonical

def fix_readiness(input_file, output_file, defined_ownership, canonical_sources):
    """Fix the readiness CSV file."""
    with open(input_file, 'r') as f:
        reader = csv.reader(f)
        rows = list(reader)

    if not rows:
        return

    header = rows[0]
    data_rows = rows[1:]

    # Columns: address,name,area,manifest_status,source_form,explicit_src_reference,source_files,matching_status
    source_files_idx = header.index('source_files')
    name_idx = header.index('name')

    fixed_count = 0
    new_rows = [header]

    for row in data_rows:
        if len(row) <= source_files_idx:
            new_rows.append(row)
            continue

        old_source_files = row[source_files_idx]
        if not old_source_files:
            new_rows.append(row)
            continue

        # Split by semicolon
        sources = old_source_files.split(';')
        new_sources = []

        for src in sources:
            # Simple rename: remove _recovered suffix
            if src.endswith('_recovered.c'):
                candidate = src.replace('_recovered.c', '.c')
                if candidate in canonical_sources:
                    new_sources.append(candidate)
                    fixed_count += 1
                    print(f"  {src} -> {candidate}")
                    continue

            # For progress files that were split, try to find the canonical source
            # by looking up the symbol name in defined_ownership
            if 'progress' in src and src.endswith('_recovered.c'):
                # The row has a name column - use that to find the canonical source
                symbol_name = row[name_idx] if len(row) > name_idx else None
                if symbol_name and symbol_name in defined_ownership:
                    canonical_source = defined_ownership[symbol_name]
                    new_sources.append(canonical_source)
                    fixed_count += 1
                    print(f"  {src} -> {canonical_source} (via symbol {symbol_name})")
                    continue

                # If no exact match, keep original (will cause issues but we can't fix)
                print(f"  WARNING: Could not map progress file: {src} for symbol {row[name_idx] if len(row) > name_idx else 'unknown'}")
                new_sources.append(src)
                continue

            # Keep as-is
            new_sources.append(src)

        # Remove duplicates while preserving order
        seen = set()
        unique_sources = []
        for s in new_sources:
            if s not in seen:
                seen.add(s)
                unique_sources.append(s)

        row[source_files_idx] = ';'.join(unique_sources)
        new_rows.append(row)

    # Write output
    with open(output_file, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(new_rows)

    print(f"\nTotal fixes applied: {fixed_count}")

if __name__ == '__main__':
    readiness_file = '/root/SNESstation-Decomp/analysis/source_readiness.csv'
    defined_map = '/root/SNESstation-Decomp/analysis/source_tree/defined_symbol_ownership.tsv'
    manifest_file = '/root/SNESstation-Decomp/analysis/source_tree/translation_units.tsv'
    output_file = '/root/SNESstation-Decomp/analysis/source_readiness.csv'

    print("Loading defined symbol ownership...")
    defined_ownership = load_defined_ownership(defined_map)
    print(f"Loaded {len(defined_ownership)} defined symbols")

    print("Loading canonical sources...")
    canonical_sources = load_canonical_sources(manifest_file)
    print(f"Loaded {len(canonical_sources)} canonical sources")

    print("Fixing readiness file...")
    fix_readiness(readiness_file, output_file, defined_ownership, canonical_sources)