#!/usr/bin/env python3
"""Prove direct EE memory-access spans for the Stage-3F address contracts.

A direct target load/store proves its consumed byte width, NOT an entire C
object, array bound, initialized region or final section placement. This gate
never uses the structural lift's provisional uint64_t declarations as sizes.
Block-local constants and fixed-point must-constant control-flow proofs inside
strict matched function spans are accepted. Unproved/indexed contracts remain
explicit; a branch is never pruned merely to obtain a desired address.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import struct
from collections import Counter
from pathlib import Path
from typing import Sequence

import libgcc_contracts as libgcc
import named_data
import ee_dataflow
import rom_offsets
import source_aliases

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/unnamed_data_accesses.tsv"
PROVED = "DIRECT_ACCESS_PROVED"
BLOCKED = "NO_DIRECT_ACCESS_WITNESS"
FIELDS = ("symbol", "target_address", "status", "extent_hex", "region", "sha256",
          "function_address", "function_extent_hex", "access_address", "access_opcode",
          "base_register", "trace_addresses", "instruction_window_sha256", "callee_address", "callee_sha256",
          "matching_evidence", "matching_evidence_sha256", "requesters",
          "proof_kind", "function_sha256", "analysis_sha256", "execution_step", "claim")
# Partial unaligned transfers, atomics, cache instructions and indirect/indexed
# base addresses are deliberately not converted into object-width claims.
MEMORY = {
    32: ("lb", 1), 33: ("lh", 2), 35: ("lw", 4), 36: ("lbu", 1),
    37: ("lhu", 2), 39: ("lwu", 4), 55: ("ld", 8), 30: ("lq", 16),
    40: ("sb", 1), 41: ("sh", 2), 43: ("sw", 4), 63: ("sd", 8), 31: ("sq", 16),
    49: ("lwc1", 4), 57: ("swc1", 4), 54: ("lqc2", 16), 62: ("sqc2", 16),
}
STORES = {40, 41, 43, 63, 31, 57, 62}
MNEMONICS = {name: width for name, width in MEMORY.values()}
CALLS = {
    0x19C364: ("memcpy", (4, 5), "libc/memcpy.o"),
    0x19C39C: ("memset", (4,), "libc/memset.o"),
    0x19C4A0: ("memmove", (4, 5), "libc/memmove.o"),
}
BRANCHES = {1, 4, 5, 6, 7, 20, 21, 22, 23}
CLAIM = "minimum directly accessed span; complete object/array extent and final layout unproved"
LOCAL = "block-local-constant"
FLOW = "cfg-must-constant"
PREFIX = "deterministic-prefix"
CODE_ALIAS = "CODE_POINTER_SOURCE_ALIAS_CLOSED"
CODE_ALIAS_OWNER = "historical:source-address-alias"
CODE_ALIAS_CLAIM = "historical code-pointer label resolved as a zero-byte alias to proved global text; no data storage claimed"


class UnnamedDataError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise UnnamedDataError(message)


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def analysis_hash() -> str:
    """Changing either analyzer requires explicit private recapture/review."""
    return digest(b"\0".join(Path(module).read_bytes() for module in
                            (__file__, ee_dataflow.__file__, rom_offsets.__file__)))


def signed16(value: int) -> int:
    return value if value < 0x8000 else value - 0x10000


def branch(word: int) -> bool:
    op = word >> 26
    return op in BRANCHES or op in (16, 17, 18) and (word >> 21 & 31) == 8


def jump(word: int) -> bool:
    return word >> 26 in (2, 3) or word >> 26 == 0 and word & 63 in (8, 9)


def scan_body(body: bytes, base: int) -> list[dict]:
    """Conservative local constant propagation; clear at every control join."""
    if base % 4 or len(body) % 4:
        fail("unaligned instruction span")
    words = list(struct.unpack("<" + "I" * (len(body) // 4), body))
    boundaries, delays = {base}, set()
    for index, word in enumerate(words):
        pc = base + index * 4
        if branch(word):
            boundaries.add(pc + 4 + signed16(word & 0xFFFF) * 4)
        if branch(word) or jump(word):
            boundaries.add(pc + 8)
            delays.add(pc + 4)
        if word >> 26 in (2, 3):
            boundaries.add(((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2))
    regs = {0: (0, ())}
    accesses = []
    pending_call = None

    def assign(register: int, value: int, trace: tuple[int, ...]) -> None:
        # Reject overflow/high-address arithmetic rather than assuming a
        # 32-bit wrap is also valid for every 64-bit EE arithmetic variant.
        if 0 <= value < 0x80000000 and len(trace) <= 8:
            regs[register] = (value, trace)
        else:
            regs.pop(register, None)

    for index, word in enumerate(words):
        pc = base + index * 4
        if pc in boundaries:
            regs = {0: (0, ())}
        op, rs, rt, rd, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word & 63
        immediate = word & 0xFFFF
        if op in MEMORY:
            if rs in regs:
                address = regs[rs][0] + signed16(immediate)
                mnemonic, width = MEMORY[op]
                if 0 <= address < 0x80000000 and address % width == 0 and regs[rs][1]:
                    accesses.append({"address": address, "width": width, "pc": pc,
                        "opcode": mnemonic, "base_register": rs, "trace": regs[rs][1]})
            if op not in STORES and op not in (49, 54):
                regs.pop(rt, None)
        elif op == 15:
            assign(rt, immediate << 16, (pc,))
        elif op in (8, 9, 24, 25, 12, 13, 14):
            previous = regs.get(rs)
            regs.pop(rt, None)
            if previous is not None:
                value, trace = previous
                value = (value + signed16(immediate) if op in (8, 9, 24, 25)
                         else value & immediate if op == 12
                         else value | immediate if op == 13 else value ^ immediate)
                assign(rt, value, trace + (pc,))
        elif op == 0 and fn in (33, 45, 37, 38, 36):
            first, second = regs.get(rs), regs.get(rt)
            regs.pop(rd, None)
            if first is not None and second is not None:
                a, ta = first
                b, tb = second
                value = (a + b if fn in (33, 45) else a | b if fn == 37
                         else a ^ b if fn == 38 else a & b)
                assign(rd, value, tuple(sorted(set(ta + tb))) + (pc,))
        elif branch(word) or jump(word):
            # Link instructions write before the delay slot executes.
            if op in (1, 3):
                regs.pop(31, None)
            if op == 0 and fn == 9:
                regs.pop(rd, None)
            if op == 3:
                callee = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
                pending_call = (callee, pc)
        elif word == 0:
            pass
        elif op == 0 and fn not in (12, 13):
            regs.pop(rd, None)
        else:
            # Unknown instructions/syscalls are barriers, not opportunities to
            # guess register effects or exploit ABI assumptions across calls.
            regs = {0: (0, ())}
        regs[0] = (0, ())
        if pc in delays:
            if pending_call is not None and pending_call[1] + 4 == pc and pending_call[0] in CALLS:
                callee, call_pc = pending_call
                name, arguments, _member = CALLS[callee]
                count = regs.get(6)
                if count is not None and 0 < count[0] <= libgcc.TARGET_SIZE:
                    for argument in arguments:
                        start = regs.get(argument)
                        if start is None or not start[1]:
                            continue
                        trace = tuple(sorted(set(start[1] + count[1] + (call_pc,))))
                        if len(trace) <= 8:
                            accesses.append({"address": start[0], "width": count[0], "pc": pc,
                                "opcode": name + (".write" if argument == 4 else ".read"),
                                "base_register": argument, "trace": trace, "callee": callee})
            regs = {0: (0, ())}
            pending_call = None
    return accesses


def strict_spans(root: Path = ROOT) -> dict[int, tuple[int, str]]:
    with (root / "analysis/progress_targets.csv").open(encoding="utf-8", newline="") as stream:
        targets = {int(r["address"], 0) for r in csv.DictReader(stream) if r["status"] == "MATCHING"}
    spans = {}
    for path in sorted((root / "analysis/matching").glob("*.tsv")):
        with path.open(encoding="utf-8", newline="") as stream:
            reader = csv.DictReader(stream, delimiter="\t")
            if not {"object_size", "address", "result", "differing_bytes"} <= set(reader.fieldnames or []):
                continue
            for row in reader:
                if row["result"] != "MATCH" or row["differing_bytes"] != "0" or row.get("unknown_relocations"):
                    continue
                try:
                    address, size = int(row["address"], 0), int(row["object_size"])
                except (TypeError, ValueError):
                    continue
                if address not in targets or size <= 0 or size % 4 or not libgcc.TARGET_BASE <= address < address + size <= 0x1B0000:
                    continue
                if address not in spans or spans[address][0] < size:
                    spans[address] = (size, path.relative_to(root).as_posix())
    return spans


def scan_function(body: bytes, base: int) -> list[dict]:
    local = [{**hit, "proof_kind": LOCAL} for hit in scan_body(body, base)]
    return local + ee_dataflow.scan_body(body, base, MEMORY, CALLS) + ee_dataflow.scan_prefix(body, base, MEMORY, CALLS)


def historical_code_aliases(path: Path) -> list[dict[str, str]]:
    args = argparse.Namespace(
        external_map=path,
        defined_map=source_aliases.DEFAULT_DEFINED,
        progress_manifest=source_aliases.DEFAULT_PROGRESS,
        manifest=source_aliases.DEFAULT_MANIFEST,
        reviews=source_aliases.DEFAULT_REVIEWS,
    )
    aliases = source_aliases.validate_frozen_manifest(args)
    return [
        {"symbol": row["alias"], "category": "target-address-data",
         "provider_kind": "program-data", "owner": CODE_ALIAS_OWNER,
         "resolution_gate": "source-address-alias", "requesters": row["requesters"]}
        for row in aliases
        if row["status"] == source_aliases.PROVED and row["alias"].startswith("LAB_")
    ]


def external_rows(path: Path) -> list[dict[str, str]]:
    rows = [r for r in libgcc.read_table(path, libgcc.EXTERNAL_FIELDS) if r["category"] == "target-address-data"]
    if {r["symbol"] for r in rows} & rom_offsets.SPEC.keys():
        fail("ROM offsets must no longer be live image-address contracts")
    code_aliases = historical_code_aliases(path)
    if {r["symbol"] for r in rows} & {r["symbol"] for r in code_aliases}:
        fail("closed code-pointer aliases remain live image-address contracts")
    rows += rom_offsets.historical_externals()
    rows += code_aliases
    if len(rows) != 1265 or len({r["symbol"] for r in rows}) != 1265:
        fail("Stage-3F must preserve the original 1,265 unique contracts")
    return sorted(rows, key=lambda r: r["symbol"])


def address_of(symbol: str) -> int:
    suffix = symbol.rsplit("_", 1)[-1]
    if len(suffix) != 8:
        fail(f"Stage-3F symbol has no eight-digit target address: {symbol}")
    return int(suffix, 16)


def region_for(address: int, extent: int, layout: dict) -> str:
    if extent <= 0 or address < int(layout["base"]) or address + extent > int(layout["memory_end"]):
        fail("unnamed access outside target RAM")
    end = int(layout["initialized_end"])
    return "initialized" if address + extent <= end else "zero-fill" if address >= end else "initialized+zero-fill"


def empty_row(external: dict[str, str]) -> dict[str, str]:
    row = {**{field: "" for field in FIELDS}, "symbol": external["symbol"],
            "target_address": f"0x{address_of(external['symbol']):08x}", "status": BLOCKED,
            "requesters": external["requesters"], "claim": "indexed/pointer/array/object bounds require separate proof"}
    if external["symbol"] in rom_offsets.SPEC:
        row.update(status=rom_offsets.STATUS, claim=rom_offsets.CLAIM)
    elif external["owner"] == CODE_ALIAS_OWNER:
        row.update(status=CODE_ALIAS, claim=CODE_ALIAS_CLAIM)
    return row


def callee_hashes() -> dict[int, str]:
    import runtime_members
    _rows, objects = runtime_members.validate_manifest(runtime_members.parse_args(["validate"]))
    by_name = {r["member"]: r for r in objects}
    result = {}
    for address, (_name, _arguments, member) in CALLS.items():
        row = by_name[member]
        if row["status"] != runtime_members.EXACT or int(row["target_base"], 0) != address:
            fail("memory-call historical callee identity drift")
        result[address] = row["target_sha256"]
    return result


def capture(args: argparse.Namespace) -> list[dict[str, str]]:
    raw = libgcc.load_reference(args.reference)
    external = external_rows(args.external_map)
    wanted = {address_of(r["symbol"]) for r in external if r["symbol"] not in rom_offsets.SPEC}
    layout = named_data.load_layout(args.layout_manifest)
    callees = callee_hashes()
    selected = {}
    for base, (size, evidence) in sorted(strict_spans().items()):
        offset = base - libgcc.TARGET_BASE
        for hit in scan_function(raw[offset:offset + size], base):
            address = hit["address"]
            if address not in wanted or address + hit["width"] > int(layout["memory_end"]):
                continue
            rank = (-hit["width"], (LOCAL, FLOW, PREFIX).index(hit["proof_kind"]),
                    len(hit["trace"]), hit["pc"], base, hit.get("execution_step", 0))
            if address in selected and selected[address][0] <= rank:
                continue
            selected[address] = (rank, hit, base, size, evidence)
    rows = []
    for item in external:
        row = empty_row(item)
        address = address_of(item["symbol"])
        if row["status"] in (rom_offsets.STATUS, CODE_ALIAS):
            rows.append(row)
            continue
        if address in selected:
            _, hit, base, size, evidence = selected[address]
            width, pc, trace = hit["width"], hit["pc"], hit["trace"]
            row.update({"status": PROVED, "extent_hex": hex(width), "region": region_for(address, width, layout),
                "function_address": f"0x{base:08x}", "function_extent_hex": hex(size),
                "access_address": f"0x{pc:08x}", "access_opcode": hit["opcode"],
                "base_register": str(hit["base_register"]), "trace_addresses": ";".join(f"0x{x:08x}" for x in trace),
                "instruction_window_sha256": digest(raw[min(trace)-libgcc.TARGET_BASE:pc-libgcc.TARGET_BASE+4]),
                "matching_evidence": evidence, "matching_evidence_sha256": digest((ROOT / evidence).read_bytes()),
                "claim": CLAIM, "proof_kind": hit["proof_kind"],
                "function_sha256": digest(raw[base-libgcc.TARGET_BASE:base-libgcc.TARGET_BASE+size]),
                "analysis_sha256": analysis_hash()})
            if hit["proof_kind"] in (FLOW, PREFIX):
                # A CFG proof covers all branch predecessors/backedges. A
                # linear instruction window alone would omit relevant paths.
                row["instruction_window_sha256"] = row["function_sha256"]
            if hit["proof_kind"] == PREFIX:
                row["execution_step"] = str(hit["execution_step"])
            if "callee" in hit:
                row.update({"callee_address": f"0x{hit['callee']:08x}", "callee_sha256": callees[hit["callee"]]})
            row["sha256"] = digest(named_data.range_bytes(raw, row, layout))
        rows.append(row)
    return rows


def validate_manifest(args: argparse.Namespace) -> list[dict[str, str]]:
    external = external_rows(args.external_map)
    rows = libgcc.read_table(args.manifest, FIELDS)
    if [r["symbol"] for r in rows] != [r["symbol"] for r in external]:
        fail("Stage-3F access ledger roster/order drift")
    layout = named_data.load_layout(args.layout_manifest)
    spans = strict_spans()
    callees = callee_hashes()
    evidence_hashes = {}
    profile = analysis_hash()
    for row, ext in zip(rows, external):
        address = address_of(row["symbol"])
        if row["target_address"] != f"0x{address:08x}" or row["requesters"] != ext["requesters"]:
            fail(f"Stage-3F address/requester drift: {row['symbol']}")
        if row["status"] in (BLOCKED, rom_offsets.STATUS, CODE_ALIAS):
            if row != empty_row(ext):
                fail("non-storage address must not claim extent/bytes or instruction evidence")
            continue
        if row["status"] != PROVED or row["claim"] != CLAIM:
            fail("direct access is not a complete-object identity claim")
        if row["proof_kind"] not in (LOCAL, FLOW, PREFIX) or row["analysis_sha256"] != profile:
            fail("access analysis profile/proof kind changed; private recapture required")
        width = int(row["extent_hex"], 0)
        call = bool(row["callee_address"])
        if call:
            callee = int(row["callee_address"], 0)
            if callee not in CALLS or row["callee_sha256"] != callees[callee]:
                fail("memory-call callee/hash drift")
            name, arguments, _ = CALLS[callee]
            argument = int(row["base_register"])
            if (argument not in arguments or width <= 0 or width > libgcc.TARGET_SIZE
                    or row["access_opcode"] != name + (".write" if argument == 4 else ".read")):
                fail("memory-call argument/extent drift")
        elif MNEMONICS.get(row["access_opcode"]) != width or address % width or row["callee_sha256"]:
            fail("access opcode/width/alignment mismatch")
        if row["region"] != region_for(address, width, layout):
            fail("access initialized/zero-fill region mismatch")
        base, size, pc = int(row["function_address"], 0), int(row["function_extent_hex"], 0), int(row["access_address"], 0)
        if spans.get(base) != (size, row["matching_evidence"]):
            fail("access witness not inside a strict matched function span")
        trace = tuple(int(x, 0) for x in row["trace_addresses"].split(";"))
        flow = row["proof_kind"] in (FLOW, PREFIX)
        if row["proof_kind"] == PREFIX:
            if not row["execution_step"].isdigit() or not 1 <= int(row["execution_step"]) <= ee_dataflow.MAX_PREFIX_STEPS:
                fail("invalid deterministic prefix occurrence")
        elif row["execution_step"]:
            fail("only deterministic-prefix witnesses have an execution occurrence")
        trace_end = base + size if flow else pc + (1 if call else 0)
        if (not trace or len(trace) > (ee_dataflow.MAX_TRACE if flow else 8) or trace != tuple(sorted(set(trace)))
                or any(x % 4 or not base <= x < trace_end for x in trace)
                or pc % 4 or not base <= pc < base + size
                or call and pc - 4 not in trace
                or not 1 <= int(row["base_register"]) <= 31):
            fail("invalid local address-construction trace")
        for key in ("sha256", "instruction_window_sha256", "matching_evidence_sha256", "function_sha256", "analysis_sha256"):
            if not libgcc.SHA_RE.fullmatch(row[key]):
                fail(f"missing access proof hash: {key}")
        if flow and row["instruction_window_sha256"] != row["function_sha256"]:
            fail("CFG proof must fingerprint the complete function, including all predecessors")
        evidence = row["matching_evidence"]
        if evidence not in evidence_hashes:
            evidence_hashes[evidence] = digest((ROOT / evidence).read_bytes())
        if row["matching_evidence_sha256"] != evidence_hashes[evidence]:
            fail("strict matching evidence changed")
    return rows


def statistics(rows: Sequence[dict[str, str]]) -> dict:
    proved = [r for r in rows if r["status"] == PROVED]
    rom_closed = sum(r["status"] == rom_offsets.STATUS for r in rows)
    code_closed = sum(r["status"] == CODE_ALIAS for r in rows)
    closed = rom_closed + code_closed
    intervals = sorted({(int(r["target_address"], 0), int(r["target_address"], 0) + int(r["extent_hex"], 0)) for r in proved})
    clusters = []
    for start, end in intervals:
        if clusters and start <= clusters[-1][1]:
            clusters[-1][1] = max(end, clusters[-1][1])
        else:
            clusters.append([start, end])
    return {"contracts_total": len(rows), "direct_access_proved": len(proved),
            "awaiting_direct_access": len(rows) - len(proved) - closed,
            "rom_offset_refactors_closed": rom_closed,
            "code_pointer_source_aliases_closed": code_closed, "unique_addresses": len(intervals),
            "overlap_aware_clusters": len(clusters), "unique_consumed_bytes": sum(b-a for a,b in clusters),
            "access_widths": dict(sorted(Counter(int(r["extent_hex"], 0) for r in proved).items())),
            "constant_call_ranges": sum(bool(r["callee_address"]) for r in proved),
            "proof_kinds": dict(sorted(Counter(r["proof_kind"] for r in proved).items())),
            "complete_object_extents_proved": False, "stage3f_closed": False,
            "scope": "minimum directly accessed spans only; not full objects/arrays or final layout"}


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "verify", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--external-map", type=Path, default=libgcc.DEFAULT_EXTERNAL)
    parser.add_argument("--layout-manifest", type=Path, default=libgcc.DEFAULT_LAYOUT)
    parser.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    parser.add_argument("--report", type=Path, default=ROOT / "build/unnamed-data/report.json")
    args = parser.parse_args(argv)
    for key, value in vars(args).items():
        if isinstance(value, Path):
            setattr(args, key, value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "validate":
            rows = validate_manifest(args)
        else:
            frozen = validate_manifest(args) if args.command == "verify" else None
            rows = capture(args)
            if frozen is not None and rows != frozen:
                fail("private direct-access proofs differ from frozen manifest")
            if args.command == "capture":
                from runtime_members import render
                args.manifest.write_text(render(FIELDS, rows), encoding="utf-8")
                validate_manifest(args)
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps(statistics(rows), indent=2, sort_keys=True) + "\n", encoding="utf-8")
        report = statistics(rows)
        print(f"verified Stage-3F direct accesses: proved={report['direct_access_proved']}/{report['contracts_total']} "
              f"unproved={report['awaiting_direct_access']} rom_refactors={report['rom_offset_refactors_closed']} "
              f"code_aliases={report['code_pointer_source_aliases_closed']} unique_bytes={report['unique_consumed_bytes']} "
              "(minimum accesses only; Stage 3F remains open)")
        return 0
    except (UnnamedDataError, libgcc.LibgccContractError, named_data.NamedDataError, OSError, KeyError, ValueError) as error:
        print(f"unnamed data: FAIL: {error}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
