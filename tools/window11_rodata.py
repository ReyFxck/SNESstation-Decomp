#!/usr/bin/env python3
"""Integrate proven public-source rodata from image window 11.

Stage 3O rebuilds Snes9x 1.41-1 and zlib 1.1.3 sections and reuses already
validated PS2 runtime/libgcc/libsupc++ objects.  The private reference is used
only to verify extents and to supply final R_MIPS_32 words.  Generated payloads
remain in ignored build storage; no private target payload is committed.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import shlex
import subprocess
from pathlib import Path
from typing import Sequence

import data_backing
import historical_data
import historical_tail_data as stage3i
import link_layout_probe
import startup_integration as startup
import window35_data as stage3n
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/window11_rodata.json"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_STAGE3I = ROOT / "build/historical-tail-data"
DEFAULT_STAGE3J = ROOT / "build/runtime-tail-data"
DEFAULT_STAGE3K = ROOT / "build/tail-metadata"
DEFAULT_STAGE3L = ROOT / "build/window36-data"
DEFAULT_STAGE3M = ROOT / "build/media-assets"
DEFAULT_STAGE3N = ROOT / "build/window35-data"
DEFAULT_RUNTIME = ROOT / "build/runtime-members"
DEFAULT_SOURCE_TREE = ROOT / "build/source-tree/objects"
DEFAULT_BUILD = ROOT / "build/window11-rodata"

FORMAT = "snesstation-stage3o-window11-public-rodata"
SCHEMA = 1
TARGET_BASE = 0x00100000
TARGET_SHA256 = stage3n.TARGET_SHA256

SNES_SOURCE_HASHES = {
    "CHEATS.CPP": "1084dd908f528b8759aaa611e78746c20f2e88e70f852903b9c5d91c7c930316",
    "DMA.CPP": "00f554a8b1a9d5c44f184510742840ca2646785f96928afaeac6301eb8811d55",
    "DSP1.CPP": "4c0046c4bc05565b0638defc450e635a55403ce7d15be536d1a738312b337045",
    "fxdbg.cpp": "5e137d94d38ed0c2bfdf61a8f890716e8ada163884905df9a6de7da431676d37",
    "fxinst.cpp": "f427773319dfc6412642a160b199d73b1a72203503a391d60b7e425d10e6e801",
    "GFX.CPP": "1311d8a596bd9c6e4db6a94708f693f1235c238e2ae3858354f051e02194e622",
    "MEMMAP.CPP": "134ac471bceb0685409e159ed76d2a165c563f283d505e713fad63087ff717b4",
    "ppu.cpp": "18af9cc3d2feadaabc3c74a01abce54068d9cdc2b76d035c9cedd160efb50de7",
    "sa1.cpp": "0c4bd5ad3940820be0fabf6727571936e3c17b9c5a6391238fb2219099f2adb5",
    "seta010.cpp": "a06f4e816bcdb6071fc9595cfaece822f99e3bcb834126838bb230ffc200cd8f",
    "seta011.cpp": "db279d79692e618c724b74ef3391ae044bd3905e19a0a6bc91aab561ecfebdc2",
    "seta018.cpp": "0675c5b38a4bd2c3e30fca5403f64fcba2a7accdaf88a610a126eca06f6be3be",
    "snaporig.cpp": "f399fb8c5255371f79134b6d0e0b0c544efd1cde249273cbc574bd5ea7a92a44",
    "SNAPSHOT.CPP": "1eb232f492fcea488473083d515ae59970eb45a2c35e3cb90ebcda65b81661bc",
    "SOUNDUX.CPP": "449fc031b399ac86daf369d45bb69e68bab3cc00492b12734f7f78e3a87596eb",
    "SPC700.CPP": "e38f0c1030c0d8e909074f22149f019b66f8604f39db1419dc86c546572ad739",
    "spc7110.cpp": "9fe7a8a8bf04faf2b04823527c87304393647bc1fc1abacbb7d4b009c9354888",
    "srtc.cpp": "b6bbeaed929fffc965fb0a3833037fb742ef79110690f61f3e5e6a60a02b6bf9",
}

ZLIB_SOURCE_HASHES = {
    "deflate.c": "bc22df6685788955a9aeb049022424088d7be916a91e019d3ca5ff5167baf159",
    "inflate.c": "fac42b9bfdcd2ff4751e642a518dff921ae5a79a22f7eeda73845c803a89a623",
    "crc32.c": "8cf6068cb700e6aaa6ef1de424eeeb5526d75bd04a7188045eb27626655a31e0",
    "gzio.c": "9877368b9ad8ff4a7a57a082c57f5d9a35c6a16f878e9314224a854e0334d609",
    "infblock.c": "ba062cff2a248ce95062bc0f7f713f3fe70a276d13ba5566b0be51e8af1e7d18",
    "infcodes.c": "77d7f9a0ca839931e3d1d2a16648233f4ea048292c183607d18ad90b9080ff33",
    "inffast.c": "dc25c1a8c7de256437605a9dcb4356b23ef6a785529fe9beb782abd23d0bb4be",
    "inftrees.c": "ceb90191ca5b2bcccdfe3cfc3af92d3c6ebf6d4a2130c7c76989c32b6870c98b",
    "trees.c": "a671c6273a79429beef27a5d096506ecf899964ffa4e9f11fb7eb51d4943feb0",
    "zutil.c": "2f7f7d3273ea427a0ddac36b99f5536fbf110f72cee5ce557dcd600aeee775d5",
}


def P(name: str, obj: str, source_section: str, source_offset: int,
      address: int, size: int, full_size: int, relocations: int,
      full_sha256: str, raw_sha256: str, linked_sha256: str) -> dict:
    return {
        "name": name,
        "object": obj,
        "source_section": source_section,
        "source_offset": source_offset,
        "section": f".data.stage3o.source.{name}",
        "address": address,
        "size": size,
        "full_size": full_size,
        "relocations": relocations,
        "full_sha256": full_sha256,
        "raw_sha256": raw_sha256,
        "linked_sha256": linked_sha256,
    }


SOURCE_SECTIONS = (
    P("cheats_tail", "snes:CHEATS", ".rodata", 0xE1, 0x001B1C01, 0x37, 0x198, 0,
      "825ef4824df54ac03bf69471bba691272ad037730b58fea6215ba77ae1e97f47", "389326ab606b8b63f24d9b1c68821fa89c703aba455fd9dcb0afd081793a9def", "389326ab606b8b63f24d9b1c68821fa89c703aba455fd9dcb0afd081793a9def"),
    P("cheats_debug", "snes:CHEATS", ".rodata", 0x150, 0x001B1C38, 0x48, 0x198, 0,
      "825ef4824df54ac03bf69471bba691272ad037730b58fea6215ba77ae1e97f47", "3fe9e3f2441e96e5181f0468deb1edc02cee60c9d3ae70834c697c149c56fdee", "3fe9e3f2441e96e5181f0468deb1edc02cee60c9d3ae70834c697c149c56fdee"),
    P("dma_dispatch", "snes:DMA", ".rodata", 0, 0x001B1F20, 0x38, 0x38, 14,
      "51b4d1064a6ab0a1448f1e78d956c426e2cc738e2af672eb167d98c8a144a1a3", "51b4d1064a6ab0a1448f1e78d956c426e2cc738e2af672eb167d98c8a144a1a3", "e07df28581c169773fa711df20a20d75ccbfbc389cc60ef2bebc726b5cfc72af"),
    P("dma_inline_abs", "snes-inline:DMA", ".rodata", 0x110, 0x001B2030, 0x44, 0x198, 17,
      "5721190cb482d1c22f3eb99489937635b519bb223996e8196b34f7f2ccac45f7", "b585ec56ddd3c20c6cedb960baeb22603bf958f539af351d2b9120552e716604", "e0e414e12e937a64cc6363867e000549fc883bdf2ce82ce7e4a413fd4c8b6d3f"),
    P("dma_inline_offsets", "snes-inline:DMA", ".rodata", 0x154, 0x001B2074, 0x44, 0x198, 17,
      "5721190cb482d1c22f3eb99489937635b519bb223996e8196b34f7f2ccac45f7", "d0eb77760af1633e23775e6a8d9412a58e7fd6b698932221a3e40e97f96def4b", "d0eb77760af1633e23775e6a8d9412a58e7fd6b698932221a3e40e97f96def4b"),
    P("dsp1", "snes:DSP1", ".rodata", 0, 0x001B2120, 0xB80, 0xB80, 222,
      "aaa9075dce9b7cea299e6e9405f71b6a35de10651bb2bc615ef105f5d75c6777", "aaa9075dce9b7cea299e6e9405f71b6a35de10651bb2bc615ef105f5d75c6777", "b2cf93186ed9f934d8eb0e9be396f53a10491ce1e36e7633560b6f63a51460b6"),
    P("fxdbg", "snes:fxdbg", ".rodata", 0, 0x001B2CA0, 0x19D8, 0x19D8, 0,
      "c7f7d2742747e6cea1d583129e9023940d2a281f48a47a3d637570891f3e17f5", "c7f7d2742747e6cea1d583129e9023940d2a281f48a47a3d637570891f3e17f5", "c7f7d2742747e6cea1d583129e9023940d2a281f48a47a3d637570891f3e17f5"),
    P("fxinst", "snes:fxinst", ".rodata", 0, 0x001B4678, 0x40, 0x40, 0,
      "1a5b1effff5ff30023ec3dbd1ffd16c5d17b92013b794194e3cb7796ca01d34b", "1a5b1effff5ff30023ec3dbd1ffd16c5d17b92013b794194e3cb7796ca01d34b", "1a5b1effff5ff30023ec3dbd1ffd16c5d17b92013b794194e3cb7796ca01d34b"),
    P("gfx", "snes:GFX", ".rodata", 0, 0x001B46B8, 0x1CE0, 0x1CE0, 6,
      "c4e2d285df247340ee21802fc95e13b87eb915287e4e9ed886233abcbb269796", "c4e2d285df247340ee21802fc95e13b87eb915287e4e9ed886233abcbb269796", "62184d92fbcbcf23c7f4d7f865201fd6f2b33de380f1f2c2ff60bbf329cf5051"),
    P("memmap_prefix", "snes:MEMMAP", ".rodata", 0, 0x001B63D8, 0xB20, 0x1100, 0,
      "9137276f30f4eff45412264c0a20379b487f2114b4f037a3aa9ed31ba4816c63", "d77ceaa56edfbe58df4448df1c1cf77ef7295707acafda54f47e012f6eedc7be", "d77ceaa56edfbe58df4448df1c1cf77ef7295707acafda54f47e012f6eedc7be"),
    P("ppu", "snes:ppu", ".rodata", 0, 0x001B7318, 0xAC0, 0xAC0, 688,
      "1de0e1f700fc6b69cac20105ede85681b5d0c8378264b79d22a825dbf956476b", "1de0e1f700fc6b69cac20105ede85681b5d0c8378264b79d22a825dbf956476b", "d6b603abaa99ec2ee933d998fc63a06694619f57a90e7553eb7f2ba0e332c16f"),
    P("sa1", "snes:sa1", ".rodata", 0, 0x001B7DD8, 0x278, 0x278, 142,
      "28fb8f6227d14fbbc7d4c5ed6a79944bb7e8cb2cd46c2888d80f860af32c0148", "28fb8f6227d14fbbc7d4c5ed6a79944bb7e8cb2cd46c2888d80f860af32c0148", "3840850473acf567a4928690bd09759d35c58fc572a527cd9833ae6e2fd10b8b"),
    P("seta010", "snes:seta010", ".rodata", 0, 0x001B8058, 0x58, 0x58, 8,
      "f96a9e921e1b8c1dbe1cdecd1bfde7454d352d0ff3e919b33e26d15541c1d018", "f96a9e921e1b8c1dbe1cdecd1bfde7454d352d0ff3e919b33e26d15541c1d018", "89148f248b5291e888340b4c8aaf11335a1649aab72c1623e13283de749fb565"),
    P("seta011", "snes:seta011", ".rodata", 0, 0x001B80B0, 0x28, 0x28, 0,
      "2ac5634406c164d558a1dbc741fb3fda02c48301a3dd6d2ed317873682bf8725", "2ac5634406c164d558a1dbc741fb3fda02c48301a3dd6d2ed317873682bf8725", "2ac5634406c164d558a1dbc741fb3fda02c48301a3dd6d2ed317873682bf8725"),
    P("seta018", "snes:seta018", ".rodata", 0, 0x001B80D8, 0x18, 0x18, 0,
      "ae8ecbe06d560bb54ec48a81d4930d9638148fe7433d36597133a161e52f9239", "ae8ecbe06d560bb54ec48a81d4930d9638148fe7433d36597133a161e52f9239", "ae8ecbe06d560bb54ec48a81d4930d9638148fe7433d36597133a161e52f9239"),
    P("snaporig", "snes-inline:snaporig", ".rodata", 0, 0x001B80F0, 0x100, 0x100, 12,
      "27e026c5d4bf8ea9f2eeb3d236dba1bd78abcd12fd0f10a7d63a50d5ab02478e", "27e026c5d4bf8ea9f2eeb3d236dba1bd78abcd12fd0f10a7d63a50d5ab02478e", "27e026c5d4bf8ea9f2eeb3d236dba1bd78abcd12fd0f10a7d63a50d5ab02478e"),
    P("snapshot_prefix", "snes-inline:SNAPSHOT", ".rodata", 0, 0x001B81F0, 0x1A0, 0x200, 0,
      "57a2d61b061cf64a87e42dda387d558bc464254384da5fd413125e8b5f8ae91b", "4612922af2d70da229e87cc2db0e916bdab563930b590ff1367244f5305c3082", "4612922af2d70da229e87cc2db0e916bdab563930b590ff1367244f5305c3082"),
    P("snapshot_tail", "snes-inline:SNAPSHOT", ".rodata", 0x1B0, 0x001B8390, 0x50, 0x200, 12,
      "57a2d61b061cf64a87e42dda387d558bc464254384da5fd413125e8b5f8ae91b", "d04815e5c740112e145156817da0abfe2889c62c40114ce27bc3ab4c128ac4d4", "d04815e5c740112e145156817da0abfe2889c62c40114ce27bc3ab4c128ac4d4"),
    P("soundux", "snes:SOUNDUX", ".rodata", 0, 0x001B8408, 0xB0, 0xB0, 36,
      "06eb19b38daa648751ae6c7393f5327cdde61630d82a60c459a7943052fd6d5a", "06eb19b38daa648751ae6c7393f5327cdde61630d82a60c459a7943052fd6d5a", "14aa079a981e2ab33a5269356f14855b68dc8ff0593c4f153eca91e3d0148556"),
    P("spc700", "snes:SPC700", ".rodata", 0, 0x001B84B8, 0x38, 0x38, 0,
      "4faeb2f43ff9b4a2b7e8ef72537b8bea5a90c673848aad45a72fa1b20bddfbd0", "4faeb2f43ff9b4a2b7e8ef72537b8bea5a90c673848aad45a72fa1b20bddfbd0", "4faeb2f43ff9b4a2b7e8ef72537b8bea5a90c673848aad45a72fa1b20bddfbd0"),
    P("spc7110_messages", "snes:spc7110", ".rodata", 0x18, 0x001B85A0, 0x3C, 0x388, 0,
      "29bcd91b8a188de63cc3a8328234da19486109b976c5577cddd78c29d3cabc34", "523cbe03414b71154c988c190e186bf387091f979ad0b79f3c04339cccb45858", "523cbe03414b71154c988c190e186bf387091f979ad0b79f3c04339cccb45858"),
    P("spc7110_tables", "snes:spc7110", ".rodata", 0x5C, 0x001B85DC, 0x23C, 0x388, 142,
      "29bcd91b8a188de63cc3a8328234da19486109b976c5577cddd78c29d3cabc34", "19e7e6381149d5c979ec62241b0a4fcc85987de873abf8918b3e9b401eb500ab", "4a14fd01c79228283d6f2d2725c892d62ded9272157aad6c19ce1298205c14ed"),
    P("srtc", "snes:srtc", ".rodata", 0, 0x001B8878, 0x28, 0x28, 10,
      "6feed0c106ae4bcff7e64f13231803887bb849990cd30e0bfcb27f4616156604", "6feed0c106ae4bcff7e64f13231803887bb849990cd30e0bfcb27f4616156604", "a4319234f5238179f8d96847ffcf16b8db3d5fd73567843875b3a414153e20e5"),
    P("deflate", "zlib:deflate", ".rodata", 0, 0x001B8910, 0xB8, 0xB8, 10,
      "25efcacb04ae27e6227cc45f522c0b6fda318a7c59fabc39431a07b9fde266ea", "25efcacb04ae27e6227cc45f522c0b6fda318a7c59fabc39431a07b9fde266ea", "cdd4b99fe078f07da2d3e95ecf1bb58506ab66fad78ea4c902c38c341f89ea3a"),
    P("inflate", "zlib:inflate", ".rodata", 0, 0x001B89C8, 0xB8, 0xB8, 14,
      "af313b3f3e9ecdf27c0b330b8b3f184b2c26a8d21e5c0c35ecf64cdb3670e914", "af313b3f3e9ecdf27c0b330b8b3f184b2c26a8d21e5c0c35ecf64cdb3670e914", "f9854144bb5b5a3427972845c17beba0ecd569ba3aef53f0b08da7792903d8a3"),
    P("crc32", "zlib:crc32", ".rodata", 0, 0x001B8A80, 0x800, 0x800, 0,
      "a1eabca5212896cc791fc10209cf1de1140d81a28744fe76bb03bded740f5c9c", "a1eabca5212896cc791fc10209cf1de1140d81a28744fe76bb03bded740f5c9c", "a1eabca5212896cc791fc10209cf1de1140d81a28744fe76bb03bded740f5c9c"),
    P("gzio", "zlib:gzio", ".rodata", 0, 0x001B9280, 0x38, 0x38, 0,
      "88dbadae2bdfe86e588e1c54735bbd91971e5b4fcc9c4b218da882c92fae6d3f", "88dbadae2bdfe86e588e1c54735bbd91971e5b4fcc9c4b218da882c92fae6d3f", "88dbadae2bdfe86e588e1c54735bbd91971e5b4fcc9c4b218da882c92fae6d3f"),
    P("infblock", "zlib:infblock", ".rodata", 0, 0x001B92B8, 0xF8, 0xF8, 10,
      "44b6bc39fc8bd71968196611e955cae7e5370e2f9f03b8763fa4dad11957fa1c", "44b6bc39fc8bd71968196611e955cae7e5370e2f9f03b8763fa4dad11957fa1c", "778e57a727c5ea3494e171f30e924fbc001f666622788252f2d285495c1c3302"),
    P("infcodes", "zlib:infcodes", ".rodata", 0, 0x001B93B0, 0x60, 0x60, 10,
      "0935db441105b7494a520eb8b43cc27c233cf65f24fbdb2a487f552e84f95b31", "0935db441105b7494a520eb8b43cc27c233cf65f24fbdb2a487f552e84f95b31", "47b8969f9dde99b0dfc8cedd26c999520f643e203f5694c7e68ea0a17ecc762d"),
    P("inffast", "zlib:inffast", ".rodata", 0, 0x001B9410, 0x38, 0x38, 0,
      "1370f077419ada04caf5d6fe2f5cab8948401504e075f12ce91f0548bef89795", "1370f077419ada04caf5d6fe2f5cab8948401504e075f12ce91f0548bef89795", "1370f077419ada04caf5d6fe2f5cab8948401504e075f12ce91f0548bef89795"),
    P("inftrees", "zlib:inftrees", ".rodata", 0, 0x001B9448, 0x320, 0x320, 0,
      "df60de83617e55563ea08f7f91aa7403b9d8ef195824f73d45ca24e9c765e34f", "df60de83617e55563ea08f7f91aa7403b9d8ef195824f73d45ca24e9c765e34f", "df60de83617e55563ea08f7f91aa7403b9d8ef195824f73d45ca24e9c765e34f"),
    P("trees", "zlib:trees", ".rodata", 0, 0x001B9768, 0xA40, 0xA40, 0,
      "20016fec6f2cd70d97f7e9e1e1318cde1e2ef886ac443798540f3f8ca974f011", "20016fec6f2cd70d97f7e9e1e1318cde1e2ef886ac443798540f3f8ca974f011", "20016fec6f2cd70d97f7e9e1e1318cde1e2ef886ac443798540f3f8ca974f011"),
    P("zutil", "zlib:zutil", ".rodata", 0, 0x001BA1A8, 0xA0, 0xA0, 0,
      "47b2c78087060573bd53b258ab40e3e46934b452e05f345da91fb01e5ca6c390", "47b2c78087060573bd53b258ab40e3e46934b452e05f345da91fb01e5ca6c390", "47b2c78087060573bd53b258ab40e3e46934b452e05f345da91fb01e5ca6c390"),
    P("terminate", "runtime:terminate", ".rodata", 0, 0x001BA460, 0x18, 0x18, 0,
      "402643e0be8a9b6b5c82d6a4a1bb0dbbbb385bccb8a17978c694e91f8015753f", "402643e0be8a9b6b5c82d6a4a1bb0dbbbb385bccb8a17978c694e91f8015753f", "402643e0be8a9b6b5c82d6a4a1bb0dbbbb385bccb8a17978c694e91f8015753f"),
    P("formatter", "source:formatter", ".rodata", 0, 0x001BA478, 0x230, 0x230, 121,
      "1ab928fa0e3f7041873530ca72fc1d7d9089db1399cbdbb48f435e754fe22073", "1ab928fa0e3f7041873530ca72fc1d7d9089db1399cbdbb48f435e754fe22073", "1fbf1a5a03b710e6da91397155a00807b192bec33c21c6bfbf2f2abe006c008a"),
    P("libmc", "runtime:libmc", ".rodata", 0, 0x001BA758, 0x88, 0x88, 0,
      "f9f20fdd24810dbf0f5c4731b73781aa811d76e68ef6da15ef5f807d2c04a560", "f9f20fdd24810dbf0f5c4731b73781aa811d76e68ef6da15ef5f807d2c04a560", "f9f20fdd24810dbf0f5c4731b73781aa811d76e68ef6da15ef5f807d2c04a560"),
    P("unwind_fde", "libgcc:unwind-dw2-fde", ".rodata", 0, 0x001BA7F8, 0x34, 0x34, 13,
      "83f043798c93cd425efbb7c39b6e44c18ab8cf0d1888c912ccb436c67782e1a2", "83f043798c93cd425efbb7c39b6e44c18ab8cf0d1888c912ccb436c67782e1a2", "dd5e77771f30452a683639284e32c9b07adcf1b163cef04f9282a855837ae325"),
    P("clz", "libgcc:_clz", ".rodata", 0, 0x001BABC8, 0x100, 0x100, 0,
      "14a5d850c255623f9472e3c650abce0c78d32f0276b315b3a276a0462d97a1ac", "14a5d850c255623f9472e3c650abce0c78d32f0276b315b3a276a0462d97a1ac", "14a5d850c255623f9472e3c650abce0c78d32f0276b315b3a276a0462d97a1ac"),
    P("libpad", "source:libpad", ".rodata", 0, 0x001BACC8, 0xB0, 0xB0, 0,
      "aca55aff32d5fb65b777fafe4dfadfa179ff1e4b3c2bb8c37eaed1cba6d6a845", "aca55aff32d5fb65b777fafe4dfadfa179ff1e4b3c2bb8c37eaed1cba6d6a845", "aca55aff32d5fb65b777fafe4dfadfa179ff1e4b3c2bb8c37eaed1cba6d6a845"),
    P("eh_personality", "supcxx:eh_personality", ".rodata", 0, 0x001BAD78, 0x34, 0x34, 13,
      "58bc7e2b8c806205301ae78f30f63ae2f29a0beca4c75654dad043d14e938c4a", "58bc7e2b8c806205301ae78f30f63ae2f29a0beca4c75654dad043d14e938c4a", "b616496a5a5116de877c38ad3550f8e8da8c91d521a51c608ca5aea73080fc03"),
    P("rtti_vmi", "supcxx:tinfo", ".gnu.linkonce.r._ZTSN10__cxxabiv121__vmi_class_type_infoE", 0, 0x001BADC8, 0x28, 0x28, 0,
      "d386eb44724ea09b1f0dac37a8c4d403ebab1a7a4bbcfabd4d22797d2f72790c", "d386eb44724ea09b1f0dac37a8c4d403ebab1a7a4bbcfabd4d22797d2f72790c", "d386eb44724ea09b1f0dac37a8c4d403ebab1a7a4bbcfabd4d22797d2f72790c"),
    P("rtti_si", "supcxx:tinfo", ".gnu.linkonce.r._ZTSN10__cxxabiv120__si_class_type_infoE", 0, 0x001BADF0, 0x28, 0x28, 0,
      "bca0bbb3b89bc94a0fe98673e7f5ac881b17783f0db9cfddbd06d268c6196f00", "bca0bbb3b89bc94a0fe98673e7f5ac881b17783f0db9cfddbd06d268c6196f00", "bca0bbb3b89bc94a0fe98673e7f5ac881b17783f0db9cfddbd06d268c6196f00"),
    P("rtti_class", "supcxx:tinfo", ".gnu.linkonce.r._ZTSN10__cxxabiv117__class_type_infoE", 0, 0x001BAE18, 0x28, 0x28, 0,
      "453a673aac32bade1f9ab74e5a9ca727464fc572f227d70eb148ee08ac114367", "453a673aac32bade1f9ab74e5a9ca727464fc572f227d70eb148ee08ac114367", "453a673aac32bade1f9ab74e5a9ca727464fc572f227d70eb148ee08ac114367"),
    P("rtti_bad_typeid", "supcxx:tinfo", ".gnu.linkonce.r._ZTSSt10bad_typeid", 0, 0x001BAE40, 0x10, 0x10, 0,
      "68491e669e575bc275e796db0688f7cfe1ff6becf0f8a4385fa3d448f55eb4b0", "68491e669e575bc275e796db0688f7cfe1ff6becf0f8a4385fa3d448f55eb4b0", "68491e669e575bc275e796db0688f7cfe1ff6becf0f8a4385fa3d448f55eb4b0"),
    P("rtti_bad_cast", "supcxx:tinfo", ".gnu.linkonce.r._ZTSSt8bad_cast", 0, 0x001BAE50, 0x10, 0x10, 0,
      "0bcd98fa04402dfbbcf5a8b1880432286b0f034f970083d2170c8f6b6f8f55b9", "0bcd98fa04402dfbbcf5a8b1880432286b0f034f970083d2170c8f6b6f8f55b9", "0bcd98fa04402dfbbcf5a8b1880432286b0f034f970083d2170c8f6b6f8f55b9"),
    P("rtti_type_info", "supcxx:tinfo", ".gnu.linkonce.r._ZTSSt9type_info", 0, 0x001BAE60, 0x10, 0x10, 0,
      "34561976e7c77dc62f265933d56a6cb43ad72b2ca6e58a58da3a77ddb0f8282f", "34561976e7c77dc62f265933d56a6cb43ad72b2ca6e58a58da3a77ddb0f8282f", "34561976e7c77dc62f265933d56a6cb43ad72b2ca6e58a58da3a77ddb0f8282f"),
    P("rtti_bad_exception", "supcxx:eh_exception", ".gnu.linkonce.r._ZTSSt13bad_exception", 0, 0x001BAE70, 0x18, 0x18, 0,
      "f138e99bf77f94a4d8d8d74bc8aaf785debf7c5df9b5f2bf42124e0961fef3ee", "f138e99bf77f94a4d8d8d74bc8aaf785debf7c5df9b5f2bf42124e0961fef3ee", "f138e99bf77f94a4d8d8d74bc8aaf785debf7c5df9b5f2bf42124e0961fef3ee"),
    P("rtti_exception", "supcxx:eh_exception", ".gnu.linkonce.r._ZTSSt9exception", 0, 0x001BAE88, 0x10, 0x10, 0,
      "70b8be5fa9287333d9948cec81983a60bc0ee9b05b4230d08ec0c74f1ed6f9b1", "70b8be5fa9287333d9948cec81983a60bc0ee9b05b4230d08ec0c74f1ed6f9b1", "70b8be5fa9287333d9948cec81983a60bc0ee9b05b4230d08ec0c74f1ed6f9b1"),
    P("rtti_bad_alloc", "supcxx:new_handler", ".gnu.linkonce.r._ZTSSt9bad_alloc", 0, 0x001BAE98, 0x10, 0x10, 0,
      "1313ac91ff83b9bba803981c55dcff4a518c4be9d6438a58c96c86facb9ccca8", "1313ac91ff83b9bba803981c55dcff4a518c4be9d6438a58c96c86facb9ccca8", "1313ac91ff83b9bba803981c55dcff4a518c4be9d6438a58c96c86facb9ccca8"),
)

ABSORBED_FIXED = (
    ".data.stage3ce.va_001b6ed0",
    ".data.stage3f.va_001b8440",
    ".data.stage3f.recovered.va_001b85c8",
    ".data.stage3ce.va_001b98c0",
    ".data.stage3ce.va_001ba130",
)

EXACT_CHUNKS = list(range(12, 51))
EXPECTED = {
    "source_sections": 49,
    "source_bytes": 33_311,
    "source_relocations": 1_517,
    "absorbed_fixed_sections": 5,
    "window11_differing_bytes": 2_460,
    "exact_chunks": 39,
    "mismatching_chunks": 12,
    "differing_bytes": 620_746,
    "prior_differing_bytes": 644_215,
    "differences_removed": 23_469,
    "chunk_count": 51,
    "target_initialized_size": 3_304_936,
    "first_differing_address": 0x00100114,
    "integrated_padded_sha256": "f355fc45cb8b87631cafb982943ea02bb819126d3aef0d6aa847b555710b6180",
    "target_sha256": TARGET_SHA256,
}


class Window11RodataError(RuntimeError):
    """The Stage-3O public contract or private integration drifted."""


def fail(message: str) -> None:
    raise Window11RodataError(message)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def run(command: Sequence[str | Path], cwd: Path = ROOT) -> str:
    result = subprocess.run(
        [str(item) for item in command], cwd=cwd, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    if result.returncode:
        fail(f"command failed: {shlex.join(map(str, command))}\n{result.stdout[-5000:]}")
    return result.stdout.strip()


def section(elf: ELFFile, name: str):
    found = [item for item in elf.sections if item.name == name]
    if len(found) != 1:
        fail(f"missing/duplicate source section: {name}")
    return found[0]


def source_document() -> list[dict]:
    return [dict(row) for row in SOURCE_SECTIONS]


def claims() -> dict[str, bool]:
    return {
        "window_11_exact": False,
        "public_source_sections_rebuilt": True,
        "non_relocation_bytes_raw_exact": True,
        "private_oracle_limited_to_r_mips_32_results": True,
        "private_target_bytes_stored": False,
        "replacement_elf": False,
        "unpacked_hash_matched": False,
        "packed_hash_matched": False,
    }


def frozen_document(result: dict) -> dict:
    return {
        "format": FORMAT,
        "schema_version": SCHEMA,
        "prior_manifest_sha256": digest(stage3n.DEFAULT_MANIFEST.read_bytes()),
        "snes_source_hashes": SNES_SOURCE_HASHES,
        "zlib_source_hashes": ZLIB_SOURCE_HASHES,
        "source_sections": source_document(),
        "absorbed_fixed_sections": list(ABSORBED_FIXED),
        "result": result,
        "claims": claims(),
    }


def validate(args: argparse.Namespace) -> dict:
    stage3n.validate(stage3n.parse_args(["validate"]))
    try:
        document = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read window-11 rodata manifest: {exc}")
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("window-11 rodata identity drift")
    if document.get("prior_manifest_sha256") != digest(stage3n.DEFAULT_MANIFEST.read_bytes()):
        fail("prior checkpoint drift")
    if document.get("snes_source_hashes") != SNES_SOURCE_HASHES:
        fail("Snes9x source identity drift")
    if document.get("zlib_source_hashes") != ZLIB_SOURCE_HASHES:
        fail("zlib source identity drift")
    if document.get("source_sections") != source_document():
        fail("source-section contract drift")
    if document.get("absorbed_fixed_sections") != list(ABSORBED_FIXED):
        fail("absorbed section roster drift")
    for key, value in EXPECTED.items():
        if document.get("result", {}).get(key) != value:
            fail(f"frozen metric drift: {key}")
    if document.get("result", {}).get("exact_chunk_indices") != EXACT_CHUNKS:
        fail("exact-window roster drift")
    if document.get("claims") != claims():
        fail("claim boundary drift")
    return document


def prepare_objects(args: argparse.Namespace, cxx: Path) -> dict[str, Path]:
    ps2dev = stage3i.v47.PS2DEV
    try:
        stage3i.v47.ensure_git_commit(
            ps2dev, stage3i.v47.PS2DEV_REPO, stage3i.v47.PS2DEV_COMMIT
        )
    except SystemExit as exc:
        # Some older research helpers patched files in the shared ignored
        # checkout.  Never repair that cache in place: clone its committed
        # object database and compile against a clean detached worktree.
        if "tracked changes" not in str(exc):
            raise
        ps2dev = args.build_dir / "upstream/PS2DEV-bac0006c"
        if not (ps2dev / ".git").is_dir():
            run(["git", "clone", "-q", "--shared", str(stage3i.v47.PS2DEV), str(ps2dev)])
        stage3i.v47.ensure_git_commit(
            ps2dev, stage3i.v47.PS2DEV_REPO, stage3i.v47.PS2DEV_COMMIT
        )
    cc = cxx.with_name("ee-gcc")
    newlib = ps2dev / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    gcc_include = Path(run([cxx, "-print-file-name=include"])).resolve()
    old_build = stage3i.v52.BUILD
    try:
        stage3i.v52.BUILD = args.build_dir / "v52-rebuild"
        source_root, _original, layout = stage3i.v52.prepare_snes_layout()
        stage3i.v52.patch_sources(layout)
        compat = args.build_dir / "compat-v52"
        stage3i.v52.write_compat_headers(compat)
        include = stage3i.v47.include_args(
            (compat, newlib, layout, layout / "unzip", source_root / "zlib", gcc_include)
        )
        base_flags = [
            *stage3i.v47.COMMON_FLAGS, "-Os", *stage3i.v47.SNES_DEFINES,
            "-DZLIB", "-nostdinc", *include, "-x", "c++",
        ]
        inline_flags = [flag for flag in base_flags if flag != "-DNO_INLINE_SET_GET"]
        names = {
            key.split(":", 1)[1]
            for key in (row["object"] for row in SOURCE_SECTIONS)
            if key.startswith("snes:") or key.startswith("snes-inline:")
        }
        filename_by_stem = {Path(name).stem: name for name in SNES_SOURCE_HASHES}
        objects: dict[str, Path] = {}
        output = args.build_dir / "source-objects"
        for stem in sorted(names):
            filename = filename_by_stem[stem]
            source = layout / filename
            if digest(source.read_bytes()) != SNES_SOURCE_HASHES[filename]:
                fail(f"historical Snes9x source drift: {filename}")
            if any(row["object"] == f"snes:{stem}" for row in SOURCE_SECTIONS):
                obj = output / f"snes-{stem}.o"
                stage3i.compile_one(cxx, base_flags, source, obj)
                objects[f"snes:{stem}"] = obj
            if any(row["object"] == f"snes-inline:{stem}" for row in SOURCE_SECTIONS):
                obj = output / f"snes-inline-{stem}.o"
                stage3i.compile_one(cxx, inline_flags, source, obj)
                objects[f"snes-inline:{stem}"] = obj

        zlib = source_root / "zlib"
        zflags = [
            *stage3i.v47.COMMON_FLAGS, "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST",
            "-nostdinc", *stage3i.v47.include_args(
                (newlib, layout, layout / "unzip", zlib, gcc_include)
            ),
        ]
        for filename, expected_hash in ZLIB_SOURCE_HASHES.items():
            source = zlib / filename
            if digest(source.read_bytes()) != expected_hash:
                fail(f"historical zlib source drift: {filename}")
            stem = Path(filename).stem
            obj = output / f"zlib-{stem}.o"
            stage3i.compile_one(cc, zflags, source, obj)
            objects[f"zlib:{stem}"] = obj
    finally:
        stage3i.v52.BUILD = old_build

    objects.update({
        "runtime:terminate": args.runtime_build / "objects/libc/terminate.o",
        "runtime:libmc": args.runtime_build / "objects/mc/libmc.o",
        "source:formatter": args.source_tree / "ps2/ps2lib_formatter_recovered.o",
        "source:libpad": args.source_tree / "ps2/libpad_recovered.o",
        "libgcc:unwind-dw2-fde": args.stage3k_build / "libgcc/unwind-dw2-fde.o",
        "libgcc:_clz": args.stage3k_build / "libgcc/_clz.o",
        "supcxx:eh_personality": args.stage3j_build / "source-objects/eh_personality.o",
        "supcxx:tinfo": args.stage3j_build / "source-objects/tinfo.o",
        "supcxx:eh_exception": args.stage3k_build / "source-objects/eh_exception.o",
        "supcxx:new_handler": args.stage3j_build / "source-objects/new_handler.o",
    })
    missing = sorted(key for key, path in objects.items() if not path.is_file())
    if missing:
        fail(f"missing prerequisite object(s): {', '.join(missing)}")
    return objects


def rebuild_payloads(objects: dict[str, Path], reference: bytes,
                     build_dir: Path) -> list[dict]:
    rows = []
    for spec in SOURCE_SECTIONS:
        elf = ELFFile(objects[spec["object"]])
        item = section(elf, spec["source_section"])
        full = stage3i.section_bytes(elf, item)
        if len(full) != spec["full_size"] or digest(full) != spec["full_sha256"]:
            fail(f"full source section drift: {spec['name']}")
        start, size = spec["source_offset"], spec["size"]
        source = full[start:start + size]
        if len(source) != size or digest(source) != spec["raw_sha256"]:
            fail(f"source slice drift: {spec['name']}")
        relocs = [
            (offset - start, kind, name)
            for offset, kind, name in historical_data.relocations(elf, item.index)
            if start <= offset and offset + 4 <= start + size
        ]
        if len(relocs) != spec["relocations"] or any(kind != 2 for _, kind, _ in relocs):
            fail(f"R_MIPS_32 roster drift: {spec['name']}")
        target_start = spec["address"] - TARGET_BASE
        target = reference[target_start:target_start + size]
        patched, mask = bytearray(source), bytearray(size)
        for offset, _kind, _name in relocs:
            mask[offset:offset + 4] = b"\1" * 4
            patched[offset:offset + 4] = target[offset:offset + 4]
        if any(a != b for a, b, marked in zip(source, target, mask) if not marked):
            fail(f"source differs outside relocations: {spec['name']}")
        if bytes(patched) != target or digest(target) != spec["linked_sha256"]:
            fail(f"linked source payload drift: {spec['name']}")
        path = build_dir / f"payloads/{spec['name']}.bin"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(patched)
        rows.append({**spec, "payload": path})
    return rows


def render_payload_source(rows: Sequence[dict]) -> str:
    lines = ["/* Public source bytes plus verified R_MIPS_32 results. */"]
    for row in rows:
        lines.extend([
            f'.section {row["section"]},"aw",@progbits',
            f'.incbin "{row["payload"].resolve()}"',
        ])
    return "\n".join(lines) + "\n"


def verify_generated_object(path: Path) -> None:
    elf = ELFFile(path)
    for row in SOURCE_SECTIONS:
        item = section(elf, row["section"])
        if item.type != 1 or item.size != row["size"]:
            fail(f"generated geometry drift: {row['name']}")


def update_linker_script(base_script: str, input_path: Path,
                         sections: Sequence[dict]) -> tuple[str, list[dict]]:
    by_name = {row["section"]: row for row in sections}
    script = base_script
    for name in ABSORBED_FIXED:
        row = by_name.get(name)
        if row is None:
            fail(f"missing fixed section selected for absorption: {name}")
        address = int(row["target_address"], 0)
        placement = f"  {name} 0x{address:08x} : {{ KEEP(*({name})) }}\n"
        if script.count(placement) != 1:
            fail(f"cannot remove prior placement for {name}")
        script = script.replace(placement, "", 1)

    marker = "  .bss.stage3g.crt0 0x00426e80"
    insertion = "\n".join(
        f"  {row['section']} 0x{row['address']:08x} : {{ KEEP(*({row['section']})) }}"
        for row in sorted(SOURCE_SECTIONS, key=lambda item: item["address"])
    )
    if script.count(marker) != 1:
        fail("Stage-3N linker insertion marker drift")
    script = script.replace(marker, insertion + "\n" + marker, 1)

    discard_marker = "*(.data.stage3g.crt0)"
    if script.count(discard_marker) != 1:
        fail("Stage-3N discard marker drift")
    discarded_text = " ".join(f"*({name})" for name in ABSORBED_FIXED)
    script = script.replace(discard_marker, f"{discarded_text} {discard_marker}", 1)

    aliases = stage3n.stage3l.absorbed_aliases(input_path, sections, ABSORBED_FIXED)
    assignments = "".join(
        f"{name} = 0x{address:08x};\n" for name, address in sorted(aliases.items())
    )
    discarded = (
        set(stage3n.stage3l.stage3k.ABSORBED_FIXED)
        | set(stage3n.stage3l.ABSORBED_FIXED)
        | set(stage3n.stage3m.ABSORBED_FIXED)
        | set(stage3n.ABSORBED_FIXED)
        | set(ABSORBED_FIXED)
    )
    retained = [row for row in sections if row["section"] not in discarded]
    return assignments + script, retained


def probe(args: argparse.Namespace) -> dict:
    prior = stage3n.validate(stage3n.parse_args(["validate"]))
    sections, layout = startup.load_inputs(startup.parse_args([
        "validate", "--sections", str(args.sections), "--layout", str(args.layout)
    ]))
    reference = args.reference.read_bytes()
    if len(reference) != EXPECTED["target_initialized_size"] or digest(reference) != TARGET_SHA256:
        fail("private unpacked reference identity drift")
    data_backing.check_sections(args.input, sections)
    cxx = resolve_tool(args.compiler)
    if run([cxx, "-dumpversion"]) != "3.2.2" or run([cxx, "-dumpmachine"]) != "ee":
        fail("Stage-3O requires EE GCC 3.2.2")
    linker = resolve_tool(args.ld) if args.ld else cxx.with_name("ee-ld")
    objcopy = resolve_tool(args.objcopy) if args.objcopy else cxx.with_name("ee-objcopy")

    prior_inputs = [
        args.startup_object,
        args.input,
        args.stage3i_build / "frontend-eh-frames.o",
        args.stage3i_build / "providers.o",
        args.stage3i_build / "semantic-cfi.o",
        args.stage3j_build / "runtime-tail.o",
        args.stage3k_build / "tail-payloads.o",
        args.stage3k_build / "tail-semantics.o",
        args.stage3l_build / "window36-payloads.o",
        args.stage3l_build / "window36-semantics.o",
        args.stage3m_build / "media-assets.o",
        args.stage3n_build / "window35-payloads.o",
        args.stage3n_build / "window35-semantics.o",
    ]
    base_script = args.stage3n_build / "window35-data.ld"
    if not all(path.is_file() for path in [*prior_inputs, base_script]):
        fail("missing Stage-3N dependency; run make window35-data")

    args.build_dir.mkdir(parents=True, exist_ok=True)
    objects = prepare_objects(args, cxx)
    payloads = rebuild_payloads(objects, reference, args.build_dir)
    payload_source = args.build_dir / "window11-rodata.S"
    payload_source.write_text(render_payload_source(payloads), encoding="utf-8")
    payload_object = args.build_dir / "window11-rodata.o"
    stage3i.compile_one(
        cxx,
        ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
        payload_source,
        payload_object,
    )
    verify_generated_object(payload_object)

    script, retained = update_linker_script(
        base_script.read_text(encoding="utf-8"), args.input, sections
    )
    linker_script = args.build_dir / "window11-rodata.ld"
    linker_script.write_text(script, encoding="utf-8")
    output = args.build_dir / "stage3o-window11-rodata-integrated.elf"
    run([linker, "-EL", "-T", linker_script, "-o", output, *prior_inputs, payload_object])

    elf = ELFFile(output)
    startup.verify_symbols(elf)
    stage3i.verify_fixed_output(elf, retained)
    for row in SOURCE_SECTIONS:
        item = section(elf, row["section"])
        target = reference[
            row["address"] - TARGET_BASE:row["address"] - TARGET_BASE + row["size"]
        ]
        actual = stage3i.section_bytes(elf, item)
        if item.address != row["address"] or actual != target:
            fail(f"linked window-11 section differs: {row['name']}")

    raw_path = args.build_dir / "stage3o-window11-rodata-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3o-window11-rodata-integrated.padded.bin"
    run([objcopy, "-O", "binary", output, raw_path])
    raw = raw_path.read_bytes()
    if len(raw) > len(reference):
        fail("integrated diagnostic exceeds target size")
    padded = raw + bytes(len(reference) - len(raw))
    padded_path.write_bytes(padded)
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [i for i, (a, b) in enumerate(zip(padded, reference)) if a != b]
    result = {
        "source_sections": len(SOURCE_SECTIONS),
        "source_bytes": sum(row["size"] for row in SOURCE_SECTIONS),
        "source_relocations": sum(row["relocations"] for row in SOURCE_SECTIONS),
        "absorbed_fixed_sections": len(ABSORBED_FIXED),
        "window11_differing_bytes": sum(
            a != b for a, b in zip(
                padded[11 * 65536:12 * 65536], reference[11 * 65536:12 * 65536]
            )
        ),
        "exact_chunks": len(exact),
        "mismatching_chunks": len(different),
        "exact_chunk_indices": exact,
        "mismatching_chunk_indices": different,
        "differing_bytes": len(differences),
        "prior_differing_bytes": prior["result"]["differing_bytes"],
        "differences_removed": prior["result"]["differing_bytes"] - len(differences),
        "chunk_count": len(exact) + len(different),
        "target_initialized_size": len(reference),
        "first_differing_address": TARGET_BASE + differences[0],
        "integrated_padded_sha256": digest(padded),
        "target_sha256": digest(reference),
    }
    return result


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "probe", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--sections", type=Path, default=DEFAULT_SECTIONS)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--startup-object", type=Path, default=DEFAULT_STARTUP)
    parser.add_argument("--stage3i-build", type=Path, default=DEFAULT_STAGE3I)
    parser.add_argument("--stage3j-build", type=Path, default=DEFAULT_STAGE3J)
    parser.add_argument("--stage3k-build", type=Path, default=DEFAULT_STAGE3K)
    parser.add_argument("--stage3l-build", type=Path, default=DEFAULT_STAGE3L)
    parser.add_argument("--stage3m-build", type=Path, default=DEFAULT_STAGE3M)
    parser.add_argument("--stage3n-build", type=Path, default=DEFAULT_STAGE3N)
    parser.add_argument("--runtime-build", type=Path, default=DEFAULT_RUNTIME)
    parser.add_argument("--source-tree", type=Path, default=DEFAULT_SOURCE_TREE)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--compiler", default="ee-g++")
    parser.add_argument("--ld")
    parser.add_argument("--objcopy")
    args = parser.parse_args(argv)
    for name, value in vars(args).items():
        if isinstance(value, Path):
            setattr(args, name, value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "validate":
            document = validate(args)
        else:
            result = probe(args)
            document = frozen_document(result)
            if args.command == "capture":
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(
                    json.dumps(document, indent=2, sort_keys=True) + "\n", encoding="utf-8"
                )
            elif validate(args) != document:
                fail("private result differs from frozen manifest")
        result = document["result"]
        print(
            "verified window-11 public rodata: "
            f"source={result['source_sections']} sections/{result['source_bytes']} bytes; "
            f"relocations={result['source_relocations']}"
        )
        print(
            f"window11 remaining={result['window11_differing_bytes']} bytes; "
            f"whole-image chunks={result['exact_chunks']}/51 "
            f"differing_bytes={result['differing_bytes']}; replacement ELF: not yet"
        )
        return 0
    except (
        Window11RodataError,
        stage3n.Window35Error,
        stage3i.HistoricalTailError,
        data_backing.DataBackingError,
        OSError,
        ValueError,
        KeyError,
        RuntimeError,
    ) as exc:
        print(f"window-11 public rodata: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
