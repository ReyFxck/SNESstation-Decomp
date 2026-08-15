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
BASE_COMMIT = "ffdfc8e946cd29c0331aa0c27888ae38f6ab7a04"
BASELINE = 102
PRIOR_REQUIRED = 82
NEEDED_TOTAL_NEW = 98
OLD_REPO = "https://github.com/duduclx/PS2DEV.git"
OLD_COMMIT = "bac0006c6302edcf1bdae253799484497b4e5032"
TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

BUILD = ROOT / "build/matching/progress54-historical-per-object"
OBJDIR = BUILD / "objects"
LOGDIR = BUILD / "logs"
UPSTREAM = ROOT / "build/upstream/PS2DEV-bac0006c"
PRIOR = ROOT / "build/matching/progress54-200plus-screen2/matches.tsv"

for p in (BUILD, OBJDIR, LOGDIR, UPSTREAM.parent):
    p.mkdir(parents=True, exist_ok=True)

def die(msg: str) -> None:
    raise SystemExit(f"Progress54 historical-per-object: {msg}")

def run(cmd: list[str], cwd: Path = ROOT, timeout: int | None = None) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        text=True, timeout=timeout
    )

head = run(["git", "rev-parse", "HEAD"]).stdout.strip()
if head != BASE_COMMIT:
    die(f"expected committed 102-MATCH main at {BASE_COMMIT}; found {head}")
if run(["git", "diff", "--quiet"]).returncode:
    die("tracked working tree is not clean")
if run(["git", "diff", "--cached", "--quiet"]).returncode:
    die("index has staged changes")
if not PRIOR.exists():
    die(f"prior Screen2 evidence missing: {PRIOR}")

with PRIOR.open(newline="", encoding="utf-8") as f:
    prior_rows = list(csv.DictReader(f, delimiter="\t"))
prior_matches = {
    int(r["address"], 16): r
    for r in prior_rows
    if r.get("result") == "MATCH" or not r.get("result")
}
if len(prior_matches) != PRIOR_REQUIRED:
    die(f"expected {PRIOR_REQUIRED} strict matches from Screen2; found {len(prior_matches)}")

# Fetch only the historical source families needed for the original per-object
# build contract. Everything remains ignored under build/upstream.
if not (UPSTREAM / ".git").exists():
    print("Fetching historical PS2DEV source snapshot (sparse, build/upstream only) ...")
    UPSTREAM.mkdir(parents=True, exist_ok=True)
    cp = run(["git", "init", "-q", str(UPSTREAM)])
    if cp.returncode:
        die(cp.stdout)
    cp = run(["git", "-C", str(UPSTREAM), "remote", "add", "origin", OLD_REPO])
    if cp.returncode:
        die(cp.stdout)
    cp = run(["git", "-C", str(UPSTREAM), "config", "core.sparseCheckout", "true"])
    if cp.returncode:
        die(cp.stdout)
    sparse = UPSTREAM / ".git/info/sparse-checkout"
    sparse.write_text(
        "ps2sdk/ee/\n"
        "ps2sdk/common/\n",
        encoding="utf-8",
    )
    cp = run([
        "git", "-C", str(UPSTREAM), "fetch", "-q", "--depth=1",
        "origin", OLD_COMMIT
    ], timeout=180)
    if cp.returncode:
        die("historical PS2DEV fetch failed:\n" + cp.stdout)
    cp = run(["git", "-C", str(UPSTREAM), "checkout", "-q", "--detach", "FETCH_HEAD"])
    if cp.returncode:
        die("historical PS2DEV checkout failed:\n" + cp.stdout)
else:
    got = run(["git", "-C", str(UPSTREAM), "rev-parse", "HEAD"]).stdout.strip()
    if got != OLD_COMMIT:
        cp = run([
            "git", "-C", str(UPSTREAM), "fetch", "-q", "--depth=1",
            "origin", OLD_COMMIT
        ], timeout=180)
        if cp.returncode:
            die(cp.stdout)
        cp = run(["git", "-C", str(UPSTREAM), "checkout", "-q", "--detach", "FETCH_HEAD"])
        if cp.returncode:
            die(cp.stdout)

