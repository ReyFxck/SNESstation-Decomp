#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import importlib.util
import os
import re
import shutil
import subprocess
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()

BASE_COMMIT = ""  # resolved from the current checkout at runtime
BASELINE = -1       # resolved from analysis/progress_targets.csv at runtime
TARGET_GOAL = int(os.environ.get("TARGET_CHECKPOINT", "350"))
TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

PS2DEV_REPO = "https://github.com/duduclx/PS2DEV.git"
PS2DEV_COMMIT = "bac0006c6302edcf1bdae253799484497b4e5032"

ZLIB_REPO = "https://github.com/madler/zlib.git"
# Official signed v1.1.3 tag d95de550... dereferences to this commit.
ZLIB_113_COMMIT = "14763ac7c6c03bca62c39e35c03cf5bfc7728802"

PGEN_REPO = "https://github.com/ps2homebrew/pgen.git"
PGEN_COMMIT = "403f1710e5eacb7d04e5031e1cb0a40435ff9d33"

BUILD = ROOT / "build/matching/progress58-hunt350"
OBJDIR = BUILD / "objects"
LOGDIR = BUILD / "logs"
PS2DEV = ROOT / "build/upstream/PS2DEV-bac0006c"
ZLIB = ROOT / "build/upstream/zlib-1.1.3-git"
PGEN = ROOT / "build/upstream/pgen-403f1710"
PREV_ATTEMPTS = ROOT / "build/matching/progress54-200plus-screen2/all_attempts.tsv"

for p in (BUILD, OBJDIR, LOGDIR, PS2DEV.parent, ZLIB.parent, PGEN.parent):
    p.mkdir(parents=True, exist_ok=True)

def die(msg: str) -> None:
    raise SystemExit(f"Progress58 hunt350: {msg}")

def run(
    cmd: list[str],
    *,
    cwd: Path = ROOT,
    timeout: int | None = None,
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        cmd,
        cwd=cwd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        timeout=timeout,
    )

def safe(text: str) -> str:
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", text)

# ---------------------------------------------------------------------------
# Repository / compiler gate
# ---------------------------------------------------------------------------
head = run(["git", "rev-parse", "HEAD"]).stdout.strip()
if not head:
    die("not inside a Git checkout")
BASE_COMMIT = head

dirty_worktree = bool(run(["git", "diff", "--quiet"]).returncode)
dirty_index = bool(run(["git", "diff", "--cached", "--quiet"]).returncode)
if dirty_worktree or dirty_index:
    print(
        "WARNING: working tree/index is dirty; the probe will still run, "
        "but evidence records the current HEAD and must be promoted only after review."
    )

EE_CC = Path(
    os.environ.get(
        "EE_CC",
        str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"),
    )
)
if not EE_CC.exists() or not os.access(EE_CC, os.X_OK):
    die(f"historical EE compiler unavailable: {EE_CC}")

# Official comparator implementation.
cmp_path = ROOT / "tools/compare_elf_functions.py"
spec = importlib.util.spec_from_file_location("cmp_p55", cmp_path)
if spec is None or spec.loader is None:
    die("cannot import tools/compare_elf_functions.py")
cmpmod = importlib.util.module_from_spec(spec)
sys.modules["cmp_p55"] = cmpmod
spec.loader.exec_module(cmpmod)
ELFFile = cmpmod.ELFFile
compare_function = cmpmod.compare_function

# Authoritative progress universe.
with (ROOT / "analysis/progress_targets.csv").open(newline="", encoding="utf-8") as f:
    progress_rows = list(csv.DictReader(f))

progress = {int(r["address"], 16): r for r in progress_rows}
baseline = sum(r["status"] == "MATCHING" for r in progress_rows)
BASELINE = baseline
if BASELINE >= TARGET_GOAL:
    print(f"Already at {BASELINE}/1041; target {TARGET_GOAL} is satisfied.")
    raise SystemExit(0)

starts = sorted(progress)
next_start = {
    addr: starts[i + 1] if i + 1 < len(starts) else None
    for i, addr in enumerate(starts)
}

# Name aliases for historical symbols. Exact canonical name wins; a target name
# ending in _00123456 also exposes its unsuffixed historical base name.
alias_to_addrs: dict[str, list[int]] = defaultdict(list)
for addr, row in progress.items():
    if row["status"] != "RECONSTRUCTED":
        continue
    name = row["name"]
    alias_to_addrs[name].append(addr)
    m = re.match(r"^(.*)_([0-9A-Fa-f]{8})$", name)
    if m:
        alias_to_addrs[m.group(1)].append(addr)

def itanium_method_alias(name: str) -> str | None:
    """Map simple old-GCC Itanium names like _ZN6gsFont10uploadFontE... to gsFont_uploadFont."""
    if not name.startswith("_ZN"):
        return None
    i = 3
    parts: list[str] = []
    try:
        while i < len(name) and len(parts) < 2:
            j = i
            while j < len(name) and name[j].isdigit():
                j += 1
            if j == i:
                return None
            n = int(name[i:j])
            part = name[j : j + n]
            if len(part) != n:
                return None
            parts.append(part)
            i = j + n
    except ValueError:
        return None
    if len(parts) == 2:
        return f"{parts[0]}_{parts[1]}"
    return None

