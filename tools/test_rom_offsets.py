import copy
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
import rom_offsets as r
import runtime_members
import data_backing as backing
from compare_elf_functions import Symbol
import test_data_backing as helpers


class ROMOffsetTests(unittest.TestCase):
    def test_29_closed_not_storage_claims(self):
        rows = r.validate()
        self.assertEqual(29, len(rows))
        self.assertEqual(4, len({row["function_address"] for row in rows}))
        self.assertTrue(all(row["status"] == r.STATUS and row["claim"] == r.CLAIM for row in rows))

    def test_public_validation_needs_no_private_image(self):
        with patch.object(r.libgcc, "load_reference", side_effect=AssertionError("private")):
            r.validate()

    def test_drifted_source_evidence_or_claim_rejected(self):
        for field in ("source_function_sha256", "function_sha256", "kind", "claim", "helper_sha256", "offset_hex"):
            rows = r.validate()
            rows[0][field] = "wrong"
            with tempfile.TemporaryDirectory() as tmp:
                path = Path(tmp)/"ledger.tsv"
                path.write_text(runtime_members.render(r.FIELDS, rows))
                with self.assertRaises(r.ROMOffsetError):
                    r.validate(path)

    def test_live_import_of_closed_rom_name_is_rejected(self):
        live = r.libgcc.read_table(r.libgcc.DEFAULT_EXTERNAL, r.libgcc.EXTERNAL_FIELDS)
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp)/"externals.tsv"
            path.write_text(runtime_members.render(r.libgcc.EXTERNAL_FIELDS, live+r.historical_externals()[:1]))
            with self.assertRaisesRegex(r.ROMOffsetError, "live externals"):
                r.validate(external_map=path)

    def test_numeric_overlap_with_snapshot_buffer_is_not_backing(self):
        rows, sections = backing.validate(backing.parse_args(["validate"]))
        closed = [row for row in rows if row["status"] == r.STATUS]
        self.assertEqual(29, len(closed))
        self.assertTrue(any(any(a <= int(row["target_address"],0) < b for a,b in map(backing.interval,sections))
                            for row in closed))
        self.assertTrue(all(not row["section"] and not row["access_extent_hex"] for row in closed))

    def test_removed_name_cannot_survive_as_absolute_symbol(self):
        name = next(iter(r.SPEC))
        rows = [{"symbol":name, "status":r.STATUS}]
        backing.check_input(helpers.DataBackingTests.fake_elf([]),rows)
        with self.assertRaises(backing.DataBackingError):
            backing.check_input(helpers.DataBackingTests.fake_elf([Symbol(name,0x100000,0,0xfff1,16)]),rows)

    def test_original_byte_witness_mismatch_rejected(self):
        with patch.object(r.libgcc, "load_reference", return_value=bytes(r.libgcc.TARGET_SIZE)):
            with self.assertRaisesRegex(r.ROMOffsetError,"fingerprint"):
                r.verify_reference(Path("unused"))

    @unittest.skipUnless(shutil.which("cc"), "host C compiler required")
    def test_byte_stride_and_32bit_wrap_in_compiled_helper(self):
        code = '#include "rom_offsets_recovered.h"\nint main(void) {\n'
        for base, offset, expected in ((0x100000,0x1385ec,0x2385ec), (0,1,1), (0xfffffff0,0x20,0x10)):
            code += f'if ((uintptr_t)P28_ROM_AT({hex(base)}u,{hex(offset)}u) != {hex(expected)}u) return 1;\n'
        code += 'return 0; }\n'
        with tempfile.TemporaryDirectory() as tmp:
            source, exe = Path(tmp)/"test.c", Path(tmp)/"test"
            source.write_text(code)
            subprocess.run(["cc","-std=c99","-Wall","-Werror","-I",str(r.ROOT/"include"),str(source),"-o",str(exe)],check=True)
            subprocess.run([str(exe)],check=True)