got = run(["git", "-C", str(UPSTREAM), "rev-parse", "HEAD"]).stdout.strip()
if got != OLD_COMMIT:
    die(f"historical source commit mismatch: {got}")

# Comparator from repo.
cmp_path = ROOT / "tools/compare_elf_functions.py"
spec = importlib.util.spec_from_file_location("cmpmod_hist", cmp_path)
if spec is None or spec.loader is None:
    die("cannot import comparator")
cmpmod = importlib.util.module_from_spec(spec)
sys.modules["cmpmod_hist"] = cmpmod
spec.loader.exec_module(cmpmod)
ELFFile = cmpmod.ELFFile
compare_function = cmpmod.compare_function

# Canonical target universe.
with (ROOT / "analysis/progress_targets.csv").open(newline="", encoding="utf-8") as f:
    prog_rows = list(csv.DictReader(f))
progress = {int(r["address"], 16): r for r in prog_rows}
name_to_addr: dict[str, int] = {}
dupe_names: set[str] = set()
for a, r in progress.items():
    n = r["name"]
    if n in name_to_addr:
        dupe_names.add(n)
    else:
        name_to_addr[n] = a
for n in dupe_names:
    name_to_addr.pop(n, None)
starts = sorted(progress)
next_start = {
    a: starts[i + 1] if i + 1 < len(starts) else None
    for i, a in enumerate(starts)
}

# Target bytes: formal if present, otherwise same committed-listing sparse gate
# that produced the 82 Screen2 matches.
reference = ROOT / "build/SNES_EMU.unpacked.bin"
formal = False
if reference.exists() and hashlib.sha256(reference.read_bytes()).hexdigest() == TARGET_SHA256:
    target = reference.read_bytes()
    known = None
    formal = True
else:
    insn_re = re.compile(
        r"^\s*([0-9A-Fa-f]+):\s+"
        r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
        r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
    )
    bm: dict[int, int] = {}
    conflicts: set[int] = set()
    for listing in sorted((ROOT / "analysis/functions").glob("*.asm")):
        for line in listing.read_text(encoding="utf-8", errors="replace").splitlines():
            m = insn_re.match(line)
            if not m:
                continue
            a = int(m.group(1), 16)
            bs = bytes(int(m.group(i), 16) for i in range(2, 6))
            for i, b in enumerate(bs):
                p = a + i
                if p in bm and bm[p] != b:
                    conflicts.add(p)
                else:
                    bm[p] = b
    for p in conflicts:
        bm.pop(p, None)
    end = max(bm) + 1
    tb = bytearray(end - TARGET_BASE)
    for a, b in bm.items():
        if a >= TARGET_BASE:
            tb[a - TARGET_BASE] = b
    target = bytes(tb)
    known = set(bm)

def bytes_known(start: int, end: int) -> bool:
    if formal:
        return TARGET_BASE <= start <= end <= TARGET_BASE + len(target)
    assert known is not None
    return all(a in known for a in range(start, end))

def target_slice(start: int, end: int) -> bytes:
    return target[start - TARGET_BASE:end - TARGET_BASE]

# Historical EE compiler.
cc = Path(os.environ.get(
    "EE_CC",
    str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
))
if not cc.exists() or not os.access(cc, os.X_OK):
    die(f"EE compiler unavailable: {cc}")

# Historical headers. Use the old PS2SDK tree rather than modern headers.
include_dirs = sorted({
    p for p in (UPSTREAM / "ps2sdk").rglob("include")
    if p.is_dir()
})
if not include_dirs:
    die("historical PS2SDK include directories not found after sparse checkout")

INC_FLAGS: list[str] = []
for d in include_dirs:
    INC_FLAGS.extend(["-I", str(d)])