def map_symbol(name: str) -> int | None:
    candidates = [name]
    alias = itanium_method_alias(name)
    if alias:
        candidates.append(alias)
    for suffix in ("_candidate", "_recovered"):
        if name.endswith(suffix):
            candidates.append(name[: -len(suffix)])
    m = re.match(r"^(.*)_([0-9A-Fa-f]{8})$", name)
    if m:
        candidates.append(m.group(1))
        try:
            addr = int(m.group(2), 16)
            if addr in progress and progress[addr]["status"] == "RECONSTRUCTED":
                return addr
        except ValueError:
            pass
    for candidate in candidates:
        vals = sorted(set(alias_to_addrs.get(candidate, [])))
        if len(vals) == 1:
            return vals[0]
    return None

# ---------------------------------------------------------------------------
# Target bytes: formal original image when available; committed listings else.
# ---------------------------------------------------------------------------
reference = ROOT / "build/SNES_EMU.unpacked.bin"
formal = False
if reference.exists():
    digest = hashlib.sha256(reference.read_bytes()).hexdigest()
    if digest == TARGET_SHA256:
        target = reference.read_bytes()
        known: set[int] | None = None
        formal = True
    else:
        print(
            "WARNING: build/SNES_EMU.unpacked.bin has unexpected SHA-256; "
            "using committed listings instead"
        )

if not formal:
    insn_re = re.compile(
        r"^\s*([0-9A-Fa-f]+):\s+"
        r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
        r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
    )
    byte_map: dict[int, int] = {}
    conflicts: set[int] = set()
    listings = sorted((ROOT / "analysis/functions").glob("*.asm"))
    for listing in listings:
        for line in listing.read_text(
            encoding="utf-8", errors="replace"
        ).splitlines():
            m = insn_re.match(line)
            if not m:
                continue
            addr = int(m.group(1), 16)
            bs = bytes(int(m.group(i), 16) for i in range(2, 6))
            for off, value in enumerate(bs):
                a = addr + off
                old = byte_map.get(a)
                if old is not None and old != value:
                    conflicts.add(a)
                else:
                    byte_map[a] = value
    for a in conflicts:
        byte_map.pop(a, None)
    if not byte_map:
        die("no committed target-listing bytes parsed")
    target_end = max(byte_map) + 1
    image = bytearray(target_end - TARGET_BASE)
    for addr, value in byte_map.items():
        if addr >= TARGET_BASE:
            image[addr - TARGET_BASE] = value
    target = bytes(image)
    known = set(byte_map)

def bytes_known(start: int, end: int) -> bool:
    if formal:
        return TARGET_BASE <= start <= end <= TARGET_BASE + len(target)
    assert known is not None
    return all(addr in known for addr in range(start, end))

def target_slice(start: int, end: int) -> bytes:
    return target[start - TARGET_BASE : end - TARGET_BASE]

# ---------------------------------------------------------------------------
# Boundary proof
# ---------------------------------------------------------------------------
@dataclass
class MappedSymbol:
    addr: int
    symbol: object

def section_bytes(elf: object, section_index: int, start: int, end: int) -> bytes:
    sec = elf.sections[section_index]
    rel_start = start - sec.address
    rel_end = end - sec.address
    if rel_start < 0 or rel_end < rel_start or rel_end > sec.size:
        return b""
    return elf.data[sec.offset + rel_start : sec.offset + rel_end]

def mapped_symbols(elf: object) -> list[MappedSymbol]:
    result: list[MappedSymbol] = []
    seen: set[tuple[int, str, int]] = set()
    for sym in elf.symbols:
        if sym.section_index == 0 or sym.size <= 0 or (sym.info & 0xF) != 2:
            continue
        addr = map_symbol(sym.name)
        if addr is None:
            continue
        key = (addr, sym.name, sym.value)
        if key in seen:
            continue
        seen.add(key)
        result.append(MappedSymbol(addr, sym))
    result.sort(key=lambda item: (item.symbol.section_index, item.symbol.value, item.addr))
    return result

def terminal_return_proof(addr: int, size: int) -> bool:
    if size < 8 or not bytes_known(addr, addr + size):
        return False
    tail = target_slice(addr + size - 8, addr + size)
    return tail[:4] == bytes.fromhex("0800e003")  # jr $ra

def boundary_proof(
    elf: object,
    items: list[MappedSymbol],
    index: int,
) -> tuple[bool, str]:
    item = items[index]
    addr = item.addr
    sym = item.symbol
    size = sym.size
    nxt = next_start.get(addr)
    if nxt is None or size <= 0:
        return False, "no-next-target"
    span = nxt - addr
    if size > span:
        return False, f"oversize:+0x{size-span:x}"
    if size == span:
        return True, "exact-next-boundary"

    # Strong object-layout proof: the next mapped function in this section is
    # also the next target and the object delta exactly equals the target delta.
    next_item = None
    for j in range(index + 1, len(items)):
        candidate = items[j]
        if candidate.symbol.section_index != sym.section_index:
            continue
        if candidate.symbol.value > sym.value:
            next_item = candidate
            break
    if next_item is not None and next_item.addr == nxt:
        obj_delta = next_item.symbol.value - sym.value
        if obj_delta == span:
            obj_gap = section_bytes(
                elf,
                sym.section_index,
                sym.value + size,
                next_item.symbol.value,
            )
            if bytes_known(addr + size, nxt):
                tgt_gap = target_slice(addr + size, nxt)
                if obj_gap == tgt_gap:
                    return True, f"object-layout-gap:0x{span-size:x}"

    # Conservative alignment padding proof.
    gap = span - size
    if 0 < gap <= 0x3C and gap % 4 == 0 and bytes_known(addr + size, nxt):
        tgt_gap = target_slice(addr + size, nxt)
        if not any(tgt_gap) and terminal_return_proof(addr, size):
            return True, f"jr-ra+nop-padding:0x{gap:x}"

    return False, f"unproven-gap:0x{gap:x}"

