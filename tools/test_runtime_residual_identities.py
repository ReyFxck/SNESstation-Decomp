import copy
import tempfile
import unittest
from pathlib import Path

import runtime_residual_identities as gate


class RuntimeResidualIdentityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.rows = gate.validate()

    def reject(self, rows):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "runtime.tsv"
            path.write_text(gate.render(rows), encoding="utf-8")
            with self.assertRaises(gate.RuntimeResidualError):
                gate.validate(path)

    def test_exact_roster(self):
        self.assertEqual(
            {"DAT_001babc8", "UNK_001ba7e0", "UNK_001a6320"},
            {row["symbol"] for row in self.rows},
        )

    def test_two_data_one_code(self):
        self.assertEqual(
            2,
            sum(row["status"] == gate.DATA_STATUS for row in self.rows),
        )
        self.assertEqual(
            1,
            sum(row["status"] == gate.CODE_STATUS for row in self.rows),
        )

    def test_exact_identities(self):
        by_name = {row["symbol"]: row for row in self.rows}
        self.assertEqual("__clz_tab", by_name["DAT_001babc8"]["identity"])
        self.assertEqual("__thenan_df", by_name["UNK_001ba7e0"]["identity"])
        self.assertEqual(
            "fde_unencoded_compare",
            by_name["UNK_001a6320"]["identity"],
        )

    def test_exact_extents(self):
        by_name = {row["symbol"]: row for row in self.rows}
        self.assertEqual("0x100", by_name["DAT_001babc8"]["extent_hex"])
        self.assertEqual("0x18", by_name["UNK_001ba7e0"]["extent_hex"])
        self.assertEqual("0x28", by_name["UNK_001a6320"]["extent_hex"])

    def test_archive_copy_count_is_four(self):
        for row in self.rows:
            self.assertEqual("4", row["archive_copy_count"])

    def test_archive_roster_is_explicit_not_report_discovery(self):
        self.assertEqual(4, len(gate.EXPECTED_ARCHIVES))
        self.assertFalse(hasattr(gate, "PART4D_REPORT"))
        self.assertEqual(4, len({str(path) for path in gate.EXPECTED_ARCHIVES}))

    def test_identity_drift_rejected(self):
        rows = copy.deepcopy(self.rows)
        rows[0]["identity"] = "wrong"
        self.reject(rows)


if __name__ == "__main__":
    unittest.main()
