#!/usr/bin/env python3
"""Rebuild and verify the HUNT1000+ V45 GCC/libsupc++ runtime corridor."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))

from bootstrap_ee_gcc_stage1 import atomic_write_text, safe_extract_archive  # noqa: E402
from compare_elf_functions import ELFFile, compare_function  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
BUILD = ROOT / "build" / "matching" / "hunt1000plus-v45-runtime"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1000plus-v45-validated-runtime.tsv"
NEWLIB_ARCHIVE = (
    ROOT
    / "third_party"
    / "toolchain"
    / "newlib-1.10.0"
    / "newlib-1.10.0.tar.gz"
)


@dataclass(frozen=True)
class CandidateSpec:
    address: int
    family: str
    object_name: str
    symbol: str
    source_name: str
    identity: str = "historical-symbol-strict"


LIBGCC_SPECS = (
    CandidateSpec(0x001A4848, "libgcc-unwind", "unwind-dw2.o", "execute_cfa_program", "gcc/unwind-dw2.c"),
    CandidateSpec(0x001A4EF0, "libgcc-unwind", "unwind-dw2.o", "__frame_state_for", "gcc/unwind-dw2.c"),
    CandidateSpec(0x001A5AC0, "libgcc-unwind", "unwind-dw2.o", "_Unwind_Resume", "gcc/unwind-dw2.c"),
    CandidateSpec(0x001A5C58, "libgcc-unwind", "unwind-dw2.o", "_Unwind_DeleteException", "gcc/unwind-dw2.c"),
    CandidateSpec(0x001A5C80, "libgcc-unwind", "unwind-dw2.o", "_Unwind_GetGR", "gcc/unwind-dw2.c"),
    CandidateSpec(0x001A5C98, "libgcc-unwind", "unwind-dw2.o", "_Unwind_SetGR", "gcc/unwind-dw2.c"),
    CandidateSpec(0x001A5CC0, "libgcc-fde", "unwind-dw2-fde.o", "size_of_encoded_value", "gcc/unwind-dw2-fde.c"),
    CandidateSpec(0x001A5D30, "libgcc-fde", "unwind-dw2-fde.o", "read_uleb128", "gcc/unwind-dw2-fde.c"),
    CandidateSpec(0x001A5F30, "libgcc-fde", "unwind-dw2-fde.o", "__register_frame_info_bases", "gcc/unwind-dw2-fde.c"),
    CandidateSpec(0x001A5FA0, "libgcc-fde", "unwind-dw2-fde.o", "__register_frame", "gcc/unwind-dw2-fde.c"),
    CandidateSpec(0x001A5FF0, "libgcc-fde", "unwind-dw2-fde.o", "__register_frame_info_table_bases", "gcc/unwind-dw2-fde.c"),
    CandidateSpec(0x001A6058, "libgcc-fde", "unwind-dw2-fde.o", "__register_frame_table", "gcc/unwind-dw2-fde.c"),
    CandidateSpec(0x001A6168, "libgcc-fde", "unwind-dw2-fde.o", "__deregister_frame_info", "gcc/unwind-dw2-fde.c"),
    CandidateSpec(0x001A68F0, "libgcc-fde", "unwind-dw2-fde.o", "add_fdes", "gcc/unwind-dw2-fde.c", "anchored-object-layout"),
    CandidateSpec(0x001A70C0, "libgcc-fde", "unwind-dw2-fde.o", "init_object", "gcc/unwind-dw2-fde.c", "anchored-object-layout"),
    CandidateSpec(0x001A7438, "libgcc-fde", "unwind-dw2-fde.o", "fde_split", "gcc/unwind-dw2-fde.c", "anchored-object-layout"),
    CandidateSpec(0x001A7580, "libgcc", "_floatdidf.o", "__floatdidf", "gcc/libgcc2.c"),
    CandidateSpec(0x001A7638, "libgcc", "_moddi3.o", "__moddi3", "gcc/libgcc2.c", "anchored-object-layout"),
    CandidateSpec(0x001A7788, "libgcc", "_moddi3.o", "__udivmoddi4", "gcc/libgcc2.c", "anchored-object-layout"),
    CandidateSpec(0x001A7E30, "libgcc", "_unpack_sf.o", "__unpack_f", "gcc/config/fp-bit.c"),
    CandidateSpec(0x001A7F40, "libgcc", "_pack_df.o", "__pack_d", "gcc/config/fp-bit.c"),
    CandidateSpec(0x001A80D0, "libgcc", "_unpack_df.o", "__unpack_d", "gcc/config/fp-bit.c"),
)


LIBSUPCXX_SPECS = (
    CandidateSpec(0x001A91B0, "libsupcxx-eh", "eh_personality.o", "_Z21base_of_encoded_valuehP15_Unwind_Context", "libstdc++-v3/libsupc++/eh_personality.cc"),
    CandidateSpec(0x001A9308, "libsupcxx-eh", "eh_personality.o", "_Z28read_encoded_value_with_basehjPKhPj", "libstdc++-v3/libsupc++/eh_personality.cc"),
    CandidateSpec(0x001A9488, "libsupcxx-eh", "eh_personality.o", "_Z17parse_lsda_headerP15_Unwind_ContextPKhP16lsda_header_info", "libstdc++-v3/libsupc++/eh_personality.cc"),
    CandidateSpec(0x001A9588, "libsupcxx-eh", "eh_personality.o", "_Z15get_ttype_entryP16lsda_header_infom", "libstdc++-v3/libsupc++/eh_personality.cc"),
    CandidateSpec(0x001A95F8, "libsupcxx-eh", "eh_personality.o", "_Z16get_adjusted_ptrPKSt9type_infoS1_PPv", "libstdc++-v3/libsupc++/eh_personality.cc"),
    CandidateSpec(0x001A9698, "libsupcxx-eh", "eh_personality.o", "_Z20check_exception_specP16lsda_header_infoPKSt9type_infoPvl", "libstdc++-v3/libsupc++/eh_personality.cc"),
    CandidateSpec(0x001A9728, "libsupcxx-eh", "eh_personality.o", "__gxx_personality_v0", "libstdc++-v3/libsupc++/eh_personality.cc"),
    CandidateSpec(0x001A9B88, "libsupcxx-eh", "eh_personality.o", "__cxa_call_unexpected", "libstdc++-v3/libsupc++/eh_personality.cc"),
    CandidateSpec(0x001A9D58, "libsupcxx-eh", "eh_throw.o", "_Z23__gxx_exception_cleanup19_Unwind_Reason_CodeP17_Unwind_Exception", "libstdc++-v3/libsupc++/eh_throw.cc"),
    CandidateSpec(0x001A9DB8, "libsupcxx-eh", "eh_throw.o", "__cxa_throw", "libstdc++-v3/libsupc++/eh_throw.cc"),
    CandidateSpec(0x001A9E40, "libsupcxx-eh", "eh_throw.o", "__cxa_rethrow", "libstdc++-v3/libsupc++/eh_throw.cc"),
    CandidateSpec(0x001A9E88, "libsupcxx-new", "new_op.o", "_Znwj", "libstdc++-v3/libsupc++/new_op.cc"),
    CandidateSpec(0x001A9F68, "libsupcxx-new", "new_opv.o", "_Znaj", "libstdc++-v3/libsupc++/new_opv.cc"),
    CandidateSpec(0x001AA2C8, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv117__class_type_info10__do_catchEPKSt9type_infoPPvj", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AA320, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv117__class_type_info11__do_upcastEPKS0_PPv", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AA3A8, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv120__si_class_type_info20__do_find_public_srcEiPKvPKNS_17__class_type_infoES2_", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AA400, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv121__vmi_class_type_info20__do_find_public_srcEiPKvPKNS_17__class_type_infoES2_", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AA548, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv117__class_type_info12__do_dyncastEiNS0_10__sub_kindEPKS0_PKvS3_S5_RNS0_16__dyncast_resultE", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AA590, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv120__si_class_type_info12__do_dyncastEiNS_17__class_type_info10__sub_kindEPKS1_PKvS4_S6_RNS1_16__dyncast_resultE", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AA638, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv121__vmi_class_type_info12__do_dyncastEiNS_17__class_type_info10__sub_kindEPKS1_PKvS4_S6_RNS1_16__dyncast_resultE", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AAB20, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv117__class_type_info11__do_upcastEPKS0_PKvRNS0_15__upcast_resultE", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AAB58, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv120__si_class_type_info11__do_upcastEPKNS_17__class_type_infoEPKvRNS1_15__upcast_resultE", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AABD8, "libsupcxx-rtti", "tinfo.o", "_ZNK10__cxxabiv121__vmi_class_type_info11__do_upcastEPKNS_17__class_type_infoEPKvRNS1_15__upcast_resultE", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AAE70, "libsupcxx-rtti", "tinfo.o", "__dynamic_cast", "libstdc++-v3/libsupc++/tinfo.cc"),
    CandidateSpec(0x001AAFB8, "libsupcxx-eh", "eh_alloc.o", "__cxa_allocate_exception", "libstdc++-v3/libsupc++/eh_alloc.cc"),
    CandidateSpec(0x001AB080, "libsupcxx-eh", "eh_alloc.o", "__cxa_free_exception", "libstdc++-v3/libsupc++/eh_alloc.cc"),
    CandidateSpec(0x001AB0F0, "libsupcxx-eh", "eh_catch.o", "__cxa_begin_catch", "libstdc++-v3/libsupc++/eh_catch.cc"),
    CandidateSpec(0x001AB158, "libsupcxx-eh", "eh_catch.o", "__cxa_end_catch", "libstdc++-v3/libsupc++/eh_catch.cc"),
)


EVIDENCE_FIELDS = (
    "address", "name", "area", "provenance", "source", "profile", "detail",
    "object", "object_symbol", "object_size", "boundary", "result",
    "differing_bytes", "raw_equal", "normalized_equal", "unknown_relocations",
    "object_sha256", "cache_key",
)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT).as_posix()


def run(command: list[str], *, cwd: Path = ROOT) -> None:
    result = subprocess.run(
        command, cwd=cwd, text=True, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT, check=False,
    )
    if result.returncode:
        raise SystemExit(f"command failed ({result.returncode}): {' '.join(command)}\n{result.stdout[-6000:]}")


def link_force(source: Path, destination: Path) -> None:
    destination.parent.mkdir(parents=True, exist_ok=True)
    if destination.is_symlink() or destination.exists():
        destination.unlink()
    destination.symlink_to(source.resolve())


def prepare_cxx_headers(gcc_source: Path) -> tuple[Path, Path]:
    stage = BUILD / "cxx-include"
    bits = stage / "bits"
    bits.mkdir(parents=True, exist_ok=True)
    libstd = gcc_source / "libstdc++-v3"
    sup = libstd / "libsupc++"

    for header in (libstd / "include" / "c_std").glob("std_*.h"):
        link_force(header, stage / header.name.removeprefix("std_").removesuffix(".h"))
    for name in ("new", "exception", "typeinfo", "cxxabi.h", "exception_defines.h", "unwind-cxx.h"):
        link_force(sup / name, stage / name)
    for header in (libstd / "include" / "bits").iterdir():
        if header.is_file():
            link_force(header, bits / header.name)
    link_force(libstd / "config" / "os" / "generic" / "bits" / "os_defines.h", bits / "os_defines.h")
    link_force(gcc_source / "gcc" / "gthr-single.h", bits / "gthr.h")
    link_force(gcc_source / "gcc" / "unwind.h", stage / "unwind.h")
    link_force(gcc_source / "gcc" / "unwind-pe.h", stage / "unwind-pe.h")
    atomic_write_text(
        bits / "c++config.h",
        """#ifndef _CPP_CPPCONFIG
