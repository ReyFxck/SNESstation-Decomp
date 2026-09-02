import random
import struct
import unittest
from unittest.mock import patch
import ee_dataflow as f
import unnamed_data as d
from test_ee_dataflow import ins, reg, BASE


def scan(*words):
    return f.scan_prefix(struct.pack("<"+"I"*len(words), *words), BASE, d.MEMORY, d.CALLS)


class PrefixDataflowTests(unittest.TestCase):
    def test_partial_load_clobbers_only_destination_without_width_claim(self):
        for op in (26, 27, 34, 38):
            hits = scan(ins(15, rt=2, imm=0x35), ins(15, rt=4, imm=0x36),
                        ins(op, rs=4, rt=2), ins(35, rs=2, rt=3), ins(35, rs=4, rt=3))
            self.assertEqual([0x360000], [h["address"] for h in hits])

    def test_partial_store_preserves_gprs_without_width_claim(self):
        for op in (42, 44, 45, 46):
            hits = scan(ins(15, rt=2, imm=0x35), ins(op, rs=2, rt=3), ins(35, rs=2, rt=3))
            self.assertEqual([0x350000], [h["address"] for h in hits])

    def loop(self, count, stride):
        return scan(ins(15, rt=2, imm=0x35), ins(9, rt=4, imm=count),
                    ins(35, rs=2, rt=3), ins(9, rs=2, rt=2, imm=stride),
                    ins(9, rs=4, rt=4, imm=-1), ins(5, rs=4, imm=-4), 0)

    def test_finite_counted_loop_records_separate_occurrences(self):
        hits = self.loop(8, 4)
        self.assertEqual([0x350000+4*i for i in range(8)], [h["address"] for h in hits])
        self.assertEqual(list(range(3, 43, 5)), [h["execution_step"] for h in hits])
        self.assertTrue(all(h["proof_kind"] == d.PREFIX for h in hits))

    def test_random_counted_loops_match_independent_arithmetic(self):
        rng = random.Random(97)
        for _ in range(100):
            count, stride = rng.randint(1, 50), rng.randint(1, 20)*4
            self.assertEqual([0x350000+stride*i for i in range(count)],
                             [h["address"] for h in self.loop(count, stride)])

    def test_unknown_branch_stops_before_delay_slot(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), ins(4, rs=8, rt=9, imm=2),
                                  ins(35, rs=2, rt=3), ins(35, rs=2, rt=3), 0))

    def test_branch_uses_pre_delay_condition(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(9, rt=4, imm=1),
                    ins(5, rs=4, imm=2), ins(9, rt=4, imm=0),
                    ins(35, rs=2, rt=3), ins(35, rs=2, rt=3, imm=4))
        self.assertEqual([0x350004], [h["address"] for h in hits])

    def test_likely_false_annuls_delay(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(9, rt=4, imm=1),
                    ins(20, rs=4, imm=2), ins(15, rt=2, imm=0x36), ins(35, rs=2, rt=3), 0)
        self.assertEqual([0x350000], [h["address"] for h in hits])

    def test_call_stops_after_delay(self):
        hits = scan(ins(15, rt=2, imm=0x35), 0x0c000000 | (0x180000 >> 2),
                    ins(35, rs=2, rt=3), ins(35, rs=2, rt=3))
        self.assertEqual([BASE+8], [h["pc"] for h in hits])

    def test_link_register_is_unknown_in_call_delay(self):
        self.assertEqual([], scan(ins(15, rt=31, imm=0x35), 0x0c000000 | (0x180000 >> 2),
                                  ins(35, rs=31, rt=3)))

    def test_memory_call_proves_only_explicit_count(self):
        hits = scan(ins(15, rt=4, imm=0x35), 0x0c000000 | (0x19c39c >> 2), ins(9, rt=6, imm=20))
        self.assertEqual([(0x350000, 20)], [(h["address"], h["width"]) for h in hits])

    def test_memory_contents_do_not_become_pointer_constants(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=2), ins(35, rs=2, rt=3))
        self.assertEqual(1, len(hits))

    def test_unknown_instruction_stops_and_does_not_restart(self):
        self.assertEqual([], scan(0x48ffffff, ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3)))

    def test_budget_exhaustion_discards_partial_loop_claim(self):
        with patch.object(f, "MAX_PREFIX_STEPS", 8):
            self.assertEqual([], self.loop(30, 4))

    def test_indirect_jump_cannot_select_an_address(self):
        self.assertEqual([], scan(reg(8, rs=8), 0, ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3)))

    def test_truncated_or_nested_delay_is_rejected(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3), reg(8, rs=31)))
        self.assertEqual([], scan(ins(4, imm=2), ins(4, imm=1), 0, 0))

    def test_target_into_delay_is_rejected(self):
        self.assertEqual([], scan(ins(4, imm=0), ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3)))

    def test_slti_signed_unsigned_negative_immediate(self):
        state = {**f.ZERO, 2: (7, (BASE,))}
        self.assertEqual(0, f.transfer(state, ins(10, rs=2, rt=3, imm=-1), BASE+4, {}, {})[3][0])
        self.assertEqual(1, f.transfer(state, ins(11, rs=2, rt=3, imm=-1), BASE+4, {}, {})[3][0])

    def test_mult_tracks_only_nonoverflowing_products(self):
        state = {**f.ZERO, 2: (11, (BASE,)), 3: (12, (BASE+4,))}
        self.assertEqual(132, f.transfer(state, reg(24, rs=2, rt=3, rd=4), BASE+8, {}, {})[4][0])
        state[2] = (0x40000000, (BASE,))
        self.assertNotIn(4, f.transfer(state, reg(25, rs=2, rt=3, rd=4), BASE+8, {}, {}))

    def test_unaligned_input_rejected(self):
        self.assertEqual([], f.scan_prefix(b"x", BASE, d.MEMORY, {}))
        self.assertEqual([], f.scan_prefix(bytes(8), BASE+1, d.MEMORY, {}))
