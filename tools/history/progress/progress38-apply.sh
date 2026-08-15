#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f docs/PROGRESS37.md || ! -f src/ps2/gslib_hw_recovered.c ]]; then
  echo "Run this from the SNESstation-Decomp root after Progress 37." >&2
  exit 2
fi

python3 - <<'PY'
from pathlib import Path

p = Path('Makefile')
text = p.read_text(encoding='utf-8')

def replace_once(old, new, label):
    global text
    count = text.count(old)
    if count != 1:
        raise SystemExit(f'Makefile: {label}: expected one insertion point, found {count}')
    text = text.replace(old, new, 1)

# Variables: keep this corridor independent from mathfp/libgcc.
anchor = 'EE_SOURCE_SCAN_DIR := $(BUILD_DIR)/ee-source-scan\n'
vars_block = (
    'GSLIB_HW_SOURCE := src/ps2/gslib_hw_recovered.c\n'
    'GSLIB_HW_OBJECT := $(MATCH_DIR)/gslib_hw/gslib_hw.o\n'
    'GSLIB_HW_MANIFEST := analysis/matching/gslib_hw_listing.csv\n'
    'GSLIB_HW_LISTING := analysis/functions/gslib_hw_0019bd38.asm\n'
    'GSLIB_HW_LISTING_RAW := $(MATCH_DIR)/gslib_hw/listing.bin\n'
    'GSLIB_HW_LISTING_REPORT := analysis/matching/gslib-hw-listing-report.md\n'
)
replace_once(anchor, vars_block + anchor, 'variable block')

# Phony targets.
old = '\tmatch-libgcc-unwind-listing match-libgcc-unwind-listing-strict \\\n\telf-status elf clean-matching\n'
new = '\tmatch-libgcc-unwind-listing match-libgcc-unwind-listing-strict \\\n\tmatch-gslib-hw-listing match-gslib-hw-listing-strict \\\n\telf-status elf clean-matching\n'
replace_once(old, new, 'phony targets')

# Help entry.
old = '\t@echo "  make match-libgcc-unwind-listing  probe 7 committed GCC unwind helpers locally"\n'
new = old + '\t@echo "  make match-gslib-hw-listing  probe 7 recovered GSLIB hw helpers locally"\n'
replace_once(old, new, 'help entry')

# Build rules inserted before GET_TREE object rule.
anchor = '$(GET_TREE_OBJECT): matching/candidates/get_tree.c $(GET_TREE_MANIFEST) | check-ee-compiler\n'
rules = r'''$(GSLIB_HW_OBJECT): $(GSLIB_HW_SOURCE) $(GSLIB_HW_MANIFEST) | check-ee-compiler
	@mkdir -p "$(dir $@)"
	$(EE_CC) $(EE_CFLAGS) -c $< -o $@

$(GSLIB_HW_LISTING_RAW): $(GSLIB_HW_LISTING) tools/objdump_listing_to_binary.py Makefile
	$(PYTHON) tools/objdump_listing_to_binary.py \
		--input "$<" \
		--output "$@" \
		--base-address 0x0019bd38 \
		--end-address 0x0019be70

'''
replace_once(anchor, rules + anchor, 'build rules')

# Match targets inserted before elf-status.
anchor = 'elf-status: audit-source-check\n'
targets = r'''match-gslib-hw-listing: $(GSLIB_HW_LISTING_RAW) $(GSLIB_HW_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(GSLIB_HW_LISTING_RAW)" \
		--base-address 0x0019bd38 \
		--object "$(GSLIB_HW_OBJECT)" \
		--manifest "$(GSLIB_HW_MANIFEST)" \
		--report "$(GSLIB_HW_LISTING_REPORT)"
	$(PYTHON) tools/summarize_matching_report.py "$(GSLIB_HW_LISTING_REPORT)"
	@echo "Local GSLIB hw listing probe complete; original ELF remains the formal gate."

match-gslib-hw-listing-strict: $(GSLIB_HW_LISTING_RAW) $(GSLIB_HW_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(GSLIB_HW_LISTING_RAW)" \
		--base-address 0x0019bd38 \
		--object "$(GSLIB_HW_OBJECT)" \
		--manifest "$(GSLIB_HW_MANIFEST)" \
		--report "$(GSLIB_HW_LISTING_REPORT)" \
		--require-all-matching

'''
replace_once(anchor, targets + anchor, 'match targets')

