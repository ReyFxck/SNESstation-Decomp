#!/usr/bin/env python3
"""Fail-closed verifier for the frozen 1,041-function V81 checkpoint."""
from __future__ import annotations

import csv
import hashlib
import subprocess
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[1]
CHECKSUMS = Path("analysis/checkpoints/function-frontier-1041-v81.sha256")
EXPECTED_TARGETS = 1_041
EXPECTED_TAG = "function-frontier-1041-v81"
PACKED_SHA256 = "4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487"
UNPACKED_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
EXPECTED_EVIDENCE_PATHS = {
    "analysis/matching/hunt1041-v81-validated-final20.tsv",
    "analysis/matching/hunt1041-v81-frontier-map-0.tsv",
    "matching/candidates/hunt1041_v81_final20_exact.S",
    "tools/history/research/hunt1041_v81_final20.py",
    "docs/status/V81_FUNCTION_FRONTIER_CLOSED.md",
}


class CheckpointError(RuntimeError):
    """Raised when a frozen checkpoint invariant no longer holds."""


@dataclass(frozen=True)
class CheckpointReport:
    targets: int
    evidence_files: int
    tag: str = EXPECTED_TAG


def _rows(path: Path, delimiter: str = ",") -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


def _sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def read_checksum_manifest(path: Path) -> dict[str, str]:
    entries: dict[str, str] = {}
    for line_number, raw_line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        fields = line.split(None, 1)
        if len(fields) != 2:
            raise CheckpointError(f"{path}:{line_number}: malformed checksum row")
        digest, relative = fields
        relative = relative.lstrip("*")
        pure = PurePosixPath(relative)
        if len(digest) != 64 or any(char not in "0123456789abcdef" for char in digest):
            raise CheckpointError(f"{path}:{line_number}: invalid SHA-256")
        if pure.is_absolute() or ".." in pure.parts or str(pure) != relative:
            raise CheckpointError(f"{path}:{line_number}: unsafe repository path")
        if relative in entries:
            raise CheckpointError(f"{path}:{line_number}: duplicate path {relative}")
        entries[relative] = digest
    return entries


def _verify_evidence_hashes(root: Path) -> int:
    manifest = root / CHECKSUMS
    entries = read_checksum_manifest(manifest)
    if set(entries) != EXPECTED_EVIDENCE_PATHS:
        missing = sorted(EXPECTED_EVIDENCE_PATHS - set(entries))
        extra = sorted(set(entries) - EXPECTED_EVIDENCE_PATHS)
        raise CheckpointError(f"checksum path set changed; missing={missing}, extra={extra}")
    for relative, expected in entries.items():
        candidate = root / relative
        if not candidate.is_file():
            raise CheckpointError(f"missing frozen evidence: {relative}")
        actual = _sha256(candidate)
        if actual != expected:
            raise CheckpointError(
                f"frozen evidence changed: {relative}\nexpected {expected}\nactual   {actual}"
            )
    return len(entries)


def _verify_manifests(root: Path) -> int:
    progress = _rows(root / "analysis/progress_targets.csv")
    symbols = _rows(root / "analysis/symbols.csv")
    readiness = _rows(root / "analysis/source_readiness.csv")
    for name, rows in (
        ("progress_targets.csv", progress),
        ("symbols.csv", symbols),
        ("source_readiness.csv", readiness),
    ):
        if len(rows) != EXPECTED_TARGETS:
            raise CheckpointError(f"{name}: expected {EXPECTED_TARGETS} rows, found {len(rows)}")
        addresses = [row["address"].lower() for row in rows]
        if len(set(addresses)) != EXPECTED_TARGETS:
            raise CheckpointError(f"{name}: duplicate target addresses")

    progress_addresses = {row["address"].lower() for row in progress}
    if progress_addresses != {row["address"].lower() for row in symbols}:
        raise CheckpointError("symbols.csv address universe differs from progress_targets.csv")
    if progress_addresses != {row["address"].lower() for row in readiness}:
        raise CheckpointError("source_readiness.csv address universe differs from progress_targets.csv")
    if {row["status"] for row in progress} != {"MATCHING"}:
        raise CheckpointError("progress target frontier is no longer fully MATCHING")
    if {row["status"] for row in symbols} != {"MATCHING"}:
        raise CheckpointError("symbol frontier is no longer fully MATCHING")
    if {row["manifest_status"] for row in readiness} != {"MATCHING"}:
        raise CheckpointError("source-readiness manifest status is no longer fully MATCHING")
    if {row["matching_status"] for row in readiness} != {"MATCHING"}:
        raise CheckpointError("source-readiness matching status is no longer fully MATCHING")
    if {row["source_form"] for row in readiness} != {"BEHAVIORAL_SOURCE_MODEL"}:
        raise CheckpointError("source-form closure changed from behavioral/source-model coverage")
    return len(progress)


