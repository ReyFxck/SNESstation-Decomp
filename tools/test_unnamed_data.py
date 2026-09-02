import struct
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import libgcc_contracts as libgcc
import runtime_members
import unnamed_data as data


def ins(op, rs=0, rt=0, imm=0):
    return op << 26 | rs << 21 | rt << 16 | (imm & 0xFFFF)


def reg(fn, rs=0, rt=0, rd=0):
    return rs << 21 | rt << 16 | rd << 11 | fn


def scan(*words):
    return data.scan_body(struct.pack("<" + "I" * len(words), *words), 0x100000)


class UnnamedDataScannerTests(unittest.TestCase):
    def test_lui_load_proves_actual_byte_width_not_lift_uint64(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(36, rs=2, rt=3, imm=0x123))
        self.assertEqual(1, len(hits))
        self.assertEqual((0x350123, 1, (0x100000,)), (hits[0]["address"], hits[0]["width"], hits[0]["trace"]))

    def test_signed_low_half_and_register_self_assignment(self):
        hits = scan(ins(15, rt=2, imm=0x36), ins(9, rs=2, rt=2, imm=-32768), ins(35, rs=2, rt=3))
        self.assertEqual(0x358000, hits[0]["address"])

    def test_ori_and_move_preserve_the_address_trace(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(13, rs=2, rt=2, imm=0x1000),
                    reg(37, rs=2, rd=4), ins(55, rs=4, rt=5))
        self.assertEqual(0x351000, hits[0]["address"])
        self.assertEqual((0x100000, 0x100004, 0x100008), hits[0]["trace"])

    def test_constant_addition_combines_both_inputs(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(9, rt=3, imm=8),
                    reg(33, rs=2, rt=3, rd=4), ins(55, rs=4, rt=5))
        self.assertEqual(0x350008, hits[0]["address"])

    def test_memory_load_does_not_treat_initial_data_as_a_constant_pointer(self):
        hits = scan(ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=2), ins(35, rs=2, rt=3))
        self.assertEqual(1, len(hits))

    def test_unknown_index_cannot_create_an_array_extent(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), reg(33, rs=2, rt=9, rd=2), ins(35, rs=2, rt=3)))

    def test_branch_join_invalidates_incoming_constant_state(self):
        self.assertEqual([], scan(ins(4, rs=3, rt=4, imm=2), 0,
                                  ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3)))

    def test_coprocessor_branch_target_also_invalidates_state(self):
        self.assertEqual([], scan(ins(17, rs=8, rt=1, imm=2), 0,
                                  ins(15, rt=2, imm=0x35), ins(35, rs=2, rt=3)))

    def test_call_delay_executes_before_unknown_callee(self):
        hits = scan(ins(15, rt=2, imm=0x35), 0x0C040000, ins(35, rs=2, rt=3), ins(35, rs=2, rt=4))
        self.assertEqual([0x100008], [h["pc"] for h in hits])

    def test_link_register_is_written_before_the_delay_slot(self):
        self.assertEqual([], scan(ins(15, rt=31, imm=0x35), 0x0C040000, ins(35, rs=31, rt=3)))

    def test_syscall_is_an_unknown_register_barrier(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), 12, ins(35, rs=2, rt=3)))

    def test_unknown_instruction_is_a_barrier(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), ins(28), ins(35, rs=2, rt=3)))

    def test_unknown_gpr_operation_invalidates_its_destination(self):
        self.assertEqual([], scan(ins(15, rt=2, imm=0x35), reg(0, rt=4, rd=2), ins(35, rs=2, rt=3)))

    def test_unaligned_or_partial_transfers_do_not_prove_a_width(self):
        for word in (ins(35, rs=2, rt=3, imm=1), ins(34, rs=2, rt=3), ins(38, rs=2, rt=3)):
            with self.subTest(word=word):
                self.assertEqual([], scan(ins(15, rt=2, imm=0x35), word))

    def test_stores_and_fpu_loads_do_not_clobber_gpr_data_register(self):
        for opcode in (40, 41, 43, 63, 49, 54):
            hits = scan(ins(15, rt=2, imm=0x35), ins(opcode, rs=2, rt=2), ins(35, rs=2, rt=3))
            self.assertEqual(2, len(hits))

    def test_unknown_input_or_high_address_is_not_proved(self):
        self.assertEqual([], scan(ins(35, rs=2, rt=3)))
        self.assertEqual([], scan(ins(15, rt=2, imm=0xFFFF), ins(35, rs=2, rt=3)))

    def test_instruction_stream_alignment_is_checked(self):
        for body, base in ((b"\0", 0x100000), (b"\0" * 4, 0x100001)):
            with self.assertRaisesRegex(data.UnnamedDataError, "unaligned"):
                data.scan_body(body, base)

    def test_memset_range_uses_the_count_set_in_the_delay_slot(self):
        hits = scan(ins(15, rt=4, imm=0x35), 0x0C000000 | (0x19C39C >> 2), ins(9, rt=6, imm=256))
        self.assertEqual(1, len(hits))
        self.assertEqual(("memset.write", 256, 0x350000), (hits[0]["opcode"], hits[0]["width"], hits[0]["address"]))
        self.assertIn(0x100008, hits[0]["trace"])

    def test_memcpy_proves_both_constant_source_and_destination(self):
        hits = scan(ins(15, rt=4, imm=0x35), ins(15, rt=5, imm=0x36),
                    0x0C000000 | (0x19C364 >> 2), ins(9, rt=6, imm=32))
        self.assertEqual(["memcpy.write", "memcpy.read"], [h["opcode"] for h in hits])

    def test_unknown_call_cannot_be_treated_as_memset(self):
        self.assertEqual([], scan(ins(15, rt=4, imm=0x35), 0x0C000000 | (0x19C388 >> 2), ins(9, rt=6, imm=256)))

    def test_nonconstant_or_zero_call_length_is_not_an_extent(self):
        for delay in (0, ins(9, rt=6, imm=0), ins(9, rt=6, imm=-1)):
            self.assertEqual([], scan(ins(15, rt=4, imm=0x35), 0x0C000000 | (0x19C39C >> 2), delay))


