#!/usr/bin/env python3
"""Self-test for relocation-normalized ELF function comparison."""
from __future__ import annotations

import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

from compare_elf_functions import ELFFile, compare_function


class ComparatorTest(unittest.TestCase):
    def test_relocations_are_masked_but_code_differences_are_not(self) -> None:
        compiler = shutil.which("cc")
        if compiler is None:
            self.skipTest("host C compiler is unavailable")

        with tempfile.TemporaryDirectory(prefix="snesstation-elf-test-") as directory:
            root = Path(directory)
            source = root / "candidate.c"
            object_path = root / "candidate.o"
            source.write_text(
                "extern int decomp_external;\n"
                "__attribute__((noinline)) int candidate_math(int x)\n"
                "{\n"
                "    return x + decomp_external;\n"
                "}\n",
                encoding="utf-8",
            )
            subprocess.run(
                [compiler, "-O2", "-fno-pic", "-c", str(source), "-o", str(object_path)],
                check=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
            )

            elf = ELFFile(object_path)
            symbol = elf.find_symbol("candidate_math")
            candidate = elf.symbol_bytes(symbol, 0)
            relocations = elf.relocation_ranges(symbol, 4)
            self.assertTrue(relocations, "test object unexpectedly has no text relocation")

            target = bytearray(candidate)
            for start, end in relocations:
                for index in range(start, end):
                    target[index] ^= 0x5A
            normalized = compare_function(
                bytes(target), 0, len(candidate), elf, "candidate_math", 4
            )
            self.assertFalse(normalized.raw_equal)
            self.assertTrue(normalized.normalized_equal)
            self.assertTrue(normalized.matching)

            masked = {
                index
                for start, end in relocations
                for index in range(start, end)
            }
            non_relocation = next(index for index in range(len(target)) if index not in masked)
            target[non_relocation] ^= 0x01
            different = compare_function(
                bytes(target), 0, len(candidate), elf, "candidate_math", 4
            )
            self.assertFalse(different.normalized_equal)
            self.assertIn(non_relocation, different.first_differences)


if __name__ == "__main__":
    unittest.main()
