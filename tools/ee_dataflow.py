#!/usr/bin/env python3
"""Conservative must-constant GPR analysis for ordinary R5900 control flow.

Facts survive a join only when EVERY reachable predecessor agrees. Memory
contents are never constants, calls clobber all tracked GPRs, and uncertain
instructions are barriers. Indirect jumps and malformed delay-slot graphs
disable this additional proof; the older block-local analysis remains usable.
Instruction encodings follow the pinned EE binutils-2.14 mips-opc.c contract.
This module proves effective access addresses, never array/object bounds.
"""
from __future__ import annotations

from collections import deque
from dataclasses import dataclass
import struct

MAX_TRACE = 24
MAX_PREFIX_STEPS = 4096
Fact = tuple[int, tuple[int, ...]]
State = dict[int, Fact]
ZERO: State = {0: (0, ())}


class UnknownInstruction(ValueError):
    """The deterministic prefix cannot cross an instruction we do not model."""


def signed16(value: int) -> int:
    return value - 0x10000 if value & 0x8000 else value


def traced(*facts: Fact, pc: int | None = None) -> tuple[int, ...]:
    values = {address for _value, trace in facts for address in trace}
    if pc is not None:
        values.add(pc)
    return tuple(sorted(values))


def assign(state: State, register: int, value: int, trace: tuple[int, ...]) -> None:
    if register and 0 <= value < 0x80000000 and len(trace) <= MAX_TRACE:
        state[register] = (value, trace)
    elif register:
        state.pop(register, None)
    state[0] = (0, ())


def meet(left: State | None, right: State) -> State:
    if left is None:
        return dict(right)
    result = dict(ZERO)
    for register in left.keys() & right.keys():
        if left[register][0] == right[register][0]:
            assign(result, register, left[register][0], traced(left[register], right[register]))
    return result


# Exact/masked COP1 encodings that do not write ANY GPR. Do not interpret FPU
# values, conflate FPR rt with GPR rt, or treat an arbitrary coprocessor opcode
# as harmless. R5900 sqrt.s uses the T field, unlike the generic MIPS variant.
FPU_PRESERVES_GPRS = (
    (0x44800000, 0xFFE007FF),  # mtc1
    (0x44C00000, 0xFFE007FF),  # ctc1
    *((0x46000000 | fn, 0xFFE0003F) for fn in (0, 1, 2, 3, 0x16, 0x1C, 0x1D, 0x28, 0x29)),
    *((0x46000000 | fn, 0xFFFF003F) for fn in (5, 6, 7, 0x24)),
    (0x46000004, 0xFFE0F83F),  # EE sqrt.s
    (0x46800020, 0xFFFF003F),  # cvt.s.w
    *((0x46000000 | fn, 0xFFE007FF) for fn in (0x18, 0x19, 0x1A, 0x1E, 0x1F, 0x30, 0x32, 0x34, 0x36)),
)
FPU_WRITES_RT = ((0x44000000, 0xFFE007FF), (0x44400000, 0xFFE007FF))
MMI_WRITES_RD = (
    *((0x70000000 | fn, 0xFC0007FF) for fn in (0, 1, 0x18, 0x19, 0x20, 0x21)),
    (0x70000010, 0xFFFF07FF), (0x70000012, 0xFFFF07FF),
)
MMI_PRESERVES_GPRS = (
    (0x70000011, 0xFC1FFFFF), (0x70000013, 0xFC1FFFFF),
    (0x7000001A, 0xFC00FFFF), (0x7000001B, 0xFC00FFFF),
    (0x04180000, 0xFC1F0000), (0x04190000, 0xFC1F0000),  # mtsab/mtsah
)


