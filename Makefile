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
SOURCE_TREE_BUILD_DIR := $(BUILD_DIR)/source-tree
SOURCE_TREE_MANIFEST := analysis/source_tree/translation_units.tsv
SOURCE_TREE_DEFINED_MAP := analysis/source_tree/defined_symbol_ownership.tsv
SOURCE_TREE_EXTERNAL_MAP := analysis/source_tree/external_symbol_ownership.tsv
SOURCE_TREE_ABI_CONTRACT := analysis/source_tree/ee_abi_contract.c
SOURCE_TREE_SPECIAL_MAP := analysis/source_tree/special_ownership.tsv
SOURCE_TREE_FINGERPRINTS := analysis/source_tree/object_fingerprints.tsv
SOURCE_ALIAS_MANIFEST := analysis/link_identity/source_address_aliases.tsv
SOURCE_ALIAS_REVIEWS := analysis/link_identity/source_alias_reviews.tsv
SOURCE_ALIAS_BUILD_DIR := $(BUILD_DIR)/source-aliases
SOURCE_ALIAS_INPUT := $(SOURCE_TREE_BUILD_DIR)/source-tree.partial.o
SOURCE_ALIAS_OUTPUT := $(SOURCE_ALIAS_BUILD_DIR)/source-tree.alias-resolved.partial.o
SOURCE_ALIAS_REPORT := $(SOURCE_ALIAS_BUILD_DIR)/report.json
LINK_CONTRACT_MANIFEST := analysis/link_identity/link_contracts.tsv
LINK_CONTRACT_REVIEWS := analysis/link_identity/link_contract_reviews.tsv
LINK_CONTRACT_BUILD_DIR := $(BUILD_DIR)/link-contracts
LINK_CONTRACT_INPUT := $(SOURCE_ALIAS_OUTPUT)
LINK_CONTRACT_OUTPUT := $(LINK_CONTRACT_BUILD_DIR)/source-tree.link-contracts.partial.o
LINK_CONTRACT_REPORT := $(LINK_CONTRACT_BUILD_DIR)/report.json
PRIVATE_ASSET_MANIFEST := analysis/link_identity/private_asset_providers.tsv
PRIVATE_ASSET_BUILD_DIR := $(BUILD_DIR)/private-assets
PRIVATE_ASSET_INPUT := $(LINK_CONTRACT_OUTPUT)
PRIVATE_ASSET_OUTPUT := $(PRIVATE_ASSET_BUILD_DIR)/source-tree.private-assets.partial.o
PRIVATE_ASSET_REPORT := $(PRIVATE_ASSET_BUILD_DIR)/report.json
PROVIDER_FRONTIER_MANIFEST := analysis/link_identity/provider_frontier_closure.tsv
PROVIDER_FRONTIER_BUILD_DIR := $(BUILD_DIR)/provider-frontier
PROVIDER_FRONTIER_INPUT := $(PRIVATE_ASSET_OUTPUT)
PROVIDER_FRONTIER_OUTPUT := $(PROVIDER_FRONTIER_BUILD_DIR)/source-tree.provider-closed.partial.o
PROVIDER_FRONTIER_REPORT := $(PROVIDER_FRONTIER_BUILD_DIR)/report.json
NAMED_DATA_MANIFEST := analysis/link_identity/named_data.tsv
NAMED_DATA_REVIEWS := analysis/link_identity/named_data_reviews.tsv
NAMED_DATA_BUILD_DIR := $(BUILD_DIR)/named-data
NAMED_DATA_INPUT := $(PRIVATE_ASSET_OUTPUT)
NAMED_DATA_OUTPUT := $(NAMED_DATA_BUILD_DIR)/source-tree.named-data.partial.o
NAMED_DATA_REPORT := $(NAMED_DATA_BUILD_DIR)/report.json
REFERENCE_RAW := $(BUILD_DIR)/SNES_EMU.unpacked.bin
ASSET_OUTPUT ?= $(BUILD_DIR)/extracted-assets
UNPACKED_LAYOUT_MANIFEST := analysis/link_identity/unpacked_layout.json
LAYOUT_ORACLE_REPORT := $(BUILD_DIR)/layout-oracle/comparison.json
CANDIDATE_RAW ?= $(BUILD_DIR)/SNES_EMU.rebuilt.bin

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
EE_SOURCE_TREE_FLAGS := $(filter-out -Werror,$(EE_CFLAGS))
EE_SOURCE_TREE_FLAGS := $(subst -Wa$(comma)-al,,$(EE_SOURCE_TREE_FLAGS))
EE_SOURCE_TREE_FLAGS += -Iinclude/ee_stage1_compat -w
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

