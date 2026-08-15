#!/usr/bin/env bash
set -euo pipefail

if [[ ! -f Makefile || ! -f matching/candidates/libgcc_unwind_leaves.c ]]; then
    echo "Run this from the SNESstation-Decomp repository root." >&2
    exit 2
fi

python3 - <<'PY'
from pathlib import Path


def replace_once(path: str, old: str, new: str) -> None:
    p = Path(path)
    text = p.read_text(encoding="utf-8")
    count = text.count(old)
    if count != 1:
        raise SystemExit(f"{path}: expected exactly one matching block, found {count}")
    p.write_text(text.replace(old, new, 1), encoding="utf-8")

# --- Makefile: fix the scanner flags and make the committed-listing probe honest.
replace_once(
    "Makefile",
    "LIBGCC_UNWIND_MANIFEST := analysis/matching/libgcc_unwind_leaves.csv\n"
    "LIBGCC_UNWIND_REPORT := $(MATCH_DIR)/libgcc_unwind/report.md\n",
    "LIBGCC_UNWIND_MANIFEST := analysis/matching/libgcc_unwind_leaves.csv\n"
    "LIBGCC_UNWIND_LISTING_MANIFEST := analysis/matching/libgcc_unwind_listing.csv\n"
    "LIBGCC_UNWIND_REPORT := $(MATCH_DIR)/libgcc_unwind/report.md\n",
)

replace_once(
    "Makefile",
    "# The source scan is diagnostic: warnings must not hide the first real EE\n"
    "# compatibility failure, and assembler listings are irrelevant to -fsyntax-only.\n"
    "EE_SOURCE_SCAN_FLAGS := $(filter-out -Werror -Wa,-al,$(EE_CFLAGS))\n",
    "# The source scan is diagnostic: warnings must not hide the first real EE\n"
    "# compatibility failure, and assembler listings are irrelevant to -fsyntax-only.\n"
    "# Do not put -Wa,-al directly inside a make function: its comma is parsed as\n"
    "# a function-argument separator. Strip it with an explicit comma variable.\n"
    "comma := ,\n"
    "EE_SOURCE_SCAN_FLAGS := $(filter-out -Werror,$(EE_CFLAGS))\n"
    "EE_SOURCE_SCAN_FLAGS := $(subst -Wa$(comma)-al,,$(EE_SOURCE_SCAN_FLAGS))\n",
)

replace_once(
    "Makefile",
    '\t@echo "  make match-libgcc-unwind-listing  probe 12 GCC unwind helpers locally"\n',
    '\t@echo "  make match-libgcc-unwind-listing  probe 7 committed GCC unwind helpers locally"\n',
)

replace_once(
    "Makefile",
    "$(LIBGCC_FRONTIER_RAW): $(LIBGCC_FRONTIER_LISTING) tools/objdump_listing_to_binary.py\n"
    "\t$(PYTHON) tools/objdump_listing_to_binary.py \\\n"
    "\t\t--input \"$<\" \\\n"
    "\t\t--output \"$@\" \\\n"
    "\t\t--base-address 0x001a1b00 \\\n"
    "\t\t--end-address 0x001a5cc0\n",
    "$(LIBGCC_FRONTIER_RAW): $(LIBGCC_FRONTIER_LISTING) tools/objdump_listing_to_binary.py Makefile\n"
    "\t$(PYTHON) tools/objdump_listing_to_binary.py \\\n"
    "\t\t--input \"$<\" \\\n"
    "\t\t--output \"$@\" \\\n"
    "\t\t--base-address 0x001a1b00 \\\n"
    "\t\t--end-address 0x001a4100\n",
)

