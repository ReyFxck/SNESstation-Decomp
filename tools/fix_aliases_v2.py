#!/usr/bin/env python3
"""
Fix alias files by replacing _recovered suffixes with canonical names,
but only when the canonical target exists in the manifest.
"""

import csv
import sys

def load_canonical_sources(manifest_file):
    """Load set of canonical source files from manifest."""
    canonical = set()
    with open(manifest_file, 'r') as f:
        for line in f:
            parts = line.strip().split('\t')
            if len(parts) >= 2:
                canonical.add(parts[1])  # source file path
    return canonical

def fix_aliases(input_file, output_file, canonical_sources):
    """Fix the aliases TSV file."""
    with open(input_file, 'r') as f:
        reader = csv.reader(f, delimiter='\t')
        rows = list(reader)

    if not rows:
        return

    header = rows[0]
    data_rows = rows[1:]

    # Columns: alias, target_address, status, canonical_symbol, evidence,
    # canonical_source, canonical_object, requesters, detail

    canonical_source_idx = header.index('canonical_source')
    canonical_object_idx = header.index('canonical_object')
    requesters_idx = header.index('requesters')

    fixed_count = 0
    removed_count = 0
    new_rows = [header]

    for row in data_rows:
        if len(row) <= max(canonical_source_idx, canonical_object_idx, requesters_idx):
            new_rows.append(row)
            continue

        # Fix canonical_source
        old_source = row[canonical_source_idx]
        new_source = old_source
        if old_source and '_recovered.c' in old_source:
            candidate = old_source.replace('_recovered.c', '.c')
            if candidate in canonical_sources:
                new_source = candidate
                fixed_count += 1
                print(f"  Source: {old_source} -> {new_source}")
            else:
                # This is a progress file that was split - mark for removal
                print(f"  Source: {old_source} -> NO CANONICAL TARGET (progress file split)")
                new_source = None  # Will remove this row

        # Fix canonical_object
        old_object = row[canonical_object_idx]
        new_object = old_object
        if old_object and '_recovered.o' in old_object:
            candidate = old_object.replace('_recovered.o', '.o')
            # Check if corresponding source exists
            source_candidate = candidate.replace('.o', '.c').replace('ps2/', 'src/ps2/').replace('snes9x/', 'src/snes9x/').replace('unzip/', 'src/unzip/').replace('zlib/', 'src/zlib/')
            if source_candidate in canonical_sources:
                new_object = candidate
                fixed_count += 1
                print(f"  Object: {old_object} -> {new_object}")
            else:
                print(f"  Object: {old_object} -> NO CANONICAL TARGET (progress file split)")
                new_object = None

        # Fix requesters - remove progress28_structural_lift_recovered.o references
        old_requesters = row[requesters_idx]
        new_requesters = old_requesters
        if old_requesters and 'progress28_structural_lift_recovered.o' in old_requesters:
            new_requesters = old_requesters.replace('ps2/progress28_structural_lift_recovered.o;', '')
            new_requesters = new_requesters.replace('ps2/progress28_structural_lift_recovered.o', '')
            new_requesters = new_requesters.replace(';;', ';').strip(';')
            print(f"  Requesters: {old_requesters} -> {new_requesters}")
            fixed_count += 1

        # Only keep row if both source and object have valid canonical targets
        if new_source is not None and new_object is not None:
            row[canonical_source_idx] = new_source
            row[canonical_object_idx] = new_object
            row[requesters_idx] = new_requesters
            new_rows.append(row)
        else:
            # This was a progress file alias - remove it
            removed_count += 1
            print(f"  REMOVING alias: {row[0]} at {row[1]} (was {old_source}/{old_object})")

    # Write output
    with open(output_file, 'w', newline='') as f:
        writer = csv.writer(f, delimiter='\t')
        writer.writerows(new_rows)

    print(f"\nTotal fixes applied: {fixed_count}")
    print(f"Total progress aliases removed: {removed_count}")
    print(f"Final row count: {len(new_rows) - 1} (was {len(data_rows)})")

if __name__ == '__main__':
    manifest_file = '/root/SNESstation-Decomp/analysis/source_tree/translation_units.tsv'
    input_file = '/root/SNESstation-Decomp/analysis/link_identity/source_address_aliases.tsv'
    output_file = '/root/SNESstation-Decomp/analysis/link_identity/source_address_aliases.tsv'

    canonical_sources = load_canonical_sources(manifest_file)
    print(f"Loaded {len(canonical_sources)} canonical sources from manifest")
    fix_aliases(input_file, output_file, canonical_sources)