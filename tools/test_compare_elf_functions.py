#!/usr/bin/env python3
"""Self-test for relocation-aware ELF function comparison."""
from __future__ import annotations

import shutil
import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

from compare_elf_functions import (
    ELFFile,
    EM_MIPS,
    MIPS32_RELOCATION_MASKS,
    R_MIPS_26,
    R_MIPS_HI16,
    RelocationMask,
    Symbol,
    compare_function,
)


class ComparatorTest(unittest.TestCase):
    def test_non_mips_legacy_relocations_are_masked_but_code_differences_are_not(self) -> None:
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

    def test_elf32_mips_hi16_masks_immediate_but_not_register_bits(self) -> None:
        def align(value: int, alignment: int) -> int:
            return (value + alignment - 1) & ~(alignment - 1)

        with tempfile.TemporaryDirectory(prefix="snesstation-mips-elf-test-") as directory:
            path = Path(directory) / "candidate.o"
            text = (0x3C031234).to_bytes(4, "little")  # lui $v1, 0x1234
            rel = struct.pack("<II", 0, (1 << 8) | R_MIPS_HI16)
            strtab = b"\0candidate\0"
            shstrtab = b"\0.text\0.rel.text\0.symtab\0.strtab\0.shstrtab\0"

            off_text = 52
            off_rel = align(off_text + len(text), 4)
            off_symtab = align(off_rel + len(rel), 4)
            symtab = b"\0" * 16 + struct.pack("<IIIBBH", 1, 0, 4, 0x12, 0, 1)
            off_strtab = off_symtab + len(symtab)
            off_shstrtab = off_strtab + len(strtab)
            shoff = align(off_shstrtab + len(shstrtab), 4)

            ident = bytearray(16)
            ident[:4] = b"\x7fELF"
            ident[4] = 1
            ident[5] = 1
            ident[6] = 1
            header = struct.pack(
                "<16sHHIIIIIHHHHHH",
                bytes(ident),
                1,
                EM_MIPS,
                1,
                0,
                0,
                shoff,
                0,
                52,
                0,
                0,
                40,
                6,
                5,
            )

            def nameoff(name: bytes) -> int:
                return shstrtab.index(name)

            sections = [
                (0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                (nameoff(b".text"), 1, 0x6, 0, off_text, len(text), 0, 0, 4, 0),
                (nameoff(b".rel.text"), 9, 0, 0, off_rel, len(rel), 3, 1, 4, 8),
                (nameoff(b".symtab"), 2, 0, 0, off_symtab, len(symtab), 4, 1, 4, 16),
                (nameoff(b".strtab"), 3, 0, 0, off_strtab, len(strtab), 0, 0, 1, 0),
                (nameoff(b".shstrtab"), 3, 0, 0, off_shstrtab, len(shstrtab), 0, 0, 1, 0),
            ]

            blob = bytearray(shoff + 40 * len(sections))
            blob[:52] = header
            blob[off_text:off_text + len(text)] = text
            blob[off_rel:off_rel + len(rel)] = rel
            blob[off_symtab:off_symtab + len(symtab)] = symtab
            blob[off_strtab:off_strtab + len(strtab)] = strtab
            blob[off_shstrtab:off_shstrtab + len(shstrtab)] = shstrtab
            for index, section in enumerate(sections):
                struct.pack_into("<IIIIIIIIII", blob, shoff + index * 40, *section)
            path.write_bytes(blob)

            elf = ELFFile(path)
            symbol = elf.find_symbol("candidate")
            relocations = elf.relocation_masks(symbol, 4)
            self.assertEqual(elf.machine, EM_MIPS)
            self.assertEqual(len(relocations), 1)
            self.assertEqual(relocations[0].relocation_type, R_MIPS_HI16)
            self.assertEqual(relocations[0].mask_bytes, b"\xff\xff\x00\x00")

            immediate_only = (0x3C035678).to_bytes(4, "little")
            normalized = compare_function(immediate_only, 0, 4, elf, "candidate")
            self.assertTrue(normalized.matching)
            self.assertFalse(normalized.raw_equal)

            register_changed = (0x3C025678).to_bytes(4, "little")  # lui $v0, ...
            different = compare_function(register_changed, 0, 4, elf, "candidate")
            self.assertFalse(different.matching)
            self.assertTrue(different.first_differences)

    def test_mips_26_masks_jump_target_but_not_opcode(self) -> None:
        class FakeELF:
            def __init__(self, word: int):
                self.candidate = word.to_bytes(4, "little")

            def find_symbol(self, name: str) -> Symbol:
                return Symbol(name, 0, 4, 1, 2)

            def symbol_bytes(self, symbol: Symbol, fallback_size: int) -> bytes:
                return self.candidate

            def relocation_masks(self, symbol: Symbol, width: int):
                mask = MIPS32_RELOCATION_MASKS[R_MIPS_26]
                return (
                    RelocationMask(
                        0,
                        4,
                        R_MIPS_26,
                        mask.to_bytes(4, "little"),
                        True,
                    ),
                )

        elf = FakeELF(0x08001234)  # j target
        different_target = (0x08005678).to_bytes(4, "little")
        self.assertTrue(compare_function(different_target, 0, 4, elf, "candidate").matching)

        changed_opcode = (0x0C005678).to_bytes(4, "little")  # jal target
        self.assertFalse(compare_function(changed_opcode, 0, 4, elf, "candidate").matching)

    def test_unknown_mips_relocation_fails_closed(self) -> None:
        class FakeELF:
            candidate = (0x3C031234).to_bytes(4, "little")

            def find_symbol(self, name: str) -> Symbol:
                return Symbol(name, 0, 4, 1, 2)

            def symbol_bytes(self, symbol: Symbol, fallback_size: int) -> bytes:
                return self.candidate

            def relocation_masks(self, symbol: Symbol, width: int):
                return (RelocationMask(0, 4, 99, b"\0\0\0\0", False),)

        target = (0x3C035678).to_bytes(4, "little")
        result = compare_function(target, 0, 4, FakeELF(), "candidate")
        self.assertFalse(result.matching)
        self.assertEqual(result.unknown_relocation_types, (99,))


if __name__ == "__main__":
    unittest.main()
