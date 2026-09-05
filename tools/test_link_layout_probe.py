import copy
import json
import tempfile
import unittest
from pathlib import Path

import link_layout_probe as gate


class LinkLayoutProbeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)
        cls.sections = gate.load_sections(cls.args.sections)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "probe.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.LinkLayoutProbeError):
                gate.validate(args)

    def test_frozen_result_is_the_measured_diagnostic(self):
        result = self.document["result"]
        for key, value in gate.EXPECTED.items():
            self.assertEqual(value, result[key])
        self.assertNotEqual(result["diagnostic_padded_sha256"], result["target_sha256"])

    def test_fixed_section_roster_is_complete(self):
        self.assertEqual(179, len(self.sections))
        self.assertEqual(155, sum(row["region"] == "initialized" for row in self.sections))
        self.assertEqual(24, sum(row["region"] == "zero-fill" for row in self.sections))

    def test_accounting_is_exact(self):
        result = self.document["result"]
        self.assertEqual(51, result["exact_chunks"] + result["mismatching_chunks"])
        self.assertEqual(
            result["target_initialized_size"],
            result["equal_bytes"] + result["differing_bytes"],
        )
        self.assertEqual(100, result["terminal_zero_padding"])

    def test_claims_refuse_replacement_and_packing(self):
        claims = self.document["claims"]
        self.assertTrue(claims["fixed_section_vmas_and_sizes_proved"])
        for name in (
            "complete_object_extents_proved",
            "exact_implementation_selection_complete",
            "replacement_elf",
            "sjcrunch2_packing_reproduced",
            "packed_hash_matched",
            "unpacked_hash_matched",
        ):
            self.assertFalse(claims[name])

    def test_metric_or_claim_drift_is_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["result"]["differing_bytes"] -= 1
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["claims"]["replacement_elf"] = True
        self.reject(changed)

    def test_script_places_every_proved_section_explicitly(self):
        script = gate.render_script(self.sections)
        self.assertIn("ENTRY(snes_p28_00100008)", script)
        for row in self.sections:
            self.assertIn(f"{row['section']} {row['target_address']}", script)


if __name__ == "__main__":
    unittest.main()
