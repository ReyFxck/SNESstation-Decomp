#!/usr/bin/env python3
"""
Fix alias files by replacing _recovered suffixes with canonical names.
"""

import csv
import sys

def fix_aliases(input_file, output_file):
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
    for row in data_rows:
        if len(row) <= max(canonical_source_idx, canonical_object_idx, requesters_idx):
            continue

        # Fix canonical_source
        old_source = row[canonical_source_idx]
        if old_source and '_recovered.c' in old_source:
            new_source = old_source.replace('_recovered.c', '.c')
            row[canonical_source_idx] = new_source
            fixed_count += 1
            print(f"  Source: {old_source} -> {new_source}")

        # Fix canonical_object
        old_object = row[canonical_object_idx]
        if old_object and '_recovered.o' in old_object:
            new_object = old_object.replace('_recovered.o', '.o')
            row[canonical_object_idx] = new_object
            fixed_count += 1
            print(f"  Object: {old_object} -> {new_object}")

        # Fix requesters - replace progress28_structural_lift_recovered.o references
        # Since progress28 was split into multiple files, we'll remove this requester
        # or replace with empty (the requesters column seems to be for reference)
        old_requesters = row[requesters_idx]
        if old_requesters and 'progress28_structural_lift_recovered.o' in old_requesters:
            # Remove this requester since it no longer exists
            new_requesters = old_requesters.replace('ps2/progress28_structural_lift_recovered.o;', '')
            new_requesters = new_requesters.replace('ps2/progress28_structural_lift_recovered.o', '')
            new_requesters = new_requesters.replace(';;', ';').strip(';')
            row[requesters_idx] = new_requesters
            fixed_count += 1
            print(f"  Requesters: {old_requesters} -> {new_requesters}")

    # Write output
    with open(output_file, 'w', newline='') as f:
        writer = csv.writer(f, delimiter='\t')
        writer.writerow(header)
        writer.writerows(data_rows)

    print(f"\nTotal fixes applied: {fixed_count}")

if __name__ == '__main__':
    input_file = '/root/SNESstation-Decomp/analysis/link_identity/source_address_aliases.tsv'
    output_file = '/root/SNESstation-Decomp/analysis/link_identity/source_address_aliases.tsv'
    fix_aliases(input_file, output_file)