.PHONY: help help-legacy status docs frontier-map check-generated check-links \
	checkpoint-1041-audit checkpoint-1041-check checkpoint-1041-reference-check \
	reproduce-status reproduce-check reproduce \
	audit-source audit-source-check host-syntax test-tools check \
	reference verify-reference extract-assets fetch-newlib fetch-ee-toolchain-recipe \
	layout-oracle layout-oracle-check layout-oracle-refresh layout-oracle-public-check compare-unpacked \
	bootstrap-ee-stage1 bootstrap-ee-cxx-stage1 \
	source-tree source-tree-check source-tree-refresh \
	source-aliases source-aliases-check source-aliases-refresh source-aliases-public-check \
	link-contracts link-contracts-check link-contracts-refresh link-contracts-public-check \
	private-assets private-assets-check private-assets-refresh private-assets-public-check \
	provider-frontier provider-frontier-check provider-frontier-refresh provider-frontier-public-check \
	named-data named-data-check named-data-verify named-data-refresh named-data-public-check \
	hunt1000plus-v45-runtime hunt1000plus-v45-historical hunt1000plus-v45-evidence \
	hunt1000plus-v46-evidence hunt1000plus-v47-evidence hunt1041-v48-evidence hunt1041-v49-evidence hunt1041-v51-evidence hunt1041-v52-evidence hunt1041-v72-evidence hunt1041-v73-evidence hunt1041-v74-evidence hunt1041-v75-evidence hunt1041-v76-evidence hunt1041-v77-evidence hunt1041-v78-evidence hunt1041-v79-evidence hunt1041-v80-evidence hunt1041-v81-evidence \
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
	@echo "  make status          show formal, pending and working checkpoints"
	@echo "  make check           run manifests, generated docs, links, syntax and tests"
	@echo "  make checkpoint-1041-check  verify the frozen public 1041/1041 checkpoint"
	@echo "  make checkpoint-1041-reference-check  rerun its private-reference proof"
	@echo "  make docs            regenerate all maintained status files"
	@echo "  make reference       unpack and verify original/SNES_EMU.ELF privately"
	@echo "  make reproduce-check verify every implemented whole-program gate"
	@echo "  make reproduce       run the stable full pipeline (final link still blocked)"
	@echo "  make layout-oracle   verify the private unpacked layout against public hashes"
	@echo "  make compare-unpacked CANDIDATE_RAW=path  report the first rebuilt-byte difference"
	@echo "  make bootstrap-ee-stage1  build isolated binutils 2.14 + EE GCC 3.2.2"
	@echo "  make source-tree     build toolchain and verify the frozen Stage-2 object set"
	@echo "  make source-tree-check  verify Stage 2 with an available EE compiler"
	@echo "  make source-aliases  prove and apply zero-byte Stage-3 address aliases"
	@echo "  make link-contracts  apply the zero-byte Stage-3 link-contract frontier"
	@echo "  make private-assets  verify and link the five private embedded-asset bundles"
	@echo "  make provider-frontier  close the post-refactor 248-name source-link frontier"
	@echo "  make named-data      verify the closed 54/54 Stage-3C ledger and exact ranges"
	@echo "  make match-miner     run the cached three-profile strict match search"
	@echo "  make elf-status      show remaining exact-ELF blockers"
	@echo "  make help-legacy     list frozen historical evidence runners"
	@echo
	@echo "For matching, run make bootstrap-ee-stage1 or pass EE_CC=/path/to/ee-gcc."