def transfer(state: State, word: int, pc: int, memory: dict, calls: dict,
             hits: list[dict] | None = None, *, strict: bool = False) -> State:
    """Apply one NON-control instruction; values outside low positive RAM drop."""
    result = dict(state)
    op, rs, rt, rd, sa, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word >> 6 & 31, word & 63
    imm = word & 0xFFFF
    if op in memory:
        if rs in state:
            value, trace = state[rs]
            name, width = memory[op]
            address = value + signed16(imm)
            if hits is not None and trace and 0 <= address < 0x80000000 and address % width == 0:
                hits.append({"address": address, "width": width, "pc": pc, "opcode": name,
                             "base_register": rs, "trace": trace, "proof_kind": "cfg-must-constant"})
        if op not in (40, 41, 43, 63, 31, 57, 62, 49, 54):
            result.pop(rt, None)
    elif op in (26, 27, 34, 38):
        # Partial unaligned loads write rt, but prove no fixed byte width.
        result.pop(rt, None)
    elif op in (42, 44, 45, 46):
        pass  # Partial stores read GPRs; their data extent remains unproved.
    elif op == 15 and rs == 0:
        assign(result, rt, imm << 16, (pc,))
    elif op in (8, 9, 24, 25, 12, 13, 14):
        result.pop(rt, None)
        if rs in state:
            value = state[rs][0]
            value = (value + signed16(imm) if op in (8, 9, 24, 25) else
                     value & imm if op == 12 else value | imm if op == 13 else value ^ imm)
            assign(result, rt, value, traced(state[rs], pc=pc))
    elif op in (10, 11):
        result.pop(rt, None)
        if rs in state:
            immediate = signed16(imm)
            value = (int(state[rs][0] < immediate) if op == 10 or immediate >= 0 else 1)
            assign(result, rt, value, traced(state[rs], pc=pc))
    elif op == 0 and fn in (24, 25) and sa == 0:
        # EE three-register mult[u]; retain only non-overflowing low positives.
        result.pop(rd, None)
        if rs in state and rt in state:
            assign(result, rd, state[rs][0] * state[rt][0], traced(state[rs], state[rt], pc=pc))
    elif op == 0 and fn in (33, 35, 45, 47, 36, 37, 38) and sa == 0:
        result.pop(rd, None)
        if rs in state and rt in state:
            a, b = state[rs][0], state[rt][0]
            value = (a+b if fn in (33, 45) else a-b if fn in (35, 47) else
                     a & b if fn == 36 else a | b if fn == 37 else a ^ b)
            assign(result, rd, value, traced(state[rs], state[rt], pc=pc))
    elif op == 0 and fn in (0, 2, 3, 56, 58, 59, 60, 62, 63) and rs == 0:
        result.pop(rd, None)
        if rt in state:
            count = sa + (32 if fn >= 60 else 0)
            value = state[rt][0] << count if fn in (0, 56, 60) else state[rt][0] >> count
            assign(result, rd, value, traced(state[rt], pc=pc))
    elif op == 0 and fn in (10, 11) and sa == 0:  # movz/movn
        result.pop(rd, None)
        if rt in state:
            selected = rs if (state[rt][0] == 0) == (fn == 10) else rd
            if selected in state:
                assign(result, rd, state[selected][0], traced(state[selected], state[rt], pc=pc))
        elif rs in state and rd in state and state[rs][0] == state[rd][0]:
            assign(result, rd, state[rd][0], traced(state[rs], state[rd], pc=pc))
    elif op == 0 and fn in (4, 6, 7, 16, 18, 20, 22, 23, 24, 25, 32, 34, 39, 40, 42, 43, 44, 46):
        # Known instructions write rd (or only HI/LO for mult with rd=0).
        # Their result is deliberately unknown, even when more can be inferred.
        result.pop(rd, None)
    elif op == 0 and ((fn in (17, 19, 41) and (word & 0x001FFFFF) == fn)
                      or (fn in (26, 27, 30, 31) and (word & 0xFFFF) == fn)
                      or word in (0, 0xF, 0x40F)):
        pass  # mthi/mtlo/mtsa, div[u]/ddiv[u], nop, sync[.p]
    elif any(word & mask == match for match, mask in FPU_PRESERVES_GPRS):
        pass
    elif any(word & mask == match for match, mask in FPU_WRITES_RT):
        result.pop(rt, None)
    elif any(word & mask == match for match, mask in MMI_WRITES_RD):
        result.pop(rd, None)
    elif any(word & mask == match for match, mask in MMI_PRESERVES_GPRS):
        pass
    else:
        # In particular unknown MMI/COP0/COP2 encodings, syscalls and traps are
        # barriers. Do not assume destination fields for reserved instructions.
        if strict:
            raise UnknownInstruction(f"unmodelled instruction at 0x{pc:08x}")
        result = dict(ZERO)
    result[0] = (0, ())
    return result


@dataclass(frozen=True)
class Control:
    kind: str
    target: int | None = None
    likely: bool = False
    link_register: int | None = None


