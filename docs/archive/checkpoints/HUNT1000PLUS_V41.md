# HUNT1000+ V41 — cached direct matching

This pass introduced the cached address-anchored miner and promoted **28 strict
matches**, moving the repository from **695/1041 (66.76%)** to **723/1041
(69.45%)**.

The immutable evidence is
[`analysis/matching/hunt1000plus-v41-validated-28.tsv`](../analysis/matching/hunt1000plus-v41-validated-28.tsv).
It contains 22 exact-next-boundary proofs, four terminal-control-flow proofs,
and two four-byte target-zero-gap proofs. Twenty-four selected candidates use
`-O2`; four use `-Os`.

The source corrections are ordinary C-level reconstruction improvements:
addressed data/global accesses were restored so the historical compiler emits
the corresponding MIPS relocations, small return/empty leaves were given their
actual shapes, and the `0x0016fcd4` leaf selected the size-oriented profile that
avoids an unwanted R5900 multiply form.

No instruction word or byte sequence was copied from the target into source.
Every row records its source, object symbol, compiler profile, object SHA-256,
identity mode, boundary proof, and relocation-aware comparison result.
