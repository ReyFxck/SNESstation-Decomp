#!/usr/bin/env python3
"""Fetch and verify the earliest public PS2DEV GCC 3.2.2 build recipe."""
from __future__ import annotations

import argparse
import hashlib
import shutil
import subprocess
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
REPOSITORY = "https://github.com/ps2dev/ps2toolchain.git"
COMMIT = "16a47184b3a5fdf4aea45fcc8fee082d3c4d4183"
EXPECTED_FILES = {
    "README.TXT": "514c2b9bffdba7e64ef9f436fc45eb63c881b0ba0dd05d2ba2ec24e5c519b714",
    "toolchain.sh": "3962bf7c32209b84db543b59273dd46c78c49387397d88c625156aeba3a7b9ff",
    "binutils-2.14.patch": "f63a6d656d51e9ed74bd26d7cde4fb237d6552569980f00c979ef73f52ff3cda",
    "gcc-3.2.2.patch": "803395ac6345d71ebdcdf6bd4a8981863e64b0cfb36cfb48c607c59d433c5b9a",
    "newlib-1.10.0.patch": "618634ff422e17aa517445308a2acbf8bfba95f8fee8b0bd5cbc123a0d69db38",
}


def digest(path: Path) -> str:
    hasher = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            hasher.update(block)
    return hasher.hexdigest()


def run_git(arguments: list[str], directory: Path) -> str:
    try:
        result = subprocess.run(
            ["git", "-C", str(directory), *arguments],
            check=True,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
    except FileNotFoundError as exc:
        raise SystemExit("git is required to fetch the historical recipe") from exc
    except subprocess.CalledProcessError as exc:
        diagnostic = (exc.stderr or exc.stdout).strip()
        raise SystemExit(f"git {' '.join(arguments)} failed: {diagnostic}") from exc
    return result.stdout.strip()


def verify_checkout(output: Path) -> None:
    if not (output / ".git").is_dir():
        raise SystemExit(f"refusing to use non-git directory: {output}")
    actual_commit = run_git(["rev-parse", "HEAD"], output)
    if actual_commit != COMMIT:
        raise SystemExit(
            f"historical recipe commit mismatch: expected {COMMIT}, got {actual_commit}"
        )
    status = run_git(["status", "--porcelain"], output)
    if status:
        raise SystemExit(f"historical recipe checkout is modified: {output}")
    for name, expected_hash in EXPECTED_FILES.items():
        path = output / name
        if not path.is_file():
            raise SystemExit(f"historical recipe is missing {name}")
        actual_hash = digest(path)
        if actual_hash != expected_hash:
            raise SystemExit(
                f"{name} SHA-256 mismatch: expected {expected_hash}, got {actual_hash}"
            )


def fetch(output: Path) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        verify_checkout(output)
        return

    with tempfile.TemporaryDirectory(prefix="ps2toolchain-recipe-", dir=output.parent) as tmp:
        checkout = Path(tmp) / "checkout"
        checkout.mkdir()
        run_git(["init", "--quiet"], checkout)
        run_git(["remote", "add", "origin", REPOSITORY], checkout)
        run_git(["fetch", "--quiet", "--depth=1", "origin", COMMIT], checkout)
        run_git(["checkout", "--quiet", "--detach", "FETCH_HEAD"], checkout)
        verify_checkout(checkout)
        shutil.move(str(checkout), str(output))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output",
        type=Path,
        default=ROOT / "build" / "upstream" / "ps2toolchain-2004",
    )
    args = parser.parse_args()

    fetch(args.output)
    verify_checkout(args.output)
    print(f"recipe commit: OK {COMMIT}")
    print(f"verified {len(EXPECTED_FILES)} historical files in {args.output}")
    print("versions: binutils 2.14; GCC 3.2.2; Newlib 1.10.0")
    print("note: sources and compiler binaries were not downloaded or installed")


if __name__ == "__main__":
    main()
