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
NAMED_CONTRACT_MANIFEST := analysis/link_identity/named_contracts.tsv
NAMED_CONTRACT_BUILD_DIR := $(BUILD_DIR)/named-contracts
NAMED_CONTRACT_INPUT := $(PRIVATE_ASSET_OUTPUT)
NAMED_CONTRACT_OUTPUT := $(NAMED_CONTRACT_BUILD_DIR)/source-tree.named-contracts.partial.o
NAMED_CONTRACT_REPORT := $(NAMED_CONTRACT_BUILD_DIR)/report.json
LIBGCC_CONTRACT_MANIFEST := analysis/link_identity/libgcc_contracts.tsv
LIBGCC_CONTRACT_BUILD_DIR := $(BUILD_DIR)/libgcc-contracts
LIBGCC_CONTRACT_REPORT := $(LIBGCC_CONTRACT_BUILD_DIR)/report.json
RUNTIME_REFACTOR_MANIFEST := analysis/link_identity/runtime_refactors.tsv
RUNTIME_REFACTOR_REPORT := $(BUILD_DIR)/runtime-refactors/report.json
RUNTIME_MEMBER_MANIFEST := analysis/link_identity/runtime_members.tsv
RUNTIME_MEMBER_OBJECTS := analysis/link_identity/runtime_member_objects.tsv
RUNTIME_MEMBER_INPUTS := analysis/link_identity/runtime_member_inputs.tsv
RUNTIME_MEMBER_BUILD_DIR := $(BUILD_DIR)/runtime-members
RUNTIME_MEMBER_REPORT := $(RUNTIME_MEMBER_BUILD_DIR)/report.json
RUNTIME_MEMBER_CACHE_ARG := $(if $(strip $(RUNTIME_MEMBER_SOURCE_CACHE)),--source-cache "$(RUNTIME_MEMBER_SOURCE_CACHE)",)
DATA_BACKING_MANIFEST := analysis/link_identity/data_backing.tsv
DATA_BACKING_SECTIONS := analysis/link_identity/data_backing_sections.tsv
DATA_BACKING_BUILD_DIR := $(BUILD_DIR)/data-backing
DATA_BACKING_OUTPUT := $(DATA_BACKING_BUILD_DIR)/source-tree.data-backed.partial.o
DATA_BACKING_REPORT := $(DATA_BACKING_BUILD_DIR)/report.json
LINK_LAYOUT_PROBE_MANIFEST := analysis/link_identity/link_layout_probe.json
LINK_LAYOUT_PROBE_BUILD_DIR := $(BUILD_DIR)/link-layout-probe
LINK_LAYOUT_PROBE_REPORT := $(LINK_LAYOUT_PROBE_BUILD_DIR)/report.json
STARTUP_INTEGRATION_MANIFEST := analysis/link_identity/startup_integration.json
STARTUP_INTEGRATION_BUILD_DIR := $(BUILD_DIR)/startup-integration
STARTUP_INTEGRATION_SOURCE_CACHE_ARG := $(if $(strip $(STARTUP_INTEGRATION_SOURCE_CACHE)),--source-cache "$(STARTUP_INTEGRATION_SOURCE_CACHE)",)
FRONTEND_EH_MANIFEST := analysis/link_identity/frontend_eh_frames.json
FRONTEND_EH_BUILD_DIR := $(BUILD_DIR)/frontend-eh-frames
FRONTEND_EH_SOURCE := matching/candidates/stage3h_frontend_eh_frames.S
HISTORICAL_TAIL_MANIFEST := analysis/link_identity/historical_tail_data.json
HISTORICAL_TAIL_BUILD_DIR := $(BUILD_DIR)/historical-tail-data
RUNTIME_TAIL_MANIFEST := analysis/link_identity/runtime_tail_data.json
RUNTIME_TAIL_BUILD_DIR := $(BUILD_DIR)/runtime-tail-data
TAIL_METADATA_MANIFEST := analysis/link_identity/tail_metadata.json
TAIL_METADATA_BUILD_DIR := $(BUILD_DIR)/tail-metadata
WINDOW36_DATA_MANIFEST := analysis/link_identity/window36_data.json
WINDOW36_DATA_BUILD_DIR := $(BUILD_DIR)/window36-data
MEDIA_ASSET_MANIFEST := analysis/link_identity/media_assets.json
MEDIA_ASSET_BUILD_DIR := $(BUILD_DIR)/media-assets
WINDOW35_DATA_MANIFEST := analysis/link_identity/window35_data.json
WINDOW35_DATA_BUILD_DIR := $(BUILD_DIR)/window35-data
WINDOW11_RODATA_MANIFEST := analysis/link_identity/window11_rodata.json
WINDOW11_RODATA_BUILD_DIR := $(BUILD_DIR)/window11-rodata
CODE_WINDOWS_MANIFEST := analysis/link_identity/code_windows.json
CODE_WINDOWS_BUILD_DIR := $(BUILD_DIR)/code-windows
SJCRUNCH_PACKING_MANIFEST := analysis/link_identity/sjcrunch_packing.json
SJCRUNCH_PACKING_BUILD_DIR := $(BUILD_DIR)/sjcrunch-packing
SJCRUNCH_PACKING_OUTPUT := $(SJCRUNCH_PACKING_BUILD_DIR)/container.bin
SJCRUNCH_LZO_ARG := $(if $(strip $(SNESSTATION_LZO_LIBRARY)),--lzo-library "$(SNESSTATION_LZO_LIBRARY)",)
DECOMPDEV_REPORT_CONTRACT := analysis/decompdev/report_contract.json
DECOMPDEV_REPORT := $(BUILD_DIR)/decompdev/report.json
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
	named-contracts named-contracts-check named-contracts-verify named-contracts-refresh named-contracts-public-check \
	libgcc-contracts libgcc-contracts-check libgcc-contracts-verify libgcc-contracts-refresh libgcc-contracts-public-check \
	runtime-refactors runtime-refactors-check runtime-refactors-verify runtime-refactors-public-check \
	runtime-members runtime-members-check runtime-members-verify runtime-members-refresh runtime-members-public-check \
	data-backing data-backing-check data-backing-verify data-backing-refresh data-backing-public-check \
	link-layout-probe link-layout-probe-check link-layout-probe-refresh link-layout-probe-public-check \
	startup-integration startup-integration-check startup-integration-refresh startup-integration-public-check \
	frontend-eh-frames frontend-eh-frames-check frontend-eh-frames-refresh frontend-eh-frames-public-check \
	historical-tail-data historical-tail-data-check historical-tail-data-refresh historical-tail-data-public-check \
	runtime-tail-data runtime-tail-data-check runtime-tail-data-refresh runtime-tail-data-public-check \
	tail-metadata tail-metadata-check tail-metadata-refresh tail-metadata-public-check \
	window36-data window36-data-check window36-data-refresh window36-data-public-check \
	media-assets media-assets-check media-assets-refresh media-assets-public-check \
	window35-data window35-data-check window35-data-refresh window35-data-public-check \
	window11-rodata window11-rodata-check window11-rodata-refresh window11-rodata-public-check \
	code-windows code-windows-check code-windows-refresh code-windows-public-check \
	sjcrunch-packing sjcrunch-packing-check sjcrunch-packing-public-check \
	decompdev-report decompdev-report-check decompdev-report-public-check \
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
	@echo "SNES Station v0.23 decompilation"
	@echo
	@echo "  make status          show the current audited counts"
	@echo "  make check           run every public repository check"
	@echo "  make docs            regenerate the current status files"
	@echo "  make reference       verify and unpack original/SNES_EMU.ELF privately"
	@echo "  make reproduce-check run every implemented public and private gate"
	@echo "  make sjcrunch-packing-check  verify all 13 compressed blocks privately"
	@echo "  make reproduce       run the complete maintained pipeline"
	@echo "  make bootstrap-ee-stage1  build the historical EE C compiler"
	@echo "  make bootstrap-ee-cxx-stage1  build the historical EE C/C++ compiler"
	@echo "  make decompdev-report generate the public Objdiff report"
	@echo "  make elf-status      show the remaining final-ELF blockers"
	@echo
	@echo "See docs/TOOLS.md for commands and docs/RECOVERY_HISTORY.md for provenance."