p.write_text(text, encoding='utf-8')
PY

cat > analysis/matching/gslib_hw_listing.csv <<'EOF2'
address,end,name,object_symbol,source
0x0019bd38,0x0019bd4c,VRstart_handler,VRstart_handler_0019bd38,src/ps2/gslib_hw_recovered.c
0x0019bd50,0x0019bd78,WaitForNextVRstart,WaitForNextVRstart_0019bd50,src/ps2/gslib_hw_recovered.c
0x0019bd78,0x0019bd84,TestVRstart,TestVRstart_0019bd78,src/ps2/gslib_hw_recovered.c
0x0019bd88,0x0019bd94,ClearVRcount,ClearVRcount_0019bd88,src/ps2/gslib_hw_recovered.c
0x0019bd98,0x0019be20,DmaReset,DmaReset_0019bd98,src/ps2/gslib_hw_recovered.c
0x0019be20,0x0019be40,SendDma02,SendDma02_0019be20,src/ps2/gslib_hw_recovered.c
0x0019be40,0x0019be70,Dma02Wait,Dma02Wait_0019be40,src/ps2/gslib_hw_recovered.c
EOF2

cat > tools/test_progress38_gslib_hw_probe.py <<'EOF2'
from __future__ import annotations

import csv
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress38GSLIBHWProbeTests(unittest.TestCase):
    def test_manifest_covers_seven_functions(self):
        path = ROOT / "analysis/matching/gslib_hw_listing.csv"
        with path.open(newline="", encoding="utf-8") as f:
            rows = list(csv.DictReader(f))
        self.assertEqual(7, len(rows))
        self.assertEqual("0x0019bd38", rows[0]["address"])
        self.assertEqual("0x0019be70", rows[-1]["end"])

    def test_makefile_has_local_probe(self):
        text = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("match-gslib-hw-listing:", text)
        self.assertIn("--base-address 0x0019bd38", text)
        self.assertIn("--end-address 0x0019be70", text)

    def test_probe_compiles_recovered_source_directly(self):
        text = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("GSLIB_HW_SOURCE := src/ps2/gslib_hw_recovered.c", text)


if __name__ == "__main__":
    unittest.main()
EOF2

cat > docs/PROGRESS38.md <<'EOF2'
# Progress 38 — first non-runtime local matching expansion: GSLIB hw

The historical EE front-end gate is closed at 101/101 translation units and the
committed libgcc-unwind local gate remains 7/7.  The next matching probe moves
outside Newlib/libgcc into SNES Station's recovered early Hiryu GSLIB hardware
tail.

The committed target listing spans `0x0019bd38..0x0019be70` and contains seven
small helpers:

- `VRstart_handler`
- `WaitForNextVRstart`
- `TestVRstart`
- `ClearVRcount`
- `DmaReset`
- `SendDma02`
- `Dma02Wait`

The probe deliberately compiles `src/ps2/gslib_hw_recovered.c` directly rather
than duplicating it under `matching/candidates/`.  This keeps the historical
101-TU front-end checkpoint stable and tests whether the recovered source itself
reproduces the committed machine-code corridor.

Run the non-strict probe first:

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make check
make historical-ee-gate EE_CC="$EE_CC"
make match-gslib-hw-listing EE_CC="$EE_CC"
```

Do not promote this corridor to a matching claim until the report is measured.
The original user-supplied ELF remains the formal gate.
EOF2

echo "Progress 38 applied."
echo
echo 'Run:'
echo '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"'
echo '  make check'
echo '  make historical-ee-gate EE_CC="$EE_CC"'
echo '  make match-gslib-hw-listing EE_CC="$EE_CC"'
