from pathlib import Path
import csv
import unittest

ROOT = Path(__file__).resolve().parents[1]


class Progress43HistoricalCdvdTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT / "src/ps2/cdvd_rpc.c").read_text(
            encoding="utf-8"
        )

    def test_all_eight_historical_functions_are_present(self):
        for name in (
            "CDVD_Init()",
            "CDVD_DiskReady(int mode)",
            "CDVD_FindFile(const char* fname, struct TocEntry* tocEntry)",
            "CDVD_Stop()",
            "CDVD_TrayReq(int mode)",
            "CDVD_getdir(",
            "CDVD_FlushCache()",
            "CDVD_GetSize()",
        ):
            self.assertIn(name, self.source)

    def test_historical_rpc_constants_include_getsize(self):
        header = (ROOT / "include/cdvd_legacy_compat.h").read_text(encoding="utf-8")
        self.assertIn("#define CDVD_IRX        0xB001337", header)
        self.assertIn("#define CDVD_GETSIZE    0x08", header)

    def test_client_server_field_matches_target_offset(self):
        header = (ROOT / "include/cdvd_legacy_compat.h").read_text(encoding="utf-8")
        self.assertIn("struct t_SifRpcHeader hdr;", header)
        self.assertIn("struct t_SifRpcServerData *server;", header)

    def test_toc_entry_has_historical_144_byte_shape(self):
        header = (ROOT / "include/cdvd_legacy_compat.h").read_text(encoding="utf-8")
        self.assertIn("char filename[128+1];", header)
        self.assertIn("} __attribute__((packed));", header)

    def test_manifest_covers_target_corridor(self):
        with (ROOT / "analysis/matching/cdvd_rpc_listing.csv").open(
            newline="", encoding="utf-8"
        ) as f:
            rows = list(csv.DictReader(f))
        self.assertEqual(8, len(rows))
        self.assertEqual("0x0019be70", rows[0]["address"])
        self.assertEqual("0x0019c364", rows[-1]["end"])

    def test_runner_forces_fresh_profile_candidates_and_keeps_strict_gate(self):
        text = (ROOT / "tools/run-cdvd-frontier.sh").read_text(encoding="utf-8")
        self.assertIn('rm -f "$BUILD"/shape-*.o', text)
        self.assertIn('local obj="$BUILD/shape-${name}.o"', text)
        self.assertIn("--require-all-matching", text)
        self.assertIn("try_profile snesticle-freestanding", text)
        self.assertIn("-ffreestanding -fno-builtin", text)


if __name__ == "__main__":
    unittest.main()
