#!/usr/bin/env bash
set -euo pipefail
cd "${1:-$PWD}"

[[ -f Makefile && -f src/ps2/gslib_hw_recovered.c ]] || {
  echo "Run from the SNESstation-Decomp repository root." >&2
  exit 2
}

grep -q 'match-gslib-hw-listing' Makefile || {
  echo "GSLIB listing probe is missing from this working tree." >&2
  exit 2
}

python3 - <<'PY'
from pathlib import Path

Path("src/ps2/gslib_hw_recovered.c").write_text('/*\n * Historical GSLIB hw.c corridor recovered for SNES Station v0.23.\n * Target corridor: 0x0019bd38..0x0019be6c.\n *\n * Historical evidence: ps2homebrew/gslib source/hw.c\n * commit d9e623a351627e53420f44b00d494346cee5d5a2.\n *\n * Preserve historical source idioms: the DMA helpers were GNU basic inline\n * assembly, not C volatile-MMIO wrappers.\n */\n\nstatic unsigned int VRcount_recovered = 0;\n\n/* 0x0019bd38 */\nvoid VRstart_handler_0019bd38(void)\n{\n    VRcount_recovered++;\n    return;\n}\n\n/* 0x0019bd50 */\nvoid WaitForNextVRstart_0019bd50(int numvrs)\n{\n    VRcount_recovered = 0;\n\n    while (VRcount_recovered < numvrs)\n        ;\n\n    return;\n}\n\n/* 0x0019bd78 */\nint TestVRstart_0019bd78(void)\n{\n    return VRcount_recovered;\n}\n\n/* 0x0019bd88 */\nvoid ClearVRcount_0019bd88(void)\n{\n    VRcount_recovered = 0;\n    return;\n}\n\n/* 0x0019bd98 */\nvoid DmaReset_0019bd98(void)\n{\n    __asm__("\\tsw  $0, 0x1000a080");\n    __asm__("\\tsw  $0, 0x1000a000");\n    __asm__("\\tsw  $0, 0x1000a030");\n    __asm__("\\tsw  $0, 0x1000a010");\n    __asm__("\\tsw  $0, 0x1000a050");\n    __asm__("\\tsw  $0, 0x1000a040");\n    __asm__("\\tli  $2, 0xff1f");\n    __asm__("\\tsw  $2, 0x1000e010");\n    __asm__("\\tsw  $0, 0x1000e000");\n    __asm__("\\tsw  $0, 0x1000e020");\n    __asm__("\\tsw  $0, 0x1000e030");\n    __asm__("\\tsw  $0, 0x1000e050");\n    __asm__("\\tsw  $0, 0x1000e040");\n    __asm__("\\tlw  $2, 0x1000e000");\n    __asm__("\\tori $3,$2,1");\n    __asm__("\\tnop");\n    __asm__("\\tsw  $3, 0x1000e000");\n    __asm__("\\tnop");\n\n    return;\n}\n\n/* 0x0019be20 */\nvoid SendDma02_0019be20(void *DmaTag)\n{\n    __asm__("\\tli $3, 0x1000a000");\n\n    __asm__("\\tsw $4, 0x0030($3)");\n    __asm__("\\tsw $0, 0x0020($3)");\n    __asm__("\\tlw $2, 0x0000($3)");\n    __asm__("\\tori $2, 0x0105");\n    __asm__("\\tsw $2, 0x0000($3)");\n\n    return;\n}\n\n/* 0x0019be40 */\nvoid Dma02Wait_0019be40(void)\n{\n    __asm__("\\taddiu $29, -4");\n    __asm__("\\tsw $8, 0($29)");\n\n    __asm__("Dma02Wait.poll:");\n    __asm__("\\tlw $8, 0x1000a000");\n    __asm__("\\tnop");\n    __asm__("\\tandi $8, $8, 0x0100");\n    __asm__("\\tbnez $8, Dma02Wait.poll");\n    __asm__("\\tnop");\n\n    __asm__("\\tlw $8, 0($29)");\n    __asm__("\\taddiu $29, 4");\n\n    return;\n}\n', encoding="utf-8")
Path("analysis/matching/gslib_hw_listing.csv").write_text('address,end,name,object_symbol,source\n0x0019bd38,0x0019bd4c,VRstart_handler,VRstart_handler_0019bd38,src/ps2/gslib_hw_recovered.c\n0x0019bd50,0x0019bd74,WaitForNextVRstart,WaitForNextVRstart_0019bd50,src/ps2/gslib_hw_recovered.c\n0x0019bd78,0x0019bd84,TestVRstart,TestVRstart_0019bd78,src/ps2/gslib_hw_recovered.c\n0x0019bd88,0x0019bd94,ClearVRcount,ClearVRcount_0019bd88,src/ps2/gslib_hw_recovered.c\n0x0019bd98,0x0019be1c,DmaReset,DmaReset_0019bd98,src/ps2/gslib_hw_recovered.c\n0x0019be20,0x0019be40,SendDma02,SendDma02_0019be20,src/ps2/gslib_hw_recovered.c\n0x0019be40,0x0019be70,Dma02Wait,Dma02Wait_0019be40,src/ps2/gslib_hw_recovered.c\n', encoding="utf-8")
Path("tools/run-gslib-frontier.sh").write_text('#!/usr/bin/env bash\nset -euo pipefail\n\nROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"\ncd "$ROOT"\n\nDEFAULT_EE_CC="$ROOT/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"\nEE_CC="${EE_CC:-$DEFAULT_EE_CC}"\n\nif [[ ! -x "$EE_CC" ]]; then\n    echo "Missing historical EE compiler: $EE_CC" >&2\n    echo "Run: make bootstrap-ee-stage1" >&2\n    exit 2\nfi\n\nmkdir -p build/matching/gslib_hw\nLOG="build/matching/gslib_hw/frontier-run.log"\n\n# Critical: never reuse a stale object while iterating matching source.\nrm -f build/matching/gslib_hw/gslib_hw.o\nrm -f analysis/matching/gslib-hw-listing-report.md\n\nif ! make match-gslib-hw-listing EE_CC="$EE_CC" >"$LOG" 2>&1; then\n    echo "=== GSLIB frontier: build failed ==="\n    tail -n 80 "$LOG"\n    exit 1\nfi\n\necho "=== GSLIB frontier ==="\ngrep -E \'wrote analysis/matching/gslib-hw-listing-report|matching summary:|^[[:space:]]*(MATCH|MISS)[[:space:]]\' "$LOG" \\\n    | tail -n 20 || true\n\nif grep -q \'matching summary: 7/7\' "$LOG"; then\n    echo\n    echo "=== strict gate ==="\n    if make match-gslib-hw-listing-strict EE_CC="$EE_CC" >"$LOG.strict" 2>&1; then\n        grep -E \'wrote analysis/matching/gslib-hw-listing-report|7/7|Local GSLIB\' "$LOG.strict" | tail -n 10 || true\n        echo "GSLIB strict listing gate: OK"\n    else\n        tail -n 80 "$LOG.strict"\n        exit 1\n    fi\nfi\n\necho\necho "Full log: $LOG"\n', encoding="utf-8")
Path("tools/test_progress42_gslib_historical_runner.py").write_text('from pathlib import Path\nimport unittest\n\nROOT = Path(__file__).resolve().parents[1]\n\n\nclass Progress42GslibHistoricalRunnerTests(unittest.TestCase):\n    def test_historical_dma_inline_asm_is_present(self):\n        text = (ROOT / "src/ps2/gslib_hw_recovered.c").read_text(encoding="utf-8")\n        self.assertIn(\'__asm__("\\\\tsw  $0, 0x1000a080");\', text)\n        self.assertIn(\'__asm__("\\\\tli $3, 0x1000a000");\', text)\n        self.assertIn(\'__asm__("\\\\taddiu $29, -4");\', text)\n        self.assertNotIn("mmio32_store", text)\n        self.assertNotIn("mmio32_load", text)\n\n    def test_wait_preserves_historical_expression(self):\n        text = (ROOT / "src/ps2/gslib_hw_recovered.c").read_text(encoding="utf-8")\n        self.assertIn("while (VRcount_recovered < numvrs)", text)\n\n    def test_runner_forces_fresh_object(self):\n        text = (ROOT / "tools/run-gslib-frontier.sh").read_text(encoding="utf-8")\n        self.assertIn("rm -f build/matching/gslib_hw/gslib_hw.o", text)\n        self.assertIn("match-gslib-hw-listing-strict", text)\n\n\nif __name__ == "__main__":\n    unittest.main()\n', encoding="utf-8")
Path("docs/PROGRESS42.md").write_text('# Progress 42 — historical GSLIB source + stale-object-proof runner\n\nThe previous 4/7 measurement did not show an EE compile command and therefore\nreused an existing `gslib_hw.o`.  It was not a measurement of the newly\nrecovered historical `hw.c` source.\n\nThis progress:\n\n- restores the surviving historical GSLIB source idioms for the seven-function\n  corridor;\n- restores the measured manifest boundaries, excluding alignment padding;\n- adds `tools/run-gslib-frontier.sh`, which always deletes the candidate object\n  before compiling;\n- prints only the matching summary while retaining the full compiler/listing\n  output in `build/matching/gslib_hw/frontier-run.log`;\n- automatically runs the strict local listing gate when 7/7 is reached.\n\nHistorical source evidence:\n`ps2homebrew/gslib/source/hw.c`,\ncommit `d9e623a351627e53420f44b00d494346cee5d5a2`.\n', encoding="utf-8")

make = Path("Makefile")
text = make.read_text(encoding="utf-8")
old = "EE_CC ?= ee-gcc"
new = "EE_CC ?= $(if $(wildcard build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc),$(abspath build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc),ee-gcc)"
if old in text:
    text = text.replace(old, new, 1)
elif new not in text:
    raise SystemExit("Makefile: unexpected EE_CC assignment")
make.write_text(text, encoding="utf-8")
PY

chmod +x tools/run-gslib-frontier.sh

echo "Progress 42 applied."
echo "Run only:"
echo "  ./tools/run-gslib-frontier.sh"
