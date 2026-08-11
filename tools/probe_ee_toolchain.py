#!/usr/bin/env python3
"""Verify that an EE compiler satisfies the first historical matching contract."""
from __future__ import annotations

import argparse
import os
import platform
import shutil
import struct
import subprocess
import tempfile
from dataclasses import dataclass
from pathlib import Path


EXPECTED_GCC = "3.2.2"
EXPECTED_TARGETS = {"ee", "mips64r5900el-scei-elf"}
SMOKE_FLAGS = (
    "-G0",
    "-O2",
    "-EL",
    "-pipe",
    "-Wall",
    "-Werror",
    "-Wa,-al",
    "-fomit-frame-pointer",
    "-fstrict-aliasing",
    "-fno-common",
    "-ffreestanding",
    "-fno-builtin",
    "-fshort-double",
    "-mlong64",
    "-mhard-float",
    "-mno-abicalls",
    "-march=r5900",
    "-mtune=r5900",
)
SMOKE_SOURCE = """\
extern int snesstation_probe_external;

int snesstation_toolchain_probe(int value)
{
    return value + snesstation_probe_external;
}
"""


@dataclass(frozen=True)
class ELFIdentity:
    elf_class: int
    data_encoding: int
    object_type: int
    machine: int
    flags: int

    @property
    def is_ee_relocatable(self) -> bool:
        # ELFCLASS32, ELFDATA2LSB, ET_REL and EM_MIPS.
        return (
            self.elf_class == 1
            and self.data_encoding == 1
            and self.object_type == 1
            and self.machine == 8
        )


def parse_elf_identity(data: bytes) -> ELFIdentity:
    if len(data) < 52 or data[:4] != b"\x7fELF":
        raise ValueError("compiler output is not a complete ELF32 object")
    elf_class = data[4]
    data_encoding = data[5]
    if elf_class != 1:
        raise ValueError(f"expected ELF32 output, got ELF class {elf_class}")
    if data_encoding == 1:
        byte_order = "<"
    elif data_encoding == 2:
        byte_order = ">"
    else:
        raise ValueError(f"unknown ELF data encoding {data_encoding}")
    object_type, machine = struct.unpack_from(f"{byte_order}HH", data, 16)
    flags = struct.unpack_from(f"{byte_order}I", data, 36)[0]
    return ELFIdentity(elf_class, data_encoding, object_type, machine, flags)


def base_version_matches(version: str) -> bool:
    return version.strip() == EXPECTED_GCC


def target_matches(target: str) -> bool:
    normalized = target.strip().lower()
    return normalized in EXPECTED_TARGETS or (
        "r5900" in normalized and normalized.endswith(("-elf", "-scei"))
    )


def run_capture(command: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )


def resolve_compiler(value: str) -> str | None:
    if os.sep in value:
        path = Path(value).expanduser()
        return str(path.resolve()) if path.is_file() and os.access(path, os.X_OK) else None
    return shutil.which(value)


def first_line(text: str) -> str:
    lines = text.strip().splitlines()
    return lines[0] if lines else "<no output>"


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--compiler",
        default=os.environ.get("EE_CC", "ee-gcc"),
        help="ee-gcc executable or absolute path (default: EE_CC or ee-gcc)",
    )
    args = parser.parse_args()

    host = platform.machine() or "unknown"
    print(f"host architecture: {host}")
    compiler = resolve_compiler(args.compiler)
    if compiler is None:
        print(f"compiler: MISSING ({args.compiler})")
        if host.lower() in {"aarch64", "arm64"}:
            print("note: an old i686/Cygwin ee-gcc binary cannot run natively on ARM64")
        print("result: BLOCKED -- provide a native historical compiler build with EE_CC=/path")
        raise SystemExit(2)

    print(f"compiler: {compiler}")
    try:
        version_result = run_capture([compiler, "--version"])
        dumpversion_result = run_capture([compiler, "-dumpversion"])
        machine_result = run_capture([compiler, "-dumpmachine"])
    except OSError as exc:
        print(f"result: BLOCKED -- compiler could not execute: {exc}")
        raise SystemExit(2) from exc

    if version_result.returncode != 0:
        print(f"result: FAIL -- --version exited {version_result.returncode}")
        print(first_line(version_result.stderr))
        raise SystemExit(1)
    if dumpversion_result.returncode != 0 or machine_result.returncode != 0:
        print("result: FAIL -- compiler does not implement the GCC identity probes")
        raise SystemExit(1)

    version_banner = first_line(version_result.stdout or version_result.stderr)
    dumpversion = dumpversion_result.stdout.strip()
    machine = machine_result.stdout.strip()
    print(f"version banner: {version_banner}")
    print(f"base version: {dumpversion}")
    print(f"target: {machine}")

    with tempfile.TemporaryDirectory(prefix="snesstation-ee-probe-") as directory:
        root = Path(directory)
        source = root / "probe.c"
        output = root / "probe.o"
        source.write_text(SMOKE_SOURCE, encoding="utf-8")
        compile_result = run_capture(
            [compiler, *SMOKE_FLAGS, "-c", str(source), "-o", str(output)]
        )
        if compile_result.returncode != 0:
            print(f"smoke compile: FAIL (exit {compile_result.returncode})")
            diagnostic = (compile_result.stderr or compile_result.stdout).strip()
            if diagnostic:
                print(diagnostic[-4000:])
            raise SystemExit(1)
        try:
            identity = parse_elf_identity(output.read_bytes())
        except (OSError, ValueError) as exc:
            print(f"smoke object: FAIL -- {exc}")
            raise SystemExit(1) from exc

    print(
        "smoke object: "
        f"ELF32 data={identity.data_encoding} type={identity.object_type} "
        f"machine={identity.machine} flags=0x{identity.flags:08x}"
    )

    failures: list[str] = []
    if not base_version_matches(dumpversion):
        failures.append(f"base GCC is not exactly {EXPECTED_GCC}")
    if not target_matches(machine):
        failures.append("target is not a recognized little-endian R5900/EE target")
    if not identity.is_ee_relocatable:
        failures.append("smoke object is not ELF32 little-endian MIPS ET_REL")
    if failures:
        for failure in failures:
            print(f"contract: FAIL -- {failure}")
        raise SystemExit(1)

    print("contract: PASS -- GCC 3.2.2 EE base family and flags are usable")
    print("patch level: UNPROVEN -- the external '-b1' label is not in GCC -dumpversion")
    print("next: run make match-get-tree with this EE_CC path")


if __name__ == "__main__":
    main()