class UnnamedDataManifestTests(unittest.TestCase):
    def setUp(self):
        self.args = data.parse_args(["validate"])
        self.rows = libgcc.read_table(data.DEFAULT_MANIFEST, data.FIELDS)
        self.proved = next(r for r in self.rows if r["status"] == data.PROVED)
        self.blocked = next(r for r in self.rows if r["status"] == data.BLOCKED)

    def changed(self, message):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "changed.tsv"
            path.write_text(runtime_members.render(data.FIELDS, self.rows), encoding="utf-8")
            self.args.manifest = path
            with self.assertRaisesRegex(data.UnnamedDataError, message):
                data.validate_manifest(self.args)

    def test_public_gate_needs_no_private_reference(self):
        with patch.object(libgcc, "load_reference", side_effect=AssertionError("no private read")):
            data.validate_manifest(self.args)

    def test_direct_access_counts_do_not_claim_complete_stage3f(self):
        report = data.statistics(self.rows)
        self.assertEqual((1265, 872, 364, 167659), tuple(report[k] for k in
                         ("contracts_total", "direct_access_proved", "awaiting_direct_access", "unique_consumed_bytes")))
        self.assertFalse(report["complete_object_extents_proved"])
        self.assertFalse(report["stage3f_closed"])
        self.assertEqual(10, report["constant_call_ranges"])
        self.assertEqual({data.LOCAL: 693, data.FLOW: 146, data.PREFIX: 33}, report["proof_kinds"])
        self.assertEqual(29, report["rom_offset_refactors_closed"])

    def test_overlaps_are_counted_once(self):
        rows = [dict(self.proved, target_address="0x100000", extent_hex="0x8"),
                dict(self.proved, target_address="0x100004", extent_hex="0x8")]
        self.assertEqual(12, data.statistics(rows)["unique_consumed_bytes"])

    def test_missing_contract_fails(self):
        self.rows.pop()
        self.changed("roster/order")

    def test_minimum_access_cannot_be_promoted_to_whole_object(self):
        self.proved["claim"] = "complete original C object proved"
        self.changed("complete-object")

    def test_blocked_pointer_cannot_claim_a_guessed_size(self):
        self.blocked["extent_hex"] = "0x8"
        self.changed("must not claim")

    def test_target_byte_access_cannot_claim_widened_lift_type(self):
        row = next(r for r in self.rows if r["status"] == data.PROVED and r["extent_hex"] == "0x1")
        row["extent_hex"] = "0x8"
        self.changed("opcode/width")

    def test_zero_fill_must_follow_the_layout_contract(self):
        self.proved["region"] = "unknown"
        self.changed("zero-fill")

    def test_evidence_hash_drift_fails(self):
        self.proved["matching_evidence_sha256"] = "0" * 64
        self.changed("matching evidence changed")

    def test_trace_outside_function_fails(self):
        self.proved["trace_addresses"] = "0x00000000"
        self.changed("construction trace")

    def test_changed_address_or_requester_fails(self):
        self.proved["requesters"] = "not/the/owner.o"
        self.changed("address/requester")

    def test_memory_call_callee_hash_cannot_drift(self):
        row = next(r for r in self.rows if r["callee_address"])
        row["callee_sha256"] = "0" * 64
        self.changed("callee/hash")

    def test_memory_call_argument_role_cannot_drift(self):
        row = next(r for r in self.rows if r["callee_address"])
        row["base_register"] = "7"
        self.changed("argument/extent")

    def test_cfg_proof_cannot_use_a_partial_instruction_window(self):
        row = next(r for r in self.rows if r['proof_kind'] == data.FLOW)
        row['instruction_window_sha256'] = '0'*64
        self.changed("complete function")

    def test_analyzer_drift_requires_private_recapture(self):
        self.proved['analysis_sha256'] = '0'*64
        self.changed("analysis profile")

    def test_unknown_analysis_kind_rejected(self):
        self.proved['proof_kind'] = 'assume-all-paths'
        self.changed("proof kind")


if __name__ == "__main__":
    unittest.main()
