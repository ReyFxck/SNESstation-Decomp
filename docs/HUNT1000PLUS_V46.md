# HUNT1000+ V46 — official-source and archive closure

V46 promotes **42** strict matches, moving the project from **779/1041
(74.83%)** to **821/1041 (78.87%)**. There are **220** audited targets left.

| Boundary proof | Matches | Gate |
|---|---:|---|
| Exact next address | 11 | Candidate size equals the audited target span |
| Terminal control flow | 29 | Exact prefix ends in a MIPS return or closed tail; candidate is at most 4 KiB |
| Historical symbol plus zero gap | 2 | Exact symbol is followed by four verified zero bytes |

Every row has zero differing non-relocation bits, no unknown relocation type,
and is independently recomputed from the generated ELF object before
promotion. Repeated fingerprints are not treated as name proof: their evidence
records the historical object instance or anchored source order while the
existing address-labelled manifest identity is retained.

The reproducible inputs are:

- the official Snes9x 1.41-1 source archive from Lysator, pinned by SHA-256;
- PGEN commit `403f1710e5eacb7d04e5031e1cb0a40435ff9d33`;
- PS2DEV commit `bac0006c6302edcf1bdae253799484497b4e5032`;
- the pinned EE GCC 3.2.2 stage-one `libgcc.a`;
- PGEN's historical `libgs.a` member `gsFont.o`.

The two calendar helpers are separated by local object layout: the SPC7110
copy follows the already matched `S9xGetSPC7110Byte`, while the SRTC copy follows
the already matched `S9xSRTCComputeDayOfWeek`. The multitap and DSP duplicate
fingerprints use the same adjacent-object anchoring.

## Reproduce

Keep the verified 24 January 2004 ELF at `original/SNES_EMU.ELF` locally, then
run:

```sh
make hunt1000plus-v46-evidence EE_BUILD_JOBS=8
python3 tools/promote_match_evidence.py \
  analysis/matching/hunt1000plus-v46-validated-42.tsv \
  --label "HUNT1000+ V46 closure"
```

The promotion command is a non-mutating validation pass unless `--apply` is
explicitly added. The evidence table is
[`hunt1000plus-v46-validated-42.tsv`](../analysis/matching/hunt1000plus-v46-validated-42.tsv).

Only the official SNES Station v0.23 WIP target is used. Later unofficial ELF
builds are outside this evidence chain, and no original ELF or proprietary IRX
is committed.
