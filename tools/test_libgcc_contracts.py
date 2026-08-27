import tempfile
import unittest
from collections import Counter
from pathlib import Path

import libgcc_contracts


class LibgccContractTests(unittest.TestCase):
    def test_historical_seven_row_ledger_is_closed(self) -> None:
        args = libgcc_contracts.parse_args(["validate"])
        rows = libgcc_contracts.validate_manifest(args)
        self.assertEqual(
            {
                libgcc_contracts.ARCHIVE_TEXT_EXACT: 4,
                libgcc_contracts.SOURCE_REFACTOR_CLOSED: 3,
            },
            dict(Counter(row["status"] for row in rows)),
        )

    def test_exact_member_geometry_is_frozen(self) -> None:
        rows = libgcc_contracts.read_table(
            libgcc_contracts.DEFAULT_MANIFEST, libgcc_contracts.MANIFEST_FIELDS
        )
        exact = [row for row in rows if row["status"] == libgcc_contracts.ARCHIVE_TEXT_EXACT]
        self.assertEqual(0xF08, sum(int(row["extent_hex"], 0) for row in exact))
        self.assertEqual(21, sum(int(row["relocation_count"]) for row in exact))
        self.assertEqual(
            {"_muldi3.o", "_floatdisf.o", "_udivdi3.o", "_umoddi3.o"},
            {row["archive_member"] for row in exact},
        )

    def test_refactors_are_absent_from_live_source_contracts(self) -> None:
        external = libgcc_contracts.read_table(
            libgcc_contracts.DEFAULT_EXTERNAL, libgcc_contracts.EXTERNAL_FIELDS
        )
        live = {row["symbol"] for row in external}
        self.assertTrue(libgcc_contracts.REFACTOR_SYMBOLS.isdisjoint(live))
        self.assertEqual(libgcc_contracts.EXACT_SYMBOLS, live & set(libgcc_contracts.SPEC_BY_SYMBOL))

    def test_public_gate_rejects_missing_member_hash(self) -> None:
        rows = libgcc_contracts.read_table(
            libgcc_contracts.DEFAULT_MANIFEST, libgcc_contracts.MANIFEST_FIELDS
        )
        rows[0]["member_text_sha256"] = ""
        with tempfile.TemporaryDirectory() as tmp:
            manifest = Path(tmp) / "libgcc_contracts.tsv"
            manifest.write_text(libgcc_contracts.render_tsv(rows), encoding="utf-8")
            args = libgcc_contracts.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaisesRegex(libgcc_contracts.LibgccContractError, "member_text_sha256"):
                libgcc_contracts.validate_manifest(args)


if __name__ == "__main__":
    unittest.main()
