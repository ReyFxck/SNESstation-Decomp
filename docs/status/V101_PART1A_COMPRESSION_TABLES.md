# V101 Part 1A — compression-table source owners

V101 Part 1A closes two Stage-3F storage identities with minimal public-source
table reconstructions, each privately byte-verified against the SHA-pinned
unpacked target:

- `DAT_004243d8`: SNESticle/unzip `UWORD mask_bits[17]`;
- `DAT_00425970`: zlib `inflate_mask[17]`.

These owners claim only the exact table extents, not surrounding translation
unit layout.

Expected Stage-3F:
- 1206 `SECTION_BACKED_ADDRESS`;
- 29 ROM source refactors;
- 10 code-pointer aliases;
- 20 remaining `NO_PROVED_BACKING`;
- 1265 total contracts.

No replacement ELF identity is claimed.
