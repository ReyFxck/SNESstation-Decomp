# Tool layout

Maintained entry points live directly under `tools/` and are exercised by
`make check` or a documented Make target.

- `project_status.py` — reports formal and any recovered-but-unpromoted checkpoints.
- `update_progress.py` / `audit_source_completeness.py` — generate and verify
  project state.
- `update_frontier_map.py` — generate and verify the complete V77 45-entry
  two-track work queue.
- `compare_elf_functions.py` / `promote_match_evidence.py` — strict comparison
  and promotion support.
- `run_match_miner.py` — cached compiler-profile search.
- `reproduce.sh` — stable one-command whole-program pipeline.
- `research/` — current unfinished investigations only.
- `history/` — frozen one-off checkpoint generators and completed research.
- `patches/` — modern-host compatibility patches for the historical toolchain.

New reusable logic should include tests named `test_*.py`. New one-off scripts
should be moved to `tools/history/` once their evidence is frozen.

The completed V73 PS2-I/O source-variant proof remains reproducible through
`make hunt1041-v73-evidence`; its runner is archived under `history/research/`
because the accepted evidence set is immutable.

The completed V74 SPC7110 RTC proof follows the same policy through
`make hunt1041-v74-evidence`; its runner is archived beside the V73 runner.

The completed V75 C4 float/math proof follows the same policy through
`make hunt1041-v75-evidence`; it reproduces five formal rows and one separately
counted exact `C4Op15` boundary companion.

The completed V76 `C4SprDisintegrate` proof is reproduced through
`make hunt1041-v76-evidence`; the runner pins the official source, the narrow
PS2 code-generation shim, the historical compiler and the complete target
span before accepting the match.

The completed V77 `C4DrawWireFrame` proof is reproduced through
`make hunt1041-v77-evidence`; it adds the target-proven packed three-byte read
form and compares the complete function with precise MIPS relocation masks.