def _verify_zero_frontier_and_final_batch(root: Path) -> None:
    frontier = _rows(
        root / "analysis/matching/hunt1041-v81-frontier-map-0.tsv",
        delimiter="\t",
    )
    if frontier:
        raise CheckpointError(f"V81 frontier contains {len(frontier)} unexpected rows")

    evidence = _rows(
        root / "analysis/matching/hunt1041-v81-validated-final20.tsv",
        delimiter="\t",
    )
    if len(evidence) != 20:
        raise CheckpointError(f"V81 evidence expected 20 rows, found {len(evidence)}")
    if sum(int(row["object_size"]) for row in evidence) != 71_384:
        raise CheckpointError("V81 evidence no longer covers exactly 71,384 bytes")
    if any(row["result"] != "MATCH" or row["differing_bytes"] != "0" for row in evidence):
        raise CheckpointError("V81 evidence contains a non-exact result")
    if any(row["target_gate"] != f"formal-unpacked-elf:{UNPACKED_SHA256}" for row in evidence):
        raise CheckpointError("V81 evidence is not pinned to the frozen unpacked target")


def _verify_fingerprints(root: Path) -> None:
    for relative in ("README.md", "docs/REPRODUCTION.md"):
        text = (root / relative).read_text(encoding="utf-8")
        if PACKED_SHA256 not in text or UNPACKED_SHA256 not in text:
            raise CheckpointError(f"{relative}: frozen target fingerprints are missing")
    original_readme = (root / "original/README.md").read_text(encoding="utf-8")
    if PACKED_SHA256 not in original_readme:
        raise CheckpointError("original/README.md: packed target fingerprint is missing")


def _verify_no_tracked_private_binary(root: Path) -> None:
    if not (root / ".git").exists():
        return
    result = subprocess.run(
        ["git", "ls-files", "-z", "--", "original", "build"],
        cwd=root,
        check=True,
        stdout=subprocess.PIPE,
    )
    tracked = [item.decode() for item in result.stdout.split(b"\0") if item]
    forbidden = [
        path
        for path in tracked
        if path.startswith("build/")
        or (path.startswith("original/") and Path(path).suffix.lower() in {".elf", ".bin"})
    ]
    if forbidden:
        raise CheckpointError(f"private/generated binaries are tracked: {forbidden}")


def verify_checkpoint(root: Path = ROOT) -> CheckpointReport:
    evidence_files = _verify_evidence_hashes(root)
    targets = _verify_manifests(root)
    _verify_zero_frontier_and_final_batch(root)
    _verify_fingerprints(root)
    _verify_no_tracked_private_binary(root)
    return CheckpointReport(targets=targets, evidence_files=evidence_files)


def main() -> None:
    try:
        report = verify_checkpoint()
    except (CheckpointError, OSError, KeyError, ValueError) as exc:
        raise SystemExit(f"function-frontier checkpoint: FAILED: {exc}") from exc
    print("SNESstation-Decomp frozen function checkpoint")
    print(f"  formal function gate: {report.targets}/{report.targets} MATCHING")
    print("  working frontier:     0")
    print(f"  frozen evidence:      {report.evidence_files} files SHA-256 verified")
    print(f"  canonical tag:        {report.tag}")
    print(f"  packed target:        {PACKED_SHA256}")
    print(f"  unpacked target:      {UNPACKED_SHA256}")
    print("  replacement ELF:      not yet (not claimed by this checkpoint)")


if __name__ == "__main__":
    main()
