#!/usr/bin/env python3
"""Rebuild and integrate the easiest remaining historical C++ tail records.

This Stage-3J gate covers one Snes9x TILE.CPP unwind container and the selected
GCC 3.2.2 libsupc++ unwind/LSDA containers.  Source bytes are rebuilt; the
private reference is consulted only for R_MIPS_32 relocation results.  The
output remains a diagnostic ELF until the complete unpacked image matches.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import shlex
import struct
import subprocess
import sys
from pathlib import Path
from typing import Sequence

import data_backing
import historical_data
import historical_tail_data as prior
import link_layout_probe
import startup_integration as startup
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/runtime_tail_data.json"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_PRIOR_BUILD = ROOT / "build/historical-tail-data"
DEFAULT_BUILD = ROOT / "build/runtime-tail-data"

FORMAT = "snesstation-stage3j-runtime-tail-data"
SCHEMA = 1
TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
SOURCE_HASHES = {
    "TILE.CPP": "84604306482e4aa14247bf878bfadb767b550a16c0e1c650d001dabc1e052a5e",
    "eh_alloc.cc": "fe91e5c2e841c1f5316cceb7a9546af532484a9f4452e71ff1d4b15549b49b10",
    "eh_catch.cc": "2c6c6ebf4d285e5c66d94348ff91ffc92f8e591705adaa602f2ab0813cb2979c",
    "eh_personality.cc": "28af68c7005617874114e6e66a66b85e445d17220389a06bcdf486e52b9757b7",
    "eh_terminate.cc": "a48811103b3b46f814be8797450a003b643f317f527112d40fc0a702eb3ae677",
    "eh_throw.cc": "988d37dcc816a641dd4f63d99f1d535a6e0e92c6ffdd33148d7800b140f98aa5",
    "new_handler.cc": "071b08071835a0d7f0661c976147381f03614edeb141fc68e2fb21fab9e95121",
    "new_op.cc": "1361436a8fc1b0bdb4dd180021b9edc12294194ff06202ec084a1d13a57ae533",
    "new_opv.cc": "07fc5dc109d4cac91b9938d0ac301358f4ed47ba39172d9727757c9444c7fcb1",
    "tinfo.cc": "28d8473f75586ba33158f44b2c6c2e014f23a4f18604b9f85f7d76e7b95024dc",
}


def S(name: str, obj: str, source_section: str, address: int, size: int,
      relocations: int, raw_sha256: str, linked_sha256: str) -> dict:
    return {
        "name": name, "object": obj, "source_section": source_section,
        "section": f".data.stage3j.{name}", "address": address, "size": size,
        "relocations": relocations, "raw_sha256": raw_sha256,
        "linked_sha256": linked_sha256,
    }


SECTIONS = (
    S("tile_unwind", "TILE.CPP", ".data", 0x004238A8, 0x8AC, 31,
      "5441b6b8950708f0ca28fdf8e497256ee78a57d6d91d5536d9cd3355a4a4fcb4",
      "31b75506d0986cd28f6071053818892111da150303725629ec1e4a958f6bfca3"),
    S("eh_personality_unwind", "eh_personality.cc", ".data", 0x00426528, 0x180, 8,
      "6b5016a523f51b5792894942acf337a7e6a1d0630cc02f8599234ebef5a66e2a",
      "fcd2e2513bbf504f6d2d15a5d1ff641bdbaeb0a4757f7f1d0a3fc47daa1f8e39"),
    S("eh_terminate_unwind", "eh_terminate.cc", ".data", 0x004266A8, 0xB8, 8,
      "bd0db972f4986aad0c143b45c3ee5eeae942e4b7502d87e65e30409b6170f9e5",
      "16bae976676c8721c9d8e23379a43ad1dadbb33ee570149ddebefddcd12b2422"),
    S("eh_throw_unwind", "eh_throw.cc", ".data", 0x00426760, 0x90, 4,
      "bc2b7b7093f2a8786942defacc0abbb71fe854f465350ca5fc620080041b828f",
      "bd436d77c1900b1602bcb9a2ec8865dd80ad4c7c6f4b15f67aabd01351ab9c3c"),
    S("new_op_unwind", "new_op.cc", ".data", 0x004267F0, 0x54, 3,
      "0ec276adca305ceebdf356320e5017e56183bef31fff38ead78b2de472f19ba0",
      "cb37cee2544a8f7cfc471eae1cfa57125cf142cd041e36f6362dbc8c3496832a"),
    S("new_opv_unwind", "new_opv.cc", ".data", 0x00426844, 0x44, 3,
      "c10b0d044cea2642f77d6b482f8bc3eaf19bb87185f018949ac59f0a8b53257b",
      "51cc24d3ac2ae8df1d358c5315d0a75740da8c9a8adfefdbba48133d095ec0e4"),
    S("tinfo_unwind", "tinfo.cc", ".data", 0x00426888, 0x208, 10,
      "3e7aa1f220fac5e1984ea1f968bb38077917e45855411870c70f6ebd9b5f3aa4",
      "ee2e6bf4fa23da363fe1a69d9d26c3e05ec51d94203318f15c9b53bb80544616"),
    S("eh_alloc_unwind", "eh_alloc.cc", ".data", 0x00426A90, 0x48, 2,
      "6bd1c073882e79c5121b16c2f7473d7280f130650063fbff3e4e15dfcff3212d",
      "08d9be7ed1246f33edadf83f70a8d600117116b3cf8c7ecc26b77896c1054fd0"),
    S("eh_catch_unwind", "eh_catch.cc", ".data", 0x00426AD8, 0x44, 2,
      "a938e2df35098e29aae1262b629a92d56999ccb902b6ba2f8cc5a3ebc0223fea",
      "0118ab0d3655c92e88dbc9867b1ef7cf38ffe6dcf75b55accae7610dc5518f3a"),
    S("new_handler_slot", "new_handler.cc", ".data", 0x00426B1C, 4, 0,
      "df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119",
      "df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119"),
    S("eh_personality_lsda", "eh_personality.cc", ".gcc_except_table", 0x00426BA0, 0x28, 0,
      "e96ca0d2b8c19a82950d277f1e2c97815a2954103a2b61a12f96e04ee9c2c2a1",
      "e96ca0d2b8c19a82950d277f1e2c97815a2954103a2b61a12f96e04ee9c2c2a1"),
    S("eh_terminate_lsda", "eh_terminate.cc", ".gcc_except_table", 0x00426BC8, 0x14, 0,
      "05fbd5477ddc47e529c5540228583b7206ecb3d95f8a655deb745348c0cdf7cc",
      "05fbd5477ddc47e529c5540228583b7206ecb3d95f8a655deb745348c0cdf7cc"),
    S("new_op_lsda", "new_op.cc", ".gcc_except_table", 0x00426BDC, 0x24, 1,
      "660ea9536aecc903c45483200962b05b751dfd47a89e2ce460132b35eda7e26f",
      "a6fd098bd3e6d94d070a09fd3f3ee50910a86b697a848a7b2a9003f2a7fa01e4"),
    S("new_opv_lsda", "new_opv.cc", ".gcc_except_table", 0x00426C00, 0x18, 1,
      "535bdc59ec3d3480899db820dd7ac6dc7098a7a24863b336fc4e9e79c2685f05",
      "5e8b6a1878bf463f902ba408e33265a0e62ec04da91e0dc58a62ec8c13efe9ac"),
)

EXPECTED = {
    "source_sections": 14,
    "source_bytes": 3868,
    "source_relocations": 73,
    "unwind_fdes": 56,
    "exact_chunks": 16,
    "mismatching_chunks": 35,
    "differing_bytes": 1857419,
    "chunk50_differing_bytes": 3173,
    "first_differing_address": 0x00100114,
    "target_sha256": TARGET_SHA256,
    "prior_differing_bytes": 1859772,
}


class RuntimeTailError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise RuntimeTailError(message)


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def run(command: Sequence[str | Path], cwd: Path = ROOT) -> str:
    result = subprocess.run([str(x) for x in command], cwd=cwd, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        fail(f"command failed: {shlex.join(map(str, command))}\n{result.stdout[-5000:]}")
    return result.stdout.strip()


def claims() -> dict[str, bool]:
    return {
        "historical_source_sections_rebuilt": True,
        "private_oracle_limited_to_r_mips_32_results": True,
        "tile_unwind_container_exact": True,
        "selected_libsupcxx_unwind_and_lsda_exact": True,
        "private_target_bytes_stored": False,
        "window_50_exact": False,
        "replacement_elf": False,
        "unpacked_hash_matched": False,
        "packed_hash_matched": False,
    }


def section_document() -> list[dict]:
    return [dict(row) for row in SECTIONS]


def frozen_document(result: dict) -> dict:
    return {
        "format": FORMAT, "schema_version": SCHEMA,
        "prior_manifest_sha256": digest(prior.DEFAULT_MANIFEST.read_bytes()),
        "source_hashes": SOURCE_HASHES, "sections": section_document(),
        "result": result, "claims": claims(),
    }


def validate(args: argparse.Namespace) -> dict:
    prior.validate(prior.parse_args(["validate"]))
    try:
        document = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read runtime-tail manifest: {exc}")
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("runtime-tail manifest identity drift")
    if document.get("prior_manifest_sha256") != digest(prior.DEFAULT_MANIFEST.read_bytes()):
        fail("runtime-tail prior checkpoint drift")
    if document.get("source_hashes") != SOURCE_HASHES or document.get("sections") != section_document():
        fail("runtime-tail source contract drift")
    for key, value in EXPECTED.items():
        if document.get("result", {}).get(key) != value:
            fail(f"runtime-tail frozen metric drift: {key}")
    if document.get("claims") != claims():
        fail("runtime-tail claim boundary drift")
    return document


def link_force(source: Path, destination: Path) -> None:
    destination.parent.mkdir(parents=True, exist_ok=True)
    if destination.exists() or destination.is_symlink():
        destination.unlink()
    destination.symlink_to(source.resolve())


def build_tile(compiler: Path, build_dir: Path) -> Path:
    old = prior.v52.BUILD
    try:
        prior.v52.BUILD = build_dir / "v52-rebuild"
        source_root, _original, layout = prior.v52.prepare_snes_layout()
        prior.v52.patch_sources(layout)
        prior.v52.replace_once(layout / "spc7110.h", *prior.TIME32_PATCH, "32-bit target time_t layout")
        compat = build_dir / "compat-v52"
        prior.v52.write_compat_headers(compat)
    finally:
        prior.v52.BUILD = old
    source = layout / "TILE.CPP"
    if digest(source.read_bytes()) != SOURCE_HASHES[source.name]:
        fail("TILE.CPP source hash drift")
    newlib = prior.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    gcc_include = Path(run([compiler, "-print-file-name=include"])).resolve()
    flags = [*prior.v47.COMMON_FLAGS, "-Os", *prior.v47.SNES_DEFINES, "-DZLIB", "-nostdinc",
             *prior.v47.include_args([compat, newlib, layout, layout / "unzip", source_root / "zlib", gcc_include]),
             "-x", "c++"]
    output = build_dir / "source-objects/TILE.o"
    prior.compile_one(compiler, flags, source, output)
    return output


def prepare_cxx_headers(gcc_source: Path, build_dir: Path) -> tuple[Path, Path]:
    stage = build_dir / "cxx-include"
    bits = stage / "bits"
    bits.mkdir(parents=True, exist_ok=True)
    libstd = gcc_source / "libstdc++-v3"
    sup = libstd / "libsupc++"
    for header in (libstd / "include/c_std").glob("std_*.h"):
        link_force(header, stage / header.name.removeprefix("std_").removesuffix(".h"))
    for name in ("new", "exception", "typeinfo", "cxxabi.h", "exception_defines.h", "unwind-cxx.h"):
        link_force(sup / name, stage / name)
    for header in (libstd / "include/bits").iterdir():
        if header.is_file():
            link_force(header, bits / header.name)
    link_force(libstd / "config/os/generic/bits/os_defines.h", bits / "os_defines.h")
    link_force(gcc_source / "gcc/gthr-single.h", bits / "gthr.h")
    link_force(gcc_source / "gcc/unwind.h", stage / "unwind.h")
    link_force(gcc_source / "gcc/unwind-pe.h", stage / "unwind-pe.h")
    (bits / "c++config.h").write_text(
        "#ifndef _CPP_CPPCONFIG\n#define _CPP_CPPCONFIG 1\n#include <bits/os_defines.h>\n"
        "#define __GLIBCPP__ 20030205\n#define _GLIBCPP_NO_TEMPLATE_EXPORT 1\n"
        "#define _GLIBCPP_FULLY_COMPLIANT_HEADERS 1\n#define _GLIBCPP_RESOLVE_LIB_DEFECTS 1\n"
        "#define _GLIBCPP_AT_AT \"@@\"\n#define __GXX_MERGED_TYPEINFO_NAMES 0\n#endif\n",
        encoding="utf-8",
    )
    newlib = prior.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    return stage, newlib


def build_libsupcxx(compiler: Path, build_dir: Path) -> dict[str, Path]:
    work = compiler.resolve().parents[2]
    gcc_source = work / "source/gcc-3.2.2"
    if not (gcc_source / "libstdc++-v3/libsupc++").is_dir():
        fail(f"cannot find GCC 3.2.2 source beside compiler: {gcc_source}")
    stage, newlib = prepare_cxx_headers(gcc_source, build_dir)
    sup = gcc_source / "libstdc++-v3/libsupc++"
    flags = ["-nostdinc++", f"-I{stage}", f"-I{sup}", f"-I{newlib}", "-G0", "-O2", "-EL", "-pipe",
             "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mhard-float",
             "-mno-abicalls", "-march=r5900", "-mtune=r5900"]
    outputs = {}
    for name in sorted(set(SOURCE_HASHES) - {"TILE.CPP"}):
        source = sup / name
        if digest(source.read_bytes()) != SOURCE_HASHES[name]:
            fail(f"libsupc++ source hash drift: {name}")
        output = build_dir / "source-objects" / f"{source.stem}.o"
        prior.compile_one(compiler, [f"-B{compiler.parent}{os.sep}", *flags], source, output)
        outputs[name] = output
    return outputs


def source_section(elf: ELFFile, name: str):
    found = [item for item in elf.sections if item.name == name]
    if len(found) != 1:
        fail(f"missing/duplicate source section: {name}")
    return found[0]


def rebuild_payloads(objects: dict[str, Path], reference: bytes, build_dir: Path) -> list[dict]:
    rows = []
    for spec in SECTIONS:
        elf = ELFFile(objects[spec["object"]])
        item = source_section(elf, spec["source_section"])
        raw = prior.section_bytes(elf, item)
        if len(raw) != spec["size"] or digest(raw) != spec["raw_sha256"]:
            fail(f"source object section drift: {spec['name']}")
        relocs = historical_data.relocations(elf, item.index)
        if len(relocs) != spec["relocations"] or any(kind != 2 for _off, kind, _name in relocs):
            fail(f"R_MIPS_32 roster drift: {spec['name']}")
        target = reference[spec["address"] - TARGET_BASE:spec["address"] - TARGET_BASE + spec["size"]]
        patched = bytearray(raw)
        mask = bytearray(len(raw))
        for offset, _kind, _symbol in relocs:
            mask[offset:offset + 4] = b"\1" * 4
            patched[offset:offset + 4] = target[offset:offset + 4]
        if any(a != b for a, b, marked in zip(raw, target, mask) if not marked):
            fail(f"source range differs outside relocations: {spec['name']}")
        if bytes(patched) != target or digest(target) != spec["linked_sha256"]:
            fail(f"linked source range drift: {spec['name']}")
        payload = build_dir / "payloads" / f"{spec['name']}.bin"
        payload.parent.mkdir(parents=True, exist_ok=True)
        payload.write_bytes(patched)
        rows.append({**spec, "payload": payload})
    return rows


def render_source(rows: Sequence[dict]) -> str:
    lines = ["/* Generated from public source plus verified R_MIPS_32 results. */"]
    for row in rows:
        path = str(row["payload"]).replace("\\", "\\\\").replace('"', '\\"')
        lines += [f'.section {row["section"]},"aw",@progbits', ".align 2", f'.incbin "{path}"']
    return "\n".join(lines) + "\n"


def probe(args: argparse.Namespace) -> dict:
    prior_doc = prior.validate(prior.parse_args(["validate"]))
    sections, layout = startup.load_inputs(startup.parse_args(["validate", "--sections", str(args.sections), "--layout", str(args.layout)]))
    reference = args.reference.read_bytes()
    if digest(reference) != TARGET_SHA256:
        fail("private unpacked reference SHA-256 drift")
    compiler = resolve_tool(args.compiler)
    if run([compiler, "-dumpversion"]) != "3.2.2" or run([compiler, "-dumpmachine"]) != "ee":
        fail("runtime-tail integration requires EE GCC 3.2.2 C++")
    linker = resolve_tool(args.ld) if args.ld else compiler.with_name("ee-ld")
    objcopy = resolve_tool(args.objcopy) if args.objcopy else compiler.with_name("ee-objcopy")
    required = [args.input, args.startup_object, args.prior_build / "frontend-eh-frames.o",
                args.prior_build / "providers.o", args.prior_build / "semantic-cfi.o"]
    if not all(path.is_file() for path in required):
        fail("missing V105 build dependency; run make historical-tail-data first")
    data_backing.check_sections(args.input, sections)
    args.build_dir.mkdir(parents=True, exist_ok=True)
    objects = {"TILE.CPP": build_tile(compiler, args.build_dir)}
    objects.update(build_libsupcxx(compiler, args.build_dir))
    rows = rebuild_payloads(objects, reference, args.build_dir)
    source = args.build_dir / "runtime-tail.S"
    obj = args.build_dir / "runtime-tail.o"
    source.write_text(render_source(rows), encoding="utf-8")
    prior.compile_one(compiler, ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"), source, obj)

    prior_providers = [{"address": row["target_address"], "section": f".data.stage3i.source.{row['name']}"}
                       for row in prior.PROVIDERS]
    aliases = prior.absorbed_symbol_aliases(args.input, sections)
    script = prior.render_linker_script(sections, prior_providers, aliases)
    marker = "  .bss.stage3g.crt0 0x00426e80"
    placements = "\n".join(f"  {row['section']} 0x{row['address']:08x} : {{ KEEP(*({row['section']})) }}" for row in rows)
    script = script.replace(marker, placements + "\n" + marker, 1)
    linker_script = args.build_dir / "runtime-tail.ld"
    linker_script.write_text(script, encoding="utf-8")
    output = args.build_dir / "stage3j-runtime-tail-integrated.elf"
    run([linker, "-EL", "-T", linker_script, "-o", output, args.startup_object, args.input,
         args.prior_build / "frontend-eh-frames.o", args.prior_build / "providers.o",
         args.prior_build / "semantic-cfi.o", obj])
    elf = ELFFile(output)
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 2):
        fail("runtime-tail output is not MIPS ELF32 ET_EXEC")
    startup.verify_symbols(elf)
    prior.verify_fixed_output(elf, sections)
    for row in rows:
        item = prior.section(elf, row["section"])
        target = reference[row["address"] - TARGET_BASE:row["address"] - TARGET_BASE + row["size"]]
        if item.address != row["address"] or prior.section_bytes(elf, item) != target:
            fail(f"linked runtime-tail range differs: {row['name']}")
    raw_path = args.build_dir / "stage3j-runtime-tail-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3j-runtime-tail-integrated.padded.bin"
    run([objcopy, "-O", "binary", output, raw_path])
    raw = raw_path.read_bytes()
    if len(raw) > len(reference):
        fail("runtime-tail image exceeds target initialized size")
    padded = raw + bytes(len(reference) - len(raw))
    padded_path.write_bytes(padded)
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [i for i, (a, b) in enumerate(zip(padded, reference)) if a != b]
    if not differences:
        fail("unexpected complete match; Stage 3J is not a replacement-ELF claim")
    return {
        "source_sections": len(rows), "source_bytes": sum(row["size"] for row in rows),
        "source_relocations": sum(row["relocations"] for row in rows), "unwind_fdes": 56,
        "exact_chunks": len(exact), "mismatching_chunks": len(different),
        "exact_chunk_indices": exact, "mismatching_chunk_indices": different,
        "differing_bytes": len(differences),
        "chunk50_differing_bytes": sum(a != b for a, b in zip(padded[50 * 65536:], reference[50 * 65536:])),
        "first_differing_address": TARGET_BASE + differences[0],
        "integrated_padded_sha256": digest(padded), "target_sha256": digest(reference),
        "prior_differing_bytes": prior_doc["result"]["differing_bytes"],
    }


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "probe", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--sections", type=Path, default=DEFAULT_SECTIONS)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--startup-object", type=Path, default=DEFAULT_STARTUP)
    parser.add_argument("--prior-build", type=Path, default=DEFAULT_PRIOR_BUILD)
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
                args.manifest.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n", encoding="utf-8")
            elif validate(args) != document:
                fail("private runtime-tail result differs from frozen manifest")
        result = document["result"]
        print(f"verified runtime tail data: sections={result['source_sections']} bytes={result['source_bytes']} "
              f"relocations={result['source_relocations']} fdes={result['unwind_fdes']}")
        print(f"whole-image chunks={result['exact_chunks']}/51 remaining={result['mismatching_chunks']} "
              f"differing_bytes={result['differing_bytes']} chunk50_differences={result['chunk50_differing_bytes']}; "
              "replacement ELF: not yet")
        return 0
    except (RuntimeTailError, prior.HistoricalTailError, OSError, ValueError, KeyError, RuntimeError) as exc:
        print(f"runtime tail data: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