#define _CPP_CPPCONFIG 1
#include <bits/os_defines.h>
#define __GLIBCPP__ 20030205
#define _GLIBCPP_NO_TEMPLATE_EXPORT 1
#define _GLIBCPP_FULLY_COMPLIANT_HEADERS 1
#define _GLIBCPP_RESOLVE_LIB_DEFECTS 1
#define _GLIBCPP_AT_AT "@@"
#define __GXX_MERGED_TYPEINFO_NAMES 0
#endif
""",
    )

    extracted = safe_extract_archive(NEWLIB_ARCHIVE, BUILD / "newlib-source", "newlib-1.10.0")
    newlib_include = extracted / "newlib" / "libc" / "include"
    return stage, newlib_include


def compiler_prefix_args(cxx: Path, assembler_prefix: Path) -> list[str]:
    args: list[str] = []
    if cxx.name == "xgcc":
        args.extend([f"-B{cxx.parent.resolve()}{os.sep}"])
    args.append(f"-B{assembler_prefix.resolve()}{os.sep}")
    return args


def compile_libsupcxx(cxx: Path, assembler_prefix: Path, gcc_source: Path) -> Path:
    stage, newlib_include = prepare_cxx_headers(gcc_source)
    sup = gcc_source / "libstdc++-v3" / "libsupc++"
    output = BUILD / "libsupcxx"
    output.mkdir(parents=True, exist_ok=True)
    common = [
        "-nostdinc++", f"-I{stage}", f"-I{sup}", f"-I{newlib_include}",
        "-G0", "-O2", "-EL", "-pipe", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mhard-float",
        "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    ]
    source_names = sorted({spec.source_name for spec in LIBSUPCXX_SPECS})
    for source_name in source_names:
        source = gcc_source / source_name
        object_path = output / (source.stem + ".o")
        command = [str(cxx.resolve()), *compiler_prefix_args(cxx, assembler_prefix), *common, "-c", str(source), "-o", str(object_path)]
        run(command)
        cache_key = hashlib.sha256(
            json.dumps(
                {
                    "compiler_sha256": sha256_file(cxx),
                    "flags": command[1:-3],
                    "source_sha256": sha256_file(source),
                },
                sort_keys=True,
                separators=(",", ":"),
            ).encode()
        ).hexdigest()
        atomic_write_text(
            object_path.with_suffix(".json"),
            json.dumps(
                {"cache_key": cache_key, "source": rel(source), "profile": "libsupcxx-o2-long32"},
                indent=2,
                sort_keys=True,
            ) + "\n",
        )
    return output


def extract_libgcc(archive: Path, gcc_source: Path) -> Path:
    ar = archive.parents[4] / "bin" / "ee-ar"
    if not ar.is_file():
        raise SystemExit(f"missing ee-ar next to libgcc archive: {ar}")
    output = BUILD / "libgcc"
    output.mkdir(parents=True, exist_ok=True)
    members = sorted({spec.object_name for spec in LIBGCC_SPECS})
    for member in members:
        run([str(ar), "x", str(archive.resolve()), member], cwd=output)
        object_path = output / member
        source_name = next(spec.source_name for spec in LIBGCC_SPECS if spec.object_name == member)
        source = gcc_source / source_name
        cache_key = hashlib.sha256(
            f"{sha256_file(archive)}\0{member}\0{sha256_file(source)}".encode()
        ).hexdigest()
        atomic_write_text(
            object_path.with_suffix(".json"),
            json.dumps(
                {"cache_key": cache_key, "source": rel(source), "profile": "prebuilt-libgcc-archive"},
                indent=2,
                sort_keys=True,
            ) + "\n",
        )
    return output


def load_progress() -> tuple[dict[int, dict[str, str]], dict[int, int]]:
    with (ROOT / "analysis" / "progress_targets.csv").open(newline="", encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream))
    progress = {int(row["address"], 0): row for row in rows}
    starts = sorted(progress)
    return progress, {start: end for start, end in zip(starts, starts[1:])}


def metadata_for(object_path: Path) -> dict[str, str]:
    return json.loads(object_path.with_suffix(".json").read_text(encoding="utf-8"))


def make_evidence(
    reference: bytes,
    progress: dict[int, dict[str, str]],
    next_address: dict[int, int],
    libgcc_dir: Path,
    libsupcxx_dir: Path,
) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for spec in (*LIBGCC_SPECS, *LIBSUPCXX_SPECS):
        manifest = progress.get(spec.address)
        end = next_address.get(spec.address)
        if manifest is None or end is None:
            raise SystemExit(f"0x{spec.address:08x}: absent from audited target boundaries")
        if manifest["status"] not in {"RECONSTRUCTED", "MATCHING"}:
            raise SystemExit(f"0x{spec.address:08x}: unexpected status {manifest['status']}")
        object_path = (libgcc_dir if spec in LIBGCC_SPECS else libsupcxx_dir) / spec.object_name
        elf = ELFFile(object_path)
        symbol = elf.find_symbol(spec.symbol)
        comparison = compare_function(
            reference, spec.address - TARGET_BASE, symbol.size, elf, spec.symbol
        )
        if not comparison.matching or comparison.differing_bytes:
            raise SystemExit(f"0x{spec.address:08x}: candidate is not relocation-normalized exact")
        if comparison.unknown_relocation_types:
            raise SystemExit(f"0x{spec.address:08x}: unknown relocation types")
        span = end - spec.address
        gap = span - symbol.size
        if gap < 0 or gap > 64 or gap % 4:
            raise SystemExit(f"0x{spec.address:08x}: invalid boundary gap 0x{gap:x}")
        if gap:
            gap_start = spec.address - TARGET_BASE + symbol.size
            if reference[gap_start : gap_start + gap] != b"\0" * gap:
                raise SystemExit(f"0x{spec.address:08x}: nonzero target boundary gap")
            boundary = f"historical-symbol+target-zero-gap:0x{gap:x}"
        else:
            boundary = "exact-next-boundary"

        meta = metadata_for(object_path)
        provenance = (
            "ee-gcc-3.2.2-libgcc-a-v45"
            if spec in LIBGCC_SPECS
            else "gcc-3.2.2-libsupcxx-source-v45"
        )
        rows.append(
            {
                "address": f"0x{spec.address:08x}",
                "name": manifest["name"],
                "area": manifest["area"],
                "provenance": provenance,
                "source": meta["source"],
                "profile": meta["profile"],
                "detail": (
                    f"{spec.identity}; family={spec.family}; "
                    f"historical-symbol-size={symbol.size}; relocations={len(comparison.relocation_ranges)}"
                ),
                "object": rel(object_path),
                "object_symbol": spec.symbol,
                "object_size": str(symbol.size),
                "boundary": boundary,
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(comparison.raw_equal),
                "normalized_equal": "True",
                "unknown_relocations": "",
                "object_sha256": sha256_file(object_path),
                "cache_key": meta["cache_key"],
            }
        )
    return rows


def write_evidence(rows: list[dict[str, str]]) -> None:
    EVIDENCE.parent.mkdir(parents=True, exist_ok=True)
    temporary = EVIDENCE.with_suffix(EVIDENCE.suffix + ".tmp")
    with temporary.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=EVIDENCE_FIELDS, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    os.replace(temporary, EVIDENCE)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--libgcc",
        type=Path,
        default=ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1" / "prefix" / "lib" / "gcc-lib" / "ee" / "3.2.2" / "libgcc.a",
    )
    parser.add_argument(
        "--cxx",
        type=Path,
        default=ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-cxx-stage1" / "prefix" / "bin" / "ee-g++",
    )
    parser.add_argument(
        "--assembler-prefix",
        type=Path,
        default=ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1" / "prefix" / "ee" / "bin",
    )
    parser.add_argument(
        "--gcc-source",
        type=Path,
        default=ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1" / "source" / "gcc-3.2.2",
    )
    args = parser.parse_args()

    reference_path = ROOT / "build" / "SNES_EMU.unpacked.bin"
    required = (reference_path, args.libgcc, args.cxx, args.gcc_source, NEWLIB_ARCHIVE)
    missing = [str(path) for path in required if not path.exists()]
    if missing:
        raise SystemExit("missing runtime matcher input(s):\n" + "\n".join(missing))
    if sha256_file(reference_path) != TARGET_SHA256:
        raise SystemExit("unpacked target SHA-256 mismatch")

    BUILD.mkdir(parents=True, exist_ok=True)
    libgcc_dir = extract_libgcc(args.libgcc.resolve(), args.gcc_source.resolve())
    libsupcxx_dir = compile_libsupcxx(
        args.cxx.resolve(), args.assembler_prefix.resolve(), args.gcc_source.resolve()
    )
    progress, next_address = load_progress()
    rows = make_evidence(
        reference_path.read_bytes(), progress, next_address, libgcc_dir, libsupcxx_dir
    )
    if len(rows) != 50 or len({row["address"] for row in rows}) != 50:
        raise SystemExit("runtime evidence cardinality is not the pinned 50 unique targets")
    write_evidence(rows)
    print(f"runtime strict matches: {len(rows)}")
    print(f"evidence: {rel(EVIDENCE)}")


if __name__ == "__main__":
    main()
