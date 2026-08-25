# SNESstation-Decomp

Preservation-oriented decompilation of **SNES Station v0.23 WIP
(24 January 2004)** for PlayStation 2.

The project reconstructs the original PS2 frontend, Snes9x-derived core,
renderer, audio glue, filesystem code and historical runtime as faithfully as
the binary evidence allows. It is not a modern rewrite of the emulator.

> **Status:** active reverse engineering. Structural/source-model coverage is
> closed, but a complete byte-identical replacement ELF is not yet available.

<!-- DECOMP_PROGRESS_START -->
## Decompilation progress

<p align="center">
  <img src="assets/progress.svg" width="720" alt="SNES Station v0.23 decompilation progress" />
</p>

> **100% means structural coverage of the audited 1,041-entry target universe.** It is not a claim of byte matching or an exact compiler function count. See [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md) for the full accounting.

- **Matching:** 98.08%
- **Formal checkpoint:** **1,021/1,041** strict promoted matches
- **Recovered pending:** **0** (the V53 recovery set is fully formal)
- **Working checkpoint:** **1,021/1,041** with **20** entries remaining
- **Reconstructed:** **100.00%** (1,041/1,041 validated targets)
- **Mapped / identified:** **100.00%** (1,041/1,041 validated targets)
- **Source-form checkpoint:** **1,041 behavioral/source-model + 0 structural-pseudocode-only**
- **Complete replacement ELF:** **not yet**
- **Renderer draw family:** **100.0% reconstructed / 100.0% mapped**

The renderer-specific grid lives in [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md); the build/matching audit lives in [`docs/SOURCE_COMPLETENESS.generated.md`](docs/SOURCE_COMPLETENESS.generated.md).
<!-- DECOMP_PROGRESS_END -->

The formal count and the working checkpoint are intentionally separate. A row
becomes formal `MATCHING` only after its compiler object, function boundary,
relocations and target bytes are reproducibly verified. V80 applies that gate
to 23 complete non-C4 spans totaling 14,244 bytes through clearly labelled
raw-exact assembly reconstructions and tracks every one of the 20 remaining
entries; the V75 through V79 C4 proofs remain frozen as evidence.

## One-command reproduction goal

The stable entry point is:

```bash
make reproduce
```

Today that command validates every implemented gate and then stops honestly at
the unproven final-link stage. As the remaining work closes, the same command
will compile, link, pack and compare the replacement against the private
reference ELF.

Matching all audited functions is necessary, but it is not sufficient for an
identical ELF. Global data, section layout, relocations, object/archive order,
the linker script and SJCRUNCH2 packing must also match. See
[`docs/REPRODUCTION.md`](docs/REPRODUCTION.md) for the complete proof ladder.

Binary matching also cannot recover information erased by compilation, such as
the author's comments or every original local-variable name. The defensible
goal is source that reproduces the executable behavior and bytes, with original
identities used only where evidence proves them.

## Quick start

Requirements for the repository-only checks are Python 3, GNU Make and a host C
compiler:

```bash
make status
make check
```

To verify a legally obtained reference binary:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make reference
make reproduce-check
```

The historical EE stage-one compiler can be built without root installation:

```bash
make bootstrap-ee-stage1
```

Run `make help` for the maintained workflow and `make help-legacy` for frozen
historical evidence runners.

## Target fingerprint

| Item | Value |
|---|---|
| Identity | SNES Station v0.23 WIP, 24 January 2004 |
| Packed SHA-256 | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Packed entry | `0x01b00008` |
| Unpacked SHA-256 | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |
| Unpacked base / entry | `0x00100000` / `0x00100008` |
| Unpacked size | `3,304,936` bytes |
| Primary upstream baseline | Snes9x 1.41 |

Exact dependency fingerprints and remaining unknown revisions are recorded in
[`docs/DEPENDENCY_VERSIONS.md`](docs/DEPENDENCY_VERSIONS.md).

## Repository map

| Path | Purpose |
|---|---|
| `src/` | Reconstructed behavioral/source models grouped by subsystem |
| `include/` | Recovered declarations, ABI compatibility and symbols |
| `analysis/` | Authoritative manifests, maps, xrefs and immutable evidence |
| `matching/` | Isolated compiler candidates used by exact comparison gates |
| `tools/` | Maintained analysis, verification and reproduction entry points |
| `tools/history/` | Frozen one-off checkpoint and research scripts |
| `docs/` | Maintained guides, maps, status and research references |
| `docs/archive/` | Historical checkpoint reports retained for provenance |
| `third_party/` | Pinned historical material and provenance records |
| `original/` | Private, hash-gated reference supplied by the user |
| `build/` | Generated downloads, objects, reports and extracted assets |

The documentation index is [`docs/README.md`](docs/README.md). The analysis
layout and evidence policy are described in
[`analysis/README.md`](analysis/README.md).

## Project rules

- Binary evidence comes before upstream-source similarity.
- Modern PS2SDK code is never silently substituted for an unknown 2003–2004
  dependency.
- Structural reconstruction, recovered-but-unpromoted evidence and formal
  matching are reported as separate measurements.
- Generated status files must be refreshed with `make docs` and are checked by
  `make check`.
- Historical evidence is archived, not silently discarded.

## Reference ELF and legal note

The private reference is deliberately ignored by Git. Only its fingerprints,
provenance rules and deterministic extraction procedure belong in the public
repository. Read [`original/README.md`](original/README.md) and
[`docs/LEGAL.md`](docs/LEGAL.md) before distributing any binary or extracted
asset.

## Contributing

The highest-value work is closing the remaining compiler matches, recovering
the exact historical link environment and turning source models into coherent
build-ready translation units without weakening evidence. Read
[`CONTRIBUTING.md`](CONTRIBUTING.md) and
[`docs/DECOMP_PLAYBOOK.md`](docs/DECOMP_PLAYBOOK.md) before changing identities
or match status.
