#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import importlib.util
import os
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
CURRENT_ADDR = 0x001A896C
SCAN_STARTS = [0x001A8960, 0x001A8964, 0x001A8968, 0x001A896C, 0x001A8970]
OUT = ROOT / "build/matching/progress66-padstate-boundary"
OUT.mkdir(parents=True, exist_ok=True)

def die(msg: str) -> None:
    raise SystemExit(f"Progress66 padState boundary: {msg}")

def run(cmd: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
    )

# ---------------------------------------------------------------------------
# Repository / manifest
# ---------------------------------------------------------------------------
with (ROOT / "analysis/progress_targets.csv").open(newline="", encoding="utf-8") as f:
    rows = list(csv.DictReader(f))

by_addr = {int(r["address"], 16): r for r in rows}
if CURRENT_ADDR not in by_addr:
    die(f"manifest does not contain 0x{CURRENT_ADDR:08x}")

starts = sorted(by_addr)
i = starts.index(CURRENT_ADDR)
prev_addr = starts[i - 1] if i else None
next_addr = starts[i + 1] if i + 1 < len(starts) else None

# ---------------------------------------------------------------------------
# Official comparator
# ---------------------------------------------------------------------------
cmp_path = ROOT / "tools/compare_elf_functions.py"
spec = importlib.util.spec_from_file_location("cmp_p66", cmp_path)
if spec is None or spec.loader is None:
    die("cannot import tools/compare_elf_functions.py")
cmpmod = importlib.util.module_from_spec(spec)
sys.modules["cmp_p66"] = cmpmod
spec.loader.exec_module(cmpmod)
ELFFile = cmpmod.ELFFile
compare_function = cmpmod.compare_function

# ---------------------------------------------------------------------------
# Target bytes: formal original image if present, committed listings otherwise.
# ---------------------------------------------------------------------------
reference = ROOT / "build/SNES_EMU.unpacked.bin"
formal = False
known: set[int] | None = None

if reference.exists():
    digest = hashlib.sha256(reference.read_bytes()).hexdigest()
    if digest == TARGET_SHA256:
        target = reference.read_bytes()
        formal = True
    else:
        print("WARNING: unexpected reference SHA; using committed listings")

if not formal:
    insn_re = re.compile(
        r"^\s*([0-9A-Fa-f]+):\s+"
        r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
        r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
    )
    byte_map: dict[int, int] = {}
    conflicts: set[int] = set()
    for listing in sorted((ROOT / "analysis/functions").glob("*.asm")):
        for line in listing.read_text(encoding="utf-8", errors="replace").splitlines():
            m = insn_re.match(line)
            if not m:
                continue
            addr = int(m.group(1), 16)
            bs = bytes(int(m.group(k), 16) for k in range(2, 6))
            for off, value in enumerate(bs):
                a = addr + off
                old = byte_map.get(a)
                if old is not None and old != value:
                    conflicts.add(a)
                else:
                    byte_map[a] = value
    for a in conflicts:
        byte_map.pop(a, None)
    if not byte_map:
        die("no committed listing bytes found")
    end = max(byte_map) + 1
    image = bytearray(end - TARGET_BASE)
    for addr, value in byte_map.items():
        if addr >= TARGET_BASE:
            image[addr - TARGET_BASE] = value
    target = bytes(image)
    known = set(byte_map)

def bytes_known(start: int, end: int) -> bool:
    if formal:
        return TARGET_BASE <= start <= end <= TARGET_BASE + len(target)
    assert known is not None
    return all(a in known for a in range(start, end))

def target_slice(start: int, end: int) -> bytes:
    return target[start - TARGET_BASE:end - TARGET_BASE]

