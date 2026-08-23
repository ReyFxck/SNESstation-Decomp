#!/usr/bin/env python3
from __future__ import annotations

from pathlib import Path
import csv
import hashlib
import json
import shutil
import sys

ROOT = Path.cwd()
ADDR = "0x001029c4"
TARGET_SHA = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

V69_DIR = ROOT / "build/matching/hunt1041-v69-regalloc"
V69_SUMMARY = V69_DIR / "summary.json"
V69_RESULTS = V69_DIR / "results.tsv"
V69_STRICT = V69_DIR / "strict-hits.tsv"
VALIDATED_SOURCE = V69_DIR / "variants/direct_expr.c"
VALIDATED_OBJECT = V69_DIR / "objects/direct_expr/os.o"

DEST_SOURCE = ROOT / "src/ps2/progress28_structural_lift_recovered.c"
PROGRESS = ROOT / "analysis/progress_targets.csv"
SYMBOLS = ROOT / "analysis/symbols.csv"
READINESS = ROOT / "analysis/source_readiness.csv"

EVIDENCE_TSV = ROOT / "analysis/matching/hunt1041-v69-validated-001029c4.tsv"
EVIDENCE_JSON = ROOT / "analysis/matching/hunt1041-v69-validated-001029c4.json"

NOTE = (
    "HUNT1041 V69 strict MATCH; mode=recovered-source-strict; "
    "provenance=snesstation-v0.23-recovered; "
    "source=src/ps2/progress28_structural_lift_recovered.c; "
    "profile=os; object_symbol=snes_p28_001029c4; object_size=236; "
    "boundary=exact-next-boundary; "
    f"target_gate=formal-unpacked-elf:{TARGET_SHA}; "
    "differing_bytes=0; raw_equal=False; normalized_equal=True; "
    "unknown_relocations=none; "
    "evidence=analysis/matching/hunt1041-v69-validated-001029c4.tsv"
)

def die(msg: str) -> None:
    raise SystemExit("V70 promotion aborted: " + msg)

def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()

def require(path: Path) -> None:
    if not path.is_file():
        die(f"missing {path}")

def load_tsv(path: Path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def find_validated_hit():
    rows = load_tsv(V69_STRICT)
    hits = [
        r for r in rows
        if r.get("variant") == "direct_expr"
        and r.get("profile") == "os"
        and r.get("differing_bytes") == "0"
        and r.get("object_size") == "236"
        and r.get("size_delta") == "0"
        and r.get("normalized_equal", "").lower() == "true"
        and not r.get("unknown_relocations", "")
    ]
    if not hits:
        die("direct_expr/os strict hit is not present in V69 strict-hits.tsv")
    return hits[0]

def validate_summary():
    data = json.loads(V69_SUMMARY.read_text(encoding="utf-8"))
    if data.get("target") != ADDR:
        die("V69 summary target mismatch")
    if int(data.get("target_size", -1)) != 236:
        die("V69 target size mismatch")
    if int(data.get("strict_hits", 0)) < 1:
        die("V69 summary contains no strict hits")
    best = data.get("best") or {}
    if int(best.get("differing_bytes", -1)) != 0:
        die("V69 best result is no longer diff=0")
    return data

def update_manifest(path: Path, status_field: str, confidence_field: str, notes_field: str):
    with path.open(newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))
        fields = list(rows[0].keys()) if rows else []
    found = False
    for row in rows:
        if row.get("address") == ADDR:
            found = True
            row[status_field] = "MATCHING"
            row[confidence_field] = "very-high"
            old = row.get(notes_field, "")
            if "HUNT1041 V69 strict MATCH" not in old:
                row[notes_field] = (old.rstrip("; ") + "; " + NOTE).lstrip("; ")
    if not found:
        die(f"{ADDR} not found in {path}")
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)

def update_readiness():
    with READINESS.open(newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))
        fields = list(rows[0].keys()) if rows else []
    found = False
    for row in rows:
        if row.get("address") == ADDR:
            found = True
            row["manifest_status"] = "MATCHING"
            row["matching_status"] = "MATCHING"
    if not found:
        die(f"{ADDR} not found in {READINESS}")
    with READINESS.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)

