#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import importlib.util
import re
import statistics
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
OUT = ROOT / "build/matching/progress67-libpad-object-layout"
OUT.mkdir(parents=True, exist_ok=True)

def die(msg: str) -> None:
    raise SystemExit(f"Progress67 libpad object-layout: {msg}")

# ---------------------------------------------------------------------------
# Manifest
# ---------------------------------------------------------------------------
with (ROOT / "analysis/progress_targets.csv").open(newline="", encoding="utf-8") as f:
    manifest_rows = list(csv.DictReader(f))

manifest_by_name: dict[str, list[dict[str, str]]] = {}
for row in manifest_rows:
    manifest_by_name.setdefault(row["name"], []).append(row)

# ---------------------------------------------------------------------------
# Comparator
# ---------------------------------------------------------------------------
cmp_path = ROOT / "tools/compare_elf_functions.py"
spec = importlib.util.spec_from_file_location("cmp_p67", cmp_path)
if spec is None or spec.loader is None:
    die("cannot import tools/compare_elf_functions.py")
cmpmod = importlib.util.module_from_spec(spec)
sys.modules["cmp_p67"] = cmpmod
spec.loader.exec_module(cmpmod)
ELFFile = cmpmod.ELFFile
compare_function = cmpmod.compare_function

# ---------------------------------------------------------------------------
# Target bytes
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

# ---------------------------------------------------------------------------
# Reuse the already-built P63 -Os historical libpad object.
# ---------------------------------------------------------------------------
candidate_path: Path | None = None
candidate_detail = ""
candidate_profile = ""

for tsv in [
    ROOT / "build/matching/progress63-mc-linkage/best.tsv",
    ROOT / "build/matching/progress61-source-lineage/best.tsv",
]:
    if not tsv.exists():
        continue
    with tsv.open(newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f, delimiter="\t"))
    for r in rows:
        if (
            (r.get("name") or "") == "padStateInt2String"
            and (r.get("detail") or "") == "libpad-newpadman"
            and (r.get("profile") or "").endswith("-os")
        ):
            p = ROOT / r["object"]
            if p.exists():
                candidate_path = p
                candidate_detail = r["detail"]
                candidate_profile = r["profile"]
                break
    if candidate_path:
        break

if candidate_path is None:
    # Conservative fallback to the unique expected object name.
    hits = list(
        ROOT.glob(
            "build/matching/progress63-mc-linkage/objects/"
            "*libpad-newpadman__libpad-newpadman.c__p63-os.o"
        )
    )
    if len(hits) == 1:
        candidate_path = hits[0]
        candidate_detail = "libpad-newpadman"
        candidate_profile = "p63-os"

if candidate_path is None:
    die("could not locate existing libpad-newpadman -Os object")

elf = ELFFile(candidate_path)

# Function symbols only, grouped by name.
funcs = [
    s for s in elf.symbols
    if s.section_index != 0 and s.size > 0 and (s.info & 0xF) == 2 and s.name
]
funcs.sort(key=lambda s: (s.section_index, s.value, s.name))

symbol_by_name: dict[str, object] = {}
for s in funcs:
    symbol_by_name.setdefault(s.name, s)

# ---------------------------------------------------------------------------
# Infer target load base from manifest names already known to this object.
# We deliberately use all unique-name associations and then take the dominant
# base. A systematic +8 bad boundary only creates a minority alternate base.
# ---------------------------------------------------------------------------
anchor_rows: list[dict[str, str]] = []
base_counter: Counter[int] = Counter()

for name, sym in symbol_by_name.items():
    mrows = manifest_by_name.get(name, [])
    if len(mrows) != 1:
        continue
    mr = mrows[0]
    addr = int(mr["address"], 16)
    base = addr - sym.value
    # Existing MATCHING rows are strongest anchors. RECONSTRUCTED rows are
    # recorded for diagnostics but are not allowed to determine the base.
    anchor_rows.append({
        "name": name,
        "manifest_address": f"0x{addr:08x}",
        "manifest_status": mr["status"],
        "object_value": f"0x{sym.value:x}",
        "candidate_base": f"0x{base:08x}",
    })
    if mr["status"] == "MATCHING":
        base_counter[base] += 1

