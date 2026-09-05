#!/usr/bin/env python3
"""Public regression guards for the exact Stage-3G startup integration."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import startup_integration as gate


class StartupIntegrationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)
        cls.sections, cls.layout = gate.load_inputs(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "startup.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.StartupIntegrationError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_exact_entry_startup_and_function_contract(self):
        result = self.document["result"]
        self.assertEqual(0x00100008, result["integrated_entry_address"])
        self.assertEqual(0x00100114, result["first_differing_address"])
        self.assertEqual(276, result["startup_exact_bytes"])
        self.assertEqual(3, result["startup_functions_exact"])
        self.assertEqual(27, result["startup_relocations_applied"])
        self.assertEqual(
            {"_start": (0x00100008, 0xD8), "_exit": (0x001000E0, 0x2C), "_root": (0x0010010C, 8)},
            {name: (row["address"], row["size"]) for name, row in self.document["startup_symbols"].items() if name.startswith("_") and name not in {"_args", "_args_ptr"}},
        )

    def test_historical_source_and_compiler_are_pinned(self):
        source = self.document["source_contract"]
        self.assertEqual(gate.SOURCE_REPOSITORY, source["repository"])
        self.assertEqual(gate.SOURCE_REVISION, source["revision"])
        self.assertEqual(gate.SOURCE_PATH, source["path"])
        self.assertEqual(gate.SOURCE_SHA256, source["source_sha256"])
        self.assertEqual("3.2.2", source["compiler_base_version"])
        self.assertEqual("ee", source["compiler_target"])

    def test_linker_script_places_startup_and_preserves_fixed_sections(self):
        script = gate.render_script(self.sections)
        self.assertIn("ENTRY(_start)", script)
        self.assertIn(".startup 0x00100000", script)
        self.assertIn(".text 0x00100114", script)
        for row in self.sections:
            if row["section"] in gate.ABSORBED_ZERO_FILL:
                self.assertIn(f"*({row['section']})", script)
            else:
                self.assertIn(f"{row['section']} {row['target_address']}", script)

    def test_startup_bss_absorbs_only_four_proved_zero_fills(self):
        result = self.document["result"]
        self.assertEqual(0x00426E80, result["startup_bss_address"])
        self.assertEqual(0x180, result["startup_bss_size"])
        self.assertEqual(4, result["absorbed_zero_fill_sections"])
        self.assertEqual(175, result["preserved_fixed_sections"])
        self.assertEqual(179, result["fixed_sections_accounted"])

    def test_whole_image_accounting_remains_explicitly_incomplete(self):
        result, claims = self.document["result"], self.document["claims"]
        self.assertEqual(12, result["exact_chunks"])
        self.assertEqual(39, result["mismatching_chunks"])
        self.assertEqual(result["target_initialized_size"], result["equal_bytes"] + result["differing_bytes"])
        self.assertNotEqual(result["integrated_padded_sha256"], result["target_sha256"])
        for name in ("behavioral_startup_lift_removed", "exact_implementation_selection_complete",
                     "historical_link_order_proved", "replacement_elf",
                     "sjcrunch2_packing_reproduced", "packed_hash_matched", "unpacked_hash_matched"):
            self.assertFalse(claims[name])

    def test_metric_and_false_completion_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["result"]["startup_exact_bytes"] -= 4
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["claims"]["replacement_elf"] = True
        self.reject(changed)


if __name__ == "__main__":
    unittest.main()
