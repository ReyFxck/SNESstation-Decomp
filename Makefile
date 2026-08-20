# Reproducible analysis and matching entry points for SNES Station v0.23 WIP.
#
# The historical SNESticle build is evidence for the candidate flags below; it
# is not proof that SNES Station used the same source list or final link order.

PYTHON ?= python3
HOST_CC ?= cc
EE_CC ?= $(if $(wildcard build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc),$(abspath build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc),ee-gcc)
EE_GCC_VERSION ?= 3.2.2-b1

BUILD_DIR := build
EE_STAGE1_WORK_DIR ?= $(abspath $(BUILD_DIR)/toolchains/ee-gcc-3.2.2-stage1)
EE_STAGE1_CC := $(EE_STAGE1_WORK_DIR)/prefix/bin/ee-gcc
EE_CXX_STAGE1_WORK_DIR ?= $(abspath $(BUILD_DIR)/toolchains/ee-gcc-3.2.2-cxx-stage1)
EE_STAGE1_CXX := $(EE_CXX_STAGE1_WORK_DIR)/prefix/bin/ee-g++
EE_BOOTSTRAP_JOBS_ARG := $(if $(strip $(EE_BUILD_JOBS)),--jobs "$(EE_BUILD_JOBS)",)
MATCH_DIR := $(BUILD_DIR)/matching
MATHFP_SOURCE := matching/candidates/mathfp.c
MATHFP_NUMTEST_SOURCE := matching/candidates/mathfp_numtest.S
MATHFP_CORE_OBJECT := $(MATCH_DIR)/mathfp/mathfp_core.o
MATHFP_NUMTEST_OBJECT := $(MATCH_DIR)/mathfp/mathfp_numtest.o
MATHFP_OBJECT := $(MATCH_DIR)/mathfp/mathfp.o
MATHFP_REPORT := $(MATCH_DIR)/mathfp/report.md
MATHFP_MANIFEST := analysis/matching/mathfp.csv
MATHFP_LISTING := analysis/functions/math_frontier_0019fddc.asm
MATHFP_LISTING_RAW := $(MATCH_DIR)/mathfp/listing.bin
MATHFP_LISTING_REPORT := analysis/matching/mathfp-listing-report.md
GET_TREE_SOURCE := matching/candidates/get_tree.S
GET_TREE_OBJECT := $(MATCH_DIR)/get_tree/get_tree.o
GET_TREE_REPORT := $(MATCH_DIR)/get_tree/report.md
GET_TREE_MANIFEST := analysis/matching/get_tree.csv
GET_TREE_LISTING := analysis/functions/unzip_explode_0018c124.asm
GET_TREE_LISTING_RAW := $(MATCH_DIR)/get_tree/listing.bin
GET_TREE_LISTING_REPORT := analysis/matching/get-tree-listing-report.md
LIBGCC_UNWIND_SOURCE := matching/candidates/libgcc_unwind_leaves.c
LIBGCC_UNWIND_OBJECT := $(MATCH_DIR)/libgcc_unwind/libgcc_unwind_leaves.o
LIBGCC_UNWIND_MANIFEST := analysis/matching/libgcc_unwind_leaves.csv
LIBGCC_UNWIND_LISTING_MANIFEST := analysis/matching/libgcc_unwind_listing.csv
LIBGCC_UNWIND_REPORT := $(MATCH_DIR)/libgcc_unwind/report.md
LIBGCC_FRONTIER_LISTING := analysis/functions/libgcc_frontier_001a1b00.asm
LIBGCC_FRONTIER_RAW := $(MATCH_DIR)/libgcc_unwind/listing.bin
LIBGCC_UNWIND_LISTING_REPORT := analysis/matching/libgcc-unwind-leaves-listing-report.md
GSLIB_HW_SOURCE := src/ps2/gslib_hw_recovered.c
GSLIB_HW_OBJECT := $(MATCH_DIR)/gslib_hw/gslib_hw.o
GSLIB_HW_EE_CFLAGS = $(EE_CFLAGS) -Imatching/ee_abi_compat
GSLIB_HW_MANIFEST := analysis/matching/gslib_hw_listing.csv
GSLIB_HW_LISTING := analysis/functions/gslib_hw_0019bd38.asm
GSLIB_HW_LISTING_RAW := $(MATCH_DIR)/gslib_hw/listing.bin
GSLIB_HW_LISTING_REPORT := analysis/matching/gslib-hw-listing-report.md
EE_SOURCE_SCAN_DIR := $(BUILD_DIR)/ee-source-scan
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
# The source scan is diagnostic: warnings must not hide the first real EE
# compatibility failure, and assembler listings are irrelevant to -fsyntax-only.
# Do not put -Wa,-al directly inside a make function: its comma is parsed as
# a function-argument separator. Strip it with an explicit comma variable.
comma := ,
EE_SOURCE_SCAN_FLAGS := $(filter-out -Werror,$(EE_CFLAGS))
EE_SOURCE_SCAN_FLAGS := $(subst -Wa$(comma)-al,,$(EE_SOURCE_SCAN_FLAGS))
EE_SOURCE_SCAN_FLAGS += -Iinclude/ee_stage1_compat -w
# The target mathfp corridor uses the normal 64-bit double ABI and does not
# carry GCC's optional jump-target padding.  Keeping both differences local
# reproduces sinf/tanf without disturbing the application-object contract.
MATHFP_EE_CFLAGS := $(filter-out -fshort-double,$(EE_CFLAGS)) -fno-align-jumps

