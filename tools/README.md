# Tool layout

Maintained entry points live directly under `tools/` and are exercised by
`make check` or a documented Make target.

- `project_status.py` — reports formal and recovered-pending checkpoints.
- `update_progress.py` / `audit_source_completeness.py` — generate and verify
  project state.
- `compare_elf_functions.py` / `promote_match_evidence.py` — strict comparison
  and promotion support.
- `run_match_miner.py` — cached compiler-profile search.
- `reproduce.sh` — stable one-command whole-program pipeline.
- `research/` — current unfinished investigations only.
- `history/` — frozen one-off checkpoint generators and completed research.
- `patches/` — modern-host compatibility patches for the historical toolchain.

New reusable logic should include tests named `test_*.py`. New one-off scripts
should be moved to `tools/history/` once their evidence is frozen.
