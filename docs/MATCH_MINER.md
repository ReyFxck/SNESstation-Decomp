# Cached strict match miner

`tools/run_match_miner.py` replaces repeated manual MIPS trial-and-error with a
deterministic, resumable scan. It compiles every address-labelled source unit in
parallel, caches successes and deterministic compiler failures, and compares
candidate functions directly against the hash-gated unpacked reference.

## Fast path

```bash
make reference
make bootstrap-ee-stage1
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make match-miner EE_CC="$EE_CC" MATCH_MINER_JOBS=8
```

The default matrix is deliberately limited to `o2`, `os`, and `o2-nosched1`.
These three profiles found every strict hit discovered by the 16-profile sweep
at this checkpoint. Run `make match-miner-full` only after changing source,
headers, compiler, or flags; unchanged inputs are served from the content cache.
Already promoted addresses are excluded by default, so zero strict hits means
“no new direct match.” Pass `--include-matching` when running the Python tool to
turn known matches into a regression gate.

To keep evidence in the repository, choose an immutable report name:

```bash
python3 tools/run_match_miner.py \
  --compiler "$EE_CC" \
  --jobs 8 \
  --report analysis/matching/hunt-next-validated.tsv
```

## Evidence and promotion

A row is emitted only when all opcode/register bits compare exactly after known
MIPS relocation fields are normalized. Unknown relocation types are rejected.
The candidate must also have one of these boundary proofs:

- its size reaches the next audited target exactly;
- the remaining target gap is at most 16 bytes, aligned, and all zero;
- an exact prefix ends in `jr $ra` or a closed unconditional self-loop.

Promotion is separate and dry-run by default. It rechecks the reference hash,
candidate object/cache metadata, source path and identity, independently repeats
the relocation-aware byte comparison, proves the boundary from target/object
bytes, and verifies agreement between both manifests:

```bash
python3 tools/promote_match_evidence.py \
  analysis/matching/hunt-next-validated.tsv \
  --label "HUNT next"

python3 tools/promote_match_evidence.py \
  analysis/matching/hunt-next-validated.tsv \
  --label "HUNT next" \
  --apply
```

`build/match-miner/near-misses.tsv` retains the best mismatch per address. Work
from the smallest byte delta first instead of recompiling the whole corpus by
hand. Neither tool writes target-derived instructions into source or treats
structural reconstruction as byte matching.