# ---------------------------------------------------------------------------
# Evidence accumulator / common object evaluator
# ---------------------------------------------------------------------------
matches: dict[int, dict[str, object]] = {}
attempts: list[dict[str, object]] = []
compile_failures: list[dict[str, str]] = []

def checkpoint() -> int:
    return BASELINE + len(matches)

def goal_reached() -> bool:
    return checkpoint() >= TARGET_GOAL

def evaluate_object(
    obj: Path,
    *,
    provenance: str,
    source: str,
    profile: str,
    detail: str = "",
) -> int:
    before = len(matches)
    try:
        elf = ELFFile(obj)
        items = mapped_symbols(elf)
    except Exception as exc:
        compile_failures.append(
            {
                "provenance": provenance,
                "source": source,
                "profile": profile,
                "error": f"ELF parse: {exc}",
            }
        )
        return 0

    for idx, item in enumerate(items):
        addr = item.addr
        if progress[addr]["status"] != "RECONSTRUCTED" or addr in matches:
            continue
        sym = item.symbol
        record: dict[str, object] = {
            "address": f"0x{addr:08x}",
            "name": progress[addr]["name"],
            "area": progress[addr]["area"],
            "provenance": provenance,
            "source": source,
            "profile": profile,
            "detail": detail,
            "object": str(obj.relative_to(ROOT)),
            "object_symbol": sym.name,
            "object_size": sym.size,
        }
        if not bytes_known(addr, addr + sym.size):
            record["result"] = "target-bytes-missing"
            attempts.append(record)
            continue

        ok, proof = boundary_proof(elf, items, idx)
        record["boundary"] = proof
        if not ok:
            record["result"] = "boundary-reject"
            attempts.append(record)
            continue

        try:
            comp = compare_function(
                target,
                addr - TARGET_BASE,
                sym.size,
                elf,
                sym.name,
            )
        except Exception as exc:
            record["result"] = f"compare-error:{exc}"
            attempts.append(record)
            continue

        record["differing_bytes"] = comp.differing_bytes
        record["raw_equal"] = comp.raw_equal
        record["normalized_equal"] = comp.normalized_equal
        record["unknown_relocations"] = ",".join(
            map(str, comp.unknown_relocation_types)
        )

        if comp.matching and not comp.unknown_relocation_types:
            record["result"] = "MATCH"
            matches[addr] = record.copy()
            print(
                f"  MATCH 0x{addr:08x} {progress[addr]['name']} "
                f"[{provenance}; {profile}; {proof}]",
                flush=True,
            )
        else:
            record["result"] = "mismatch"
        attempts.append(record)

    return len(matches) - before

def compile_source(
    source: Path,
    *,
    provenance: str,
    profile_name: str,
    profile_flags: list[str],
    include_flags: list[str],
    defines: list[str] | None = None,
    tag: str = "",
) -> Path | None:
    defines = defines or []
    stem = safe(
        f"{provenance}__{tag}__{source.name}__{profile_name}"
    )
    obj = OBJDIR / f"{stem}.o"
    log = LOGDIR / f"{stem}.log"

    common = [
        "-G0", "-EL", "-pipe", "-w",
        "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common",
        "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls",
        "-march=r5900", "-mtune=r5900",
        "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD",
        "-DCODE_PLATFORM=3",
    ]
    cmd = [
        str(EE_CC),
        *common,
        *profile_flags,
        *defines,
        *include_flags,
        "-c",
        str(source),
        "-o",
        str(obj),
    ]
    cp = run(cmd, timeout=90)
    log.write_text(cp.stdout, encoding="utf-8")
    if cp.returncode:
        compile_failures.append(
            {
                "provenance": provenance,
                "source": str(source),
                "profile": profile_name,
                "error": cp.stdout.splitlines()[-1] if cp.stdout.splitlines()
                else "compile-fail",
            }
        )
        return None
    return obj

# ---------------------------------------------------------------------------
# Historical source checkout helpers
# ---------------------------------------------------------------------------
def ensure_git_commit(path: Path, repo: str, commit: str) -> None:
    if not (path / ".git").exists():
        if path.exists():
            shutil.rmtree(path)
        path.mkdir(parents=True, exist_ok=True)
        cp = run(["git", "init", "-q", str(path)])
        if cp.returncode:
            die(cp.stdout)
        cp = run(["git", "-C", str(path), "remote", "add", "origin", repo])
        if cp.returncode:
            die(cp.stdout)
    got = run(["git", "-C", str(path), "rev-parse", "HEAD"]).stdout.strip()
    if got == commit:
        return
    cp = run(
        ["git", "-C", str(path), "fetch", "-q", "--depth=1", "origin", commit],
        timeout=180,
    )
    if cp.returncode:
        die(f"fetch failed for {repo}@{commit}:\n{cp.stdout}")
    cp = run(["git", "-C", str(path), "checkout", "-q", "--detach", "FETCH_HEAD"])
    if cp.returncode:
        die(f"checkout failed for {repo}@{commit}:\n{cp.stdout}")
    got = run(["git", "-C", str(path), "rev-parse", "HEAD"]).stdout.strip()
    if got != commit:
        die(f"source checkout mismatch: wanted {commit}, got {got}")

