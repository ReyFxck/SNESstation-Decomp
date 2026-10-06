#!/usr/bin/env python3
"""Refresh public ownership provenance without changing frozen binary evidence.

Only requester-derived section evidence and hashes of preceding public
manifests may change. Geometry, payload hashes, source pins and comparison
results remain frozen; any other drift requires the original capture gates.
"""
from __future__ import annotations

import hashlib
import json

import code_windows
import data_backing
import frontend_eh_frames
import historical_tail_data
import libgcc_contracts
import link_layout_probe
import media_assets
import runtime_members
import runtime_tail_data
import startup_integration
import tail_metadata
import window11_rodata
import window35_data
import window36_data
import unnamed_data
from build_source_tree import render_tsv


def refresh_contract(path, expected, allowed):
    document = json.loads(path.read_text(encoding="utf-8"))
    found = document["source_contract"]
    changed = {key for key in found.keys() | expected.keys() if found.get(key) != expected.get(key)}
    if not changed <= allowed:
        raise ValueError(f"recapture required for {path.name}: {sorted(changed - allowed)}")
    if changed:
        document["source_contract"] = expected
        path.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n")
        print(f"refreshed {path.name}: {', '.join(sorted(changed))}")


def refresh_prior(module, prior):
    path = module.DEFAULT_MANIFEST
    document = json.loads(path.read_text(encoding="utf-8"))
    digest = hashlib.sha256(prior.DEFAULT_MANIFEST.read_bytes()).hexdigest()
    if document["prior_manifest_sha256"] != digest:
        document["prior_manifest_sha256"] = digest
        path.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n")
        print(f"refreshed {path.name}: prior_manifest_sha256")
    module.validate(module.parse_args(["validate"]))


def refresh_requesters(path, fields, expected):
    """Change consumer ownership only; retain every captured proof field."""
    frozen = libgcc_contracts.read_table(path, fields)
    if len(frozen) != len(expected):
        raise ValueError(f"requester roster changed: {path.name}; recapture required")
    for found, wanted in zip(frozen, expected):
        if any(found[key] != wanted[key] for key in fields if key != "requesters"):
            raise ValueError(f"requester proof changed: {path.name}; recapture required")
    if frozen != expected:
        path.write_text(render_tsv(fields, expected))
        print(f"refreshed {path.name}: requesters only")


def main():
    access_args = unnamed_data.parse_args(["validate"])
    frozen = libgcc_contracts.read_table(access_args.manifest, unnamed_data.FIELDS)
    external = unnamed_data.external_rows(access_args.external_map)
    if [row["symbol"] for row in frozen] != [row["symbol"] for row in external]:
        raise ValueError("unnamed-data roster changed; recapture required")
    expected = [{**row, "requesters": ext["requesters"]}
                for row, ext in zip(frozen, external)]
    refresh_requesters(access_args.manifest, unnamed_data.FIELDS, expected)
    unnamed_data.validate_manifest(access_args)
    args = data_backing.parse_args(["validate"])
    rows, sections = data_backing.derive(args)
    refresh_requesters(args.manifest, data_backing.FIELDS, rows)
    frozen = libgcc_contracts.read_table(args.sections, data_backing.SECTION_FIELDS)
    if len(frozen) != len(sections):
        raise ValueError("data-backing section count changed; recapture required")
    for expected, found in zip(sections, frozen):
        geometry = set(data_backing.SECTION_FIELDS) - {"sha256", "evidence_sha256"}
        if any(found[key] != expected[key] for key in geometry):
            raise ValueError(f"data-backing geometry changed: {expected['section']}; recapture required")
        if not libgcc_contracts.SHA_RE.fullmatch(found["sha256"]):
            raise ValueError(f"missing frozen payload hash: {expected['section']}")
        if found["evidence_sha256"] != expected["evidence_sha256"]:
            found["evidence_sha256"] = expected["evidence_sha256"]
            print(f"refreshed section provenance: {expected['section']}")
    args.sections.write_text(runtime_members.render(data_backing.SECTION_FIELDS, frozen))
    data_backing.validate(args)

    args = link_layout_probe.parse_args(["validate"])
    sections = link_layout_probe.load_sections(args.sections)
    layout = link_layout_probe.load_layout(args.layout)
    refresh_contract(args.manifest, link_layout_probe.source_contract(args, sections, layout),
                     {"stage3f_sections_sha256"})
    link_layout_probe.validate(args)

    args = startup_integration.parse_args(["validate"])
    refresh_contract(args.manifest, startup_integration.source_contract(args, sections, layout),
                     {"data_backing_sections_sha256"})
    startup_integration.validate(args)

    args = frontend_eh_frames.parse_args(["validate"])
    refresh_contract(args.manifest, frontend_eh_frames.source_contract(args),
                     {"startup_manifest_sha256"})
    frontend_eh_frames.validate(args)

    args = historical_tail_data.parse_args(["validate"])
    refresh_contract(args.manifest, historical_tail_data.source_contract(args),
                     {"frontend_manifest_sha256"})
    historical_tail_data.validate(args)

    for module, prior in (
        (runtime_tail_data, historical_tail_data),
        (tail_metadata, runtime_tail_data),
        (window36_data, tail_metadata),
        (media_assets, window36_data),
        (window35_data, media_assets),
        (window11_rodata, window35_data),
        (code_windows, window11_rodata),
    ):
        refresh_prior(module, prior)
    print("public provenance refreshed; frozen payload hashes and comparison results preserved")


if __name__ == "__main__":
    main()
