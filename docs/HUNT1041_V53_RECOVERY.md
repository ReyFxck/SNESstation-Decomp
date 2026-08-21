# HUNT1041 V53 recovery — 984/1041 checkpoint

This file preserves the final exact-match results recovered from the interrupted
V53 research session after the committed V52 checkpoint at 978/1041.

The interrupted session validated **six additional audited entries**, taking the
working checkpoint to **984/1041 (94.52%)**, with **57 entries remaining**.

| Address | Recovered identity | Bytes | Recovery fact |
|---:|---|---:|---|
| `0x00129af4` | `S9xDoDMA` partition 1/2 | 2316 | first audited span of one historical symbol |
| `0x0012a400` | `S9xDoDMA` partition 2/2 | 4072 | same symbol; control flow crosses the boundary |
| `0x0015068c` | `LoadZip` | 1168 | historical Snes9x ZIP-loader path |
| `0x00158b74` | `SetOBC1` | 1116 | unaligned PS2 load profile (`lwl`/`lwr`) |
| `0x00181bac` | `S9xSetSPC7110` | 2480 | old SPC7110 implementation; PS2 clock path |
| `0x00182638` | `S9xUpdateRTC` | 728 | old SPC7110 RTC implementation; PS2 clock path |

## What was recovered

The original session recorded four exact entries first: the two audited
`S9xDoDMA` partitions, `LoadZip`, and `SetOBC1`, reaching 982/1041. It then
closed `S9xSetSPC7110` and `S9xUpdateRTC` by reproducing the PS2 port behavior
where the `time(NULL)` source path contributes zero, reaching 984/1041.

The `0x0012a400` boundary is demonstrably inside the same `S9xDoDMA` machine-code
function: the target contains a direct branch from `0x0012a36c` to `0x0012a400`,
and code after that boundary branches back into the first partition. It is an
audited-manifest split, not a second historical function.

`SetOBC1` contains repeated unaligned loads in the target (`lwl` followed by
`lwr`, then masking to 16 bits), matching the recovered PS2-specific source/profile
adjustment from the interrupted session.

## Evidence preservation status

The target-side spans and their SHA-256 hashes are preserved in
`analysis/matching/hunt1041-v53-recovered-target-spans.tsv` and can be checked
with `tools/research/verify_hunt1041_v53_recovery.py` after `make reference`.

The interrupted session did **not** leave behind its generated compiler-object
TSV/ZIP. Therefore this recovery file deliberately does **not** invent object
SHA-256 values or cache keys. Before promoting V53 into the strict repository
manifests, regenerate the historical GCC 3.2.2 objects and the normal
relocation-aware comparisons so the compiler-side evidence is restored too.

The private original ELF must remain outside version control.