help-legacy:
	@echo "Historical evidence runners remain available for frozen-manifest verification."
	@echo "They are implementation details, not the normal project interface."
	@echo "See tools/history/ and docs/RECOVERY_HISTORY.md."

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

check: check-generated check-links host-syntax test-tools checkpoint-1041-audit layout-oracle-public-check source-aliases-public-check link-contracts-public-check private-assets-public-check provider-frontier-public-check named-data-public-check named-contracts-public-check libgcc-contracts-public-check runtime-refactors-public-check runtime-members-public-check runtime-overrides-public-check rom-offsets-public-check historical-data-public-check unnamed-data-public-check data-backing-public-check link-layout-probe-public-check startup-integration-public-check frontend-eh-frames-public-check historical-tail-data-public-check runtime-tail-data-public-check tail-metadata-public-check window36-data-public-check media-assets-public-check window35-data-public-check window11-rodata-public-check code-windows-public-check sjcrunch-packing-public-check decompdev-report-public-check
	@echo "repository checks: OK"

decompdev-report:
	$(PYTHON) tools/decompdev_report.py --contract "$(DECOMPDEV_REPORT_CONTRACT)" generate \
		--output "$(DECOMPDEV_REPORT)"
	$(PYTHON) tools/decompdev_report.py --contract "$(DECOMPDEV_REPORT_CONTRACT)" validate \
		--report "$(DECOMPDEV_REPORT)"

decompdev-report-check: decompdev-report

decompdev-report-public-check:
	$(PYTHON) tools/decompdev_report.py --contract "$(DECOMPDEV_REPORT_CONTRACT)" validate

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
	@echo "Historical runtime/source batch: OK (50 runtime + 4 strict matches)"

hunt1000plus-v46-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1000plus_v46_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)" \
		--libgcc "$(EE_STAGE1_WORK_DIR)/build/gcc-ee-stage1/gcc/libgcc.a"
	@echo "Historical compiler batch: OK (42 strict matches)"

hunt1000plus-v47-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1000plus_v47_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "Historical source batch: OK (79 strict matches)"

hunt1041-v48-evidence: bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v48_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "Strict matching batch: OK (25 matches)"

hunt1041-v49-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v49_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "Strict ELF batch: OK (20 matches)"

