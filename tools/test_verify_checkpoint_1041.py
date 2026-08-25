#!/usr/bin/env python3
from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

import verify_checkpoint_1041 as checkpoint


class VerifyCheckpoint1041Tests(unittest.TestCase):
    def test_repository_matches_frozen_checkpoint(self) -> None:
        report = checkpoint.verify_checkpoint()
        self.assertEqual(report.targets, 1_041)
        self.assertEqual(report.evidence_files, 5)
        self.assertEqual(report.tag, "function-frontier-1041-v81")

    def test_checksum_manifest_rejects_parent_traversal(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            manifest = Path(directory) / "bad.sha256"
            manifest.write_text(f"{'0' * 64}  ../private.ELF\n", encoding="utf-8")
            with self.assertRaises(checkpoint.CheckpointError):
                checkpoint.read_checksum_manifest(manifest)


if __name__ == "__main__":
    unittest.main()