# Existing Progress54 sparse PS2DEV checkout may not contain every EE RPC
# directory if it came from an interrupted checkout. Ensure the sparse pattern.
if (PS2DEV / ".git").exists():
    run(["git", "-C", str(PS2DEV), "config", "core.sparseCheckout", "true"])
    sparse = PS2DEV / ".git/info/sparse-checkout"
    sparse.parent.mkdir(parents=True, exist_ok=True)
    sparse.write_text("ps2sdk/ee/\nps2sdk/common/\n", encoding="utf-8")

ensure_git_commit(PS2DEV, PS2DEV_REPO, PS2DEV_COMMIT)
if (PS2DEV / ".git/info/sparse-checkout").exists():
    run(["git", "-C", str(PS2DEV), "read-tree", "-mu", "HEAD"])

ensure_git_commit(ZLIB, ZLIB_REPO, ZLIB_113_COMMIT)
ensure_git_commit(PGEN, PGEN_REPO, PGEN_COMMIT)

# Verify the zlib source identity in-band too.
zlib_h = (ZLIB / "zlib.h").read_text(encoding="latin-1", errors="replace")
if 'ZLIB_VERSION "1.1.3"' not in zlib_h:
    die("official zlib checkout does not identify itself as 1.1.3")

# Historical include universe.
hist_includes = sorted(
    {
        p
        for p in (PS2DEV / "ps2sdk").rglob("include")
        if p.is_dir()
    }
)
hist_inc_flags: list[str] = []
for inc in hist_includes:
    hist_inc_flags += ["-I", str(inc)]

def local_include_flags(source: Path) -> list[str]:
    flags: list[str] = []
    # Prefer module-local include/ before the global historical include set.
    candidates = [
        source.parent / "include",
        source.parent.parent / "include",
        source.parent.parent.parent / "include",
    ]
    seen: set[Path] = set()
    for p in candidates:
        if p.is_dir() and p not in seen:
            flags += ["-I", str(p)]
            seen.add(p)
    flags += hist_inc_flags
    return flags

# ---------------------------------------------------------------------------
# Phase 0A: PGEN's prebuilt GSLIB 0.51 archive.
#
# This is especially valuable because it removes compiler-flag uncertainty:
# the archive stores the historical EE objects themselves.
# ---------------------------------------------------------------------------
if not goal_reached():
    print()
    print("=== Phase 0A: PGEN prebuilt GSLIB 0.51 archive ===", flush=True)
    libgs = PGEN / "lib/gslib051/lib/libgs.a"
    ar_dir = BUILD / "pgen-libgs-a"
    if libgs.exists():
        if ar_dir.exists():
            shutil.rmtree(ar_dir)
        ar_dir.mkdir(parents=True, exist_ok=True)
        ar_tool = shutil.which("ar")
        if ar_tool is None:
            compile_failures.append(
                {
                    "provenance": "pgen-libgs-a",
                    "source": str(libgs),
                    "profile": "prebuilt",
                    "error": "host ar tool not found; archive phase skipped",
                }
            )
            cp = None
        else:
            cp = run([ar_tool, "x", str(libgs)], cwd=ar_dir, timeout=30)
            if cp.returncode:
                compile_failures.append(
                    {
                        "provenance": "pgen-libgs-a",
                        "source": str(libgs),
                        "profile": "prebuilt",
                        "error": cp.stdout.strip() or "ar extraction failed",
                    }
                )
        if cp is not None and cp.returncode == 0:
            before = len(matches)
            for obj in sorted(ar_dir.glob("*.o")):
                if goal_reached():
                    break
                evaluate_object(
                    obj,
                    provenance="pgen-libgs-a",
                    source=str(libgs.relative_to(PGEN)),
                    profile="prebuilt-archive",
                    detail=PGEN_COMMIT,
                )
            print(
                f"PGEN libgs.a: +{len(matches)-before} -> checkpoint {checkpoint()}/1041",
                flush=True,
            )
    else:
        print("PGEN libgs.a not found; skipping archive pass.")

