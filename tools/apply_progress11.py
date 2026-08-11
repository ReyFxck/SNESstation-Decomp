#!/usr/bin/env python3
"""Apply the Progress 11 manifest/docs update after overlay extraction."""

from __future__ import annotations

import csv
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "analysis" / "progress_targets.csv"
JAL = ROOT / "analysis" / "jal_candidates.csv"
SRC_README = ROOT / "src" / "README.md"
VALIDATION = ROOT / "analysis" / "progress11_validation.txt"

UPDATES = {
    "0x0010a840": (
        "apu_buffer_allocator",
        "audio",
        "RECONSTRUCTED",
        "high",
        "Three target allocations and failure cleanup recovered directly from R5900 assembly",
    ),
    "0x0010a8bc": (
        "apu_buffer_cleanup",
        "audio",
        "RECONSTRUCTED",
        "high",
        "Frees and nulls APU buffers at target global offsets +0x04/+0x20/+0x24",
    ),
    "0x00151330": (
        "per_rom_cleanup",
        "memory",
        "RECONSTRUCTED",
        "high",
        "Calls 0x00151360 then target helper 0x00150f54 with memory and mode 0; complete target body recovered",
    ),
    "0x00151360": (
        "per_rom_buffer_cleanup",
        "memory",
        "RECONSTRUCTED",
        "high",
        "Frees and nulls object temporary pointers at effective offsets +0xb064/+0xb068",
    ),
}

README_MARKER = "## Progress 11 APU allocation / per-ROM cleanup recovery"
README_SECTION = r"""

## Progress 11 APU allocation / per-ROM cleanup recovery

- `snes9x/apu_alloc_recovered.c` — exact three-buffer allocation/failure cleanup
  corridor at `0x0010a840..0x0010a933`;
- `snes9x/memory_cleanup_recovered.c` — per-ROM cleanup orchestrator and
  temporary-buffer teardown at `0x00151330..0x001513bb`.

The target itself remains authoritative. `0x0012a400` is intentionally left
identified rather than promoted because its focused assembly capture ends with
live control flow back into the parent PPU routine. See `docs/PROGRESS11.md`.
"""


def read_jal_targets() -> set[str]:
    with JAL.open(newline="", encoding="utf-8") as f:
        return {row["target"].lower() for row in csv.DictReader(f)}


def update_manifest() -> None:
    text = MANIFEST.read_text(encoding="utf-8")
    lines = text.splitlines()
    if not lines or not lines[0].startswith("address,name,area,status,confidence,notes"):
        raise SystemExit("unexpected analysis/progress_targets.csv header")

    header, body = lines[0], lines[1:]
    replacement = {}
    for address, values in UPDATES.items():
        replacement[address] = ",".join((address, *values))

    seen = set()
    out = []
    for line in body:
        if not line.strip():
            continue
        address = line.split(",", 1)[0].lower()
        if address in replacement:
            out.append(replacement[address])
            seen.add(address)
        else:
            out.append(line)

    for address, line in replacement.items():
        if address not in seen:
            out.append(line)

    out.sort(key=lambda line: int(line.split(",", 1)[0], 16))
    MANIFEST.write_text(header + "\n" + "\n".join(out) + "\n", encoding="utf-8")


def update_src_readme() -> None:
    text = SRC_README.read_text(encoding="utf-8")
    if README_MARKER not in text:
        SRC_README.write_text(text.rstrip() + README_SECTION + "\n", encoding="utf-8")


def run(cmd: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        cmd,
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )


def main() -> None:
    required = (
        ROOT / "src/snes9x/apu_alloc_recovered.c",
        ROOT / "src/snes9x/memory_cleanup_recovered.c",
        ROOT / "docs/PROGRESS11.md",
        ROOT / "tools/update_progress.py",
    )
    missing_files = [p for p in required if not p.exists()]
    if missing_files:
        raise SystemExit("missing overlay/repo files: " + ", ".join(map(str, missing_files)))

    jal_targets = read_jal_targets()
    missing_targets = [a for a in UPDATES if a not in jal_targets]
    if missing_targets:
        raise SystemExit(
            "refusing to update manifest: addresses absent from jal_candidates.csv: "
            + ", ".join(missing_targets)
        )

    update_manifest()
    update_src_readme()

    progress = run([sys.executable, "tools/update_progress.py"])
    if progress.returncode != 0:
        print(progress.stdout, end="")
        raise SystemExit("tools/update_progress.py failed")

    validation = [
        "Progress 11 local validation",
        "============================",
        "",
        "JAL target verification: PASS",
    ]
    validation.extend(f"  {a}: present" for a in UPDATES)
    validation.extend(["", progress.stdout.rstrip(), ""])

    cc = shutil.which("cc") or shutil.which("gcc") or shutil.which("clang")
    sources = (
        "src/snes9x/apu_alloc_recovered.c",
        "src/snes9x/memory_cleanup_recovered.c",
    )
    if cc:
        syntax_ok = True
        for src in sources:
            proc = run([cc, "-std=c11", "-Wall", "-Wextra", "-Werror",
                        "-fsyntax-only", src])
            validation.append(f"syntax {src}: " + ("PASS" if proc.returncode == 0 else "FAIL"))
            if proc.stdout.strip():
                validation.append(proc.stdout.rstrip())
            syntax_ok &= proc.returncode == 0
        if not syntax_ok:
            VALIDATION.write_text("\n".join(validation) + "\n", encoding="utf-8")
            raise SystemExit("new Progress 11 C source failed host syntax validation")
    else:
        validation.append("host syntax validation: SKIPPED (no cc/gcc/clang found)")

    git = shutil.which("git")
    if git:
        diffcheck = run([git, "diff", "--check"])
        validation.extend([
            "",
            "git diff --check: " + ("PASS" if diffcheck.returncode == 0 else "FAIL"),
        ])
        if diffcheck.stdout.strip():
            validation.append(diffcheck.stdout.rstrip())
        if diffcheck.returncode != 0:
            VALIDATION.write_text("\n".join(validation) + "\n", encoding="utf-8")
            raise SystemExit("git diff --check failed")

    VALIDATION.write_text("\n".join(validation) + "\n", encoding="utf-8")
    print(progress.stdout, end="")
    print("Progress 11 applied successfully.")
    print(f"wrote {VALIDATION.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
