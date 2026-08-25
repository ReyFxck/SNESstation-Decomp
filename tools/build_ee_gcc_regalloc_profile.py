#!/usr/bin/env python3
"""Build an isolated EE GCC cc1plus register-allocation profile.

The historical EE GCC bootstrap keeps its complete host-side build tree.  This
utility reuses those already-built objects to relink a private ``cc1plus``
whose MIPS local-allocation order prefers ``$t5`` immediately before ``$t4``.
The canonical compiler, source tree, and libraries are never modified.

This narrow profile is useful when target evidence proves the older compiler
fingerprint but the portable GCC 3.2.2 rebuild differs only in that tie-break.
Consumers opt in with ``ee-g++ -B<returned-directory>/``.
"""
from __future__ import annotations

import hashlib
import json
import os
import shlex
import shutil
import subprocess
from pathlib import Path


PROFILE_NAME = "mips-local-t5-before-t4"
PROFILE_VERSION = 1

PATCH_CONTEXT = """  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    reg_alloc_order[i] = i;

  if (TARGET_MIPS16)
"""

PATCH_REPLACEMENT = """  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    reg_alloc_order[i] = i;

  /* Target-proven local-allocation tie-break used by one opt-in profile.  */
  reg_alloc_order[12] = 13;
  reg_alloc_order[13] = 12;

  if (TARGET_MIPS16)
"""


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def patch_mips_source(text: str) -> str:
    if text.count(PATCH_CONTEXT) != 1:
        raise RuntimeError("EE GCC mips.c register-allocation context changed")
    return text.replace(PATCH_CONTEXT, PATCH_REPLACEMENT, 1)


def run(command: list[str], *, cwd: Path) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        command,
        cwd=cwd,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    if result.returncode:
        rendered = " ".join(shlex.quote(part) for part in command)
        raise RuntimeError(
            f"command failed ({result.returncode}): {rendered}\n"
            f"{result.stdout[-6000:]}"
        )
    return result


def find_cc1plus_link_command(make_output: str) -> list[str]:
    logical = make_output.replace("\\\n", " ")
    candidates: list[list[str]] = []
    for line in logical.splitlines():
        if " -o cc1plus " not in f" {line.strip()} " or "cp/call.o" not in line:
            continue
        tokens = shlex.split(line)
        if "libbackend.a" in tokens:
            candidates.append(tokens)
    if len(candidates) != 1:
        raise RuntimeError(
            "could not isolate the EE GCC cc1plus link command "
            f"(found {len(candidates)})"
        )
    return candidates[0]


def _tool_command(variable: str, fallback: str) -> list[str]:
    value = os.environ.get(variable, fallback)
    command = shlex.split(value)
    if not command:
        raise RuntimeError(f"empty {variable} command")
    return command


def build_profile(cxx: Path, output: Path | None = None) -> Path:
    """Return a directory containing the isolated profile's ``cc1plus``."""
    cxx = cxx.expanduser().resolve()
    if not cxx.is_file():
        raise RuntimeError(f"missing EE C++ driver: {cxx}")
    work = cxx.parents[2]
    source_gcc = work / "source" / "gcc-3.2.2" / "gcc"
    build_gcc = work / "build" / "gcc-ee-stage1" / "gcc"
    canonical_cc1plus = (
        work / "prefix" / "lib" / "gcc-lib" / "ee" / "3.2.2" / "cc1plus"
    )
    mips_source = source_gcc / "config" / "mips" / "mips.c"
    libbackend = build_gcc / "libbackend.a"
    makefile = build_gcc / "Makefile"
    required = (canonical_cc1plus, mips_source, libbackend, makefile)
    missing = [str(path) for path in required if not path.is_file()]
    if missing:
        raise RuntimeError(
            "EE GCC C++ bootstrap build tree is incomplete; missing: "
            + ", ".join(missing)
        )

    profile = (output or work / "profiles" / PROFILE_NAME).resolve()
    profile.mkdir(parents=True, exist_ok=True)
    patched_text = patch_mips_source(mips_source.read_text(encoding="latin-1"))
    payload = {
        "profile": PROFILE_NAME,
        "version": PROFILE_VERSION,
        "canonical_cc1plus_sha256": sha256_file(canonical_cc1plus),
        "mips_source_sha256": sha256_file(mips_source),
        "patched_mips_sha256": hashlib.sha256(
            patched_text.encode("latin-1")
        ).hexdigest(),
        "libbackend_sha256": sha256_file(libbackend),
        "makefile_sha256": sha256_file(makefile),
    }
    metadata = profile / "profile.json"
    custom_cc1plus = profile / "cc1plus"
    if metadata.is_file() and custom_cc1plus.is_file():
        try:
            cached = json.loads(metadata.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError):
            cached = None
        if cached == payload:
            return profile

    patched_source = profile / "mips-regalloc.c"
    patched_source.write_text(patched_text, encoding="latin-1")
    custom_mips = profile / "mips.o"
    include = [
        build_gcc,
        build_gcc,
        source_gcc,
        source_gcc,
        source_gcc / "config",
        source_gcc.parent / "include",
    ]
    compile_command = [
        *_tool_command("CC", "gcc"),
        "-c",
        "-DIN_GCC",
        "-DCROSS_COMPILE",
        "-g",
        "-W",
        "-Wall",
        "-Wwrite-strings",
        "-Wstrict-prototypes",
        "-Wmissing-prototypes",
        "-Wtraditional",
        "-pedantic",
        "-Wno-long-long",
        "-DHAVE_CONFIG_H",
    ]
    for path in include:
        compile_command.extend(("-I", str(path)))
    compile_command.extend((str(patched_source), "-o", str(custom_mips)))
    run(compile_command, cwd=build_gcc)

    custom_backend = profile / "libbackend.a"
    shutil.copy2(libbackend, custom_backend)
    run(
        [*_tool_command("AR", "ar"), "r", str(custom_backend), str(custom_mips)],
        cwd=build_gcc,
    )
    run([*_tool_command("RANLIB", "ranlib"), str(custom_backend)], cwd=build_gcc)

    dry_run = run(
        [
            *_tool_command("MAKE", "make"),
            "-n",
            "-W",
            "libbackend.a",
            "cc1plus",
        ],
        cwd=build_gcc,
    )
    link_command = find_cc1plus_link_command(dry_run.stdout)
    for index, token in enumerate(link_command):
        if token == "libbackend.a":
            link_command[index] = str(custom_backend)
        elif token == "cc1plus" and index and link_command[index - 1] == "-o":
            link_command[index] = str(custom_cc1plus)
    run(link_command, cwd=build_gcc)
    custom_cc1plus.chmod(custom_cc1plus.stat().st_mode | 0o111)
    if not custom_cc1plus.is_file():
        raise RuntimeError("profile cc1plus link did not produce an executable")
    metadata.write_text(
        json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8"
    )
    return profile


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cxx", required=True, type=Path)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    print(build_profile(args.cxx, args.output))