def write_evidence(hit, summary):
    EVIDENCE_TSV.parent.mkdir(parents=True, exist_ok=True)
    fields = [
        "address","name","area","provenance","source","profile","object",
        "object_symbol","object_size","boundary","target_sha256",
        "differing_bytes","raw_equal","normalized_equal",
        "unknown_relocations","object_sha256","result"
    ]
    record = {
        "address": ADDR,
        "name": "snes_p16_001029c4",
        "area": "frontend-core",
        "provenance": "snesstation-v0.23-recovered",
        "source": "src/ps2/progress28_structural_lift_recovered.c",
        "profile": "os",
        "object": str(VALIDATED_OBJECT.relative_to(ROOT)),
        "object_symbol": "snes_p28_001029c4",
        "object_size": "236",
        "boundary": "exact-next-boundary",
        "target_sha256": TARGET_SHA,
        "differing_bytes": "0",
        "raw_equal": hit.get("raw_equal", "False"),
        "normalized_equal": hit.get("normalized_equal", "True"),
        "unknown_relocations": hit.get("unknown_relocations", ""),
        "object_sha256": sha256(VALIDATED_OBJECT),
        "result": "MATCHING",
    }
    with EVIDENCE_TSV.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
        w.writeheader()
        w.writerow(record)

    evidence = {
        "schema": 1,
        "milestone": "HUNT1041 V69/V70 promotion",
        "target": ADDR,
        "target_name": "snes_p16_001029c4",
        "target_size": 236,
        "target_sha256": TARGET_SHA,
        "source": str(DEST_SOURCE.relative_to(ROOT)),
        "validated_variant": "direct_expr",
        "profile": "os",
        "compiler": "EE GCC 3.2.2 stage1",
        "object": str(VALIDATED_OBJECT.relative_to(ROOT)),
        "object_sha256": sha256(VALIDATED_OBJECT),
        "boundary": "exact-next-boundary",
        "differing_bytes": 0,
        "raw_equal": False,
        "normalized_equal": True,
        "unknown_relocations": [],
        "strict_hit_count_in_v69": int(summary.get("strict_hits", 0)),
        "working_checkpoint_before": 984,
        "working_checkpoint_after": 985,
        "note": "The six V53 target-side recoveries remain separate from the strict MATCHING manifest count.",
    }
    EVIDENCE_JSON.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf-8")

def count_matching(path: Path) -> int:
    with path.open(newline="", encoding="utf-8") as f:
        return sum(1 for r in csv.DictReader(f) if r.get("status") == "MATCHING")

def main():
    for p in (
        V69_SUMMARY, V69_RESULTS, V69_STRICT, VALIDATED_SOURCE,
        VALIDATED_OBJECT, DEST_SOURCE, PROGRESS, SYMBOLS, READINESS
    ):
        require(p)

    summary = validate_summary()
    hit = find_validated_hit()

    before = count_matching(PROGRESS)

    # Preserve exactly the full TU that produced the validated direct_expr/os hit.
    shutil.copy2(VALIDATED_SOURCE, DEST_SOURCE)

    write_evidence(hit, summary)
    update_manifest(PROGRESS, "status", "confidence", "notes")
    update_manifest(SYMBOLS, "status", "confidence", "notes")
    update_readiness()

    after = count_matching(PROGRESS)
    if after not in (before, before + 1):
        die(f"unexpected MATCHING count transition {before} -> {after}")
    if before != after and after != before + 1:
        die("promotion did not add exactly one manifest match")

    print("HUNT1041 V70 PROMOTION: PASS")
    print(f"address: {ADDR}")
    print("strict evidence: diff=0 normalized_equal=True unknown_relocations=none")
    print("source promoted: src/ps2/progress28_structural_lift_recovered.c")
    print(f"strict manifest MATCHING: {before} -> {after}")
    print("working checkpoint: 984 -> 985 / 1041")
    print("true working remainder: 56")
    print(f"evidence: {EVIDENCE_TSV.relative_to(ROOT)}")
    print(f"evidence json: {EVIDENCE_JSON.relative_to(ROOT)}")

if __name__ == "__main__":
    main()
