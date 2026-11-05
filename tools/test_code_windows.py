#!/usr/bin/env python3
"""Public regression guards for the Stage-3P code-window gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import code_windows as gate


class CodeWindowsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "code-windows.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.CodeWindowsError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_all_code_windows_are_exact(self):
        result = self.document["result"]
        self.assertEqual((65536 - 0x114) + 10 * 65536, result["source_bytes"])
        self.assertEqual(0, result["residual_bytes"])
        self.assertEqual(71480, result["listing_bytes"])
        self.assertEqual(720620, result["direct_object_bytes"])
        self.assertEqual(221, result["direct_candidate_object_inputs"])
        self.assertEqual(3928, result["evidence_candidate_object_bytes"])
        self.assertEqual(18592, result["evidence_listing_bytes"])
        self.assertEqual(437, result["direct_object_sections"])
        self.assertEqual(0, result["incbin_payload_bytes"])
        self.assertEqual(0, result["incbin_payload_sections"])
        self.assertEqual((51, 0), (result["exact_chunks"], result["mismatching_chunks"]))
        self.assertEqual(list(range(51)), result["exact_chunk_indices"])
        self.assertEqual([], result["mismatching_chunk_indices"])
        self.assertEqual(gate.TARGET_SHA256, result["integrated_padded_sha256"])
        self.assertEqual(
            gate.EXPECTED["selected_source_contract_sha256"],
            result["selected_source_contract_sha256"],
        )

    def test_zlib_113_buffer_wrappers_link_from_pinned_upstream_source(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        expected = {
            "zlib-buffer-compress": (
                "build/code-windows/canonical/zlib-compress.o",
                0x00190700, 0x00E8, 7, 0x001B8900,
            ),
            "zlib-buffer-uncompress": (
                "build/code-windows/canonical/zlib-uncompress.o",
                0x001907E8, 0x00D4, 6, 0x001B8908,
            ),
        }
        for tag, (obj, address, size, relocs, rodata) in expected.items():
            row = rows[tag]
            self.assertEqual(row[1], obj)
            self.assertEqual(row[2:6], (address, 0, size, relocs))
            self.assertEqual(row[6], ((".rodata", rodata),))
            self.assertEqual(row[7], {".data": 0, ".rodata": 8, ".bss": 0})
            self.assertEqual(row[8], size)
            self.assertEqual(row[9], frozenset())
        self.assertEqual(gate.ZLIB_BUFFER_SOURCE_ROOT,
                         "third_party/historical_refs/zlib-1.1.3")
        self.assertIn("-Os", gate.ZLIB_BUFFER_SOURCE_FLAGS)

    def test_late_sif_check_stat_rpc_selector_links_directly(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        row = rows["sifrpc-checkstat"]
        self.assertEqual(row[1], "build/code-windows/canonical/sifrpc-checkstat.o")
        self.assertEqual(row[2:6], (0x001A1AF4, 0, 0x002C, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})

    def test_simple_libgcc_frontier_members_link_from_archive(self):
        rows = {row[0]: row for row in gate.DIRECT_LIBGCC_SLICES}
        self.assertEqual(
            rows["gccmuldi"],
            ("gccmuldi", "_muldi3.o", 0x001A1B20, 0, 120, 0,
             {".data": 0, ".bss": 0}, 120),
        )
        self.assertEqual(
            rows["gccmakedp"],
            ("gccmakedp", "_make_df.o", 0x001A3C90, 0, 48, 1,
             {".data": 0, ".bss": 0}, 48),
        )
        self.assertEqual(
            gate.LIBGCC_PRODUCER_SHA256,
            "fecc0c0a7e30f977c126274275965785745c2894c7e6fd59b7d07d076961e430",
        )

    def test_complete_historical_libpad_object_links_directly(self):
        self.assertEqual(
            gate.DIRECT_PAD_SLICES,
            (("padfull", 0x001A8420, 0, 0x0CD8, 131),),
        )
        self.assertEqual(
            gate.PAD_SOURCE,
            "matching/candidates/progress68/libpad-ps2lib-2004-newpadman.c.txt",
        )
        self.assertEqual(
            gate.PAD_SOURCE_SHA256,
            "4d96a31c5f8d3f814f3bec872fadf6afd9a4cb67c12705053a9073ae4f8e76d1",
        )
        for span in ((0x001A84D4, 20), (0x001A84F0, 12),
                     (0x001A8534, 20), (0x001A8550, 12)):
            self.assertNotIn(span, gate.LINKER_ZERO_FILL_SPANS)

    def test_c4drawwireframe_links_from_canonical_cpp_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "c4draw-wireframe"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/snes9x/c4drawwireframe.o")
        self.assertEqual(row[2:6], (0x0010CDCC, 0, 0x01D8, 15))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {})
        self.assertEqual(row[8], 0x01D8)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_intr_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-intr"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_heap_runtime_recovered.o")
        self.assertEqual(row[2:6], (0x0019F018, 0, 0x0060, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x00A0)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_syscalls_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-syscalls"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_syscalls_recovered.o")
        self.assertEqual(row[2:6], (0x0019FCD0, 0, 0x0050, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x0050)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_syscalls2_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-syscalls2"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_syscalls2_recovered.o")
        self.assertEqual(row[2:6], (0x0019CE60, 0, 0x00B0, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x00B0)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_sif_syscalls_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-sif-syscalls"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_sif_syscalls_recovered.o")
        self.assertEqual(row[2:6], (0x0019F5D0, 0, 0x0030, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x0030)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_heap_find_block_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-heap-find-block"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_heap_runtime_recovered.o")
        self.assertEqual(row[2:6], (0x0019E474, 0, 0x0040, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x0040)
        self.assertEqual(row[9], frozenset())

    def test_libkernel_client_init_links_from_canonical_c_object(self):
        matches = [
            row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES
            if row[0] == "libkernel-client-init"
        ]
        self.assertEqual(len(matches), 1)
        row = matches[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/libkernel_client_init_recovered.o")
        self.assertEqual(row[2:6], (0x0019F600, 0x0040, 0x0100, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 56, ".bss": 0})
        self.assertEqual(row[8], 0x0120)
        self.assertEqual(row[9], frozenset())

    def test_c4drawline_links_from_clean_cpp_function_section(self):
        slices = [
            row for row in gate.CLEAN_SOURCE_SLICES
            if row.name == "clean_c4_draw_line"
        ]
        self.assertEqual(len(slices), 1)
        row = slices[0]
        self.assertEqual(row.object, "build/code-windows/clean-source/native_c4_raster.o")
        self.assertEqual((row.address, row.size, row.symbol),
                         (0x0010CBB0, 540, "_Z10C4DrawLineiisiish"))
        units = [row for row in gate.CLEAN_SOURCE_UNITS if row[0] == "native_c4_raster"]
        self.assertEqual(
            units,
            [("native_c4_raster", "src/snes9x/native_c4_raster.cpp",
              "build/code-windows/clean-source/native_c4_raster.o", False)],
        )
        for name, address in {
            "_DAT_00335938": 0x00335938,
            "_DAT_0033593a": 0x0033593A,
            "DAT_0033593c": 0x0033593C,
            "DAT_0033593e": 0x0033593E,
            "DAT_00335940": 0x00335940,
            "DAT_00335942": 0x00335942,
            "DAT_00335944": 0x00335944,
            "C4TransfWireFrame2": 0x0010BBCC,
            "C4CalcWireFrame": 0x0010BECC,
        }.items():
            self.assertEqual(gate.CLEAN_SOURCE_PROVIDER_TARGETS[name], address)

    def test_get_tree_reconstruction_links_directly(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("get-tree", rows)
        row = rows["get-tree"]
        self.assertEqual(row[1], "build/source-tree/objects/unzip/get_tree.o")
        self.assertEqual(row[2:6], (0x0018C124, 0, 0x00D4, 9))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x00D4)
        self.assertEqual(row[9], frozenset())

    def test_numtestf_reconstruction_links_directly(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("numtestf", rows)
        row = rows["numtestf"]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/numtestf.o")
        self.assertEqual(row[2:6], (0x001A06C0, 0, 0x0080, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x0080)
        self.assertEqual(row[9], frozenset())

    def test_sbrk_links_from_canonical_source_with_program_break_provider(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("sbrk", rows)
        row = rows["sbrk"]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/sbrk.o")
        self.assertEqual(row[2:6], (0x0019F078, 0, 0x00C0, 13))
        self.assertEqual(row[6], ((".data", 0x00425A80),))
        self.assertEqual(row[7], {".data": 4, ".rel.data": 8, ".bss": 0})
        self.assertEqual(row[8], 0x00C0)
        self.assertEqual(row[9], frozenset(("_heap_ptr",)))

    def test_cdvd_rpc_links_from_canonical_os_source(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("cdvd-rpc", rows)
        row = rows["cdvd-rpc"]
        self.assertEqual(row[1], gate.CDVD_SOURCE_OBJECT)
        self.assertEqual(row[2:6], (0x0019BE70, 0, 0x04F4, 69))
        self.assertEqual(row[6], ((".data", 0x00425A38), (".bss", 0x0043ED00)))
        self.assertEqual(row[7], {".data": 4, ".bss": 0x4C28})
        self.assertEqual(row[8], 0x04F4)
        self.assertEqual(row[9], frozenset(("cdvd_inited",)))
        for span in ((0x0019BEB4, 24), (0x0019C050, 12), (0x0019C0B4, 12)):
            self.assertNotIn(span, gate.LINKER_ZERO_FILL_SPANS)
        self.assertIn("-Os", gate.CDVD_SOURCE_FLAGS)
        self.assertNotIn("-O2", gate.CDVD_SOURCE_FLAGS)

    def test_dmac_glue_links_from_canonical_os_source(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("dmac-glue", rows)
        row = rows["dmac-glue"]
        self.assertEqual(row[1], gate.GLUE_SOURCE_OBJECT)
        self.assertEqual(row[2:6], (0x0019FB00, 0, 0x00F0, 6))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x00F0)
        self.assertEqual(row[9], frozenset())
        self.assertIn("-Os", gate.GLUE_SOURCE_FLAGS)
        self.assertNotIn("-O2", gate.GLUE_SOURCE_FLAGS)

    def test_xprintf_core_links_from_canonical_vsnprintf_selector(self):
        self.assertEqual(
            gate.DIRECT_XPRINTF_CODE_OBJECT,
            ("build/code-windows/canonical/xprintf-vsnprintf.o",
             0x0019D84C, 0x0B18,
             ((".text", 0x0019D84C), (".rodata", 0x001BA478))),
        )

    def test_vprintf_links_from_canonical_historical_selector(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("xprintf-vprintf", rows)
        row = rows["xprintf-vprintf"]
        self.assertEqual(row[1], gate.XPRINTF_VPRINTF_OBJECT)
        self.assertEqual(row[2:6], (0x0019FAA8, 0, 0x0058, 4))
        self.assertEqual(row[6], ((".bss", 0x004454C0),))
        self.assertEqual(row[7], {".data": 0, ".bss": 0x1000})
        self.assertEqual(row[8], 0x0058)
        self.assertEqual(row[9], frozenset())
        self.assertIn("-Os", gate.XPRINTF_VPRINTF_FLAGS)
        self.assertIn("-DF_vprintf", gate.XPRINTF_VPRINTF_FLAGS)

    def test_xprintf_puts_links_from_proved_flat_target_override(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("xprintf-puts", rows)
        row = rows["xprintf-puts"]
        self.assertEqual(row[1], gate.XPRINTF_PUTS_OBJECT)
        self.assertEqual(row[2:6], (0x0019E414, 0, 0x0060, 1))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x0060)
        self.assertEqual(row[9], frozenset())
        self.assertIn("-Os", gate.XPRINTF_PUTS_FLAGS)
        self.assertNotIn("-ffunction-sections", gate.XPRINTF_PUTS_FLAGS)

    def test_sifcmd_irq_links_from_canonical_historical_selector(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("sifcmd-irq", rows)
        row = rows["sifcmd-irq"]
        self.assertEqual(row[1], gate.SIFCMD_IRQ_OBJECT)
        self.assertEqual(row[2:6], (0x0019FBF0, 0, 0x00E0, 6))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x00E0)
        self.assertEqual(row[9], frozenset())
        self.assertIn("-Os", gate.SIFCMD_IRQ_FLAGS)
        self.assertIn("-DF__sif_cmd_int_handler", gate.SIFCMD_IRQ_FLAGS)

    def test_remaining_sifcmd_helpers_link_from_canonical_selectors(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        expected = {
            "sifcmd-addhandler": ("build/code-windows/canonical/sifcmd-addhandler.o",
                                  0x0019F544, 0x0038, 2),
            "sifcmd-sreg-get": ("build/code-windows/canonical/sifcmd-sreg-get.o",
                                0x0019F57C, 0x0018, 2),
        }
        for tag, (obj, address, size, relocs) in expected.items():
            self.assertIn(tag, rows)
            row = rows[tag]
            self.assertEqual(row[1], obj)
            self.assertEqual(row[2:6], (address, 0, size, relocs))
            self.assertEqual(row[6], ())
            self.assertEqual(row[7], {".data": 0, ".bss": 0})
            self.assertEqual(row[8], size)
            self.assertEqual(row[9], frozenset())
        self.assertEqual(
            {(tag, define, address, size, relocs)
             for tag, _obj, define, address, size, relocs in gate.SIFCMD_SELECTOR_SPECS},
            {("sifcmd-addhandler", "F_sif_cmd_addhandler", 0x0019F544, 0x0038, 2),
             ("sifcmd-sreg-get", "F_sif_sreg_get", 0x0019F57C, 0x0018, 2)},
        )

    def test_remaining_sifrpc_helpers_link_from_canonical_selectors(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        expected = {
            "sifrpc-checkstat": ("build/code-windows/canonical/sifrpc-checkstat.o",
                                 0x001A1AF4, 0x002C, 0),
            "sifrpc-get-packet": ("build/code-windows/canonical/sifrpc-get-packet.o",
                                  0x0019CD70, 0x00BC, 3),
            "sifrpc-get-fpacket": ("build/code-windows/canonical/sifrpc-get-fpacket.o",
                                   0x0019CE2C, 0x0030, 0),
        }
        for tag, (obj, address, size, relocs) in expected.items():
            self.assertIn(tag, rows)
            row = rows[tag]
            self.assertEqual(row[1], obj)
            self.assertEqual(row[2:6], (address, 0, size, relocs))
            self.assertEqual(row[6], ())
            self.assertEqual(row[7], {".data": 0, ".bss": 0})
            self.assertEqual(row[8], size)
            self.assertEqual(row[9], frozenset())
        self.assertIn((0x0019CE5C, 4), gate.LINKER_ZERO_FILL_SPANS)
        self.assertEqual(
            {(tag, define, address, size, relocs)
             for tag, _obj, define, address, size, relocs in gate.SIFRPC_SELECTOR_SPECS},
            {("sifrpc-checkstat", "F_SifCheckStatRpc", 0x001A1AF4, 0x002C, 0),
             ("sifrpc-get-packet", "F__rpc_get_packet", 0x0019CD70, 0x00BC, 3),
             ("sifrpc-get-fpacket", "F__rpc_get_fpacket", 0x0019CE2C, 0x0030, 0)},
        )

    def test_gslib_hw_links_as_complete_canonical_corridor(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("gslib-hw", rows)
        row = rows["gslib-hw"]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/gslib_hw_recovered.o")
        self.assertEqual(row[2:6], (0x0019BD38, 0, 0x0138, 9))
        self.assertEqual(row[6], ((".data", 0x00425A30),))
        self.assertEqual(row[7], {".data": 4, ".bss": 0})
        self.assertEqual(row[8], 0x0138)
        self.assertEqual(row[9], frozenset(("VRcount_recovered",)))
        for span in ((0x0019BD5C, 8), (0x0019BD70, 8),
                     (0x0019BE18, 8), (0x0019BE5C, 8)):
            self.assertNotIn(span, gate.LINKER_ZERO_FILL_SPANS)

    def test_remaining_fileio_wrappers_link_from_canonical_selectors(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        expected = {
            "fileio-mkdir": ("build/code-windows/canonical/fileio-fio-mkdir.o",
                             0x0019D410, 0x00A0, 12),
            "fileio-putc": ("build/code-windows/canonical/fileio-fio-putc.o",
                            0x0019D534, 0x0024, 1),
            "fileio-gets": ("build/code-windows/canonical/fileio-fio-gets.o",
                            0x0019D558, 0x00A8, 3),
        }
        for tag, (path, address, size, relocs) in expected.items():
            self.assertIn(tag, rows)
            row = rows[tag]
            self.assertEqual(row[1], path)
            self.assertEqual(row[2:6], (address, 0, size, relocs))
            self.assertEqual(row[6], ())
            self.assertEqual(row[7], {".data": 0, ".bss": 0})
            self.assertEqual(row[8], size)
            self.assertEqual(row[9], frozenset())
        self.assertEqual(
            [(tag, path, define, address, size, relocs)
             for tag, path, define, address, size, relocs
             in gate.FILEIO_SELECTOR_SPECS],
            [
                ("fileio-mkdir", "build/code-windows/canonical/fileio-fio-mkdir.o",
                 "F_fio_mkdir", 0x0019D410, 0x00A0, 12),
                ("fileio-putc", "build/code-windows/canonical/fileio-fio-putc.o",
                 "F_fio_putc", 0x0019D534, 0x0024, 1),
                ("fileio-gets", "build/code-windows/canonical/fileio-fio-gets.o",
                 "F_fio_gets", 0x0019D558, 0x00A8, 3),
            ],
        )

    def test_loadfile_front_wrappers_link_from_canonical_selectors(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        expected = {
            "loadfile-load-module": (
                "build/code-windows/canonical/loadfile-load-module.o",
                0x0019D600, 0x0020, 1),
            "loadfile-load-buffer": (
                "build/code-windows/canonical/loadfile-load-buffer.o",
                0x0019D620, 0x001C, 1),
        }
        for tag, (path, address, size, relocs) in expected.items():
            self.assertIn(tag, rows)
            row = rows[tag]
            self.assertEqual(row[1], path)
            self.assertEqual(row[2:6], (address, 0, size, relocs))
            self.assertEqual(row[6], ())
            self.assertEqual(row[7], {".data": 0, ".bss": 0})
            self.assertEqual(row[8], size)
            self.assertEqual(row[9], frozenset())

    def test_zero_relocation_c4_reconstructions_link_directly(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        expected = {
            "c4-conv-oam": (
                "build/source-tree/objects/ps2/c4convoam.o", 0x0010C340, 0x03B8,
            ),
            "c4-scale-rotate": (
                "build/source-tree/objects/ps2/c4doscalerotate.o", 0x0010C6F8, 0x04B8,
            ),
            "c4-transform-lines": (
                "build/source-tree/objects/ps2/c4transformlines.o", 0x0010CFA4, 0x0304,
            ),
            "s9x-set-c4": (
                "build/source-tree/objects/ps2/s9xsetc4.o", 0x0010D7DC, 0x0B60,
            ),
        }
        for tag, (obj, address, size) in expected.items():
            self.assertIn(tag, rows)
            row = rows[tag]
            self.assertEqual(row[1], obj)
            self.assertEqual(row[2:6], (address, 0, size, 0))
            self.assertEqual(row[6], ())
            self.assertEqual(row[7], {".data": 0, ".bss": 0})
            self.assertEqual(row[8], size)
            self.assertEqual(row[9], frozenset())

    def test_canonical_ps2lib_ctype_corridor_links_directly(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        self.assertIn("ctype", rows)
        row = rows["ctype"]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/ctype.o")
        self.assertEqual(row[2:6], (0x0019EDAC, 0, 0x0268, 13))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x0268)
        self.assertEqual(row[9], frozenset())

    def test_canonical_ps2lib_string_c_corridor_links_directly(self):
        rows = {row[0]: row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES}
        expected = {
            "strcasecmp": (
                "build/source-tree/objects/ps2/strcasecmp.o",
                0x0019E860, 0x0084, 4, (), {".data": 0, ".bss": 0},
            ),
            "strncasecmp": (
                "build/source-tree/objects/ps2/strncasecmp.o",
                0x0019E8E4, 0x00B8, 4, (), {".data": 0, ".bss": 0},
            ),
            "strtok": (
                "build/source-tree/objects/ps2/strtok.o",
                0x0019E99C, 0x0108, 27, ((".bss", 0x00445140),),
                {".data": 0, ".bss": 8},
            ),
            "strrchr": (
                "build/source-tree/objects/ps2/string.o",
                0x0019EAA4, 0x0054, 2, (), {".data": 0, ".bss": 0},
            ),
        }
        for tag, (obj, address, size, relocs, providers, layout) in expected.items():
            self.assertIn(tag, rows)
            row = rows[tag]
            self.assertEqual(row[1], obj)
            self.assertEqual(row[2:6], (address, 0, size, relocs))
            self.assertEqual(row[6], providers)
            self.assertEqual(row[7], layout)
            self.assertEqual(row[8], size)
            self.assertEqual(row[9], frozenset())

    def test_strcat_runtime_member_links_with_proved_strcpy_call(self):
        rows = {row[0]: row for row in gate.DIRECT_RUNTIME_OBJECTS}
        self.assertIn("rtstrcat", rows)
        row = rows["rtstrcat"]
        self.assertEqual(row[1:5], ("libc/strcat.o", 0x0019C3D4, 56, 1))
        self.assertEqual(row[5], ())
        self.assertEqual(row[6], {".data": 0, ".bss": 0})
        self.assertEqual(gate.CLEAN_SOURCE_PROVIDER_TARGETS["strcpy"], 0x0019C528)

    def test_strncmp_links_from_canonical_source_tree_object(self):
        rows = [row for row in gate.DIRECT_RECOVERED_SOURCE_SLICES if row[0] == "strncmp"]
        self.assertEqual(len(rows), 1)
        row = rows[0]
        self.assertEqual(row[1], "build/source-tree/objects/ps2/strncmp.o")
        self.assertEqual(row[2:6], (0x0019C410, 0, 0x0048, 0))
        self.assertEqual(row[6], ())
        self.assertEqual(row[7], {".data": 0, ".bss": 0})
        self.assertEqual(row[8], 0x0048)
        self.assertEqual(row[9], frozenset())

    def test_zero_relocation_ps2lib_string_runtime_members_link_directly(self):
        rows = {row[0]: row for row in gate.DIRECT_RUNTIME_OBJECTS}
        expected = {
            "rtmemcpy": ("libc/memcpy.o", 0x0019C364, 56),
            "rtmemset": ("libc/memset.o", 0x0019C39C, 56),
            "rtmemcmp": ("libc/memcmp.o", 0x0019C458, 72),
            "rtmemmove": ("libc/memmove.o", 0x0019C4A0, 136),
            "rtstrcpy": ("libc/strcpy.o", 0x0019C528, 40),
            "rtstrncpy": ("libc/strncpy.o", 0x0019C550, 88),
            "rtstrlen": ("libc/strlen.o", 0x0019C5E8, 40),
            "rtstrchr": ("libc/strchr.o", 0x0019C610, 56),
            "rtstrcmp": ("libc/strcmp.o", 0x0019C648, 64),
        }
        for tag, (basename, address, size) in expected.items():
            self.assertIn(tag, rows)
            row = rows[tag]
            self.assertEqual(row[1:5], (basename, address, size, 0))
            self.assertEqual(row[5], ())
            self.assertEqual(row[6], {".data": 0, ".bss": 0})

    def test_final_linker_isolates_only_source_aggregate_models(self):
        source_input = Path("build/test/source-tree.partial.o")
        rule = gate.source_model_rule(source_input)
        self.assertIn(f'"{source_input.resolve().as_posix()}"(', rule)
        self.assertIn(".text.* .gnu.linkonce.t.* .rodata.native_dsp1_rom", rule)
        self.assertIn(
            ".data.*_ZZ9S9xSetPPUE8IncCount .data.*_ZZ9S9xSetPPUE5Shift",
            rule,
        )
        self.assertNotIn("(.data.*", rule)

    def test_private_payload_is_not_frozen(self):
        self.assertFalse(self.document["claims"]["private_target_bytes_stored"])
        self.assertTrue(self.document["claims"]["windows_0_through_6_exact"])
        self.assertTrue(self.document["claims"]["windows_1_through_6_exact"])
        self.assertTrue(self.document["claims"]["windows_0_through_10_exact"])
        self.assertTrue(self.document["claims"]["unpacked_hash_matched"])
        self.assertFalse(self.document["claims"]["replacement_elf"])
        self.assertNotIn(".incbin", json.dumps(self.document))

    def test_claim_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["claims"]["replacement_elf"] = True
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["result"]["exact_chunks"] = 45
        self.reject(changed)

    def test_evidence_rebuild_preserves_ledger_bytes(self):
        with tempfile.TemporaryDirectory() as name:
            ledgers = [Path(name) / "a.tsv", Path(name) / "b.tsv"]
            original = [b"user-ledger-a\n", b"user-ledger-b\n"]
            for path, data in zip(ledgers, original):
                path.write_bytes(data)

            def rebuild(*_args, **_kwargs):
                for path in ledgers:
                    path.write_bytes(b"generated-local-path-hash\n")
                return SimpleNamespace(returncode=0)

            args = gate.parse_args(["prepare"])
            with patch.object(gate, "EVIDENCE_MANIFESTS", tuple(map(str, ledgers))), \
                 patch.object(gate, "resolve_tool", return_value=Path("/tool/ee-g++")), \
                 patch.object(gate.subprocess, "run", side_effect=rebuild):
                gate.prepare_evidence(args)
            self.assertEqual(original, [path.read_bytes() for path in ledgers])


if __name__ == "__main__":
    unittest.main()