hunt1041-v51-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v51_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "Exact ELF batch: OK (16 matches)"

hunt1041-v52-evidence: reference bootstrap-ee-stage1 bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v52_closure.py \
		--cc "$(EE_STAGE1_CC)" \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "Exact ELF batch: OK (17 matches)"

hunt1041-v72-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v72_promote_v53.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "Recovered exact batch: OK (6 promoted matches)"

hunt1041-v73-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v73_historical_io.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "PS2 I/O evidence: OK (2 matches)"

hunt1041-v74-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v74_spc7110_rtc.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "SPC7110 RTC evidence: OK (2 matches)"

hunt1041-v75-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v75_c4.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "C4 float/math evidence: OK (5 matches + 1 exact companion)"

hunt1041-v76-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v76_c4spr.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "C4SprDisintegrate evidence: OK (1 match)"

hunt1041-v77-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v77_c4draw.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "C4DrawWireFrame evidence: OK (1 match)"

hunt1041-v78-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v78_c4bit.py \
		--cxx "$(EE_STAGE1_CXX)"
	@echo "C4BitPlaneWave evidence: OK (1 match)"

hunt1041-v79-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v79_c4conv.py \
		--assembler "$(EE_CXX_STAGE1_WORK_DIR)/prefix/bin/ee-as"
	@echo "C4ConvOAM evidence: OK (1 match)"

hunt1041-v80-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v80_quickwins.py \
		--assembler "$(EE_CXX_STAGE1_WORK_DIR)/prefix/bin/ee-as"
	@echo "Quick-win evidence: OK (23 matches)"

hunt1041-v81-evidence: reference bootstrap-ee-cxx-stage1
	$(PYTHON) tools/history/research/hunt1041_v81_final20.py \
		--assembler "$(EE_CXX_STAGE1_WORK_DIR)/prefix/bin/ee-as"
	@echo "Final function evidence: OK (20 matches; 1041/1041 closed)"

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

# Verify the closed original 54-row named-data set without
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

# Close the historical 205 named contracts plus seven zlib peers.
# Public verification uses only addresses, extents and hashes. Private link
# verification materializes exact ranges only below ignored build/.
named-contracts: reference bootstrap-ee-stage1
	$(MAKE) named-contracts-check EE_CC="$(EE_STAGE1_CC)"

named-contracts-check: private-assets-check
	@test -f "$(REFERENCE_RAW)" || { \
		echo "Missing private unpacked reference: $(REFERENCE_RAW)" >&2; \
		echo "Run make reference first." >&2; \
		exit 2; \
	}
	$(PYTHON) tools/named_contracts.py link \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--source-alias-manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--stage3c-manifest "$(NAMED_DATA_MANIFEST)" \
		--manifest "$(NAMED_CONTRACT_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--compiler "$(EE_CC)" \
		--input "$(NAMED_CONTRACT_INPUT)" \
		--build-dir "$(NAMED_CONTRACT_BUILD_DIR)" \
		--output "$(NAMED_CONTRACT_OUTPUT)" \
		--report "$(NAMED_CONTRACT_REPORT)"

