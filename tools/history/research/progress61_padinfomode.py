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

BUILD = ROOT / "build/matching/progress61-source-lineage"
OBJDIR = BUILD / "objects"
LOGDIR = BUILD / "logs"
PS2DEV = ROOT / "build/upstream/PS2DEV-bac0006c"
ZLIB = ROOT / "build/upstream/zlib-1.1.3-git"
PGEN = ROOT / "build/upstream/pgen-403f1710"
PREV_ATTEMPTS = ROOT / "build/matching/progress54-200plus-screen2/all_attempts.tsv"

for p in (BUILD, OBJDIR, LOGDIR, PS2DEV.parent, ZLIB.parent, PGEN.parent):
    p.mkdir(parents=True, exist_ok=True)

def die(msg: str) -> None:
    raise SystemExit(f"Progress61 source-lineage: {msg}")

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
# Progress61: source-lineage recovery from Progress59 byte-level near misses.
#
# NON-MUTATING.  Historical/recovered source is copied under build/, modified
# only there, compiled with the historical EE GCC and compared through the same
# relocation-precise strict gate.  No progress CSV is changed by this script.
# ---------------------------------------------------------------------------

VARIANT_DIR = BUILD / "variants"
VARIANT_DIR.mkdir(parents=True, exist_ok=True)

matches.clear()
attempts.clear()
compile_failures.clear()

FIELD_ORDER = [
    "address", "name", "area", "provenance", "source", "profile", "detail",
    "object", "object_symbol", "object_size", "boundary", "result",
    "differing_bytes", "first_differences", "difference_words",
    "raw_equal", "normalized_equal", "unknown_relocations",
]

