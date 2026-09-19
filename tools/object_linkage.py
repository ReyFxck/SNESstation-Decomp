#!/usr/bin/env python3
"""Audit the strict, object-native code-linkage boundary.

The whole-image gate proves byte identity.  This audit answers a narrower
question: are the code bytes linked directly from the ELF objects that produced
them, or transported through a consolidated generated payload?
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/object_linkage.json"
DEFAULT_CODE_WINDOWS_MANIFEST = ROOT / "analysis/link_identity/code_windows.json"
DEFAULT_IMPLEMENTATION = ROOT / "tools/code_windows.py"
FORMAT = "snesstation-object-native-linkage"
SCHEMA_VERSION = 1


class ObjectLinkageError(RuntimeError):
    """Raised when the frozen object-linkage boundary drifts."""


def _load_json(path: Path) -> dict[str, Any]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise ObjectLinkageError(f"cannot read {path}: {exc}") from exc
    if not isinstance(value, dict):
        raise ObjectLinkageError(f"{path} must contain a JSON object")
    return value


def _transport_contract(implementation: str, direct_object_bytes: int, incbin_payload_bytes: int, direct_candidate_objects: int) -> dict[str, Any]:
    markers = (
        "def render_payload_source(",
        '.incbin "{path}"',
        'payload_object = args.build_dir / "code-windows.o"',
        'run([linker, "-EL", "-T", linker_script, "-o", output, *prior_inputs, payload_object])',
    )
    missing = [marker for marker in markers if marker not in implementation]
    if missing:
        raise ObjectLinkageError(
            "code-window transport changed; refresh the object-linkage audit: "
            + ", ".join(missing)
        )
    if direct_object_bytes and incbin_payload_bytes:
        transport = "mixed-direct-objects-and-generated-incbin-payload"
    elif direct_object_bytes:
        transport = "direct-producer-objects"
    else:
        transport = "generated-incbin-payload-object"
    return {
        "final_code_input": "build/code-windows/code-windows.o",
        "generated_binary_include": incbin_payload_bytes > 0,
        "selected_candidate_objects_linked_directly": direct_candidate_objects > 0,
        "direct_object_inputs_present": direct_object_bytes > 0,
        "transport": transport,
    }


def derive(
    code_windows: dict[str, Any],
    implementation: str,
) -> dict[str, Any]:
    result = code_windows.get("result")
    if not isinstance(result, dict):
        raise ObjectLinkageError("code-window result is missing")
    selected = result.get("selected_sources")
    if not isinstance(selected, list):
        raise ObjectLinkageError("selected-source roster is missing")

    object_rows = []
    listing_rows = []
    residual_rows = []
    unsupported = []
    for row in selected:
        if not isinstance(row, dict) or not isinstance(row.get("object"), str):
            raise ObjectLinkageError("malformed selected-source entry")
        path = row["object"]
        if path.endswith(".o"):
            object_rows.append(row)
        elif path.endswith(".asm"):
            listing_rows.append(row)
        elif path.endswith(".S"):
            residual_rows.append(row)
        else:
            unsupported.append(path)
    if unsupported:
        raise ObjectLinkageError(
            "unclassified selected-source paths: " + ", ".join(sorted(set(unsupported)))
        )

    def byte_sum(rows: list[dict[str, Any]]) -> int:
        try:
            return sum(int(row["new_bytes"]) for row in rows)
        except (KeyError, TypeError, ValueError) as exc:
            raise ObjectLinkageError("invalid selected-source byte count") from exc

    covered = byte_sum(selected)
    source_bytes = int(result.get("source_bytes", -1))
    if covered != source_bytes:
        raise ObjectLinkageError(
            f"selected-source coverage drift: {covered} != {source_bytes}"
        )

    direct_bytes = int(result.get("direct_object_bytes", 0))
    direct_objects = int(result.get("direct_object_inputs", 0))
    direct_candidate_objects = int(result.get("direct_candidate_object_inputs", 0))
    direct_sections = int(result.get("direct_object_sections", 0))
    incbin_bytes = int(result.get("incbin_payload_bytes", 0))
    incbin_sections = int(result.get("incbin_payload_sections", 0))
    if min(direct_bytes, direct_objects, direct_sections, incbin_bytes, incbin_sections) < 0:
        raise ObjectLinkageError("negative object-linkage metric")
    if direct_bytes + incbin_bytes != source_bytes:
        raise ObjectLinkageError(
            "direct/incbin coverage drift: "
            f"{direct_bytes} + {incbin_bytes} != {source_bytes}"
        )
    if not 0 <= direct_candidate_objects <= direct_objects:
        raise ObjectLinkageError("invalid direct candidate-object count")
    transport = _transport_contract(implementation, direct_bytes, incbin_bytes, direct_candidate_objects)
    return {
        "candidate_elf_object_bytes": byte_sum(object_rows),
        "candidate_elf_object_slices": len(object_rows),
        "candidate_elf_objects": len({row["object"] for row in object_rows}),
        "direct_candidate_object_inputs": direct_candidate_objects,
        "direct_object_bytes": direct_bytes,
        "direct_object_inputs": direct_objects,
        "direct_object_sections": direct_sections,
        "exact_code_bytes": source_bytes,
        "historical_recovered_object_bytes": int(
            result.get("historical_recovered_bytes", -1)
        ),
        "incbin_payload_bytes": incbin_bytes,
        "incbin_payload_sections": incbin_sections,
        "listing_bytes": int(result.get("listing_bytes", -1)),
        "listing_sources": len({row["object"] for row in listing_rows}),
        "object_native_complete": (
            direct_bytes == source_bytes
            and not transport["generated_binary_include"]
            and not listing_rows
        ),
        "prior_exact_assembly_bytes": int(
            result.get("prior_exact_assembly_bytes", -1)
        ),
        "remaining_object_native_bytes": source_bytes - direct_bytes,
        "residual_assembly_bytes": int(result.get("residual_bytes", -1)),
        "residual_assembly_slices": len(residual_rows),
        "selected_source_slices": len(selected),
        "transport": transport,
    }


def frozen_document(
    code_windows: dict[str, Any],
    implementation: str,
) -> dict[str, Any]:
    return {
        "claims": {
            "candidate_object_inventory_includes_direct_link_inputs": True,
            "exact_whole_image_is_proved": True,
            "object_native_linkage_complete": False,
            "private_target_payload_committed": False,
        },
        "format": FORMAT,
        "inputs": {
            "code_windows": "analysis/link_identity/code_windows.json",
            "implementation": "tools/code_windows.py",
        },
        "result": derive(code_windows, implementation),
        "schema_version": SCHEMA_VERSION,
    }


def validate(
    manifest_path: Path = DEFAULT_MANIFEST,
    code_windows_path: Path = DEFAULT_CODE_WINDOWS_MANIFEST,
    implementation_path: Path = DEFAULT_IMPLEMENTATION,
) -> dict[str, Any]:
    manifest = _load_json(manifest_path)
    code_windows = _load_json(code_windows_path)
    try:
        implementation = implementation_path.read_text(encoding="utf-8")
    except OSError as exc:
        raise ObjectLinkageError(
            f"cannot read {implementation_path}: {exc}"
        ) from exc
    expected = frozen_document(code_windows, implementation)
    if manifest != expected:
        raise ObjectLinkageError(
            "frozen object-linkage manifest drift; inspect the linker inputs "
            "and refresh the manifest only from verified evidence"
        )
    return manifest


def require_complete(document: dict[str, Any]) -> None:
    result = document["result"]
    if result.get("object_native_complete") is not True:
        raise ObjectLinkageError(
            "object-native linkage is incomplete: "
            f"{result['direct_object_bytes']}/{result['exact_code_bytes']} direct bytes; "
            f"{result['remaining_object_native_bytes']} remain behind "
            f"{result['transport']['transport']}"
        )


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("command", nargs="?", choices=("validate",), default="validate")
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument(
        "--code-windows-manifest",
        type=Path,
        default=DEFAULT_CODE_WINDOWS_MANIFEST,
    )
    parser.add_argument("--implementation", type=Path, default=DEFAULT_IMPLEMENTATION)
    parser.add_argument("--require-complete", action="store_true")
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        document = validate(
            args.manifest,
            args.code_windows_manifest,
            args.implementation,
        )
        if args.require_complete:
            require_complete(document)
    except ObjectLinkageError as exc:
        print(f"object-linkage audit: FAIL: {exc}", file=sys.stderr)
        return 1

    result = document["result"]
    print(
        "object-linkage audit: OK "
        f"(direct={result['direct_object_bytes']}/{result['exact_code_bytes']} bytes; "
        f"candidate_objects={result['candidate_elf_objects']}; "
        f"transport={result['transport']['transport']})"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
