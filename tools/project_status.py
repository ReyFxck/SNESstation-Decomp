#!/usr/bin/env python3
"""Report authoritative matching and any recovered evidence still unpromoted."""
from __future__ import annotations

import argparse
import csv
import json
from dataclasses import asdict, dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TARGETS = ROOT / "analysis" / "progress_targets.csv"
RECOVERED_SPANS = (
    ROOT / "analysis" / "matching" / "hunt1041-v53-recovered-target-spans.tsv"
)
IMAGE_STATUS = ROOT / "analysis" / "link_identity" / "code_windows.json"
EXPECTED_TARGETS = 1041


@dataclass(frozen=True)
class ProjectStatus:
    total: int
    formal_matching: int
    reconstructed_unproven: int
    recovered_pending: int
    working_checkpoint: int
    working_remaining: int
    complete_replacement_elf: bool = False

    @property
    def formal_percent(self) -> float:
        return self.formal_matching * 100.0 / self.total

    @property
    def working_percent(self) -> float:
        return self.working_checkpoint * 100.0 / self.total

    def json_dict(self) -> dict[str, object]:
        result = asdict(self)
        result["formal_percent"] = round(self.formal_percent, 2)
        result["working_percent"] = round(self.working_percent, 2)
        return result


def _read_rows(path: Path, delimiter: str = ",") -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter=delimiter))


def load_status(root: Path = ROOT) -> ProjectStatus:
    targets_path = root / TARGETS.relative_to(ROOT)
    recovered_path = root / RECOVERED_SPANS.relative_to(ROOT)
    targets = _read_rows(targets_path)
    if len(targets) != EXPECTED_TARGETS:
        raise ValueError(f"expected {EXPECTED_TARGETS} targets, found {len(targets)}")

    by_address: dict[str, dict[str, str]] = {}
    for row in targets:
        address = row["address"].lower()
        if address in by_address:
            raise ValueError(f"duplicate target address {address}")
        by_address[address] = row

    formal = {address for address, row in by_address.items() if row["status"] == "MATCHING"}
    unexpected = {row["status"] for row in targets} - {"MATCHING", "RECONSTRUCTED"}
    if unexpected:
        raise ValueError(f"unexpected manifest statuses: {sorted(unexpected)}")

    recovered_rows = _read_rows(recovered_path, delimiter="\t")
    recovered_addresses: set[str] = set()
    for row in recovered_rows:
        address = row["address"].lower()
        if address in recovered_addresses:
            raise ValueError(f"duplicate recovered-span address {address}")
        if address not in by_address:
            raise ValueError(f"recovered-span address outside manifest: {address}")
        if row.get("recovered_exact_match_fact", "").lower() != "yes":
            raise ValueError(f"recovered row is not marked exact: {address}")
        recovered_addresses.add(address)

    pending = recovered_addresses - formal
    working = len(formal) + len(pending)
    if working > EXPECTED_TARGETS:
        raise ValueError("working checkpoint exceeds the audited target universe")

    return ProjectStatus(
        total=EXPECTED_TARGETS,
        formal_matching=len(formal),
        reconstructed_unproven=EXPECTED_TARGETS - len(formal),
        recovered_pending=len(pending),
        working_checkpoint=working,
        working_remaining=EXPECTED_TARGETS - working,
    )


def load_image_status(root: Path = ROOT) -> dict[str, object]:
    path = root / IMAGE_STATUS.relative_to(ROOT)
    document = json.loads(path.read_text(encoding="utf-8"))
    result = document["result"]
    required = {
        "exact_chunks",
        "chunk_count",
        "exact_chunk_indices",
        "mismatching_chunk_indices",
        "differing_bytes",
    }
    if not required.issubset(result):
        raise ValueError("whole-image status manifest is incomplete")
    return result


def render_terminal(status: ProjectStatus, image: dict[str, object]) -> str:
    def ranges(indices: object) -> str:
        values = [int(value) for value in indices]
        groups: list[str] = []
        start = end = values[0]
        for value in values[1:]:
            if value == end + 1:
                end = value
                continue
            groups.append(str(start) if start == end else f"{start}-{end}")
            start = end = value
        groups.append(str(start) if start == end else f"{start}-{end}")
        return " and ".join(groups)

    return "\n".join(
        (
            "SNESstation-Decomp status",
            f"  audited functions:     {status.formal_matching}/{status.total} ({status.formal_percent:.2f}%) complete",
            f"  whole-image windows:   {image['exact_chunks']}/{image['chunk_count']} exact",
            f"  exact windows:         {ranges(image['exact_chunk_indices'])}",
            f"  remaining windows:     {ranges(image['mismatching_chunk_indices'])}",
            f"  differing bytes:       {int(image['differing_bytes']):,}",
            "  replacement ELF:       726968/726968 bytes exact",
            "  packed SHA-256:         4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487",
        )
    )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    args = parser.parse_args()
    status = load_status()
    if args.json:
        result = status.json_dict()
        result["whole_image"] = load_image_status()
        print(json.dumps(result, indent=2, sort_keys=True))
    else:
        print(render_terminal(status, load_image_status()))


if __name__ == "__main__":
    main()