help-legacy:
	@echo "Historical and focused evidence runners"
	@echo
	@echo "  make hunt1000plus-v45-evidence ... hunt1041-v52-evidence"
	@echo "  make hunt1041-v72-evidence  reproduce the six promoted V53 proofs"
	@echo "  make hunt1041-v73-evidence  reproduce the two PS2-I/O historical proofs"
	@echo "  make hunt1041-v74-evidence  reproduce the two SPC7110 RTC proofs"
	@echo "  make hunt1041-v75-evidence  reproduce five C4 proofs plus companion"
	@echo "  make hunt1041-v76-evidence  reproduce the C4SprDisintegrate proof"
	@echo "  make hunt1041-v77-evidence  reproduce the C4DrawWireFrame proof"
	@echo "  make hunt1041-v78-evidence  reproduce the C4BitPlaneWave proof"
	@echo "  make hunt1041-v79-evidence  reproduce the C4ConvOAM proof"
	@echo "  make hunt1041-v80-evidence  reproduce the 23 quick-win proofs"
	@echo "  make hunt1041-v81-evidence  reproduce the final 20 function proofs"
	@echo "  make match-miner-full"
	@echo "  make historical-ee-gate"
	@echo "  make match-get-tree-listing-strict"
	@echo "  make match-mathfp-listing-strict"
	@echo "  make match-libgcc-unwind-listing-strict"
	@echo "  make match-gslib-hw-listing-strict"
	@echo "  make match-libkernel-leaves-listing-strict"
	@echo "  make match-libkernel-size-strings-listing-strict"
	@echo "  make match-libkernel-libc-strings-listing-strict"
	@echo "  make match-cpp-runtime-small-listing-strict"
	@echo "  make match-cdvd-rpc-exact-listing-strict"

status:
	$(PYTHON) tools/project_status.py

docs: audit-source
	$(PYTHON) tools/update_frontier_map.py
	$(PYTHON) tools/update_progress.py

check-generated: audit-source-check
	$(PYTHON) tools/update_frontier_map.py --check
	$(PYTHON) tools/update_progress.py --check

frontier-map:
	$(PYTHON) tools/update_frontier_map.py

check-links:
	$(PYTHON) tools/check_links.py

reproduce-status:
	bash tools/reproduce.sh status

reproduce-check:
	bash tools/reproduce.sh verify

reproduce:
	bash tools/reproduce.sh full

audit-source:
	$(PYTHON) tools/audit_source_completeness.py

audit-source-check:
	$(PYTHON) tools/audit_source_completeness.py --check

host-syntax:
	@set -eu; \
	mkdir -p "$(BUILD_DIR)"; \
	log="$(BUILD_DIR)/host-syntax.log"; \
	: > "$$log"; \
	count=0; \
	for source in $(SOURCE_C) $(MATCHING_C); do \
		if ! $(HOST_CC) -std=c11 -Wall -Wextra -fno-builtin -DSNESSTATION_HOST_SYNTAX=1 -fsyntax-only -Iinclude -iquote matching/ee_abi_compat "$$source" >>"$$log" 2>&1; then \
			cat "$$log" >&2; \
			exit 1; \
		fi; \
		count=$$((count + 1)); \
	done; \
	echo "host syntax: OK ($$count independent C translation units; warnings in $$log)"

test-tools:
	$(PYTHON) -m unittest discover -s tools -p 'test_*.py'

checkpoint-1041-audit:
	$(PYTHON) tools/verify_checkpoint_1041.py

checkpoint-1041-check: check
	@echo "function-frontier-1041-v81 public checkpoint: OK"

