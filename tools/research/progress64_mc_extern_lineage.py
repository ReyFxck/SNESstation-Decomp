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

BUILD = ROOT / "build/matching/progress64-mc-extern-lineage"
OBJDIR = BUILD / "objects"
LOGDIR = BUILD / "logs"
PS2DEV = ROOT / "build/upstream/PS2DEV-bac0006c"
ZLIB = ROOT / "build/upstream/zlib-1.1.3-git"
PGEN = ROOT / "build/upstream/pgen-403f1710"
PREV_ATTEMPTS = ROOT / "build/matching/progress54-200plus-screen2/all_attempts.tsv"

for p in (BUILD, OBJDIR, LOGDIR, PS2DEV.parent, ZLIB.parent, PGEN.parent):
    p.mkdir(parents=True, exist_ok=True)

def die(msg: str) -> None:
    raise SystemExit(f"Progress64 mc-extern-lineage: {msg}")

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
spec = importlib.util.spec_from_file_location("cmp_p64", cmp_path)
if spec is None or spec.loader is None:
    die("cannot import tools/compare_elf_functions.py")
cmpmod = importlib.util.module_from_spec(spec)
sys.modules["cmp_p64"] = cmpmod
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
# Progress64: mcFormat/mcUnformat external-object source-lineage probe.
#
# NON-MUTATING.  Progress63 showed that merely removing `static` is not enough:
# the globals are still defined in the same translation unit, so GCC can keep
# section-relative knowledge and generate the same code.  The target instead
# materializes g_descParam as an independent symbol base early in both
# mcFormat/mcUnformat.  This probe changes selected object definitions to
# `extern` declarations, as if their storage lived in another translation unit.
#
# The strict relocation-aware comparator remains authoritative.
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

PAIR_ADDRS = (0x001A1554, 0x001A1610)

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

def write_variant(name: str, text: str) -> Path:
    path = VARIANT_DIR / f"{safe(name)}.c"
    path.write_text(text, encoding="latin-1")
    return path

def compile_and_score(
    source: Path,
    *,
    original_source: Path,
    provenance: str,
    detail: str,
    profiles: list[tuple[str, list[str]]],
) -> None:
    incs = local_include_flags(original_source)
    for profile_name, profile_flags in profiles:
        before_attempts = len(attempts)
        obj = compile_source(
            source,
            provenance=provenance,
            profile_name=profile_name,
            profile_flags=profile_flags,
            include_flags=incs,
            defines=[],
            tag=detail,
        )
        if obj is None:
            continue
        gained = evaluate_verbose(
            obj,
            provenance=provenance,
            source=str(original_source.relative_to(PS2DEV)),
            profile=profile_name,
            detail=detail,
        )
        new_rows = attempts[before_attempts:]
        pair = {
            int(str(r["address"]), 16): r
            for r in new_rows
            if r.get("result") in ("mismatch", "MATCH")
            and int(str(r["address"]), 16) in PAIR_ADDRS
        }
        vals = []
        for addr in PAIR_ADDRS:
            row = pair.get(addr)
            if row is None:
                vals.append(f"0x{addr:08x}=n/a")
            elif row.get("result") == "MATCH":
                vals.append(f"0x{addr:08x}=MATCH")
            else:
                vals.append(f"0x{addr:08x}=diff{row.get('differing_bytes')}")
        print(f"   PAIR {detail}/{profile_name}: " + " ".join(vals), flush=True)
        if gained:
            print(f"   {detail}/{profile_name}: +{gained}", flush=True)

def replace_one_regex(text: str, pattern: str, repl, label: str) -> str:
    out, n = re.subn(pattern, repl, text, count=1, flags=re.S)
    if n != 1:
        die(f"could not externize {label}; pattern matched {n} times")
    return out

def externize(text: str, symbol: str) -> str:
    if symbol == "g_descParam":
        return replace_one_regex(
            text,
            r"static(\s+struct\s*\{.*?\}\s+g_descParam\s+__attribute__\(\(aligned\(64\)\)\);)",
            r"extern\1",
            symbol,
        )
    if symbol == "g_nameParam":
        return replace_one_regex(
            text,
            r"static(\s+struct\s*\{.*?\}\s+g_nameParam\s+__attribute__\(\(aligned\(64\)\)\);)",
            r"extern\1",
            symbol,
        )
    exact = {
        "g_cdata": (
            r"static(\s+SifRpcClientData_t\s+g_cdata\s+__attribute__\(\(aligned\(64\)\)\);)",
            r"extern\1",
        ),
        "g_rdata": (
            r"static(\s+u8\s+g_rdata\[RSIZE\]\s+__attribute__\(\(aligned\(64\)\)\);)",
            r"extern\1",
        ),
        "g_mclibInited": (
            r"static\s+int\s+g_mclibInited\s*=\s*0\s*;",
            "extern int g_mclibInited;",
        ),
        "g_currentCmd": (
            r"static\s+unsigned\s+int\s+g_currentCmd\s*=\s*0\s*;",
            "extern unsigned int g_currentCmd;",
        ),
        "g_mcType": (
            r"static\s+int\s+g_mcType\s*=\s*MC_TYPE_MC\s*;",
            "extern int g_mcType;",
        ),
    }
    pattern, repl = exact[symbol]
    return replace_one_regex(text, pattern, repl, symbol)

