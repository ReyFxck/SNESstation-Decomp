#!/usr/bin/env python3
"""Generate the public objdiff Report v2 consumed by decomp.dev.

This report deliberately exposes two different proof levels:

* the frozen 1,041-function gate is fully matched, but its source units are not
  marked complete/linked;
* the Stage-3G whole-image gate counts only completely exact 64-KiB chunks.

No original executable, extracted payload, absolute build path or private hash
input is needed to generate the report in GitHub Actions.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
from collections import defaultdict
from pathlib import Path
from typing import Any, Iterable


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_CONTRACT = ROOT / "analysis" / "decompdev" / "report_contract.json"
DEFAULT_OUTPUT = ROOT / "build" / "decompdev" / "report.json"

FUNCTION_CATEGORY = "function_matching"
IMAGE_CATEGORY = "whole_image_identity"
CHUNK_SIZE = 64 * 1024
UINT64_MEASURE_FIELDS = (
    "total_code",
    "matched_code",
    "total_data",
    "matched_data",
    "complete_code",
    "complete_data",
)
PERCENT_FIELDS = (
    "fuzzy_match_percent",
    "matched_code_percent",
    "matched_data_percent",
    "matched_functions_percent",
    "complete_code_percent",
    "complete_data_percent",
)


class DecompDevReportError(RuntimeError):
    """A public input or generated report violates the frozen contract."""


def _sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _load_json(path: Path) -> dict[str, Any]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise DecompDevReportError(f"cannot read JSON {path}: {exc}") from exc
    if not isinstance(value, dict):
        raise DecompDevReportError(f"expected JSON object in {path}")
    return value


def _load_csv(path: Path, delimiter: str = ",") -> list[dict[str, str]]:
    try:
        with path.open(newline="", encoding="utf-8") as stream:
            return list(csv.DictReader(stream, delimiter=delimiter))
    except OSError as exc:
        raise DecompDevReportError(f"cannot read table {path}: {exc}") from exc


def load_contract(path: Path = DEFAULT_CONTRACT) -> dict[str, Any]:
    contract = _load_json(path)
    if contract.get("schema_version") != 1:
        raise DecompDevReportError("unsupported decomp.dev contract schema")
    if contract.get("format") != "snesstation-decompdev-report-contract":
        raise DecompDevReportError("unexpected decomp.dev contract format")
    if contract.get("report_version") != 2:
        raise DecompDevReportError("decomp.dev requires objdiff Report version 2")

    privacy = contract.get("privacy", {})
    expected_privacy = {
        "contains_binary_payloads": False,
        "requires_private_reference": False,
        "uses_absolute_local_paths": False,
    }
    if privacy != expected_privacy:
        raise DecompDevReportError("privacy contract must remain entirely public")

    image = contract.get("whole_image_identity", {})
    if image.get("replacement_elf") is not False:
        raise DecompDevReportError("report must not claim a replacement ELF")
    if image.get("unpacked_hash_matched") is not False:
        raise DecompDevReportError("report must not claim the unpacked hash")

    for relative, expected in contract.get("inputs", {}).items():
        path = ROOT / relative
        actual = _sha256(path)
        if actual != expected:
            raise DecompDevReportError(
                f"public input drift for {relative}: expected {expected}, got {actual}"
            )
    return contract


def _percent(numerator: int, denominator: int) -> float:
    if denominator == 0:
        return 100.0
    return round(numerator * 100.0 / denominator, 6)


def _measures(
    *,
    total_code: int = 0,
    matched_code: int = 0,
    total_data: int = 0,
    matched_data: int = 0,
    total_functions: int = 0,
    matched_functions: int = 0,
    complete_code: int = 0,
    complete_data: int = 0,
    total_units: int = 1,
    complete_units: int = 0,
) -> dict[str, Any]:
    return {
        "fuzzy_match_percent": _percent(matched_code, total_code),
        "total_code": str(total_code),
        "matched_code": str(matched_code),
        "matched_code_percent": _percent(matched_code, total_code),
        "total_data": str(total_data),
        "matched_data": str(matched_data),
        "matched_data_percent": _percent(matched_data, total_data),
        "total_functions": total_functions,
        "matched_functions": matched_functions,
        "matched_functions_percent": _percent(matched_functions, total_functions),
        "complete_code": str(complete_code),
        "complete_code_percent": _percent(complete_code, total_code),
        "complete_data": str(complete_data),
        "complete_data_percent": _percent(complete_data, total_data),
        "total_units": total_units,
        "complete_units": complete_units,
    }


def _sum_measures(values: Iterable[dict[str, Any]]) -> dict[str, Any]:
    totals = {
        "total_code": 0,
        "matched_code": 0,
        "total_data": 0,
        "matched_data": 0,
        "total_functions": 0,
        "matched_functions": 0,
        "complete_code": 0,
        "complete_data": 0,
        "total_units": 0,
        "complete_units": 0,
    }
    for value in values:
        for field in UINT64_MEASURE_FIELDS:
            totals[field] += int(value[field])
        for field in (
            "total_functions",
            "matched_functions",
            "total_units",
            "complete_units",
        ):
            totals[field] += value[field]
    return _measures(**totals)


def _canonical_sources() -> set[str]:
    rows = _load_csv(
        ROOT / "analysis" / "source_tree" / "translation_units.tsv", delimiter="\t"
    )
    canonical = {row["source"] for row in rows if row["link_role"] == "canonical"}
    if len(canonical) != 96:
        raise DecompDevReportError(
            f"expected 96 canonical source units, found {len(canonical)}"
        )
    return canonical


def _function_rows(contract: dict[str, Any]) -> list[dict[str, Any]]:
    progress = _load_csv(ROOT / "analysis" / "progress_targets.csv")
    readiness = _load_csv(ROOT / "analysis" / "source_readiness.csv")
    expected_count = contract["function_matching"]["entries"]
    if len(progress) != expected_count or len(readiness) != expected_count:
        raise DecompDevReportError(
            f"expected {expected_count} progress/readiness rows, got "
            f"{len(progress)}/{len(readiness)}"
        )

    ready_by_address = {row["address"]: row for row in readiness}
    if len(ready_by_address) != len(readiness):
        raise DecompDevReportError("duplicate source-readiness address")

    addresses = [int(row["address"], 0) for row in progress]
    if addresses != sorted(addresses) or len(set(addresses)) != len(addresses):
        raise DecompDevReportError("progress addresses must be unique and increasing")

    canonical = _canonical_sources()
    result: list[dict[str, Any]] = []
    for index, row in enumerate(progress):
        ready = ready_by_address.get(row["address"])
        if ready is None or ready["name"] != row["name"]:
            raise DecompDevReportError(f"source-readiness mismatch at {row['address']}")
        if row["status"] != "MATCHING" or ready["matching_status"] != "MATCHING":
            raise DecompDevReportError(f"non-matching function at {row['address']}")

        address = addresses[index]
        if index + 1 < len(progress):
            size = addresses[index + 1] - address
        else:
            size = contract["function_matching"]["terminal_function_size"]
        if size <= 0 or size % 4:
            raise DecompDevReportError(f"invalid function span at {row['address']}")

        sources = [
            value
            for value in ready["source_files"].split(";")
            if value and value in canonical
        ]
        unit = sources[0] if sources else f"historical/{row['area']}"
        result.append(
            {
                "address": address,
                "name": row["name"],
                "size": size,
                "unit": unit,
            }
        )

    total_bytes = sum(row["size"] for row in result)
    expected_bytes = contract["function_matching"]["code_bytes"]
    if total_bytes != expected_bytes:
        raise DecompDevReportError(
            f"function-span drift: expected {expected_bytes}, got {total_bytes}"
        )
    return result


def _function_units(
    contract: dict[str, Any], rows: list[dict[str, Any]]
) -> list[dict[str, Any]]:
    grouped: dict[str, list[dict[str, Any]]] = defaultdict(list)
    for row in rows:
        grouped[row["unit"]].append(row)

    expected_units = contract["function_matching"]["units"]
    if len(grouped) != expected_units:
        raise DecompDevReportError(
            f"function-unit drift: expected {expected_units}, got {len(grouped)}"
        )

    units: list[dict[str, Any]] = []
    ordered = sorted(grouped.items(), key=lambda item: min(r["address"] for r in item[1]))
    for unit_name, functions in ordered:
        functions.sort(key=lambda row: row["address"])
        base_address = functions[0]["address"]
        code_bytes = sum(row["size"] for row in functions)
        items = []
        for row in functions:
            items.append(
                {
                    "name": row["name"],
                    "size": str(row["size"]),
                    "fuzzy_match_percent": 100.0,
                    "metadata": {"virtual_address": str(row["address"])},
                    "address": str(row["address"] - base_address),
                }
            )
        metadata: dict[str, Any] = {
            "complete": False,
            "progress_categories": [FUNCTION_CATEGORY],
        }
        if unit_name.startswith("src/"):
            metadata["source_path"] = unit_name
        else:
            metadata["auto_generated"] = True
        units.append(
            {
                "name": unit_name,
                "measures": _measures(
                    total_code=code_bytes,
                    matched_code=code_bytes,
                    total_functions=len(functions),
                    matched_functions=len(functions),
                ),
                "sections": [],
                "functions": items,
                "metadata": metadata,
            }
        )
    return units


def _image_units(contract: dict[str, Any]) -> list[dict[str, Any]]:
    probe = _load_json(ROOT / "analysis" / "link_identity" / "link_layout_probe.json")
    claims = probe.get("claims", {})
    result = probe.get("result", {})
    frozen = contract["whole_image_identity"]
    if claims.get("replacement_elf") is not False or claims.get("unpacked_hash_matched") is not False:
        raise DecompDevReportError("Stage-3G probe no longer has the frozen open claims")
    for field, probe_field in (
        ("total_chunks", "chunk_count"),
        ("exact_chunks", "exact_chunks"),
        ("target_bytes", "target_initialized_size"),
    ):
        if frozen[field] != result.get(probe_field):
            raise DecompDevReportError(
                f"whole-image drift for {field}: expected {frozen[field]}, "
                f"got {result.get(probe_field)}"
            )

    exact_indices = set(result.get("exact_chunk_indices", []))
    total_size = frozen["target_bytes"]
    units = []
    exact_bytes = 0
    for index in range(frozen["total_chunks"]):
        offset = index * CHUNK_SIZE
        size = min(CHUNK_SIZE, total_size - offset)
        if size <= 0:
            raise DecompDevReportError("chunk geometry exceeds target size")
        exact = index in exact_indices
        matched = size if exact else 0
        exact_bytes += matched
        virtual_address = 0x00100000 + offset
        units.append(
            {
                "name": f"whole-image/chunk-{index:02d}",
                "measures": _measures(
                    total_data=size,
                    matched_data=matched,
                    complete_data=matched,
                    complete_units=1 if exact else 0,
                ),
                "sections": [
                    {
                        "name": f"image-chunk-{index:02d}",
                        "size": str(size),
                        "fuzzy_match_percent": 100.0 if exact else 0.0,
                        "metadata": {"virtual_address": str(virtual_address)},
                        "address": "0",
                    }
                ],
                "functions": [],
                "metadata": {
                    "complete": exact,
                    "progress_categories": [IMAGE_CATEGORY],
                    "auto_generated": True,
                },
            }
        )
    if exact_bytes != frozen["exact_chunk_bytes"]:
        raise DecompDevReportError(
            f"exact-chunk byte drift: expected {frozen['exact_chunk_bytes']}, got {exact_bytes}"
        )
    return units


def build_report(contract_path: Path = DEFAULT_CONTRACT) -> dict[str, Any]:
    contract = load_contract(contract_path)
    function_units = _function_units(contract, _function_rows(contract))
    image_units = _image_units(contract)
    units = function_units + image_units
    function_measures = _sum_measures(unit["measures"] for unit in function_units)
    image_measures = _sum_measures(unit["measures"] for unit in image_units)
    report = {
        "measures": _sum_measures(unit["measures"] for unit in units),
        "units": units,
        "version": 2,
        "categories": [
            {
                "id": FUNCTION_CATEGORY,
                "name": "Function matching (frozen 1041 gate)",
                "measures": function_measures,
            },
            {
                "id": IMAGE_CATEGORY,
                "name": "Whole-image link identity (64-KiB chunks)",
                "measures": image_measures,
            },
        ],
    }
    validate_report(report, contract)
    return report


def _require_uint64_strings(value: Any, location: str) -> None:
    if not isinstance(value, dict):
        raise DecompDevReportError(f"missing measures at {location}")
    for field in UINT64_MEASURE_FIELDS:
        raw = value.get(field)
        if not isinstance(raw, str) or not raw.isdigit():
            raise DecompDevReportError(
                f"protobuf uint64 {location}.{field} must be a decimal JSON string"
            )
    for field in PERCENT_FIELDS:
        if not isinstance(value.get(field), (int, float)):
            raise DecompDevReportError(f"missing percentage {location}.{field}")


def validate_report(report: dict[str, Any], contract: dict[str, Any]) -> None:
    if report.get("version") != 2:
        raise DecompDevReportError("report is not objdiff Report version 2")
    units = report.get("units")
    categories = report.get("categories")
    if not isinstance(units, list) or not isinstance(categories, list):
        raise DecompDevReportError("report units/categories must be arrays")
    _require_uint64_strings(report.get("measures"), "report")
    for index, unit in enumerate(units):
        _require_uint64_strings(unit.get("measures"), f"units[{index}]")
        metadata = unit.get("metadata", {})
        if not isinstance(metadata.get("progress_categories"), list):
            raise DecompDevReportError(f"unit {index} lacks progress categories")
        source = metadata.get("source_path")
        if source is not None and (Path(source).is_absolute() or ".." in Path(source).parts):
            raise DecompDevReportError(f"unsafe source path in unit {index}")
        for collection in (unit.get("sections", []), unit.get("functions", [])):
            for item in collection:
                if not isinstance(item.get("size"), str) or not item["size"].isdigit():
                    raise DecompDevReportError("report item size must be a uint64 string")
                if "address" in item and (
                    not isinstance(item["address"], str) or not item["address"].isdigit()
                ):
                    raise DecompDevReportError("report item address must be a uint64 string")
                virtual = item.get("metadata", {}).get("virtual_address")
                if virtual is not None and (
                    not isinstance(virtual, str) or not virtual.isdigit()
                ):
                    raise DecompDevReportError(
                        "report item virtual_address must be a uint64 string"
                    )
    for index, category in enumerate(categories):
        _require_uint64_strings(category.get("measures"), f"categories[{index}]")

    function = next((c for c in categories if c.get("id") == FUNCTION_CATEGORY), None)
    image = next((c for c in categories if c.get("id") == IMAGE_CATEGORY), None)
    if function is None or image is None or len(categories) != 2:
        raise DecompDevReportError("report must contain exactly the two audited categories")

    fm = function["measures"]
    frozen_functions = contract["function_matching"]
    if fm["total_functions"] != frozen_functions["entries"]:
        raise DecompDevReportError("function total does not match frozen contract")
    if fm["matched_functions"] != frozen_functions["entries"]:
        raise DecompDevReportError("function matching total does not match frozen contract")
    if int(fm["complete_code"]) != 0 or fm["complete_units"] != 0:
        raise DecompDevReportError("function matches must not claim complete linked units")

    im = image["measures"]
    frozen_image = contract["whole_image_identity"]
    if im["total_units"] != frozen_image["total_chunks"]:
        raise DecompDevReportError("whole-image chunk total drift")
    if im["complete_units"] != frozen_image["exact_chunks"]:
        raise DecompDevReportError("whole-image exact chunk total drift")
    if int(im["total_data"]) != frozen_image["target_bytes"]:
        raise DecompDevReportError("whole-image byte total drift")
    if int(im["matched_data"]) != frozen_image["exact_chunk_bytes"]:
        raise DecompDevReportError("whole-image exact byte total drift")

    encoded = json.dumps(report, sort_keys=True)
    forbidden = ("original/", "SNES_EMU.ELF", "/root/", "/storage/", "file://")
    leaked = [token for token in forbidden if token in encoded]
    if leaked:
        raise DecompDevReportError(f"private/local path leaked into report: {leaked}")


def render_report(report: dict[str, Any]) -> str:
    return json.dumps(report, indent=2, sort_keys=False) + "\n"


def generate(output: Path, contract_path: Path = DEFAULT_CONTRACT) -> dict[str, Any]:
    report = build_report(contract_path)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(render_report(report), encoding="utf-8")
    return report


def _summary(report: dict[str, Any]) -> str:
    categories = {row["id"]: row["measures"] for row in report["categories"]}
    functions = categories[FUNCTION_CATEGORY]
    image = categories[IMAGE_CATEGORY]
    return (
        f"functions={functions['matched_functions']}/{functions['total_functions']} "
        f"linked_function_units={functions['complete_units']}/{functions['total_units']} "
        f"exact_image_chunks={image['complete_units']}/{image['total_units']} "
        f"private_inputs=0"
    )


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--contract", type=Path, default=DEFAULT_CONTRACT)
    sub = parser.add_subparsers(dest="command", required=True)

    generate_parser = sub.add_parser("generate", help="write a deterministic report.json")
    generate_parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)

    validate_parser = sub.add_parser("validate", help="validate inputs or an existing report")
    validate_parser.add_argument("--report", type=Path)
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> None:
    args = parse_args(argv)
    try:
        expected = build_report(args.contract)
        if args.command == "generate":
            report = generate(args.output, args.contract)
            print(f"generated decomp.dev objdiff v2 report: {_summary(report)}")
            print(f"report={args.output}")
            return

        report = expected
        if args.report is not None:
            report = _load_json(args.report)
            validate_report(report, load_contract(args.contract))
            if report != expected:
                raise DecompDevReportError(
                    "report content is stale or not deterministically generated"
                )
        print(f"verified decomp.dev objdiff v2 report: {_summary(report)}")
    except DecompDevReportError as exc:
        raise SystemExit(f"decomp.dev report: FAIL -- {exc}") from exc


if __name__ == "__main__":
    main()
