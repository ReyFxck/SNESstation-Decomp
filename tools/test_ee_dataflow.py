import struct
import random
import unittest

import ee_dataflow as flow
import unnamed_data as data

BASE = 0x100000


def ins(op, rs=0, rt=0, imm=0):
    return op << 26 | rs << 21 | rt << 16 | (imm & 0xFFFF)


def reg(fn, rs=0, rt=0, rd=0, sa=0):
    return rs << 21 | rt << 16 | rd << 11 | sa << 6 | fn


def scan(*words):
    return flow.scan_body(struct.pack("<"+"I"*len(words), *words), BASE, data.MEMORY, data.CALLS)


def at(hits, index):
    return [h for h in hits if h["pc"] == BASE+index*4]


class EEMustConstantTests(unittest.TestCase):
    def test_identical_diamond_predecessors_agree(self):
        hits = scan(ins(4, rs=4, rt=5, imm=4), 0, ins(15, rt=2, imm=0x35),
                    ins(4, imm=3), 0, ins(15, rt=2, imm=0x35), 0, ins(35, rs=2, rt=3))
        self.assertEqual(0x350000, at(hits, 7)[0]["address"])
        self.assertEqual((BASE+8, BASE+20), at(hits, 7)[0]["trace"])

    def test_conflicting_diamond_predecessors_reject_address(self):
        hits = scan(ins(4, rs=4, rt=5, imm=4), 0, ins(15, rt=2, imm=0x35),
                    ins(4, imm=3), 0, ins(15, rt=2, imm=0x36), 0, ins(35, rs=2, rt=3))
        self.assertEqual([], at(hits, 7))

    def test_missing_value_on_one_predecessor_rejects_address(self):
        self.assertEqual([], scan(ins(4, rs=4, rt=5, imm=2), 0,
                                  ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3)))

    def test_clobbered_predecessor_rejects_address(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(4, rs=4, rt=5, imm=2), 0,
                    ins(35, rs=6, rt=2), ins(35, rs=2, rt=3))
        self.assertEqual([], at(hits, 4))

    def test_loop_invariant_survives_fixed_point(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3),
                    ins(5, rs=4, rt=5, imm=-2), 0)
        self.assertEqual(0x350000, at(hits, 1)[0]["address"])

    def test_first_iteration_constant_is_not_emitted_as_loop_invariant(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3),
                    ins(9, rs=2, rt=2, imm=4), ins(5, rs=4, rt=5, imm=-3), 0)
        self.assertEqual([], at(hits, 1))

    def test_unconditional_branch_skips_dead_definition(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(4, imm=2), 0,
                    ins(15, rt=2, imm=0x36), ins(35, rs=2, rt=3))
        self.assertEqual(0x350000, at(hits, 4)[0]["address"])

    def test_likely_branch_annuls_delay_definition_on_fallthrough(self):
        hits = scan(ins(20, rs=4, rt=5, imm=2), ins(15, rt=2, imm=0x35),
                    ins(35, rs=2, rt=3), ins(35, rs=2, rt=3))
        self.assertEqual([], hits)

    def test_likely_taken_path_executes_delay_definition(self):
        hits = scan(ins(20, rs=4, rt=5, imm=3), ins(15, rt=2, imm=0x35),
                    ins(4, imm=3), 0, ins(35, rs=2, rt=3), 0, 0)
        self.assertEqual(0x350000, at(hits, 4)[0]["address"])

    def test_ordinary_branch_delay_definition_applies_to_both_paths(self):
        hits = scan(ins(4, rs=4, rt=5, imm=2), ins(15, rt=2, imm=0x35),
                    ins(35, rs=2, rt=3), ins(35, rs=2, rt=3))
        self.assertEqual([0x350000, 0x350000], [h["address"] for h in hits])

    def test_call_delay_access_precedes_total_gpr_clobber(self):
        hits = scan(ins(15, rt=2, imm=0x35), 0x0C000000 | (0x180000 >> 2),
                    ins(35, rs=2, rt=3), ins(35, rs=2, rt=3))
        self.assertEqual([BASE+8], [h["pc"] for h in hits])

    def test_link_register_clobbered_before_call_delay(self):
        self.assertEqual([], scan(ins(15, rt=31, imm=0x35), 0x0C000000 | (0x180000 >> 2), ins(35, rs=31, rt=3)))

    def test_memset_count_survives_branch_delay_and_ori(self):
        hits = scan(ins(4, rs=8, rt=9, imm=3), ins(15, rt=6, imm=1),
                    ins(4, imm=5), 0, ins(15, rt=4, imm=0x3B), ins(9, rs=4, rt=4, imm=0xB748),
                    0x0C000000 | (0x19C39C >> 2), ins(13, rs=6, rt=6, imm=0x7700), 0)
        calls = [h for h in hits if h["opcode"] == "memset.write"]
        self.assertEqual([(0x3AB748, 96000)], [(h["address"], h["width"]) for h in calls])

    def test_initial_memory_contents_do_not_become_constant_pointers(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=2), ins(35, rs=2, rt=3))
        self.assertEqual([BASE+4], [h["pc"] for h in hits])

    def test_register_destination_invalidation_preserves_other_bases(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(15, rt=4, imm=0x36),
                    reg(0x18, rs=8, rt=9, rd=2), ins(35, rs=2, rt=3), ins(35, rs=4, rt=3))
        self.assertEqual([0x360000], [h["address"] for h in hits])

    def test_mfc1_invalidates_only_destination_gpr(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(15, rt=4, imm=0x36),
                    0x44020000, ins(35, rs=2, rt=3), ins(35, rs=4, rt=3))
        self.assertEqual([0x360000], [h["address"] for h in hits])

    def test_cfc1_invalidates_only_destination_gpr(self):
        hits = scan(ins(15, rt=2, imm=0x35), 0x44420000, ins(35, rs=2, rt=3))
        self.assertEqual([], hits)

    def test_mtc1_and_fpu_arithmetic_preserve_gpr_address(self):
        for word in (0x44820000, 0x44C20000, 0x46020800, 0x46020802,
                     0x46020084, 0x468008A0, 0x460008A4, 0x46020832):
            with self.subTest(word=hex(word)):
                hits = scan(ins(15, rt=2, imm=0x35), word, ins(35, rs=2, rt=3))
                self.assertEqual([0x350000], [h["address"] for h in hits])

    def test_unknown_fpu_or_mmi_opcode_remains_barrier(self):
        for word in (0x4600003F, 0x700007FF, 0x48FFFFFF):
            self.assertEqual([], scan(ins(15, rt=2, imm=0x35), word, ins(35, rs=2, rt=3)))

    def test_mmi_mflo1_writes_rd_not_all_gprs(self):
        hits = scan(ins(15, rt=2, imm=0x35), 0x70001812, ins(35, rs=2, rt=4))
        self.assertEqual([0x350000], [h["address"] for h in hits])
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), 0x70001012, ins(35, rs=2, rt=4)))

    def test_mtsa_variants_are_not_branches(self):
        for word in (0x04180000, 0x04190000, 0x00400029):
            self.assertIsNone(flow.control(word, BASE))
            self.assertEqual([0x350000], [h["address"] for h in scan(ins(15, rt=2, imm=0x35), word, ins(35, rs=2, rt=3))])

    def test_movz_true_condition_copies_constant(self):
        hits = scan(ins(15, rt=2, imm=0x35), reg(10, rs=2, rd=4), ins(35, rs=4, rt=3))
        self.assertEqual(0x350000, hits[0]["address"])

    def test_movn_unknown_condition_does_not_invent_destination(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), reg(11, rs=2, rt=8, rd=4), ins(35, rs=4, rt=3)))

    def test_movn_unknown_condition_with_equal_values_is_safe(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(15, rt=4, imm=0x35),
                    reg(11, rs=2, rt=8, rd=4), ins(35, rs=4, rt=3))
        self.assertEqual(0x350000, hits[0]["address"])

    def test_shifted_constants_are_tracked_without_32bit_wrap(self):
        hits = scan(ins(9, rt=2, imm=0x35), reg(0, rt=2, rd=2, sa=16), ins(35, rs=2, rt=3))
        self.assertEqual(0x350000, hits[0]["address"])
        self.assertEqual([], scan(ins(15, rt=2, imm=0x4000), reg(0, rt=2, rd=2, sa=2), ins(35, rs=2, rt=3)))

    def test_indirect_jump_disables_whole_graph(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3), reg(8, rs=8), 0))

    def test_branch_into_delay_slot_disables_graph(self):
        self.assertEqual([], scan(ins(4, rs=4, rt=5, imm=0), ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3)))

    def test_control_in_delay_slot_disables_graph(self):
        self.assertEqual([], scan(ins(4, imm=2), ins(4, imm=1), 0, ins(35, rs=2, rt=3)))

    def test_truncated_control_disables_graph(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3), reg(8, rs=31)))

    def test_internal_call_or_branch_link_disables_graph(self):
        for ctl in (0x0C000000 | ((BASE+12) >> 2), ins(1, rs=8, rt=17, imm=1)):
            self.assertEqual([], scan(ins(15, rt=2, imm=0x35), ctl, 0, ins(35, rs=2, rt=3)))

    def test_unknown_syscall_clobbers_all_facts(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), 12, ins(35, rs=2, rt=3)))

    def test_return_delay_access_is_retained(self):
        hits = scan(ins(15, rt=2, imm=0x35), reg(8, rs=31), ins(35, rs=2, rt=3))
        self.assertEqual([BASE+8], [h["pc"] for h in hits])

    def test_meet_discards_disagreeing_values_and_unbounded_traces(self):
        self.assertEqual(flow.ZERO, flow.meet({**flow.ZERO, 2:(1,(BASE,))}, {**flow.ZERO, 2:(2,(BASE+4,))}))
        trace = tuple(BASE+4*i for i in range(flow.MAX_TRACE))
        self.assertNotIn(2, flow.meet({**flow.ZERO, 2:(1,trace)}, {**flow.ZERO, 2:(1,(BASE+1000,))}))

    def test_unsupported_alignment_produces_no_proof(self):
        self.assertEqual([], flow.scan_body(b"\0", BASE, data.MEMORY, data.CALLS))
        self.assertEqual([], flow.scan_body(b"\0"*4, BASE+1, data.MEMORY, data.CALLS))

    def test_differential_diamonds_against_concrete_branch_paths(self):
        """Independent tiny execution oracle, including taken/annulled delays."""
        rng = random.Random(96)

        def execute(words, initial):
            regs = list(initial)
            observed = {}
            pc, steps = 0, 0

            def ordinary(index):
                word = words[index]
                op, rs, rt = word >> 26, word >> 21 & 31, word >> 16 & 31
                low = word & 65535
                immediate = low-65536 if low & 32768 else low
                if op == 15:
                    regs[rt] = low << 16
                elif op == 9:
                    regs[rt] = (regs[rs]+immediate) & 0xFFFFFFFF
                elif op == 35:
                    observed.setdefault(BASE+index*4, set()).add((regs[rs]+immediate) & 0xFFFFFFFF)
                    regs[rt] = 123  # unknown memory data never has address provenance
                elif word != 0:
                    raise AssertionError(hex(word))
                regs[0] = 0

            while pc < len(words) and steps < 64:
                steps += 1
                word = words[pc]
                op, rs, rt = word >> 26, word >> 21 & 31, word >> 16 & 31
                if op in (4, 5, 20, 21):
                    taken = (regs[rs] == regs[rt]) == (op in (4, 20))
                    displacement = word & 65535
                    if displacement & 32768:
                        displacement -= 65536
                    if op in (4, 5) or taken:
                        ordinary(pc+1)
                    pc = pc+1+displacement if taken else pc+2
                else:
                    ordinary(pc)
                    pc += 1
            return observed

        for _ in range(200):
            words = [ins(15, rt=2, imm=0x35),
                     ins(rng.choice((4, 5, 20, 21)), rs=8, rt=9, imm=4),
                     rng.choice((0, ins(9, rs=2, rt=2, imm=4))),
                     rng.choice((0, ins(15, rt=2, imm=rng.choice((0x35, 0x36))))),
                     ins(4, imm=3), 0,
                     rng.choice((0, ins(15, rt=2, imm=rng.choice((0x35, 0x36))))),
                     0, ins(35, rs=2, rt=3)]
            seen = {}
            for left in range(3):
                for right in range(3):
                    initial = [0]*32
                    initial[8], initial[9] = left, right
                    for pc, values in execute(words, initial).items():
                        seen.setdefault(pc, set()).update(values)
            for hit in scan(*words):
                if hit["pc"] in seen:
                    self.assertEqual({hit["address"]}, seen[hit["pc"]], words)


if __name__ == "__main__":
    unittest.main()
