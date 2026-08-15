#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import importlib.util
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path

BASE_COMMIT = "ffdfc8e946cd29c0331aa0c27888ae38f6ab7a04"
BASELINE = 102
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
TARGET_BASE = 0x00100000

ROOT = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
BUILD = ROOT / "build/matching/progress54-200plus-screen2"
OBJDIR = BUILD / "objects"
LOGDIR = BUILD / "logs"
for p in (BUILD, OBJDIR, LOGDIR):
    p.mkdir(parents=True, exist_ok=True)

def die(msg: str) -> None:
    raise SystemExit(f"Progress54 screen2: {msg}")

def run(cmd: list[str], *, cwd: Path = ROOT, timeout: int | None = None) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        text=True, timeout=timeout
    )

head = run(["git", "rev-parse", "HEAD"]).stdout.strip()
if head != BASE_COMMIT:
    die(f"expected 102-MATCH main at {BASE_COMMIT}; found {head}")

if run(["git", "diff", "--quiet"]).returncode != 0:
    die("tracked working tree is not clean")
if run(["git", "diff", "--cached", "--quiet"]).returncode != 0:
    die("index has staged changes")

# Load comparator implementation from the repository itself.
cmp_path = ROOT / "tools/compare_elf_functions.py"
spec = importlib.util.spec_from_file_location("cmpmod_screen2", cmp_path)
if spec is None or spec.loader is None:
    die("cannot import tools/compare_elf_functions.py")
cmpmod = importlib.util.module_from_spec(spec)
sys.modules["cmpmod_screen2"] = cmpmod
spec.loader.exec_module(cmpmod)
ELFFile = cmpmod.ELFFile
compare_function = cmpmod.compare_function

# Canonical target universe.
with (ROOT / "analysis/progress_targets.csv").open(newline="", encoding="utf-8") as f:
    rows = list(csv.DictReader(f))
progress = {int(r["address"], 16): r for r in rows}
starts = sorted(progress)
next_start = {
    a: starts[i + 1] if i + 1 < len(starts) else None
    for i, a in enumerate(starts)
}
baseline = sum(r["status"] == "MATCHING" for r in rows)
if baseline != BASELINE:
    die(f"expected {BASELINE} MATCHING rows; found {baseline}")

name_to_addrs: dict[str, list[int]] = defaultdict(list)
for a, r in progress.items():
    name_to_addrs[r["name"]].append(a)

# Prefer the formal locally-owned reference.  The repo intentionally ignores
# original/*.ELF and build/, so this never redistributes the binary.
reference = ROOT / "build/SNES_EMU.unpacked.bin"
formal = False
if not reference.exists() and (ROOT / "original/SNES_EMU.ELF").exists():
    print("formal reference missing; rebuilding it from original/SNES_EMU.ELF ...")
    cp = run(["make", "reference"])
    (LOGDIR / "make-reference.log").write_text(cp.stdout, encoding="utf-8")
    if cp.returncode:
        print("make reference failed; falling back to committed-listing bytes")

if reference.exists():
    h = hashlib.sha256(reference.read_bytes()).hexdigest()
    if h == TARGET_SHA256:
        target = reference.read_bytes()
        known = None
        formal = True
    else:
        print(f"WARNING: build/SNES_EMU.unpacked.bin has unexpected sha256 {h}; not using it")

if not formal:
    # Sparse committed-listing fallback.  Unknown bytes are never compared.
    insn_re = re.compile(
        r"^\s*([0-9A-Fa-f]+):\s+"
        r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
        r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
    )
    byte_map: dict[int, int] = {}
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
                old = byte_map.get(p)
                if old is not None and old != b:
                    conflicts.add(p)
                else:
                    byte_map[p] = b
    for p in conflicts:
        byte_map.pop(p, None)
    if not byte_map:
        die("formal reference unavailable and no committed listing bytes parsed")
    end = max(byte_map) + 1
    target_b = bytearray(end - TARGET_BASE)
    for a, b in byte_map.items():
        if a >= TARGET_BASE:
            target_b[a - TARGET_BASE] = b
    target = bytes(target_b)
    known = set(byte_map)

def bytes_known(start: int, end: int) -> bool:
    if formal:
        return TARGET_BASE <= start <= end <= TARGET_BASE + len(target)
    assert known is not None
    return all(a in known for a in range(start, end))