replace_once(
    "Makefile",
    'match-libgcc-unwind-listing: $(LIBGCC_FRONTIER_RAW) $(LIBGCC_UNWIND_OBJECT)\n'
    '\t$(PYTHON) tools/compare_elf_functions.py \\\n'
    '\t\t--target "$(LIBGCC_FRONTIER_RAW)" \\\n'
    '\t\t--base-address 0x001a1b00 \\\n'
    '\t\t--object "$(LIBGCC_UNWIND_OBJECT)" \\\n'
    '\t\t--manifest "$(LIBGCC_UNWIND_MANIFEST)" \\\n',
    'match-libgcc-unwind-listing: $(LIBGCC_FRONTIER_RAW) $(LIBGCC_UNWIND_OBJECT)\n'
    '\t$(PYTHON) tools/compare_elf_functions.py \\\n'
    '\t\t--target "$(LIBGCC_FRONTIER_RAW)" \\\n'
    '\t\t--base-address 0x001a1b00 \\\n'
    '\t\t--object "$(LIBGCC_UNWIND_OBJECT)" \\\n'
    '\t\t--manifest "$(LIBGCC_UNWIND_LISTING_MANIFEST)" \\\n',
)

replace_once(
    "Makefile",
    'match-libgcc-unwind-listing-strict: $(LIBGCC_FRONTIER_RAW) $(LIBGCC_UNWIND_OBJECT)\n'
    '\t$(PYTHON) tools/compare_elf_functions.py \\\n'
    '\t\t--target "$(LIBGCC_FRONTIER_RAW)" \\\n'
    '\t\t--base-address 0x001a1b00 \\\n'
    '\t\t--object "$(LIBGCC_UNWIND_OBJECT)" \\\n'
    '\t\t--manifest "$(LIBGCC_UNWIND_MANIFEST)" \\\n',
    'match-libgcc-unwind-listing-strict: $(LIBGCC_FRONTIER_RAW) $(LIBGCC_UNWIND_OBJECT)\n'
    '\t$(PYTHON) tools/compare_elf_functions.py \\\n'
    '\t\t--target "$(LIBGCC_FRONTIER_RAW)" \\\n'
    '\t\t--base-address 0x001a1b00 \\\n'
    '\t\t--object "$(LIBGCC_UNWIND_OBJECT)" \\\n'
    '\t\t--manifest "$(LIBGCC_UNWIND_LISTING_MANIFEST)" \\\n',
)

# --- Candidate: mirror the GCC 3.x unwind word modes and source shape.
replace_once(
    "matching/candidates/libgcc_unwind_leaves.c",
    "typedef signed long long p29_s64;\n"
    "typedef unsigned long long p29_u64;\n",
    "typedef signed long long p29_s64;\n"
    "typedef unsigned long long p29_u64;\n"
    "typedef unsigned p29_unwind_word __attribute__((__mode__(__word__)));\n"
    "typedef signed p29_unwind_sword __attribute__((__mode__(__word__)));\n",
)

replace_once(
    "matching/candidates/libgcc_unwind_leaves.c",
    "    switch (encoding & 7u) {\n"
    "    case P29_DW_EH_PE_ABSPTR:\n"
    "        return 4;\n"
    "    case P29_DW_EH_PE_UDATA2:\n"
    "        return 2;\n"
    "    case P29_DW_EH_PE_UDATA4:\n"
    "        return 4;\n"
    "    case P29_DW_EH_PE_UDATA8:\n"
    "        return 8;\n"
    "    default:\n"
    "        abort();\n"
    "    }\n",
    "    switch (encoding & 7u) {\n"
    "    case P29_DW_EH_PE_ABSPTR:\n"
    "        return sizeof(void *);\n"
    "    case P29_DW_EH_PE_UDATA2:\n"
    "        return 2;\n"
    "    case P29_DW_EH_PE_UDATA4:\n"
    "        return 4;\n"
    "    case P29_DW_EH_PE_UDATA8:\n"
    "        return 8;\n"
    "    }\n"
    "\n"
    "    abort();\n",
)

replace_once(
    "matching/candidates/libgcc_unwind_leaves.c",
    "    case P29_DW_EH_PE_FUNCREL:\n"
    "        return unwind_get_region_start_candidate(context);\n"
    "    default:\n"
    "        abort();\n"
    "    }\n",
    "    case P29_DW_EH_PE_FUNCREL:\n"
    "        return unwind_get_region_start_candidate(context);\n"
    "    }\n"
    "\n"
    "    abort();\n",
)

