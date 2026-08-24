# V53 recovered exact results pending promotion

The interrupted V53 research session recovered six additional exact function
results after the formal V52 checkpoint at 978/1041. Their target-side spans
and hashes survived, but the generated compiler-object TSV/ZIP did not.

They are therefore tracked separately from the formal `MATCHING` manifest. The
later promotion of `0x001029c4` raised the formal count to 979, so the current
working accounting is:

```text
979 formal + 6 recovered pending = 985/1041 working; 56 remain
```

| Address | Recovered identity | Bytes | Recovery fact |
|---:|---|---:|---|
| `0x00129af4` | `S9xDoDMA` partition 1/2 | 2316 | first audited span of one historical symbol |
| `0x0012a400` | `S9xDoDMA` partition 2/2 | 4072 | same symbol; control flow crosses the boundary |
| `0x0015068c` | `LoadZip` | 1168 | historical Snes9x ZIP-loader path |
| `0x00158b74` | `SetOBC1` | 1116 | unaligned PS2 load profile (`lwl`/`lwr`) |
| `0x00181bac` | `S9xSetSPC7110` | 2480 | old SPC7110 implementation; PS2 clock path |
| `0x00182638` | `S9xUpdateRTC` | 728 | old SPC7110 RTC implementation; PS2 clock path |

The `0x0012a400` audited boundary lies inside the same historical `S9xDoDMA`
machine-code function. Direct control flow crosses the split in both directions;
it is not a claim that the historical source contained two functions.

`SetOBC1` preserves the target's repeated unaligned `lwl`/`lwr` loads. The two
SPC7110 results preserve the PS2 port behavior in which the `time(NULL)` source
path contributes zero.

## Promotion requirements

Target-side spans and hashes are frozen in
[`hunt1041-v53-recovered-target-spans.tsv`](../../analysis/matching/hunt1041-v53-recovered-target-spans.tsv).
After `make reference`, verify them with:

```bash
python3 tools/research/verify_hunt1041_v53_recovery.py
```

Before changing either authoritative manifest to `MATCHING`, regenerate:

1. the historical GCC 3.2.2 compiler objects;
2. the normal relocation-aware comparisons;
3. exact boundary and unknown-relocation checks;
4. object SHA-256 values and immutable evidence rows.

The private original ELF remains outside version control.
