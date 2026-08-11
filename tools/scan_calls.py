#!/usr/bin/env python3
import argparse
import csv
import struct
from collections import Counter
from pathlib import Path


def main():
    ap = argparse.ArgumentParser(description="Initial MIPS JAL target scanner (heuristic; data may cause false positives)")
    ap.add_argument("input", type=Path)
    ap.add_argument("output", type=Path)
    ap.add_argument("--base", type=lambda x: int(x, 0), default=0x00100000)
    args = ap.parse_args()

    data = args.input.read_bytes()
    calls = Counter()
    sites = {}
    end = args.base + len(data)

    for off in range(0, len(data) - 3, 4):
        word = struct.unpack_from("<I", data, off)[0]
        if (word >> 26) != 0x03:  # JAL
            continue
        pc = args.base + off
        target = ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
        if args.base <= target < end:
            calls[target] += 1
            sites.setdefault(target, []).append(pc)

    with args.output.open("w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["target", "call_count", "first_call_site"])
        for target, count in sorted(calls.items()):
            w.writerow([f"0x{target:08x}", count, f"0x{sites[target][0]:08x}"])
    print(f"candidate_targets={len(calls)}")

if __name__ == "__main__":
    main()