checkpoint-1041-reference-check: checkpoint-1041-check
	$(MAKE) hunt1041-v81-evidence
	$(MAKE) elf-status
	@echo "function-frontier-1041-v81 private-reference checkpoint: OK"

check: check-generated check-links host-syntax test-tools checkpoint-1041-audit layout-oracle-public-check source-aliases-public-check link-contracts-public-check private-assets-public-check provider-frontier-public-check named-data-public-check
	@echo "repository checks: OK"

reference:
	bash tools/analyze.sh
	$(PYTHON) tools/verify_reference.py

verify-reference:
	$(PYTHON) tools/verify_reference.py

layout-oracle: reference
	$(MAKE) layout-oracle-check

layout-oracle-check:
	$(PYTHON) tools/layout_oracle.py check \
		--packed original/SNES_EMU.ELF \
		--unpacked "$(REFERENCE_RAW)" \
		--manifest "$(UNPACKED_LAYOUT_MANIFEST)"

# Refreshing the public hash oracle is deliberately separate from checking it.
# This target never writes the reference image itself into the repository.
layout-oracle-refresh: reference
	$(PYTHON) tools/layout_oracle.py capture \
		--packed original/SNES_EMU.ELF \
		--unpacked "$(REFERENCE_RAW)" \
		--manifest "$(UNPACKED_LAYOUT_MANIFEST)"

layout-oracle-public-check:
	$(PYTHON) tools/layout_oracle.py validate \
		--manifest "$(UNPACKED_LAYOUT_MANIFEST)"

compare-unpacked: layout-oracle
	@test -f "$(CANDIDATE_RAW)" || { \
		echo "Missing rebuilt candidate: $(CANDIDATE_RAW)" >&2; \
		exit 2; \
	}
	$(PYTHON) tools/layout_oracle.py compare \
		--reference "$(REFERENCE_RAW)" \
		--candidate "$(CANDIDATE_RAW)" \
		--manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--report "$(LAYOUT_ORACLE_REPORT)"

extract-assets: reference
	$(PYTHON) tools/extract_embedded_assets.py \
		--input "$(REFERENCE_RAW)" --output "$(ASSET_OUTPUT)"

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
	$(PYTHON) tools/history/research/hunt1000plus_v45_runtime.py \
		--libgcc "$(EE_STAGE1_WORK_DIR)/prefix/lib/gcc-lib/ee/3.2.2/libgcc.a" \
		--cxx "$(EE_STAGE1_CXX)" \
		--assembler-prefix "$(EE_STAGE1_WORK_DIR)/prefix/ee/bin" \
		--gcc-source "$(EE_STAGE1_WORK_DIR)/source/gcc-3.2.2"

hunt1000plus-v45-historical: reference bootstrap-ee-stage1
	$(PYTHON) tools/history/research/hunt1000plus_v45_historical.py \
		--compiler "$(EE_STAGE1_CC)"

hunt1000plus-v45-evidence: hunt1000plus-v45-runtime hunt1000plus-v45-historical
	@echo "HUNT1000+ V45 evidence: OK (50 runtime + 4 historical strict matches)"

hunt1000plus-v46-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1000plus_v46_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)" \
		--libgcc "$(EE_STAGE1_WORK_DIR)/build/gcc-ee-stage1/gcc/libgcc.a"
	@echo "HUNT1000+ V46 evidence: OK (42 strict matches)"

hunt1000plus-v47-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1000plus_v47_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1000+ V47 evidence: OK (79 strict matches)"

hunt1041-v48-evidence: bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v48_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V48 evidence: OK (25 strict matches)"

hunt1041-v49-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v49_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V49 evidence: OK (20 formal-ELF strict matches)"

hunt1041-v51-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v51_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V51 evidence: OK (16 formal-ELF exact matches)"

hunt1041-v52-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v52_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V52 evidence: OK (17 formal-ELF exact matches)"