# Historical SNESticle reference only. SNES Station's linker script, archive
# revisions and exact library order are still evidence gates, so `make elf`
# deliberately refuses to invent them.
SNESTICLE_REFERENCE_LDFLAGS := -nostartfiles -T../linkfile -Wl,-Map,SNESticle.map
SNESTICLE_REFERENCE_LIBS := -lmc -lpad -lps2ip -lkernel -lc -lm -lgcc -lstdc++

.DEFAULT_GOAL := help

.PHONY: help audit-source audit-source-check host-syntax test-tools check \
	reference verify-reference fetch-newlib fetch-ee-toolchain-recipe \
	bootstrap-ee-stage1 bootstrap-ee-cxx-stage1 \
	hunt1000plus-v45-runtime hunt1000plus-v45-historical hunt1000plus-v45-evidence \
	hunt1000plus-v46-evidence \
	toolchain-info toolchain-probe check-ee-compiler \
	match-miner match-miner-full \
	ee-source-scan ee-source-scan-strict historical-ee-gate \
	match-get-tree match-get-tree-strict match-get-tree-listing match-get-tree-listing-strict \
	match-mathfp match-mathfp-strict \
	match-mathfp-listing match-mathfp-listing-strict \
	match-libgcc-unwind match-libgcc-unwind-strict \
	match-libgcc-unwind-listing match-libgcc-unwind-listing-strict \
	match-gslib-hw-listing match-gslib-hw-listing-strict \
	match-libkernel-leaves-listing-strict match-libkernel-size-strings-listing-strict match-libkernel-libc-strings-listing-strict \
	match-cpp-runtime-small-listing-strict match-cdvd-rpc-exact-listing-strict \
	elf-status elf clean-matching

