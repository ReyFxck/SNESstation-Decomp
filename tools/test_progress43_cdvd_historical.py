from pathlib import Path
import csv
import unittest

ROOT = Path(__file__).resolve().parents[1]


class Progress43HistoricalCdvdTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT / "src/ps2/cdvd_rpc_historical_recovered.c").read_text(
            encoding="utf-8"
        )

    def test_all_eight_historical_functions_are_present(self):
        for name in (
            "CDVD_Init_0019be70",
            "CDVD_DiskReady_0019bf00",
            "CDVD_FindFile_0019bf70",
            "CDVD_Stop_0019c0d0",
            "CDVD_TrayReq_0019c128",
            "CDVD_getdir_0019c190",
            "CDVD_FlushCache_0019c2ac",
            "CDVD_GetSize_0019c304",
        ):
            self.assertIn(name, self.source)

    def test_historical_rpc_constants_include_getsize(self):
        self.assertIn("#define CDVD_IRX        0x0B001337", self.source)
        self.assertIn("#define CDVD_GETSIZE    0x08", self.source)

    def test_client_server_field_matches_target_offset(self):
        self.assertIn("u8 _before_server[0x24];", self.source)
        self.assertIn("void *server;", self.source)

    def test_toc_entry_has_historical_144_byte_shape(self):
        self.assertIn("char filename[128 + 1];", self.source)
        self.assertIn("} __attribute__((packed));", self.source)

    def test_manifest_covers_target_corridor(self):
        with (ROOT / "analysis/matching/cdvd_rpc_listing.csv").open(
            newline="", encoding="utf-8"
        ) as f:
            rows = list(csv.DictReader(f))
        self.assertEqual(8, len(rows))
        self.assertEqual("0x0019be70", rows[0]["address"])
        self.assertEqual("0x0019c364", rows[-1]["end"])

    def test_runner_forces_fresh_candidate(self):
        text = (ROOT / "tools/run-cdvd-frontier.sh").read_text(encoding="utf-8")
        self.assertIn('rm -f "$OBJ"', text)
        self.assertIn("--require-all-matching", text)
        self.assertNotIn("-ffreestanding", text)
        self.assertNotIn("-fno-builtin", text)


if __name__ == "__main__":
    unittest.main()
