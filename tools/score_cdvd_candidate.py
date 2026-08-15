#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import importlib.util
from pathlib import Path


def load_compare(root: Path):
    import sys

    path = root / "tools" / "compare_elf_functions.py"
    module_name = "compare_elf_functions_local"
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        raise SystemExit("cannot load compare_elf_functions.py")

    module = importlib.util.module_from_spec(spec)

    # Python 3.13 dataclasses resolve class annotations through sys.modules
    # while decorators execute. Register this transient module exactly as
    # normal import machinery would before exec_module().
    sys.modules[module_name] = module
    try:
        spec.loader.exec_module(module)
    except Exception:
        sys.modules.pop(module_name, None)
        raise

    return module


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--target", required=True, type=Path)
    parser.add_argument("--base-address", required=True, type=lambda x: int(x, 0))
    parser.add_argument("--object", required=True, type=Path)
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--label", default="")
    parser.add_argument("--tsv", action="store_true")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    cmpmod = load_compare(root)

    target = args.target.read_bytes()
    elf = cmpmod.ELFFile(args.object)
    rows = list(csv.DictReader(args.manifest.open(encoding="utf-8", newline="")))

    results = []
    for row in rows:
        address = int(row["address"], 0)
        end = int(row["end"], 0)
        result = cmpmod.compare_function(
            target,
            address - args.base_address,
            end - address,
            elf,
            row["object_symbol"],
        )
        results.append((row, result))

    matches = sum(r.matching for _, r in results)
    same_size = sum(r.expected_size == r.candidate_size for _, r in results)
    size_delta = sum(abs(r.expected_size - r.candidate_size) for _, r in results)
    diff_bytes = sum(r.differing_bytes for _, r in results)

    if args.tsv:
        print(
            f"{args.label}\t{matches}\t{same_size}\t{size_delta}\t{diff_bytes}"
        )
        return

    print(
        f"score: matches={matches}/{len(results)} "
        f"same-size={same_size}/{len(results)} "
        f"size-delta={size_delta} diff-bytes={diff_bytes}"
    )
    for row, r in results:
        status = "MATCH" if r.matching else "MISS "
        first = ", ".join(f"+0x{x:x}" for x in r.first_differences) or "-"
        print(
            f"  {status} {row['address']} {row['name']}: "
            f"{r.expected_size}/{r.candidate_size}, "
            f"diff-bytes={r.differing_bytes}, first={first}"
        )


if __name__ == "__main__":
    main()
