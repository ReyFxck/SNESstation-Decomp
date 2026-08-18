# HUNT400 batch

Base checkpoint: **376/1041** at `d77f013902e495fb44f65b856b295ec8754f4288`.
Strict matches added: **64**.
Resulting checkpoint: **440/1041 = 42.27%**.
Goal 400 reached: **yes**.

The batch accepts only relocation-aware strict comparator matches or anchored raw `.text` spans with no relocations and exact target bytes. Target-derived mass `.word` reconstruction is excluded.

## Evidence

- `analysis/matching/hunt400-v9-validated-64.tsv`
- `analysis/matching/hunt400-v9-object-provenance.tsv`
- `analysis/matching/hunt400-v9-inferred-zero-ranges.tsv`
- `analysis/matching/hunt400-v9-identity-audit.tsv`
- `analysis/matching/hunt400-v9-historical-source-archives.tsv`

## New strict functions

- `0x0010a18c` `snes_p13_color_blend_0010a18c` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0010a840` `apu_buffer_allocator` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0010b72c` `snes_p13_apu_io_write_0010b72c` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012b9a4` `snes_p13_record_ram_init_0012b9a4` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012d79c` `snes_p12_0012d79c` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012d85c` `snes_p12_0012d85c` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012d91c` `snes_p12_0012d91c` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012d9dc` `snes_p12_0012d9dc` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012da9c` `snes_p12_0012da9c` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012db5c` `snes_p12_0012db5c` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012e2e0` `snes_p11_0012e2e0` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0012e374` `snes_p13_bitplane_transpose_0012e374` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x00151360` `snes_p11_00151360` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0016f9b0` `snes_p11_0016f9b0` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x0016fa18` `snes_p11_0016fa18` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x00173e6c` `snes_p13_config_slot_00173e6c` — snes9x-1.42-official-lysator-source / anchored-object-layout / exact-next-boundary
- `0x00173f24` `snes_p12_00173f24` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x00173f50` `snes_p12_00173f50` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x00173ff0` `snes_p11_00173ff0` — snes9x-1.42-official-lysator-source / anchored-object-layout / exact-next-boundary
- `0x0017409c` `snes_p11_0017409c` — snes9x-1.42-official-lysator-source / anchored-object-layout / exact-next-boundary
- `0x0017422c` `snes_p12_0017422c` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x001742b4` `snes_p11_001742b4` — snes9x-1.42-official-lysator-source / anchored-object-layout / exact-next-boundary
- `0x001742f8` `snes_p12_001742f8` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x001744d0` `snes_p11_001744d0` — snes9x-1.42-official-lysator-source / anchored-object-layout / exact-next-boundary
- `0x00174618` `snes_p12_00174618` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x001746a4` `snes_p12_001746a4` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x00174830` `snes_p12_00174830` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x00183e04` `ConvertTile` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x0018428c` `DrawTile` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001859a8` `DrawLargePixel` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00185d8c` `DrawTile16` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001860a8` `DrawClippedTile16` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00186540` `DrawTile16x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x0018685c` `DrawClippedTile16x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00186cf4` `DrawTile16x2x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00187010` `DrawClippedTile16x2x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001874a8` `DrawLargePixel16` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x0018789c` `DrawTile16Add` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00187bb8` `DrawClippedTile16Add` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00188050` `DrawTile16Add1_2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x0018836c` `DrawClippedTile16Add1_2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00188804` `DrawTile16Sub` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00188b20` `DrawClippedTile16Sub` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00188fb8` `DrawTile16Sub1_2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001892d4` `DrawClippedTile16Sub1_2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x0018976c` `DrawTile16FixedAdd1_2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00189a88` `DrawClippedTile16FixedAdd1_2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x00189f20` `DrawTile16FixedSub1_2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x0018a23c` `DrawClippedTile16FixedSub1_2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x0019c5cc` `snes_dispatch_0019c5cc` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x001ac734` `GetBasePointer` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001aca50` `S9xAPUSetByte` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x001acbf0` `S9xAPUSetByteZ` — snes9x-1.42-official-lysator-source / anonymous-strict-fingerprint / exact-next-boundary
- `0x001acd04` `WRITE_4PIXELS` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001ace28` `WRITE_4PIXELS_FLIPPED` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001acf4c` `WRITE_4PIXELSx2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001ad1d4` `WRITE_4PIXELSx2x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001ad398` `WRITE_4PIXELS_FLIPPEDx2x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001ad55c` `WRITE_4PIXELS16` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001ad684` `WRITE_4PIXELS16_FLIPPED` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001ad7ac` `WRITE_4PIXELS16x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001ad8f4` `WRITE_4PIXELS16_FLIPPEDx2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001ada3c` `WRITE_4PIXELS16x2x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
- `0x001adc34` `WRITE_4PIXELS16_FLIPPEDx2x2` — snes9x-1.42-official-lysator-source / name-strict / exact-next-boundary
