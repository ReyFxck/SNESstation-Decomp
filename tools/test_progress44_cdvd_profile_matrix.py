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
        self.assertIn("try_profile snesticle-freestanding", self.runner)
        self.assertIn("-ffreestanding -fno-builtin", self.runner)

    def test_tests_optimization_level_controls(self):
        self.assertIn("try_profile o1", self.runner)
        self.assertIn("try_profile o3", self.runner)

    def test_tests_scheduler_fingerprint(self):
        self.assertIn("try_profile o2-no-sched2", self.runner)
        self.assertIn("-fno-schedule-insns2", self.runner)
        self.assertIn("try_profile o2-no-sched12", self.runner)

    def test_selects_best_profile_without_reusing_objects(self):
        self.assertIn('rm -f "$BUILD"/shape-*.o', self.runner)
        self.assertIn('local obj="$BUILD/shape-${name}.o"', self.runner)
        self.assertIn("shape-scores.tsv", self.runner)
        self.assertIn("best-profile.txt", self.runner)

    def test_best_profile_is_scored_and_strict_gate_is_fail_closed(self):
        self.assertIn("rows.sort(reverse=True)", self.runner)
        self.assertIn('--object "$BUILD/shape-${BEST}.o"', self.runner)
        self.assertIn("--require-all-matching", self.runner)


if __name__ == "__main__":
    unittest.main()
