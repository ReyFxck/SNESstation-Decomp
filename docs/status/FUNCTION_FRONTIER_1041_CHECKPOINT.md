# Frozen 1,041-function checkpoint

The canonical public checkpoint is tagged **`function-frontier-1041-v81`**.
It freezes the V81 state in which all **1,041/1,041** audited function spans
have formal exact evidence and the generated working frontier contains zero
entries.

This tag closes only the function-code proof gate. It deliberately does not
claim a coherent historical object set, an unpacked replacement image, a
packed replacement ELF or textual identity with the lost original source.

## Permanent public gate

Run this from any clean checkout of the tag:

```bash
make checkpoint-1041-check
```

The gate runs all repository checks and additionally verifies:

- the three 1,041-row authoritative manifests have identical address sets;
- every formal and source-readiness status remains `MATCHING`;
- every row retains a behavioral/source model;
- the V81 frontier remains empty;
- the final V81 batch remains 20 exact spans totaling 71,384 bytes;
- the frozen V81 evidence files match
  [`function-frontier-1041-v81.sha256`](../../analysis/checkpoints/function-frontier-1041-v81.sha256);
- the packed and unpacked target fingerprints remain present; and
- no private ELF, unpacked image or generated `build/` binary is tracked.

## Private-reference gate

Copy a legally obtained reference into the ignored local path and run:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make checkpoint-1041-reference-check
```

This verifies and unpacks the hash-pinned reference, rebuilds the V81 final-20
candidate with the historical EE toolchain, compares all 71,384 bytes and then
prints the still-open whole-program ELF gates.

## Frozen fingerprints

| Form | SHA-256 |
|---|---|
| Packed reference ELF | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Unpacked reference image | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |

The reference ELF, unpacked image, extracted IRX modules and other embedded
assets remain private and are not release assets. The public checkpoint
contains only source, maps, hashes and deterministic verification logic.

## Tagging the checkpoint

After committing the checkpoint files on `main`, create and push the annotated
tag without rewriting an existing tag:

```bash
test -z "$(git tag -l function-frontier-1041-v81)"
git tag -a function-frontier-1041-v81 -m "Freeze audited function frontier at 1041 of 1041"
git push origin main
git push origin function-frontier-1041-v81
```

Any later build-ready, linker or packing work starts after this immutable tag.
Historical proof files named in the checksum manifest must not be rewritten;
new evidence belongs in a new checkpoint.