# ---------------------------------------------------------------------------
# Find the already-built historical libpad object from P63/P61 evidence.
# ---------------------------------------------------------------------------
candidate_rows: list[dict[str, str]] = []
for tsv in [
    ROOT / "build/matching/progress63-mc-linkage/best.tsv",
    ROOT / "build/matching/progress61-source-lineage/best.tsv",
    ROOT / "build/matching/progress60-source-lineage/best.tsv",
]:
    if not tsv.exists():
        continue
    with tsv.open(newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            if (r.get("address") or "").lower() == "0x001a896c":
                rr = dict(r)
                rr["_evidence"] = str(tsv.relative_to(ROOT))
                candidate_rows.append(rr)

if not candidate_rows:
    die("no existing padStateInt2String candidate row found in P60/P61/P63 evidence")

def rank(r: dict[str, str]) -> tuple[int, int]:
    try:
        d = int(r.get("differing_bytes") or 10**9)
    except ValueError:
        d = 10**9
    # Prefer the observed p63-os / libpad-newpadman near miss.
    pref = 0 if r.get("profile", "").endswith("-os") else 1
    return (d, pref)

candidate_row = sorted(candidate_rows, key=rank)[0]
obj = ROOT / candidate_row["object"]
symbol_name = candidate_row["object_symbol"]
if not obj.exists():
    die(f"candidate object no longer exists: {obj.relative_to(ROOT)}")

elf = ELFFile(obj)
syms = [
    s for s in elf.symbols
    if s.name == symbol_name and s.section_index != 0 and s.size > 0
]
if len(syms) != 1:
    die(f"expected one object symbol {symbol_name}, found {len(syms)}")
sym = syms[0]
size = sym.size

# ---------------------------------------------------------------------------
# Scan hypothetical target starts with the exact same strict comparator.
# ---------------------------------------------------------------------------
scan_rows: list[dict[str, str]] = []
for start in SCAN_STARTS:
    row = {
        "target_start": f"0x{start:08x}",
        "candidate_symbol": symbol_name,
        "candidate_size": str(size),
        "target_end": f"0x{start + size:08x}",
    }
    if not bytes_known(start, start + size):
        row.update({
            "result": "target-bytes-missing",
            "differing_bytes": "",
            "raw_equal": "",
            "normalized_equal": "",
            "unknown_relocations": "",
            "first_differences": "",
        })
        scan_rows.append(row)
        continue
    comp = compare_function(target, start - TARGET_BASE, size, elf, symbol_name)
    row.update({
        "result": "MATCH" if comp.matching and not comp.unknown_relocation_types else "mismatch",
        "differing_bytes": str(comp.differing_bytes),
        "raw_equal": str(comp.raw_equal),
        "normalized_equal": str(comp.normalized_equal),
        "unknown_relocations": ",".join(map(str, comp.unknown_relocation_types)),
        "first_differences": ",".join(f"0x{x:x}" for x in comp.first_differences),
    })
    scan_rows.append(row)

with (OUT / "start_scan.tsv").open("w", newline="", encoding="utf-8") as f:
    fields = [
        "target_start", "candidate_symbol", "candidate_size", "target_end",
        "result", "differing_bytes", "raw_equal", "normalized_equal",
        "unknown_relocations", "first_differences",
    ]
    w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    w.writeheader()
    w.writerows(scan_rows)

# ---------------------------------------------------------------------------
# Boundary evidence around predecessor/current/next.
# ---------------------------------------------------------------------------
window_start = 0x001A8940
window_end = 0x001A89D0
listing_lines: list[str] = []
line_re = re.compile(r"^\s*([0-9A-Fa-f]+):")
for listing in sorted((ROOT / "analysis/functions").glob("*.asm")):
    for line in listing.read_text(encoding="utf-8", errors="replace").splitlines():
        m = line_re.match(line)
        if not m:
            continue
        a = int(m.group(1), 16)
        if window_start <= a < window_end:
            listing_lines.append(f"{listing.name}: {line}")

(OUT / "target-window.txt").write_text(
    "\n".join(listing_lines) + ("\n" if listing_lines else ""),
    encoding="utf-8",
)

# Decode the key 4-byte target words directly.
def word_hex(addr: int) -> str:
    if not bytes_known(addr, addr + 4):
        return "????????"
    return target_slice(addr, addr + 4).hex()

key_words = [
    0x001A8958, 0x001A895C, 0x001A8960, 0x001A8964,
    0x001A8968, 0x001A896C, 0x001A8970, 0x001A8974,
]
(OUT / "key-target-words.txt").write_text(
    "".join(f"0x{a:08x}\t{word_hex(a)}\n" for a in key_words),
    encoding="utf-8",
)

# Historical object symbol neighborhood.
neighbors = []
for s in elf.symbols:
    if s.section_index != sym.section_index or s.size <= 0 or (s.info & 0xF) != 2:
        continue
    if abs(s.value - sym.value) <= 0x180:
        neighbors.append((s.value, s.size, s.name))
neighbors.sort()
(OUT / "object-symbol-neighborhood.txt").write_text(
    "".join(f"0x{v:08x}\t0x{sz:x}\t{name}\n" for v, sz, name in neighbors),
    encoding="utf-8",
)

# Try ee-objdump for the exact symbol neighborhood.
objdump = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-objdump"
if objdump.exists():
    cp = subprocess.run(
        [str(objdump), "-dr", str(obj)],
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
    )
    asm = cp.stdout
    p = re.compile(
        r"^[0-9a-fA-F]+\s+<" + re.escape(symbol_name) + r">:\n"
        r"(.*?)(?=^[0-9a-fA-F]+\s+<|\Z)",
        re.M | re.S,
    )
    m = p.search(asm)
    (OUT / "candidate-disassembly.txt").write_text(
        m.group(0) if m else asm, encoding="utf-8"
    )

# J/JAL xref scan for the alternate and current address.
# MIPS J-type target = ((PC+4)&0xf0000000) | (index<<2).
xrefs: dict[int, list[int]] = {0x001A8964: [], 0x001A8968: [], 0x001A896C: []}
scan_min = TARGET_BASE
scan_max = TARGET_BASE + len(target) - 4
for pc in range(scan_min, scan_max + 1, 4):
    if not bytes_known(pc, pc + 4):
        continue
    insn = int.from_bytes(target_slice(pc, pc + 4), "little")
    op = insn >> 26
    if op not in (2, 3):  # j / jal
        continue
    dest = ((pc + 4) & 0xF0000000) | ((insn & 0x03FFFFFF) << 2)
    if dest in xrefs:
        xrefs[dest].append(pc)

(OUT / "jtype-xrefs.txt").write_text(
    "".join(
        f"0x{dest:08x}\t" +
        (",".join(f"0x{pc:08x}" for pc in pcs) if pcs else "<none>") +
        "\n"
        for dest, pcs in xrefs.items()
    ),
    encoding="utf-8",
)

# ---------------------------------------------------------------------------
# Verdict.
# ---------------------------------------------------------------------------
matches = [r for r in scan_rows if r["result"] == "MATCH"]
summary: list[str] = [
    "Progress66 padStateInt2String boundary diagnostic",
    f"manifest_current=0x{CURRENT_ADDR:08x}",
    f"manifest_name={by_addr[CURRENT_ADDR]['name']}",
    f"manifest_status={by_addr[CURRENT_ADDR]['status']}",
    f"manifest_prev={f'0x{prev_addr:08x}' if prev_addr is not None else '<none>'}",
    f"manifest_prev_name={by_addr[prev_addr]['name'] if prev_addr is not None else '<none>'}",
    f"manifest_next={f'0x{next_addr:08x}' if next_addr is not None else '<none>'}",
    f"manifest_next_name={by_addr[next_addr]['name'] if next_addr is not None else '<none>'}",
    f"candidate_evidence={candidate_row['_evidence']}",
    f"candidate_object={obj.relative_to(ROOT)}",
    f"candidate_symbol={symbol_name}",
    f"candidate_size=0x{size:x}",
    "",
    "start_scan:",
]
for r in scan_rows:
    summary.append(
        f"  {r['target_start']} -> {r['result']} "
        f"diff={r['differing_bytes'] or '-'} "
        f"normalized={r['normalized_equal'] or '-'} "
        f"unknown={r['unknown_relocations'] or '<none>'}"
    )

summary += [
    "",
    "key_target_words:",
]
summary.extend(f"  0x{a:08x} {word_hex(a)}" for a in key_words)

summary += ["", "jtype_xrefs:"]
for dest, pcs in xrefs.items():
    summary.append(
        f"  0x{dest:08x}: " +
        (", ".join(f"0x{pc:08x}" for pc in pcs) if pcs else "<none>")
    )

strict_alt = [
    r for r in matches
    if int(r["target_start"], 16) != CURRENT_ADDR
    and r["normalized_equal"] == "True"
    and not r["unknown_relocations"]
]

if strict_alt:
    best = strict_alt[0]
    alt = int(best["target_start"], 16)
    pred_tail_ok = (
        alt >= 8
        and bytes_known(alt - 8, alt)
        and target_slice(alt - 8, alt - 4) == bytes.fromhex("0800e003")
    )
    # `jr ra` at alt-8 means alt-4 is the delay slot and alt begins immediately
    # after the predecessor return sequence.
    candidate_end = alt + size
    no_manifest_overlap = next_addr is None or candidate_end <= next_addr
    summary += [
        "",
        "boundary_candidate:",
        f"  alternate_start=0x{alt:08x}",
        f"  candidate_end=0x{candidate_end:08x}",
        f"  predecessor_jr_ra_plus_delay={pred_tail_ok}",
        f"  no_overlap_with_next_manifest={no_manifest_overlap}",
    ]
    if pred_tail_ok and no_manifest_overlap:
        summary += [
            "",
            "VERDICT=BOUNDARY_SHIFT_STRONGLY_PROVEN",
            f"suggested_start=0x{alt:08x}",
            "NOTE: do not mutate manifests automatically; promotion requires review of",
            "      predecessor ownership and any address-indexed evidence files.",
        ]
    else:
        summary += [
            "",
            "VERDICT=ALTERNATE_STRICT_MATCH_BUT_BOUNDARY_REVIEW_REQUIRED",
        ]
else:
    summary += [
        "",
        "VERDICT=NO_ALTERNATE_STRICT_MATCH",
    ]

(OUT / "summary.txt").write_text("\n".join(summary) + "\n", encoding="utf-8")

print("\n".join(summary))
print(f"\nEvidence: {OUT.relative_to(ROOT)}/")