hunt1041-v72-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v72_promote_v53.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V72 evidence: OK (6 promoted V53 formal-ELF exact matches)"

hunt1041-v73-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v73_historical_io.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V73 evidence: OK (2 formal-ELF PS2-I/O matches)"

hunt1041-v74-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v74_spc7110_rtc.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V74 evidence: OK (2 formal-ELF SPC7110 RTC matches)"

hunt1041-v75-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v75_c4.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V75 evidence: OK (5 formal C4 matches + 1 exact companion)"

hunt1041-v76-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v76_c4spr.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V76 evidence: OK (1 formal C4SprDisintegrate match)"

hunt1041-v77-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v77_c4draw.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V77 evidence: OK (1 formal C4DrawWireFrame match)"

hunt1041-v78-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v78_c4bit.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "HUNT1041 V78 evidence: OK (1 formal C4BitPlaneWave match)"

hunt1041-v79-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v79_c4conv.py \
		--assembler "$(EE_CXX_STAGE1_WORK_DIR)/prefix/bin/ee-as"
	@echo "HUNT1041 V79 evidence: OK (1 formal C4ConvOAM match)"

hunt1041-v80-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v80_quickwins.py \
		--assembler "$(EE_CXX_STAGE1_WORK_DIR)/prefix/bin/ee-as"
	@echo "HUNT1041 V80 evidence: OK (23 formal quick-win matches)"

hunt1041-v81-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v81_final20.py \
		--assembler "$(EE_CXX_STAGE1_WORK_DIR)/prefix/bin/ee-as"
	@echo "HUNT1041 V81 evidence: OK (20 final-frontier matches; 1041/1041 closed)"

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

source-tree: bootstrap-ee-stage1
	$(MAKE) source-tree-check EE_CC="$(EE_STAGE1_CC)"

source-tree-check: check-ee-compiler
	$(PYTHON) tools/build_source_tree.py \
		--compiler "$(EE_CC)" \
		--cflags '$(EE_SOURCE_TREE_FLAGS)' \
		--manifest "$(SOURCE_TREE_MANIFEST)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--abi-contract "$(SOURCE_TREE_ABI_CONTRACT)" \
		--special-map "$(SOURCE_TREE_SPECIAL_MAP)" \
		--fingerprints "$(SOURCE_TREE_FINGERPRINTS)" \
		--build-dir "$(SOURCE_TREE_BUILD_DIR)" \
		--jobs "$${EE_SOURCE_TREE_JOBS:-8}"

# Deliberately separate from the check target: refreshing ownership is a
# reviewed source-boundary decision, never an automatic side effect.
source-tree-refresh: check-ee-compiler
	$(PYTHON) tools/build_source_tree.py \
		--compiler "$(EE_CC)" \
		--cflags '$(EE_SOURCE_TREE_FLAGS)' \
		--manifest "$(SOURCE_TREE_MANIFEST)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--abi-contract "$(SOURCE_TREE_ABI_CONTRACT)" \
		--special-map "$(SOURCE_TREE_SPECIAL_MAP)" \
		--fingerprints "$(SOURCE_TREE_FINGERPRINTS)" \
		--build-dir "$(SOURCE_TREE_BUILD_DIR)" \
		--jobs "$${EE_SOURCE_TREE_JOBS:-8}" \
		--update

source-aliases: bootstrap-ee-stage1
	$(MAKE) source-aliases-check EE_CC="$(EE_STAGE1_CC)"

source-aliases-check: source-tree-check
	$(PYTHON) tools/source_aliases.py link \
		--compiler "$(EE_CC)" \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--progress-manifest "analysis/progress_targets.csv" \
		--manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--reviews "$(SOURCE_ALIAS_REVIEWS)" \
		--input "$(SOURCE_ALIAS_INPUT)" \
		--output "$(SOURCE_ALIAS_OUTPUT)" \
		--report "$(SOURCE_ALIAS_REPORT)"

