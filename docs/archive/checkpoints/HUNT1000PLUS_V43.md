# HUNT1000+ V43 — signed boolean leaf

V43 promoted `snes_leaf_00193290` at `0x00193290`, moving matching from
**723/1041 (69.45%)** to **724/1041 (69.55%)**.

The compact boolean expression caused GCC 3.2.2 to select a different instruction
shape. Restoring the explicit signed branch/return form produced the exact
eight-byte target leaf under `-O2`. The strict exact-next-boundary result is in
[`analysis/matching/hunt1000plus-v43-validated-1.tsv`](../analysis/matching/hunt1000plus-v43-validated-1.tsv).