def externize_many(text: str, symbols: tuple[str, ...]) -> str:
    for symbol in symbols:
        text = externize(text, symbol)
    return text

print("=== Progress64 mc external-object source-lineage ===")
print(f"base checkpoint: {BASELINE}/1041")
print(f"target gate: {'formal original image' if formal else 'committed listings'}")
print(f"EE_CC: {EE_CC}")
print()

mc_src = PS2DEV / "ps2sdk/ee/rpc/memorycard/src/libmc.c"
mc_text = mc_src.read_text(encoding="latin-1")

# The target pair is a p61/p63 -Os near miss.  Keep the profile focused:
# changing source linkage is the experiment, not farming compiler flags.
PROFILES = [("p64-os", ["-Os"])]

variants: list[tuple[str, tuple[str, ...]]] = [
    ("mc-extern-desc", ("g_descParam",)),
    ("mc-extern-desc-cdata", ("g_descParam", "g_cdata")),
    ("mc-extern-desc-rdata", ("g_descParam", "g_rdata")),
    ("mc-extern-desc-cdata-rdata", ("g_descParam", "g_cdata", "g_rdata")),
    (
        "mc-extern-desc-cdata-rdata-state",
        (
            "g_descParam", "g_cdata", "g_rdata",
            "g_mclibInited", "g_currentCmd", "g_mcType",
        ),
    ),
    (
        "mc-extern-rpc-buffers",
        ("g_nameParam", "g_descParam", "g_cdata", "g_rdata"),
    ),
    (
        "mc-extern-rpc-buffers-state",
        (
            "g_nameParam", "g_descParam", "g_cdata", "g_rdata",
            "g_mclibInited", "g_currentCmd", "g_mcType",
        ),
    ),
]

print("-- external-object linkage variants", flush=True)
for name, symbols in variants:
    text = externize_many(mc_text, symbols)
    src = write_variant(name, text)
    compile_and_score(
        src,
        original_source=mc_src,
        provenance="p64-ps2dev-libmc-extern-lineage",
        detail=name,
        profiles=PROFILES,
    )

# ---------------------------------------------------------------------------
# Evidence
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

# Pair matrix: one line per linkage variant, directly comparable.
pair_rows = [
    r for r in attempts
    if int(str(r.get("address", "0x0")), 16) in PAIR_ADDRS
    and r.get("result") in ("mismatch", "MATCH")
]
write_tsv(BUILD / "pair_matrix.tsv", pair_rows)

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
    key=lambda r: (
        int(r.get("differing_bytes", 10**9)),
        int(str(r.get("address", "0x0")), 16),
    ),
)
write_tsv(BUILD / "best.tsv", best_rows)

with (BUILD / "compile_failures.tsv").open("w", newline="", encoding="utf-8") as f:
    fields = ["provenance", "source", "profile", "error"]
    writer = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(compile_failures)

lines = [
    "Progress64 mc external-object source-lineage",
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
    "pair_matrix:",
]
for detail, _symbols in variants:
    rows = [r for r in pair_rows if r.get("detail") == detail]
    by_addr = {int(str(r["address"]), 16): r for r in rows}
    vals = []
    for addr in PAIR_ADDRS:
        r = by_addr.get(addr)
        if r is None:
            vals.append(f"0x{addr:08x}=n/a")
        elif r.get("result") == "MATCH":
            vals.append(f"0x{addr:08x}=MATCH")
        else:
            vals.append(
                f"0x{addr:08x}=diff{r.get('differing_bytes')}"
                f"[{r.get('first_differences')}]"
            )
    lines.append(f"  {detail}: " + " ".join(vals))

lines += ["", "matches:"]
for addr in sorted(matches):
    r = matches[addr]
    lines.append(
        f"  {r['address']} {r['name']} detail={r['detail']} "
        f"profile={r['profile']} boundary={r['boundary']}"
    )
lines += ["", "best_remaining_near_misses:"]
for r in best_rows[:30]:
    lines.append(
        f"  diff={r.get('differing_bytes')} {r.get('address')} {r.get('name')} "
        f"detail={r.get('detail')} profile={r.get('profile')} "
        f"first={r.get('first_differences')} words={r.get('difference_words')}"
    )
(BUILD / "summary.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")

print()
print("=== Progress64 RESULT ===")
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
    print("No strict match yet; pair_matrix.tsv shows which linkage shape moved the bytes.")

print()
print("Pair matrix:")
for detail, _symbols in variants:
    rows = [r for r in pair_rows if r.get("detail") == detail]
    by_addr = {int(str(r["address"]), 16): r for r in rows}
    vals = []
    for addr in PAIR_ADDRS:
        r = by_addr.get(addr)
        if r is None:
            vals.append(f"0x{addr:08x}=n/a")
        elif r.get("result") == "MATCH":
            vals.append(f"0x{addr:08x}=MATCH")
        else:
            vals.append(f"0x{addr:08x}=diff{r.get('differing_bytes')}")
    print(f"  {detail}: " + " ".join(vals))