# Refreshing aliases is a reviewed identity decision, kept separate from the
# normal repository and historical-compiler checks.
source-aliases-refresh:
	$(PYTHON) tools/source_aliases.py refresh \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--progress-manifest "analysis/progress_targets.csv" \
		--manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--reviews "$(SOURCE_ALIAS_REVIEWS)"

source-aliases-public-check:
	$(PYTHON) tools/source_aliases.py validate \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--progress-manifest "analysis/progress_targets.csv" \
		--manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--reviews "$(SOURCE_ALIAS_REVIEWS)"

link-contracts: bootstrap-ee-stage1
	$(MAKE) link-contracts-check EE_CC="$(EE_STAGE1_CC)"

link-contracts-check: source-aliases-check
	$(PYTHON) tools/link_contracts.py link \
		--compiler "$(EE_CC)" \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--progress-manifest "analysis/progress_targets.csv" \
		--source-alias-manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(LINK_CONTRACT_MANIFEST)" \
		--reviews "$(LINK_CONTRACT_REVIEWS)" \
		--input "$(LINK_CONTRACT_INPUT)" \
		--output "$(LINK_CONTRACT_OUTPUT)" \
		--report "$(LINK_CONTRACT_REPORT)"

# Refreshing the frontier is a reviewed link-identity decision and therefore
# stays separate from normal repository and historical-compiler checks.
link-contracts-refresh:
	$(PYTHON) tools/link_contracts.py refresh \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--progress-manifest "analysis/progress_targets.csv" \
		--source-alias-manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(LINK_CONTRACT_MANIFEST)" \
		--reviews "$(LINK_CONTRACT_REVIEWS)"

link-contracts-public-check:
	$(PYTHON) tools/link_contracts.py validate \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--progress-manifest "analysis/progress_targets.csv" \
		--source-alias-manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(LINK_CONTRACT_MANIFEST)" \
		--reviews "$(LINK_CONTRACT_REVIEWS)"

private-assets: reference bootstrap-ee-stage1
	$(MAKE) private-assets-check EE_CC="$(EE_STAGE1_CC)"

private-assets-check: link-contracts-check
	@test -f "$(REFERENCE_RAW)" || { \
		echo "Missing private unpacked reference: $(REFERENCE_RAW)" >&2; \
		echo "Run make reference first." >&2; \
		exit 2; \
	}
	$(PYTHON) tools/private_asset_providers.py link \
		--compiler "$(EE_CC)" \
		--reference "$(REFERENCE_RAW)" \
		--assets "analysis/embedded_assets.csv" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(PRIVATE_ASSET_MANIFEST)" \
		--input "$(PRIVATE_ASSET_INPUT)" \
		--build-dir "$(PRIVATE_ASSET_BUILD_DIR)" \
		--output "$(PRIVATE_ASSET_OUTPUT)" \
		--report "$(PRIVATE_ASSET_REPORT)"

# Refreshing this name/range map is a reviewed provider-identity decision.
private-assets-refresh:
	$(PYTHON) tools/private_asset_providers.py refresh \
		--assets "analysis/embedded_assets.csv" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(PRIVATE_ASSET_MANIFEST)"

private-assets-public-check:
	$(PYTHON) tools/private_asset_providers.py validate \
		--assets "analysis/embedded_assets.csv" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(PRIVATE_ASSET_MANIFEST)"

provider-frontier: private-assets bootstrap-ee-stage1
	$(MAKE) provider-frontier-check EE_CC="$(EE_STAGE1_CC)"

provider-frontier-check: private-assets-check
	$(PYTHON) tools/provider_frontier.py link \
		--compiler "$(EE_CC)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--private-manifest "$(PRIVATE_ASSET_MANIFEST)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--input "$(PROVIDER_FRONTIER_INPUT)" \
		--build-dir "$(PROVIDER_FRONTIER_BUILD_DIR)" \
		--output "$(PROVIDER_FRONTIER_OUTPUT)" \
		--report "$(PROVIDER_FRONTIER_REPORT)"

