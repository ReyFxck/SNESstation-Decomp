#!/usr/bin/env python3
"""
Add FUN_00105898 to external_symbol_ownership.tsv as a source-address-alias.
"""

import csv

def add_fun_entry(input_file, output_file):
    """Add FUN_00105898 entry to external ownership map."""
    with open(input_file, 'r') as f:
        reader = csv.reader(f, delimiter='\t')
        rows = list(reader)

    if not rows:
        return

    header = rows[0]
    data_rows = rows[1:]

    # The new entry for FUN_00105898
    new_entry = [
        "FUN_00105898",
        "target-address-symbol",
        "source-address-alias",
        "src/ps2/path_helpers.c",
        "link-identity",
        "ps2/path_helpers.o"
    ]

    # Find the insertion point (alphabetical by symbol)
    inserted = False
    new_rows = [header]

    for row in data_rows:
        if not inserted and row[0] > "FUN_00105898":
            new_rows.append(new_entry)
            inserted = True
            print(f"Inserted FUN_00105898 before {row[0]}")
        new_rows.append(row)

    if not inserted:
        # Append at the end
        new_rows.append(new_entry)
        print("Appended FUN_00105898 at the end")

    # Write output
    with open(output_file, 'w', newline='') as f:
        writer = csv.writer(f, delimiter='\t')
        writer.writerows(new_rows)

    print(f"Total rows: {len(new_rows) - 1}")

if __name__ == '__main__':
    input_file = '/root/SNESstation-Decomp/analysis/source_tree/external_symbol_ownership.tsv'
    output_file = '/root/SNESstation-Decomp/analysis/source_tree/external_symbol_ownership.tsv'
    add_fun_entry(input_file, output_file)