def control(word: int, pc: int) -> Control | None:
    op, rs, rt, rd, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word & 63
    if op in (2, 3):
        target = ((pc+4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
        return Control("jump" if op == 2 else "call", target, link_register=31 if op == 3 else None)
    if op == 0 and fn == 8:
        return Control("return" if rs == 31 and word & 0x001FFFFF == 8 else "indirect")
    if op == 0 and fn == 9:
        return Control("call", link_register=rd)
    if op in (4, 5, 6, 7, 20, 21, 22, 23):
        # blez/bgtz require rt=0. Malformed control encodings disable the graph.
        if op in (6, 7, 22, 23) and rt != 0:
            return Control("invalid")
        # beq zero,zero is the ordinary unconditional b pseudo instruction.
        unconditional = op == 4 and rs == rt == 0
        return Control("jump" if unconditional else "branch", pc+4+signed16(word & 0xFFFF)*4, op >= 20)
    if op == 1:
        if rt in (24, 25):
            return None  # R5900 mtsab/mtsah, not REGIMM branches
        if rt not in (0, 1, 2, 3, 16, 17, 18, 19):
            return Control("invalid")
        return Control("branch", pc+4+signed16(word & 0xFFFF)*4, rt in (2, 3, 18, 19),
                       31 if rt in (16, 17, 18, 19) else None)
    if op in (16, 17, 18) and rs == 8:
        if rt not in (0, 1, 2, 3):
            return Control("invalid")
        return Control("branch", pc+4+signed16(word & 0xFFFF)*4, rt in (2, 3))
    if op == 29 or op == 16 and rs >= 16:
        return Control("invalid")  # jalx, eret and privileged control operations
    return None


def memory_call(state: State, ctl: Control, pc: int, calls: dict, hits: list[dict]) -> None:
    if ctl.target not in calls or 6 not in state:
        return
    name, arguments, _member = calls[ctl.target]
    count = state[6][0]
    if not 0 < count <= 3_304_936:
        return
    for argument in arguments:
        if argument not in state or not state[argument][1]:
            continue
        trace = traced(state[argument], state[6], pc=pc)
        if len(trace) <= MAX_TRACE:
            hits.append({"address": state[argument][0], "width": count, "pc": pc+4,
                         "opcode": name + (".write" if argument == 4 else ".read"),
                         "base_register": argument, "trace": trace, "callee": ctl.target,
                         "proof_kind": "cfg-must-constant"})


def branch_taken(word: int, state: State) -> bool | None:
    """Use only fully known low-positive integer operands; never choose a path."""
    op, rs, rt = word >> 26, word >> 21 & 31, word >> 16 & 31
    if rs not in state:
        return None
    a = state[rs][0]
    if op in (4, 5, 20, 21) and rt in state:
        return (a == state[rt][0]) == (op in (4, 20))
    if op in (6, 7, 22, 23) and rt == 0:
        return a == 0 if op in (6, 22) else a > 0
    if op == 1 and rt in (0, 1, 2, 3):
        return rt in (1, 3)  # tracked values are nonnegative
    return None


def scan_prefix(body: bytes, base: int, memory: dict, calls: dict) -> list[dict]:
    """Bounded concrete prefix from the real entry; no input/memory assumptions.

    Each loop occurrence is a separate witness, NOT a loop invariant. Stop at
    the first unknown branch, unknown instruction, or call; do not resume after
    it. Reaching the budget discards every prefix hit, rather than claiming a
    finite execution for an unproved loop.
    """
    if base % 4 or len(body) % 4 or not body:
        return []
    words = struct.unpack("<" + "I" * (len(body) // 4), body)
    end = base + len(body)
    controls = {base + i*4: ctl for i, word in enumerate(words)
                if (ctl := control(word, base + i*4)) is not None}
    delays = {pc + 4 for pc in controls}
    if any(pc >= end or pc in controls for pc in delays):
        return []
    if any(ctl.kind in ("jump", "branch") and ctl.target in delays for ctl in controls.values()):
        return []
    pc, steps, state, hits = base, 0, dict(ZERO), []

    def execute(address: int, current: State) -> State:
        nonlocal steps
        steps += 1
        found: list[dict] = []
        result = transfer(current, words[(address-base)//4], address, memory, calls, found, strict=True)
        hits.extend({**hit, "proof_kind": "deterministic-prefix", "execution_step": steps} for hit in found)
        return result

    try:
        while base <= pc < end:
            if steps >= MAX_PREFIX_STEPS:
                return []
            ctl = controls.get(pc)
            if ctl is None:
                state = execute(pc, state)
                pc += 4
                continue
            if ctl.kind in ("invalid", "indirect") or ctl.kind == "branch" and ctl.link_register is not None:
                break
            decision = branch_taken(words[(pc-base)//4], state) if ctl.kind == "branch" else True
            if decision is None:
                break
            steps += 1
            if not ctl.likely or decision:
                if steps >= MAX_PREFIX_STEPS:
                    return []
                prior = dict(state)
                if ctl.link_register is not None:
                    prior.pop(ctl.link_register, None)
                state = execute(pc+4, prior)
            if ctl.kind == "call":
                found = []
                memory_call(state, ctl, pc, calls, found)
                hits.extend({**hit, "proof_kind": "deterministic-prefix", "execution_step": steps} for hit in found)
                break
            if ctl.kind == "return":
                break
            pc = ctl.target if decision else pc + 8
            if pc is None or pc % 4:
                break
    except UnknownInstruction:
        pass
    return hits


def scan_body(body: bytes, base: int, memory: dict, calls: dict) -> list[dict]:
    """Reach a fixed point BEFORE emitting hits; transient loop facts are not proofs."""
    if base % 4 or len(body) % 4 or not body:
        return []
    words = struct.unpack("<" + "I"*(len(body)//4), body)
    end = base + len(body)
    controls = {base+i*4: ctl for i, word in enumerate(words)
                if (ctl := control(word, base+i*4)) is not None}
    if any(ctl.kind in ("invalid", "indirect") for ctl in controls.values()):
        return []
    delays = {pc+4 for pc in controls}
    if any(pc in controls or pc >= end for pc in delays):
        return []
    targets = {ctl.target for ctl in controls.values() if ctl.kind in ("branch", "jump") and ctl.target is not None}
    if targets & delays:
        return []
    # A direct call to an interior instruction adds an unmodelled function
    # entry. Reject the graph, rather than treating it as an external call.
    if any(ctl.kind == "call" and ctl.target is not None and base <= ctl.target < end for ctl in controls.values()):
        return []
    if any(ctl.link_register is not None and ctl.kind == "branch"
           and ctl.target is not None and base <= ctl.target < end for ctl in controls.values()):
        return []

    def outgoing(pc: int, state: State, hits: list[dict] | None = None) -> list[tuple[int, State]]:
        ctl = controls.get(pc)
        if ctl is None:
            next_state = transfer(state, words[(pc-base)//4], pc, memory, calls, hits)
            return [(pc+4, next_state)]
        prior = dict(state)
        if ctl.link_register is not None:
            prior.pop(ctl.link_register, None)
        after = transfer(prior, words[(pc-base)//4+1], pc+4, memory, calls, hits)
        if ctl.kind == "call":
            if hits is not None:
                memory_call(after, ctl, pc, calls, hits)
            return [(pc+8, dict(ZERO))]
        if ctl.kind == "return":
            return []
        # Branch-and-link can invoke a returning external target; no callee
        # preservation is assumed on its fallthrough/return continuation.
        if ctl.link_register is not None:
            return [(pc+8, dict(ZERO))]
        if ctl.kind == "jump":
            return [(ctl.target, after)]
        # A likely branch annuls its delay instruction on the untaken path.
        return [(ctl.target, after), (pc+8, prior if ctl.likely else after)]

    incoming: dict[int, State] = {base: dict(ZERO)}
    work = deque([base])
    queued = {base}
    visits = 0
    while work:
        pc = work.popleft()
        queued.remove(pc)
        visits += 1
        # Failing closed avoids unbounded analysis on crafted graphs/long
        # trace cycles. A timeout is not evidence for a memory address.
        if visits > len(words)*128:
            return []
        for target, state in outgoing(pc, incoming[pc]):
            if target is None or not base <= target < end or target in delays:
                continue
            merged = meet(incoming.get(target), state)
            if merged != incoming.get(target):
                incoming[target] = merged
                if target not in queued:
                    work.append(target)
                    queued.add(target)
    hits: list[dict] = []
    for pc in sorted(incoming):
        outgoing(pc, incoming[pc], hits)
    # Delay-slot hits on branch-and-link are possible, but never leak facts
    # into an unmodelled taken subroutine body.
    return hits