if not base_counter:
    die("no MATCHING manifest names overlap historical libpad object; cannot anchor layout")

dominant = base_counter.most_common()
load_base, anchor_count = dominant[0]
if len(dominant) > 1 and dominant[1][1] == anchor_count:
    die(f"ambiguous object load base: {dominant[:4]}")
if anchor_count < 2:
    die(f"weak object-layout anchor: only {anchor_count} MATCHING symbol")

with (OUT / "anchors.tsv").open("w", newline="", encoding="utf-8") as f:
    fields = [
        "name", "manifest_address", "manifest_status",
        "object_value", "candidate_base",
    ]
    w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    w.writeheader()
    w.writerows(anchor_rows)

# ---------------------------------------------------------------------------
# Strict sweep at object-layout-predicted addresses.
# ---------------------------------------------------------------------------
sweep_rows: list[dict[str, str]] = []
strict_rows: list[dict[str, str]] = []

# next function in same section, used for object-layout boundary proof
next_func: dict[tuple[int, int, str], object | None] = {}
for idx, s in enumerate(funcs):
    nxt = None
    for t in funcs[idx + 1:]:
        if t.section_index == s.section_index and t.value >= s.value:
            nxt = t
            break
        if t.section_index != s.section_index:
            break
    next_func[(s.section_index, s.value, s.name)] = nxt

for name, sym in symbol_by_name.items():
    mrows = manifest_by_name.get(name, [])
    if len(mrows) != 1:
        continue
    mr = mrows[0]
    if mr.get("area") != "libpad":
        continue

    manifest_addr = int(mr["address"], 16)
    predicted = load_base + sym.value
    delta = predicted - manifest_addr
    end = predicted + sym.size

    row: dict[str, str] = {
        "name": name,
        "manifest_address": f"0x{manifest_addr:08x}",
        "predicted_address": f"0x{predicted:08x}",
        "delta": f"{delta:+d}",
        "manifest_status": mr["status"],
        "object_value": f"0x{sym.value:x}",
        "object_size": str(sym.size),
        "target_end": f"0x{end:08x}",
        "profile": candidate_profile,
        "detail": candidate_detail,
        "object": str(candidate_path.relative_to(ROOT)),
    }

    nxt = next_func.get((sym.section_index, sym.value, sym.name))
    if nxt is None:
        boundary = "object-section-tail"
    else:
        gap = nxt.value - (sym.value + sym.size)
        if gap == 0:
            boundary = "object-layout-exact-next"
        elif gap > 0:
            boundary = f"object-layout-gap:0x{gap:x}"
        else:
            boundary = f"object-layout-overlap:0x{-gap:x}"
    row["boundary"] = boundary

    if not bytes_known(predicted, end):
        row.update({
            "result": "target-bytes-missing",
            "differing_bytes": "",
            "raw_equal": "",
            "normalized_equal": "",
            "unknown_relocations": "",
            "first_differences": "",
        })
        sweep_rows.append(row)
        continue

    comp = compare_function(
        target,
        predicted - TARGET_BASE,
        sym.size,
        elf,
        sym.name,
    )
    unknown = ",".join(map(str, comp.unknown_relocation_types))
    is_match = comp.matching and not comp.unknown_relocation_types
    row.update({
        "result": "MATCH" if is_match else "mismatch",
        "differing_bytes": str(comp.differing_bytes),
        "raw_equal": str(comp.raw_equal),
        "normalized_equal": str(comp.normalized_equal),
        "unknown_relocations": unknown,
        "first_differences": ",".join(
            f"0x{x:x}" for x in comp.first_differences
        ),
    })
    sweep_rows.append(row)

    if (
        is_match
        and mr["status"] == "RECONSTRUCTED"
        and boundary != "object-section-tail"
        and not boundary.startswith("object-layout-overlap")
    ):
        strict_rows.append(dict(row))

