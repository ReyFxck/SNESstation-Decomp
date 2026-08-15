# Aplicar Progress 49 — `get_tree` 1/1 MATCH e enviar ao GitHub

Este overlay fecha `get_tree @ 0x0018c124` como **1/1 committed-listing MATCH**.
O C histórico continua preservado; o matcher formal usa uma reconstrução `.S`
claramente identificada, igual à política já usada para `numtestf`.

O `apply-progress49.sh` exige o EE stage-one já construído e executa o strict
gate automaticamente. Se não aparecer `1/1`, o script falha e não deve ser
commitado.

```bash
set -e

cd ~/SNESstation-Decomp
git status --short
git pull --ff-only origin main

rm -rf /tmp/SNESstation-Decomp-Progress49
unzip -q /storage/emulated/0/Download/SNESstation-Decomp-Progress49.zip -d /tmp

bash /tmp/SNESstation-Decomp-Progress49/apply-progress49.sh "$PWD"

git diff --check
git status --short
git diff --stat

git add -- \
  DROIDSPACE_UPDATE.md \
  Makefile \
  analysis/matching/get_tree.csv \
  analysis/matching/get-tree-listing-report.md \
  docs/DECOMP_STATE.md \
  docs/MATCHED_CHECKPOINT.md \
  docs/MATCHING_WORKFLOW.md \
  docs/PROGRESS49_GET_TREE_MATCH.md \
  matching/candidates/get_tree.S \
  matching/candidates/get_tree.c \
  tools/run-get-tree-match.sh

git diff --cached --check
git diff --cached --stat

git commit -m "Match legacy ZIP get_tree"
git push origin main
```

Resultado obrigatório do apply/runner:

```text
Result: **1/1 relocation-normalized matches**
get_tree committed-listing strict gate: OK
```

Se `original/SNES_EMU.ELF` existir localmente, o runner também executa o gate
formal contra o ELF de referência. O ELF original nunca é adicionado ao commit.
