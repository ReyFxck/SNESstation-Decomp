#!/usr/bin/env python3
"""Build the pinned PS2 EE GCC 3.2.2 stage-one candidate without root access."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import platform
import shlex
import shutil
import subprocess
import sys
import tarfile
import tempfile
import time
import urllib.error
import urllib.request
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

from fetch_ee_toolchain_recipe import COMMIT, fetch, verify_checkout


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_WORK_DIR = ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1"
RECIPE_DIR = ROOT / "build" / "upstream" / "ps2toolchain-2004"
MODERN_GCC_PATCH = ROOT / "tools" / "patches" / "gcc-3.2.2-modern-host.patch"
AARCH64_CONFIG_PATCH = ROOT / "tools" / "patches" / "gnu-config-aarch64.patch"
AARCH64_GCC_HOST_PATCH = (
    ROOT / "tools" / "patches" / "gcc-3.2.2-aarch64-host.patch"
)
HOST_CFLAGS = "-O2 -g -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0"
MINIMUM_FREE_BYTES = 650 * 1024 * 1024


@dataclass(frozen=True)
class Archive:
    name: str
    url: str
    sha256: str
    source_directory: str
    historical_patch: str


ARCHIVES = (
    Archive(
        name="binutils-2.14.tar.gz",
        url="https://ftp.gnu.org/gnu/binutils/binutils-2.14.tar.gz",
        sha256="ba91202a1aefca79f5eeb534e6c4235c874b220a2975725296712e42e6b91df1",
        source_directory="binutils-2.14",
        historical_patch="binutils-2.14.patch",
    ),
    Archive(
        name="gcc-3.2.2.tar.gz",
        url="https://ftp.gnu.org/gnu/gcc/gcc-3.2.2/gcc-3.2.2.tar.gz",
        sha256="a0a626b10be8f793349a5309dd054a224c00772c83207d0354499c17e8deb187",
        source_directory="gcc-3.2.2",
        historical_patch="gcc-3.2.2.patch",
    ),
)


class BuildFailure(RuntimeError):
    """A recoverable bootstrap failure with a user-facing diagnostic."""


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def atomic_write_text(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(f".{path.name}.tmp")
    temporary.write_text(text, encoding="utf-8")
    os.replace(temporary, path)


def require_programs() -> None:
    required = ("git", "patch", "make", "gcc", "ar", "ranlib", "ld", "nm")
    missing = [name for name in required if shutil.which(name) is None]
    if missing:
        raise BuildFailure(
            "missing host build programs: "
            + ", ".join(missing)
            + "\nDebian/Ubuntu: apt install git patch make gcc binutils"
        )
    version = subprocess.run(
        ["make", "--version"],
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    ).stdout
    if "GNU Make" not in version:
        raise BuildFailure("the historical build requires GNU Make")


def validate_work_directory(path: Path) -> Path:
    resolved = path.expanduser().resolve()
    if any(character.isspace() for character in str(resolved)):
        raise BuildFailure(
            f"old configure scripts cannot safely use whitespace in paths: {resolved}"
        )
    resolved.mkdir(parents=True, exist_ok=True)
    return resolved


def check_disk_space(work_dir: Path, compiler: Path) -> None:
    if compiler.is_file():
        return
    free = shutil.disk_usage(work_dir).free
    if free < MINIMUM_FREE_BYTES:
        free_mib = free // (1024 * 1024)
        required_mib = MINIMUM_FREE_BYTES // (1024 * 1024)
        raise BuildFailure(
            f"only {free_mib} MiB free; stage one requires at least "
            f"{required_mib} MiB free (1 GiB is recommended)"
        )


def download_archive(archive: Archive, directory: Path) -> Path:
    directory.mkdir(parents=True, exist_ok=True)
    destination = directory / archive.name
    if destination.is_file():
        actual = sha256_file(destination)
        if actual != archive.sha256:
            raise BuildFailure(
                f"refusing unverified archive {destination}\n"
                f"expected SHA-256 {archive.sha256}\nactual   SHA-256 {actual}"
            )
        print(f"archive: verified {archive.name}")
        return destination

    partial = destination.with_name(destination.name + ".part")
    if partial.is_file() and sha256_file(partial) == archive.sha256:
        os.replace(partial, destination)
        print(f"archive: verified recovered download {archive.name}")
        return destination
    for attempt in range(1, 4):
        offset = partial.stat().st_size if partial.is_file() else 0
        headers = {"User-Agent": "SNESstation-Decomp-toolchain-bootstrap/1"}
        if offset:
            headers["Range"] = f"bytes={offset}-"
        request = urllib.request.Request(archive.url, headers=headers)
        print(
            f"download: {archive.name}"
            + (f" (resuming at {offset} bytes)" if offset else "")
        )
        try:
            with urllib.request.urlopen(request, timeout=90) as response:
                status = getattr(response, "status", response.getcode())
                content_range = response.headers.get("Content-Range", "")
                resume = offset > 0 and status == 206 and content_range.startswith(
                    f"bytes {offset}-"
                )
                mode = "ab" if resume else "wb"
                with partial.open(mode) as output:
                    shutil.copyfileobj(response, output, length=1024 * 1024)
        except (OSError, urllib.error.URLError) as exc:
            if attempt == 3:
                raise BuildFailure(
                    f"download failed after {attempt} attempts: {archive.url}: {exc}"
                ) from exc
            print(f"download retry {attempt}/3 after: {exc}")
            continue

        actual = sha256_file(partial)
        if actual != archive.sha256:
            raise BuildFailure(
                f"downloaded {archive.name} failed SHA-256 verification\n"
                f"expected {archive.sha256}\nactual   {actual}\n"
                f"partial file retained at {partial}"
            )
        os.replace(partial, destination)
        print(f"archive: verified {archive.name}")
        return destination

    raise AssertionError("unreachable download retry state")


def safe_archive_path(name: str, expected_root: str) -> tuple[str, ...]:
    path = PurePosixPath(name)
    parts = path.parts
    if path.is_absolute() or not parts or any(part in {"", ".", ".."} for part in parts):
        raise BuildFailure(f"unsafe archive path: {name!r}")
    if parts[0] != expected_root:
        raise BuildFailure(
            f"archive member is outside expected {expected_root}/ root: {name!r}"
        )
    return parts


def safe_extract_archive(archive: Path, source_root: Path, expected_root: str) -> Path:
    """Extract regular files/directories only, without preserving archive owners."""
    destination = source_root / expected_root
    if destination.exists():
        if not destination.is_dir():
            raise BuildFailure(f"source path is not a directory: {destination}")
        return destination

    source_root.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(
        prefix=f".{expected_root}-extract-", dir=source_root
    ) as temporary_name:
        temporary = Path(temporary_name)
        directory_metadata: list[tuple[Path, int, int]] = []
        with tarfile.open(archive, mode="r:gz") as tar:
            for member in tar:
                parts = safe_archive_path(member.name, expected_root)
                target = temporary.joinpath(*parts)
                if member.isdir():
                    target.mkdir(parents=True, exist_ok=True)
                    directory_metadata.append(
                        (target, member.mode & 0o777, member.mtime)
                    )
                    continue
                if not member.isfile():
                    raise BuildFailure(
                        f"unsupported archive member type for {member.name!r}"
                    )
                target.parent.mkdir(parents=True, exist_ok=True)
                source = tar.extractfile(member)
                if source is None:
                    raise BuildFailure(f"could not read archive member {member.name!r}")
                with source, target.open("xb") as output:
                    shutil.copyfileobj(source, output, length=1024 * 1024)
                os.chmod(target, member.mode & 0o777)
                os.utime(target, (member.mtime, member.mtime))
        for directory, mode, timestamp in reversed(directory_metadata):
            os.chmod(directory, mode)
            os.utime(directory, (timestamp, timestamp))

        extracted = temporary / expected_root
        if not extracted.is_dir():
            raise BuildFailure(f"archive did not create {expected_root}/")
        os.replace(extracted, destination)
    print(f"source: extracted {expected_root}")
    return destination


def patch_probe(source: Path, patch_file: Path, reverse: bool) -> subprocess.CompletedProcess[str]:
    command = ["patch", "--batch", "--silent", "--dry-run", "-p1"]
    if reverse:
        command.append("--reverse")
    command.extend(["--input", str(patch_file)])
    return subprocess.run(
        command,
        cwd=source,
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )


def apply_patch_once(source: Path, patch_file: Path, stamp_name: str) -> None:
    expected = f"{patch_file.name} {sha256_file(patch_file)}\n"
    stamp = source / stamp_name
    if stamp.is_file():
        if stamp.read_text(encoding="utf-8") != expected:
            raise BuildFailure(f"patch stamp does not match current patch: {stamp}")
        print(f"patch: already applied {patch_file.name}")
        return

    forward = patch_probe(source, patch_file, reverse=False)
    if forward.returncode == 0:
        result = subprocess.run(
            [
                "patch",
                "--batch",
                "--silent",
                "-p1",
                "--input",
                str(patch_file),
            ],
            cwd=source,
            check=False,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
        if result.returncode != 0:
            raise BuildFailure(
                f"failed to apply {patch_file.name}:\n{result.stdout[-4000:]}"
            )
        atomic_write_text(stamp, expected)
        print(f"patch: applied {patch_file.name}")
        return

    reverse = patch_probe(source, patch_file, reverse=True)
    if reverse.returncode == 0:
        atomic_write_text(stamp, expected)
        print(f"patch: recovered interrupted stamp for {patch_file.name}")
        return

    diagnostic = (forward.stdout + "\n" + reverse.stdout).strip()
    raise BuildFailure(
        f"source is neither clean nor fully patched for {patch_file.name}:\n"
        f"{diagnostic[-4000:]}"
    )


def tail_log(path: Path, limit: int = 12000) -> str:
    with path.open("rb") as stream:
        size = stream.seek(0, os.SEEK_END)
        stream.seek(max(0, size - limit))
        return stream.read().decode("utf-8", errors="replace")


def run_logged(
    label: str,
    command: list[str],
    cwd: Path,
    environment: dict[str, str],
    log: Path,
) -> None:
    log.parent.mkdir(parents=True, exist_ok=True)
    print(f"step: {label}")
    started = time.monotonic()
    with log.open("a", encoding="utf-8") as output:
        output.write(f"\n$ {shlex.join(command)}\n")
        output.flush()
        process = subprocess.Popen(
            command,
            cwd=cwd,
            env=environment,
            text=True,
            stdout=output,
            stderr=subprocess.STDOUT,
        )
        next_update = started + 30
        while process.poll() is None:
            time.sleep(2)
            now = time.monotonic()
            if now >= next_update:
                print(f"  {label}: still running ({int(now - started)}s); log {log}")
                next_update = now + 30
    elapsed = int(time.monotonic() - started)
    if process.returncode != 0:
        raise BuildFailure(
            f"{label} failed with exit {process.returncode}; log: {log}\n"
            f"--- log tail ---\n{tail_log(log)}"
        )
    print(f"step: OK {label} ({elapsed}s)")


def stamp_matches(path: Path, signature: str) -> bool:
    return path.is_file() and path.read_text(encoding="utf-8") == signature


def run_stamped_step(
    name: str,
    command: list[str],
    cwd: Path,
    environment: dict[str, str],
    stamp_directory: Path,
    log_directory: Path,
) -> None:
    relevant_environment = {
        key: environment.get(key, "")
        for key in ("CC", "CFLAGS", "CXXFLAGS", "CPPFLAGS", "LDFLAGS", "LC_ALL")
    }
    signature = json.dumps(
        {"command": command, "cwd": str(cwd), "environment": relevant_environment},
        sort_keys=True,
        separators=(",", ":"),
    ) + "\n"
    stamp = stamp_directory / name
    if stamp_matches(stamp, signature):
        print(f"step: resume {name}")
        return
    run_logged(name, command, cwd, environment, log_directory / f"{name}.log")
    atomic_write_text(stamp, signature)


def prepare_sources(work_dir: Path, recipe: Path) -> dict[str, Path]:
    downloads = work_dir / "downloads"
    source_root = work_dir / "source"
    result: dict[str, Path] = {}
    for archive in ARCHIVES:
        archive_path = download_archive(archive, downloads)
        source = safe_extract_archive(
            archive_path, source_root, archive.source_directory
        )
        apply_patch_once(
            source,
            recipe / archive.historical_patch,
            ".snesstation-historical-patch",
        )
        apply_patch_once(
            source,
            AARCH64_CONFIG_PATCH,
            ".snesstation-aarch64-config-patch",
        )
        result[archive.source_directory] = source

    apply_patch_once(
        result["gcc-3.2.2"],
        MODERN_GCC_PATCH,
        ".snesstation-modern-host-patch",
    )
    apply_patch_once(
        result["gcc-3.2.2"],
        AARCH64_GCC_HOST_PATCH,
        ".snesstation-aarch64-gcc-host-patch",
    )
    return result


def clean_host_environment() -> dict[str, str]:
    environment = os.environ.copy()
    for name in ("CPPFLAGS", "LDFLAGS"):
        environment.pop(name, None)
    environment["CC"] = "gcc"
    environment["CFLAGS"] = HOST_CFLAGS
    environment["CXXFLAGS"] = HOST_CFLAGS
    environment["LC_ALL"] = "C"
    return environment


def build_stage_one(work_dir: Path, sources: dict[str, Path], jobs: int) -> Path:
    build_root = work_dir / "build"
    prefix = work_dir / "prefix"
    stamps = work_dir / "stamps"
    logs = work_dir / "logs"
    binutils_build = build_root / "binutils-ee"
    gcc_build = build_root / "gcc-ee-stage1"
    for directory in (prefix, stamps, binutils_build, gcc_build):
        directory.mkdir(parents=True, exist_ok=True)

    host_environment = clean_host_environment()
    run_stamped_step(
        "01-binutils-configure",
        [
            str(sources["binutils-2.14"] / "configure"),
            f"--prefix={prefix}",
            "--target=ee",
        ],
        binutils_build,
        host_environment,
        stamps,
        logs,
    )
    run_stamped_step(
        "02-binutils-build",
        ["make", f"-j{jobs}"],
        binutils_build,
        host_environment,
        stamps,
        logs,
    )
    run_stamped_step(
        "03-binutils-install",
        ["make", "install"],
        binutils_build,
        host_environment,
        stamps,
        logs,
    )
    required_binutils = ("ee-as", "ee-ar", "ee-ld", "ee-ranlib")
    missing = [
        name for name in required_binutils if not (prefix / "bin" / name).is_file()
    ]
    if missing:
        raise BuildFailure(
            "binutils install stamp exists but tools are missing: "
            + ", ".join(missing)
        )

    target_environment = clean_host_environment()
    target_environment["PATH"] = str(prefix / "bin") + os.pathsep + target_environment.get(
        "PATH", ""
    )
    run_stamped_step(
        "04-gcc-configure",
        [
            str(sources["gcc-3.2.2"] / "configure"),
            f"--prefix={prefix}",
            "--target=ee",
            "--enable-languages=c",
            "--with-newlib",
            "--without-headers",
        ],
        gcc_build,
        target_environment,
        stamps,
        logs,
    )
    run_stamped_step(
        "05-gcc-build",
        ["make", f"-j{jobs}", "all-gcc"],
        gcc_build,
        target_environment,
        stamps,
        logs,
    )
    run_stamped_step(
        "06-gcc-install",
        ["make", "install-gcc"],
        gcc_build,
        target_environment,
        stamps,
        logs,
    )
    return prefix / "bin" / "ee-gcc"


def command_first_line(command: list[str]) -> str:
    result = subprocess.run(
        command,
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    return result.stdout.strip().splitlines()[0] if result.stdout.strip() else "unknown"


def verify_and_record(work_dir: Path, compiler: Path, jobs: int) -> None:
    if not compiler.is_file() or not os.access(compiler, os.X_OK):
        raise BuildFailure(f"stage-one compiler was not installed: {compiler}")
    probe = subprocess.run(
        [
            sys.executable,
            str(ROOT / "tools" / "probe_ee_toolchain.py"),
            "--compiler",
            str(compiler),
        ],
        cwd=ROOT,
        check=False,
    )
    if probe.returncode != 0:
        raise BuildFailure(f"installed compiler failed the EE contract probe: {compiler}")

    manifest = {
        "schema": 1,
        "scope": "binutils EE plus GCC C stage one; no Newlib, C++, PS2SDK or final ELF",
        "recipe_commit": COMMIT,
        "archives": [
            {"name": archive.name, "url": archive.url, "sha256": archive.sha256}
            for archive in ARCHIVES
        ],
        "modern_host_patch_sha256": sha256_file(MODERN_GCC_PATCH),
        "aarch64_config_patch_sha256": sha256_file(AARCH64_CONFIG_PATCH),
        "aarch64_gcc_host_patch_sha256": sha256_file(AARCH64_GCC_HOST_PATCH),
        "host_cflags": HOST_CFLAGS,
        "host_architecture": platform.machine() or "unknown",
        "host_gcc": command_first_line(["gcc", "--version"]),
        "jobs": jobs,
        "compiler": str(compiler),
        "compiler_sha256": sha256_file(compiler),
        "compiler_banner": command_first_line([str(compiler), "--version"]),
    }
    atomic_write_text(
        work_dir / "bootstrap-manifest.json",
        json.dumps(manifest, indent=2, sort_keys=True) + "\n",
    )


def main() -> None:
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(line_buffering=True)
    default_jobs = min(4, os.cpu_count() or 1)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work-dir", type=Path, default=DEFAULT_WORK_DIR)
    parser.add_argument("--jobs", type=int, default=default_jobs)
    args = parser.parse_args()
    if args.jobs < 1 or args.jobs > 64:
        parser.error("--jobs must be between 1 and 64")

    try:
        require_programs()
        work_dir = validate_work_directory(args.work_dir)
        compiler = work_dir / "prefix" / "bin" / "ee-gcc"
        check_disk_space(work_dir, compiler)
        print(f"host: {platform.system()} {platform.machine() or 'unknown'}")
        print(f"work directory: {work_dir}")
        print(f"parallel jobs: {args.jobs}")
        fetch(RECIPE_DIR)
        verify_checkout(RECIPE_DIR)
        print(f"recipe: verified PS2DEV {COMMIT}")
        sources = prepare_sources(work_dir, RECIPE_DIR)
        compiler = build_stage_one(work_dir, sources, args.jobs)
        verify_and_record(work_dir, compiler, args.jobs)
    except BuildFailure as exc:
        print(f"bootstrap: FAIL -- {exc}", file=sys.stderr)
        raise SystemExit(1) from exc

    print("bootstrap: PASS -- historical EE GCC stage one is usable")
    print(
        "scope: compile-only matching; Newlib, C++, PS2SDK and final linking "
        "are not built"
    )
    print(f"EE_CC={compiler}")
    print(f"manifest={work_dir / 'bootstrap-manifest.json'}")


if __name__ == "__main__":
    main()
