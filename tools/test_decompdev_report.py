import copy
import json
import tempfile
import unittest
from pathlib import Path

import decompdev_report as gate


class DecompDevReportTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.contract = gate.load_contract()
        cls.report = gate.build_report()
        cls.categories = {row["id"]: row for row in cls.report["categories"]}

    def test_report_is_objdiff_v2_with_two_proof_levels(self):
        self.assertEqual(2, self.report["version"])
        self.assertEqual(
            {gate.FUNCTION_CATEGORY, gate.IMAGE_CATEGORY}, set(self.categories)
        )

    def test_function_gate_is_matching_but_not_link_complete(self):
        measures = self.categories[gate.FUNCTION_CATEGORY]["measures"]
        self.assertEqual(1041, measures["total_functions"])
        self.assertEqual(1041, measures["matched_functions"])
        self.assertEqual("722892", measures["matched_code"])
        self.assertEqual("0", measures["complete_code"])
        self.assertEqual(90, measures["total_units"])
        self.assertEqual(0, measures["complete_units"])

    def test_whole_image_reports_only_exact_chunks(self):
        measures = self.categories[gate.IMAGE_CATEGORY]["measures"]
        self.assertEqual(51, measures["total_units"])
        self.assertEqual(12, measures["complete_units"])
        self.assertEqual("3304936", measures["total_data"])
        self.assertEqual("786432", measures["matched_data"])
        self.assertEqual("786432", measures["complete_data"])

    def test_every_function_is_present_once(self):
        names = []
        addresses = []
        for unit in self.report["units"]:
            for function in unit["functions"]:
                names.append(function["name"])
                addresses.append(function["metadata"]["virtual_address"])
        self.assertEqual(1041, len(names))
        self.assertEqual(1041, len(set(addresses)))

    def test_uint64_fields_use_proto_json_strings(self):
        measures = [self.report["measures"]]
        measures.extend(unit["measures"] for unit in self.report["units"])
        measures.extend(category["measures"] for category in self.report["categories"])
        for value in measures:
            for field in gate.UINT64_MEASURE_FIELDS:
                self.assertIsInstance(value[field], str)
                self.assertTrue(value[field].isdigit())

    def test_report_contains_no_private_or_absolute_paths(self):
        encoded = json.dumps(self.report)
        for token in ("original/", "SNES_EMU.ELF", "/root/", "/storage/", "file://"):
            self.assertNotIn(token, encoded)
        for unit in self.report["units"]:
            source = unit["metadata"].get("source_path")
            if source:
                self.assertFalse(Path(source).is_absolute())

    def test_generation_is_deterministic_and_round_trips(self):
        with tempfile.TemporaryDirectory() as name:
            output = Path(name) / "report.json"
            first = gate.generate(output)
            first_bytes = output.read_bytes()
            second = gate.generate(output)
            self.assertEqual(first, second)
            self.assertEqual(first_bytes, output.read_bytes())
            loaded = json.loads(output.read_text(encoding="utf-8"))
            gate.validate_report(loaded, self.contract)

    def test_false_completion_or_metric_drift_is_rejected(self):
        changed = copy.deepcopy(self.report)
        changed["categories"][0]["measures"]["complete_units"] = 90
        with self.assertRaises(gate.DecompDevReportError):
            gate.validate_report(changed, self.contract)

        changed = copy.deepcopy(self.report)
        changed["categories"][1]["measures"]["matched_data"] = "3304936"
        with self.assertRaises(gate.DecompDevReportError):
            gate.validate_report(changed, self.contract)

    def test_workflow_publishes_the_discoverable_artifact(self):
        workflow = (gate.ROOT / ".github" / "workflows" / "decomp-dev.yml").read_text(
            encoding="utf-8"
        )
        self.assertIn("branches: [main]", workflow)
        self.assertIn("name: SNES_EMU_report", workflow)
        self.assertIn("path: build/decompdev/report.json", workflow)
        self.assertIn("actions/upload-artifact@v4", workflow)


if __name__ == "__main__":
    unittest.main()