# ---------------------------------------------------------------------------
# Phase 0B: PGEN exact-era sources.
# PGEN ships the zlib/unzip/CDVD/audio-RPC families used by contemporary PS2
# homebrew and records the -O3/-fshort-double/-mlong64 release fingerprint.
# ---------------------------------------------------------------------------
if not goal_reached():
    print()
    print("=== Phase 0B: PGEN exact-era source matrix ===", flush=True)
    pgen_sources = [
        "lib/cdvd_rpc.c",
        "lib/sjpcm_rpc.c",
        "lib/amigamod_rpc.c",
        "unzip/explode.c",
        "unzip/unreduce.c",
        "unzip/unshrink.c",
        "unzip/unzip.c",
        "zlib/adler32.c",
        "zlib/compress.c",
        "zlib/crc32.c",
        "zlib/deflate.c",
        "zlib/gzio.c",
        "zlib/infblock.c",
        "zlib/infcodes.c",
        "zlib/inffast.c",
        "zlib/inflate.c",
        "zlib/inftrees.c",
        "zlib/infutil.c",
        "zlib/trees.c",
        "zlib/uncompr.c",
        "zlib/zutil.c",
    ]
    pgen_profiles = [
        ("pgen-o3", ["-O3"]),
        ("pgen-o2", ["-O2"]),
        ("pgen-os", ["-Os"]),
        ("pgen-o3-noalign", ["-O3", "-fno-align-jumps"]),
        ("pgen-o3-nosched2", ["-O3", "-fno-schedule-insns2"]),
        ("pgen-o3-nobuiltin", ["-O3", "-fno-builtin"]),
    ]
    pgen_inc = [
        "-I", str(PGEN),
        "-I", str(PGEN / "lib"),
        "-I", str(PGEN / "lib/gslib051/include"),
        "-I", str(PGEN / "unzip"),
        "-I", str(PGEN / "zlib"),
        *hist_inc_flags,
    ]
    for rel in pgen_sources:
        if goal_reached():
            break
        source = PGEN / rel
        if not source.exists():
            continue
        before = len(matches)
        for profile_name, profile_flags in pgen_profiles:
            if goal_reached():
                break
            obj = compile_source(
                source,
                provenance="pgen-403f1710",
                profile_name=profile_name,
                profile_flags=profile_flags,
                include_flags=pgen_inc,
                tag=safe(rel),
            )
            if obj is None:
                continue
            evaluate_object(
                obj,
                provenance="pgen-403f1710",
                source=rel,
                profile=profile_name,
                detail=PGEN_COMMIT,
            )
        gained = len(matches) - before
        if gained:
            print(f"   PGEN/{rel}: +{gained}; checkpoint {checkpoint()}/1041", flush=True)

# ---------------------------------------------------------------------------
# Phase 1: finish old PS2DEV per-object build contract
# ---------------------------------------------------------------------------
def parse_obj_variable(makefile: Path, var: str) -> list[str]:
    text = makefile.read_text(encoding="utf-8", errors="replace")
    lines = text.splitlines()
    pieces: list[str] = []
    collecting = False
    for line in lines:
        if not collecting:
            m = re.match(rf"^\s*{re.escape(var)}\s*=\s*(.*)$", line)
            if not m:
                continue
            rhs = m.group(1)
            collecting = True
        else:
            rhs = line
        cont = rhs.rstrip().endswith("\\")
        pieces.append(rhs.rstrip().rstrip("\\"))
        if not cont:
            break
    if not pieces:
        return []
    result = []
    for token in " ".join(pieces).split():
        if token.endswith(".o") and "$(" not in token:
            result.append(token[:-2])
    return result

PER_OBJECT_GROUPS = [
    ("libc-string", "ps2sdk/ee/libc/src/string.c", "ps2sdk/ee/libc/Makefile", "STRING_C_OBJS"),
    ("libc-stdlib", "ps2sdk/ee/libc/src/stdlib.c", "ps2sdk/ee/libc/Makefile", "STDLIB_OBJS"),
    ("libc-alloc", "ps2sdk/ee/libc/src/alloc.c", "ps2sdk/ee/libc/Makefile", "ALLOC_OBJS"),
    ("libc-xprintf", "ps2sdk/ee/libc/src/xprintf.c", "ps2sdk/ee/libc/Makefile", "XPRINTF_OBJS"),
    ("libc-xscanf", "ps2sdk/ee/libc/src/xscanf.c", "ps2sdk/ee/libc/Makefile", "XSCANF_OBJS"),
    ("libc-stdio", "ps2sdk/ee/libc/src/stdio.c", "ps2sdk/ee/libc/Makefile", "STDIO_OBJS"),
    ("libc-unistd", "ps2sdk/ee/libc/src/unistd.c", "ps2sdk/ee/libc/Makefile", "UNISTD_OBJS"),
    ("libc-time", "ps2sdk/ee/libc/src/time.c", "ps2sdk/ee/libc/Makefile", "TIME_OBJS"),
    ("libc-libgen", "ps2sdk/ee/libc/src/libgen.c", "ps2sdk/ee/libc/Makefile", "LIBGEN_OBJS"),
    ("libc-dirent", "ps2sdk/ee/libc/src/dirent.c", "ps2sdk/ee/libc/Makefile", "DIRENT_OBJS"),
    ("libc-init", "ps2sdk/ee/libc/src/init.c", "ps2sdk/ee/libc/Makefile", "INIT_OBJS"),
    ("kernel-fileio", "ps2sdk/ee/kernel/src/fileio.c", "ps2sdk/ee/kernel/Makefile", "FILEIO_OBJS"),
    ("kernel-sifcmd", "ps2sdk/ee/kernel/src/sifcmd.c", "ps2sdk/ee/kernel/Makefile", "SIFCMD_OBJS"),
    ("kernel-sifrpc", "ps2sdk/ee/kernel/src/sifrpc.c", "ps2sdk/ee/kernel/Makefile", "SIFRPC_OBJS"),
    ("kernel-loadfile", "ps2sdk/ee/kernel/src/loadfile.c", "ps2sdk/ee/kernel/Makefile", "LOADFILE_OBJS"),
    ("kernel-iopheap", "ps2sdk/ee/kernel/src/iopheap.c", "ps2sdk/ee/kernel/Makefile", "IOPHEAP_OBJS"),
    ("kernel-iopcontrol", "ps2sdk/ee/kernel/src/iopcontrol.c", "ps2sdk/ee/kernel/Makefile", "IOPCONTROL_OBJS"),
    ("kernel-config", "ps2sdk/ee/kernel/src/osd_config.c", "ps2sdk/ee/kernel/Makefile", "CONFIG_OBJS"),
    ("kernel-timer", "ps2sdk/ee/kernel/src/timer.c", "ps2sdk/ee/kernel/Makefile", "TIMER_OBJS"),
    ("kernel-getkernel", "ps2sdk/ee/kernel/src/getkernel.c", "ps2sdk/ee/kernel/Makefile", "GETKERNEL_OBJS"),
    ("kernel-glue", "ps2sdk/ee/kernel/src/glue.c", "ps2sdk/ee/kernel/Makefile", "GLUE_OBJS"),
    ("kernel-sio", "ps2sdk/ee/kernel/src/sio.c", "ps2sdk/ee/kernel/Makefile", "SIO_OBJS"),
    ("kernel-syscalls", "ps2sdk/ee/kernel/src/kernel.S", "ps2sdk/ee/kernel/Makefile", "KERNEL_OBJS"),
]

