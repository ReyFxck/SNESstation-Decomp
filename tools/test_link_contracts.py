#!/usr/bin/env python3
from __future__ import annotations

import argparse
import unittest

from link_contracts import (
    ABSOLUTE_ANCHOR,
    BLOCKED,
    DEFAULT_DEFINED,
    DEFAULT_EXTERNAL,
    DEFAULT_LAYOUT,
    DEFAULT_MANIFEST,
    DEFAULT_PROGRESS,
    DEFAULT_REVIEWS,
    DEFAULT_SOURCE_ALIASES,
    MANIFEST_FIELDS,
    RESOLVED,
    SEMANTIC_ALIAS,
    ContractError,
    NmSymbol,
    derive_rows,
    normalized_names,
    render_tsv,
    summarize,
    validate_frozen_manifest,
    verify_link_result,
)


def external(
    symbol: str,
    provider: str = "link-contract",
    category: str = "runtime-call-contract",
) -> dict[str, str]:
    return {
        "symbol": symbol,
        "category": category,
        "provider_kind": provider,
        "owner": "historical provider",
        "resolution_gate": "link-identity",
        "requesters": "ps2/requester.o",
    }


def defined(symbol: str) -> dict[str, str]:
    return {
        "symbol": symbol,
        "binding": "global",
        "section_class": "text",
        "size_hex": "0x8",
        "source": "src/example.c",
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


def source_alias(alias: str, status: str, canonical: str = "") -> dict[str, str]:
    return {
        "alias": alias,
        "target_address": "0x00100040",
        "status": status,
        "canonical_symbol": canonical,
        "evidence": "synthetic-source-alias",
        "canonical_source": "src/example.c" if canonical else "",
        "canonical_object": "example.o" if canonical else "",
        "requesters": "ps2/requester.o",
        "detail": "synthetic blocker" if status == BLOCKED else "",
    }


def review(symbol: str, canonical: str, address: str) -> dict[str, str]:
    return {
        "symbol": symbol,
        "canonical_symbol": canonical,
        "target_address": address,
        "evidence_path": "analysis/progress_targets.csv",
        "evidence_token": "synthetic",
        "detail": "reviewed synthetic identity",
    }


class LinkContractTests(unittest.TestCase):
    def test_address_anchor_and_semantic_aliases_are_strict(self) -> None:
        external_rows = [
            external("DAT_00100010", "program-data", "target-address-data"),
            external("plain_data", "program-data", "named-program-data"),
            external("exact_call"),
            external("suffix_call_00100030"),
            external("like_call_like"),
            external("runtime_recovered", "historical-archive"),
        ]
        defined_rows = [
            defined("exact_call_00100020"),
            defined("suffix_call_recovered"),
            defined("like_call_00100040"),
            defined("snes_runtime"),
        ]
        progress_rows = [
            progress("0x00100020", "exact_call"),
            progress("0x00100030", "suffix_call"),
            progress("0x00100040", "like_call"),
            progress("0x00100050", "runtime"),
        ]

        rows = derive_rows(
            external_rows,
            defined_rows,
            progress_rows,
            [],
            [],
            0x00100000,
            0x00200000,
        )
        by_symbol = {row["symbol"]: row for row in rows}
        self.assertEqual(ABSOLUTE_ANCHOR, by_symbol["DAT_00100010"]["resolution_kind"])
        self.assertEqual("0x00100010", by_symbol["DAT_00100010"]["target_address"])
        self.assertEqual(BLOCKED, by_symbol["plain_data"]["status"])
        for symbol in ("exact_call", "suffix_call_00100030", "like_call_like", "runtime_recovered"):
            self.assertEqual(RESOLVED, by_symbol[symbol]["status"])
            self.assertEqual(SEMANTIC_ALIAS, by_symbol[symbol]["resolution_kind"])
        self.assertEqual("exact_call_00100020", by_symbol["exact_call"]["canonical_symbol"])
        self.assertEqual("suffix_call_recovered", by_symbol["suffix_call_00100030"]["canonical_symbol"])
        self.assertEqual("like_call_00100040", by_symbol["like_call_like"]["canonical_symbol"])
        self.assertEqual("snes_runtime", by_symbol["runtime_recovered"]["canonical_symbol"])

    def test_normalization_composes_address_and_like_suffixes(self) -> None:
        variants = dict(normalized_names("fioPutc_like_0019d534"))
        self.assertEqual("strip-address", variants["fioPutc_like"])
        self.assertEqual("strip-address+strip-like", variants["fioPutc"])

    def test_ambiguous_semantic_candidate_stays_blocked(self) -> None:
        rows = derive_rows(
            [external("inflate")],
            [defined("inflate_recovered"), defined("snes_inflate")],
            [progress("0x00100020", "inflate")],
            [],
            [],
            0x00100000,
            0x00200000,
        )
        self.assertEqual(BLOCKED, rows[0]["status"])
        self.assertEqual("blocked-ambiguous-global-text", rows[0]["resolution_kind"])
        self.assertEqual("inflate_recovered;snes_inflate", rows[0]["detail"])

    def test_review_override_and_source_alias_carry_forward(self) -> None:
        external_rows = [
            external("seek_like_00100020"),
            external("FUN_00100040", "source-address-alias", "target-function-alias"),
            external("FUN_00100050", "source-address-alias", "target-function-alias"),
        ]
        rows = derive_rows(
            external_rows,
            [defined("historical_seek"), defined("chosen_00100040")],
            [progress("0x00100020", "seek"), progress("0x00100040", "chosen")],
            [
                source_alias("FUN_00100040", "PROVED", "chosen_00100040"),
                {**source_alias("FUN_00100050", BLOCKED), "target_address": "0x00100050"},
            ],
            [review("seek_like_00100020", "historical_seek", "0x00100020")],
            0x00100000,
            0x00200000,
        )
        self.assertEqual(["FUN_00100050", "seek_like_00100020"], [row["symbol"] for row in rows])
        by_symbol = {row["symbol"]: row for row in rows}
        self.assertEqual("historical_seek", by_symbol["seek_like_00100020"]["canonical_symbol"])
        self.assertEqual("reviewed-semantic-contract", by_symbol["seek_like_00100020"]["evidence"])
        self.assertEqual("blocked-source-address-alias", by_symbol["FUN_00100050"]["resolution_kind"])

    def test_address_anchor_must_be_inside_frozen_layout(self) -> None:
        with self.assertRaisesRegex(ContractError, "outside the unpacked layout"):
            derive_rows(
                [external("DAT_00300000", "program-data", "target-address-data")],
                [],
                [],
                [],
                [],
                0x00100000,
                0x00200000,
            )

    def test_render_is_deterministic_and_uses_frozen_columns(self) -> None:
        row = {field: "" for field in MANIFEST_FIELDS}
        row.update(
            {
                "symbol": "plain_data",
                "status": BLOCKED,
                "resolution_kind": "blocked-program-data-storage",
            }
        )
        rendered = render_tsv([row])
        self.assertEqual("\t".join(MANIFEST_FIELDS), rendered.splitlines()[0])
        self.assertEqual(rendered, render_tsv([row]))

    def test_link_result_verifies_anchor_alias_and_frontier(self) -> None:
        rows = [
            {
                **{field: "" for field in MANIFEST_FIELDS},
                "symbol": "DAT_00100010",
                "target_address": "0x00100010",
                "status": RESOLVED,
                "resolution_kind": ABSOLUTE_ANCHOR,
            },
            {
                **{field: "" for field in MANIFEST_FIELDS},
                "symbol": "call",
                "status": RESOLVED,
                "resolution_kind": SEMANTIC_ALIAS,
                "canonical_symbol": "call_00100020",
            },
            {
                **{field: "" for field in MANIFEST_FIELDS},
                "symbol": "still_missing",
                "status": BLOCKED,
            },
        ]
        input_symbols = [
            NmSymbol("DAT_00100010", "U"),
            NmSymbol("call", "U"),
            NmSymbol("still_missing", "U"),
            NmSymbol("call_00100020", "T", "00000020"),
        ]
        output_symbols = [
            NmSymbol("DAT_00100010", "A", "00100010"),
            NmSymbol("call", "T", "00000020"),
            NmSymbol("still_missing", "U"),
            NmSymbol("call_00100020", "T", "00000020"),
        ]
        self.assertEqual((3, 1), verify_link_result(input_symbols, output_symbols, rows))

    def test_frozen_repository_manifest_has_expected_live_counts(self) -> None:
        args = argparse.Namespace(
            external_map=DEFAULT_EXTERNAL,
            defined_map=DEFAULT_DEFINED,
            progress_manifest=DEFAULT_PROGRESS,
            source_alias_manifest=DEFAULT_SOURCE_ALIASES,
            layout_manifest=DEFAULT_LAYOUT,
            manifest=DEFAULT_MANIFEST,
            reviews=DEFAULT_REVIEWS,
        )
        rows = validate_frozen_manifest(args)
        report = summarize(rows)
        self.assertEqual(1570, report["contracts_total"])
        self.assertEqual(1336, report["resolved"])
        self.assertEqual(234, report["blocked"])
        self.assertEqual(1273, report["address_anchors"])
        self.assertEqual(63, report["semantic_aliases"])
        self.assertEqual(
            {
                "historical-archive": 2,
                "link-contract": 176,
                "private-asset": 10,
                "program-data": 32,
                "source-address-alias": 14,
            },
            report["blocked_provider_counts"],
        )


if __name__ == "__main__":
    unittest.main()