replace_once(
    "matching/candidates/libgcc_unwind_leaves.c",
    "const p29_u8 *unwind_read_uleb128_candidate(const p29_u8 *p, p29_u64 *value)\n"
    "{\n"
    "    unsigned int shift;\n"
    "    p29_u64 result;\n"
    "    p29_u8 byte;\n"
    "\n"
    "    shift = 0;\n"
    "    result = 0;\n"
    "    do {\n"
    "        byte = *p++;\n"
    "        result |= (p29_u64)(byte & 0x7fu) << shift;\n"
    "        shift += 7;\n"
    "    } while ((byte & 0x80u) != 0);\n"
    "\n"
    "    *value = result;\n"
    "    return p;\n"
    "}\n",
    "const p29_u8 *unwind_read_uleb128_candidate(const p29_u8 *p, p29_unwind_word *value)\n"
    "{\n"
    "    unsigned int shift;\n"
    "    p29_u8 byte;\n"
    "    p29_unwind_word result;\n"
    "\n"
    "    shift = 0;\n"
    "    result = 0;\n"
    "    do {\n"
    "        byte = *p++;\n"
    "        result |= ((p29_unwind_word)byte & 0x7fu) << shift;\n"
    "        shift += 7;\n"
    "    } while (byte & 0x80u);\n"
    "\n"
    "    *value = result;\n"
    "    return p;\n"
    "}\n",
)

replace_once(
    "matching/candidates/libgcc_unwind_leaves.c",
    "const p29_u8 *unwind_read_sleb128_candidate(const p29_u8 *p, p29_s64 *value)\n"
    "{\n"
    "    unsigned int shift;\n"
    "    p29_u64 result;\n"
    "    p29_u8 byte;\n"
    "\n"
    "    shift = 0;\n"
    "    result = 0;\n"
    "    do {\n"
    "        byte = *p++;\n"
    "        result |= (p29_u64)(byte & 0x7fu) << shift;\n"
    "        shift += 7;\n"
    "    } while ((byte & 0x80u) != 0);\n"
    "\n"
    "    if (shift < 64u && (byte & 0x40u) != 0)\n"
    "        result |= (~(p29_u64)0) << shift;\n"
    "\n"
    "    *value = (p29_s64)result;\n"
    "    return p;\n"
    "}\n",
    "const p29_u8 *unwind_read_sleb128_candidate(const p29_u8 *p, p29_unwind_sword *value)\n"
    "{\n"
    "    unsigned int shift;\n"
    "    p29_u8 byte;\n"
    "    p29_unwind_word result;\n"
    "\n"
    "    shift = 0;\n"
    "    result = 0;\n"
    "    do {\n"
    "        byte = *p++;\n"
    "        result |= ((p29_unwind_word)byte & 0x7fu) << shift;\n"
    "        shift += 7;\n"
    "    } while (byte & 0x80u);\n"
    "\n"
    "    if (shift < 8 * sizeof(result) && (byte & 0x40u) != 0)\n"
    "        result |= -(((p29_unwind_word)1L) << shift);\n"
    "\n"
    "    *value = (p29_unwind_sword)result;\n"
    "    return p;\n"
    "}\n",
)

# --- Local listing manifest: only rows actually present in the committed listing.
Path("analysis/matching/libgcc_unwind_listing.csv").write_text(
    "address,end,name,object_symbol,source\n"
    "0x001a3dc0,0x001a3e30,size_of_encoded_value,unwind_size_of_encoded_value_candidate,matching/candidates/libgcc_unwind_leaves.c\n"
    "0x001a3e30,0x001a3ee8,base_of_encoded_value,unwind_base_of_encoded_value_candidate,matching/candidates/libgcc_unwind_leaves.c\n"
    "0x001a3ee8,0x001a3f28,read_uleb128,unwind_read_uleb128_candidate,matching/candidates/libgcc_unwind_leaves.c\n"
    "0x001a3f28,0x001a3f88,read_sleb128,unwind_read_sleb128_candidate,matching/candidates/libgcc_unwind_leaves.c\n"
    "0x001a40e8,0x001a40f0,_Unwind_GetLanguageSpecificData,unwind_get_lsda_candidate,matching/candidates/libgcc_unwind_leaves.c\n"
    "0x001a40f0,0x001a40f8,_Unwind_GetRegionStart,unwind_get_region_start_candidate,matching/candidates/libgcc_unwind_leaves.c\n"
    "0x001a40f8,0x001a4100,_Unwind_GetDataRelBase,unwind_get_data_rel_base_candidate,matching/candidates/libgcc_unwind_leaves.c\n",
    encoding="utf-8",
)