HIST_PROFILES = [
    ("hist-o2", ["-O2"]),
    ("hist-os", ["-Os"]),
    ("hist-o3", ["-O3"]),
    ("hist-o2-noalign", ["-O2", "-fno-align-jumps"]),
    ("hist-o2-nosched2", ["-O2", "-fno-schedule-insns2"]),
]

print(f"=== Progress58: {BASELINE} -> {TARGET_GOAL} strict screening ===")
print(f"base: {BASE_COMMIT[:12]} ({BASELINE}/1041)")
print(f"need: {TARGET_GOAL-BASELINE} new strict matches")
print(f"target gate: {'FORMAL original unpacked image' if formal else 'committed listings'}")
print(f"EE_CC: {EE_CC}")
print(f"PS2DEV source: {PS2DEV_COMMIT}")
print(f"zlib source: official v1.1.3 commit {ZLIB_113_COMMIT}")
print(f"PGEN source/archive: {PGEN_COMMIT}")
print()

print("=== Phase 1: finish historical PS2DEV per-object matrix ===", flush=True)
for group, source_rel, make_rel, var in PER_OBJECT_GROUPS:
    if goal_reached():
        break
    source = PS2DEV / source_rel
    makefile = PS2DEV / make_rel
    if not source.exists() or not makefile.exists():
        continue
    macros = parse_obj_variable(makefile, var)
    if not macros:
        continue

    group_before = len(matches)
    print(f"-- {group}: {len(macros)} object macros", flush=True)

    for profile_name, profile_flags in HIST_PROFILES:
        if goal_reached():
            break
        p_before = len(matches)
        for macro in macros:
            if goal_reached():
                break
            obj = compile_source(
                source,
                provenance="ps2dev-per-object",
                profile_name=profile_name,
                profile_flags=profile_flags,
                include_flags=local_include_flags(source),
                defines=[f"-DF_{macro}"],
                tag=f"{group}__{macro}",
            )
            if obj is None:
                continue
            evaluate_object(
                obj,
                provenance="ps2dev-per-object",
                source=source_rel,
                profile=profile_name,
                detail=f"{group}; F_{macro}",
            )
        gained = len(matches) - p_before
        if gained:
            print(f"   {profile_name}: +{gained}", flush=True)

    gained = len(matches) - group_before
    print(
        f"   {group}: +{gained}; checkpoint {checkpoint()}/1041",
        flush=True,
    )

# ---------------------------------------------------------------------------
# Phase 2: automatically discover normal historical PS2SDK EE translation units
# whose source mentions still-RECONSTRUCTED target function aliases.
# ---------------------------------------------------------------------------
if not goal_reached():
    print()
    print("=== Phase 2: historical PS2SDK whole-TU discovery ===", flush=True)

    remaining_aliases = sorted(
        {
            alias
            for alias, addrs in alias_to_addrs.items()
            if any(
                progress[a]["status"] == "RECONSTRUCTED" and a not in matches
                for a in addrs
            )
            and len(alias) >= 4
        },
        key=len,
        reverse=True,
    )

    shared_sources = {
        (PS2DEV / source_rel).resolve()
        for _group, source_rel, _make, _var in PER_OBJECT_GROUPS
    }

    historical_tus: list[tuple[Path, list[str]]] = []
    for source in sorted((PS2DEV / "ps2sdk/ee").rglob("*.c")):
        if source.resolve() in shared_sources:
            continue
        try:
            text = source.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        hits = [
            alias
            for alias in remaining_aliases
            if alias in text
        ]
        if not hits:
            continue
        historical_tus.append((source, hits[:12]))

    print(f"historical EE TUs mentioning remaining targets: {len(historical_tus)}")

    WHOLE_PROFILES = [
        ("hist-tu-o2", ["-O2"]),
        ("hist-tu-os", ["-Os"]),
        ("hist-tu-o3", ["-O3"]),
        ("hist-tu-o2-noalign", ["-O2", "-fno-align-jumps"]),
        ("hist-tu-o2-nosched2", ["-O2", "-fno-schedule-insns2"]),
        ("hist-tu-snesticle", ["-O2", "-ffreestanding", "-fno-builtin"]),
    ]

    for source, hits in historical_tus:
        if goal_reached():
            break
        rel = str(source.relative_to(PS2DEV))
        source_before = len(matches)
        for profile_name, profile_flags in WHOLE_PROFILES:
            if goal_reached():
                break
            obj = compile_source(
                source,
                provenance="ps2dev-whole-tu",
                profile_name=profile_name,
                profile_flags=profile_flags,
                include_flags=local_include_flags(source),
                tag=safe(rel),
            )
            if obj is None:
                continue
            evaluate_object(
                obj,
                provenance="ps2dev-whole-tu",
                source=rel,
                profile=profile_name,
                detail="source hits: " + ",".join(hits),
            )
        gained = len(matches) - source_before
        if gained:
            print(
                f"   {rel}: +{gained}; checkpoint {checkpoint()}/1041",
                flush=True,
            )

