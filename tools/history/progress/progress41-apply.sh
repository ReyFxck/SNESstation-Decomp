#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS40.md || ! -f src/ps2/gslib_hw_recovered.c ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 40." >&2
  exit 2
fi

python3 - <<'PY'

from pathlib import Path

def replace_once(path, old, new):
    p = Path(path)
    text = p.read_text(encoding="utf-8")
    n = text.count(old)
    if n != 1:
        raise SystemExit(f"{path}: expected exactly one occurrence, found {n}")
    p.write_text(text.replace(old, new), encoding="utf-8")

replace_once(
    "src/ps2/gslib_hw_recovered.c",
    '''void WaitForNextVRstart_0019bd50(int numvrs)
{
    VRcount_recovered = 0;
    if (numvrs > 0) {
        for (;;) {
            /* target 0x19bd60/0x19bd64: NOP + unconditional-in-practice loop */
        }
    }
}
''',
    '''void WaitForNextVRstart_0019bd50(unsigned int numvrs)
{
    VRcount_recovered = 0;
    while (VRcount_recovered < numvrs) {
        /* Historical source intent; non-volatile VRcount is folded by GCC. */
    }
}
''',
)

replace_once(
    "src/ps2/gslib_hw_recovered.c",
    '''static inline void mmio32_store(uintptr_t address, uint32_t value)
{
    *(volatile uint32_t *)address = value;
}

static inline uint32_t mmio32_load(uintptr_t address)
{
    return *(volatile uint32_t *)address;
}
''',
    '''#define GSLIB_HW32(address) \\
    (*(volatile uint32_t *)(uintptr_t)(address))

static inline uint32_t mmio32_load(uintptr_t address)
{
    return *(volatile uint32_t *)address;
}
''',
)

replace_once(
    "src/ps2/gslib_hw_recovered.c",
    '''void DmaReset_0019bd98(void)
{
    mmio32_store(0x1000a080u, 0);
    mmio32_store(0x1000a000u, 0);
    mmio32_store(0x1000a030u, 0);
    mmio32_store(0x1000a010u, 0);
    mmio32_store(0x1000a050u, 0);
    mmio32_store(0x1000a040u, 0);

    mmio32_store(0x1000e010u, 0xff1fu);
    mmio32_store(0x1000e000u, 0);
    mmio32_store(0x1000e020u, 0);
    mmio32_store(0x1000e030u, 0);
    mmio32_store(0x1000e050u, 0);
    mmio32_store(0x1000e040u, 0);
    mmio32_store(0x1000e000u, mmio32_load(0x1000e000u) | 1u);
}
''',
    '''void DmaReset_0019bd98(void)
{
    GSLIB_HW32(0x1000a080u) = 0;
    GSLIB_HW32(0x1000a000u) = 0;
    GSLIB_HW32(0x1000a030u) = 0;
    GSLIB_HW32(0x1000a010u) = 0;
    GSLIB_HW32(0x1000a050u) = 0;
    GSLIB_HW32(0x1000a040u) = 0;

    GSLIB_HW32(0x1000e010u) = 0xff1fu;
    GSLIB_HW32(0x1000e000u) = 0;
    GSLIB_HW32(0x1000e020u) = 0;
    GSLIB_HW32(0x1000e030u) = 0;
    GSLIB_HW32(0x1000e050u) = 0;
    GSLIB_HW32(0x1000e040u) = 0;
    GSLIB_HW32(0x1000e000u) = GSLIB_HW32(0x1000e000u) | 1u;
}
''',
)

replace_once(
    "src/ps2/gslib_hw_recovered.c",
    '''void SendDma02_0019be20(const void *dma_tag)
{
    const uintptr_t ch2 = 0x1000a000u;
    mmio32_store(ch2 + 0x30u, (uint32_t)(uintptr_t)dma_tag);
    mmio32_store(ch2 + 0x20u, 0);
    mmio32_store(ch2 + 0x00u, mmio32_load(ch2) | 0x105u);
}
''',
    '''void SendDma02_0019be20(const void *dma_tag)
{
    volatile uint32_t *ch2 =
        (volatile uint32_t *)(uintptr_t)0x1000a000u;

    ch2[0x30u / 4u] = (uint32_t)(uintptr_t)dma_tag;
    ch2[0x20u / 4u] = 0;
    ch2[0] = ch2[0] | 0x105u;
}
''',
)