help:
	@echo "SNES Station v0.23 preservation workflow"
	@echo
	@echo "  make check          audit manifests, parse all C units, test comparator"
	@echo "  make reference      unpack and verify your original/SNES_EMU.ELF"
	@echo "  make fetch-newlib   fetch verified Newlib 1.10.0 mathfp source"
	@echo "  make fetch-ee-toolchain-recipe  fetch the pinned 2004 PS2DEV recipe"
	@echo "  make bootstrap-ee-stage1  build isolated binutils 2.14 + EE GCC 3.2.2"
	@echo "  make bootstrap-ee-cxx-stage1  build the isolated C/C++ runtime-matching front ends"
	@echo "  make hunt1000plus-v45-evidence  reproduce the 50 runtime + 4 historical strict matches"
	@echo "  make hunt1000plus-v46-evidence  reproduce the next 42 strict source/archive matches"
	@echo "  make toolchain-info show the candidate historical EE compiler contract"
	@echo "  make toolchain-probe test EE_CC version, target, flags and ELF output"
	@echo "  make ee-source-scan  baseline every C TU against the historical EE front end"
	@echo "  make match-miner     cached three-profile strict scan for new address-labelled matches"
	@echo "  make match-miner-full  cached 16-profile scan (use only after source/toolchain changes)"
	@echo "  make historical-ee-gate  strict 101/101 EE scan + strict 7/7 unwind listing gate"
	@echo "  make match-get-tree  formal reference-ELF gate for byte-exact get_tree"
	@echo "  make match-get-tree-listing  strict local get_tree gate against committed bytes"
	@echo "  make match-mathfp   compile and compare the seven-function math corridor"
	@echo "  make match-mathfp-listing  compare math against the committed disassembly"
	@echo "  make match-libgcc-unwind-listing  probe 7 committed GCC unwind helpers locally"
	@echo "  make match-gslib-hw-listing  probe 7 recovered GSLIB hw helpers locally"
	@echo "  make match-libkernel-leaves-listing-strict  strict 21/21 old EE libkernel leaf gate"
	@echo "  make match-libkernel-size-strings-listing-strict  strict 4/4 old EE size-string gate"
	@echo "  make match-libkernel-libc-strings-listing-strict  strict 7/7 old EE libc assembly gate"
	@echo "  make match-cpp-runtime-small-listing-strict  strict 48/48 GCC/libsupc++ runtime gate"
	@echo "  make match-cdvd-rpc-exact-listing-strict  strict 2/2 remaining CDVD exact gate"
	@echo "  make elf-status     show why a complete replacement ELF is not ready"
	@echo
	@echo "For matching, run make bootstrap-ee-stage1 or pass EE_CC=/path/to/ee-gcc."

audit-source:
	$(PYTHON) tools/audit_source_completeness.py

audit-source-check:
	$(PYTHON) tools/audit_source_completeness.py --check

host-syntax:
	@set -eu; \
	count=0; \
	for source in $(SOURCE_C) $(MATCHING_C); do \
		$(HOST_CC) -std=c11 -Wall -Wextra -fno-builtin -DSNESSTATION_HOST_SYNTAX=1 -fsyntax-only -Iinclude -iquote matching/ee_abi_compat "$$source"; \
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

fetch-ee-toolchain-recipe:
	$(PYTHON) tools/fetch_ee_toolchain_recipe.py

bootstrap-ee-stage1:
	$(PYTHON) tools/bootstrap_ee_gcc_stage1.py \
		--work-dir "$(EE_STAGE1_WORK_DIR)" $(EE_BOOTSTRAP_JOBS_ARG)

bootstrap-ee-cxx-stage1:
	$(PYTHON) tools/bootstrap_ee_gcc_stage1.py \
		--work-dir "$(EE_CXX_STAGE1_WORK_DIR)" \
		--languages c,c++ $(EE_BOOTSTRAP_JOBS_ARG)

hunt1000plus-v45-runtime: reference fetch-newlib bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/research/hunt1000plus_v45_runtime.py \
		--libgcc "$(EE_STAGE1_WORK_DIR)/prefix/lib/gcc-lib/ee/3.2.2/libgcc.a" \
		--cxx "$(EE_STAGE1_CXX)" \
		--assembler-prefix "$(EE_STAGE1_WORK_DIR)/prefix/ee/bin" \
		--gcc-source "$(EE_STAGE1_WORK_DIR)/source/gcc-3.2.2"

