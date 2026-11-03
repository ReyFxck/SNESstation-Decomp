#!/usr/bin/env python3
"""
Fix readiness file by replacing _recovered source file names with canonical names,
using address-based mapping from defined ownership.
"""

import csv
import sys
import re

def load_defined_ownership(defined_map_path):
    """Load defined symbol ownership map to get canonical source for each address."""
    # Map address -> list of (symbol, source)
    address_to_sources = {}
    with open(defined_map_path, 'r') as f:
        reader = csv.DictReader(f, delimiter='\t')
        for row in reader:
            symbol = row['symbol']
            source = row['source']
            binding = row['binding']
            section_class = row['section_class']
            # Only consider global text symbols
            if binding == 'global' and section_class in {'text', 'weak-text'}:
                # Extract address from symbol name
                match = re.search(r'([0-9a-fA-F]{8})$', symbol)
                if match:
                    addr = f"0x{match.group(1).lower()}"
                    if addr not in address_to_sources:
                        address_to_sources[addr] = []
                    address_to_sources[addr].append((symbol, source))
    return address_to_sources

def load_canonical_sources(manifest_path):
    """Load set of canonical source files from manifest."""
    canonical = set()
    with open(manifest_path, 'r') as f:
        for line in f:
            parts = line.strip().split('\t')
            if len(parts) >= 2:
                canonical.add(parts[1])
    return canonical

def fix_readiness(input_file, output_file, address_sources, canonical_sources):
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
    address_idx = header.index('address')

    fixed_count = 0
    new_rows = [header]

    for row in data_rows:
        if len(row) <= max(source_files_idx, address_idx):
            new_rows.append(row)
            continue

        addr = row[address_idx]
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

            # For progress files that were split, try to find canonical sources
            # by looking up the address in defined ownership
            if 'progress' in src and src.endswith('_recovered.c'):
                if addr in address_sources:
                    # Get all canonical sources for this address
                    canonical_for_addr = [src for _, src in address_sources[addr]]
                    # Add all unique canonical sources
                    for csrc in canonical_for_addr:
                        if csrc not in new_sources:
                            new_sources.append(csrc)
                            fixed_count += 1
                            print(f"  {src} -> {csrc} (via address {addr})")
                else:
                    print(f"  WARNING: No defined ownership for address {addr}")
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

    print("Loading defined symbol ownership by address...")
    address_sources = load_defined_ownership(defined_map)
    print(f"Loaded {len(address_sources)} addresses with defined symbols")

    print("Loading canonical sources...")
    canonical_sources = load_canonical_sources(manifest_file)
    print(f"Loaded {len(canonical_sources)} canonical sources")

    print("Fixing readiness file...")
    fix_readiness(readiness_file, output_file, address_sources, canonical_sources)