m = Path("analysis/matching/gslib_hw_listing.csv")
text = m.read_text(encoding="utf-8")
old_wait = "0x0019bd50,0x0019bd78,WaitForNextVRstart,"
new_wait = "0x0019bd50,0x0019bd74,WaitForNextVRstart,"
old_reset = "0x0019bd98,0x0019be20,DmaReset,"
new_reset = "0x0019bd98,0x0019be1c,DmaReset,"
if text.count(old_wait) != 1 or text.count(old_reset) != 1:
    raise SystemExit("gslib manifest: expected Progress 38 boundaries not found")
text = text.replace(old_wait, new_wait, 1).replace(old_reset, new_reset, 1)
m.write_text(text, encoding="utf-8")

Path("tools/test_progress41_gslib_source_shape.py").write_text('from __future__ import annotations\n\nimport csv\nimport unittest\nfrom pathlib import Path\n\nROOT = Path(__file__).resolve().parents[1]\n\n\nclass Progress41GSLIBSourceShapeTests(unittest.TestCase):\n    def test_wait_uses_unsigned_counter_comparison(self):\n        text = (ROOT / "src/ps2/gslib_hw_recovered.c").read_text(encoding="utf-8")\n        self.assertIn(\n            "void WaitForNextVRstart_0019bd50(unsigned int numvrs)",\n            text,\n        )\n        self.assertIn("while (VRcount_recovered < numvrs)", text)\n\n    def test_dma_reset_uses_direct_mmio_lvalues(self):\n        text = (ROOT / "src/ps2/gslib_hw_recovered.c").read_text(encoding="utf-8")\n        self.assertIn("GSLIB_HW32(0x1000a080u) = 0;", text)\n        self.assertIn(\n            "GSLIB_HW32(0x1000e000u) = GSLIB_HW32(0x1000e000u) | 1u;",\n            text,\n        )\n\n    def test_manifest_excludes_alignment_padding(self):\n        with (ROOT / "analysis/matching/gslib_hw_listing.csv").open(\n            newline="", encoding="utf-8"\n        ) as f:\n            rows = {row["name"]: row for row in csv.DictReader(f)}\n        self.assertEqual("0x0019bd74", rows["WaitForNextVRstart"]["end"])\n        self.assertEqual("0x0019be1c", rows["DmaReset"]["end"])\n\n\nif __name__ == "__main__":\n    unittest.main()\n', encoding="utf-8")
Path("docs/PROGRESS41.md").write_text('# Progress 41 — shape the first GSLIB hardware misses\n\nThe corrected Progress-40 probe produced **3/7** local listing matches:\n\n- MATCH `VRstart_handler`\n- MATCH `TestVRstart`\n- MATCH `ClearVRcount`\n- MISS `WaitForNextVRstart`\n- MISS `DmaReset`\n- MISS `SendDma02`\n- MISS `Dma02Wait`\n\nThe first three matches prove the historical compiler/flags and recovered\n`VRcount` model are viable for this corridor.\n\nProgress 41 makes evidence-driven source-shape corrections:\n\n- `WaitForNextVRstart` uses an unsigned count and the original counter\n  comparison, allowing GCC 3.2.2 to produce the target `sltu`-based invariant\n  loop after folding the non-volatile global.\n- `DmaReset` uses direct volatile MMIO lvalues instead of inline store helpers,\n  matching the target\'s absolute `lui $1` / `sw` sequence.\n- `SendDma02` keeps one channel-2 base pointer, matching the target\'s single\n  base-register sequence.\n- Manifest ends for `WaitForNextVRstart` and `DmaReset` are corrected to\n  exclude alignment NOPs (`0x0019bd74` and `0x0019be1c` respectively).\n\n`Dma02Wait` is intentionally left alone for this iteration; its saved `$8`\nshape may represent historical inline assembly or a more specific source idiom\nand should be isolated after measuring these safer corrections.\n\n## Run\n\n```bash\nEE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"\n\nmake check\nrm -f build/matching/gslib_hw/gslib_hw.o\nmake match-gslib-hw-listing EE_CC="$EE_CC"\n```\n', encoding="utf-8")
print("Progress 41 applied.")

PY

echo
echo 'Run:'
echo '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"'
echo '  make check'
echo '  rm -f build/matching/gslib_hw/gslib_hw.o'
echo '  make match-gslib-hw-listing EE_CC="$EE_CC"'