named-contracts-verify: reference
	$(PYTHON) tools/named_contracts.py verify \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--source-alias-manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(NAMED_CONTRACT_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--report "$(NAMED_CONTRACT_REPORT)"

# Refreshing private fingerprints is a reviewed range-boundary decision.
named-contracts-refresh: reference
	$(PYTHON) tools/named_contracts.py refresh \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--source-alias-manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(NAMED_CONTRACT_MANIFEST)" \
		--reference "$(REFERENCE_RAW)"

named-contracts-public-check:
	$(PYTHON) tools/named_contracts.py validate \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--defined-map "$(SOURCE_TREE_DEFINED_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--source-alias-manifest "$(SOURCE_ALIAS_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(NAMED_CONTRACT_MANIFEST)"

# Freeze the seven historical compiler-runtime contracts.
# Four contracts select complete archive-member .text sections; three are
# closed source-lift refactors and therefore must stay absent from externals.
libgcc-contracts: reference bootstrap-ee-stage1
	$(MAKE) libgcc-contracts-check EE_CC="$(EE_STAGE1_CC)"

libgcc-contracts-check: named-contracts-check
	$(PYTHON) tools/libgcc_contracts.py verify \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(LIBGCC_CONTRACT_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--compiler "$(EE_CC)" \
		--build-dir "$(LIBGCC_CONTRACT_BUILD_DIR)" \
		--report "$(LIBGCC_CONTRACT_REPORT)"

libgcc-contracts-verify: reference check-ee-compiler
	$(PYTHON) tools/libgcc_contracts.py verify \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(LIBGCC_CONTRACT_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--compiler "$(EE_CC)" \
		--build-dir "$(LIBGCC_CONTRACT_BUILD_DIR)" \
		--report "$(LIBGCC_CONTRACT_REPORT)"

# Refreshing archive-member fingerprints is a reviewed identity decision.
libgcc-contracts-refresh: reference check-ee-compiler
	$(PYTHON) tools/libgcc_contracts.py refresh \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(LIBGCC_CONTRACT_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--compiler "$(EE_CC)" \
		--build-dir "$(LIBGCC_CONTRACT_BUILD_DIR)"

libgcc-contracts-public-check:
	$(PYTHON) tools/libgcc_contracts.py validate \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--layout-manifest "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(LIBGCC_CONTRACT_MANIFEST)"

# The four formatter call sites all select the existing sprintf target.
# This closes one source-only contract, not another historical archive member.
runtime-refactors: reference bootstrap-ee-stage1
	$(MAKE) runtime-refactors-check EE_CC="$(EE_STAGE1_CC)"

runtime-refactors-check: libgcc-contracts-check
	$(PYTHON) tools/runtime_refactors.py verify \
		--manifest "$(RUNTIME_REFACTOR_MANIFEST)" \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--report "$(RUNTIME_REFACTOR_REPORT)"

runtime-refactors-verify: reference
	$(PYTHON) tools/runtime_refactors.py verify \
		--manifest "$(RUNTIME_REFACTOR_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--report "$(RUNTIME_REFACTOR_REPORT)"

runtime-refactors-public-check:
	$(PYTHON) tools/runtime_refactors.py validate \
		--manifest "$(RUNTIME_REFACTOR_MANIFEST)" \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)"

# Reproduce pinned PS2LIB source recipes, archive selection and complete
# member text. Rejected puts/abort candidates stay outside selected archives.
runtime-members: reference bootstrap-ee-stage1
	$(MAKE) runtime-members-check EE_CC="$(EE_STAGE1_CC)"

runtime-members-check: runtime-refactors-check
	$(PYTHON) tools/runtime_members.py verify \
		--manifest "$(RUNTIME_MEMBER_MANIFEST)" \
		--objects "$(RUNTIME_MEMBER_OBJECTS)" \
		--inputs "$(RUNTIME_MEMBER_INPUTS)" \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)" \
		--reference "$(REFERENCE_RAW)" \
		--compiler "$(EE_CC)" \
		--build-dir "$(RUNTIME_MEMBER_BUILD_DIR)" \
		--report "$(RUNTIME_MEMBER_REPORT)" $(RUNTIME_MEMBER_CACHE_ARG)

runtime-members-verify: reference check-ee-compiler
	$(PYTHON) tools/runtime_members.py verify \
		--compiler "$(EE_CC)" $(RUNTIME_MEMBER_CACHE_ARG)

# Capturing new fingerprints is an explicit reviewed identity decision.
runtime-members-refresh: reference check-ee-compiler
	$(PYTHON) tools/runtime_members.py capture \
		--compiler "$(EE_CC)" $(RUNTIME_MEMBER_CACHE_ARG)

runtime-members-public-check:
	$(PYTHON) tools/runtime_members.py validate \
		--manifest "$(RUNTIME_MEMBER_MANIFEST)" \
		--objects "$(RUNTIME_MEMBER_OBJECTS)" \
		--inputs "$(RUNTIME_MEMBER_INPUTS)" \
		--external-map "$(SOURCE_TREE_EXTERNAL_MAP)" \
		--contracts "$(LINK_CONTRACT_MANIFEST)" \
		--frontier-manifest "$(PROVIDER_FRONTIER_MANIFEST)"

.PHONY: runtime-overrides runtime-overrides-check runtime-overrides-verify runtime-overrides-refresh runtime-overrides-public-check unnamed-data unnamed-data-verify unnamed-data-refresh unnamed-data-public-check rom-offsets-public-check rom-offsets-verify historical-data historical-data-check historical-data-public-check historical-data-verify

runtime-overrides: reference bootstrap-ee-stage1
	$(MAKE) runtime-overrides-check EE_CC="$(EE_STAGE1_CC)"

runtime-overrides-check: runtime-members-check
	$(PYTHON) tools/runtime_overrides.py verify --compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)"

runtime-overrides-verify: reference check-ee-compiler
	$(PYTHON) tools/runtime_overrides.py verify --compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)"

runtime-overrides-refresh: reference check-ee-compiler
	$(PYTHON) tools/runtime_overrides.py capture --compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)"

runtime-overrides-public-check:
	$(PYTHON) tools/runtime_overrides.py validate

unnamed-data: reference
	$(PYTHON) tools/unnamed_data.py verify --reference "$(REFERENCE_RAW)"

unnamed-data-verify: unnamed-data

unnamed-data-refresh: reference
	$(PYTHON) tools/unnamed_data.py capture --reference "$(REFERENCE_RAW)"

unnamed-data-public-check:
	$(PYTHON) tools/unnamed_data.py validate

data-backing: reference bootstrap-ee-stage1
	$(MAKE) bootstrap-ee-cxx-stage1
	$(MAKE) data-backing-check EE_CC="$(EE_STAGE1_CC)"

data-backing-check: named-contracts-check unnamed-data-verify rom-offsets-verify historical-data-check
	$(PYTHON) tools/data_backing.py link \
		--compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)" \
		--historical-compiler "$(EE_STAGE1_CXX)" \
		--manifest "$(DATA_BACKING_MANIFEST)" --sections "$(DATA_BACKING_SECTIONS)" \
		--input "$(NAMED_CONTRACT_OUTPUT)" --build-dir "$(DATA_BACKING_BUILD_DIR)" \
		--output "$(DATA_BACKING_OUTPUT)" --report "$(DATA_BACKING_REPORT)"

