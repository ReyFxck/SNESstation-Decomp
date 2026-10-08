#!/usr/bin/env python3
"""Compile and audit the frozen SNES Station EE source-tree object set.

This closes source/object ownership only.  The relocatable aggregate is
expected to retain external references whose exact data placement, archive
member, linker script, and packing identity belong to later proof gates.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import re
import shlex
import shutil
import struct
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Sequence


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis" / "source_tree" / "translation_units.tsv"
DEFAULT_DEFINED = ROOT / "analysis" / "source_tree" / "defined_symbol_ownership.tsv"
DEFAULT_EXTERNAL = ROOT / "analysis" / "source_tree" / "external_symbol_ownership.tsv"
DEFAULT_ABI = ROOT / "analysis" / "source_tree" / "ee_abi_contract.c"
DEFAULT_SPECIAL = ROOT / "analysis" / "source_tree" / "special_ownership.tsv"
DEFAULT_FINGERPRINTS = ROOT / "analysis" / "source_tree" / "object_fingerprints.tsv"
DEFAULT_BUILD = ROOT / "build" / "source-tree"

EXPECTED_COMPILER_VERSION = "3.2.2"
EXPECTED_TARGET = "ee"
EXPECTED_ELF_FLAGS_BASE = 0x20924000
ELF_FLAGS_NOREORDER = 0x00000001

# Listing output and warning policy do not affect code generation, so the
# source-tree gate omits -Wa,-al/-Werror while preserving the historical code
# generation and ABI options from the root Makefile.
DEFAULT_CFLAGS = (
    "-G0 -O2 -EL -pipe -Wall "
    "-fomit-frame-pointer -fstrict-aliasing -fno-common "
    "-ffreestanding -fno-builtin -fshort-double "
    "-mlong64 -mhard-float -mno-abicalls "
    "-march=r5900 -mtune=r5900 "
    "-DPS2_EE -D_EE -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3 "
    "-Iinclude -Iinclude/ee_stage1_compat -w"
)

# kernel.S is the exact historical selector-based PS2SDK source.  Keep the
# source byte-identical and select only the members owned by this canonical TU
# at compile time instead of adding recovery-only #defines to the source.
SOURCE_FIXED_FLAGS = {
    'src/snes9x/native_c4_raster.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-fno-builtin', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_sa1.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_sa1_execution.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_cpu_interrupts.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_chip_io.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_controllers.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_rom_deinterleave.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/s9xsetpcbase.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/native_dma.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_ppu_registers.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections', '-fdata-sections'),
    'src/snes9x/native_hdma.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_fxemu.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/ppu_reset.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_sound_controls.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/native_sound_reset.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/apu_reset.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/dsp_table_init.c': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/dsp_dispatch.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/native_chip_resets.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/cpu_execution.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/cpu_reset.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/spc7110_access.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/spc7110_rtc_io.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/spc7110_helpers.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),
    'src/snes9x/seta_dispatch.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/st010_helpers.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/c4_math.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-fno-builtin', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/sdd1_log_save.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/sdd1_log_load.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-O2', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-falign-functions=4'),
    'src/snes9x/obc1.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/sdd1_map.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/memmap_methods.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/cheats2.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/snes9x/cheat_search.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    "src/snes9x/getbasepointer.cpp": (
        '-G0', '-EL', '-pipe', '-w',
        '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double',
        '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900',
        '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE',
        '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN',
        '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET',
    ),
    "src/snes9x/rtc_days.cpp": (
        '-G0', '-EL', '-pipe', '-w',
        '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double',
        '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900',
        '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE',
        '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN',
        '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET',
    ),
    "src/snes9x/srtc.cpp": (
        '-G0', '-EL', '-pipe', '-w',
        '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double',
        '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900',
        '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE',
        '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN',
        '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET',
    ),
    'src/snes9x/selecttilerenderer.cpp': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET'),
    'src/ps2/sjpcm_rpc.c': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DALIGN_DWORD', '-DCODE_PLATFORM=3', '-Iinclude', '-Iinclude/ee_stage1_compat'),
    'src/ps2/amigamod_rpc.c': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DALIGN_DWORD', '-DCODE_PLATFORM=3', '-Iinclude', '-Iinclude/ee_stage1_compat'),
    "src/snes9x/apumem.cpp": (
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
    ),
    # C4DrawWireFrame is a recovered historical Snes9x C++ translation unit.
    # Keep the exact V77 compiler profile rather than inheriting application
    # flags such as -fshort-double.
    "src/snes9x/c4drawwireframe.cpp": (
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-mlong64", "-mhard-float",
        "-mno-abicalls", "-march=r5900", "-mtune=r5900", "-Os",
        "-fno-builtin", "-DPS2_EE", "-D_EE", "-DLSB_FIRST",
        "-DVAR_CYCLES", "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN",
        "-DEXECUTE_SUPERFX_PER_LINE", "-DSPC700_C", "-DUNZIP_SUPPORT",
        "-DNO_INLINE_SET_GET",
    ),
    "src/snes9x/s9xgetmempointer.cpp": (
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
    ),
    "src/snes9x/s9xgetbyte.cpp": (
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
    ),
    "src/snes9x/s9xsetbyte.cpp": (
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
    ),
    "src/snes9x/s9xgetword.cpp": (
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
        "-DFAST_LSB_WORD_ACCESS",
    ),
    "src/snes9x/s9xsetword.cpp": (
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
    ),
    "src/snes9x/cpushutdown.cpp": (
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
    ),
}

SOURCE_FLAGS = {
    # The selected PS2LIB libc/memcpy.o was built with the historical -Os
    # profile.  Appending -Os after the source-tree's general -O2 reproduces
    # the compiler-defined __OPTIMIZE_SIZE__ path without modifying memcpy.S.
    "src/ps2/memcpy.S": ("-Os",),
    "src/ps2/memset.S": ("-Os",),
    "src/ps2/memmove.S": ("-Os",),
    "src/ps2/strcat.S": ("-Os",),
    "src/ps2/memcmp.S": ("-Os",),
    "src/ps2/strcpy.S": ("-Os",),
    "src/ps2/strlen.S": ("-Os",),
    "src/ps2/strchr.S": ("-Os",),
    "src/ps2/strcmp.S": ("-Os",),
    "src/ps2/strncpy.S": ("-Os",),
    "src/ps2/strncmp.S": ("-Os",),
    "src/ps2/string.c": ("-Os",),
    # Original PS2LIB F_strstr was built with 32-bit long and hosted builtin
    # semantics, unlike the application's -mlong64 freestanding profile.
    # Both the historical member AND this canonical TU must link 136/136 exact bytes.
    "src/ps2/strstr.c": ("-Os", "-mlong32", "-fhosted"),
    "src/ps2/strtol.c": ("-Os",),
    "src/ps2/strcasecmp.c": ("-Os",),
    "src/ps2/strtok.c": ("-Os",),
    "src/ps2/strncasecmp.c": ("-Os",),
    "src/ps2/ctype.c": ("-Os",),
    "src/ps2/qsort.c": ("-Os",),
    "src/ps2/sbrk.c": ("-Os",),
}

# The original 2004 libc/strtol.o compiled using the old compiler default
# long ABI. Merely appending -mlong32 changes codegen: omit the application's
# -mlong64 flag exclusively for this TU, without changing any other source.
SOURCE_OMIT_FLAGS = {
    "src/ps2/strtol.c": ("-mlong64",),
    # Historical PS2LIB strncasecmp uses size_t under the compiler's default
    # 32-bit long ABI.  Keeping the application-wide -mlong64 changes the
    # n-- instruction by one opcode byte even though the source body is exact.
    "src/ps2/strncasecmp.c": ("-mlong64",),
    # PS2LIB sbrk.o was compiled with the small historical -Os profile, not
    # the application's freestanding/-mlong64 tuning bundle.
    "src/ps2/sbrk.c": (
        "-O2", "-Wall", "-fomit-frame-pointer", "-fstrict-aliasing",
        "-fno-common", "-ffreestanding", "-fno-builtin", "-fshort-double",
        "-mlong64", "-mhard-float", "-mno-abicalls", "-march=r5900",
        "-mtune=r5900", "-DLSB_FIRST", "-DALIGN_DWORD", "-DCODE_PLATFORM=3",
    ),
}


def effective_source_cflags(cflags: Sequence[str], source: str) -> list[str]:
    fixed = SOURCE_FIXED_FLAGS.get(source)
    if fixed is not None:
        return list(fixed)
    omit = SOURCE_OMIT_FLAGS.get(source, ())
    return [flag for flag in cflags if flag not in omit] + list(SOURCE_FLAGS.get(source, ()))


SOURCE_DEFINES = {
    "src/unzip/get_tree.S": (
        "get_tree_candidate=get_tree_recovered",
        "ReadByte=ReadByte_recovered",
        "bytebuf=g_bytebuf_recovered",
    ),
    "src/ps2/c4convoam.S": (
        "C4ConvOAM_candidate=snes_p28_0010c340",
    ),
    "src/ps2/progress28_structural_lift_recovered.c": (
        "C4ConvOAM_exact_target=snes_p28_0010c340",
        "C4DoScaleRotate_exact_target=snes_p28_0010c6f8",
        "C4TransformLines_exact_target=snes_p28_0010cfa4",
    ),
    "src/ps2/numtestf.S": (
        "numtestf_candidate=numtestf_001a06c0",
    ),
    "src/ps2/newlib_mathfp_recovered.c": (
        "numtestf_target=numtestf_001a06c0",
    ),
    "src/ps2/strtol.c": ("F_strtol",),
    "src/ps2/strcasecmp.c": ("F_strcasecmp",),
    "src/ps2/strtok.c": ("F_strtok",),
    "src/ps2/strncasecmp.c": ("F_strncasecmp",),
    "src/ps2/ctype.c": ("F_ctype",),
    "src/ps2/string.c": ("F_strrchr",),
    "src/ps2/strstr.c": ("F_strstr",),
    "src/ps2/kernel.S": (
        "F_iWakeupThread",
        "F_CreateSema",
        "F_DeleteSema",
        "F_iSignalSema",
        "F_WaitSema",
        "F_FlushCache",
        "F_iFlushCache",
        "F_SifDmaStat",
        "F_SifSetDma",
        "F_SifSetReg",
        "F_SifGetReg",
        "F_SifWriteBackDCache",
        "F_AddDmacHandler",
        "F_RemoveDmacHandler",
        "F_SifStopDma",
        "F_iSifSetDma",
        "F_SifSetDChain",
        "F_iSifSetDChain",
        "F__EnableDmac",
        "F__DisableDmac",
        "F_SignalSema",
        "F_PollSema",
    ),
}

MANIFEST_FIELDS = (
    "order",
    "source",
    "subsystem",
    "language",
    "link_role",
    "object",
    "replaces",
    "rationale",
)
DEFINED_FIELDS = (
    "symbol",
    "binding",
    "section_class",
    "size_hex",
    "source",
    "object",
)
EXTERNAL_FIELDS = (
    "symbol",
    "category",
    "provider_kind",
    "owner",
    "resolution_gate",
    "requesters",
)
SPECIAL_FIELDS = (
    "kind",
    "identity",
    "source_owner",
    "object_owner",
    "stage2_state",
    "next_gate",
)
FINGERPRINT_FIELDS = ("kind", "source", "object", "sha256")

ADDRESS_SYMBOL_RE = re.compile(r"(?:^|_)([0-9a-fA-F]{8})$")
TARGET_DATA_RE = re.compile(
    r"^(?:_?DAT|PTR|UNK|LAB|switchdata|stack|REG|Ram|ram|s)_[0-9a-fA-F]{8}$"
)
TARGET_FUNCTION_RE = re.compile(r"^(?:FUN|sub)_[0-9a-fA-F]{8}$")
VTABLE_RE = re.compile(r"(?:vtable|vtt|typeinfo|_ZTV|_ZTI)", re.IGNORECASE)

LIBC_SYMBOLS = {
    "abort", "atexit", "calloc", "close", "exit", "fclose", "feof", "ferror",
    "fflush", "fgetc", "fgets", "fopen", "fprintf", "fputc", "fputs", "fread",
    "free", "fseek", "ftell", "fwrite", "getcwd", "getenv", "longjmp", "malloc",
    "memchr", "memcmp", "memcpy", "memmove", "memset", "open", "printf", "puts",
    "qsort", "read", "realloc", "remove", "rename", "setjmp", "snprintf", "sprintf",
    "strcat", "strchr", "strcmp", "strcpy", "strlen", "strncat", "strncmp",
    "strncpy", "strrchr", "strstr", "strtol", "strtoul", "tolower", "toupper",
    "vfprintf", "vsnprintf", "vsprintf", "write",
}
LIBGCC_PREFIXES = (
    "__add", "__ash", "__cmp", "__div", "__eq", "__extendsf", "__fix", "__float",
    "__gcc", "__gedf", "__gtdf", "__ledf", "__lshr", "__ltdf", "__make", "__mod",
    "__mul", "__neg", "__sub", "__trunc", "__udiv", "__umod", "__unord",
)
PS2_RUNTIME_PREFIXES = (
    "AddDmac", "CreateSema", "DeleteSema", "DisableDmac", "EnableDmac",
    "ExecPS2", "FlushCache", "GetThread", "LoadExecPS2", "PollSema", "ReferSema",
    "RemoveDmac", "SetDma", "SignalSema", "SleepThread", "StartThread", "Sif",
    "WaitSema", "WakeupThread", "fio", "mc", "mtap", "pad",
)
ZLIB_EXTERNAL_PREFIXES = (
    "adler32", "crc32", "deflate", "inflate", "zlib", "zError", "gz",
)

# Historical C++ peers that are program call contracts, not libsupc++ runtime
# entries. Their target identity is pinned by the reviewed link-contract gate;
# the mangled spelling itself does not encode an address and therefore must not
# be sent through the address-suffix alias gate.
HISTORICAL_CPP_PEERS = {
    '_ZN7CMemory7InitROMEh': 'src/snes9x/native_rom_deinterleave.cpp',
    "rand": "src/snes9x/native_ppu_registers.cpp",
    'S9xUpdateScreen': "src/snes9x/native_ppu_registers.cpp",
    'S9xSetSA1': "src/snes9x/native_ppu_registers.cpp",
    'S9xGetSA1': "src/snes9x/native_ppu_registers.cpp",
    'S9xDoDMA': "src/snes9x/native_ppu_registers.cpp",
    '_ZN7CMemory11FixROMSpeedEv': "src/snes9x/native_ppu_registers.cpp",

    "_Z10C4DrawLineiisiish": "src/ps2/progress28_structural_lift_recovered.c",
    "S9xGetPPU": "src/snes9x/s9xgetbyte.cpp",
    "S9xGetCPU": "src/snes9x/s9xgetbyte.cpp",
    "S9xGetDSP": "src/snes9x/s9xgetbyte.cpp",
    "S9xGetC4": "src/snes9x/s9xgetbyte.cpp",
    "S9xGetSPC7110Byte": "src/snes9x/s9xgetbyte.cpp",
    "S9xGetSPC7110": "src/snes9x/s9xgetbyte.cpp",
    "GetOBC1": "src/snes9x/s9xgetbyte.cpp",
    "S9xGetSetaDSP": "src/snes9x/s9xgetbyte.cpp",
    "S9xGetST018": "src/snes9x/s9xgetbyte.cpp",
    "S9xSetPPU": "src/snes9x/s9xsetbyte.cpp",
    "S9xSetCPU": "src/snes9x/s9xsetbyte.cpp",
    "S9xSetDSP": "src/snes9x/s9xsetbyte.cpp",
    "S9xSetC4": "src/snes9x/s9xsetbyte.cpp",
    "SetOBC1": "src/snes9x/s9xsetbyte.cpp",
    "S9xSetSetaDSP": "src/snes9x/s9xsetbyte.cpp",
    "S9xSetST018": "src/snes9x/s9xsetbyte.cpp",
}

# Data providers whose exact target addresses are part of byte-exact historical
# source link proofs (S9xGetByte/S9xSetByte/CPUShutdown). Keep them out of the
# historical Stage-3C tranche.
EXACT_TARGET_DATA_PEERS = {
    "g_CPU_blob",
    "g_OpenBus_byte",
    "g_SA1_blob",
    "g_s7r_blob",
    "g_ICPU_00345318",
    "g_APU_003453b8",
    "g_S9xAPUCycles_003f44a8",
    "g_S9xApuOpcodes_00411010",
}


class GateError(RuntimeError):
    """A reproducibility invariant failed."""


@dataclass(frozen=True)
class TranslationUnit:
    order: int
    source: str
    subsystem: str
    language: str
    link_role: str
    object: str
    replaces: str
    rationale: str

    @property
    def canonical(self) -> bool:
        return self.link_role == "canonical"


@dataclass(frozen=True)
class Symbol:
    name: str
    type_code: str
    value: str = ""
    size: str = ""

    @property
    def undefined(self) -> bool:
        return self.type_code.upper() == "U"

    @property
    def defined(self) -> bool:
        return not self.undefined

    @property
    def global_binding(self) -> bool:
        return self.type_code.isupper()


def fail(message: str) -> None:
    raise GateError(message)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def command_path(path: Path) -> str:
    """Use repository-relative inputs so debug/file records are clone-stable."""
    try:
        return path.resolve().relative_to(ROOT).as_posix()
    except ValueError:
        return str(path)


def run(
    command: Sequence[str],
    *,
    cwd: Path = ROOT,
    capture: bool = True,
    log_path: Path | None = None,
) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        list(command),
        cwd=cwd,
        text=True,
        stdout=subprocess.PIPE if capture else None,
        stderr=subprocess.STDOUT if capture else None,
        check=False,
    )
    if log_path is not None:
        log_path.parent.mkdir(parents=True, exist_ok=True)
        log_path.write_text(result.stdout or "", encoding="utf-8")
    if result.returncode != 0:
        rendered = shlex.join(str(part) for part in command)
        details = (result.stdout or "").strip()
        if len(details) > 4000:
            details = details[-4000:]
        fail(f"command failed ({result.returncode}): {rendered}\n{details}")
    return result


def resolve_tool(value: str) -> Path:
    candidate = Path(value)
    if candidate.is_file():
        return candidate.resolve()
    discovered = shutil.which(value)
    if discovered is None:
        fail(f"required tool not found: {value}")
    return Path(discovered).resolve()


def tool_family(compiler: Path, explicit: str | None, suffix: str) -> Path:
    if explicit:
        return resolve_tool(explicit)
    name = compiler.name
    if not name.endswith("gcc"):
        fail(f"cannot derive {suffix} from compiler name {name}; pass --{suffix}")
    return resolve_tool(str(compiler.with_name(name[:-3] + suffix)))


def validate_toolchain(compiler: Path) -> dict[str, str]:
    version = run([str(compiler), "-dumpversion"]).stdout.strip()
    target = run([str(compiler), "-dumpmachine"]).stdout.strip()
    banner = run([str(compiler), "--version"]).stdout.splitlines()[0].strip()
    if version != EXPECTED_COMPILER_VERSION:
        fail(f"expected EE GCC {EXPECTED_COMPILER_VERSION}, found {version}")
    if target != EXPECTED_TARGET:
        fail(f"expected compiler target {EXPECTED_TARGET!r}, found {target!r}")
    return {"version": version, "target": target, "banner": banner}


def read_manifest(path: Path) -> list[TranslationUnit]:
    if not path.is_file():
        fail(f"missing translation-unit manifest: {path.relative_to(ROOT)}")
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != MANIFEST_FIELDS:
            fail(f"unexpected columns in {path.relative_to(ROOT)}")
        rows = list(reader)

    units: list[TranslationUnit] = []
    sources: set[str] = set()
    objects: set[str] = set()
    for index, row in enumerate(rows, 1):
        try:
            order = int(row["order"])
        except ValueError as exc:
            fail(f"invalid order in manifest row {index}: {row['order']!r}")
            raise AssertionError from exc
        unit = TranslationUnit(order=order, **{key: row[key] for key in MANIFEST_FIELDS[1:]})
        if order != index:
            fail(f"manifest order must be contiguous: row {index} records {order}")
        if unit.source in sources:
            fail(f"duplicate source in manifest: {unit.source}")
        if unit.object in objects:
            fail(f"duplicate object in manifest: {unit.object}")
        if unit.language not in {"c", "cpp", "asm-cpp"}:
            fail(f"unsupported language for {unit.source}: {unit.language}")
        suffix = Path(unit.source).suffix
        expected_language = {".c": "c", ".cpp": "cpp", ".S": "asm-cpp"}.get(suffix)
        if expected_language != unit.language:
            fail(
                f"source/language mismatch for {unit.source}: "
                f"{unit.language} != {expected_language}"
            )
        if unit.link_role not in {"canonical", "alternate"}:
            fail(f"invalid link role for {unit.source}: {unit.link_role}")
        if not (ROOT / unit.source).is_file():
            fail(f"manifest source is missing: {unit.source}")
        sources.add(unit.source)
        objects.add(unit.object)
        units.append(unit)

    actual = {
        path.relative_to(ROOT).as_posix()
        for pattern in ("*.c", "*.cpp", "*.S")
        for path in (ROOT / "src").rglob(pattern)
    }
    if sources != actual:
        missing = sorted(actual - sources)
        stale = sorted(sources - actual)
        fail(f"translation-unit manifest drift; untracked={missing}, stale={stale}")

    canonical_sources = {unit.source for unit in units if unit.canonical}
    for unit in units:
        if unit.canonical and unit.replaces:
            fail(f"canonical unit unexpectedly replaces another: {unit.source}")
        if not unit.canonical and unit.replaces not in canonical_sources:
            fail(f"alternate {unit.source} has invalid canonical replacement {unit.replaces!r}")
    if not units:
        fail("translation-unit manifest is empty")
    return units


def validate_elf_object(path: Path) -> None:
    data = path.read_bytes()[:52]
    if len(data) != 52 or data[:4] != b"\x7fELF":
        fail(f"not an ELF object: {path}")
    if data[4:6] != b"\x01\x01":
        fail(f"object is not ELF32 little-endian: {path}")
    e_type, e_machine = struct.unpack_from("<HH", data, 16)
    e_flags = struct.unpack_from("<I", data, 36)[0]
    if e_type != 1 or e_machine != 8:
        fail(f"object is not an ELF32 MIPS relocatable: {path}")
    # The assembler sets EF_MIPS_NOREORDER only when the object actually
    # contains code emitted inside a .set noreorder region.  The ABI contract
    # intentionally emits no storage/code, so either value is valid.
    if e_flags & ~ELF_FLAGS_NOREORDER != EXPECTED_ELF_FLAGS_BASE:
        fail(
            f"unexpected MIPS flags for {path}: 0x{e_flags:08x} "
            f"(expected base 0x{EXPECTED_ELF_FLAGS_BASE:08x})"
        )


def parse_nm(text: str) -> list[Symbol]:
    symbols: list[Symbol] = []
    for raw in text.splitlines():
        line = raw.strip()
        if not line or line.endswith(":") or ": no symbols" in line:
            continue
        fields = line.split()
        if len(fields) < 2 or len(fields[1]) != 1:
            fail(f"cannot parse nm output: {raw!r}")
        symbols.append(
            Symbol(
                name=fields[0],
                type_code=fields[1],
                value=fields[2] if len(fields) >= 3 else "",
                size=fields[3] if len(fields) >= 4 else "",
            )
        )
    return symbols


def section_class(type_code: str) -> str:
    value = type_code.upper()
    return {
        "A": "absolute",
        "B": "bss",
        "C": "common",
        "D": "data",
        "G": "small-data",
        "N": "debug",
        "R": "rodata",
        "S": "small-bss",
        "T": "text",
        "V": "weak-object",
        "W": "weak-text",
    }.get(value, f"elf-{value.lower()}")


def load_source_readiness() -> dict[str, dict[str, str]]:
    path = ROOT / "analysis" / "source_readiness.csv"
    with path.open(encoding="utf-8", newline="") as stream:
        return {row["address"].lower(): row for row in csv.DictReader(stream)}


def classify_external(
    symbol: str, readiness: dict[str, dict[str, str]]
) -> tuple[str, str, str, str]:
    """Return category, provider kind, owner, and the future proof gate."""
    address_match = ADDRESS_SYMBOL_RE.search(symbol)
    address = f"0x{address_match.group(1).lower()}" if address_match else ""

    if TARGET_FUNCTION_RE.match(symbol):
        row = readiness.get(address)
        owner = row["source_files"] if row and row["source_files"] else address
        return "target-function-alias", "source-address-alias", owner, "link-identity"
    # Ghidra emits LAB_ names for both interior labels and callable entry
    # points. Promote only exact audited function entries; never infer code
    # identity from the LAB_ prefix alone.
    if symbol.startswith("LAB_"):
        row = readiness.get(address)
        if (row and row["manifest_status"] == "MATCHING"
                and row["matching_status"] == "MATCHING"
                and row["source_files"]):
            return "target-function-alias", "source-address-alias", row["source_files"], "link-identity"
    if VTABLE_RE.search(symbol):
        return "vtable-or-rtti", "program-data", "reserved:vtable-data.o", "program-data"
    if TARGET_DATA_RE.match(symbol) or symbol.startswith(("DAT_", "_DAT_")):
        return "target-address-data", "program-data", "reserved:target-data.o", "program-data"
    if symbol.startswith("embedded_"):
        return "embedded-binary", "private-asset", "reserved:embedded-assets.o", "program-data"
    historical_cpp_owner = HISTORICAL_CPP_PEERS.get(symbol)
    if historical_cpp_owner is not None:
        return "target-function-contract", "link-contract", historical_cpp_owner, "link-identity"
    # GCC's C++ personality entry uses a C ABI spelling rather than Itanium
    # _Z mangling, but it is still historical libsupc++ runtime.  Classify it
    # with the rest of the C++ archive family instead of Stage-3E named links.
    if symbol == "__gxx_personality_v0" or symbol.startswith("_Z"):
        return "cxx-runtime", "historical-archive", "libsupc++/libstdc++", "archive-identity"
    if symbol.startswith(LIBGCC_PREFIXES):
        return "compiler-runtime", "historical-archive", "libgcc", "archive-identity"
    from runtime_overrides import KIND as override_kind, ownership as override_ownership
    override = override_ownership(symbol)
    if override is not None:
        return "c-runtime", override_kind, *override
    # V93 pins the pre-2004 PS2LIB member lineage instead of attributing these
    # routines generically to Newlib or a modern PS2SDK archive. Full data and
    # final relocation identity remain separate from this member-text gate.
    from runtime_members import ownership as runtime_ownership
    runtime = runtime_ownership(symbol)
    if runtime is not None:
        category = "c-runtime" if symbol in LIBC_SYMBOLS else "ps2-runtime"
        return category, "historical-archive", *runtime
    if symbol in LIBC_SYMBOLS or symbol.startswith(("_impure", "__s", "stdin", "stdout", "stderr")):
        return "c-runtime", "historical-archive", "newlib/libc", "archive-identity"
    if symbol.startswith(PS2_RUNTIME_PREFIXES):
        return "ps2-runtime", "historical-archive", "PS2SDK runtime archives", "archive-identity"
    if symbol.startswith(ZLIB_EXTERNAL_PREFIXES):
        return "zlib-peer", "source-or-archive", "zlib object family", "link-identity"
    if symbol in EXACT_TARGET_DATA_PEERS:
        return "target-address-data", "program-data", "reserved:target-data.o", "program-data"
    if symbol.startswith(("g_", "GFX", "Memory", "CPU", "APU", "Settings", "IPPU", "PPU")):
        return "named-program-data", "program-data", "reserved:program-data.o", "program-data"
    if address:
        row = readiness.get(address)
        if row:
            owner = row["source_files"] or address
            return "target-address-symbol", "source-address-alias", owner, "link-identity"
    return "named-external", "link-contract", "reserved:link-contract", "link-identity"


def render_tsv(fields: Sequence[str], rows: Iterable[dict[str, str]]) -> str:
    from io import StringIO

    output = StringIO(newline="")
    writer = csv.DictWriter(output, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    return output.getvalue()


def validate_special_ownership(
    path: Path, defined_rows: Sequence[dict[str, str]]
) -> list[dict[str, str]]:
    if not path.is_file():
        fail(f"missing special ownership map: {path.relative_to(ROOT)}")
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != SPECIAL_FIELDS:
            fail(f"unexpected columns in {path.relative_to(ROOT)}")
        rows = list(reader)
    defined = {(row["symbol"], row["object"]) for row in defined_rows}
    seen: set[tuple[str, str]] = set()
    for row in rows:
        key = (row["kind"], row["identity"])
        if key in seen:
            fail(f"duplicate special ownership row: {key}")
        seen.add(key)
        owners = row["source_owner"].split(";")
        if not owners or any(not (ROOT / owner).is_file() for owner in owners):
            fail(f"invalid special source owner for {row['identity']}: {row['source_owner']}")
        if row["kind"] == "constructor":
            if (row["identity"], row["object_owner"]) not in defined:
                fail(f"constructor ownership does not match an object symbol: {row['identity']}")
            if row["stage2_state"] != "defined":
                fail(f"constructor must be defined in Stage 2: {row['identity']}")
        elif row["kind"] == "vtable":
            if row["object_owner"] != "reserved:vtable-data.o":
                fail(f"vtable has unexpected reserved object: {row['identity']}")
            match = re.search(r"0x([0-9a-fA-F]{8})", row["identity"])
            if match is None:
                fail(f"vtable identity lacks a target address: {row['identity']}")
            token = match.group(1).lower()
            if not any(token in (ROOT / owner).read_text(encoding="utf-8").lower() for owner in owners):
                fail(f"vtable address is not traced by its source owner: {row['identity']}")
        elif row["kind"] == "retired-target-data":
            if row["object_owner"] != "reserved:target-data.o":
                fail(f"retired target data has unexpected reserved object: {row['identity']}")
            if row["stage2_state"] != "consumer-retired":
                fail(f"retired target data has unexpected Stage-2 state: {row['identity']}")
            if not (TARGET_DATA_RE.match(row["identity"]) or row["identity"].startswith(("DAT_", "_DAT_"))):
                fail(f"retired target data identity is not address-bearing: {row['identity']}")
            if not any(row["identity"] in (ROOT / owner).read_text(encoding="utf-8") for owner in owners):
                fail(f"retired target data lacks source trace: {row['identity']}")
        else:
            fail(f"unsupported special ownership kind: {row['kind']}")
        if not row["next_gate"]:
            fail(f"special ownership row lacks next gate: {row['identity']}")
    return rows


def compare_or_write(path: Path, content: str, update: bool) -> None:
    if update:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8", newline="")
        return
    if not path.is_file():
        fail(f"missing frozen ownership map: {path.relative_to(ROOT)}; run source-tree-refresh")
    if path.read_text(encoding="utf-8") != content:
        fail(f"stale ownership map: {path.relative_to(ROOT)}; run source-tree-refresh and review")


def compile_one(
    compiler: Path,
    cxx: Path | None,
    cflags: Sequence[str],
    unit: TranslationUnit,
    build_dir: Path,
) -> tuple[TranslationUnit, Path]:
    output = build_dir / "objects" / unit.object
    log = build_dir / "logs" / Path(unit.object).with_suffix(".log")
    output.parent.mkdir(parents=True, exist_ok=True)
    defines = ["-D" + name for name in SOURCE_DEFINES.get(unit.source, ())]
    source_cflags = effective_source_cflags(cflags, unit.source)
    driver = cxx if unit.language == "cpp" else compiler
    if driver is None:
        fail(f"C++ translation unit requires --cxx: {unit.source}")
    command = [
        str(driver), *source_cflags, *defines,
        "-c", unit.source, "-o", str(output)
    ]
    run(command, log_path=log)
    validate_elf_object(output)
    return unit, output


def build(args: argparse.Namespace) -> dict[str, object]:
    compiler = resolve_tool(args.compiler)
    linker = tool_family(compiler, args.ld, "ld")
    nm = tool_family(compiler, args.nm, "nm")
    readelf = tool_family(compiler, args.readelf, "readelf")
    toolchain = validate_toolchain(compiler)
    units = read_manifest(args.manifest)
    cxx: Path | None = None
    cxx_toolchain: dict[str, str] | None = None
    if any(unit.language == "cpp" for unit in units):
        if not args.cxx:
            fail("manifest contains C++ translation units; pass --cxx")
        cxx = resolve_tool(args.cxx)
        cxx_toolchain = validate_toolchain(cxx)
    cflags = shlex.split(args.cflags)
    build_dir: Path = args.build_dir
    build_dir.mkdir(parents=True, exist_ok=True)

    abi_object = build_dir / "abi" / "ee_abi_contract.o"
    abi_log = build_dir / "logs" / "ee_abi_contract.log"
    abi_object.parent.mkdir(parents=True, exist_ok=True)
    run(
        [
            str(compiler),
            *cflags,
            "-c",
            command_path(args.abi_contract),
            "-o",
            str(abi_object),
        ],
        log_path=abi_log,
    )
    validate_elf_object(abi_object)

    compiled: dict[str, Path] = {}
    workers = max(1, min(args.jobs, len(units)))
    with ThreadPoolExecutor(max_workers=workers) as pool:
        futures = {
            pool.submit(compile_one, compiler, cxx, cflags, unit, build_dir): unit
            for unit in units
        }
        try:
            for future in as_completed(futures):
                unit, output = future.result()
                compiled[unit.source] = output
        except Exception:
            for future in futures:
                future.cancel()
            raise

    unit_symbols: dict[str, list[Symbol]] = {}
    object_hash_rows: list[dict[str, str]] = []
    defined_rows: list[dict[str, str]] = []
    global_owners: dict[str, list[str]] = {}
    common_symbols: list[str] = []

    for unit in units:
        object_path = compiled[unit.source]
        symbol_rows = parse_nm(run([str(nm), "-P", "-S", str(object_path)]).stdout)
        unit_symbols[unit.source] = symbol_rows
        object_hash_rows.append(
            {
                "source": unit.source,
                "object": unit.object,
                "link_role": unit.link_role,
                "sha256": sha256_file(object_path),
            }
        )
        if not unit.canonical:
            continue
        for symbol in symbol_rows:
            if not symbol.defined:
                continue
            if symbol.type_code.upper() == "C":
                common_symbols.append(f"{symbol.name} ({unit.source})")
            binding = "global" if symbol.global_binding else "local"
            defined_rows.append(
                {
                    "symbol": symbol.name,
                    "binding": binding,
                    "section_class": section_class(symbol.type_code),
                    "size_hex": f"0x{int(symbol.size or '0', 16):x}",
                    "source": unit.source,
                    "object": unit.object,
                }
            )
            if symbol.global_binding:
                global_owners.setdefault(symbol.name, []).append(unit.source)

    if common_symbols:
        fail("common storage violates -fno-common ownership: " + ", ".join(common_symbols[:10]))
    duplicates = {name: owners for name, owners in global_owners.items() if len(owners) > 1}
    if duplicates:
        sample = ", ".join(f"{name}={owners}" for name, owners in sorted(duplicates.items())[:10])
        fail(f"duplicate canonical global definitions: {sample}")

    # An alternate must overlap its declared canonical unit and must never leak
    # into the canonical aggregate.  New unique globals require an explicit
    # ownership decision rather than silently expanding the link set.
    alternates: list[dict[str, object]] = []
    for unit in units:
        if unit.canonical:
            continue
        alternate_globals = {
            symbol.name for symbol in unit_symbols[unit.source]
            if symbol.defined and symbol.global_binding
        }
        canonical_globals = {
            symbol.name for symbol in unit_symbols[unit.replaces]
            if symbol.defined and symbol.global_binding
        }
        overlap = sorted(alternate_globals & canonical_globals)
        unique = sorted(alternate_globals - canonical_globals)
        if not overlap:
            fail(f"alternate {unit.source} does not overlap {unit.replaces}")
        if unique:
            fail(f"alternate {unit.source} exports unowned unique globals: {unique}")
        alternates.append(
            {
                "source": unit.source,
                "replaces": unit.replaces,
                "overlap": overlap,
            }
        )

    canonical_objects = [compiled[unit.source] for unit in units if unit.canonical]
    aggregate = build_dir / "source-tree.partial.o"
    map_path = build_dir / "source-tree.partial.map"
    # _end is a linker-script absolute in the original executable. The exact
    # startup integration freezes it at 0x00450c18; resolve that same symbol
    # during the ownership partial-link so canonical sbrk.c does not create a
    # synthetic Stage-3 external contract.
    run(
        [
            str(linker), "-EL", "-r", "--defsym=_end=0x00450c18",
            "-Map", str(map_path), "-o", str(aggregate),
            *map(str, canonical_objects),
        ],
        log_path=build_dir / "logs" / "partial-link.log",
    )
    validate_elf_object(aggregate)
    aggregate_symbols = parse_nm(run([str(nm), "-P", "-S", str(aggregate)]).stdout)
    unresolved = sorted(symbol.name for symbol in aggregate_symbols if symbol.undefined)

    fingerprint_rows = [
        {
            "kind": "translation-unit",
            "source": row["source"],
            "object": row["object"],
            "sha256": row["sha256"],
        }
        for row in object_hash_rows
    ]
    fingerprint_rows.extend(
        (
            {
                "kind": "abi-contract",
                "source": command_path(args.abi_contract),
                "object": "abi/ee_abi_contract.o",
                "sha256": sha256_file(abi_object),
            },
            {
                "kind": "canonical-aggregate",
                "source": "",
                "object": "source-tree.partial.o",
                "sha256": sha256_file(aggregate),
            },
        )
    )

    requesters_by_symbol: dict[str, list[str]] = {}
    for unit in units:
        if not unit.canonical:
            continue
        for symbol in unit_symbols[unit.source]:
            if symbol.undefined:
                requesters_by_symbol.setdefault(symbol.name, []).append(unit.object)

    readiness = load_source_readiness()
    external_rows: list[dict[str, str]] = []
    for symbol in unresolved:
        category, provider_kind, owner, resolution_gate = classify_external(symbol, readiness)
        external_rows.append(
            {
                "symbol": symbol,
                "category": category,
                "provider_kind": provider_kind,
                "owner": owner,
                "resolution_gate": resolution_gate,
                "requesters": ";".join(sorted(set(requesters_by_symbol.get(symbol, [])))),
            }
        )

    defined_rows.sort(key=lambda row: (row["object"], row["symbol"], row["binding"]))
    special_rows = validate_special_ownership(args.special_map, defined_rows)

    # Exact assembly promotions can retire a provisional C consumer while the
    # target-address data contract it exposed remains part of the historical
    # link universe. Keep those reviewed contracts explicit without restoring
    # the provisional function body or inventing a live undefined reference.
    source_objects = {unit.source: unit.object for unit in units if unit.canonical}
    live_external_symbols = {row["symbol"] for row in external_rows}
    for row in special_rows:
        if row["kind"] != "retired-target-data":
            continue
        symbol = row["identity"]
        if symbol in live_external_symbols:
            fail(f"retired target-data contract returned to live externals: {symbol}")
        requesters = []
        for owner in row["source_owner"].split(";"):
            if owner not in source_objects:
                fail(f"retired target-data source is not canonical: {owner}")
            requesters.append(source_objects[owner])
        external_rows.append(
            {
                "symbol": symbol,
                "category": "target-address-data",
                "provider_kind": "program-data",
                "owner": row["object_owner"],
                "resolution_gate": row["next_gate"],
                "requesters": ";".join(sorted(set(requesters))),
            }
        )
        live_external_symbols.add(symbol)

    external_rows.sort(key=lambda row: row["symbol"])
    defined_text = render_tsv(DEFINED_FIELDS, defined_rows)
    external_text = render_tsv(EXTERNAL_FIELDS, external_rows)
    fingerprints_text = render_tsv(FINGERPRINT_FIELDS, fingerprint_rows)
    compare_or_write(args.defined_map, defined_text, args.update)
    compare_or_write(args.external_map, external_text, args.update)
    compare_or_write(args.fingerprints, fingerprints_text, args.update)

    section_counts: dict[str, int] = {}
    for row in defined_rows:
        key = row["section_class"]
        section_counts[key] = section_counts.get(key, 0) + 1
    category_counts: dict[str, int] = {}
    gate_counts: dict[str, int] = {}
    for row in external_rows:
        category_counts[row["category"]] = category_counts.get(row["category"], 0) + 1
        gate_counts[row["resolution_gate"]] = gate_counts.get(row["resolution_gate"], 0) + 1

    metadata: dict[str, object] = {
        "schema": 1,
        "claim": "build-ready-source-ownership",
        "compiler": str(compiler),
        "toolchain": toolchain,
        "cxx": str(cxx) if cxx is not None else "",
        "cxx_toolchain": cxx_toolchain,
        "linker": str(linker),
        "nm": str(nm),
        "readelf": str(readelf),
        "cflags": cflags,
        "manifest_sha256": sha256_file(args.manifest),
        "abi_contract_sha256": sha256_file(args.abi_contract),
        "special_ownership_sha256": sha256_file(args.special_map),
        "fingerprints_sha256": sha256_file(args.fingerprints),
        "translation_units": len(units),
        "canonical_objects": len(canonical_objects),
        "alternate_objects": len(units) - len(canonical_objects),
        "defined_symbols": len(defined_rows),
        "defined_global_symbols": sum(row["binding"] == "global" for row in defined_rows),
        "unresolved_external_symbols": len(external_rows),
        "special_ownership_rows": len(special_rows),
        "section_counts": dict(sorted(section_counts.items())),
        "external_category_counts": dict(sorted(category_counts.items())),
        "external_gate_counts": dict(sorted(gate_counts.items())),
        "alternates": alternates,
        "objects": object_hash_rows,
        "aggregate_sha256": sha256_file(aggregate),
    }
    (build_dir / "report.json").write_text(
        json.dumps(metadata, indent=2, sort_keys=True) + "\n", encoding="utf-8"
    )

    report = [
        "# EE build-ready source-tree report",
        "",
        f"- Historical C compiler: `{toolchain['banner']}` (`{toolchain['target']}`)",
        *(
            [f"- Historical C++ compiler: `{cxx_toolchain['banner']}` (`{cxx_toolchain['target']}`)"]
            if cxx_toolchain is not None else []
        ),
        f"- Translation units compiled: **{len(units)}/{len(units)}**",
        f"- Canonical objects in relocatable aggregate: **{len(canonical_objects)}**",
        f"- Explicit alternate objects: **{len(units) - len(canonical_objects)}**",
        f"- Owned defined symbols (global + local): **{len(defined_rows)}**",
        f"- Unique canonical global definitions: **{metadata['defined_global_symbols']}**",
        f"- Common or duplicate canonical definitions: **0**",
        f"- Classified unresolved external contracts: **{len(external_rows)}**",
        f"- Aggregate SHA-256: `{metadata['aggregate_sha256']}`",
        "",
        "This relocatable aggregate is not a replacement ELF. Its external contracts are",
        "frozen for the program-data, archive/link-identity and packing gates.",
        "",
    ]
    (build_dir / "report.md").write_text("\n".join(report), encoding="utf-8")
    return metadata


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default="ee-gcc")
    parser.add_argument("--cxx")
    parser.add_argument("--ld")
    parser.add_argument("--nm")
    parser.add_argument("--readelf")
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--defined-map", type=Path, default=DEFAULT_DEFINED)
    parser.add_argument("--external-map", type=Path, default=DEFAULT_EXTERNAL)
    parser.add_argument("--abi-contract", type=Path, default=DEFAULT_ABI)
    parser.add_argument("--special-map", type=Path, default=DEFAULT_SPECIAL)
    parser.add_argument("--fingerprints", type=Path, default=DEFAULT_FINGERPRINTS)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--cflags", default=DEFAULT_CFLAGS)
    parser.add_argument("--jobs", type=int, default=min(8, os.cpu_count() or 1))
    parser.add_argument(
        "--update",
        action="store_true",
        help="refresh committed ownership maps after deliberate review",
    )
    args = parser.parse_args(argv)
    for name in (
        "manifest",
        "defined_map",
        "external_map",
        "abi_contract",
        "special_map",
        "fingerprints",
        "build_dir",
    ):
        value: Path = getattr(args, name)
        if not value.is_absolute():
            setattr(args, name, (ROOT / value).resolve())
    if args.jobs < 1:
        parser.error("--jobs must be positive")
    return args


def main(argv: Sequence[str] | None = None) -> int:
    try:
        args = parse_args(argv)
        metadata = build(args)
    except GateError as exc:
        print(f"source-tree gate failed: {exc}", file=sys.stderr)
        return 1
    action = "refreshed" if args.update else "verified"
    print(
        f"{action} EE source tree: "
        f"translation_units={metadata['translation_units']} "
        f"canonical_objects={metadata['canonical_objects']} "
        f"alternate_objects={metadata['alternate_objects']} "
        f"defined_symbols={metadata['defined_symbols']} "
        f"external_contracts={metadata['unresolved_external_symbols']}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