hunt1000plus-v45-historical: reference bootstrap-ee-stage1
	$(PYTHON) tools/research/hunt1000plus_v45_historical.py \
		--compiler "$(EE_STAGE1_CC)"

hunt1000plus-v45-evidence: hunt1000plus-v45-runtime hunt1000plus-v45-historical
	@echo "HUNT1000+ V45 evidence: OK (50 runtime + 4 historical strict matches)"

hunt1000plus-v46-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/research/hunt1000plus_v46_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)" \
		--libgcc "$(EE_STAGE1_WORK_DIR)/build/gcc-ee-stage1/gcc/libgcc.a"
	@echo "HUNT1000+ V46 evidence: OK (42 strict matches)"

toolchain-info:
	@echo "Candidate EE compiler: GCC $(EE_GCC_VERSION)"
	@echo "EE_CC=$(EE_CC)"
	@echo "Bootstrapped stage-one path: $(EE_STAGE1_CC)"
	@echo "EE_CFLAGS=$(EE_CFLAGS)"
	@echo "MATHFP_EE_CFLAGS=$(MATHFP_EE_CFLAGS)"
	@echo "SNESticle-only linker reference: $(SNESTICLE_REFERENCE_LDFLAGS)"
	@echo "SNESticle-only library reference: $(SNESTICLE_REFERENCE_LIBS)"

toolchain-probe:
	$(PYTHON) tools/probe_ee_toolchain.py --compiler "$(EE_CC)"

check-ee-compiler:
	@command -v "$(EE_CC)" >/dev/null 2>&1 || { \
		echo "Missing EE compiler: $(EE_CC)" >&2; \
		echo "Build the isolated stage-one candidate with:" >&2; \
		echo "  make bootstrap-ee-stage1" >&2; \
		echo "Then select it explicitly with:" >&2; \
		echo "  EE_CC=$(EE_STAGE1_CC)" >&2; \
		echo "  make match-get-tree EE_CC=/absolute/path/to/ee-gcc" >&2; \
		echo "  make match-mathfp EE_CC=/absolute/path/to/ee-gcc" >&2; \
		exit 2; \
	}

match-miner: reference check-ee-compiler
	$(PYTHON) tools/run_match_miner.py \
		--compiler "$(EE_CC)" \
		--jobs "$${MATCH_MINER_JOBS:-8}"

match-miner-full: reference check-ee-compiler
	$(PYTHON) tools/run_match_miner.py \
		--compiler "$(EE_CC)" \
		--jobs "$${MATCH_MINER_JOBS:-8}" \
		--full


ee-source-scan: check-ee-compiler
	$(PYTHON) tools/scan_ee_translation_units.py \
		--compiler "$(EE_CC)" \
		--flags '$(EE_SOURCE_SCAN_FLAGS)' \
		--output-dir "$(EE_SOURCE_SCAN_DIR)" \
		--jobs "$${EE_SCAN_JOBS:-2}" \
		src matching/candidates

ee-source-scan-strict: check-ee-compiler
	$(PYTHON) tools/scan_ee_translation_units.py \
		--compiler "$(EE_CC)" \
		--flags '$(EE_SOURCE_SCAN_FLAGS)' \
		--output-dir "$(EE_SOURCE_SCAN_DIR)" \
		--jobs "$${EE_SCAN_JOBS:-2}" \
		--strict \
		src matching/candidates

# Local historical EE regression gate. The original ELF remains the formal byte gate.
historical-ee-gate: check match-libgcc-unwind-listing-strict ee-source-scan-strict
	@echo "historical EE gate: OK (repository checks + 7/7 unwind + 101/101 C TUs)"

match-cdvd-rpc-exact-listing-strict: check-ee-compiler
	EE_CC="$(EE_CC)" bash tools/run-cdvd-rpc-exact-match.sh


$(MATHFP_CORE_OBJECT): $(MATHFP_SOURCE) $(MATHFP_MANIFEST) | check-ee-compiler
	@mkdir -p "$(dir $@)"
	$(EE_CC) $(MATHFP_EE_CFLAGS) -c $< -o $@

