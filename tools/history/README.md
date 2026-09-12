# Historical tools

These scripts reproduce completed matching experiments still consumed by the
current evidence gates. They are not part of the everyday interface.

- `research/` contains the historical compiler/source experiments that
  `make reproduce-check` can still rerun.

Old source-mutating generators, checkpoint applicators and commit/push helpers
were removed after their results were frozen in the manifests and Git history.
Keeping those scripts in the working tree offered no reproducibility benefit
and made the supported workflow harder to identify.

Use root Make targets and the maintained command table in
[`../../docs/TOOLS.md`](../../docs/TOOLS.md) for current work.