# --- Regression tests for the two measurement bugs.
insert = '''    def test_listing_manifest_only_uses_committed_listing_bytes(self):
        full_path = ROOT / "analysis" / "matching" / "libgcc_unwind_leaves.csv"
        listing_path = ROOT / "analysis" / "matching" / "libgcc_unwind_listing.csv"
        with full_path.open(encoding="utf-8") as stream:
            full_rows = list(csv.DictReader(stream))
        with listing_path.open(encoding="utf-8") as stream:
            listing_rows = list(csv.DictReader(stream))

        self.assertEqual(len(full_rows), 12)
        self.assertEqual(len(listing_rows), 7)
        full_keys = {(row["address"], row["object_symbol"]) for row in full_rows}
        for row in listing_rows:
            self.assertIn((row["address"], row["object_symbol"]), full_keys)
            self.assertLessEqual(int(row["end"], 0), 0x001A4100)

    def test_source_scan_flags_do_not_split_on_wa_comma(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertNotIn("filter-out -Werror -Wa,-al", makefile)
        self.assertIn("subst -Wa$(comma)-al", makefile)

'''
replace_once(
    "tools/test_progress29_matching.py",
    "    def test_scan_classifier(self):\n",
    insert + "    def test_scan_classifier(self):\n",
)

Path("docs/PROGRESS30.md").write_text(
    """# Progress 30 — repair the EE scan and make the libgcc probe honest

Progress 29 exposed two measurement problems before it exposed 101 independent
source failures.

## 1. EE source-scan flag repair

GNU make treats commas as function-argument separators. The previous
`filter-out` expression embedded `-Wa,-al` directly, so the generated scanner
argument began with a corrupted `-al,-G0` token. Progress 30 removes `-Werror`
first and strips `-Wa,-al` with an explicit comma variable.

The next `make ee-source-scan` result is therefore the first usable historical
GCC 3.2.2 translation-unit baseline.

## 2. Listing coverage is now bounded to real bytes

`analysis/functions/libgcc_frontier_001a1b00.asm` currently stops at
`0x001a40fc`. The listing-to-binary helper zero-fills unwritten addresses, so
extending that synthetic image to `0x001a5cc0` made five unavailable functions
look like byte mismatches.

The local probe now ends at `0x001a4100` and uses a seven-row manifest. The
12-row manifest remains unchanged for the formal original-ELF comparison.

## 3. GCC unwind source-shape recovery

The four compact DWARF helpers now mirror the historical GCC unwind source shape
more closely: word-mode `_Unwind_Word`/`_Unwind_Sword` equivalents, `abort()`
after switch fallthrough, `sizeof(void *)` for absolute pointers, and the old
LEB128 sign-extension expression.

Run:

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make check
make ee-source-scan EE_CC="$EE_CC"
make match-libgcc-unwind-listing EE_CC="$EE_CC"
make match-libgcc-unwind EE_CC="$EE_CC"
```

Do not promote any new match until the formal original-ELF report agrees.
""",
    encoding="utf-8",
)

print("Progress 30 applied:")
for path in (
    "Makefile",
    "matching/candidates/libgcc_unwind_leaves.c",
    "analysis/matching/libgcc_unwind_listing.csv",
    "tools/test_progress29_matching.py",
    "docs/PROGRESS30.md",
):
    print(f"  {path}")
PY

printf '\nNow run:\n'
printf '  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"\n'
printf '  make check\n'
printf '  make ee-source-scan EE_CC="$EE_CC"\n'
printf '  make match-libgcc-unwind-listing EE_CC="$EE_CC"\n'
printf '  make match-libgcc-unwind EE_CC="$EE_CC"\n'