COMMON = [
    "-G0", "-EL", "-pipe", "-w",
    "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common",
    "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls",
    "-march=r5900", "-mtune=r5900",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD", "-DCODE_PLATFORM=3",
    *INC_FLAGS,
]

# The historical makefiles compiled these shared source files once per object
# with -DF_<object-basename>. Reproduce that contract instead of compiling a
# monolithic recovered TU.
GROUPS = [
    # Highest priority: only 16 more are needed.
    ("libc-string", "ps2sdk/ee/libc/src/string.c",
     "ps2sdk/ee/libc/Makefile", "STRING_C_OBJS"),
    ("libc-stdlib", "ps2sdk/ee/libc/src/stdlib.c",
     "ps2sdk/ee/libc/Makefile", "STDLIB_OBJS"),
    ("libc-alloc", "ps2sdk/ee/libc/src/alloc.c",
     "ps2sdk/ee/libc/Makefile", "ALLOC_OBJS"),
    ("libc-xprintf", "ps2sdk/ee/libc/src/xprintf.c",
     "ps2sdk/ee/libc/Makefile", "XPRINTF_OBJS"),
    ("kernel-fileio", "ps2sdk/ee/kernel/src/fileio.c",
     "ps2sdk/ee/kernel/Makefile", "FILEIO_OBJS"),
    ("kernel-sifcmd", "ps2sdk/ee/kernel/src/sifcmd.c",
     "ps2sdk/ee/kernel/Makefile", "SIFCMD_OBJS"),
    ("kernel-sifrpc", "ps2sdk/ee/kernel/src/sifrpc.c",
     "ps2sdk/ee/kernel/Makefile", "SIFRPC_OBJS"),
    ("kernel-loadfile", "ps2sdk/ee/kernel/src/loadfile.c",
     "ps2sdk/ee/kernel/Makefile", "LOADFILE_OBJS"),
    ("kernel-iopheap", "ps2sdk/ee/kernel/src/iopheap.c",
     "ps2sdk/ee/kernel/Makefile", "IOPHEAP_OBJS"),
    ("kernel-iopcontrol", "ps2sdk/ee/kernel/src/iopcontrol.c",
     "ps2sdk/ee/kernel/Makefile", "IOPCONTROL_OBJS"),
    ("kernel-glue", "ps2sdk/ee/kernel/src/glue.c",
     "ps2sdk/ee/kernel/Makefile", "GLUE_OBJS"),
    ("kernel-syscalls", "ps2sdk/ee/kernel/src/kernel.S",
     "ps2sdk/ee/kernel/Makefile", "KERNEL_OBJS"),
]

PROFILES = [
    ("hist-o2", ["-O2"]),
    ("hist-os", ["-Os"]),
    ("hist-o2-nobuiltin", ["-O2", "-fno-builtin"]),
    ("hist-o2-noalign", ["-O2", "-fno-align-jumps"]),
]

def parse_obj_variable(makefile: Path, var: str) -> list[str]:
    text = makefile.read_text(encoding="utf-8", errors="replace")
    lines = text.splitlines()
    acc = []
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
        rhs = rhs.rstrip().rstrip("\\")
        acc.append(rhs)
        if not cont:
            break
    if not acc:
        return []
    joined = " ".join(acc)
    objs = []
    for token in joined.split():
        token = token.strip()
        if token.endswith(".o") and "$(" not in token:
            objs.append(token[:-2])
    return objs

def terminal_return_proof(addr: int, size: int) -> bool:
    if size < 8 or not bytes_known(addr, addr + size):
        return False
    return target_slice(addr + size - 8, addr + size)[:4] == bytes.fromhex("0800e003")