def target_slice(start: int, end: int) -> bytes:
    return target[start - TARGET_BASE:end - TARGET_BASE]

# Find every usable local EE compiler. Usually this is just stage1; if the user
# has another native historical build already unpacked, it is picked up too.
cc_candidates: list[Path] = []
env_cc = os.environ.get("EE_CC")
if env_cc:
    cc_candidates.append(Path(env_cc).expanduser())
cc_candidates.append(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
for p in (ROOT / "build/toolchains").glob("**/bin/ee-gcc"):
    cc_candidates.append(p)

compilers: list[tuple[str, Path]] = []
seen_cc: set[str] = set()
for p in cc_candidates:
    try:
        rp = str(p.resolve())
    except Exception:
        rp = str(p)
    if rp in seen_cc or not p.exists() or not os.access(p, os.X_OK):
        continue
    seen_cc.add(rp)
    try:
        cp = run([str(p), "-dumpmachine"], timeout=10)
    except Exception:
        continue
    machine = cp.stdout.strip()
    if cp.returncode or not machine:
        continue
    ver = run([str(p), "-dumpversion"], timeout=10).stdout.strip()
    tag = re.sub(r"[^A-Za-z0-9_.-]+", "_", f"{p.parent.parent.parent.name}-{ver}")
    compilers.append((tag, p))

if not compilers:
    die("no native EE compiler found; expected build/toolchains/.../bin/ee-gcc")

# Preflight the stage1 scan ABI bridge before compiling 103 translation units.
# This intentionally uses the same include contract as the successful first
# broad screening and avoids modern-host-only include flags.
probe = BUILD / "include-policy-probe.c"
probe.write_text(
    '#include <stdint.h>\n'
    'int32_t a; uint8_t b; int8_t c; uint64_t d; uintptr_t e;\n',
    encoding="utf-8",
)
probe_cc = compilers[0][1]
probe_cp = run([
    str(probe_cc),
    "-G0", "-O2", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Iinclude/ee_stage1_compat", "-Iinclude",
    "-fsyntax-only", str(probe),
])
(LOGDIR / "include-policy-probe.log").write_text(probe_cp.stdout, encoding="utf-8")
if probe_cp.returncode:
    die(
        "EE include-policy preflight failed; inspect "
        "build/matching/progress54-200plus-screen2/logs/include-policy-probe.log"
    )

COMMON = [
    "-G0", "-EL", "-pipe",
    "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common",
    "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls",
    "-march=r5900", "-mtune=r5900",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD", "-DCODE_PLATFORM=3",
    # Match the include contract already proven by the first broad screen.
    # GCC 3.2.2's historical EE driver is not given the modern-host -iquote
    # workaround; source-specific legacy headers are handled separately.
    "-Iinclude/ee_stage1_compat", "-Iinclude",
    "-w",
]

BASE_PROFILES = {
    "sdk-o2": ["-O2"],
    "snesticle-o2": ["-O2", "-ffreestanding", "-fno-builtin"],
}

VARIANT_PROFILES = {
    "sdk-o2-nobuiltin": ["-O2", "-fno-builtin"],
    "sdk-o2-nostrict": ["-O2", "-fno-strict-aliasing"],
    "sdk-o2-noinline": ["-O2", "-fno-inline"],
    "sdk-o2-nosched2": ["-O2", "-fno-schedule-insns2"],
    "sdk-o2-nosched": ["-O2", "-fno-schedule-insns"],
    "sdk-o2-nosched-both": ["-O2", "-fno-schedule-insns", "-fno-schedule-insns2"],
    "sdk-o2-no-reorder": ["-O2", "-fno-reorder-blocks"],
    "sdk-o2-no-ifconv": ["-O2", "-fno-if-conversion", "-fno-if-conversion2"],
    "sdk-o2-no-align-jumps": ["-O2", "-fno-align-jumps"],
    "sdk-o2-no-align-cf": ["-O2", "-fno-align-jumps", "-fno-align-labels", "-fno-align-loops"],
    "sdk-o1": ["-O1"],
    "sdk-os": ["-Os"],
    "sdk-o3": ["-O3"],
}

sources = sorted(
    {p for p in ROOT.glob("src/**/*.c")} |
    {p for p in ROOT.glob("matching/candidates/*.c")}
)

def safe(s: str) -> str:
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", s)

def source_tag(src: Path) -> str:
    return safe(str(src.relative_to(ROOT)).replace("/", "__"))

def compile_one(src: Path, cc_tag: str, cc: Path, profile: str, pflags: list[str]) -> Path | None:
    stem = source_tag(src)
    obj = OBJDIR / f"{stem}.{cc_tag}.{profile}.o"
    log = LOGDIR / f"{stem}.{cc_tag}.{profile}.log"

    extra: list[str] = []
    rel = str(src.relative_to(ROOT))
    # This historical CDVD matching candidate has a private quoted compatibility
    # header.  Do not put its directory on the global include path because its
    # tiny stdint.h would shadow the stage1 scan ABI bridge for every TU.
    if rel == "matching/candidates/cdvd_rpc.c":
        # GCC 3.2.2 supports the old -I- quote-search split. Directories before
        # -I- are used for quoted includes, not angle-bracket system includes.
        extra = ["-Imatching/ee_abi_compat", "-I-"]

    cmd = [str(cc), *extra, *COMMON, *pflags, "-c", str(src), "-o", str(obj)]
    cp = run(cmd)
    log.write_text(cp.stdout, encoding="utf-8")
    if cp.returncode:
        return None
    return obj

def unique_name_addr(name: str) -> int | None:
    vals = name_to_addrs.get(name, [])
    if len(vals) == 1:
        return vals[0]
    return None

addr_suffix = re.compile(r"_([0-9A-Fa-f]{8})(?:_candidate)?$")

@dataclass
class Mapped:
    addr: int
    symbol: object
    source: Path
    object_path: Path
    cc_tag: str
    profile: str

def map_symbol(sym_name: str) -> int | None:
    m = addr_suffix.search(sym_name)
    if m:
        a = int(m.group(1), 16)
        if a in progress:
            return a
    names = [sym_name]
    if sym_name.endswith("_candidate"):
        names.append(sym_name[:-10])
    # A few matching candidates use explicit helper aliases.
    if sym_name.endswith("_recovered"):
        names.append(sym_name[:-10])
    for n in names:
        a = unique_name_addr(n)
        if a is not None:
            return a
    return None

def discover(obj: Path, src: Path, cc_tag: str, profile: str) -> tuple[object, list[Mapped]]:
    elf = ELFFile(obj)
    mapped: list[Mapped] = []
    for sym in elf.symbols:
        if sym.section_index == 0 or sym.size <= 0 or (sym.info & 0xF) != 2:
            continue
        a = map_symbol(sym.name)
        if a is None:
            continue
        if progress[a]["status"] != "RECONSTRUCTED":
            continue
        mapped.append(Mapped(a, sym, src, obj, cc_tag, profile))
    # Deduplicate symbol aliases by (target address, object symbol).
    uniq: dict[tuple[int, str], Mapped] = {}
    for m in mapped:
        uniq[(m.addr, m.symbol.name)] = m
    return elf, sorted(uniq.values(), key=lambda x: (x.symbol.section_index, x.symbol.value, x.addr))

def section_bytes(elf: object, section_index: int, start_value: int, end_value: int) -> bytes:
    sec = elf.sections[section_index]
    rs = start_value - sec.address
    re_ = end_value - sec.address
    if rs < 0 or re_ < rs or re_ > sec.size:
        return b""
    return elf.data[sec.offset + rs:sec.offset + re_]

def terminal_return_proof(addr: int, size: int) -> bool:
    if size < 8 or not bytes_known(addr, addr + size):
        return False
    b = target_slice(addr + size - 8, addr + size)
    # little-endian `jr $ra` then one delay-slot instruction
    return b[:4] == bytes.fromhex("0800e003")

def boundary_proof(elf: object, items: list[Mapped], index: int) -> tuple[bool, str]:
    m = items[index]
    addr = m.addr
    size = m.symbol.size
    nxt = next_start.get(addr)
    if nxt is None or size <= 0:
        return False, "no-next-target"
    span = nxt - addr
    if size > span:
        return False, f"oversize:+0x{size-span:x}"
    if size == span:
        return True, "exact-next-boundary"

    # Strongest padding proof: next mapped function is also the next canonical
    # target and object-to-object layout delta equals target-to-target delta.
    next_mapped = None
    for j in range(index + 1, len(items)):
        n = items[j]
        if n.symbol.section_index != m.symbol.section_index:
            continue
        if n.symbol.value > m.symbol.value:
            next_mapped = n
            break
    if next_mapped is not None and next_mapped.addr == nxt:
        obj_delta = next_mapped.symbol.value - m.symbol.value
        if obj_delta == span:
            gap_start_val = m.symbol.value + size
            gap_obj = section_bytes(
                elf, m.symbol.section_index, gap_start_val, next_mapped.symbol.value
            )
            if bytes_known(addr + size, nxt):
                gap_target = target_slice(addr + size, nxt)
                if gap_obj == gap_target:
                    return True, f"object-layout-gap:0x{span-size:x}"

    # Conservative terminal-return + alignment proof.  0x3c is the maximum
    # padding produced by a 64-byte function alignment.
    gap = span - size
    if 0 < gap <= 0x3C and bytes_known(addr + size, nxt):
        gap_target = target_slice(addr + size, nxt)
        if not any(gap_target) and terminal_return_proof(addr, size):
            return True, f"jr-ra+nop-padding:0x{gap:x}"

    return False, f"unproven-gap:0x{gap:x}"

matches: dict[int, dict[str, object]] = {}
attempts: dict[int, list[dict[str, object]]] = defaultdict(list)
compile_failures: list[tuple[str, str, str]] = []
source_stats: dict[str, Counter] = defaultdict(Counter)
near_sources: set[Path] = set()
matched_sources: set[Path] = set()

def evaluate_object(obj: Path, src: Path, cc_tag: str, profile: str) -> None:
    try:
        elf, items = discover(obj, src, cc_tag, profile)
    except Exception as e:
        (LOGDIR / f"{safe(obj.name)}.parse-error.log").write_text(str(e), encoding="utf-8")
        return

    for i, m in enumerate(items):
        a = m.addr
        rec = {
            "address": f"0x{a:08x}",
            "name": progress[a]["name"],
            "area": progress[a]["area"],
            "source": str(src.relative_to(ROOT)),
            "profile": profile,
            "compiler": cc_tag,
            "object_symbol": m.symbol.name,
            "object_size": m.symbol.size,
        }
        if not bytes_known(a, a + m.symbol.size):
            rec["result"] = "target-bytes-missing"
            attempts[a].append(rec)
            source_stats[str(src.relative_to(ROOT))]["target_missing"] += 1
            continue

        ok, proof = boundary_proof(elf, items, i)
        rec["boundary"] = proof
        if not ok:
            rec["result"] = "boundary-reject"
            attempts[a].append(rec)
            source_stats[str(src.relative_to(ROOT))]["boundary_reject"] += 1
            # A small size/layout miss is worth compiler-fingerprint variants.
            if proof.startswith("oversize:+0x"):
                try:
                    delta = int(proof.split("0x", 1)[1], 16)
                    if delta <= 0x40:
                        near_sources.add(src)
                except Exception:
                    pass
            elif proof.startswith("unproven-gap:"):
                near_sources.add(src)
            continue

        try:
            comp = compare_function(
                target, a - TARGET_BASE, m.symbol.size, elf, m.symbol.name
            )
        except Exception as e:
            rec["result"] = f"compare-error:{e}"
            attempts[a].append(rec)
            continue

        rec["differing_bytes"] = comp.differing_bytes
        rec["raw_equal"] = comp.raw_equal
        rec["normalized_equal"] = comp.normalized_equal
        rec["unknown_relocations"] = ",".join(map(str, comp.unknown_relocation_types))
        if comp.matching and not comp.unknown_relocation_types:
            rec["result"] = "MATCH"
            attempts[a].append(rec)
            if a not in matches:
                matches[a] = rec
            matched_sources.add(src)
            source_stats[str(src.relative_to(ROOT))]["match"] += 1
        else:
            rec["result"] = "mismatch"
            attempts[a].append(rec)
            source_stats[str(src.relative_to(ROOT))]["mismatch"] += 1
            if comp.differing_bytes <= 16:
                near_sources.add(src)

def compile_and_eval(profile_map: dict[str, list[str]], selected: set[Path] | None = None) -> None:
    selected_sources = sources if selected is None else [s for s in sources if s in selected]
    for cc_tag, cc in compilers:
        for src in selected_sources:
            for profile, pflags in profile_map.items():
                obj = compile_one(src, cc_tag, cc, profile, pflags)
                if obj is None:
                    compile_failures.append(
                        (str(src.relative_to(ROOT)), cc_tag, profile)
                    )
                    continue
                evaluate_object(obj, src, cc_tag, profile)

print("=== Progress54 screen2: all recovered C, formal target when available ===")
print(f"target gate: {'FORMAL original unpacked image' if formal else 'committed listings fallback'}")
print(f"C translation units discovered: {len(sources)}")
print(f"native EE compilers discovered: {len(compilers)}")
for tag, cc in compilers:
    print(f"  {tag}: {cc}")

print()
print("Pass 1: baseline historical O2 profiles across all C translation units")
compile_and_eval(BASE_PROFILES)

checkpoint = BASELINE + len(matches)
print(f"pass1 strict new matches: {len(matches)} -> checkpoint {checkpoint}/1041")
if len(compile_failures) == len(sources) * len(compilers) * len(BASE_PROFILES):
    die(
        "every baseline compile failed; refusing to report this as a 0-MATCH result. "
        "Inspect compile_failures.tsv/logs."
    )

if checkpoint < 200:
    variant_sources = matched_sources | near_sources
    print()
    print(
        f"Pass 2: compiler/source-shape fingerprint variants on "
        f"{len(variant_sources)} high-yield translation units"
    )
    before = len(matches)
    compile_and_eval(VARIANT_PROFILES, variant_sources)
    print(f"pass2 additional strict matches: {len(matches) - before}")

checkpoint = BASELINE + len(matches)

# Evidence files.
fields = [
    "address", "name", "area", "source", "profile", "compiler",
    "object_symbol", "object_size", "boundary", "result",
    "differing_bytes", "raw_equal", "normalized_equal", "unknown_relocations",
]
with (BUILD / "matches.tsv").open("w", newline="", encoding="utf-8") as f:
    w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    w.writeheader()
    for a in sorted(matches):
        row = {k: matches[a].get(k, "") for k in fields}
        w.writerow(row)

with (BUILD / "all_attempts.tsv").open("w", newline="", encoding="utf-8") as f:
    w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    w.writeheader()
    for a in sorted(attempts):
        for rec in attempts[a]:
            w.writerow({k: rec.get(k, "") for k in fields})

with (BUILD / "compile_failures.tsv").open("w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["source", "compiler", "profile"])
    w.writerows(compile_failures)

area_counts = Counter(str(r["area"]) for r in matches.values())
source_counts = Counter(str(r["source"]) for r in matches.values())
profile_counts = Counter(str(r["profile"]) for r in matches.values())

summary = [
    f"target_gate={'formal-original' if formal else 'committed-listings'}",
    f"baseline={BASELINE}",
    f"new_strict_matches={len(matches)}",
    f"checkpoint_if_promoted={checkpoint}",
    f"threshold_200_reached={'YES' if checkpoint >= 200 else 'NO'}",
    f"compile_failures={len(compile_failures)}",
    "",
    "matches_by_area:",
    *[f"  {k}: {v}" for k, v in area_counts.most_common()],
    "",
    "matches_by_source:",
    *[f"  {k}: {v}" for k, v in source_counts.most_common()],
    "",
    "matches_by_profile:",
    *[f"  {k}: {v}" for k, v in profile_counts.most_common()],
]
(BUILD / "summary.txt").write_text("\n".join(summary) + "\n", encoding="utf-8")

print()
print("=== RESULT ===")
print(f"new strict matches vs committed 102: {len(matches)}")
print(f"checkpoint if promoted: {checkpoint}/1041")
print(f"200+ threshold reached: {'YES' if checkpoint >= 200 else 'NO'}")
print()
print("MATCHING:")
for a in sorted(matches):
    r = matches[a]
    print(
        f"  MATCH {r['address']} {r['name']} "
        f"[{r['profile']}; {r['source']}; {r.get('boundary','')}]"
    )

print()
print("Matches by area:")
for k, v in area_counts.most_common():
    print(f"  {k}: {v}")
print()
print(f"Full evidence: {BUILD.relative_to(ROOT)}/")
print(f"Compile failures recorded: {len(compile_failures)}")

if checkpoint >= 200:
    print()
    print("200+ STRICT SCREEN REACHED — ready for promotion/closed-checkpoint package.")
    raise SystemExit(0)

print()
print("Still below 200; all near misses are preserved in all_attempts.tsv for the next source-shape pass.")
raise SystemExit(3)
