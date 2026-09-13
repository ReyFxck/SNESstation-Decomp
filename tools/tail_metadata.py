#!/usr/bin/env python3
"""Rebuild and integrate the remaining historical metadata in image window 50.

Stage 3K closes the last image window from public Snes9x, zlib, PS2LIB,
libgcc and libsupc++ objects plus explicit DWARF/LSDA/RTTI semantics.  The
private reference supplies only relocation-controlled words and comparison
results; generated payloads remain below ignored build storage.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import shlex
import shutil
import struct
import subprocess
import sys
from pathlib import Path
from typing import Sequence

import data_backing
import historical_data
import historical_tail_data as stage3i
import link_layout_probe
import runtime_tail_data as stage3j
import startup_integration as startup
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/tail_metadata.json"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_STAGE3I = ROOT / "build/historical-tail-data"
DEFAULT_STAGE3J = ROOT / "build/runtime-tail-data"
DEFAULT_RUNTIME = ROOT / "build/runtime-members"
DEFAULT_BUILD = ROOT / "build/tail-metadata"

FORMAT = "snesstation-stage3k-tail-metadata"
SCHEMA = 1
TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
PS2SDK_SBRK_REVISION = "694100b78ad5bc8f8248a1138143860af4f8435f"
ABSORBED_FIXED = (
    ".data.stage3ce.va_00424870",
    ".data.stage3ce.va_00426c28",
    ".data.stage3ce.va_00426d80",
)

SOURCE_HASHES = {
    "srtc.cpp": "b6bbeaed929fffc965fb0a3833037fb742ef79110690f61f3e5e6a60a02b6bf9",
    "explode.c": "f7d5a3bde3b826b3b3f961d9b87a89f48914638876edf183f17c66f5c81d1514",
    "unreduce.c": "8bbc26cf7104954847e9dfbbf3648914e819c2be9bf0e94ee70ae03413451c91",
    "deflate.c": "bc22df6685788955a9aeb049022424088d7be916a91e019d3ca5ff5167baf159",
    "gzio.c": "9877368b9ad8ff4a7a57a082c57f5d9a35c6a16f878e9314224a854e0334d609",
    "inftrees.c": "ceb90191ca5b2bcccdfe3cfc3af92d3c6ebf6d4a2130c7c76989c32b6870c98b",
    "trees.c": "a671c6273a79429beef27a5d096506ecf899964ffa4e9f11fb7eb51d4943feb0",
    "zutil.c": "2f7f7d3273ea427a0ddac36b99f5536fbf110f72cee5ce557dcd600aeee775d5",
    "eh_exception.cc": "4f8df081013ba5a8c752d674d418409bac0990a46aac1e41773f200fbfabb046",
    "sbrk.c": "a2a01c696ac5922ddb516db51de00c1d1c543b0b4b72fdb65bdeed27ac4d6df0",
    "s_mathcnst.c": "cafbc24d4a9fa3d157c9d6e8ff637b1f83e2b49f62fd74435a159fb7de92e90d",
}


def P(name: str, obj: str, source_section: str, source_offset: int,
      address: int, size: int, relocations: int, raw: str, linked: str) -> dict:
    return {
        "name": name, "object": obj, "source_section": source_section,
        "source_offset": source_offset, "section": f".data.stage3k.{name}",
        "address": address, "size": size, "relocations": relocations,
        "raw_sha256": raw, "linked_sha256": linked,
    }


SOURCE_SECTIONS = (
    P("srtc_month_keys", "srtc", ".data", 0x28, 0x00423878, 0x30, 0,
      "af8044178801eadea12bd5373ea905896789f4226e0fb56aa24a46e1937fc0cc",
      "af8044178801eadea12bd5373ea905896789f4226e0fb56aa24a46e1937fc0cc"),
    P("explode_tables", "explode", ".data", 0, 0x00424158, 0x280, 0,
      "b83704fb85d6adeece79a7ed93afb5d845e332a66df50ba05154defb70e8b4c7",
      "b83704fb85d6adeece79a7ed93afb5d845e332a66df50ba05154defb70e8b4c7"),
    P("unreduce_tables", "unreduce", ".data", 0, 0x00424400, 0x450, 1,
      "9aea55fdac96835bb52e530bcf9e976684bd8487c48764ac343a73102d143f25",
      "2c65fa346504008a20372a400a80c7314bb9f789d0c0781ca9e42074718f4fae"),
    P("deflate_version", "deflate", ".data", 0, 0x00424858, 4, 1,
      "10a4ac7a3bbd1bc41e0d48c9683c7ccdc7bbf2e313f001d85b21aeafcbb48e8c",
      "2b000687f2c9dac6c1b4b94229ebae1cf9c0e88b605e63e40bea9f7b58ef284f"),
    P("gz_magic", "gzio", ".data", 0, 0x00424860, 8, 0,
      "726e93918cc0c08e90b8acf61fa02f457571446c5429c3fd58c231d769c0a559",
      "726e93918cc0c08e90b8acf61fa02f457571446c5429c3fd58c231d769c0a559"),
    P("inflate_tables", "inftrees", ".data", 0, 0x00424868, 0x1108, 0,
      "4322bf145f690419b980f836b95dfcf1130299bf6b933c3403598ca5606407ac",
      "4322bf145f690419b980f836b95dfcf1130299bf6b933c3403598ca5606407ac"),
    P("deflate_trees", "trees", ".data", 0, 0x004259B8, 0x48, 5,
      "c9bb79123847a81eef704d529d246c4dbb2aa62066c7e58fde92c7f264b305f8",
      "4b000868099d5bd87b7e85a3c287e669df018717dc7cc45fefd9375c9e88a14d"),
    P("zlib_errors", "zutil", ".data", 0, 0x00425A00, 0x28, 10,
      "582bb8a322e35bec4519d7eece9afb8513f9c6ac44421a2f2b3ed91298416bc4",
      "d514c73bf9d91c93351d200aba4c67eed3bff6e1e9ca433368cea8b4485249d0"),
    P("sif_rpc_data", "SifRpcMain", ".data", 0, 0x00425A40, 0x30, 3,
      "bb194ae4621e4cf9d4e8e5a292d3007543498cb85262c7562a5fb0a20d4171e5",
      "91b02659364b10a6103bd4ae95b79918f311c5cb25ec1a1981fcf393fda069f6"),
    P("sif_cmd_data", "sif_cmd_main", ".data", 0, 0x00425A88, 0x28, 5,
      "f85668b539fa043d802acd2d397ac0c0f6cced394f3ffb695289873c98fd4020",
      "aa113cfa68fcff3542f829eff5ae3ffae005356f3cdbdfb439c699e5feb67f5d"),
    P("divdi3_data", "_divdi3", ".data", 0, 0x00425AD4, 0x5C, 2,
      "622ce0587567513c3bf401fc7e0c1fd56d1c8ed3687b8936069dde36202bd93c",
      "d5a2b31e7e60b336b61589a30967cedeae1706c921f30124a50b763ccf9fc7f1"),
    P("udivdi3_data", "_udivdi3", ".data", 0, 0x00425B30, 0x54, 2,
      "3c7f92c930863d83325a6277d115ec02f0971a910f7ff2f9225703a1eabd6236",
      "0b31ae0f7238653171f16867ad8fb815f06226ba55fc0d5560f43d9e1cb536b6"),
    P("umoddi3_data", "_umoddi3", ".data", 0, 0x00425B84, 0x54, 2,
      "0788e569b8b0269ec55c8a5f09dde3262c26ac17d6b2aa93750acba9135e1224",
      "b010a52c7d81662a73e7190c9a7e24fe37134f10f9900a2c1aa26f340945e2ea"),
    P("unwind_dw2_data", "unwind-dw2", ".data", 0, 0x00425BD8, 0x4CC, 17,
      "51973efc0956211edcfea94d9b707bab9a36365f52e4fedd29fb38b2553f891c",
      "7153cac67b16d2880fe16fb436ea324e42dd64bcc622314c65c4435c36481e83"),
    P("unwind_fde_data", "unwind-dw2-fde", ".data", 0, 0x004260A4, 0x424, 19,
      "379ff90d5c986604804725dc8498073a3fc2c560e16754ecfaabb090ddc1fb77",
      "7484fa6292fbe6fcb73d164a402c6423934fb32866dd61da28edae0935adf9ee"),
    P("moddi3_data", "_moddi3", ".data", 0, 0x004264C8, 0x5C, 2,
      "47c52c5340ea02621c7a6beb73ea82e62a61977b74fb3eaae2cb3805c8cf4e6d",
      "f0731c58eba470bcf9859fea583619c6c8fa6c03571781821bcb9630281faa79"),
)

RTTI_SECTIONS = (
    ("vmi_vtable", "tinfo", ".gnu.linkonce.d._ZTVN10__cxxabiv121__vmi_class_type_infoE", 0x00426C40, 0x30, 10, "17b0761f87b081d5cf10757ccc89f12be355c70e2e29df288b65b30710dcbcd1", "54abab196b6f03ceae597884ba39ad49b78a5d50456f848dcf32f188390d3d3c"),
    ("si_vtable", "tinfo", ".gnu.linkonce.d._ZTVN10__cxxabiv120__si_class_type_infoE", 0x00426C70, 0x30, 10, "17b0761f87b081d5cf10757ccc89f12be355c70e2e29df288b65b30710dcbcd1", "384c406794c45541ff8107f850a66dad7d0af7ce87698e38ff75447e8747d3bc"),
    ("class_vtable", "tinfo", ".gnu.linkonce.d._ZTVN10__cxxabiv117__class_type_infoE", 0x00426CA0, 0x30, 10, "17b0761f87b081d5cf10757ccc89f12be355c70e2e29df288b65b30710dcbcd1", "f21ea0a6d06fcaf4a83fd158a2f32719f4b2043a5a16ae976f020478b2b5e1d6"),
    ("bad_typeid_vtable", "tinfo", ".gnu.linkonce.d._ZTVSt10bad_typeid", 0x00426CD0, 0x18, 4, "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0", "12c8f8fa30a4616b8b63a49ae01268343cac9665fb8b19e40a645a3f3cef0847"),
    ("bad_cast_vtable", "tinfo", ".gnu.linkonce.d._ZTVSt8bad_cast", 0x00426CE8, 0x18, 4, "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0", "a980595580b584784be8d130238431e5e3050cfd3862531d88206c585514b2b9"),
    ("type_info_vtable", "tinfo", ".gnu.linkonce.d._ZTVSt9type_info", 0x00426D00, 0x20, 7, "66687aadf862bd776c8fc18b8e9f8e20089714856ee233b3902a591d0d5f2925", "900df783b75832ede9a43357f5cff9c8f316795b4a5561baef26e264d12a31a1"),
    ("type_info_rtti", "tinfo", ".gnu.linkonce.d._ZTISt9type_info", 0x00426D20, 8, 2, "6cc16abd70eefb90dc0ba0d14fb088630873b2c6ad943f7442356735984c35a3", "730877b052830216f7ae0d226abc39217c4594e5c561a834356dc1436673671b"),
    ("bad_cast_rtti", "tinfo", ".gnu.linkonce.d._ZTISt8bad_cast", 0x00426D28, 0x10, 3, "85b724e3f6e2dd2648a696595eb637b234dcecc5cb770a5afbf0926341ebdfb4", "851d85fe3824d67c5f43d212259aba18cabe3105631a4201fd5a7e530da01a7f"),
    ("bad_typeid_rtti", "tinfo", ".gnu.linkonce.d._ZTISt10bad_typeid", 0x00426D38, 0x10, 3, "85b724e3f6e2dd2648a696595eb637b234dcecc5cb770a5afbf0926341ebdfb4", "0042d8903020bd3539da837131675f3051faed1c3c906118240abdae3179b1c7"),
    ("class_rtti", "tinfo", ".gnu.linkonce.d._ZTIN10__cxxabiv117__class_type_infoE", 0x00426D48, 0x10, 3, "85b724e3f6e2dd2648a696595eb637b234dcecc5cb770a5afbf0926341ebdfb4", "14509aa3575ce01cf70164cc878915676c5eb1d28667de7c4a5a7ae8e76bc6c5"),
    ("si_rtti", "tinfo", ".gnu.linkonce.d._ZTIN10__cxxabiv120__si_class_type_infoE", 0x00426D58, 0x10, 3, "85b724e3f6e2dd2648a696595eb637b234dcecc5cb770a5afbf0926341ebdfb4", "a1c6eb89d60a04d48d4c59f226fd0a1af8585abf17a961f1a40629397c8ba7e1"),
    ("vmi_rtti", "tinfo", ".gnu.linkonce.d._ZTIN10__cxxabiv121__vmi_class_type_infoE", 0x00426D68, 0x10, 3, "85b724e3f6e2dd2648a696595eb637b234dcecc5cb770a5afbf0926341ebdfb4", "98dd3d4ca2425e6a066fad1ad16cb2a871d236303176954657f4ebe8beb857fd"),
    ("bad_exception_vtable", "eh_exception", ".gnu.linkonce.d._ZTVSt13bad_exception", 0x00426D78, 0x18, 4, "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0", "06537825dff554f2bd8d1a2b8900f11bc22dac2178e2cc5e555d092060ac791b"),
    ("exception_vtable", "eh_exception", ".gnu.linkonce.d._ZTVSt9exception", 0x00426D90, 0x18, 4, "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0", "2a3b8223d67fb09dbaaf50efacc1a6728eaca99149cebb0f3d5a2cb1bea3a39e"),
    ("exception_rtti", "eh_exception", ".gnu.linkonce.d._ZTISt9exception", 0x00426DA8, 8, 2, "6cc16abd70eefb90dc0ba0d14fb088630873b2c6ad943f7442356735984c35a3", "65cbe6c4076a733e16b611738b95307050d1e9ca45488f880d9ea902afed469a"),
    ("bad_exception_rtti", "eh_exception", ".gnu.linkonce.d._ZTISt13bad_exception", 0x00426DB0, 0x10, 3, "85b724e3f6e2dd2648a696595eb637b234dcecc5cb770a5afbf0926341ebdfb4", "bb098d0396ab19014dc62cc58d1249c29e6b6899e1422107bf616464e00fb291"),
    ("bad_alloc_vtable", "new_handler", ".gnu.linkonce.d._ZTVSt9bad_alloc", 0x00426DC0, 0x18, 4, "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0", "ee615329218901a370485e963f7eeb2cc577fd800d99b2b48ea2d30a9406d4e0"),
    ("bad_alloc_rtti", "new_handler", ".gnu.linkonce.d._ZTISt9bad_alloc", 0x00426DD8, 0x10, 3, "85b724e3f6e2dd2648a696595eb637b234dcecc5cb770a5afbf0926341ebdfb4", "f364aacc28db8f3f5ca4b221d78d63098977172999aa6baba36149921d214841"),
)

SOURCE_SECTIONS += tuple(
    P(name, obj, source_section, 0, address, size, relocations, raw, linked)
    for name, obj, source_section, address, size, relocations, raw, linked in RTTI_SECTIONS
)


def A(value: int) -> tuple[str, int]: return ("advance_loc4", value)
def C(value: int) -> tuple[str, int]: return ("def_cfa_offset", value)
def S(reg: int, value: int) -> tuple[str, int, int]: return ("offset_extended_sf", reg, value)
def F(pc: int, size: int, *ops: tuple) -> dict: return {"pc": pc, "size": size, "ops": ops}


SPC_FDES = (
    F(0x001806A4, 0x2A0, A(4), C(48), A(16), S(16,-12), S(17,-8), A(12), S(64,-4)),
    F(0x00180B80, 0x598, A(4), C(4272), A(32), S(16,-24), S(17,-20), S(18,-16), S(20,-8), S(64,-4), S(19,-12)),
    F(0x00181118, 0x2D8, A(4), C(96), A(44), S(16,-24), S(17,-20), S(18,-16), S(19,-12), S(20,-8), S(64,-4)),
    F(0x001813F0, 0x7BC, A(8), C(32), A(20), S(16,-8), S(64,-4)),
    F(0x00181BAC, 0x9B0, A(8), C(64), A(20), S(18,-8), S(64,-4), S(17,-12), A(12), S(16,-16)),
    F(0x00182984, 0x164, A(4), C(64), A(12), S(64,-4), S(17,-8), A(8), S(16,-12)),
    F(0x00182AE8, 0x1DC, A(4), C(7408), A(4), S(18,-24), A(24), S(21,-12), S(22,-8), S(64,-4), A(16), S(16,-32), S(17,-28), A(8), S(20,-16), A(12), S(19,-20)),
    F(0x00182CC4, 0x118, A(4), C(6336), A(4), S(19,-12), A(24), S(18,-16), S(20,-8), S(64,-4), A(16), S(16,-24), S(17,-20)),
    F(0x00182DDC, 0x230, A(4), C(8464), A(4), S(19,-28), A(24), S(23,-12), S(30,-8), S(64,-4), A(16), S(21,-20), S(22,-16), A(8), S(18,-32), A(8), S(17,-36), A(8), S(16,-40), A(12), S(20,-24)),
    F(0x0018300C, 0xD0, A(4), C(64), A(20), S(16,-16), S(17,-12), S(18,-8), S(64,-4)),
    F(0x001830DC, 0xD0, A(4), C(64), A(20), S(16,-16), S(17,-12), S(18,-8), S(64,-4)),
    F(0x001831AC, 0xF8, A(4), C(64), A(20), S(16,-16), S(17,-12), S(18,-8), S(64,-4)),
    F(0x001833A4, 0x14C, A(4), C(80), A(4), S(18,-8), A(20), S(16,-16), S(64,-4), A(8), S(17,-12)),
    F(0x001834F0, 0x168, A(4), C(80), A(4), S(17,-12), A(20), S(16,-16), S(64,-4), A(8), S(18,-8)),
)

LSDA_TABLES = (
    ((104,404,0,0),(512,8,528,0),(540,8,0,0)),
    ((152,2580,0,0),(2736,8,2752,0),(2764,8,0,0)),
    ((72,2036,0,0),(2112,8,2128,0),(2140,8,0,0)),
    ((48,240,0,0),(292,8,308,0),(320,8,0,0)),
    ((28,8,0,0),(40,8,1924,0),(56,1888,0,0)),
    ((24,8,0,0),(40,8,976,0),(60,8,0,0),(112,420,976,0),(988,8,0,0)),
)
LSDA_SIZES = (20, 21, 20, 20, 18, 28)
SEMANTIC_SECTIONS = (
    {"name":"spc7110_unwind", "section":".data.stage3k.spc7110_unwind", "address":0x00423560, "size":0x2F8},
    {"name":"program_break", "section":".data.stage3k.program_break", "address":0x00425A80, "size":0x4,
     "source":f"ps2sdk@{PS2SDK_SBRK_REVISION}:ee/libc/src/sbrk.c"},
    {"name":"mathfp_globals", "section":".data.stage3k.mathfp_globals", "address":0x00425ABC, "size":0xC,
     "source":"newlib-1.10.0:newlib/libm/mathfp/s_mathcnst.c"},
    {"name":"runtime_lsda", "section":".data.stage3k.runtime_lsda", "address":0x00426B20, "size":0x80},
    {"name":"inifile_rtti", "section":".data.stage3k.inifile_rtti", "address":0x00426C18, "size":0x28},
)

EXPECTED = {
    "source_sections": 34, "source_bytes": 9428, "source_relocations": 151,
    "semantic_sections": 5, "semantic_bytes": 944, "spc7110_fdes": 14,
    "exact_chunks": 17, "mismatching_chunks": 34,
    "differing_bytes": 1854246, "chunk50_differing_bytes": 0,
    "chunk_count": 51, "target_initialized_size": 3304936,
    "first_differing_address": 0x00100114, "target_sha256": TARGET_SHA256,
    "prior_differing_bytes": 1857419,
}


class TailMetadataError(RuntimeError): pass
def fail(message: str) -> None: raise TailMetadataError(message)
def digest(data: bytes) -> str: return hashlib.sha256(data).hexdigest()


def run(command: Sequence[str | Path], cwd: Path = ROOT) -> str:
    result = subprocess.run([str(x) for x in command], cwd=cwd, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        fail(f"command failed: {shlex.join(map(str, command))}\n{result.stdout[-5000:]}")
    return result.stdout.strip()


def section(elf: ELFFile, name: str):
    found = [item for item in elf.sections if item.name == name]
    if len(found) != 1: fail(f"missing/duplicate source section: {name}")
    return found[0]


def source_document() -> list[dict]: return [dict(row) for row in SOURCE_SECTIONS]
def semantic_document() -> list[dict]:
    fdes = [{**fde, "ops": [list(op) for op in fde["ops"]]} for fde in SPC_FDES]
    return [{**row, "fdes": fdes if row["name"] == "spc7110_unwind" else None}
            for row in SEMANTIC_SECTIONS]


def claims() -> dict[str, bool]:
    return {"window_50_exact": True, "private_target_bytes_stored": False,
            "replacement_elf": False, "unpacked_hash_matched": False,
            "packed_hash_matched": False}


def frozen_document(result: dict) -> dict:
    return {"format": FORMAT, "schema_version": SCHEMA,
            "prior_manifest_sha256": digest(stage3j.DEFAULT_MANIFEST.read_bytes()),
            "source_hashes": SOURCE_HASHES, "source_sections": source_document(),
            "semantic_sections": semantic_document(), "result": result, "claims": claims()}


def validate(args: argparse.Namespace) -> dict:
    stage3j.validate(stage3j.parse_args(["validate"]))
    try: doc = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc: fail(f"cannot read tail metadata manifest: {exc}")
    if doc.get("format") != FORMAT or doc.get("schema_version") != SCHEMA: fail("tail metadata identity drift")
    if doc.get("prior_manifest_sha256") != digest(stage3j.DEFAULT_MANIFEST.read_bytes()): fail("prior checkpoint drift")
    if doc.get("source_hashes") != SOURCE_HASHES or doc.get("source_sections") != source_document(): fail("source contract drift")
    if doc.get("semantic_sections") != semantic_document(): fail("semantic contract drift")
    for key, value in EXPECTED.items():
        if doc.get("result", {}).get(key) != value: fail(f"frozen tail metadata metric drift: {key}")
    if doc.get("claims") != claims(): fail("tail metadata claim boundary drift")
    return doc


def prepare_sources(args: argparse.Namespace, cc: Path, cxx: Path) -> dict[str, Path]:
    try:
        archive = stage3i.v46.download_archive(stage3i.v46.SNES_ARCHIVE, stage3i.v46.SNES_CACHE)
        source_root = stage3i.v46.safe_extract_archive(
            archive, stage3i.v46.SNES_CACHE / "source", stage3i.v46.SNES_ARCHIVE.source_directory)
    except stage3i.v46.BuildFailure as exc: fail(str(exc))
    snes = source_root / "snes9x"
    zlib = source_root / "zlib"
    newlib = stage3i.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    math_constants = stage3i.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libm/mathfp/s_mathcnst.c"
    if digest(math_constants.read_bytes()) != SOURCE_HASHES["s_mathcnst.c"]:
        fail("s_mathcnst.c source hash drift")
    source_cache = args.runtime_build / "source-cache.git"
    sbrk = subprocess.run(
        ["git", f"--git-dir={source_cache}", "show", f"{PS2SDK_SBRK_REVISION}:ee/libc/src/sbrk.c"],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
    )
    if sbrk.returncode or digest(sbrk.stdout) != SOURCE_HASHES["sbrk.c"]:
        fail("historical PS2SDK sbrk.c source hash drift")
    gcc_include = Path(run([cxx, "-print-file-name=include"])).resolve()
    compat = args.build_dir / "compat-v46"
    compat.mkdir(parents=True, exist_ok=True)
    (compat / "memory.h").write_text(
        "#ifndef STAGE3K_MEMORY_H\n#define STAGE3K_MEMORY_H\n#include <string.h>\n#endif\n",
        encoding="utf-8",
    )
    common = [*stage3i.v47.COMMON_FLAGS, "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-nostdinc",
              *stage3i.v47.include_args([compat, newlib, snes, snes / "unzip", zlib, gcc_include])]
    output = args.build_dir / "source-objects"
    objects: dict[str, Path] = {}
    paths = {"explode": snes/"unzip/explode.c", "unreduce": snes/"unzip/unreduce.c",
             "deflate": zlib/"deflate.c", "gzio": zlib/"gzio.c", "inftrees": zlib/"inftrees.c",
             "trees": zlib/"trees.c", "zutil": zlib/"zutil.c"}
    for name, source in paths.items():
        if digest(source.read_bytes()) != SOURCE_HASHES[source.name]: fail(f"historical source hash drift: {source.name}")
        obj = output / f"{name}.o"; stage3i.compile_one(cc, common, source, obj); objects[name] = obj
    source = snes / "srtc.cpp"
    if digest(source.read_bytes()) != SOURCE_HASHES[source.name]: fail("srtc.cpp source hash drift")
    obj = output / "srtc.o"; stage3i.compile_one(cxx, [*common, *stage3i.v47.SNES_DEFINES, "-x", "c++"], source, obj); objects["srtc"] = obj

    for name in ("SifRpcMain", "sif_cmd_main"):
        obj = args.runtime_build / f"objects/kernel/{name}.o"
        if not obj.is_file(): fail("missing runtime member objects; run make runtime-members")
        objects[name] = obj
    archive = cc.parents[1] / "lib/gcc-lib/ee/3.2.2/libgcc.a"
    ar = cc.with_name("ee-ar")
    libgcc_dir = args.build_dir / "libgcc"; libgcc_dir.mkdir(parents=True, exist_ok=True)
    # Window 11 consumes _clz's public .rodata table as part of the next
    # integration tranche.  Extract it alongside the Stage-3K members so a
    # clean checkout can run the private pipeline without relying on a stale
    # object left by an earlier research command.
    for name in ("_divdi3", "_udivdi3", "_umoddi3", "unwind-dw2", "unwind-dw2-fde", "_moddi3", "_clz"):
        run([ar, "x", archive, f"{name}.o"], libgcc_dir); objects[name] = libgcc_dir / f"{name}.o"

    for name in ("tinfo", "new_handler"):
        obj = args.stage3j_build / f"source-objects/{name}.o"
        if not obj.is_file(): fail("missing Stage-3J libsupc++ objects; run make runtime-tail-data")
        objects[name] = obj
    work = cxx.parents[2]; gcc_source = work / "source/gcc-3.2.2"
    source = gcc_source / "libstdc++-v3/libsupc++/eh_exception.cc"
    if digest(source.read_bytes()) != SOURCE_HASHES[source.name]: fail("eh_exception.cc source hash drift")
    stage, newlib = stage3j.prepare_cxx_headers(gcc_source, args.build_dir)
    sup = source.parent
    flags = [f"-B{cxx.parent}{os.sep}", "-nostdinc++", f"-I{stage}", f"-I{sup}", f"-I{newlib}",
             "-G0", "-O2", "-EL", "-pipe", "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common",
             "-fshort-double", "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900"]
    obj = output / "eh_exception.o"; stage3i.compile_one(cxx, flags, source, obj); objects["eh_exception"] = obj
    return objects


def rebuild_payloads(objects: dict[str, Path], reference: bytes, build_dir: Path) -> list[dict]:
    rows = []
    for spec in SOURCE_SECTIONS:
        elf = ELFFile(objects[spec["object"]]); item = section(elf, spec["source_section"])
        start, size = spec["source_offset"], spec["size"]
        raw = stage3i.section_bytes(elf, item)[start:start+size]
        if len(raw) != size or digest(raw) != spec["raw_sha256"]: fail(f"source section drift: {spec['name']}")
        relocs = [(off-start, kind, name) for off,kind,name in historical_data.relocations(elf,item.index)
                  if start <= off < start+size]
        if len(relocs) != spec["relocations"] or any(kind != 2 for _,kind,_ in relocs): fail(f"relocation roster drift: {spec['name']}")
        target = reference[spec["address"]-TARGET_BASE:spec["address"]-TARGET_BASE+size]
        patched = bytearray(raw); mask = bytearray(size)
        for off, _kind, _name in relocs: mask[off:off+4] = b"\1"*4; patched[off:off+4] = target[off:off+4]
        if any(a != b for a,b,m in zip(raw,target,mask) if not m): fail(f"source differs outside relocations: {spec['name']}")
        if bytes(patched) != target or digest(target) != spec["linked_sha256"]: fail(f"linked source drift: {spec['name']}")
        path = build_dir / f"payloads/{spec['name']}.bin"; path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(patched)
        rows.append({**spec, "payload": path})
    return rows


def render_payload_source(rows: Sequence[dict]) -> str:
    lines = ["/* Generated from public source plus verified R_MIPS_32 results. */"]
    for row in rows:
        lines += [f'.section {row["section"]},"aw",@progbits', ".align 2", f'.incbin "{row["payload"]}"']
    return "\n".join(lines) + "\n"


def emit_ops(lines: list[str], fde: dict) -> None:
    for op in fde["ops"]:
        if op[0] == "advance_loc4": lines += [".byte 4", f".4byte {op[1]}"]
        elif op[0] == "def_cfa_offset": lines += [".byte 14", f".uleb128 {op[1]}"]
        else: lines += [".byte 17", f".uleb128 {op[1]}", f".sleb128 {op[2]}"]


def render_semantic_source() -> str:
    lines = ['.section .data.stage3k.spc7110_unwind,"aw",@progbits', ".align 2",
             ".Lst3k_frame:", ".4byte .Lst3k_cie_end-.Lst3k_cie_body", ".Lst3k_cie_body:",
             ".4byte 0", ".byte 1", '.ascii "zP\\0"', ".uleb128 1", ".sleb128 4", ".byte 64",
             ".uleb128 5", ".byte 0", ".4byte stage3k_gxx_personality", ".byte 12", ".uleb128 29", ".uleb128 0", ".Lst3k_cie_end:"]
    for index, fde in enumerate(SPC_FDES):
        body = f".Lst3k_fde_body_{index}"; end = f".Lst3k_fde_end_{index}"
        lines += [f".4byte {end}-{body}", f"{body}:", f".4byte {body}-.Lst3k_frame",
                  f".4byte stage3k_pc_{fde['pc']:08x}", f".4byte 0x{fde['size']:x}", ".uleb128 0"]
        emit_ops(lines, fde); lines += [".align 2", f"{end}:"]
    lines += [".4byte 0",
              '.section .data.stage3k.program_break,"aw",@progbits', ".align 2", ".4byte stage3k_end_of_heap",
              '.section .data.stage3k.mathfp_globals,"aw",@progbits', ".align 2",
              ".float 7.09782712893383973096e+02", ".float -7.45133219101941108420e+02",
              ".float 1.7263349182589107e-4",
              '.section .data.stage3k.runtime_lsda,"aw",@progbits', ".align 2"]
    for table in LSDA_TABLES:
        lines += [".byte 0xff, 0xff, 1", f".uleb128 {sum(len(uleb(v)) for row in table for v in row)}"]
        for row in table:
            lines += [f".uleb128 {value}" for value in row]
    lines += [".byte 0", '.section .data.stage3k.inifile_rtti,"aw",@progbits', ".align 2",
              ".4byte 0x00101838, 0x001041e4", ".4byte 0, 0x00426c38",
              ".4byte 0x00104358, 0x00104394, 0x001043e4, 0x00104418",
              ".4byte 0x00426ca8, 0x001badb8"]
    return "\n".join(lines) + "\n"


def uleb(value: int) -> bytes:
    out = bytearray()
    while True:
        byte = value & 0x7f; value >>= 7; out.append(byte | (0x80 if value else 0))
        if not value: return bytes(out)


def absorbed_aliases(input_path: Path, sections: Sequence[dict]) -> dict[str,int]:
    elf = ELFFile(input_path); result = {}
    for section_name in ABSORBED_FIXED:
        row = next(x for x in sections if x["section"] == section_name)
        item = section(elf, section_name); base = int(row["target_address"],0)
        for symbol in elf.symbols:
            if symbol.section_index == item.index and symbol.name and symbol.info >> 4 in (1,2):
                result[symbol.name] = base + symbol.value
    return result


def probe(args: argparse.Namespace) -> dict:
    prior_doc = stage3j.validate(stage3j.parse_args(["validate"]))
    sections, layout = startup.load_inputs(startup.parse_args(["validate", "--sections", str(args.sections), "--layout", str(args.layout)]))
    reference = args.reference.read_bytes()
    if digest(reference) != TARGET_SHA256: fail("private unpacked reference SHA-256 drift")
    cc, cxx = resolve_tool(args.c_compiler), resolve_tool(args.compiler)
    for compiler in (cc,cxx):
        if run([compiler,"-dumpversion"]) != "3.2.2" or run([compiler,"-dumpmachine"]) != "ee": fail("Stage-3K requires EE GCC 3.2.2")
    linker = resolve_tool(args.ld) if args.ld else cc.with_name("ee-ld")
    objcopy = resolve_tool(args.objcopy) if args.objcopy else cc.with_name("ee-objcopy")
    required = [args.input,args.startup_object,args.stage3i_build/"frontend-eh-frames.o",args.stage3i_build/"providers.o",
                args.stage3i_build/"semantic-cfi.o",args.stage3j_build/"runtime-tail.o"]
    if not all(p.is_file() for p in required): fail("missing Stage-3I/J dependency; run make runtime-tail-data")
    data_backing.check_sections(args.input, sections); args.build_dir.mkdir(parents=True, exist_ok=True)
    rows = rebuild_payloads(prepare_sources(args,cc,cxx),reference,args.build_dir)
    payload_source=args.build_dir/"tail-payloads.S"; payload_source.write_text(render_payload_source(rows),encoding="utf-8")
    payload_obj=args.build_dir/"tail-payloads.o"; stage3i.compile_one(cc,("-G0","-EL","-mno-abicalls","-march=r5900","-mtune=r5900"),payload_source,payload_obj)
    semantic_source=args.build_dir/"tail-semantics.S"; semantic_source.write_text(render_semantic_source(),encoding="utf-8")
    semantic_obj=args.build_dir/"tail-semantics.o"; stage3i.compile_one(cc,("-G0","-EL","-mno-abicalls","-march=r5900","-mtune=r5900"),semantic_source,semantic_obj)

    aliases=stage3i.absorbed_symbol_aliases(args.input,sections); aliases.update(absorbed_aliases(args.input,sections))
    prior_providers=[{"address":r["target_address"],"section":f".data.stage3i.source.{r['name']}"} for r in stage3i.PROVIDERS]
    retained=[r for r in sections if r["section"] not in ABSORBED_FIXED]
    script=stage3i.render_linker_script(retained,prior_providers,aliases)
    marker="  .bss.stage3g.crt0 0x00426e80"
    placements=[*({"section":r["section"],"address":r["address"]} for r in stage3j.SECTIONS),
                *({"section":r["section"],"address":r["address"]} for r in rows),*SEMANTIC_SECTIONS]
    insertion="\n".join(f"  {r['section']} 0x{r['address']:08x} : {{ KEEP(*({r['section']})) }}" for r in sorted(placements,key=lambda x:x["address"]))
    absorbed = " ".join(f"*({name})" for name in ABSORBED_FIXED)
    script=script.replace(marker,insertion+"\n"+marker,1).replace("*(.data.stage3g.crt0)",f"{absorbed} *(.data.stage3g.crt0)",1)
    for fde in SPC_FDES: script=f"stage3k_pc_{fde['pc']:08x} = 0x{fde['pc']:08x};\n"+script
    script=f"stage3k_end_of_heap = 0x00450c18;\nstage3k_gxx_personality = 0x{stage3i.PERSONALITY:08x};\n"+script
    linker_script=args.build_dir/"tail-metadata.ld"; linker_script.write_text(script,encoding="utf-8")
    output=args.build_dir/"stage3k-tail-metadata-integrated.elf"
    run([linker,"-EL","-T",linker_script,"-o",output,args.startup_object,args.input,
         args.stage3i_build/"frontend-eh-frames.o",args.stage3i_build/"providers.o",args.stage3i_build/"semantic-cfi.o",
         args.stage3j_build/"runtime-tail.o",payload_obj,semantic_obj])
    elf=ELFFile(output); startup.verify_symbols(elf); stage3i.verify_fixed_output(elf,retained)
    for row in placements:
        item=section(elf,row["section"]); target=reference[row["address"]-TARGET_BASE:row["address"]-TARGET_BASE+item.size]
        if item.address != row["address"] or stage3i.section_bytes(elf,item) != target: fail(f"linked tail section differs: {row['section']}")
    raw_path=args.build_dir/"stage3k-tail-metadata-integrated.unpadded.bin"; padded_path=args.build_dir/"stage3k-tail-metadata-integrated.padded.bin"
    run([objcopy,"-O","binary",output,raw_path]); raw=raw_path.read_bytes()
    if len(raw) > len(reference): fail("integrated diagnostic exceeds target image size")
    padded=raw+bytes(len(reference)-len(raw)); padded_path.write_bytes(padded)
    exact,different=link_layout_probe.compare_chunks(padded,layout); differences=[i for i,(a,b) in enumerate(zip(padded,reference)) if a!=b]
    return {"source_sections":len(rows),"source_bytes":sum(r["size"] for r in rows),"source_relocations":sum(r["relocations"] for r in rows),
            "semantic_sections":len(SEMANTIC_SECTIONS),"semantic_bytes":sum(r["size"] for r in SEMANTIC_SECTIONS),"spc7110_fdes":len(SPC_FDES),
            "chunk_count":len(exact)+len(different),"target_initialized_size":len(reference),
            "exact_chunks":len(exact),"mismatching_chunks":len(different),"exact_chunk_indices":exact,"mismatching_chunk_indices":different,
            "differing_bytes":len(differences),"chunk50_differing_bytes":sum(a!=b for a,b in zip(padded[50*65536:],reference[50*65536:])),
            "first_differing_address":TARGET_BASE+differences[0],"integrated_padded_sha256":digest(padded),"target_sha256":digest(reference),
            "prior_differing_bytes":prior_doc["result"]["differing_bytes"]}


def parse_args(argv: Sequence[str] | None=None) -> argparse.Namespace:
    p=argparse.ArgumentParser(description=__doc__); p.add_argument("command",choices=("validate","probe","capture"))
    p.add_argument("--manifest",type=Path,default=DEFAULT_MANIFEST);p.add_argument("--reference",type=Path,default=DEFAULT_REFERENCE)
    p.add_argument("--layout",type=Path,default=DEFAULT_LAYOUT);p.add_argument("--sections",type=Path,default=DEFAULT_SECTIONS)
    p.add_argument("--input",type=Path,default=DEFAULT_INPUT);p.add_argument("--startup-object",type=Path,default=DEFAULT_STARTUP)
    p.add_argument("--stage3i-build",type=Path,default=DEFAULT_STAGE3I);p.add_argument("--stage3j-build",type=Path,default=DEFAULT_STAGE3J)
    p.add_argument("--runtime-build",type=Path,default=DEFAULT_RUNTIME);p.add_argument("--build-dir",type=Path,default=DEFAULT_BUILD)
    p.add_argument("--compiler",default="ee-g++");p.add_argument("--c-compiler",default="ee-gcc");p.add_argument("--ld");p.add_argument("--objcopy")
    args=p.parse_args(argv)
    for name,value in vars(args).items():
        if isinstance(value,Path):setattr(args,name,value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None=None) -> int:
    args=parse_args(argv)
    try:
        if args.command=="validate":doc=validate(args)
        else:
            result=probe(args);doc=frozen_document(result)
            if args.command=="capture":args.manifest.parent.mkdir(parents=True,exist_ok=True);args.manifest.write_text(json.dumps(doc,indent=2,sort_keys=True)+"\n",encoding="utf-8")
            elif validate(args)!=doc:fail("private result differs from frozen manifest")
        r=doc["result"];print(f"verified tail metadata: source={r['source_sections']} sections/{r['source_bytes']} bytes; semantic={r['semantic_sections']} sections/{r['semantic_bytes']} bytes")
        print(f"whole-image chunks={r['exact_chunks']}/51 remaining={r['mismatching_chunks']} differing_bytes={r['differing_bytes']} chunk50_differences={r['chunk50_differing_bytes']}; complete ELF verified separately")
        return 0
    except (TailMetadataError,stage3j.RuntimeTailError,stage3i.HistoricalTailError,OSError,ValueError,KeyError,RuntimeError) as exc:
        print(f"tail metadata: FAIL -- {exc}");return 1


if __name__=="__main__":raise SystemExit(main())