def boundary_ok(addr: int, size: int) -> tuple[bool, str]:
    nxt = next_start.get(addr)
    if nxt is None or size <= 0:
        return False, "no-next"
    span = nxt - addr
    if size > span:
        return False, f"oversize:+0x{size-span:x}"
    if size == span:
        return True, "exact-next-boundary"
    gap = span - size
    if 0 < gap <= 0x3C and bytes_known(addr + size, nxt):
        g = target_slice(addr + size, nxt)
        if not any(g) and terminal_return_proof(addr, size):
            return True, f"jr-ra+nop-padding:0x{gap:x}"
    return False, f"unproven-gap:0x{gap:x}"

@dataclass
class Result:
    addr: int
    name: str
    group: str
    macro: str
    profile: str
    object_symbol: str
    object_size: int
    boundary: str
    differing_bytes: int

new_matches: dict[int, Result] = {}
attempt_rows: list[dict[str, object]] = []
compile_failures: list[tuple[str, str, str, str]] = []

def combined_count() -> int:
    return len(set(prior_matches) | set(new_matches))

def threshold_reached() -> bool:
    return combined_count() >= NEEDED_TOTAL_NEW

def map_symbol(name: str) -> int | None:
    # Exact canonical target name is the primary mapping.
    if name in name_to_addr:
        return name_to_addr[name]
    # Historical source occasionally prefixes one leading underscore.
    if name.startswith("_") and name[1:] in name_to_addr:
        return name_to_addr[name[1:]]
    return None

def compile_object(
    source: Path, macro: str, group: str, objbase: str,
    profile: str, pflags: list[str]
) -> Path | None:
    ext = ".S" if source.suffix == ".S" else ".c"
    obj = OBJDIR / f"{group}__{objbase}__{profile}.o"
    log = LOGDIR / f"{group}__{objbase}__{profile}.log"
    cmd = [
        str(cc), *COMMON, *pflags, f"-DF_{macro}",
        "-c", str(source), "-o", str(obj)
    ]
    cp = run(cmd, timeout=60)
    log.write_text(cp.stdout, encoding="utf-8")
    if cp.returncode:
        compile_failures.append((group, objbase, profile, cp.stdout.splitlines()[-1] if cp.stdout.splitlines() else "compile-fail"))
        return None
    return obj

print("=== Progress54 historical per-object source gate ===")
print(f"historical commit: {OLD_COMMIT}")
print(f"target gate: {'FORMAL original unpacked image' if formal else 'committed listings fallback'}")
print(f"prior strict Screen2 matches: {len(prior_matches)}")
print(f"needed additional beyond Screen2: {NEEDED_TOTAL_NEW - len(prior_matches)}")
print(f"EE_CC: {cc}")
print()