$(MATHFP_NUMTEST_OBJECT): $(MATHFP_NUMTEST_SOURCE) $(MATHFP_MANIFEST) | check-ee-compiler
	@mkdir -p "$(dir $@)"
	$(EE_CC) $(MATHFP_EE_CFLAGS) -c $< -o $@

$(MATHFP_OBJECT): $(MATHFP_CORE_OBJECT) $(MATHFP_NUMTEST_OBJECT)
	$(EE_CC) -nostdlib -Wl,-r -o $@ $^

$(MATHFP_LISTING_RAW): $(MATHFP_LISTING) tools/objdump_listing_to_binary.py
	$(PYTHON) tools/objdump_listing_to_binary.py \
		--input "$<" \
		--output "$@" \
		--base-address 0x0019fddc \
		--end-address 0x001a0740

$(LIBGCC_UNWIND_OBJECT): $(LIBGCC_UNWIND_SOURCE) $(LIBGCC_UNWIND_MANIFEST) | check-ee-compiler
	@mkdir -p "$(dir $@)"
	$(EE_CC) $(EE_CFLAGS) -c $< -o $@

$(LIBGCC_FRONTIER_RAW): $(LIBGCC_FRONTIER_LISTING) tools/objdump_listing_to_binary.py Makefile
	$(PYTHON) tools/objdump_listing_to_binary.py \
		--input "$<" \
		--output "$@" \
		--base-address 0x001a1b00 \
		--end-address 0x001a4100

$(GSLIB_HW_OBJECT): $(GSLIB_HW_SOURCE) $(GSLIB_HW_MANIFEST) matching/ee_abi_compat/stdint.h | check-ee-compiler
	@mkdir -p "$(dir $@)"
	$(EE_CC) $(GSLIB_HW_EE_CFLAGS) -c $< -o $@

$(GSLIB_HW_LISTING_RAW): $(GSLIB_HW_LISTING) tools/objdump_listing_to_binary.py Makefile
	$(PYTHON) tools/objdump_listing_to_binary.py \
		--input "$<" \
		--output "$@" \
		--base-address 0x0019bd38 \
		--end-address 0x0019be70

$(GET_TREE_LISTING_RAW): $(GET_TREE_LISTING) tools/objdump_listing_to_binary.py Makefile
	@mkdir -p "$(dir $@)"
	$(PYTHON) tools/objdump_listing_to_binary.py \
		--input "$<" \
		--output "$@" \
		--base-address 0x0018c124 \
		--end-address 0x0018c1f8

$(GET_TREE_OBJECT): $(GET_TREE_SOURCE) $(GET_TREE_MANIFEST) | check-ee-compiler
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


