import copy
import tempfile
import unittest
from pathlib import Path

import final_residual_identities as gate


class FinalResidualIdentityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.rows = gate.validate()

    def reject(self, rows):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "final.tsv"
            path.write_text(gate.render(rows), encoding="utf-8")
            with self.assertRaises(gate.FinalResidualError):
                gate.validate(path)

    def test_exact_roster(self):
        self.assertEqual(
            {"DAT_0042355a", "DAT_00426820"},
            {row["symbol"] for row in self.rows},
        )

    def test_distinct_closure_statuses(self):
        by_name = {row["symbol"]: row for row in self.rows}
        self.assertEqual(
            gate.RTC_STATUS,
            by_name["DAT_0042355a"]["status"],
        )
        self.assertEqual(
            gate.EH_STATUS,
            by_name["DAT_00426820"]["status"],
        )

    def test_rtc_geometry_is_frozen(self):
        row = {r["symbol"]: r for r in self.rows}["DAT_0042355a"]
        self.assertEqual("0x00423548", row["container_start"])
        self.assertEqual("0x18", row["container_extent_hex"])
        self.assertEqual("0x12", row["field_offset_hex"])
        self.assertEqual("0x1", row["field_extent_hex"])

    def test_eh_frame_geometry_is_frozen(self):
        row = {r["symbol"]: r for r in self.rows}["DAT_00426820"]
        self.assertEqual("0x0042680c", row["container_start"])
        self.assertEqual("0x34", row["container_extent_hex"])
        self.assertEqual("0x14", row["field_offset_hex"])
        self.assertEqual("0x1", row["field_extent_hex"])

    def test_no_storage_section_claim_in_final_manifest(self):
        for row in self.rows:
            self.assertNotIn("SECTION_BACKED_ADDRESS", row["status"])
            self.assertTrue(row["proof_kind"])

    def test_identity_drift_is_rejected(self):
        rows = copy.deepcopy(self.rows)
        rows[0]["identity"] = "wrong"
        self.reject(rows)

    def test_claim_drift_is_rejected(self):
        rows = copy.deepcopy(self.rows)
        rows[1]["claim"] += " drift"
        self.reject(rows)

    def test_evidence_is_durable_not_an_ignored_part_report(self):
        self.assertFalse(hasattr(gate, "PART5C_REPORT"))
        self.assertFalse(hasattr(gate, "PART5D_REPORT"))
        evidence = " ".join(row["evidence"] for row in self.rows)
        self.assertIn("fresh pinned V74", evidence)
        self.assertIn("target-native CIE/FDE", evidence)


if __name__ == "__main__":
    unittest.main()
