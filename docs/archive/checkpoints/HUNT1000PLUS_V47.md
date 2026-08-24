# HUNT1000+ V47 — 900-match closure

V47 promotes **79** strict matches, moving the project from **821/1041
(78.87%)** to **900/1041 (86.46%)**. There are **141** audited targets left.

| Boundary proof | Matches | Gate |
|---|---:|---|
| Exact next address | 73 | Candidate size equals the audited target span |
| Terminal control flow | 6 | Exact prefix ends in a MIPS return or closed tail; candidate is at most 4 KiB |

Every row is rebuilt before validation and has zero differing non-relocation
bits, no unknown relocation type, a proven audited boundary, and a SHA-256
bound candidate object. The independent promotion tool then repeats the object
comparison before changing either manifest.

## Sources and profiles

| Source group | Matches | Reproducible profile |
|---|---:|---|
| Recovered SNES Station frontend bodies | 14 | EE GCC 3.2.2, `-Os`, R5900 ABI |
| Official Snes9x 1.41-1 renderer | 27 | Private fixed-RGB555 build, `-Os -fshort-double` |
| PGEN unzip/zlib | 11 | Pinned commit `403f1710`, size profile |
| Historical PS2SDK/PS2LIB | 25 | Pinned 15 and 18 April 2004 split translation units |
| Newlib 1.10.0 qsort | 1 | Historical 32-bit `size_t` TU contract |
| Official Snes9x 1.41 zlib | 1 | `read_buf`, size profile |

The renderer runner patches only a private build copy of `port.h`; the
SHA-verified official archive remains unchanged. The narrow qsort and gzio
compatibility headers encode historical ABI declarations that materially
change code generation. They do not substitute function bodies.

## Reproduce

Keep the verified 24 January 2004 ELF at `original/SNES_EMU.ELF` locally, then
run:

```sh
make hunt1000plus-v47-evidence EE_BUILD_JOBS=8
python3 tools/promote_match_evidence.py \
  analysis/matching/hunt1000plus-v47-validated-79.tsv \
  --label "HUNT1000+ V47 closure"
```

The promotion command above is a non-mutating validation pass unless
`--apply` is explicitly added. The evidence table is
[`hunt1000plus-v47-validated-79.tsv`](../analysis/matching/hunt1000plus-v47-validated-79.tsv).

Only the official SNES Station v0.23 WIP target is used. Later unofficial ELF
builds are outside this evidence chain, and no original ELF or proprietary IRX
is committed.