# ---------------------------------------------------------------------------
# Phase 3: compile the original zlib 1.1.3 source and MiniZip 0.15.
# ---------------------------------------------------------------------------
if not goal_reached():
    print()
    print("=== Phase 3: official zlib 1.1.3 exact-source matrix ===", flush=True)

    zlib_sources = [
        "adler32.c",
        "compress.c",
        "crc32.c",
        "deflate.c",
        "gzio.c",
        "infblock.c",
        "infcodes.c",
        "inffast.c",
        "inflate.c",
        "inftrees.c",
        "infutil.c",
        "trees.c",
        "uncompr.c",
        "zutil.c",
        "contrib/minizip/unzip.c",
    ]
    zlib_profiles = [
        ("z113-o2", ["-O2"]),
        ("z113-os", ["-Os"]),
        ("z113-o3", ["-O3"]),
        ("z113-o2-noalign", ["-O2", "-fno-align-jumps"]),
        ("z113-o2-nostrict", ["-O2", "-fno-strict-aliasing"]),
        ("z113-o2-nosched2", ["-O2", "-fno-schedule-insns2"]),
        ("z113-snesticle", ["-O2", "-ffreestanding", "-fno-builtin"]),
    ]

    zlib_inc = [
        "-I", str(ZLIB),
        "-I", str(ZLIB / "contrib/minizip"),
        *hist_inc_flags,
    ]

    for rel in zlib_sources:
        if goal_reached():
            break
        source = ZLIB / rel
        if not source.exists():
            continue
        source_before = len(matches)
        for profile_name, profile_flags in zlib_profiles:
            if goal_reached():
                break
            obj = compile_source(
                source,
                provenance="zlib-1.1.3",
                profile_name=profile_name,
                profile_flags=profile_flags,
                include_flags=zlib_inc,
                tag=safe(rel),
            )
            if obj is None:
                continue
            evaluate_object(
                obj,
                provenance="zlib-1.1.3",
                source=rel,
                profile=profile_name,
                detail=ZLIB_113_COMMIT,
            )
        gained = len(matches) - source_before
        if gained:
            print(
                f"   zlib/{rel}: +{gained}; checkpoint {checkpoint()}/1041",
                flush=True,
            )

# ---------------------------------------------------------------------------
# Phase 4: deeper fingerprinting only on recovered TUs that had near misses in
# Progress54 Screen2. This is a fallback, not the archaeology-first path.
# ---------------------------------------------------------------------------
if not goal_reached():
    print()
    print("=== Phase 4: targeted recovered-source compiler fingerprinting ===", flush=True)

    selected_sources: set[Path] = set()
    if PREV_ATTEMPTS.exists():
        with PREV_ATTEMPTS.open(newline="", encoding="utf-8") as f:
            for row in csv.DictReader(f, delimiter="\t"):
                try:
                    addr = int(row["address"], 16)
                except Exception:
                    continue
                if (
                    addr not in progress
                    or progress[addr]["status"] != "RECONSTRUCTED"
                    or addr in matches
                ):
                    continue
                result = row.get("result", "")
                diff_text = row.get("differing_bytes", "")
                try:
                    diff = int(diff_text) if diff_text else 999999
                except ValueError:
                    diff = 999999
                if result == "mismatch" and diff <= 96:
                    p = ROOT / row["source"]
                    if p.exists():
                        selected_sources.add(p)
                elif result == "boundary-reject":
                    p = ROOT / row["source"]
                    if p.exists():
                        selected_sources.add(p)

    # Always include high-yield recovered corridors even if the old attempts
    # TSV was removed.
    for pattern in (
        "src/zlib/*.c",
        "src/unzip/*.c",
        "src/ps2/*recovered.c",
    ):
        for p in ROOT.glob(pattern):
            selected_sources.add(p)

    deep_profiles = [
        ("deep-o2-nopeephole2", ["-O2", "-fno-peephole2"]),
        ("deep-o2-nopeephole", ["-O2", "-fno-peephole"]),
        ("deep-o2-nocrossjump", ["-O2", "-fno-crossjumping"]),
        ("deep-o2-nocsefollow", ["-O2", "-fno-cse-follow-jumps"]),
        ("deep-o2-noexpensive", ["-O2", "-fno-expensive-optimizations"]),
        ("deep-o2-noreruncse", ["-O2", "-fno-rerun-cse-after-loop"]),
        ("deep-o2-nostrength", ["-O2", "-fno-strength-reduce"]),
        ("deep-o2-noguess", ["-O2", "-fno-guess-branch-probability"]),
        ("deep-o2-noifconv", ["-O2", "-fno-if-conversion", "-fno-if-conversion2"]),
        ("deep-o2-noreorder", ["-O2", "-fno-reorder-blocks"]),
        ("deep-o2-nosched1", ["-O2", "-fno-schedule-insns"]),
        ("deep-o2-nosched2", ["-O2", "-fno-schedule-insns2"]),
        ("deep-o2-nosched", ["-O2", "-fno-schedule-insns", "-fno-schedule-insns2"]),
        ("deep-o2-noinline", ["-O2", "-fno-inline"]),
        ("deep-o2-noalignall", ["-O2", "-fno-align-jumps", "-fno-align-labels", "-fno-align-loops"]),
        ("deep-o1", ["-O1"]),
        ("deep-os", ["-Os"]),
        ("deep-o3", ["-O3"]),
    ]

    repo_inc = [
        "-I", str(ROOT / "include/ee_stage1_compat"),
        "-I", str(ROOT / "include"),
    ]

    print(f"targeted recovered TUs: {len(selected_sources)}")
    for source in sorted(selected_sources):
        if goal_reached():
            break
        source_before = len(matches)
        rel = str(source.relative_to(ROOT))
        for profile_name, profile_flags in deep_profiles:
            if goal_reached():
                break
            obj = compile_source(
                source,
                provenance="recovered-deep-fingerprint",
                profile_name=profile_name,
                profile_flags=profile_flags,
                include_flags=repo_inc,
                tag=safe(rel),
            )
            if obj is None:
                continue
            evaluate_object(
                obj,
                provenance="recovered-deep-fingerprint",
                source=rel,
                profile=profile_name,
            )
        gained = len(matches) - source_before
        if gained:
            print(
                f"   {rel}: +{gained}; checkpoint {checkpoint()}/1041",
                flush=True,
            )

