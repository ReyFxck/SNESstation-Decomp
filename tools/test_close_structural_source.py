#!/usr/bin/env python3
from __future__ import annotations

import csv
import importlib.util
import io
import sys
import unittest
from pathlib import Path


TOOL = Path(__file__).with_name("close_structural_source.py")
SPEC = importlib.util.spec_from_file_location("close_structural_source", TOOL)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError(f"could not load {TOOL}")
MODULE = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = MODULE
SPEC.loader.exec_module(MODULE)


class StructuralClosureTransformTests(unittest.TestCase):
    def test_failed_jumptable_scope_is_lowered_to_c_identifier(self) -> None:
        source = """
void FUN_0010d734(void)
{
    (*(code *)(&switchD_0010d764::switchdataD_001b18f8)[0])();
}
"""
        lowered = MODULE.normalize_chunk(source)
        self.assertNotIn("::", lowered)
        self.assertIn("switchdataD_001b18f8", lowered)

    def test_ghidra_array_value_assignment_uses_blob_helpers(self) -> None:
        source = """
void FUN_00114818(void)
{
    undefined local_f0 [8];
    uint hi;
    uint lo;
    local_f0 = (undefined [8])CONCAT44(hi, lo);
    hi = (uint)(ulong)local_f0;
}
"""
        lowered = MODULE.normalize_chunk(source)
        self.assertIn("p28_store_blob(local_f0", lowered)
        self.assertIn("p28_load_blob(local_f0", lowered)
        self.assertNotIn("(undefined [8])", lowered)


    def test_ghidra_partial_storage_notation_is_lowered(self) -> None:
        source = """
void FUN_00123456(void)
{
    undefined8 uVar1;
    undefined auVar2 [16];
    uVar1._4_4_ = 3;
    auVar2._8_8_ = uVar1;
    uVar1 = uVar1._0_4_ + (ulong)auVar2;
}
"""
        lowered = MODULE.normalize_chunk(source)
        self.assertNotIn("._4_4_", lowered)
        self.assertNotIn("._8_8_", lowered)
        self.assertIn("p28_write_piece(&uVar1", lowered)
        self.assertIn("p28_read_piece(&uVar1", lowered)

    def test_intrinsic_width_parsing(self) -> None:
        self.assertEqual(MODULE.parse_width_pair("44", "concat"), (4, 4))
        self.assertEqual(MODULE.parse_width_pair("12", "concat"), (1, 2))
        self.assertEqual(MODULE.parse_width_pair("168", "sub"), (16, 8))
        self.assertEqual(MODULE.parse_width_pair("816", "extend"), (8, 16))

    def test_representative_low_level_corridor_compiles(self) -> None:
        source = r"""
void FUN_00123456(uint x)
{
    undefined local_f0 [8];
    undefined8 uVar1;
    mystery_type local_unknown;

    local_f0 = (undefined [8])CONCAT44(x, x);
    uVar1 = 0;
    uVar1._4_4_ = 3;
    if (DAT_001abc00 != 0) {
        (*(code *)(&switchD_00123480::switchdataD_001abc20)[x & 1])();
    }
    mystery_global = (ulong)local_unknown + (ulong)local_f0 + uVar1._0_4_;
}
"""
        lowered = MODULE.normalize_chunk(source)
        prototype = MODULE.function_prototype(lowered, "0x00123456")
        compiled = MODULE.verify_with_compiler(lowered, [prototype], "cc")
        self.assertIn("typedef uint64_t mystery_type;", compiled)
        self.assertIn("extern uint64_t mystery_global;", compiled)
        self.assertNotIn("::", compiled)
        self.assertNotIn("._4_4_", compiled)


    def test_forward_declaration_does_not_enforce_decompiler_call_arity(self) -> None:
        source = "void FUN_00123456(uint x, uint y)\n{\n    (void)x; (void)y;\n}\n"
        prototype = MODULE.function_prototype(source, "0x00123456")
        self.assertEqual(prototype, "void FUN_00123456();")

    def test_old_style_definition_does_not_enforce_call_arity_or_byte_promotion(self) -> None:
        source = "void snes_p28_00123456(byte x, uint y)\n{\n    (void)x; (void)y;\n}\n"
        lowered = MODULE.lower_function_definition_to_old_style(source, "0x00123456")
        self.assertIn("void snes_p28_00123456(x, y)", lowered)
        self.assertIn("byte x;", lowered)
        corridor = "void snes_p28_00123456();\n" + lowered + "\nvoid caller(void){ snes_p28_00123456(1,2,3); }\n"
        MODULE.verify_with_compiler(corridor, [], "cc")

    def test_scalar_used_as_address_is_cast_only_at_memory_use(self) -> None:
        source = """
void snes_p28_00123456(undefined8 param_1)
{
    undefined8 uVar1;
    uVar1 = param_1 + 4;
    *uVar1 = *uVar1 + 1;
    param_1[2] = 7;
}
"""
        lowered = MODULE.normalize_chunk(source, "0x00123456")
        self.assertIn("*(undefined8 *)(uintptr_t)(uVar1)", lowered)
        self.assertIn("((undefined8 *)(uintptr_t)(param_1))[2]", lowered)
        self.assertIn("uVar1 = param_1 + 4", lowered)
        MODULE.verify_with_compiler(lowered, [MODULE.function_prototype(lowered, "0x00123456")], "cc")

    def test_known_void_return_misrecovery_gets_evidence_backed_float_override(self) -> None:
        source = "void snes_p28_0012bf5c(float x)\n{\n    (void)x;\n}\n"
        lowered = MODULE.lower_function_definition_to_old_style(source, "0x0012bf5c")
        self.assertTrue(lowered.startswith("float snes_p28_0012bf5c(x)"))

    def test_compiler_guided_array_repairs_are_narrow(self) -> None:
        body = "void f(void) {\n  x[y] = 1;\n}\n"
        source = "preamble\n" + body
        stderr = "/tmp/x.c:3:4: error: subscripted value is neither array nor pointer nor vector\n"
        repaired, count = MODULE.apply_line_diagnostic_repairs(body, source, stderr)
        self.assertEqual(count, 1)
        self.assertIn("(uint8_t *)(uintptr_t)(x)", repaired)

    def test_compiler_error_summary_keeps_only_real_errors_with_source_context(self) -> None:
        stderr = "/tmp/x.c:2:7: error: expected expression before ';' token\n/tmp/x.c:3:1: warning: ignored warning\n"
        source = "one\ntwo\nthree\n"
        summary = MODULE.compiler_error_summary(stderr, source)
        self.assertIn("line 2: error: expected expression", summary)
        self.assertIn("> 00002 | two", summary)
        self.assertNotIn("ignored warning", summary)

    def test_global_deref_inside_subscript_is_lowered(self) -> None:
        source = """
void FUN_00123456(void)
{
    DAT_003453b8 = DAT_003453b8 + (&DAT_003f44a8)[*DAT_00345498];
}
"""
        lowered = MODULE.normalize_chunk(source, "0x00123456")
        self.assertNotIn("[*DAT_00345498]", lowered)
        self.assertIn("*(uint8_t *)(uintptr_t)(DAT_00345498)", lowered)

    def test_indirect_function_pointer_diagnostic_repair(self) -> None:
        line = "  (*(uint8_t *)(uintptr_t)(DAT_0035f990))(a,b,c);"
        repaired = MODULE._repair_indirect_callable(line)
        self.assertEqual(
            repaired,
            "  (*(code *)(uintptr_t)(DAT_0035f990))(a,b,c);",
        )

    def test_address_minus_pointer_diagnostic_repair(self) -> None:
        line = "  if (0xffff < local_d8 - DAT_0034e2b8) {"
        repaired = MODULE._repair_pointer_subtraction(line)
        self.assertIn("(uintptr_t)(local_d8)", repaired)
        self.assertIn("(uintptr_t)(DAT_0034e2b8)", repaired)
        self.assertNotIn("local_d8 - DAT_0034e2b8", repaired)

    def test_12bf5c_returns_observed_float_conversion(self) -> None:
        source = """
void snes_p28_0012bf5c(float param_1)
{
    undefined8 uVar1;
    FUN_001a3cc0(uVar1);
    return;
}
"""
        lowered = MODULE.normalize_chunk(source, "0x0012bf5c")
        self.assertIn("float snes_p28_0012bf5c(param_1)", lowered)
        self.assertIn("return (float)FUN_001a3cc0(uVar1);", lowered)
        self.assertNotIn("\n  return;", lowered)

    def test_array_element_cast_is_not_rewritten_as_blob_scalar(self) -> None:
        source = """
void FUN_00123456(void)
{
    undefined local_10d3[3];
    DAT_00345486 = (ushort)local_10d3[0];
}
"""
        lowered = MODULE.normalize_chunk(source, "0x00123456")
        self.assertIn("(ushort)local_10d3[0]", lowered)
        self.assertNotIn("p28_load_blob(local_10d3, sizeof(local_10d3))[0]", lowered)

    def test_repeated_diagnostic_lines_are_all_repaired(self) -> None:
        body = """
void a()
{
  if (0xffff < local_d4 - DAT_0034e2b8) {
  }
}
void b()
{
  if (0xffff < local_d4 - DAT_0034e2b8) {
  }
}
""".lstrip()
        source = MODULE.assemble_source(body, [])
        line_no = next(
            i for i, line in enumerate(source.splitlines(), 1)
            if line == "  if (0xffff < local_d4 - DAT_0034e2b8) {"
        )
        stderr = f"/tmp/x.c:{line_no}:25: error: invalid operands to binary - (have 'int' and 'unsigned char *')\n"
        repaired, count = MODULE.apply_line_diagnostic_repairs(body, source, stderr)
        self.assertEqual(count, 2)
        self.assertEqual(repaired.count("(uintptr_t)(local_d4)"), 2)
        self.assertEqual(repaired.count("(uintptr_t)(DAT_0034e2b8)"), 2)

    def test_ghidra_uint5_cast_compiles_in_generated_preamble(self) -> None:
        body = """
void snes_p28_00123456()
{
    if (((uint5)DAT_00345508 & 0xffff000000ULL) == 0) {
        return;
    }
}
"""
        source = MODULE.assemble_source(body, ["void snes_p28_00123456();"])
        import subprocess, tempfile
        from pathlib import Path
        with tempfile.TemporaryDirectory() as tmp:
            cfile = Path(tmp) / "uint5.c"
            cfile.write_text(source)
            result = subprocess.run(
                ["cc", "-std=c11", "-fsyntax-only", str(cfile)],
                text=True, capture_output=True, check=False,
            )
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_promotion_regeneration_replaces_only_managed_rows(self) -> None:
        existing = [
            {
                "address": "0x00100000",
                "source_file": "src/ps2/hand.c",
                "evidence": "analysis/hand.asm",
                "notes": "hand",
            },
            {
                "address": "0x00100004",
                "source_file": MODULE.OUTPUT_REL,
                "evidence": "analysis/functions/progress16_r5900_pseudocode.c.txt",
                "notes": "old generated",
            },
        ]
        lifted = [
            MODULE.LiftFunction(
                address="0x00100004",
                snapshot="P16",
                manifest_name="sample",
                area="test",
                original_name="FUN_00100004",
                return_type="void",
                params="void",
                raw_chunk="void FUN_00100004(void) { return; }\n",
            )
        ]
        content = MODULE.promotion_csv(
            existing,
            {"0x00100004"},
            lifted,
        )
        rows = list(csv.DictReader(io.StringIO(content)))
        self.assertEqual([row["address"] for row in rows], ["0x00100000", "0x00100004"])
        self.assertEqual(rows[0]["source_file"], "src/ps2/hand.c")
        self.assertEqual(rows[1]["source_file"], MODULE.OUTPUT_REL)


if __name__ == "__main__":
    unittest.main()