# Local strict gate against exact bytes already committed in the target listing.
# The reference-ELF targets above remain the formal original-binary gate.
match-get-tree-listing: $(GET_TREE_LISTING_RAW) $(GET_TREE_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(GET_TREE_LISTING_RAW)" \
		--base-address 0x0018c124 \
		--object "$(GET_TREE_OBJECT)" \
		--manifest "$(GET_TREE_MANIFEST)" \
		--report "$(GET_TREE_LISTING_REPORT)"
	$(PYTHON) tools/summarize_matching_report.py "$(GET_TREE_LISTING_REPORT)"
	@echo "Local get_tree listing probe complete; original ELF remains the formal gate."

match-get-tree-listing-strict: $(GET_TREE_LISTING_RAW) $(GET_TREE_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(GET_TREE_LISTING_RAW)" \
		--base-address 0x0018c124 \
		--object "$(GET_TREE_OBJECT)" \
		--manifest "$(GET_TREE_MANIFEST)" \
		--report "$(GET_TREE_LISTING_REPORT)" \
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

# This is a convenient local diagnostic against exact bytes already committed
# in the analysis listing.  The ELF-backed targets above remain the formal gate.
match-mathfp-listing: $(MATHFP_LISTING_RAW) $(MATHFP_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(MATHFP_LISTING_RAW)" \
		--base-address 0x0019fddc \
		--object "$(MATHFP_OBJECT)" \
		--manifest "$(MATHFP_MANIFEST)" \
		--report "$(MATHFP_LISTING_REPORT)"
	@echo "Inspect $(MATHFP_LISTING_REPORT); the original ELF is still the formal gate."

match-mathfp-listing-strict: $(MATHFP_LISTING_RAW) $(MATHFP_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(MATHFP_LISTING_RAW)" \
		--base-address 0x0019fddc \
		--object "$(MATHFP_OBJECT)" \
		--manifest "$(MATHFP_MANIFEST)" \
		--report "$(MATHFP_LISTING_REPORT)" \
		--require-all-matching

match-libgcc-unwind: verify-reference $(LIBGCC_UNWIND_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(REFERENCE_RAW)" \
		--base-address 0x00100000 \
		--object "$(LIBGCC_UNWIND_OBJECT)" \
		--manifest "$(LIBGCC_UNWIND_MANIFEST)" \
		--report "$(LIBGCC_UNWIND_REPORT)"
	$(PYTHON) tools/summarize_matching_report.py "$(LIBGCC_UNWIND_REPORT)"
	@echo "Formal reference-ELF comparison complete; no progress status was changed automatically."

match-libgcc-unwind-strict: verify-reference $(LIBGCC_UNWIND_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(REFERENCE_RAW)" \
		--base-address 0x00100000 \
		--object "$(LIBGCC_UNWIND_OBJECT)" \
		--manifest "$(LIBGCC_UNWIND_MANIFEST)" \
		--report "$(LIBGCC_UNWIND_REPORT)" \
		--require-all-matching

match-libgcc-unwind-listing: $(LIBGCC_FRONTIER_RAW) $(LIBGCC_UNWIND_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(LIBGCC_FRONTIER_RAW)" \
		--base-address 0x001a1b00 \
		--object "$(LIBGCC_UNWIND_OBJECT)" \
		--manifest "$(LIBGCC_UNWIND_LISTING_MANIFEST)" \
		--report "$(LIBGCC_UNWIND_LISTING_REPORT)"
	$(PYTHON) tools/summarize_matching_report.py "$(LIBGCC_UNWIND_LISTING_REPORT)"
	@echo "Local listing probe complete; original ELF remains the formal gate."

match-libgcc-unwind-listing-strict: $(LIBGCC_FRONTIER_RAW) $(LIBGCC_UNWIND_OBJECT)
	$(PYTHON) tools/compare_elf_functions.py \
		--target "$(LIBGCC_FRONTIER_RAW)" \
		--base-address 0x001a1b00 \
		--object "$(LIBGCC_UNWIND_OBJECT)" \
		--manifest "$(LIBGCC_UNWIND_LISTING_MANIFEST)" \
		--report "$(LIBGCC_UNWIND_LISTING_REPORT)" \
		--require-all-matching

match-gslib-hw-listing: $(GSLIB_HW_LISTING_RAW) $(GSLIB_HW_OBJECT)
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

match-libkernel-leaves-listing-strict:
	bash tools/run-libkernel-leaves-match.sh

match-libkernel-size-strings-listing-strict:
	bash tools/run-libkernel-size-strings-match.sh

match-libkernel-libc-strings-listing-strict:
	bash tools/run-libkernel-libc-strings-match.sh


# Progress 53: strict GCC/libsupc++ small-runtime gate (11 EH + 37 RTTI).
match-cpp-runtime-small-listing-strict:
	bash tools/run-cpp-runtime-small-match.sh

elf-status: audit-source-check
	@echo "Complete replacement ELF: BLOCKED (honest status)"
	@echo "  - source-model coverage is closed, but EE build-ready types/ownership are not yet frozen"
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
