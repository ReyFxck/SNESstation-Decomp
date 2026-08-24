# Progress 49 — `get_tree` 1/1 committed-listing match

Target: `get_tree @ 0x0018c124..0x0018c1f8` (212 bytes).

## Historical source evidence

The readable candidate is the K&R `get_tree` from:

- repository: `iaddis/SNESticle`
- commit: `9590ebf3bf768424ebd6cb018f322e724a7aade3`
- source: `Gep/Source/common/unzip/explode.c`
- surviving EE listing:
  `SNESticle/Project/ps2/release_EE3.2.2-b1/explode.lst`

The historical source shape is confirmed, but its surviving SNESticle
EE3.2.2-b1 listing is **208 bytes**, not the SNES Station target's 212 bytes.
The difference is compiler/codegen, not a reason to deform the historical C:

- SNESticle keeps a second `bytebuf` base register live and uses `$s0` as the
  outer count;
- SNES Station uses `$s0` as the `bytebuf` base, shifts the count/state
  allocation across `$s1..$s3`, and emits one extra `lui %hi(bytebuf)` in the
  outer-loop delay slot.

This is consistent with the wider compiler-fingerprint evidence that the
surviving 2004-02-14 BETA 3 backend is not necessarily the exact January 2004
application compiler.

## Matching representation

`matching/candidates/get_tree.c` remains the readable historical-source model.
`matching/candidates/get_tree.S` is a clearly labelled byte-exact reconstruction
for the formal function matcher, following the same policy already used for the
`numtestf` leaf: assembly reconstruction is evidence, not a claim of Hiryu's
verbatim original source.

The reconstruction preserves real MIPS relocations for:

- `bytebuf`: `R_MIPS_HI16` / `R_MIPS_LO16`;
- `ReadByte`: `R_MIPS_26`.

All opcode/register bits therefore still have to match under the project's
bit-precise MIPS comparator.

## Result

Independent assembly of the candidate against the checked-in target bytes gives:

- target/candidate: **212 / 212 bytes**;
- relocations in the function: **9**;
- non-relocation differing bytes: **0**;
- result: **1/1 relocation-normalized MATCHING**;
- strict committed-listing gate: **PASS**.

`tools/run-get-tree-match.sh` always deletes the candidate object before the
repository gate, preventing stale-object false positives. If a legal
`original/SNES_EMU.ELF` is present it also runs the formal reference-ELF
function gate.

## Scope

This closes one more **function-level committed-listing match**. It does not
prove the exact original compiler build, linker layout, or complete replacement
ELF.
