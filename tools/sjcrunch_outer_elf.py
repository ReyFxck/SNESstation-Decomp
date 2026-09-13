#!/usr/bin/env python3
"""Rebuild the exact SJCRUNCH2 loader and complete packed ELF.

The historical SjCRUNCH 2.1 archive is fetched from a public mirror and
verified before its loader objects are used.  Only hashes and provenance are
committed; the generated ELF and the optional private reference remain under
ignored paths.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import subprocess
import urllib.request
import zipfile
from pathlib import Path, PurePosixPath
from typing import Sequence


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "sjcrunch_outer_elf.json"
DEFAULT_ARCHIVE = ROOT / "build" / "upstream" / "sjcrunch-2.1.zip"
DEFAULT_CONTAINER = ROOT / "build" / "sjcrunch-packing" / "container.bin"
DEFAULT_BUILD_DIR = ROOT / "build" / "sjcrunch-outer-elf"
DEFAULT_OUTPUT = ROOT / "build" / "SNES_EMU.rebuilt.ELF"
DEFAULT_REFERENCE = ROOT / "original" / "SNES_EMU.ELF"
DEFAULT_COMPILER = ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1" / "prefix" / "bin" / "ee-gcc"
FORMAT = "snesstation-sjcrunch2-outer-elf"
SCHEMA_VERSION = 1
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")


class OuterElfError(ValueError):
    """A public provenance or exact-output invariant failed."""


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    hasher = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            hasher.update(block)
    return hasher.hexdigest()


def require_int(mapping: dict[str, object], key: str, minimum: int = 0) -> int:
    value = mapping.get(key)
    if not isinstance(value, int) or isinstance(value, bool) or value < minimum:
        raise OuterElfError(f"manifest field {key!r} must be an integer >= {minimum}")
    return value


def require_sha256(mapping: dict[str, object], key: str) -> str:
    value = mapping.get(key)
    if not isinstance(value, str) or not SHA256_RE.fullmatch(value):
        raise OuterElfError(f"manifest field {key!r} is not a lowercase SHA-256")
    return value


def safe_member_path(value: object) -> str:
    if not isinstance(value, str):
        raise OuterElfError("archive member path must be text")
    path = PurePosixPath(value)
    if path.is_absolute() or not path.parts or ".." in path.parts:
        raise OuterElfError(f"unsafe archive member path: {value!r}")
    return path.as_posix()


def load_manifest(path: Path = DEFAULT_MANIFEST) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise OuterElfError(f"cannot read outer-ELF manifest {path}: {exc}") from exc
    if not isinstance(value, dict):
        raise OuterElfError("outer-ELF manifest root must be an object")
    validate_manifest(value)
    return value


def validate_manifest(manifest: dict[str, object]) -> None:
    if manifest.get("schema_version") != SCHEMA_VERSION:
        raise OuterElfError("unsupported outer-ELF manifest schema")
    if manifest.get("format") != FORMAT:
        raise OuterElfError("unexpected outer-ELF manifest format")

    package = manifest.get("package")
    archive = manifest.get("archive")
    build = manifest.get("build")
    linked_inputs = manifest.get("linked_inputs")
    source_evidence = manifest.get("source_evidence")
    if not isinstance(package, dict) or package.get("name") != "SjCRUNCH":
        raise OuterElfError("historical package identity is malformed")
    if package.get("version") != "2.1" or package.get("author") != "Sjeep":
        raise OuterElfError("historical package must remain SjCRUNCH 2.1 by Sjeep")
    if not isinstance(archive, dict) or not isinstance(build, dict):
        raise OuterElfError("archive/build metadata is malformed")
    if not isinstance(linked_inputs, list) or not isinstance(source_evidence, list):
        raise OuterElfError("archive member metadata is malformed")

    url = archive.get("url")
    if not isinstance(url, str) or not url.startswith("https://"):
        raise OuterElfError("archive URL must use HTTPS")
    require_int(archive, "size", 1)
    require_sha256(archive, "sha256")
    if require_int(build, "start_address", 1) != 0x01B00000:
        raise OuterElfError("historical loader start address must be 0x01b00000")
    if require_int(build, "entry_address", 1) != 0x01B00008:
        raise OuterElfError("historical loader entry address must be 0x01b00008")
    require_int(build, "container_size", 1)
    require_sha256(build, "container_sha256")
    require_int(build, "output_size", 1)
    require_sha256(build, "output_sha256")

    expected_linked = {
        "script/crunch_crt0.o",
        "script/libsjcrunch.a",
        "script/linkfile",
    }
    seen: set[str] = set()
    for group in (linked_inputs, source_evidence):
        for row in group:
            if not isinstance(row, dict):
                raise OuterElfError("archive member row is malformed")
            member = safe_member_path(row.get("path"))
            if member in seen:
                raise OuterElfError(f"duplicate archive member row: {member}")
            seen.add(member)
            require_int(row, "size", 1)
            require_sha256(row, "sha256")
    if {safe_member_path(row.get("path")) for row in linked_inputs} != expected_linked:
        raise OuterElfError("linked-input set does not match the historical wrapper")

    rendered = json.dumps(manifest, sort_keys=True)
    forbidden = ("packed_bytes", "reference_path", "data_base64", "private_payload")
    if any(field in rendered for field in forbidden):
        raise OuterElfError("outer-ELF manifest contains a forbidden private field")


def verify_archive(path: Path, manifest: dict[str, object]) -> None:
    archive_meta = manifest["archive"]
    assert isinstance(archive_meta, dict)
    try:
        actual_size = path.stat().st_size
        actual_hash = sha256_file(path)
    except OSError as exc:
        raise OuterElfError(f"cannot read SjCRUNCH archive {path}: {exc}") from exc
    if actual_size != int(archive_meta["size"]):
        raise OuterElfError(
            f"SjCRUNCH archive size mismatch: expected {archive_meta['size']}, got {actual_size}"
        )
    if actual_hash != str(archive_meta["sha256"]):
        raise OuterElfError(
            f"SjCRUNCH archive SHA-256 mismatch: expected {archive_meta['sha256']}, got {actual_hash}"
        )

    rows = [*manifest["linked_inputs"], *manifest["source_evidence"]]
    try:
        with zipfile.ZipFile(path) as archive:
            for row in rows:
                assert isinstance(row, dict)
                member = safe_member_path(row["path"])
                try:
                    data = archive.read(member)
                except KeyError as exc:
                    raise OuterElfError(f"SjCRUNCH archive is missing {member}") from exc
                if len(data) != int(row["size"]):
                    raise OuterElfError(f"SjCRUNCH member {member} has an unexpected size")
                if sha256_bytes(data) != str(row["sha256"]):
                    raise OuterElfError(f"SjCRUNCH member {member} has an unexpected SHA-256")
    except (OSError, zipfile.BadZipFile) as exc:
        raise OuterElfError(f"cannot inspect SjCRUNCH archive {path}: {exc}") from exc


def download_archive(destination: Path, manifest: dict[str, object]) -> None:
    archive_meta = manifest["archive"]
    assert isinstance(archive_meta, dict)
    destination.parent.mkdir(parents=True, exist_ok=True)
    temporary = destination.with_suffix(destination.suffix + ".part")
    request = urllib.request.Request(
        str(archive_meta["url"]),
        headers={
            "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 Chrome/140 Safari/537.36",
            "Accept": "application/zip,application/octet-stream;q=0.9,*/*;q=0.8",
            "Referer": "https://sksapps.haldrie.com/ps2mis.php",
        },
    )
    try:
        with urllib.request.urlopen(request, timeout=60) as response, temporary.open("wb") as output:
            shutil.copyfileobj(response, output)
        verify_archive(temporary, manifest)
        temporary.replace(destination)
    except (OSError, OuterElfError) as exc:
        try:
            temporary.unlink()
        except FileNotFoundError:
            pass
        raise OuterElfError(f"SjCRUNCH archive download failed: {exc}") from exc


def extract_linked_inputs(
    archive_path: Path, destination: Path, manifest: dict[str, object]
) -> dict[str, Path]:
    result: dict[str, Path] = {}
    try:
        with zipfile.ZipFile(archive_path) as archive:
            for row in manifest["linked_inputs"]:
                assert isinstance(row, dict)
                member = safe_member_path(row["path"])
                data = archive.read(member)
                if len(data) != int(row["size"]) or sha256_bytes(data) != str(row["sha256"]):
                    raise OuterElfError(f"refusing changed SjCRUNCH member {member}")
                output = destination.joinpath(*PurePosixPath(member).parts)
                output.parent.mkdir(parents=True, exist_ok=True)
                output.write_bytes(data)
                result[member] = output
    except (OSError, KeyError, zipfile.BadZipFile) as exc:
        raise OuterElfError(f"cannot extract verified SjCRUNCH inputs: {exc}") from exc
    return result


def assembly_source(container_name: str = "container.bin") -> str:
    if '"' in container_name or "\n" in container_name or "\r" in container_name:
        raise OuterElfError("unsafe container name for generated assembly")
    return f""".data
