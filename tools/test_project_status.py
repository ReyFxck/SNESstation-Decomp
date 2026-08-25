#!/usr/bin/env python3
from __future__ import annotations

import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from project_status import load_status  # noqa: E402


class ProjectStatusTests(unittest.TestCase):
    def test_checkpoint_accounting_is_disjoint_and_complete(self) -> None:
        status = load_status(ROOT)
        self.assertEqual(status.total, 1041)
        self.assertEqual(status.formal_matching, 1041)
        self.assertEqual(status.reconstructed_unproven, 0)
        self.assertEqual(status.recovered_pending, 0)
        self.assertEqual(status.working_checkpoint, 1041)
        self.assertEqual(status.working_remaining, 0)
        self.assertEqual(
            status.formal_matching + status.recovered_pending + status.working_remaining,
            status.total,
        )


if __name__ == "__main__":
    unittest.main()
