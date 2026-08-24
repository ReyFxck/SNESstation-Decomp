# V72: six V53 recoveries formally promoted

V72 regenerated the missing historical compiler objects for all six results
preserved from the interrupted V53 session. Each audited span now passes the
normal formal gate against the hash-pinned unpacked ELF:

- exact next-manifest boundary;
- zero differing non-relocation bytes;
- relocation-normalized equality;
- zero unknown MIPS relocation types;
- recorded object, cache-key and target-span SHA-256 values.

The promotion raises the authoritative checkpoint from **979/1041** to
**985/1041**, leaves **0 recovered results pending**, and leaves **56 audited
entries** on the function frontier.

| Address | Historical identity | Bytes | Object coverage |
|---:|---|---:|---|
| `0x00129af4` | `S9xDoDMA` partition 1/2 | 2316 | symbol bytes `0..2316` |
| `0x0012a400` | `S9xDoDMA` partition 2/2 | 4072 | symbol bytes `2316..6388` |
| `0x0015068c` | `LoadZip` | 1168 | full symbol |
| `0x00158b74` | `SetOBC1` | 1116 | full symbol |
| `0x00181bac` | `S9xSetSPC7110` | 2480 | full symbol |
| `0x00182638` | `S9xUpdateRTC` | 728 | full symbol |

The two DMA rows are adjacent audited partitions of one 6388-byte historical
`S9xDoDMA` symbol. The split is a manifest boundary, not a claim that the
historical source contained two functions.

## Reproducing the proof

Supply the private reference ELF and rebuild the historical C++ frontend, then
run the maintained evidence target:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v72-evidence
```

The runner privately copies the pinned Snes9x 1.41-1 source and reproduces the
target-specific source profile:

- the one-byte PS2 `Settings` layout used by `S9xDoDMA`;
- the SNES Station `assert` path used by `LoadZip`;
- packed `lwl`/`lwr` reads with bytewise writes in `SetOBC1`;
- 32-bit persisted time and the zero wall-clock path in the SPC7110 code.

Immutable results are in
[`hunt1041-v72-validated-v53-6.tsv`](../../analysis/matching/hunt1041-v72-validated-v53-6.tsv).
The original target-side recovery facts remain frozen in
[`hunt1041-v53-recovered-target-spans.tsv`](../../analysis/matching/hunt1041-v53-recovered-target-spans.tsv).

This closes the V53 promotion gap only. It does not claim that the final
translation-unit ownership, data layout, link order or packed replacement ELF
has already been reproduced.