fields = [
    "name", "manifest_address", "predicted_address", "delta",
    "manifest_status", "object_value", "object_size", "target_end",
    "profile", "detail", "object", "boundary", "result",
    "differing_bytes", "raw_equal", "normalized_equal",
    "unknown_relocations", "first_differences",
]
for path, data in [
    (OUT / "sweep.tsv", sweep_rows),
    (OUT / "strict_reconstructed.tsv", strict_rows),
]:
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
        w.writeheader()
        w.writerows(data)

# Coherence checks for shifted strict rows: predicted ranges may touch but may
# not overlap one another.
ordered = sorted(strict_rows, key=lambda r: int(r["predicted_address"], 16))
overlaps: list[str] = []
for a, b in zip(ordered, ordered[1:]):
    a_end = int(a["target_end"], 16)
    b_start = int(b["predicted_address"], 16)
    if a_end > b_start:
        overlaps.append(
            f"{a['name']} end={a['target_end']} overlaps "
            f"{b['name']} start={b['predicted_address']}"
        )

# Summarize all reconstructed strict matches and all shifted rows.
shifted = [
    r for r in sweep_rows
    if r["manifest_status"] == "RECONSTRUCTED" and r["delta"] != "+0"
]
shifted_strict = [r for r in strict_rows if r["delta"] != "+0"]

summary = [
    "Progress67 libpad object-layout sweep",
    f"target_gate={'formal-original' if formal else 'committed-listings'}",
    f"candidate_object={candidate_path.relative_to(ROOT)}",
    f"candidate_detail={candidate_detail}",
    f"candidate_profile={candidate_profile}",
    f"dominant_load_base=0x{load_base:08x}",
    f"matching_anchor_count={anchor_count}",
    f"alternate_anchor_bases={';'.join(f'0x{k:08x}:{v}' for k,v in dominant[1:6]) or '<none>'}",
    f"strict_reconstructed={len(strict_rows)}",
    f"shifted_strict_reconstructed={len(shifted_strict)}",
    f"strict_range_overlaps={len(overlaps)}",
    "",
    "strict_reconstructed_matches:",
]
for r in strict_rows:
    summary.append(
        f"  {r['name']}: manifest={r['manifest_address']} "
        f"predicted={r['predicted_address']} delta={r['delta']} "
        f"size={r['object_size']} diff={r['differing_bytes']} "
        f"normalized={r['normalized_equal']} boundary={r['boundary']}"
    )

summary += ["", "shifted_reconstructed_rows:"]
for r in shifted:
    summary.append(
        f"  {r['name']}: manifest={r['manifest_address']} "
        f"predicted={r['predicted_address']} delta={r['delta']} "
        f"result={r['result']} diff={r['differing_bytes'] or '-'}"
    )

if overlaps:
    summary += ["", "OVERLAPS:"]
    summary.extend(f"  {x}" for x in overlaps)

if shifted_strict and not overlaps:
    summary += [
        "",
        "VERDICT=COHERENT_SHIFTED_STRICT_MATCHES_FOUND",
        "NOTE=Do not mutate manifests automatically. Review every shifted row as",
        "     one boundary-correction set, because adjacent object layout may expose",
        "     multiple +8 prologue-heuristic errors at once.",
    ]
elif strict_rows:
    summary += ["", "VERDICT=STRICT_MATCHES_FOUND_NO_SHIFT"]
else:
    summary += ["", "VERDICT=NO_NEW_STRICT_OBJECT_LAYOUT_MATCHES"]

(OUT / "summary.txt").write_text("\n".join(summary) + "\n", encoding="utf-8")
print("\n".join(summary))
print(f"\nEvidence: {OUT.relative_to(ROOT)}/")
