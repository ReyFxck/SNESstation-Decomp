#!/usr/bin/env python3
"""Fetch and verify historical upstream source used for matching experiments."""
from __future__ import annotations

import argparse
import hashlib
import shutil
import tarfile
import urllib.request
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[1]
NEWLIB_VERSION = "1.10.0"
NEWLIB_URL = f"https://sourceware.org/pub/newlib/newlib-{NEWLIB_VERSION}.tar.gz"
NEWLIB_SHA256 = "69b62ad4c746a9acaf4f898772549f6da49f228f83a95efce7e88ae1d88c5a84"
ARCHIVE_ROOT = f"newlib-{NEWLIB_VERSION}"
MATHFP_PREFIX = f"{ARCHIVE_ROOT}/newlib/libm/mathfp/"
EXACT_FILES = {
    f"{ARCHIVE_ROOT}/COPYING.NEWLIB",
    f"{ARCHIVE_ROOT}/README",
    f"{ARCHIVE_ROOT}/newlib/libm/common/fdlibm.h",
}


def digest(path: Path) -> str:
    hasher = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            hasher.update(block)
    return hasher.hexdigest()


def wanted(name: str) -> bool:
    return name in EXACT_FILES or name.startswith(MATHFP_PREFIX)


def safe_relative(name: str) -> Path:
    pure = PurePosixPath(name)
    if pure.is_absolute() or ".." in pure.parts or not pure.parts or pure.parts[0] != ARCHIVE_ROOT:
        raise ValueError(f"unsafe archive path: {name!r}")
    return Path(*pure.parts[1:])


def download(url: str, destination: Path) -> None:
    destination.parent.mkdir(parents=True, exist_ok=True)
    request = urllib.request.Request(url, headers={"User-Agent": "SNESstation-Decomp/1.0"})
    temporary = destination.with_suffix(destination.suffix + ".part")
    with urllib.request.urlopen(request) as response, temporary.open("wb") as output:
        shutil.copyfileobj(response, output)
    temporary.replace(destination)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--archive",
        type=Path,
        default=ROOT / "build" / "upstream" / f"newlib-{NEWLIB_VERSION}.tar.gz",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=ROOT / "build" / "upstream" / f"newlib-{NEWLIB_VERSION}",
    )
    parser.add_argument("--url", default=NEWLIB_URL)
    args = parser.parse_args()

    if not args.archive.exists():
        print(f"downloading {args.url}")
        try:
            download(args.url, args.archive)
        except OSError as exc:
            raise SystemExit(f"download failed: {exc}") from exc

    actual_hash = digest(args.archive)
    if actual_hash != NEWLIB_SHA256:
        raise SystemExit(
            f"Newlib archive SHA-256 mismatch: expected {NEWLIB_SHA256}, got {actual_hash}; "
            f"refusing to extract {args.archive}"
        )

    extracted: list[str] = []
    args.output.mkdir(parents=True, exist_ok=True)
    try:
        with tarfile.open(args.archive, "r:gz") as archive:
            for member in archive.getmembers():
                if not wanted(member.name) or not member.isfile():
                    continue
                relative = safe_relative(member.name)
                source = archive.extractfile(member)
                if source is None:
                    raise ValueError(f"could not read archive member {member.name}")
                destination = args.output / relative
                destination.parent.mkdir(parents=True, exist_ok=True)
                with source, destination.open("wb") as output:
                    shutil.copyfileobj(source, output)
                extracted.append(relative.as_posix())
    except (OSError, tarfile.TarError, ValueError) as exc:
        raise SystemExit(f"safe extraction failed: {exc}") from exc

    required = {
        "COPYING.NEWLIB",
        "newlib/libm/mathfp/sf_cos.c",
        "newlib/libm/mathfp/sf_sin.c",
        "newlib/libm/mathfp/sf_sine.c",
        "newlib/libm/mathfp/sf_tan.c",
        "newlib/libm/mathfp/sf_atan.c",
        "newlib/libm/mathfp/sf_atangent.c",
        "newlib/libm/mathfp/sf_sqrt.c",
        "newlib/libm/mathfp/sf_fabs.c",
        "newlib/libm/mathfp/sf_numtest.c",
        "newlib/libm/mathfp/s_mathcnst.c",
        "newlib/libm/mathfp/zmath.h",
    }
    missing = required - set(extracted)
    if missing:
        raise SystemExit(f"archive is missing expected mathfp files: {', '.join(sorted(missing))}")

    print(f"archive: OK sha256={actual_hash}")
    print(f"extracted {len(extracted)} historical files to {args.output}")
    print(f"mathfp source: {args.output / 'newlib/libm/mathfp'}")


if __name__ == "__main__":
    main()
