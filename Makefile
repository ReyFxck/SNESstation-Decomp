# Reproducible analysis and matching entry points for SNES Station v0.23 WIP.
#
# The historical SNESticle build is evidence for the candidate flags below; it
# is not proof that SNES Station used the same source list or final link order.

PYTHON ?= python3
HOST_CC ?= cc
EE_CC ?= ee-gcc
EE_GCC_VERSION ?= 3.2.2-b1

BUILD_DIR := build
MATCH_DIR := $(BUILD_DIR)/matching
MATHFP_OBJECT := $(MATCH_DIR)/mathfp/newlib_mathfp_recovered.o
MATHFP_REPORT := $(MATCH_DIR)/mathfp/report.md
MATHFP_MANIFEST := analysis/matching/mathfp.csv
GET_TREE_OBJECT := $(MATCH_DIR)/get_tree/get_tree.o
GET_TREE_REPORT := $(MATCH_DIR)/get_tree/report.md
GET_TREE_MANIFEST := analysis/matching/get_tree.csv
REFERENCE_RAW := $(BUILD_DIR)/SNES_EMU.unpacked.bin

SOURCE_C := $(shell find src -type f -name '*.c' | LC_ALL=C sort)
MATCHING_C := $(shell find matching/candidates -type f -name '*.c' | LC_ALL=C sort)

EE_COMMON_FLAGS := \
	-G0 -O2 -EL -pipe -Wall -Werror -Wa,-al \
	-fomit-frame-pointer -fstrict-aliasing -fno-common \
	-ffreestanding -fno-builtin -fshort-double \
	-mlong64 -mhard-float -mno-abicalls \
	-march=r5900 -mtune=r5900
EE_DEFINES := -DPS2_EE -D_EE -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3
EE_CFLAGS ?= $(EE_COMMON_FLAGS) $(EE_DEFINES) -Iinclude
EE_CXXFLAGS ?= $(EE_CFLAGS) -fno-exceptions -fno-common -fno-rtti

# Historical SNESticle reference only. SNES Station's linker script, archive
# revisions and exact library order are still evidence gates, so `make elf`
# deliberately refuses to invent them.
SNESTICLE_REFERENCE_LDFLAGS := -nostartfiles -T../linkfile -Wl,-Map,SNESticle.map
SNESTICLE_REFERENCE_LIBS := -lmc -lpad -lps2ip -lkernel -lc -lm -lgcc -lstdc++

.DEFAULT_GOAL := help

.PHONY: help audit-source audit-source-check host-syntax test-tools check \
	reference verify-reference fetch-newlib toolchain-info check-ee-compiler \
	match-get-tree match-get-tree-strict match-mathfp match-mathfp-strict \
	elf-status elf clean-matching

help:
	@echo "SNES Station v0.23 preservation workflow"
	@echo
	@echo "  make check          audit manifests, parse all C units, test comparator"
	@echo "  make reference      unpack and verify your original/SNES_EMU.ELF"
	@echo "  make fetch-newlib   fetch verified Newlib 1.10.0 mathfp source"
	@echo "  make toolchain-info show the candidate historical EE compiler contract"
	@echo "  make match-get-tree run the smallest compiler-fingerprint experiment"
	@echo "  make match-mathfp   compile and compare the seven-function math corridor"
	@echo "  make elf-status     show why a complete replacement ELF is not ready"
	@echo
	@echo "For matching, install the historical compiler or pass EE_CC=/path/to/ee-gcc."

audit-source:
	$(PYTHON) tools/audit_source_completeness.py

audit-source-check:
	$(PYTHON) tools/audit_source_completeness.py --check

host-syntax:
	@set -eu; \
	count=0; \
	for source in $(SOURCE_C) $(MATCHING_C); do \
		$(HOST_CC) -std=c11 -Wall -Wextra -fsyntax-only -Iinclude "$$source"; \
		count=$$((count + 1)); \
	done; \
	echo "host syntax: OK ($$count independent C translation units)"

