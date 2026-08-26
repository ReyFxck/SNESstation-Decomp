#!/usr/bin/env python3
from __future__ import annotations

import argparse
import hashlib
import struct
import unittest
from pathlib import Path

from private_asset_providers import (
    ASSET_SYMBOLS,
    BLOCKED,
    DEFAULT_ASSETS,
    DEFAULT_CONTRACTS,
    DEFAULT_LAYOUT,
    DEFAULT_MANIFEST,
    Layout,
    NmSymbol,
    ProviderError,
    derive_rows,
    render_assembly,
    summarize,
    validate_frozen_manifest,
    validate_reference,
    verify_link_result,
)


def asset_row(name: str, va: int, data: bytes) -> dict[str, str]:
    size = len(data)
    size_word_va = (va + size + 3) & ~3
    return {
        "name": name,
        "kind": "synthetic",
        "va_start": f"0x{va:x}",
        "va_end": f"0x{va + size:x}",
        "size_hex": f"0x{size:x}",
        "size_dec": str(size),
        "sha256": hashlib.sha256(data).hexdigest(),
        "decoded_sha256": "",
        "size_word_va": f"0x{size_word_va:x}",
        "consumer": "synthetic",
    }


def contract(symbol: str, requester: str = "app/example.o") -> dict[str, str]:
    return {
        "symbol": symbol,
        "target_address": "",
        "status": BLOCKED,
        "resolution_kind": "blocked-private-asset",
        "canonical_symbol": "",
        "evidence": "provider-frontier",
        "category": "embedded-binary",
        "provider_kind": "private-asset",
        "canonical_source": "",
        "canonical_object": "",
        "requesters": requester,
        "detail": "synthetic",
    }


class PrivateAssetProviderTests(unittest.TestCase):
    def test_derivation_maps_exact_private_contract_set(self) -> None:
        base = 0x1000
        assets = [
            asset_row(asset, base + index * 0x20, bytes([index + 1]) * 3)
            for index, (asset, _data, _size) in enumerate(ASSET_SYMBOLS)
        ]
        contracts = [
            contract(symbol)
            for _asset, data_symbol, size_symbol in ASSET_SYMBOLS
            for symbol in (data_symbol, size_symbol)
        ]
        rows = derive_rows(assets, contracts, Layout(base, 0x1000, "0" * 64))
        self.assertEqual(5, len(rows))
        self.assertEqual(10, len({name for row in rows for name in (row["data_symbol"], row["size_symbol"])}))
        self.assertTrue(all(row["padding_hex"] == "0x1" for row in rows))
        self.assertTrue(all(row["section_alignment_hex"] == "0x4" for row in rows))

    def test_derivation_rejects_unmapped_private_contract(self) -> None:
        assets = [
            asset_row(asset, 0x1000 + index * 0x20, b"abc")
            for index, (asset, _data, _size) in enumerate(ASSET_SYMBOLS)
        ]
        contracts = [
            contract(symbol)
            for _asset, data_symbol, size_symbol in ASSET_SYMBOLS
            for symbol in (data_symbol, size_symbol)
        ]
        contracts.append(contract("unexpected_private_asset"))
        with self.assertRaisesRegex(ProviderError, "mapping drift"):
            derive_rows(assets, contracts, Layout(0x1000, 0x1000, "0" * 64))

    def test_private_reference_checks_hash_padding_and_size_word(self) -> None:
        payload = b"abc\0" + struct.pack("<I", 3)
        layout = Layout(0x1000, len(payload), hashlib.sha256(payload).hexdigest())
        row = {
            "asset": "synthetic",
            "data_symbol": "asset_data",
            "size_symbol": "asset_size",
            "target_va": "0x00001000",
            "size_hex": "0x3",
            "sha256": hashlib.sha256(b"abc").hexdigest(),
            "size_word_va": "0x00001004",
            "padding_hex": "0x1",
            "section_name": ".data.private_asset.synthetic",
            "section_alignment_hex": "0x4",
            "requesters": "app/example.o",
        }
        sections = validate_reference(payload, layout, [row])
        self.assertEqual(payload, sections[row["section_name"]])

        corrupt = b"abc\xff" + struct.pack("<I", 3)
        bad_layout = Layout(0x1000, len(corrupt), hashlib.sha256(corrupt).hexdigest())
        with self.assertRaisesRegex(ProviderError, "nonzero target padding"):
            validate_reference(corrupt, bad_layout, [row])

    def test_assembly_uses_incbin_and_does_not_inline_payload(self) -> None:
        row = {
            "asset": "synthetic",
            "data_symbol": "asset_data",
            "size_symbol": "asset_size",
            "target_va": "0x00001020",
            "size_hex": "0x3",
            "sha256": "0" * 64,
            "size_word_va": "0x00001024",
            "padding_hex": "0x1",
            "section_name": ".data.private_asset.synthetic",
            "section_alignment_hex": "0x4",
            "requesters": "app/example.o",
        }
        assembly = render_assembly(Path("private-reference.bin"), Layout(0x1000, 0x100, "0" * 64), [row])
        self.assertIn('.incbin "', assembly)
        self.assertIn(",32,3", assembly)
        self.assertIn(".space 1,0", assembly)
        self.assertIn(".word 3", assembly)

    def test_link_frontier_removes_only_provider_symbols(self) -> None:
        rows = [
            {
                "data_symbol": "asset_data",
                "size_symbol": "asset_size",
            }
        ]
        contracts = [contract("asset_data"), contract("asset_size"), {**contract("still_missing"), "provider_kind": "link-contract"}]
        input_symbols = [NmSymbol("asset_data", "U"), NmSymbol("asset_size", "U"), NmSymbol("still_missing", "U")]
        output_symbols = [NmSymbol("asset_data", "D", "0", "3"), NmSymbol("asset_size", "D", "4", "4"), NmSymbol("still_missing", "U")]
        self.assertEqual((3, 1), verify_link_result(input_symbols, output_symbols, contracts, rows))

    def test_frozen_repository_manifest_has_expected_v86_counts(self) -> None:
        args = argparse.Namespace(
            assets=DEFAULT_ASSETS,
            contracts=DEFAULT_CONTRACTS,
            layout_manifest=DEFAULT_LAYOUT,
            manifest=DEFAULT_MANIFEST,
        )
        rows, _layout = validate_frozen_manifest(args)
        import csv

        with DEFAULT_CONTRACTS.open(encoding="utf-8", newline="") as stream:
            contracts = list(csv.DictReader(stream, delimiter="\t"))
        report = summarize(rows, contracts)
        self.assertEqual(
            {
                "bundles": 5,
                "provider_symbols": 10,
                "provider_bytes": 62736,
                "input_frontier": 258,
                "output_frontier": 248,
            },
            report,
        )


if __name__ == "__main__":
    unittest.main()
