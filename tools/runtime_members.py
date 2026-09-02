#!/usr/bin/env python3
"""Rebuild the remaining PS2LIB member recipes and verify complete .text spans.

The migration snapshots are reproducible source witnesses, not a claim about
the target's build date or the identity of an entire historical .a container.
Only relocation-controlled bits are masked. Data, final relocation values,
global link order and two rejected libc candidates remain separate gates.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import shlex
import subprocess
import tempfile
from dataclasses import dataclass
from io import StringIO
from pathlib import Path, PurePosixPath
from typing import Sequence

import libgcc_contracts as libgcc
from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "analysis/link_identity"
DEFAULT_MANIFEST = BASE / "runtime_members.tsv"
DEFAULT_OBJECTS = BASE / "runtime_member_objects.tsv"
DEFAULT_INPUTS = BASE / "runtime_member_inputs.tsv"
DEFAULT_BUILD = ROOT / "build/runtime-members"
REPOSITORY = "https://github.com/ps2dev/ps2sdk.git"
APR15 = "694100b78ad5bc8f8248a1138143860af4f8435f"
APR18 = "a80df908256955382f102278400b5d713552dbce"
MAY04 = "94d9757035b8ea935383a11d51ed82ab3f65fc79"
REVISIONS = (APR15, APR18, MAY04)
GCC_INPUT = "gcc-3.2.2"
INPUTS_SHA256 = "cf977fe1a10e1522cac1dac3cff0559c1cdfa5551172ad459880427aef50679e"
EXACT = "MEMBER_TEXT_EXACT"
BLOCKED = "REJECTED_MEMBER_CANDIDATE"
PROFILE = "ps2lib-ee-gcc-3.2.2-os-default-abi"
FLAGS = ("-D_EE", "-DPS2_EE", "-G0", "-EL", "-pipe", "-w", "-Os", "-nostdinc")
INCLUDE_DIRS = (
    (APR18, "common/include"), (APR15, "ee/kernel/include"),
    (APR15, "ee/libc/include"), (APR18, "ee/kernel/include"),
    (APR15, "ee/rpc/memorycard/include"),
    (MAY04, "ee/libc/include"), (MAY04, "ee/kernel/include"),
)
INPUT_FIELDS = ("revision", "path", "sha256")
OBJECT_FIELDS = (
    "member", "status", "source_revision", "source", "define", "profile",
    "input_closure_sha256", "target_base", "text_size_hex", "text_sha256",
    "normalized_sha256", "target_sha256", "relocation_count", "symbols_sha256",
)
FIELDS = (
    "symbol", "status", "member", "member_symbol", "target_address",
    "member_offset_hex", "target_symbol_size_hex", "member_symbol_size_hex",
    "target_span_sha256", "canonical_symbol", "requesters", "detail",
)


class RuntimeMemberError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise RuntimeMemberError(message)


@dataclass(frozen=True)
class Member:
    key: str
    revision: str
    source: str
    define: str
    base: int | None
    size: int
    relocations: int

    @property
    def status(self) -> str:
        return EXACT if self.base is not None else BLOCKED


# Geometry includes every helper and terminal gap, not just the exported call.
MEMBERS = (
    Member("libc/memcpy.o", APR15, "ee/libc/src/memcpy.S", "", 0x19C364, 56, 0),
    Member("libc/memset.o", APR15, "ee/libc/src/memset.S", "", 0x19C39C, 56, 0),
    Member("libc/strcat.o", APR15, "ee/libc/src/strcat.S", "", 0x19C3D4, 56, 1),
    Member("libc/memcmp.o", APR15, "ee/libc/src/memcmp.S", "", 0x19C458, 72, 0),
    Member("libc/memmove.o", APR15, "ee/libc/src/memmove.S", "", 0x19C4A0, 136, 0),
    Member("libc/strcpy.o", APR15, "ee/libc/src/strcpy.S", "", 0x19C528, 40, 0),
    Member("libc/strncpy.o", APR15, "ee/libc/src/strncpy.S", "", 0x19C550, 88, 0),
    Member("libc/strlen.o", APR15, "ee/libc/src/strlen.S", "", 0x19C5E8, 40, 0),
    Member("libc/strchr.o", APR15, "ee/libc/src/strchr.S", "", 0x19C610, 56, 0),
    Member("libc/strcmp.o", APR15, "ee/libc/src/strcmp.S", "", 0x19C648, 64, 0),
    Member("libc/malloc.o", APR15, "ee/libc/src/alloc.c", "F_malloc", 0x19E474, 468, 20),
    Member("libc/calloc.o", APR15, "ee/libc/src/alloc.c", "F_calloc", 0x19E648, 80, 2),
    Member("libc/free.o", APR15, "ee/libc/src/alloc.c", "F_free", 0x19E784, 220, 10),
    Member("libc/vsprintf.o", APR15, "ee/libc/src/xprintf.c", "F_vsprintf", 0x19E364, 36, 1),
    Member("libc/printf.o", APR15, "ee/libc/src/xprintf.c", "F_printf", 0x19E388, 72, 1),
    Member("libc/sprintf.o", APR15, "ee/libc/src/xprintf.c", "F_sprintf", 0x19E3D0, 68, 1),
    Member("libc/puts.o", APR15, "ee/libc/src/xprintf.c", "F_puts", None, 80, 3),
    Member("libc/strrchr.o", APR15, "ee/libc/src/string.c", "F_strrchr", 0x19EAA4, 84, 2),
    Member("libc/strtol.o", APR15, "ee/libc/src/string.c", "F_strtol", 0x19EB80, 556, 9),
    Member("libc/terminate.o", APR18, "ee/libc/src/terminate.c", "", None, 68, 5),
    Member("kernel/fio_close.o", APR18, "ee/kernel/src/fileio.c", "F_fio_close", 0x19D090, 144, 11),
    Member("kernel/fio_main.o", APR18, "ee/kernel/src/fileio.c", "F_fio_main", 0x19F600, 488, 34),
    Member("kernel/fio_open.o", APR18, "ee/kernel/src/fileio.c", "F_fio_open", 0x19CFC0, 208, 18),
    Member("kernel/fio_putc.o", APR18, "ee/kernel/src/fileio.c", "F_fio_putc", 0x19D534, 36, 1),
    Member("kernel/fio_read.o", APR18, "ee/kernel/src/fileio.c", "F_fio_read", 0x19D120, 292, 22),
    Member("kernel/fio_lseek.o", APR18, "ee/kernel/src/fileio.c", "F_fio_lseek", 0x19D360, 176, 11),
    Member("kernel/fio_write.o", APR18, "ee/kernel/src/fileio.c", "F_fio_write", 0x19D244, 284, 20),
    Member("kernel/sif_cmd_addhandler.o", APR18, "ee/kernel/src/sifcmd.c", "F_sif_cmd_addhandler", 0x19F544, 56, 2),
    Member("kernel/sif_cmd_main.o", APR18, "ee/kernel/src/sifcmd.c", "F_sif_cmd_main", 0x19F2DC, 616, 43),
    Member("kernel/sif_sreg_get.o", APR18, "ee/kernel/src/sifcmd.c", "F_sif_sreg_get", 0x19F57C, 24, 2),
    Member("kernel/sif_cmd_send.o", APR18, "ee/kernel/src/sifcmd.c", "F_sif_cmd_send", 0x19F138, 420, 8),
    Member("kernel/SifBindRpc.o", APR18, "ee/kernel/src/sifrpc.c", "F_SifBindRpc", 0x19C688, 296, 8),
    Member("kernel/SifCallRpc.o", APR18, "ee/kernel/src/sifrpc.c", "F_SifCallRpc", 0x19C7B0, 432, 10),
    Member("kernel/SifRpcMain.o", APR18, "ee/kernel/src/sifrpc.c", "F_SifRpcMain", 0x19C960, 1040, 37),
    Member("kernel/SifAllocIopHeap.o", APR18, "ee/kernel/src/iopheap.c", "F_SifAllocIopHeap", 0x19D63C, 124, 6),
    Member("kernel/SifFreeIopHeap.o", APR18, "ee/kernel/src/iopheap.c", "F_SifFreeIopHeap", 0x19D6B8, 136, 6),
    Member("kernel/SifInitIopHeap.o", APR18, "ee/kernel/src/iopheap.c", "F_SifInitIopHeap", 0x19F9E8, 192, 10),
    Member("kernel/SifLoadFileInit.o", APR18, "ee/kernel/src/loadfile.c", "F_SifLoadFileInit", 0x19FD20, 188, 9),
    Member("kernel/SifLoadModule.o", APR18, "ee/kernel/src/loadfile.c", "F_SifLoadModule", 0x19D600, 32, 1),
    Member("kernel/SifLoadModuleBuffer.o", APR18, "ee/kernel/src/loadfile.c", "F_SifLoadModuleBuffer", 0x19D620, 28, 1),
    Member("kernel/SifIopReset.o", APR18, "ee/kernel/src/iopcontrol.c", "F_SifIopReset", 0x19D740, 268, 11),
    Member("syscall/FlushCache.o", APR18, "ee/kernel/src/kernel.S", "F_FlushCache", 0x19CEB0, 16, 0),
    Member("syscall/SifWriteBackDCache.o", APR18, "ee/kernel/src/kernel.S", "F_SifWriteBackDCache", 0x19CF10, 176, 0),
    Member("mc/libmc.o", APR15, "ee/rpc/memorycard/src/libmc.c", "", 0x1A0740, 5044, 382),
)
MEMBER_BY_KEY = {item.key: item for item in MEMBERS}


@dataclass(frozen=True)
class Contract:
    symbol: str
    member: str
    member_symbol: str
    address: int
    offset: int
    target_size: int
    member_size: int

    @property
    def status(self) -> str:
        return MEMBER_BY_KEY[self.member].status


CONTRACTS = (
    Contract("memcpy", "libc/memcpy.o", "memcpy", 0x19C364, 0, 56, 56),
    Contract("memset", "libc/memset.o", "memset", 0x19C39C, 0, 56, 56),
    Contract("strcat", "libc/strcat.o", "strcat", 0x19C3D4, 0, 56, 56),
    Contract("memcmp", "libc/memcmp.o", "memcmp", 0x19C458, 0, 72, 72),
    Contract("memmove", "libc/memmove.o", "memmove", 0x19C4A0, 0, 136, 136),
    Contract("strcpy", "libc/strcpy.o", "strcpy", 0x19C528, 0, 40, 40),
    Contract("strncpy", "libc/strncpy.o", "strncpy", 0x19C550, 0, 84, 84),
    Contract("strlen", "libc/strlen.o", "strlen", 0x19C5E8, 0, 40, 40),
    Contract("strchr", "libc/strchr.o", "strchr", 0x19C610, 0, 56, 56),
    Contract("strcmp", "libc/strcmp.o", "strcmp", 0x19C648, 0, 64, 64),
    Contract("malloc", "libc/malloc.o", "malloc", 0x19E4B4, 64, 404, 404),
    Contract("calloc", "libc/calloc.o", "calloc", 0x19E648, 0, 80, 80),
    Contract("free", "libc/free.o", "free", 0x19E784, 0, 220, 220),
    Contract("vsprintf", "libc/vsprintf.o", "vsprintf", 0x19E364, 0, 36, 36),
    Contract("printf", "libc/printf.o", "printf", 0x19E388, 0, 72, 72),
    Contract("sprintf", "libc/sprintf.o", "sprintf", 0x19E3D0, 0, 68, 68),
    Contract("puts", "libc/puts.o", "puts", 0x19E414, 0, 96, 80),
    Contract("strrchr", "libc/strrchr.o", "strrchr", 0x19EAA4, 0, 84, 84),
    Contract("strtol", "libc/strtol.o", "strtol", 0x19EB80, 0, 556, 556),
    Contract("abort", "libc/terminate.o", "abort", 0x107578, 0, 8, 36),
    Contract("fioClose_like", "kernel/fio_close.o", "fioClose", 0x19D090, 0, 144, 144),
    Contract("fioInit_recovered", "kernel/fio_main.o", "fioInit", 0x19F600, 0, 232, 232),
    Contract("fioOpen_like", "kernel/fio_open.o", "fioOpen", 0x19CFC0, 0, 208, 208),
    Contract("fioPutc_like_0019d534", "kernel/fio_putc.o", "fioPutc", 0x19D534, 0, 36, 36),
    Contract("fioRead_like", "kernel/fio_read.o", "fioRead", 0x19D120, 0, 292, 292),
    Contract("fioSeek_like_0019d360", "kernel/fio_lseek.o", "fioLseek", 0x19D360, 0, 176, 176),
    Contract("fioWrite_like", "kernel/fio_write.o", "fioWrite", 0x19D244, 0, 284, 284),
    Contract("SifAddCmdHandler", "kernel/sif_cmd_addhandler.o", "SifAddCmdHandler", 0x19F544, 0, 56, 56),
    Contract("SifExitCmd", "kernel/sif_cmd_main.o", "SifExitCmd", 0x19F510, 0x234, 52, 52),
    Contract("SifInitCmd", "kernel/sif_cmd_main.o", "SifInitCmd", 0x19F304, 40, 524, 524),
    Contract("SifGetSreg", "kernel/sif_sreg_get.o", "SifGetSreg", 0x19F57C, 0, 24, 24),
    Contract("SifSendCmd", "kernel/sif_cmd_send.o", "SifSendCmd", 0x19F264, 0x12C, 60, 60),
    Contract("SifBindRpc", "kernel/SifBindRpc.o", "SifBindRpc", 0x19C688, 0, 296, 296),
    Contract("SifCallRpc", "kernel/SifCallRpc.o", "SifCallRpc", 0x19C7B0, 0, 432, 432),
    Contract("SifInitRpc", "kernel/SifRpcMain.o", "SifInitRpc", 0x19CC0C, 0x2AC, 320, 320),
    Contract("SifAllocIopHeap", "kernel/SifAllocIopHeap.o", "SifAllocIopHeap", 0x19D63C, 0, 124, 124),
    Contract("SifFreeIopHeap", "kernel/SifFreeIopHeap.o", "SifFreeIopHeap", 0x19D6B8, 0, 136, 136),
    Contract("SifInitIopHeap_recovered", "kernel/SifInitIopHeap.o", "SifInitIopHeap", 0x19F9E8, 0, 192, 192),
    Contract("SifLoadFileInit_recovered", "kernel/SifLoadFileInit.o", "SifLoadFileInit", 0x19FD20, 0, 188, 188),
    Contract("SifLoadModule", "kernel/SifLoadModule.o", "SifLoadModule", 0x19D600, 0, 32, 32),
    Contract("SifLoadModuleBuffer", "kernel/SifLoadModuleBuffer.o", "SifLoadModuleBuffer", 0x19D620, 0, 28, 28),
    Contract("SifIopReset", "kernel/SifIopReset.o", "SifIopReset", 0x19D740, 0, 268, 268),
    Contract("FlushCache_0019ceb0", "syscall/FlushCache.o", "FlushCache", 0x19CEB0, 0, 16, 16),
    Contract("SifWriteBackDCache_0019cf10", "syscall/SifWriteBackDCache.o", "SifWriteBackDCache", 0x19CF10, 0, 172, 172),
    Contract("mcInit", "mc/libmc.o", "mcInit", 0x1A08EC, 0x1AC, 380, 380),
)
CONTRACT_BY_SYMBOL = {item.symbol: item for item in CONTRACTS}
EXACT_SYMBOLS = {item.symbol for item in CONTRACTS if item.status == EXACT}
BLOCKED_SYMBOLS = {item.symbol for item in CONTRACTS if item.status == BLOCKED}
DETAILS = {
    "puts": "pinned F_puts appends newline and returns len+1; rejected as provider; target-selected recovered override is proved separately",
    "abort": "pinned terminate.o prints and calls _exit; rejected as selected provider; its weak abort body is only a puts caller witness",
}


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def render(fields: Sequence[str], rows: Sequence[dict[str, str]]) -> str:
    stream = StringIO()
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    return stream.getvalue()


def ownership(symbol: str) -> tuple[str, str] | None:
    spec = CONTRACT_BY_SYMBOL.get(symbol)
    if spec is None:
        return None
    member = MEMBER_BY_KEY[spec.member]
    if spec.status == BLOCKED:
        from runtime_overrides import ownership as override_ownership
        return override_ownership(symbol)
    return f"PS2LIB {member.key} (ps2sdk@{member.revision[:8]} source)", "runtime-member-text-identity"


def validate_inputs(path: Path) -> list[dict[str, str]]:
    rows = libgcc.read_table(path, INPUT_FIELDS)
    keys = [(row["revision"], row["path"]) for row in rows]
    if len(rows) != 39 or keys != sorted(set(keys)):
        fail("runtime input ledger must contain 39 sorted unique files")
    for row in rows:
        part = PurePosixPath(row["path"])
        if (part.is_absolute() or ".." in part.parts or str(part) != row["path"]
                or not re.fullmatch(r"[A-Za-z0-9_./-]+", row["path"])):
            fail(f"unsafe runtime input path: {row['path']}")
        if row["revision"] not in (*REVISIONS, GCC_INPUT) or not libgcc.SHA_RE.fullmatch(row["sha256"]):
            fail("runtime input revision/hash drift")
    if {row["path"] for row in rows if row["revision"] == GCC_INPUT} != {"limits.h", "stdarg.h", "stddef.h"}:
        fail("unexpected compiler header set")
    if digest(render(INPUT_FIELDS, rows).encode()) != INPUTS_SHA256:
        fail("pinned runtime input hashes drifted")
    if not {(m.revision, m.source) for m in MEMBERS} <= set(keys):
        fail("runtime recipe source missing from input ledger")
    return rows


def live_bindings(args: argparse.Namespace) -> tuple[dict[str, dict[str, str]], dict[str, str]]:
    external = libgcc.read_table(args.external_map, libgcc.EXTERNAL_FIELDS)
    active = {r["symbol"]: r for r in external if r["category"] in ("c-runtime", "ps2-runtime")
              and r["provider_kind"] in ("historical-archive", "recovered-runtime")}
    if set(active) != set(CONTRACT_BY_SYMBOL) or len(external) != 1863:
        fail("live runtime contract universe drift")
    for symbol, row in active.items():
        if (row["owner"], row["resolution_gate"]) != ownership(symbol):
            fail(f"runtime ownership drift: {symbol}")
    contracts = {row["symbol"]: row for row in libgcc.read_table(args.contracts, libgcc.CONTRACT_FIELDS)}
    frontier = {row["symbol"]: row for row in libgcc.read_table(args.frontier_manifest, libgcc.FRONTIER_FIELDS)}
    with args.progress_manifest.open(encoding="utf-8", newline="") as stream:
        progress = {int(row["address"], 0): row for row in csv.DictReader(stream)}
    bindings = {}
    for spec in CONTRACTS:
        if progress.get(spec.address, {}).get("status") != "MATCHING":
            fail(f"runtime target missing from frozen function universe: {spec.symbol}")
        if spec.symbol == "abort":
            row = frontier.get("abort", {})
            if (row.get("resolution_kind"), row.get("target_symbol")) != ("semantic-text-alias", "snes_fatal_spin_00107578"):
                fail("abort source alias drift")
            bindings[spec.symbol] = row["target_symbol"]
        else:
            row = contracts.get(spec.symbol, {})
            if (row.get("status"), row.get("target_address")) != ("RESOLVED", f"0x{spec.address:08x}"):
                fail(f"runtime target binding drift: {spec.symbol}")
            bindings[spec.symbol] = row["canonical_symbol"]
    return active, bindings


def fixed_contract(spec: Contract, active: dict[str, dict[str, str]], bindings: dict[str, str]) -> dict[str, str]:
    return {
        "symbol": spec.symbol, "status": spec.status, "member": spec.member,
        "member_symbol": spec.member_symbol, "target_address": f"0x{spec.address:08x}",
        "member_offset_hex": hex(spec.offset), "target_symbol_size_hex": hex(spec.target_size),
        "member_symbol_size_hex": hex(spec.member_size), "canonical_symbol": bindings[spec.symbol],
        "requesters": active[spec.symbol]["requesters"],
        "detail": DETAILS.get(spec.symbol, "complete member text and symbol offset; final relocation values/data placement remain separate"),
    }


def fixed_object(member: Member) -> dict[str, str]:
    return {
        "member": member.key, "status": member.status, "source_revision": member.revision,
        "source": member.source, "define": member.define, "profile": PROFILE,
        "target_base": "" if member.base is None else f"0x{member.base:08x}",
        "text_size_hex": hex(member.size), "relocation_count": str(member.relocations),
    }


def validate_manifest(args: argparse.Namespace) -> tuple[list[dict[str, str]], list[dict[str, str]]]:
    validate_inputs(args.inputs)
    active, bindings = live_bindings(args)
    rows = libgcc.read_table(args.manifest, FIELDS)
    objects = libgcc.read_table(args.objects, OBJECT_FIELDS)
    if [r["symbol"] for r in rows] != sorted(CONTRACT_BY_SYMBOL):
        fail("runtime ledger must retain all 45 sorted contracts including two rejected member candidates")
    if [r["member"] for r in objects] != sorted(MEMBER_BY_KEY):
        fail("runtime member ledger must contain 42 selected and two rejected recipes")
    for row in rows:
        spec = CONTRACT_BY_SYMBOL[row["symbol"]]
        for key, value in fixed_contract(spec, active, bindings).items():
            if row[key] != value:
                fail(f"runtime {spec.symbol} {key} drift")
        if not libgcc.SHA_RE.fullmatch(row["target_span_sha256"]):
            fail(f"runtime {spec.symbol} missing target span hash")
        member = MEMBER_BY_KEY[spec.member]
        if spec.status == EXACT and (member.base + spec.offset != spec.address or spec.target_size != spec.member_size):
            fail(f"runtime symbol/member geometry drift: {spec.symbol}")
    for row in objects:
        member = MEMBER_BY_KEY[row["member"]]
        for key, value in fixed_object(member).items():
            if row[key] != value:
                fail(f"runtime {member.key} {key} drift")
        for field in ("text_sha256", "normalized_sha256", "input_closure_sha256", "symbols_sha256"):
            if not libgcc.SHA_RE.fullmatch(row[field]):
                fail(f"runtime {member.key} missing {field}")
        if member.status == EXACT:
            if not libgcc.SHA_RE.fullmatch(row["target_sha256"]):
                fail(f"runtime {member.key} missing target_sha256")
            if member.relocations == 0 and len({row[k] for k in ("text_sha256", "normalized_sha256", "target_sha256")}) != 1:
                fail(f"raw-exact member hash drift: {member.key}")
        elif row["target_base"] or row["target_sha256"]:
            fail(f"rejected recipe must not claim target member identity: {member.key}")
    selected = sorted((m.base, m.base + m.size) for m in MEMBERS if m.base is not None)
    if any(a[1] > b[0] for a, b in zip(selected, selected[1:])):
        fail("selected runtime member spans overlap")
    return rows, objects


def run(command: Sequence[str | Path], *, binary: bool = False, cwd: Path = ROOT) -> bytes | str:
    result = subprocess.run([str(x) for x in command], cwd=cwd, capture_output=True, text=not binary)
    if result.returncode:
        diagnostic = result.stderr or result.stdout
        if isinstance(diagnostic, bytes):
            diagnostic = diagnostic.decode(errors="replace")
        fail(f"command failed ({result.returncode}): {shlex.join(map(str, command))}\n{diagnostic[-4000:]}")
    return result.stdout


def verify_payload(payload: bytes, expected: str, label: str) -> None:
    if digest(payload) != expected:
        fail(f"runtime input SHA-256 mismatch: {label}")


def materialize_inputs(args: argparse.Namespace, compiler: Path) -> dict[Path, tuple[str, str, str]]:
    rows = validate_inputs(args.inputs)
    cache = args.source_cache or args.build_dir / "source-cache.git"
    output = args.build_dir / "inputs"
    gcc_include = Path(str(run([compiler, "-print-file-name=include"])).strip()).resolve()
    paths = {}
    missing = []
    for row in rows:
        path = (gcc_include / row["path"] if row["revision"] == GCC_INPUT
                else output / row["revision"] / row["path"])
        if path.is_file():
            verify_payload(path.read_bytes(), row["sha256"], str(path))
        elif row["revision"] == GCC_INPUT:
            fail(f"missing compiler header: {path}")
        else:
            missing.append((row, path))
        paths[path.resolve()] = (row["revision"], row["path"], row["sha256"])
    if missing:
        if not cache.exists():
            cache.parent.mkdir(parents=True, exist_ok=True)
            run(["git", "init", "--bare", "--quiet", cache])
        if not str(run(["git", "-C", cache, "rev-parse", "--git-dir"])).strip():
            fail(f"not a Git source cache: {cache}")
        for revision in sorted({row["revision"] for row, _ in missing}):
            available = subprocess.run(["git", "-C", str(cache), "cat-file", "-e", f"{revision}^{{commit}}"], capture_output=True)
            if available.returncode:
                print(f"runtime source: fetch ps2sdk@{revision[:8]}", flush=True)
                run(["git", "-C", cache, "fetch", "--quiet", "--depth=1", REPOSITORY, revision])
            actual = str(run(["git", "-C", cache, "rev-parse", f"{revision}^{{commit}}"])).strip()
            if actual != revision:
                fail("runtime source revision mismatch")
        for row, path in missing:
            payload = run(["git", "-C", cache, "show", f"{row['revision']}:{row['path']}"], binary=True)
            verify_payload(payload, row["sha256"], f"{row['revision']}:{row['path']}")
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(payload)
    return paths


def dependency_digest(text: str, allowed: dict[Path, tuple[str, str, str]], cwd: Path = ROOT) -> str:
    if ":" not in text:
        fail("malformed compiler dependency list")
    names = shlex.split(text.replace("\\\n", "").split(":", 1)[1])
    dependencies = set()
    for name in names:
        path = (cwd / name).resolve()
        if path not in allowed:
            fail(f"compiler consumed an unpinned input: {path}")
        dependencies.add(allowed[path])
    if not dependencies:
        fail("empty compiler dependency closure")
    return digest("".join("\t".join(row) + "\n" for row in sorted(dependencies)).encode())


def build_members(args: argparse.Namespace) -> tuple[dict[str, Path], dict[str, str]]:
    compiler = libgcc.resolve_tool(args.compiler)
    if (str(run([compiler, "-dumpversion"])).strip(), str(run([compiler, "-dumpmachine"])).strip()) != ("3.2.2", "ee"):
        fail("runtime members require historical EE GCC 3.2.2")
    if not args.ar and not compiler.name.endswith("gcc"):
        fail("cannot derive the historical archiver; pass --ar")
    ar = libgcc.resolve_tool(args.ar) if args.ar else libgcc.resolve_tool(str(compiler.with_name(compiler.name[:-3] + "ar")))
    allowed = materialize_inputs(args, compiler)
    include_flags = []
    for revision, relative in INCLUDE_DIRS:
        include_flags += ["-I", str(args.build_dir / "inputs" / revision / relative)]
    # -nostdinc plus this explicit directory prevents a host/SDK header fallback.
    include_flags += ["-I", str(Path(str(run([compiler, "-print-file-name=include"])).strip()).resolve())]
    built, closures = {}, {}
    for member in sorted(MEMBERS, key=lambda item: item.key):
        obj = args.build_dir / "objects" / member.key
        obj.parent.mkdir(parents=True, exist_ok=True)
        dep = obj.with_suffix(".d")
        source = args.build_dir / "inputs" / member.revision / member.source
        command = [compiler, *FLAGS, *include_flags]
        if member.define:
            command.append("-D" + member.define)
        command += ["-MD", "-MF", dep, "-c", source, "-o", obj]
        run(command)
        closures[member.key] = dependency_digest(dep.read_text(encoding="utf-8"), allowed)
        built[member.key] = obj
    # Reconstruct selection containers locally. Their whole-file hashes and
    # historical names/order are deliberately NOT identity claims.
    groups = sorted({m.key.split("/")[0] for m in MEMBERS if m.status == EXACT})
    archive_dir = args.build_dir / "archives"
    archive_dir.mkdir(parents=True, exist_ok=True)
    for group in groups:
        keys = sorted(m.key for m in MEMBERS if m.status == EXACT and m.key.startswith(group + "/"))
        with tempfile.TemporaryDirectory(prefix="archive-", dir=archive_dir) as temp:
            archive = Path(temp) / (group + ".selected.a")
            run([ar, "rcs", archive, *(built[key] for key in keys)])
            actual = str(run([ar, "t", archive])).splitlines()
            if actual != [Path(key).name for key in keys]:
                fail(f"runtime archive selection drift: {group}")
            for key in keys:
                payload = run([ar, "p", archive, Path(key).name], binary=True)
                if payload != built[key].read_bytes():
                    fail(f"runtime archive member payload drift: {key}")
                checked = args.build_dir / "checked-members" / key
                checked.parent.mkdir(parents=True, exist_ok=True)
                checked.write_bytes(payload)
                built[key] = checked
            archive.replace(archive_dir / archive.name)
    return built, closures


def symbol_digest(elf: ELFFile) -> str:
    text_indexes = {section.index for section in elf.sections if section.name == ".text"}
    rows = sorted((symbol.name, symbol.value, symbol.size, symbol.info) for symbol in elf.symbols
                  if symbol.section_index in text_indexes and symbol.info & 15 == 2)
    return digest(json.dumps(rows, separators=(",", ":"), ensure_ascii=True).encode())


def compare_member(image: bytes, masks: Sequence[object], expected: bytes, key: str) -> None:
    if len(image) != len(expected) or any(
        not item.known or item.start < 0 or item.end > len(image)
        or item.end - item.start != len(item.mask_bytes) or item.start % 4
        for item in masks
    ):
        fail(f"member size or unsupported relocation: {key}")
    differences = libgcc.differing_unmasked(expected, image, masks)
    if differences or libgcc.normalize_target(expected, masks) != libgcc.normalize_target(image, masks):
        fail(f"complete member differs outside relocation bits: {key} ({differences} bytes)")


def capture(args: argparse.Namespace) -> tuple[list[dict[str, str]], list[dict[str, str]], dict[str, object]]:
    # Fail before fetching/compiling if the reference is absent or not exact.
    raw = libgcc.load_reference(args.reference)
    active, bindings = live_bindings(args)
    built, closures = build_members(args)
    rows, objects, rejected = [], [], []
    for member in sorted(MEMBERS, key=lambda item: item.key):
        elf = ELFFile(built[member.key])
        if (elf.file_type, elf.machine, elf.elf_class, elf.endian) != (1, 8, 1, "<"):
            fail(f"not an ELF32 little-endian MIPS relocatable: {member.key}")
        image, masks, normalized = libgcc.text_image(built[member.key])
        if (len(image), len(masks)) != (member.size, member.relocations):
            fail(f"runtime member geometry drift: {member.key}")
        target_hash = ""
        if member.status == EXACT:
            start = member.base - libgcc.TARGET_BASE
            expected = raw[start:start + member.size]
            compare_member(image, masks, expected, member.key)
            target_hash = digest(expected)
        objects.append({**fixed_object(member), "input_closure_sha256": closures[member.key],
                        "text_sha256": digest(image), "normalized_sha256": digest(normalized),
                        "target_sha256": target_hash, "symbols_sha256": symbol_digest(elf)})
        for spec in sorted((c for c in CONTRACTS if c.member == member.key), key=lambda c: c.symbol):
            symbols = [s for s in elf.symbols if s.name == spec.member_symbol and s.section_index != 0 and s.info & 15 == 2]
            if len(symbols) != 1 or (symbols[0].value, symbols[0].size) != (spec.offset, spec.member_size):
                fail(f"runtime member symbol drift: {spec.symbol}")
            start = spec.address - libgcc.TARGET_BASE
            target_body = raw[start:start + spec.target_size]
            if spec.status == BLOCKED:
                candidate = elf.symbol_bytes(symbols[0], symbols[0].size)
                relocations = elf.relocation_masks(symbols[0], 4)
                if spec.target_size == spec.member_size and libgcc.differing_unmasked(target_body, candidate, relocations) == 0:
                    fail(f"rejected runtime candidate unexpectedly matches: {spec.symbol}")
                rejected.append({"symbol": spec.symbol, "candidate_member": member.key,
                                 "target_symbol_size": spec.target_size, "candidate_symbol_size": spec.member_size})
            rows.append({**fixed_contract(spec, active, bindings), "target_span_sha256": digest(target_body)})
    rows.sort(key=lambda row: row["symbol"])
    report = statistics(rows, objects)
    report.update({"target_sha256": libgcc.TARGET_SHA256, "rejected_candidates": rejected,
                   "scope": "complete member text, relocation masks and symbol offsets; not final data/relocation/link/container identity"})
    return rows, objects, report


def statistics(rows: Sequence[dict[str, str]], objects: Sequence[dict[str, str]]) -> dict[str, int]:
    selected = [row for row in objects if row["status"] == EXACT]
    closed = sum(row["status"] == EXACT for row in rows)
    return {"contracts_total": len(rows), "contracts_closed": closed, "contracts_blocked": len(rows) - closed,
            "selected_members": len(selected), "rejected_members": len(objects) - len(selected),
            "exact_text_bytes": sum(int(row["text_size_hex"], 0) for row in selected),
            "relocations_normalized": sum(int(row["relocation_count"]) for row in selected)}


def summary(report: dict[str, object]) -> str:
    return (f"contracts={report['contracts_closed']}/{report['contracts_total']} "
            f"members={report['selected_members']} text_bytes={report['exact_text_bytes']} "
            f"relocations={report['relocations_normalized']} "
            f"rejected_member_candidates={report['rejected_members']}")


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "verify", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--objects", type=Path, default=DEFAULT_OBJECTS)
    parser.add_argument("--inputs", type=Path, default=DEFAULT_INPUTS)
    parser.add_argument("--external-map", type=Path, default=libgcc.DEFAULT_EXTERNAL)
    parser.add_argument("--contracts", type=Path, default=libgcc.DEFAULT_CONTRACTS)
    parser.add_argument("--frontier-manifest", type=Path, default=libgcc.DEFAULT_FRONTIER)
    parser.add_argument("--progress-manifest", type=Path, default=ROOT / "analysis/progress_targets.csv")
    parser.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    parser.add_argument("--compiler", default="ee-gcc")
    parser.add_argument("--ar")
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--source-cache", type=Path)
    parser.add_argument("--report", type=Path, default=DEFAULT_BUILD / "report.json")
    args = parser.parse_args(argv)
    for key, value in vars(args).items():
        if isinstance(value, Path):
            setattr(args, key, value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "validate":
            rows, objects = validate_manifest(args)
            report = statistics(rows, objects)
        else:
            frozen = validate_manifest(args) if args.command == "verify" else None
            rows, objects, report = capture(args)
            if frozen is not None and frozen != (rows, objects):
                fail("private runtime member proof differs from frozen manifests")
            if args.command == "capture":
                args.manifest.write_text(render(FIELDS, rows), encoding="utf-8")
                args.objects.write_text(render(OBJECT_FIELDS, objects), encoding="utf-8")
                validate_manifest(args)
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        print(f"verified Stage-3D runtime members: {summary(report)}")
        print("rejected member providers: abort, puts (target overrides have a separate proof gate)")
        return 0
    except (RuntimeMemberError, libgcc.LibgccContractError, OSError, ValueError, KeyError) as error:
        print(f"runtime members: FAIL: {error}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
