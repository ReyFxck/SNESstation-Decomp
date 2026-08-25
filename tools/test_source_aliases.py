#!/usr/bin/env python3
from __future__ import annotations

import argparse
import unittest

from source_aliases import (
    BLOCKED,
    DEFAULT_DEFINED,
    DEFAULT_EXTERNAL,
    DEFAULT_MANIFEST,
    DEFAULT_PROGRESS,
    MANIFEST_FIELDS,
    PROVED,
    AliasError,
    NmSymbol,
    derive_rows,
    render_tsv,
    summarize,
    validate_frozen_manifest,
    verify_link_result,
)


def external(symbol: str, owner: str = "src/example.c") -> dict[str, str]:
    return {
        "symbol": symbol,
        "category": "target-function-alias",
        "provider_kind": "source-address-alias",
        "owner": owner,
        "resolution_gate": "link-identity",
        "requesters": "ps2/requester.o",
    }


def defined(symbol: str, source: str = "src/example.c") -> dict[str, str]:
    return {
        "symbol": symbol,
        "binding": "global",
        "section_class": "text",
        "size_hex": "0x8",
        "source": source,
        "object": "example.o",
    }


def progress(address: str, name: str) -> dict[str, str]:
    return {
        "address": address,
        "name": name,
        "area": "test",
        "status": "MATCHING",
        "confidence": "very-high",
        "notes": "synthetic",
    }


class SourceAliasTests(unittest.TestCase):
    def test_strict_evidence_classifies_proved_and_blocked_rows(self) -> None:
        external_rows = [
            external("FUN_00100010"),
            external("FUN_00100020"),
            external("FUN_00100030"),
            external("FUN_00100040"),
            external("FUN_00100050", owner="0x00100050"),
        ]
        defined_rows = [
            defined("readable_name"),
            defined("only_candidate_00100020"),
            defined("candidate_a_00100030"),
            defined("candidate_b_00100030"),
            {
                **defined("local_only_00100040"),
                "binding": "local",
            },
        ]
        progress_rows = [
            progress("0x00100010", "readable_name"),
            progress("0x00100020", "historical_name"),
            progress("0x00100030", "ambiguous_name"),
            progress("0x00100040", "not_exported"),
        ]

        rows = derive_rows(external_rows, defined_rows, progress_rows)
        by_alias = {row["alias"]: row for row in rows}

        self.assertEqual(PROVED, by_alias["FUN_00100010"]["status"])
        self.assertEqual("readable_name", by_alias["FUN_00100010"]["canonical_symbol"])
        self.assertEqual(
            "progress-name-global-text", by_alias["FUN_00100010"]["evidence"]
        )
        self.assertEqual(PROVED, by_alias["FUN_00100020"]["status"])
        self.assertEqual(
            "unique-address-suffix-global-text",
            by_alias["FUN_00100020"]["evidence"],
        )
        self.assertEqual(BLOCKED, by_alias["FUN_00100030"]["status"])
        self.assertEqual(
            "candidate_a_00100030;candidate_b_00100030",
            by_alias["FUN_00100030"]["detail"],
        )
        self.assertEqual(
            "progress-target-not-exported", by_alias["FUN_00100040"]["evidence"]
        )
        self.assertEqual(
            "address-outside-progress-manifest", by_alias["FUN_00100050"]["evidence"]
        )

    def test_candidate_must_belong_to_frozen_owner(self) -> None:
        with self.assertRaisesRegex(AliasError, "candidate owner mismatch"):
            derive_rows(
                [external("FUN_00100010", owner="src/other.c")],
                [defined("candidate_00100010")],
                [progress("0x00100010", "historical_name")],
            )

    def test_render_is_deterministic_and_uses_frozen_columns(self) -> None:
        row = {field: "" for field in MANIFEST_FIELDS}
        row.update(
            {
                "alias": "FUN_00100010",
                "target_address": "0x00100010",
                "status": BLOCKED,
                "evidence": "address-outside-progress-manifest",
            }
        )
        rendered = render_tsv([row])
        self.assertEqual("\t".join(MANIFEST_FIELDS), rendered.splitlines()[0])
        self.assertEqual(rendered, render_tsv([row]))

    def test_link_result_removes_only_proved_aliases(self) -> None:
        rows = [
            {
                **{field: "" for field in MANIFEST_FIELDS},
                "alias": "FUN_00100010",
                "status": PROVED,
                "canonical_symbol": "readable_name",
            },
            {
                **{field: "" for field in MANIFEST_FIELDS},
                "alias": "FUN_00100020",
                "status": BLOCKED,
            },
        ]
        input_symbols = [
            NmSymbol("FUN_00100010", "U"),
            NmSymbol("FUN_00100020", "U"),
            NmSymbol("readable_name", "T", "00000020", "00000008"),
        ]
        output_symbols = [
            NmSymbol("FUN_00100010", "T", "00000020"),
            NmSymbol("FUN_00100020", "U"),
            NmSymbol("readable_name", "T", "00000020", "00000008"),
        ]
        self.assertEqual(
            (2, 1),
            verify_link_result(
                input_symbols,
                output_symbols,
                rows,
                {"FUN_00100010", "FUN_00100020"},
            ),
        )

    def test_frozen_repository_manifest_has_expected_v83_counts(self) -> None:
        args = argparse.Namespace(
            external_map=DEFAULT_EXTERNAL,
            defined_map=DEFAULT_DEFINED,
            progress_manifest=DEFAULT_PROGRESS,
            manifest=DEFAULT_MANIFEST,
        )
        rows = validate_frozen_manifest(args)
        report = summarize(rows)

        self.assertEqual(337, report["aliases_total"])
        self.assertEqual(257, report["proved"])
        self.assertEqual(80, report["blocked"])
        self.assertEqual(242, report["canonical_targets"])
        self.assertEqual(
            {
                "address-outside-progress-manifest": 6,
                "ambiguous-address-suffix-global-text": 1,
                "progress-name-global-text": 122,
                "progress-target-not-exported": 73,
                "unique-address-suffix-global-text": 135,
            },
            report["evidence_counts"],
        )


if __name__ == "__main__":
    unittest.main()
