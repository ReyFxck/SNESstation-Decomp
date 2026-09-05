# V102 — clean Stage-3G link/layout probe

V101 closed the **address-identity frontier** at 1,265/1,265. V102 does not
reopen those 39 former blockers. It starts the next whole-program gate: link
the real Stage-3F aggregate as an executable, place every independently proved
provider section, apply the source relocations and compare the resulting
initialized image against the frozen unpacked oracle.

## Result

| Measurement | V102 result |
|---|---:|
| Stage-3F address identities | 1,265/1,265; 0 unresolved |
| Fixed provider sections at exact VMA and size | 179/179 |
| Initialized fixed sections with exact payload | 155/155 |
| Fixed zero-fill sections kept as `NOBITS` | 24/24 |
| Exact 64-KiB oracle windows | 12/51 |
| Equal initialized-image bytes | 1,421,069 / 3,304,936 |
| Bytes still different | 1,883,867 |
| Target entry | `0x00100008` |
| Diagnostic entry | `0x00111f70` |
| Terminal zero padding needed for comparison | 100 bytes |

The unpadded diagnostic image is 3,304,836 bytes with SHA-256
`f0b35112afa096488b6f3e97ff21e8d8af391764cda0148dc52e869b84664feb`.
After deterministic 100-byte terminal padding its SHA-256 is
`cf1b7e4003fe1dd5d3e9e5941074cc8c8395bd683be79d9454337838a035bc79`.
Neither equals the target SHA-256
`739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`.

The twelve exact chunks are 12, 13 and 37 through 46. Their equality is a
useful integration baseline, not permission to import the other target bytes.
The public manifest contains geometry, counts and hashes only; the private ELF
and generated diagnostic remain ignored under `build/`.

## What the gate proves

`tools/link_layout_probe.py` consumes the actual
`source-tree.data-backed.partial.o`, not a synthetic stand-in. It requires a
little-endian MIPS ELF32 `ET_REL`, applies all remaining relocations with the
historical EE binutils, produces an `ET_EXEC`, and rejects any surviving
relocation section. Every row in `data_backing_sections.tsv` must appear once
at its frozen address, size and section kind; all initialized payload hashes
must remain exact.

This proves that the current sources, aliases and data providers are linkable
and establishes a deterministic mismatch baseline. It does **not** prove:

- complete C object or array bounds for all address identities;
- selection of every exact matched implementation in the canonical objects;
- the original runtime member data, archive composition or global relocation
  order;
- the historical linker script, object/section/library order;
- an unpacked replacement image or the SJCRUNCH2/LZO packed executable.

## Why the entry still differs

The mismatch is concrete, not a linker-script guess. The canonical behavioral
tree exports `snes_p28_00100008` as an 888-byte (`0x378`) function in
`progress28_structural_lift_recovered.c`, and it begins at offset `0x11f70` in
the current combined `.text`. The strict function evidence instead identifies
the target entry as the 216-byte historical `_start` from PS2LIB/PS2SDK
`crt0.s` at `0x00100008` (`hunt500plus-v33-validated-204.tsv`, zero differing
bytes after precise relocation handling).

Simply relocating the 888-byte behavioral lift to `0x00100008` would hide the
problem and corrupt the rest of the layout. The next integration work must
select the already-proved 216-byte startup object, then repeat that process for
the exact implementation/archive roster before tuning historical ordering.

## Reproduction

Public, byte-free validation:

```bash
make link-layout-probe-public-check
```

Full private/historical gate after `make data-backing-check`:

```bash
make link-layout-probe-check \
  EE_CC=build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc
```

The C++ bootstrap also permanently patches GCC 3.2.2's invalid `foo(){}`
`-c/-o` configure probe. A previously poisoned build tree is preserved under
`build/toolchains/.../recovery/` and reconfigured; it is never silently reused
or deleted.

## Recoverable checkpoint

- Base: `be8c3ee3517918b5a550f54f138a3237cc0d5fc9` (`v101-stage3f-1265-complete`).
- Stage-3F: address frontier closed; complete bounds still open.
- Stage-3G: first clean link/layout diagnostic frozen and privately reproduced.
- Next target: exact startup/object selection, followed by runtime/archive data,
  full relocation/order identity, unpacked hash, and only then SJCRUNCH2 packing.
