# Link and whole-image evidence

These manifests define how recovered source becomes an exact R5900 image. They
contain addresses, sizes, hashes, ownership decisions and relocation facts;
private target payloads are never stored here.

## Layout and current image result

| Manifest | Purpose |
|---|---|
| `unpacked_layout.json` | Packed/unpacked target geometry and 51 hash windows |
| `link_layout_probe.json` | First honest executable-layout diagnostic |
| `startup_integration.json` | Exact target entry and startup code |
| `frontend_eh_frames.json` | Frontend C++ unwind metadata |
| `historical_tail_data.json` | Rebuilt late Snes9x data and unwind records |
| `runtime_tail_data.json` | Rebuilt runtime tail containers |
| `tail_metadata.json` | Final source/semantic metadata at the image tail |
| `window36_data.json` | Exact Snes9x data and CFI in image window 36 |
| `media_assets.json` | Hash-only private media integration contract |
| `window35_data.json` | Exact Snes9x data and CFI in image window 35 |
| `window11_rodata.json` | Public-source read-only data in window 11 |
| `code_windows.json` | Current 45/51 whole-image result and exact windows 1–6 |

## Names, providers and data ownership

| Manifest group | Purpose |
|---|---|
| `source_address_aliases.tsv`, `source_alias_reviews.tsv` | Bind alternate target names to canonical recovered text |
| `link_contracts.tsv`, `link_contract_reviews.tsv` | Classify address anchors and semantic aliases |
| `private_asset_providers.tsv`, `provider_frontier_closure.tsv` | Close the source-link provider namespace without publishing assets |
| `named_data.tsv`, `named_data_reviews.tsv` | Prove named program-data ranges and source refactors |
| `named_contracts.tsv` | Prove named text/data/external contracts |
| `unnamed_data_accesses.tsv`, `pcm_buffer_consumed_extents.tsv` | Record instruction-proved minimum data spans |
| `historical_data.json`, `historical_fragments.json` | Pin public historical source intervals and focused fragments |
| `data_backing.tsv`, `data_backing_sections.tsv` | Assign all tracked addresses to real sections or proved refactors |
| `rom_offset_refactors.tsv` | Distinguish ROM-relative constants from false image objects |
| `runtime_code_pointers.tsv`, `runtime_residual_identities.tsv`, `final_residual_identities.tsv` | Close remaining code-pointer and metadata identities |

## Runtime evidence

| Manifest group | Purpose |
|---|---|
| `libgcc_contracts.tsv` | GCC helper archive identities and source refactors |
| `runtime_refactors.tsv` | Source-level runtime dependency corrections |
| `runtime_members.tsv`, `runtime_member_objects.tsv`, `runtime_member_inputs.tsv` | Complete historical PS2LIB member recipes |
| `runtime_overrides.tsv`, `runtime_override_witnesses.tsv` | Target-selected runtime providers and incoming call evidence |

## Claim rules

- An address identity does not by itself prove an object's size or bytes.
- A minimum consumed span does not prove a complete array boundary.
- A function match does not prove object/archive order or final relocation
  values.
- A hash-verified private provider may be generated under `build/`, but its
  payload must not be committed.
- A whole-image window counts only when every byte in that 64 KiB region is
  equal to the reference.

Run `make check` for public validation and `make reproduce-check` with a legal
reference ELF for the private comparisons.