test-tools:
	$(PYTHON) -m unittest discover -s tools -p 'test_*.py'

check: audit-source-check host-syntax test-tools
	@echo "repository checks: OK"

reference:
	bash tools/analyze.sh
	$(PYTHON) tools/verify_reference.py

verify-reference:
	$(PYTHON) tools/verify_reference.py

fetch-newlib:
	$(PYTHON) tools/fetch_upstream.py

toolchain-info:
	@echo "Candidate EE compiler: GCC $(EE_GCC_VERSION)"
	@echo "EE_CC=$(EE_CC)"
	@echo "EE_CFLAGS=$(EE_CFLAGS)"
	@echo "SNESticle-only linker reference: $(SNESTICLE_REFERENCE_LDFLAGS)"
	@echo "SNESticle-only library reference: $(SNESTICLE_REFERENCE_LIBS)"

check-ee-compiler:
	@command -v "$(EE_CC)" >/dev/null 2>&1 || { \
		echo "Missing EE compiler: $(EE_CC)" >&2; \
		echo "Install the historical ee-gcc 3.2.2-b1 candidate or run:" >&2; \
		echo "  make match-get-tree EE_CC=/absolute/path/to/ee-gcc" >&2; \
		echo "  make match-mathfp EE_CC=/absolute/path/to/ee-gcc" >&2; \
		exit 2; \
	}

$(MATHFP_OBJECT): src/ps2/newlib_mathfp_recovered.c $(MATHFP_MANIFEST) | check-ee-compiler
	@mkdir -p "$(dir $@)"
	$(EE_CC) $(EE_CFLAGS) -c $< -o $@

$(GET_TREE_OBJECT): matching/candidates/get_tree.c $(GET_TREE_MANIFEST) | check-ee-compiler
	@mkdir -p "$(dir $@)"
	$(EE_CC) $(EE_CFLAGS) -c $< -o $@

match-get-tree: verify-reference $(GET_TREE_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(REFERENCE_RAW)" \
		--base-address 0x00100000 \
		--object "$(GET_TREE_OBJECT)" \
		--manifest "$(GET_TREE_MANIFEST)" \
		--report "$(GET_TREE_REPORT)"
	@echo "Inspect $(GET_TREE_REPORT); no manifest status was changed automatically."

match-get-tree-strict: verify-reference $(GET_TREE_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(REFERENCE_RAW)" \
		--base-address 0x00100000 \
		--object "$(GET_TREE_OBJECT)" \
		--manifest "$(GET_TREE_MANIFEST)" \
		--report "$(GET_TREE_REPORT)" \
		--require-all-matching

match-mathfp: verify-reference $(MATHFP_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(REFERENCE_RAW)" \
		--base-address 0x00100000 \
		--object "$(MATHFP_OBJECT)" \
		--manifest "$(MATHFP_MANIFEST)" \
		--report "$(MATHFP_REPORT)"
	@echo "Inspect $(MATHFP_REPORT); no manifest status was changed automatically."

match-mathfp-strict: verify-reference $(MATHFP_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(REFERENCE_RAW)" \
		--base-address 0x00100000 \
		--object "$(MATHFP_OBJECT)" \
		--manifest "$(MATHFP_MANIFEST)" \
		--report "$(MATHFP_REPORT)" \
		--require-all-matching

elf-status: audit-source-check
	@echo "Complete replacement ELF: BLOCKED (honest status)"
	@echo "  - 239 validated entries remain structural pseudocode, not build-ready source"
	@echo "  - global ownership/types and translation-unit boundaries are not frozen"
	@echo "  - exact EE archives, linker script, object order and library order are unproven"
	@echo "  - SJCRUNCH2 repacking is not reproduced"
	@echo "See docs/MATCHING_WORKFLOW.md"

elf: elf-status
	@echo "Refusing to emit a pretend replacement ELF." >&2
	@echo "Close the recorded evidence gates before implementing this target." >&2
	@exit 2

clean-matching:
	rm -rf "$(MATCH_DIR)"
