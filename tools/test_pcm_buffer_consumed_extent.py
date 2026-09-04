import copy
import tempfile
import unittest
from pathlib import Path

import pcm_buffer_consumed_extent as gate


class PcmBufferConsumedExtentTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.rows = gate.validate()

    def reject(self, rows):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "pcm.tsv"
            path.write_text(gate.render(rows), encoding="utf-8")
            with self.assertRaises(gate.PcmExtentError):
                gate.validate(path)

    def test_two_owners_eight_aliases(self):
        self.assertEqual({"pcm_left","pcm_right"}, {r["owner"] for r in self.rows})
        self.assertEqual(8, len(gate.SYMBOL_TO_OWNER))

    def test_exact_minimum_extent(self):
        self.assertEqual(0x3C00, gate.MINIMUM_EXTENT)
        self.assertEqual(0x3BFE, gate.WORST_HALFWORD_OFFSET)
        self.assertEqual(1920, gate.MAX_MIXER_COUNT)

    def test_claim_is_minimum_only(self):
        for row in self.rows:
            self.assertIn("minimum target-consumed", row["claim"])
            self.assertIn("complete original C array extent remains unclaimed", row["claim"])

    def test_owner_ranges_do_not_overlap(self):
        left = next(r for r in self.rows if r["owner"] == "pcm_left")
        right = next(r for r in self.rows if r["owner"] == "pcm_right")
        self.assertLessEqual(int(left["end_address"],0), int(right["base_address"],0))

    def test_private_hash_required(self):
        rows = copy.deepcopy(self.rows)
        rows[0]["private_sha256"] = "0" * 63
        self.reject(rows)

    def test_alias_roster_drift_rejected(self):
        rows = copy.deepcopy(self.rows)
        rows[0]["aliases"] = rows[0]["aliases"].replace("DAT_001bbd86", "DAT_BAD")
        self.reject(rows)

    def test_byte_count_total_is_extent(self):
        for row in self.rows:
            self.assertEqual(
                gate.MINIMUM_EXTENT,
                int(row["zero_bytes"]) + int(row["nonzero_bytes"]),
            )


if __name__ == "__main__":
    unittest.main()
