from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class Progress45CdvdExactShapeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.candidate = (ROOT / "matching/candidates/cdvd_rpc.c").read_text(
            encoding="utf-8"
        )
        cls.compat = (
            ROOT / "matching/ee_abi_compat/cdvd_legacy_compat.h"
        ).read_text(encoding="utf-8")
        cls.runner = (ROOT / "tools/run-cdvd-frontier.sh").read_text(
            encoding="utf-8"
        )

    def test_exact_historical_global_names_and_order(self):
        self.assertLess(
            self.candidate.index("static unsigned sbuff[0x1300]"),
            self.candidate.index("static SifRpcClientData_t cd0"),
        )
        self.assertLess(
            self.candidate.index("static SifRpcClientData_t cd0"),
            self.candidate.index("int cdvd_inited = 0"),
        )

    def test_exact_client_header_shape(self):
        self.assertIn("struct t_SifRpcHeader hdr;", self.compat)
        self.assertIn("void *end_param;", self.compat)
        self.assertIn("struct t_SifRpcServerData *server;", self.compat)

    def test_getsize_revision_is_present(self):
        self.assertIn("#define CDVD_GETSIZE    0x08", self.compat)
        self.assertIn("unsigned int CDVD_GetSize()", self.candidate)

    def test_matrix_scores_actual_differing_bytes(self):
        self.assertIn("tools/score_cdvd_candidate.py", self.runner)
        self.assertIn("o2-no-strict", self.runner)
        self.assertIn("o2-no-sched1", self.runner)
        self.assertIn("o2-no-delayed", self.runner)


if __name__ == "__main__":
    unittest.main()