# The generated manifest is a reviewed closure decision, not a routine side effect.
provider-frontier-refresh:
	$(PYTHON) tools/provider_frontier.py refresh \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--private-manifest "$(PRIVATE_ASSET_MANIFEST)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--manifest "$(PROVIDER_FRONTIER_MANIFEST)"

provider-frontier-public-check:
	$(PYTHON) tools/provider_frontier.py validate \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--private-manifest "$(PRIVATE_ASSET_MANIFEST)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--manifest "$(PROVIDER_FRONTIER_MANIFEST)"

# Stage 3C: verify the closed original 54-row named-data tranche without
# publishing bytes from the private reference image.
named-data: reference bootstrap-ee-stage1
	$(MAKE) named-data-check EE_CC="$(EE_STAGE1_CC)"

named-data-check: private-assets-check
	@test -f "$(REFERENCE_RAW)" || { \
		echo "Missing private unpacked reference: $(REFERENCE_RAW)" >&2; \
		echo "Run make reference first." >&2; \
		exit 2; \
	}
	$(PYTHON) tools/named_data.py link \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--private-manifest "$(PRIVATE_ASSET_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--reviews "$(NAMED_DATA_REVIEWS)" \
		--manifest "$(NAMED_DATA_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--compiler "$(EE_CC)" \
		--input "$(NAMED_DATA_INPUT)" \
		--build-dir "$(NAMED_DATA_BUILD_DIR)" \
		--output "$(NAMED_DATA_OUTPUT)" \
		--report "$(NAMED_DATA_REPORT)"

named-data-verify: reference
	$(PYTHON) tools/named_data.py verify \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--private-manifest "$(PRIVATE_ASSET_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--reviews "$(NAMED_DATA_REVIEWS)" \
		--manifest "$(NAMED_DATA_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--report "$(NAMED_DATA_REPORT)"

named-data-refresh: reference
	$(PYTHON) tools/named_data.py refresh \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--private-manifest "$(PRIVATE_ASSET_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--reviews "$(NAMED_DATA_REVIEWS)" \
		--manifest "$(NAMED_DATA_MANIFEST)" \
		--reference "$(REFERENCE_RAW)"

named-data-public-check:
	$(PYTHON) tools/named_data.py validate \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--private-manifest "$(PRIVATE_ASSET_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--reviews "$(NAMED_DATA_REVIEWS)" \
		--manifest "$(NAMED_DATA_MANIFEST)"

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
	@echo "historical EE gate: OK (repository checks + 7/7 unwind + all tracked C TUs)"

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
	@echo "Function-code gate: CLOSED (1041/1041 strict matches)"
	@echo "Build-ready source ownership: CLOSED (97/97 TUs; 96 canonical objects)"
	@echo "Unpacked layout oracle: CLOSED (1 section; 13 blocks; 51 hash windows)"
	@echo "Zero-byte link contracts: 1336/1594 resolved (1273 anchors; 63 aliases)"
	@echo "Private assets: CLOSED (10 providers; 62736 verified private bytes)"
	@echo "Source-link provider namespace: CLOSED (248 -> 0 externals)"
	@echo "Original Stage 3C: CLOSED (50 exact target ranges + 4 removed source adapters)"
	@echo "Complete replacement ELF: BLOCKED (honest status)"
	@echo "  - replace compatibility storage/shims with exact target initializers/archive members"
	@echo "  - reproduce data layout, relocations and section alignment"
	@echo "  - prove exact EE archives, linker script, object order and library order"
	@echo "  - reproduce SJCRUNCH2 packing and both reference hashes"
	@echo "See docs/REPRODUCTION.md"

elf: elf-status
	@echo "Refusing to emit a pretend replacement ELF." >&2
	@echo "Close the recorded evidence gates before implementing this target." >&2
	@exit 2

clean-matching:
	rm -rf "$(MATCH_DIR)"