for group, source_rel, make_rel, variable in GROUPS:
    if threshold_reached():
        break
    source = UPSTREAM / source_rel
    makefile = UPSTREAM / make_rel
    if not source.exists() or not makefile.exists():
        print(f"SKIP {group}: historical source/makefile missing")
        continue
    macros = parse_obj_variable(makefile, variable)
    if not macros:
        print(f"SKIP {group}: could not parse {variable}")
        continue

    before_group = len(new_matches)
    print(f"-- {group}: {len(macros)} historical per-object macros")

    # O2 first for every object. Only if still below 200 do alternative profiles.
    for profile, pflags in PROFILES:
        if threshold_reached():
            break
        before_profile = len(new_matches)
        for macro in macros:
            if threshold_reached():
                break
            obj = compile_object(source, macro, group, macro, profile, pflags)
            if obj is None:
                continue
            try:
                elf = ELFFile(obj)
            except Exception as e:
                compile_failures.append((group, macro, profile, f"ELF parse: {e}"))
                continue

            for sym in elf.symbols:
                if sym.section_index == 0 or sym.size <= 0 or (sym.info & 0xF) != 2:
                    continue
                addr = map_symbol(sym.name)
                if addr is None:
                    continue
                if progress[addr]["status"] != "RECONSTRUCTED":
                    continue
                if addr in prior_matches or addr in new_matches:
                    continue
                rec = {
                    "address": f"0x{addr:08x}",
                    "name": progress[addr]["name"],
                    "group": group,
                    "macro": macro,
                    "profile": profile,
                    "object_symbol": sym.name,
                    "object_size": sym.size,
                }
                if not bytes_known(addr, addr + sym.size):
                    rec["result"] = "target-bytes-missing"
                    attempt_rows.append(rec)
                    continue
                ok, proof = boundary_ok(addr, sym.size)
                rec["boundary"] = proof
                if not ok:
                    rec["result"] = "boundary-reject"
                    attempt_rows.append(rec)
                    continue
                try:
                    comp = compare_function(
                        target, addr - TARGET_BASE, sym.size, elf, sym.name
                    )
                except Exception as e:
                    rec["result"] = f"compare-error:{e}"
                    attempt_rows.append(rec)
                    continue
                rec["differing_bytes"] = comp.differing_bytes
                rec["unknown_relocations"] = ",".join(map(str, comp.unknown_relocation_types))
                if comp.matching and not comp.unknown_relocation_types:
                    rec["result"] = "MATCH"
                    new_matches[addr] = Result(
                        addr, progress[addr]["name"], group, macro, profile,
                        sym.name, sym.size, proof, comp.differing_bytes
                    )
                    print(
                        f"  MATCH 0x{addr:08x} {progress[addr]['name']} "
                        f"[{profile}; F_{macro}; {proof}]"
                    )
                else:
                    rec["result"] = "mismatch"
                attempt_rows.append(rec)

        got = len(new_matches) - before_profile
        if got:
            print(f"   {profile}: +{got}")

    print(f"   {group} total new: +{len(new_matches) - before_group}")
    print(f"   combined new strict: {combined_count()} -> checkpoint {BASELINE + combined_count()}/1041")
    print()

# Persist evidence.
fields = [
    "address", "name", "group", "macro", "profile", "object_symbol",
    "object_size", "boundary", "result", "differing_bytes", "unknown_relocations",
]
with (BUILD / "attempts.tsv").open("w", newline="", encoding="utf-8") as f:
    w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    w.writeheader()
    for r in attempt_rows:
        w.writerow({k: r.get(k, "") for k in fields})

with (BUILD / "new_matches.tsv").open("w", newline="", encoding="utf-8") as f:
    w = csv.DictWriter(
        f,
        fieldnames=[
            "address", "name", "group", "macro", "profile",
            "object_symbol", "object_size", "boundary"
        ],
        delimiter="\t", lineterminator="\n"
    )
    w.writeheader()
    for a in sorted(new_matches):
        r = new_matches[a]
        w.writerow({
            "address": f"0x{a:08x}", "name": r.name, "group": r.group,
            "macro": r.macro, "profile": r.profile,
            "object_symbol": r.object_symbol, "object_size": r.object_size,
            "boundary": r.boundary,
        })

with (BUILD / "compile_failures.tsv").open("w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["group", "macro", "profile", "error_tail"])
    w.writerows(compile_failures)

combined = set(prior_matches) | set(new_matches)
by_group = Counter(r.group for r in new_matches.values())

print("=== RESULT ===")
print(f"prior Screen2 strict matches: {len(prior_matches)}")
print(f"new historical per-object strict matches: {len(new_matches)}")
print(f"combined new strict matches: {len(combined)}")
print(f"checkpoint if promoted: {BASELINE + len(combined)}/1041")
print(f"200+ threshold reached: {'YES' if BASELINE + len(combined) >= 200 else 'NO'}")
print()
print("New historical-per-object matches by group:")
for k, v in by_group.most_common():
    print(f"  {k}: {v}")
print(f"compile failures: {len(compile_failures)}")
print(f"evidence: {BUILD.relative_to(ROOT)}/")

if BASELINE + len(combined) >= 200:
    print()
    print("200+ STRICT MATCH THRESHOLD REACHED")
    print("Ready for consolidated promotion package.")
    raise SystemExit(0)

print()
print(f"Still need {200 - (BASELINE + len(combined))} additional strict matches.")
raise SystemExit(3)
