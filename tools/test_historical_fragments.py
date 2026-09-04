import copy
import json
import tempfile
import unittest
from pathlib import Path
import historical_fragments as h

class HistoricalFragmentTests(unittest.TestCase):
    def setUp(self):
        self.data = h.validate()

    def reject(self, mutate):
        data = copy.deepcopy(self.data)
        mutate(data)
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "manifest.json"
            path.write_text(json.dumps(data), encoding="utf-8")
            with self.assertRaises((h.HistoricalFragmentError, ValueError, KeyError)):
                h.validate(path)

    def test_scope(self):
        self.assertEqual(3, len(self.data["owners"]))
        self.assertEqual(0x880, sum(r["size"] for r in self.data["owners"]))
        self.assertFalse(self.data["replacement_elf"])

    def test_geometry(self):
        self.assertEqual(
            [
                ("c4emu_prefix",0x3359d0,0x80,0),
                ("C4SinTable",0x335a50,0x400,0x80),
                ("C4CosTable",0x335e50,0x400,0x480),
            ],
            [(r["symbol"],r["address"],r["size"],r["source_offset"])
             for r in self.data["owners"]],
        )

    def test_no_relocations(self):
        self.assertFalse([x for x in self.data["data_relocations"] if 0 <= x < 0x880])

    def test_geometry_drift_rejected(self):
        self.reject(lambda d: d["owners"][1].update(source_offset=0x84))

    def test_replacement_claim_rejected(self):
        self.reject(lambda d: d.update(replacement_elf=True))

if __name__ == "__main__":
    unittest.main()
