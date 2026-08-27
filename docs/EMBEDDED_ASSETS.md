# Embedded asset map

This map covers the binary resources explicitly consumed by the SNES Station
v0.23 boot, frontend, audio and Memory Card paths. It deliberately does not
classify ordinary Snes9x lookup tables as artwork or media. Finding an asset
does not add a function match: the audited matching checkpoint is
978/1,041 after the V52 machine-code evidence promotion.

The complete hashes and decoded-text hashes are recorded in
[`analysis/embedded_assets.csv`](../analysis/embedded_assets.csv). The ranges
below are virtual addresses in the unpacked image based at `0x00100000`; every
end is exclusive.

| Resource | Range | Bytes | Format and identity |
|---|---:|---:|---|
| CDVD module | `0x001ec300..0x001f40d4` | 32,212 | little-endian MIPS IRX/ELF32 |
| SjPCM module | `0x001f4100..0x001f60c5` | 8,133 | little-endian MIPS IRX/ELF32 |
| AmigaMod module | `0x001f6140..0x001faf9d` | 20,061 | little-endian MIPS IRX/ELF32 |
| Frontend background | `0x001fafd0..0x00290fe0` | 614,416 | `IIF1`, 640x480, `GS_PSMCT16` |
| SNES Station logo | `0x00290ff0..0x002ab1d0` | 106,976 | `IIF1`, 382x70, `GS_PSMCT32` |
| Panel corner | `0x002ab1e0..0x002abaf0` | 2,320 | `IIF1`, 24x24, `GS_PSMCT32` |
| Frontend font | `0x002abb00..0x002ebc20` | 262,432 | `BFNT`, 256x256, 16x16 grid, `GS_PSMCT32` |
| Credits | `0x002ebc30..0x002ec1f1` | 1,473 | words XORed with `0x96695aa5` |
| Disclaimer | `0x002ec200..0x002ec53a` | 826 | words XORed with `0x96695aa5` |
| Azazel music | `0x002ec540..0x0032294c` | 222,220 | ProTracker `M.K.` module |
| Memory Card icon | `0x00322980..0x00335278` | 76,024 | PS2 3D icon plus 128x128 texture |

The three IIF pointers and the BFNT pointer are stored consecutively at
`0x001bb310..0x001bb31f`. Function `0x001019a8` uploads them to GS memory. The
headers independently derive all four boundaries, and the next resource begins
after only alignment padding. No PNG, JPEG, GIF, TIM2, DDS or other conventional
image signature occurs in the unpacked image.

## Azazel module

Function `0x00105d78` passes `0x002ec540` to the AmigaMod loader. The word at
`0x0032294c` initially contains `0x3640c`, exactly the format-derived MOD size.
The loader rounds that word to `0x36410` before the IOP transfer; the extractor
keeps the valid 222,220-byte MOD and does not append the four bytes belonging to
the adjacent size word.

The module is identified internally as:

- title: `can't stop coming`;
- signature: `M.K.` (four channels);
- song length: 30 positions;
- patterns: 19;
- sample data: 201,680 bytes;
- author strings: `_azazel / dcs_`, `_azze/dualcrewshining` and
  `(c)-by azazel in -94`.

The ProTracker equation is exact:

```text
1084-byte header + 19 * 1024 pattern bytes + 201680 sample bytes = 222220
```

## Text and graphics

The main routine decodes the disclaimer at `0x001050b4` and the credits at
`0x00105104`. Each loop XORs only complete 32-bit words, matching the target;
the one- or two-byte remainder is preserved. The decoded hashes in the CSV make
the transformation independently checkable.

The Memory Card path at `0x00107358` generates the 964-byte `icon.sys` structure
at runtime, but writes the embedded model at `0x00322980` unchanged as
`snes_emu.ico`. That file is a PlayStation 2 icon, not a Windows ICO. It contains
1,800 unindexed vertices (600 triangles), one shape, one animation frame and an
uncompressed 128x128 A1B5G5R5 texture. The extractor emits the original file,
its texture as PNG and a convenience OBJ/MTL conversion.

The IIF/BFNT layouts are also present in the pinned PGEN GSLIB headers under
`build/upstream/pgen-403f1710/lib/gslib051/include/`. The PS2 icon geometry and
texture boundary were cross-checked against the public `PS2Icon` reader in
[`ticky/ps2iconsys@7cef493`](https://github.com/ticky/ps2iconsys/tree/7cef4936dbe2ce95963e7052db49a25bad1ae538),
after the target's own header and exact file size had established the identity.

## Reproduce locally

Keep a legally obtained reference at `original/SNES_EMU.ELF`, then run:

```bash
make extract-assets
```

To materialize only the five bundles currently required by the source-tree
link contracts (three IRXs plus the encoded credits and disclaimer), run:

```bash
make private-assets
```

That gate verifies the same private ranges, padding and size words, emits an
ignored provider object, and reduces the current unresolved partial-link
frontier from 237 to 227. The frozen V86 report retains its historical
pre-Stage-3E count of 258 to 248. It does not write standalone asset copies
into the tracked tree; see
[`status/V86_PRIVATE_ASSET_PROVIDERS.md`](status/V86_PRIVATE_ASSET_PROVIDERS.md).

The `make extract-assets` command verifies both reference SHA-256 values,
unpacks the ELF and writes private results to `build/extracted-assets/`:

- the three IRXs;
- the three original IIF files and PNG previews;
- the BFNT file plus transparent and dark-background PNG previews;
- decoded credits and disclaimer text;
- `azazel-cant-stop-coming.mod`;
- `snes_emu.ico`, its texture PNG, and OBJ/MTL geometry;
- a JSON manifest and `SHA256SUMS.txt`.

`build/` is ignored. Do not force-add the original ELF, extracted IRXs, music,
graphics or Memory Card icon. Only the extractor, documentation, offsets and
hashes belong in the public repository.