data-backing-verify: reference
	$(PYTHON) tools/data_backing.py verify --reference "$(REFERENCE_RAW)" \
		--manifest "$(DATA_BACKING_MANIFEST)" --sections "$(DATA_BACKING_SECTIONS)"

data-backing-refresh: reference
	$(PYTHON) tools/data_backing.py capture --reference "$(REFERENCE_RAW)" \
		--manifest "$(DATA_BACKING_MANIFEST)" --sections "$(DATA_BACKING_SECTIONS)"

data-backing-public-check:
	$(PYTHON) tools/data_backing.py validate \
		--manifest "$(DATA_BACKING_MANIFEST)" --sections "$(DATA_BACKING_SECTIONS)"

link-layout-probe: data-backing
	$(MAKE) link-layout-probe-check EE_CC="$(EE_STAGE1_CC)"

link-layout-probe-check:
	@test -f "$(DATA_BACKING_OUTPUT)" || { echo "missing $(DATA_BACKING_OUTPUT); run make data-backing-check" >&2; exit 2; }
	$(PYTHON) tools/link_layout_probe.py probe \
		--compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --build-dir "$(LINK_LAYOUT_PROBE_BUILD_DIR)" \
		--manifest "$(LINK_LAYOUT_PROBE_MANIFEST)"

link-layout-probe-refresh:
	@test -f "$(DATA_BACKING_OUTPUT)" || { echo "missing $(DATA_BACKING_OUTPUT); run make data-backing-check" >&2; exit 2; }
	$(PYTHON) tools/link_layout_probe.py capture \
		--compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --build-dir "$(LINK_LAYOUT_PROBE_BUILD_DIR)" \
		--manifest "$(LINK_LAYOUT_PROBE_MANIFEST)"

link-layout-probe-public-check:
	$(PYTHON) tools/link_layout_probe.py validate --manifest "$(LINK_LAYOUT_PROBE_MANIFEST)"

.PHONY: startup-integration startup-integration-check startup-integration-refresh startup-integration-public-check

startup-integration: data-backing
	$(MAKE) startup-integration-check EE_CC="$(EE_STAGE1_CC)"

startup-integration-check:
	@test -f "$(DATA_BACKING_OUTPUT)" || { echo "missing $(DATA_BACKING_OUTPUT); run make data-backing-check" >&2; exit 2; }
	$(PYTHON) tools/startup_integration.py probe \
		--compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --build-dir "$(STARTUP_INTEGRATION_BUILD_DIR)" \
		$(STARTUP_INTEGRATION_SOURCE_CACHE_ARG) --manifest "$(STARTUP_INTEGRATION_MANIFEST)"

startup-integration-refresh:
	@test -f "$(DATA_BACKING_OUTPUT)" || { echo "missing $(DATA_BACKING_OUTPUT); run make data-backing-check" >&2; exit 2; }
	$(PYTHON) tools/startup_integration.py capture \
		--compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --build-dir "$(STARTUP_INTEGRATION_BUILD_DIR)" \
		$(STARTUP_INTEGRATION_SOURCE_CACHE_ARG) --manifest "$(STARTUP_INTEGRATION_MANIFEST)"

startup-integration-public-check:
	$(PYTHON) tools/startup_integration.py validate --manifest "$(STARTUP_INTEGRATION_MANIFEST)"

frontend-eh-frames: startup-integration
	$(MAKE) frontend-eh-frames-check EE_CC="$(EE_STAGE1_CC)"

frontend-eh-frames-check:
	@test -f "$(DATA_BACKING_OUTPUT)" || { echo "missing $(DATA_BACKING_OUTPUT); run make data-backing-check" >&2; exit 2; }
	@test -f "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" || { echo "missing startup object; run make startup-integration-check" >&2; exit 2; }
	$(PYTHON) tools/frontend_eh_frames.py probe \
		--compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--source "$(FRONTEND_EH_SOURCE)" --build-dir "$(FRONTEND_EH_BUILD_DIR)" \
		--manifest "$(FRONTEND_EH_MANIFEST)"

frontend-eh-frames-refresh:
	@test -f "$(DATA_BACKING_OUTPUT)" || { echo "missing $(DATA_BACKING_OUTPUT); run make data-backing-check" >&2; exit 2; }
	@test -f "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" || { echo "missing startup object; run make startup-integration-check" >&2; exit 2; }
	$(PYTHON) tools/frontend_eh_frames.py capture \
		--compiler "$(EE_CC)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--source "$(FRONTEND_EH_SOURCE)" --build-dir "$(FRONTEND_EH_BUILD_DIR)" \
		--manifest "$(FRONTEND_EH_MANIFEST)"

frontend-eh-frames-public-check:
	$(PYTHON) tools/frontend_eh_frames.py validate --manifest "$(FRONTEND_EH_MANIFEST)"

historical-tail-data: frontend-eh-frames bootstrap-ee-cxx-stage1
	$(MAKE) historical-tail-data-check EE_STAGE1_CXX="$(EE_STAGE1_CXX)"