def diff_words(elf: object, sym: object, addr: int, offsets: tuple[int, ...]) -> str:
    try:
        cand = elf.symbol_bytes(sym, sym.size)
    except Exception:
        return ""
    out: list[str] = []
    for off in offsets[:12]:
        woff = (off // 4) * 4
        if woff + 4 > len(cand) or not bytes_known(addr + woff, addr + woff + 4):
            continue
        tgt = target_slice(addr + woff, addr + woff + 4)
        got = cand[woff:woff+4]
        item = f"+0x{woff:x}:t={tgt.hex()}/c={got.hex()}"
        if item not in out:
            out.append(item)
    return ";".join(out)

def evaluate_verbose(
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
        compile_failures.append({
            "provenance": provenance, "source": source, "profile": profile,
            "error": f"ELF parse: {exc}",
        })
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
            comp = compare_function(target, addr - TARGET_BASE, sym.size, elf, sym.name)
        except Exception as exc:
            record["result"] = f"compare-error:{exc}"
            attempts.append(record)
            continue
        record["differing_bytes"] = comp.differing_bytes
        record["first_differences"] = ",".join(f"0x{x:x}" for x in comp.first_differences)
        record["difference_words"] = diff_words(elf, sym, addr, comp.first_differences)
        record["raw_equal"] = comp.raw_equal
        record["normalized_equal"] = comp.normalized_equal
        record["unknown_relocations"] = ",".join(map(str, comp.unknown_relocation_types))
        if comp.matching and not comp.unknown_relocation_types:
            record["result"] = "MATCH"
            matches[addr] = record.copy()
            print(
                f"  MATCH 0x{addr:08x} {progress[addr]['name']} "
                f"[{detail}; {profile}; {proof}]",
                flush=True,
            )
        else:
            record["result"] = "mismatch"
        attempts.append(record)
    return len(matches) - before

def write_variant(name: str, text: str, suffix: str = ".c") -> Path:
    path = VARIANT_DIR / f"{safe(name)}{suffix}"
    try:
        path.write_text(text, encoding="latin-1")
    except UnicodeEncodeError:
        path.write_text(text, encoding="utf-8")
    return path

def require_replace(text: str, old: str, new: str, label: str) -> str:
    if old not in text:
        die(f"Progress61 source-shape anchor missing ({label}): {old!r}")
    return text.replace(old, new)

def compile_and_score(
    source: Path,
    *,
    original_source: Path,
    provenance: str,
    detail: str,
    profiles: list[tuple[str, list[str]]],
    defines: list[str] | None = None,
    include_flags: list[str] | None = None,
) -> None:
    incs = include_flags if include_flags is not None else local_include_flags(original_source)
    for profile_name, profile_flags in profiles:
        obj = compile_source(
            source,
            provenance=provenance,
            profile_name=profile_name,
            profile_flags=profile_flags,
            include_flags=incs,
            defines=defines or [],
            tag=detail,
        )
        if obj is None:
            continue
        gained = evaluate_verbose(
            obj,
            provenance=provenance,
            source=str(original_source.relative_to(PS2DEV)) if original_source.is_relative_to(PS2DEV)
                   else str(original_source.relative_to(ROOT)),
            profile=profile_name,
            detail=detail,
        )
        if gained:
            print(f"   {detail}/{profile_name}: +{gained}", flush=True)

print("=== Progress61 source-lineage recovery ===")
print(f"base checkpoint: {BASELINE}/1041")
print(f"target gate: {'formal original image' if formal else 'committed listings'}")
print(f"EE_CC: {EE_CC}")
print()

OS_FIRST = [
    ("p61-os", ["-Os"]),
    ("p61-o2", ["-O2"]),
    ("p61-o3", ["-O3"]),
]

# ---------------------------------------------------------------------------
# A) libpad lineage
#
# Target facts recovered from the listing:
#   * NEW_PADMAN RPC IDs/commands (0x80000100/101, init command 0x10)
#   * old libpad.c control-flow shape (not the later libpadx retry path)
#   * pad_data frame/length/state offsets 0x58/0x60/0x70, stride 0x80
#   * PadState record size 0x14 and port stride 0x28 => [8][2]
#   * padInit clears PadState[i][0] and PadState[i][1]
#   * padRead uses signed lw for length, not lwu
# ---------------------------------------------------------------------------
pad_src = PS2DEV / "ps2sdk/ee/rpc/pad/src/libpad.c"
pad_text0 = pad_src.read_text(encoding="latin-1")

pad_variants: list[tuple[str, str]] = []

p = require_replace(pad_text0, "#define ROM_PADMAN 1", "#define NEW_PADMAN 1", "pad NEW_PADMAN")
pad_variants.append(("libpad-newpadman", p))

p2 = require_replace(p, "static struct pad_state PadState[2][8];", "static struct pad_state PadState[8][2];", "PadState 8x2")
pad_variants.append(("libpad-newpadman-8x2", p2))

p3 = p2
for old, new in [
    ("PadState[0][i].open", "PadState[i][0].open"),
    ("PadState[0][i].port", "PadState[i][0].port"),
    ("PadState[0][i].slot", "PadState[i][0].slot"),
    ("PadState[1][i].open", "PadState[i][1].open"),
    ("PadState[1][i].port", "PadState[i][1].port"),
    ("PadState[1][i].slot", "PadState[i][1].slot"),
]:
    p3 = require_replace(p3, old, new, f"padInit {old}")
pad_variants.append(("libpad-newpadman-8x2-loop", p3))

# The NEW_PADMAN/non-ROM structure has its target length at +0x60.  Keeping it
# unsigned makes GCC emit lwu; the target uses lw, so test signed int.
p4 = p3
if "unsigned int length;    // 96" in p4:
    p4 = p4.replace("unsigned int length;    // 96", "int length;             // 96")
elif "unsigned int length;" in p4:
    # Replace the second occurrence (the first lives in the inactive ROM branch).
    needle = "unsigned int length;"
    first = p4.find(needle)
    second = p4.find(needle, first + len(needle))
    if second < 0:
        die("Progress61 could not locate NEW_PADMAN length field")
    p4 = p4[:second] + "int length;" + p4[second + len(needle):]
else:
    die("Progress61 could not locate pad_data length field")
pad_variants.append(("libpad-newpadman-8x2-loop-lenint", p4))

# Progress61: the target padInfoMode has `slt v0,v0,s1` at +0xb8,
# i.e. the historical condition's operands are reversed:
#     pdata->nrOfModes < index
# rather than:
#     index < pdata->nrOfModes
# Preserve the odd historical behavior exactly and let the strict comparator
# decide whether this is the missing source lineage.
p5 = require_replace(
    p4,
    "else if (index < pdata->nrOfModes) {",
    "else if (pdata->nrOfModes < index) {",
    "padInfoMode reversed signed comparison",
)
pad_variants.append(("libpad-newpadman-8x2-loop-lenint-infomode-reversed", p5))

print("-- libpad source-lineage variants", flush=True)
for name, text in pad_variants:
    src = write_variant(name, text)
    compile_and_score(
        src,
        original_source=pad_src,
        provenance="p60-ps2dev-libpad-lineage",
        detail=name,
        profiles=OS_FIRST,
    )

# ---------------------------------------------------------------------------
# B) strncasecmp: target decrements n with addiu; size_t in the candidate emits
# daddiu under the EE long64 ABI.  Hide the historical header prototype and test
# 32-bit n spellings.
# ---------------------------------------------------------------------------
str_src = PS2DEV / "ps2sdk/ee/libc/src/string.c"
str_text0 = str_src.read_text(encoding="latin-1")
print("-- strncasecmp 32-bit count variants", flush=True)
for typename in ("unsigned int", "u32", "int"):
    s = str_text0
    s = require_replace(s, "#include <string.h>", "/* Progress61: string.h prototype hidden for 32-bit n probe */", "hide string.h")
    old_sig = "int\t strncasecmp(const char * string1, const char * string2, size_t n)"
    if old_sig not in s:
        old_sig = "int\tstrncasecmp(const char * string1, const char * string2, size_t n)"
    s = require_replace(
        s,
        old_sig,
        old_sig.replace("size_t n", f"{typename} n"),
        f"strncasecmp {typename}",
    )
    name = f"strncasecmp-n-{typename.replace(' ', '-') }"
    src = write_variant(name, s)
    compile_and_score(
        src,
        original_source=str_src,
        provenance="p60-ps2dev-libc-lineage",
        detail=name,
        profiles=[("p61-os", ["-Os"])],
        defines=["-DF_strncasecmp"],
    )

# ---------------------------------------------------------------------------
# C) FillBitBuffer: target has slti for bits_left < 25; recovered unsigned emits
# sltiu.  Make only that counter signed, with a private matching header copy.
# ---------------------------------------------------------------------------
fill_src = ROOT / "src/unzip/explode_recovered.c"
fill_text = fill_src.read_text(encoding="utf-8")
zip_hdr = ROOT / "include/legacy_zip_recovered.h"
zip_hdr_text = zip_hdr.read_text(encoding="utf-8")
zip_hdr_variant = VARIANT_DIR / "legacy_zip_recovered_p61.h"
zip_hdr_variant.write_text(
    zip_hdr_text.replace(
        "extern unsigned g_legacy_zip_bits_left;",
        "extern int g_legacy_zip_bits_left;",
    ),
    encoding="utf-8",
)
fill_text = require_replace(
    fill_text,
    '#include "../../include/legacy_zip_recovered.h"',
    '#include "legacy_zip_recovered_p61.h"',
    "FillBitBuffer private header",
)
if "unsigned g_legacy_zip_bits_left;" in fill_text:
    fill_text = require_replace(
        fill_text,
        "unsigned g_legacy_zip_bits_left;",
        "int g_legacy_zip_bits_left;",
        "signed bits_left",
    )
elif "int g_legacy_zip_bits_left;" not in fill_text:
    die("Progress61 could not establish signed bits_left source shape")
print("-- FillBitBuffer signed counter", flush=True)
fill_variant = write_variant("explode-bits-left-signed", fill_text)
compile_and_score(
    fill_variant,
    original_source=fill_src,
    provenance="p60-recovered-unzip-lineage",
    detail="FillBitBuffer-bits-left-signed",
    profiles=[("p61-os", ["-Os"])],
    include_flags=[
        "-I", str(VARIANT_DIR),
        "-I", str(ROOT / "include/ee_stage1_compat"),
        "-I", str(ROOT / "include"),
        *hist_inc_flags,
    ],
)

# ---------------------------------------------------------------------------
# D) mcRename: the target literally passes 0x20 to strncpy(newName) and loads
# mcRpcCmd entry +0x38 (index 14), while the historical near-match uses 31 and
# MC_RPCCMD_SET_INFO (index 10/+0x28).  Test both quirks independently and
# together; the combined variant is the strongest exact-source candidate.
# ---------------------------------------------------------------------------
mc_src = PS2DEV / "ps2sdk/ee/rpc/memorycard/src/libmc.c"
mc_text0 = mc_src.read_text(encoding="latin-1")
print("-- mcRename historical quirks", flush=True)
mc_variants: list[tuple[str, str]] = []
mc31 = "strncpy(g_fileInfoBuff.name, newName, 31);"
mc32 = "strncpy(g_fileInfoBuff.name, newName, 32);"
call_set = "mcRpcCmd[g_mcType][MC_RPCCMD_SET_INFO]"
call_ent = "mcRpcCmd[g_mcType][MC_RPCCMD_GET_ENT]"

m1 = require_replace(mc_text0, mc31, mc32, "mcRename strncpy 32")
mc_variants.append(("libmc-rename-copy32", m1))
# Replace only the SET_INFO occurrence inside mcRename: use a suffix split at
# the function declaration so mcSetFileInfo remains untouched.
anchor = "int mcRename(int port, int slot, const char* oldName, const char* newName)"
if anchor not in mc_text0:
    die("Progress61 could not locate historical mcRename")
head, tail = mc_text0.split(anchor, 1)
if call_set not in tail:
    die("Progress61 could not locate mcRename RPC selector")
m2 = head + anchor + tail.replace(call_set, call_ent, 1)
mc_variants.append(("libmc-rename-getent-rpc", m2))
head, tail = m1.split(anchor, 1)
m3 = head + anchor + tail.replace(call_set, call_ent, 1)
mc_variants.append(("libmc-rename-copy32-getent-rpc", m3))

for name, text in mc_variants:
    src = write_variant(name, text)
    compile_and_score(
        src,
        original_source=mc_src,
        provenance="p60-ps2dev-libmc-lineage",
        detail=name,
        profiles=[("p61-os", ["-Os"])],
    )

# ---------------------------------------------------------------------------
# E) fioMkdir: every Progress59 difference is a stack/save offset shifted by
# 0x10.  The reconstruction enlarged the local union to path[257] only to make
# the target's path[0x100] terminator legal C.  The target frame is consistent
# with the historical 0x100-byte local payload and an intentional one-byte
# out-of-bounds terminator write, so test path[256] exactly.
# ---------------------------------------------------------------------------
fio_src = ROOT / "src/ps2/fileio_recovered.c"
fio_text = fio_src.read_text(encoding="utf-8")
if "char path[257]; /* target writes a terminator one byte beyond RPC payload */" in fio_text:
    fio_text = require_replace(
        fio_text,
        "char path[257]; /* target writes a terminator one byte beyond RPC payload */",
        "char path[256]; /* Progress61 exact-shape probe: target writes path[256] */",
        "fioMkdir local size",
    )
elif "char path[256];" not in fio_text:
    die("Progress61 could not establish fioMkdir 0x100-byte local frame")
print("-- fioMkdir 0x100-byte local payload", flush=True)
fio_variant = write_variant("fileio-fioMkdir-path256", fio_text)
compile_and_score(
    fio_variant,
    original_source=fio_src,
    provenance="p60-recovered-fileio-lineage",
    detail="fioMkdir-path256",
    profiles=[("p61-os", ["-Os"]), ("p61-o2", ["-O2"])],
    include_flags=[
        "-I", str(ROOT / "include/ee_stage1_compat"),
        "-I", str(ROOT / "include"),
        "-I", str(ROOT / "matching/ee_abi_compat"),
        *hist_inc_flags,
    ],
)

# ---------------------------------------------------------------------------
# Freeze evidence.
# ---------------------------------------------------------------------------
def write_tsv(path: Path, rows: list[dict[str, object]]) -> None:
    extras = sorted({k for row in rows for k in row if k not in FIELD_ORDER})
    fields = FIELD_ORDER + extras
    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(
            f, fieldnames=fields, delimiter="\t", lineterminator="\n",
            extrasaction="ignore",
        )
        writer.writeheader()
        writer.writerows(rows)

all_attempts = sorted(
    attempts,
    key=lambda r: (
        int(str(r.get("address", "0x0")), 16),
        str(r.get("detail", "")),
        str(r.get("profile", "")),
    ),
)
write_tsv(BUILD / "attempts.tsv", all_attempts)
write_tsv(BUILD / "matches.tsv", [matches[a] for a in sorted(matches)])

best_by_addr: dict[int, dict[str, object]] = {}
for row in attempts:
    if row.get("result") != "mismatch":
        continue
    try:
        addr = int(str(row["address"]), 16)
        diff = int(row.get("differing_bytes", 10**9))
    except (ValueError, TypeError, KeyError):
        continue
    old = best_by_addr.get(addr)
    if old is None or diff < int(old.get("differing_bytes", 10**9)):
        best_by_addr[addr] = row.copy()
best_rows = sorted(
    best_by_addr.values(),
    key=lambda r: (int(r.get("differing_bytes", 10**9)), int(str(r.get("address", "0x0")), 16)),
)
write_tsv(BUILD / "best.tsv", best_rows)

with (BUILD / "compile_failures.tsv").open("w", newline="", encoding="utf-8") as f:
    fields = ["provenance", "source", "profile", "error"]
    writer = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(compile_failures)

lines = [
    "Progress61 source-lineage recovery",
    f"base_commit={BASE_COMMIT}",
    f"baseline={BASELINE}",
    f"target_gate={'formal-original' if formal else 'committed-listings'}",
    f"new_strict_matches={len(matches)}",
    f"checkpoint_if_promoted={BASELINE + len(matches)}",
    f"remaining_to_350={max(0, 350 - (BASELINE + len(matches)))}",
    f"compile_failures={len(compile_failures)}",
    f"dirty_worktree={dirty_worktree}",
    f"dirty_index={dirty_index}",
    "",
    "matches:",
]
for addr in sorted(matches):
    r = matches[addr]
    lines.append(
        f"  {r['address']} {r['name']} detail={r['detail']} profile={r['profile']} "
        f"source={r['source']} boundary={r['boundary']}"
    )
lines += ["", "best_remaining_near_misses:"]
for r in best_rows[:40]:
    lines.append(
        f"  diff={r.get('differing_bytes')} {r.get('address')} {r.get('name')} "
        f"detail={r.get('detail')} profile={r.get('profile')} "
        f"first={r.get('first_differences')} words={r.get('difference_words')}"
    )
(BUILD / "summary.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")

print()
print("=== Progress61 RESULT ===")
print(f"new strict matches: {len(matches)}")
print(f"checkpoint if promoted: {BASELINE + len(matches)}/1041")
print(f"still needed for 350: {max(0, 350 - (BASELINE + len(matches)))}")
print(f"compile failures: {len(compile_failures)}")
print(f"evidence: {BUILD.relative_to(ROOT)}/")
if matches:
    print("Strict matches found:")
    for addr in sorted(matches):
        r = matches[addr]
        print(f"  {r['address']} {r['name']} [{r['detail']}; {r['profile']}]")
else:
    print("No strict matches; inspect best.tsv and compile_failures.tsv.")
print()
print("Best remaining candidates:")
for r in best_rows[:20]:
    print(
        f"  diff={r.get('differing_bytes')} {r.get('address')} {r.get('name')} "
        f"[{r.get('detail')}; {r.get('profile')}] first={r.get('first_differences')}"
    )

raise SystemExit(0)