# ---------------------------------------------------------------------------
# Persist evidence / summary
# ---------------------------------------------------------------------------
fields = [
    "address",
    "name",
    "area",
    "provenance",
    "source",
    "profile",
    "detail",
    "object",
    "object_symbol",
    "object_size",
    "boundary",
    "result",
    "differing_bytes",
    "raw_equal",
    "normalized_equal",
    "unknown_relocations",
]
with (BUILD / "matches.tsv").open("w", newline="", encoding="utf-8") as f:
    writer = csv.DictWriter(
        f,
        fieldnames=fields,
        delimiter="\t",
        lineterminator="\n",
    )
    writer.writeheader()
    for addr in sorted(matches):
        writer.writerow({k: matches[addr].get(k, "") for k in fields})

with (BUILD / "attempts.tsv").open("w", newline="", encoding="utf-8") as f:
    writer = csv.DictWriter(
        f,
        fieldnames=fields,
        delimiter="\t",
        lineterminator="\n",
    )
    writer.writeheader()
    for row in attempts:
        writer.writerow({k: row.get(k, "") for k in fields})

failure_fields = ["provenance", "source", "profile", "error"]
with (BUILD / "compile_failures.tsv").open(
    "w", newline="", encoding="utf-8"
) as f:
    writer = csv.DictWriter(
        f,
        fieldnames=failure_fields,
        delimiter="\t",
        lineterminator="\n",
    )
    writer.writeheader()
    for row in compile_failures:
        writer.writerow({k: row.get(k, "") for k in failure_fields})

area_counts = Counter(str(row["area"]) for row in matches.values())
prov_counts = Counter(str(row["provenance"]) for row in matches.values())
profile_counts = Counter(str(row["profile"]) for row in matches.values())

summary_lines = [
    "Progress58 hunt350 non-mutating screening",
    f"base_commit={BASE_COMMIT}",
    f"baseline={BASELINE}",
    f"target_gate={'formal-original' if formal else 'committed-listings'}",
    f"new_strict_matches={len(matches)}",
    f"checkpoint_if_promoted={checkpoint()}",
    f"threshold_{TARGET_GOAL}_reached={'YES' if checkpoint() >= TARGET_GOAL else 'NO'}",
    f"compile_failures={len(compile_failures)}",
    f"ps2dev_commit={PS2DEV_COMMIT}",
    f"zlib_1_1_3_commit={ZLIB_113_COMMIT}",
    f"pgen_commit={PGEN_COMMIT}",
    f"dirty_worktree={dirty_worktree}",
    f"dirty_index={dirty_index}",
    "",
    "matches_by_area:",
]
summary_lines += [f"  {k}: {v}" for k, v in area_counts.most_common()]
summary_lines += ["", "matches_by_provenance:"]
summary_lines += [f"  {k}: {v}" for k, v in prov_counts.most_common()]
summary_lines += ["", "matches_by_profile:"]
summary_lines += [f"  {k}: {v}" for k, v in profile_counts.most_common()]
(BUILD / "summary.txt").write_text(
    "\n".join(summary_lines) + "\n",
    encoding="utf-8",
)

print()
print("=== Progress58 RESULT ===")
print(f"new strict matches: {len(matches)}")
print(f"checkpoint if promoted: {checkpoint()}/1041")
print(f"{TARGET_GOAL}+ threshold reached: {'YES' if checkpoint() >= TARGET_GOAL else 'NO'}")
print()
print("Matches by provenance:")
for key, value in prov_counts.most_common():
    print(f"  {key}: {value}")
print("Matches by area:")
for key, value in area_counts.most_common():
    print(f"  {key}: {value}")
print()
print(f"compile failures recorded: {len(compile_failures)}")
print(f"full evidence: {BUILD.relative_to(ROOT)}/")

if checkpoint() >= TARGET_GOAL:
    print()
    print(f"STRICT MATCH THRESHOLD {TARGET_GOAL} REACHED — ready for reviewed promotion package")
    raise SystemExit(0)

print()
print(f"Still need {TARGET_GOAL - checkpoint()} strict matches for {TARGET_GOAL}.")
print("No project progress was modified.")
raise SystemExit(3)
