# V104 exact startup integration

V104 replaces the Stage-3G diagnostic entry with the exact historical EE
startup. It deliberately stops at the first unproved application byte and does
not claim a replacement ELF.

## Frozen result

- Base: V103 commit `5b3bc900e274c025a0e565d7750c6ab0629beeb5`.
- Source: `ee/startup/src/crt0.s` from PS2SDK commit
  `694100b78ad5bc8f8248a1138143860af4f8435f`.
- Source SHA-256:
  `13ab436418c8b8e815173fc99a5a40f889e7e58c80ad15e6ffc83096bcd15999`.
- Compiler contract: EE GCC 3.2.2, target `ee`, with the repository's frozen
  R5900 flags.
- Exact initialized corridor: `0x00100000..0x00100114`, 276 bytes, SHA-256
  `ecdea86dc1a457ea1951ae338815041e1a5a5f7eecd80324240aaba9874d092f`.
- Exact symbols: `_start@0x00100008` (216 bytes), `_exit@0x001000e0`
  (44 bytes), and `_root@0x0010010c` (8 bytes).
- Exact ELF entry: `0x00100008`.
- Relocations: all 27 startup text relocations are applied before comparison.
- Startup BSS: `0x00426e80..0x00427000` (384 bytes), including `_args` and
  `_args_ptr`. Four existing four-byte zero-fill anchors inside this interval
  become absolute symbols instead of duplicate storage.
- Stage-3F placement: the other 175 of 179 fixed sections retain their proved
  VMAs, extents, types and initialized payloads.

The public manifest is
[`analysis/link_identity/startup_integration.json`](../../analysis/link_identity/startup_integration.json).
It contains source hashes, layout hashes, symbols, counts and whole-image
measurements, but no private target bytes.

## Whole-image boundary

The startup-integrated diagnostic has entry `0x00100008`, but it is not the
replacement executable:

- the first remaining difference is `0x00100114`, immediately after `_root`;
- 12 of 51 oracle windows are exact and 39 still differ;
- 1,420,794 bytes equal the target and 1,884,142 differ;
- the padded diagnostic SHA-256 is
  `0a6f0c0b8c84367e5e5d8ea44e40ae7e502a4bcba1967bbc5c45d20356ae5512`,
  not the target hash;
- the pre-existing behavioral `_start` lift remains inside the aggregate and
  is not discarded until exact implementation selection proves that change;
- historical application object order, archive member data, complete object
  bounds, final section layout and SJCRUNCH2 packing remain unproved.

The byte-difference count is 275 higher than the V102 diagnostic because V104
adds the exact 276-byte startup before the existing behavioral aggregate while
removing no unproved code. This regression is intentional and visible; V104
closes an identity contract, not an image-size optimization.

## Reproduce

Public validation needs no original ELF:

```bash
make startup-integration-public-check
```

With the legally obtained private reference and the historical EE compiler:

```bash
make data-backing-check \
  EE_CC=build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc

make startup-integration-check \
  EE_CC=build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc
```

The private gate rebuilds the pinned public startup source, checks the source
and relocatable hashes, links it ahead of the real Stage-3F aggregate, verifies
all startup/fixed-section invariants and compares the complete padded image.

## Next target

Continue at `0x00100114`: select exact application implementations and their
historical object/archive order, then integrate runtime member data and full
object/array bounds. Only after the unpacked SHA-256 equals
`739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`
can the SJCRUNCH2 stage pursue the packed hash.