.balign 4

.globl PackedElf
PackedElf:
  .incbin \"{container_name}\"

.section .pdr,\"\"
.balign 4
"""


def run_command(command: list[str], cwd: Path) -> None:
    try:
        subprocess.run(command, cwd=cwd, check=True)
    except FileNotFoundError as exc:
        raise OuterElfError(f"required historical tool is missing: {command[0]}") from exc
    except subprocess.CalledProcessError as exc:
        raise OuterElfError(f"historical outer-ELF command failed with status {exc.returncode}") from exc


def build_outer_elf(args: argparse.Namespace, manifest: dict[str, object]) -> None:
    verify_archive(args.archive, manifest)
    build = manifest["build"]
    assert isinstance(build, dict)
    try:
        container_size = args.container.stat().st_size
        container_hash = sha256_file(args.container)
    except OSError as exc:
        raise OuterElfError(f"cannot read exact SJCRUNCH2 container {args.container}: {exc}") from exc
    if container_size != int(build["container_size"]) or container_hash != str(build["container_sha256"]):
        raise OuterElfError("SJCRUNCH2 container does not match the frozen public contract")
    if not args.compiler.is_file():
        raise OuterElfError(f"historical EE compiler is missing: {args.compiler}")
    strip = args.strip or args.compiler.with_name("ee-strip")
    if not strip.is_file():
        raise OuterElfError(f"historical EE strip tool is missing: {strip}")

    args.build_dir.mkdir(parents=True, exist_ok=True)
    extracted = extract_linked_inputs(args.archive, args.build_dir / "upstream", manifest)
    local_container = args.build_dir / "container.bin"
    shutil.copyfile(args.container, local_container)
    source = args.build_dir / "packed_elf.s"
    source.write_text(assembly_source(local_container.name), encoding="ascii")
    unstripped = args.build_dir / "SNES_EMU.unstripped.ELF"
    args.output.parent.mkdir(parents=True, exist_ok=True)

    run_command(
        [
            str(args.compiler.resolve()),
            "-o",
            str(unstripped.resolve()),
            f"-Wl,--defsym,_start_address={int(build['start_address']):#x}",
            "-T",
            str(extracted["script/linkfile"].resolve()),
            str(source.resolve()),
            str(extracted["script/crunch_crt0.o"].resolve()),
            f"-Wl,-L{extracted['script/libsjcrunch.a'].parent.resolve()}",
            "-nostartfiles",
            "-lsjcrunch",
        ],
        args.build_dir,
    )
    shutil.copyfile(unstripped, args.output)
    run_command([str(strip.resolve()), "-F", "elf32-littlemips", str(args.output.resolve())], args.build_dir)

    output_size = args.output.stat().st_size
    output_hash = sha256_file(args.output)
    if output_size != int(build["output_size"]):
        raise OuterElfError(
            f"rebuilt ELF size differs: expected {build['output_size']}, got {output_size}"
        )
    if output_hash != str(build["output_sha256"]):
        raise OuterElfError(
            f"rebuilt ELF hash differs: expected {build['output_sha256']}, got {output_hash}"
        )
    print(f"SjCRUNCH archive: OK sha256={manifest['archive']['sha256']}")
    print(f"outer ELF={output_size}/{output_size} bytes exact")
    print(f"packed ELF sha256={output_hash}")
    print(f"output={args.output}")


def compare_reference(candidate: Path, reference: Path, manifest: dict[str, object]) -> None:
    build = manifest["build"]
    assert isinstance(build, dict)
    try:
        candidate_data = candidate.read_bytes()
        reference_data = reference.read_bytes()
    except OSError as exc:
        raise OuterElfError(f"cannot read final ELF comparison input: {exc}") from exc
    expected_hash = str(build["output_sha256"])
    if sha256_bytes(reference_data) != expected_hash:
        raise OuterElfError("private reference does not match the frozen packed ELF hash")
    if candidate_data != reference_data:
        limit = min(len(candidate_data), len(reference_data))
        difference = next((i for i in range(limit) if candidate_data[i] != reference_data[i]), limit)
        raise OuterElfError(f"rebuilt ELF first differs from the private reference at 0x{difference:08x}")
    print(f"private full-file comparison: EXACT ({len(candidate_data)}/{len(reference_data)} bytes)")


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    validate = subparsers.add_parser("validate", help="validate committed hash-only metadata")
    validate.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    fetch = subparsers.add_parser("fetch", help="download and verify the public SjCRUNCH archive")
    fetch.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    fetch.add_argument("--archive", type=Path, default=DEFAULT_ARCHIVE)
    for name in ("build", "check"):
        command = subparsers.add_parser(name)
        command.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
        command.add_argument("--archive", type=Path, default=DEFAULT_ARCHIVE)
        command.add_argument("--container", type=Path, default=DEFAULT_CONTAINER)
        command.add_argument("--compiler", type=Path, default=DEFAULT_COMPILER)
        command.add_argument("--strip", type=Path)
        command.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD_DIR)
        command.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
        if name == "check":
            command.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> None:
    args = parse_args(argv)
    try:
        manifest = load_manifest(args.manifest)
        if args.command == "validate":
            print(
                "SJCRUNCH2 outer-ELF manifest: OK "
                f"({manifest['build']['output_size']} bytes; public hashes only)"
            )
        elif args.command == "fetch":
            if not args.archive.exists():
                download_archive(args.archive, manifest)
            verify_archive(args.archive, manifest)
            print(f"SjCRUNCH 2.1 archive: OK sha256={manifest['archive']['sha256']}")
        else:
            build_outer_elf(args, manifest)
            if args.command == "check":
                compare_reference(args.output, args.reference, manifest)
    except OuterElfError as exc:
        raise SystemExit(str(exc)) from exc


if __name__ == "__main__":
    main()
