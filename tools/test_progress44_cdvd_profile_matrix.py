from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class Progress44CdvdProfileMatrixTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.runner = (ROOT / "tools/run-cdvd-frontier.sh").read_text(
            encoding="utf-8"
        )

    def test_tests_historical_snesticle_profile(self):
        self.assertIn("compile_profile snesticle-o2", self.runner)
        self.assertIn("-ffreestanding -fno-builtin", self.runner)

    def test_tests_pgen_o3_profile(self):
        self.assertIn("compile_profile pgen-o3-minimal", self.runner)
        self.assertIn("compile_profile pgen-o3-omit", self.runner)

    def test_tests_scheduler_fingerprint(self):
        self.assertIn("o2-builtins-nosched2", self.runner)
        self.assertIn("-fno-schedule-insns2", self.runner)

    def test_selects_best_profile_without_reusing_objects(self):
        self.assertIn('rm -f "$BUILD"/profile-*.o', self.runner)
        self.assertIn("profile-scores.tsv", self.runner)
        self.assertIn("best-profile.txt", self.runner)


if __name__ == "__main__":
    unittest.main()