historical-tail-data-check:
	@test -f "$(DATA_BACKING_OUTPUT)" || { echo "missing $(DATA_BACKING_OUTPUT); run make data-backing-check" >&2; exit 2; }
	@test -f "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" || { echo "missing startup object; run make startup-integration-check" >&2; exit 2; }
	$(PYTHON) tools/historical_tail_data.py probe \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--frontend-manifest "$(FRONTEND_EH_MANIFEST)" --build-dir "$(HISTORICAL_TAIL_BUILD_DIR)" \
		--manifest "$(HISTORICAL_TAIL_MANIFEST)"

historical-tail-data-refresh:
	@test -f "$(DATA_BACKING_OUTPUT)" || { echo "missing $(DATA_BACKING_OUTPUT); run make data-backing-check" >&2; exit 2; }
	@test -f "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" || { echo "missing startup object; run make startup-integration-check" >&2; exit 2; }
	$(PYTHON) tools/historical_tail_data.py capture \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--frontend-manifest "$(FRONTEND_EH_MANIFEST)" --build-dir "$(HISTORICAL_TAIL_BUILD_DIR)" \
		--manifest "$(HISTORICAL_TAIL_MANIFEST)"

historical-tail-data-public-check:
	$(PYTHON) tools/historical_tail_data.py validate --manifest "$(HISTORICAL_TAIL_MANIFEST)"

runtime-tail-data: historical-tail-data bootstrap-ee-cxx-stage1
	$(MAKE) runtime-tail-data-check EE_STAGE1_CXX="$(EE_STAGE1_CXX)"

runtime-tail-data-check:
	@test -f "$(HISTORICAL_TAIL_BUILD_DIR)/stage3i-historical-tail-integrated.elf" || { echo "missing historical tail output; run make historical-tail-data" >&2; exit 2; }
	$(PYTHON) tools/runtime_tail_data.py probe \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--prior-build "$(HISTORICAL_TAIL_BUILD_DIR)" --build-dir "$(RUNTIME_TAIL_BUILD_DIR)" \
		--manifest "$(RUNTIME_TAIL_MANIFEST)"

runtime-tail-data-refresh:
	@test -f "$(HISTORICAL_TAIL_BUILD_DIR)/stage3i-historical-tail-integrated.elf" || { echo "missing historical tail output; run make historical-tail-data" >&2; exit 2; }
	$(PYTHON) tools/runtime_tail_data.py capture \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--prior-build "$(HISTORICAL_TAIL_BUILD_DIR)" --build-dir "$(RUNTIME_TAIL_BUILD_DIR)" \
		--manifest "$(RUNTIME_TAIL_MANIFEST)"

runtime-tail-data-public-check:
	$(PYTHON) tools/runtime_tail_data.py validate --manifest "$(RUNTIME_TAIL_MANIFEST)"

tail-metadata: runtime-tail-data runtime-members bootstrap-ee-stage1
	$(MAKE) tail-metadata-check EE_CC="$(EE_CC)" EE_STAGE1_CXX="$(EE_STAGE1_CXX)"

tail-metadata-check:
	@test -f "$(RUNTIME_TAIL_BUILD_DIR)/stage3j-runtime-tail-integrated.elf" || { echo "missing runtime-tail output; run make runtime-tail-data" >&2; exit 2; }
	$(PYTHON) tools/tail_metadata.py probe \
		--c-compiler "$(EE_CC)" --compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--runtime-build "$(RUNTIME_MEMBER_BUILD_DIR)" --build-dir "$(TAIL_METADATA_BUILD_DIR)" \
		--manifest "$(TAIL_METADATA_MANIFEST)"

tail-metadata-refresh:
	@test -f "$(RUNTIME_TAIL_BUILD_DIR)/stage3j-runtime-tail-integrated.elf" || { echo "missing runtime-tail output; run make runtime-tail-data" >&2; exit 2; }
	$(PYTHON) tools/tail_metadata.py capture \
		--c-compiler "$(EE_CC)" --compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--runtime-build "$(RUNTIME_MEMBER_BUILD_DIR)" --build-dir "$(TAIL_METADATA_BUILD_DIR)" \
		--manifest "$(TAIL_METADATA_MANIFEST)"

tail-metadata-public-check:
	$(PYTHON) tools/tail_metadata.py validate --manifest "$(TAIL_METADATA_MANIFEST)"

window36-data: tail-metadata bootstrap-ee-cxx-stage1
	$(MAKE) window36-data-check EE_STAGE1_CXX="$(EE_STAGE1_CXX)"

window36-data-check:
	@test -f "$(TAIL_METADATA_BUILD_DIR)/stage3k-tail-metadata-integrated.elf" || { echo "missing tail-metadata output; run make tail-metadata" >&2; exit 2; }
	$(PYTHON) tools/window36_data.py probe \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --build-dir "$(WINDOW36_DATA_BUILD_DIR)" \
		--manifest "$(WINDOW36_DATA_MANIFEST)"

window36-data-refresh:
	@test -f "$(TAIL_METADATA_BUILD_DIR)/stage3k-tail-metadata-integrated.elf" || { echo "missing tail-metadata output; run make tail-metadata" >&2; exit 2; }
	$(PYTHON) tools/window36_data.py capture \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --build-dir "$(WINDOW36_DATA_BUILD_DIR)" \
		--manifest "$(WINDOW36_DATA_MANIFEST)"

window36-data-public-check:
	$(PYTHON) tools/window36_data.py validate --manifest "$(WINDOW36_DATA_MANIFEST)"

