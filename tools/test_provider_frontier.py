#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import unittest

from provider_frontier import (
    ABSOLUTE_ANCHOR,
    COMPAT_STORAGE,
    CONTRACT_FIELDS,
    DEFAULT_CONTRACTS,
    DEFAULT_DEFINED,
    DEFAULT_MANIFEST,
    DEFAULT_PRIVATE,
    DEFINED_FIELDS,
    FrontierError,
    NmSymbol,
    PRIVATE_FIELDS,
    RUNTIME_SHIM,
    SEMANTIC_ALIAS,
    derive_rows,
    render_runtime_c,
    render_storage_assembly,
    summarize,
    validate_frozen_manifest,
    verify_input_frontier,
)


def read(path, fields):
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        assert tuple(reader.fieldnames or ()) == tuple(fields)
        return list(reader)


class ProviderFrontierTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.contracts = read(DEFAULT_CONTRACTS, CONTRACT_FIELDS)
        cls.private = read(DEFAULT_PRIVATE, PRIVATE_FIELDS)
        cls.defined = read(DEFAULT_DEFINED, DEFINED_FIELDS)

    def test_frozen_repository_manifest_closes_post_refactor_frontier(self) -> None:
        args = argparse.Namespace(
            contracts=DEFAULT_CONTRACTS,
            private_manifest=DEFAULT_PRIVATE,
            defined_map=DEFAULT_DEFINED,
            manifest=DEFAULT_MANIFEST,
        )
        rows = validate_frozen_manifest(args)
        self.assertEqual(
            {
                "frontier_total": 227,
                "absolute_anchors": 175,
                "semantic_aliases": 9,
                "compatibility_storage_symbols": 39,
                "runtime_shims": 4,
                "compatibility_storage_bytes": 144630,
            },
            summarize(rows),
        )

    def test_every_row_has_one_resolution_mechanism(self) -> None:
        rows = derive_rows(self.contracts, self.private, self.defined)
        by_kind = {
            kind: {row["symbol"] for row in rows if row["resolution_kind"] == kind}
            for kind in (ABSOLUTE_ANCHOR, SEMANTIC_ALIAS, COMPAT_STORAGE, RUNTIME_SHIM)
        }
        self.assertEqual(227, len(set().union(*by_kind.values())))
        self.assertFalse(any(by_kind[a] & by_kind[b] for a in by_kind for b in by_kind if a < b))
        self.assertEqual("snes_fatal_spin_00107578", next(
            row["target_symbol"] for row in rows if row["symbol"] == "abort"
        ))
        self.assertEqual("0x12001000", next(
            row["target_address"] for row in rows if row["symbol"] == "REG_GS_CSR"
        ))

    def test_frontier_drift_is_rejected(self) -> None:
        contracts = self.contracts[:-1]
        with self.assertRaisesRegex(FrontierError, "227"):
            derive_rows(contracts, self.private, self.defined)

    def test_generated_sources_cover_storage_and_runtime_without_payloads(self) -> None:
        rows = derive_rows(self.contracts, self.private, self.defined)
        assembly = render_storage_assembly(rows)
        runtime = render_runtime_c()
        self.assertEqual(39, assembly.count(".type "))
        self.assertIn(".bss.stage3.compatibility", assembly)
        self.assertIn("p_u128 __ashlti3", runtime)
        self.assertIn("int snprintf", runtime)
        self.assertNotIn("ps2_add_intc_handler_recovered", runtime)
        self.assertNotIn("long lrintf", runtime)
        self.assertNotIn("__builtin_va_start", runtime)

    def test_input_frontier_must_be_exact(self) -> None:
        rows = derive_rows(self.contracts, self.private, self.defined)
        symbols = [NmSymbol(row["symbol"], "U") for row in rows]
        self.assertEqual(227, len(verify_input_frontier(symbols, rows)))
        with self.assertRaisesRegex(FrontierError, "drift"):
            verify_input_frontier(symbols[:-1], rows)


if __name__ == "__main__":
    unittest.main()