media-assets: window36-data bootstrap-ee-cxx-stage1
	$(MAKE) media-assets-check EE_STAGE1_CXX="$(EE_STAGE1_CXX)"

media-assets-check:
	@test -f "$(WINDOW36_DATA_BUILD_DIR)/stage3l-window36-integrated.elf" || { echo "missing window-36 output; run make window36-data" >&2; exit 2; }
	$(PYTHON) tools/media_assets.py probe \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --stage3l-build "$(WINDOW36_DATA_BUILD_DIR)" \
		--build-dir "$(MEDIA_ASSET_BUILD_DIR)" --manifest "$(MEDIA_ASSET_MANIFEST)"

media-assets-refresh:
	@test -f "$(WINDOW36_DATA_BUILD_DIR)/stage3l-window36-integrated.elf" || { echo "missing window-36 output; run make window36-data" >&2; exit 2; }
	$(PYTHON) tools/media_assets.py capture \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --stage3l-build "$(WINDOW36_DATA_BUILD_DIR)" \
		--build-dir "$(MEDIA_ASSET_BUILD_DIR)" --manifest "$(MEDIA_ASSET_MANIFEST)"

media-assets-public-check:
	$(PYTHON) tools/media_assets.py validate --manifest "$(MEDIA_ASSET_MANIFEST)"

window35-data: media-assets bootstrap-ee-cxx-stage1
	$(MAKE) window35-data-check EE_STAGE1_CXX="$(EE_STAGE1_CXX)"

window35-data-check:
	@test -f "$(MEDIA_ASSET_BUILD_DIR)/stage3m-media-integrated.elf" || { echo "missing media-assets output; run make media-assets" >&2; exit 2; }
	$(PYTHON) tools/window35_data.py probe \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --stage3l-build "$(WINDOW36_DATA_BUILD_DIR)" \
		--stage3m-build "$(MEDIA_ASSET_BUILD_DIR)" --build-dir "$(WINDOW35_DATA_BUILD_DIR)" \
		--manifest "$(WINDOW35_DATA_MANIFEST)"

window35-data-refresh:
	@test -f "$(MEDIA_ASSET_BUILD_DIR)/stage3m-media-integrated.elf" || { echo "missing media-assets output; run make media-assets" >&2; exit 2; }
	$(PYTHON) tools/window35_data.py capture \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --stage3l-build "$(WINDOW36_DATA_BUILD_DIR)" \
		--stage3m-build "$(MEDIA_ASSET_BUILD_DIR)" --build-dir "$(WINDOW35_DATA_BUILD_DIR)" \
		--manifest "$(WINDOW35_DATA_MANIFEST)"

window35-data-public-check:
	$(PYTHON) tools/window35_data.py validate --manifest "$(WINDOW35_DATA_MANIFEST)"

window11-rodata: window35-data bootstrap-ee-cxx-stage1
	$(MAKE) window11-rodata-check EE_STAGE1_CXX="$(EE_STAGE1_CXX)"

window11-rodata-check:
	@test -f "$(WINDOW35_DATA_BUILD_DIR)/stage3n-window35-integrated.elf" || { echo "missing window-35 output; run make window35-data" >&2; exit 2; }
	$(PYTHON) tools/window11_rodata.py probe \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --stage3l-build "$(WINDOW36_DATA_BUILD_DIR)" \
		--stage3m-build "$(MEDIA_ASSET_BUILD_DIR)" --stage3n-build "$(WINDOW35_DATA_BUILD_DIR)" \
		--runtime-build "$(RUNTIME_MEMBER_BUILD_DIR)" --source-tree "$(SOURCE_TREE_BUILD_DIR)/objects" \
		--build-dir "$(WINDOW11_RODATA_BUILD_DIR)" --manifest "$(WINDOW11_RODATA_MANIFEST)"

window11-rodata-refresh:
	@test -f "$(WINDOW35_DATA_BUILD_DIR)/stage3n-window35-integrated.elf" || { echo "missing window-35 output; run make window35-data" >&2; exit 2; }
	$(PYTHON) tools/window11_rodata.py capture \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --stage3l-build "$(WINDOW36_DATA_BUILD_DIR)" \
		--stage3m-build "$(MEDIA_ASSET_BUILD_DIR)" --stage3n-build "$(WINDOW35_DATA_BUILD_DIR)" \
		--runtime-build "$(RUNTIME_MEMBER_BUILD_DIR)" --source-tree "$(SOURCE_TREE_BUILD_DIR)/objects" \
		--build-dir "$(WINDOW11_RODATA_BUILD_DIR)" --manifest "$(WINDOW11_RODATA_MANIFEST)"

window11-rodata-public-check:
	$(PYTHON) tools/window11_rodata.py validate --manifest "$(WINDOW11_RODATA_MANIFEST)"

code-windows: window11-rodata bootstrap-ee-cxx-stage1
	$(PYTHON) tools/code_windows.py prepare \
		--c-compiler "$(EE_CC)" --compiler "$(EE_STAGE1_CXX)"
	$(MAKE) code-windows-check EE_STAGE1_CXX="$(EE_STAGE1_CXX)"

code-windows-check:
	@test -f "$(WINDOW11_RODATA_BUILD_DIR)/stage3o-window11-rodata-integrated.elf" || { echo "missing window-11 output; run make window11-rodata" >&2; exit 2; }
	$(PYTHON) tools/code_windows.py probe \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --stage3l-build "$(WINDOW36_DATA_BUILD_DIR)" \
		--stage3m-build "$(MEDIA_ASSET_BUILD_DIR)" --stage3n-build "$(WINDOW35_DATA_BUILD_DIR)" \
		--stage3o-build "$(WINDOW11_RODATA_BUILD_DIR)" --build-dir "$(CODE_WINDOWS_BUILD_DIR)" \
		--manifest "$(CODE_WINDOWS_MANIFEST)"

code-windows-refresh:
	$(PYTHON) tools/code_windows.py capture \
		--compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)" \
		--input "$(DATA_BACKING_OUTPUT)" --startup-object "$(STARTUP_INTEGRATION_BUILD_DIR)/crt0-stage3g.o" \
		--stage3i-build "$(HISTORICAL_TAIL_BUILD_DIR)" --stage3j-build "$(RUNTIME_TAIL_BUILD_DIR)" \
		--stage3k-build "$(TAIL_METADATA_BUILD_DIR)" --stage3l-build "$(WINDOW36_DATA_BUILD_DIR)" \
		--stage3m-build "$(MEDIA_ASSET_BUILD_DIR)" --stage3n-build "$(WINDOW35_DATA_BUILD_DIR)" \
		--stage3o-build "$(WINDOW11_RODATA_BUILD_DIR)" --build-dir "$(CODE_WINDOWS_BUILD_DIR)" \
		--manifest "$(CODE_WINDOWS_MANIFEST)"

code-windows-public-check:
	$(PYTHON) tools/code_windows.py validate --manifest "$(CODE_WINDOWS_MANIFEST)"

sjcrunch-packing: code-windows
	$(PYTHON) tools/sjcrunch_pack.py pack \
		--image "$(CODE_WINDOWS_BUILD_DIR)/stage3p-code-windows-integrated.padded.bin" \
		--layout "$(UNPACKED_LAYOUT_MANIFEST)" --manifest "$(SJCRUNCH_PACKING_MANIFEST)" \
		--output "$(SJCRUNCH_PACKING_OUTPUT)" $(SJCRUNCH_LZO_ARG)

sjcrunch-packing-check: code-windows
	@test -f original/SNES_EMU.ELF || { echo "missing private reference: original/SNES_EMU.ELF" >&2; exit 2; }
	$(PYTHON) tools/sjcrunch_pack.py check \
		--image "$(CODE_WINDOWS_BUILD_DIR)/stage3p-code-windows-integrated.padded.bin" \
		--packed original/SNES_EMU.ELF --layout "$(UNPACKED_LAYOUT_MANIFEST)" \
		--manifest "$(SJCRUNCH_PACKING_MANIFEST)" --output "$(SJCRUNCH_PACKING_OUTPUT)" \
		$(SJCRUNCH_LZO_ARG)

sjcrunch-packing-public-check:
	$(PYTHON) tools/sjcrunch_pack.py validate \
		--layout "$(UNPACKED_LAYOUT_MANIFEST)" --manifest "$(SJCRUNCH_PACKING_MANIFEST)"

rom-offsets-public-check:
	$(PYTHON) tools/rom_offsets.py validate

rom-offsets-verify: reference
	$(PYTHON) tools/rom_offsets.py verify --reference "$(REFERENCE_RAW)"

historical-data: reference bootstrap-ee-cxx-stage1
	$(MAKE) historical-data-check

# Use historical-data for an automatic compiler bootstrap; the -check target
# deliberately requires the selected EE_STAGE1_CXX to already exist.
historical-data-check: reference
	$(PYTHON) tools/historical_data.py build --compiler "$(EE_STAGE1_CXX)" --reference "$(REFERENCE_RAW)"

historical-data-verify: reference
	$(PYTHON) tools/historical_data.py verify --reference "$(REFERENCE_RAW)"

historical-data-public-check:
	$(PYTHON) tools/historical_data.py validate

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
	@echo "Completed:"
	@echo "  functions             1041/1041"
	@echo "  EE source ownership   97/97 translation units"
	@echo "  runtime contracts     53/53"
	@echo "  address identities    1265/1265"
	@echo "  historical backing    695316 public-source bytes"
	@echo
	@echo "Whole-image result:"
	@echo "  exact windows         51/51"
	@echo "  remaining windows     none"
	@echo "  remaining differences 0 bytes"
	@echo "  compressed blocks      13/13 exact (LZO1X-999 level 8)"
	@echo "  SJCRUNCH2 container    714268/714268 bytes exact"
	@echo
	@echo "Complete replacement ELF: NOT YET"
	@echo "  - prove complete object/array bounds needed by the link"
	@echo "  - reproduce final relocations, linker script and object/archive order"
	@echo "  - reproduce the 12700-byte loader stub/outer ELF and packed hash"
	@echo "See docs/REPRODUCTION.md"

elf: elf-status
	@echo "Refusing to emit a pretend replacement ELF." >&2
	@echo "Close the recorded evidence gates before implementing this target." >&2
	@exit 2

clean-matching:
	rm -rf "$(MATCH_DIR)"
