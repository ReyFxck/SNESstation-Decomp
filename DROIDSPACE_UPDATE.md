# Aplicar o checkpoint `mathfp` completo e enviar ao GitHub

Este ZIP é um overlay pequeno para aplicar por cima do repositório atualizado.
Ele não substitui nem apaga o toolchain já extraído em `build/`.

```bash
set -e

cd ~/SNESstation-Decomp
git status --short
git pull --ff-only origin main

python3 -m zipfile -e \
  /storage/emulated/0/Download/SNESstation-Decomp-MatchingPhase4-mathfp-complete.zip \
  .

make check
git diff --check

EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
test -x "$EE_CC"

make toolchain-probe EE_CC="$EE_CC"
make -B build/matching/mathfp/mathfp.o EE_CC="$EE_CC"
make match-mathfp-listing EE_CC="$EE_CC"

# O relatório deve mostrar: Result: 7/7 relocation-normalized matches
grep -F 'Result: **7/7 relocation-normalized matches**' \
  analysis/matching/mathfp-listing-report.md

git status --short
git add -- \
  DROIDSPACE_UPDATE.md \
  Makefile \
  README.md \
  analysis/matching/mathfp.csv \
  analysis/matching/mathfp-listing-report.md \
  analysis/progress_targets.csv \
  analysis/source_readiness.csv \
  analysis/symbols.csv \
  docs/DEPENDENCY_VERSIONS.md \
  docs/HISTORICAL_EE_TOOLCHAIN.md \
  docs/MATCHING_PHASE4_MATHFP.md \
  docs/MATCHING_WORKFLOW.md \
  docs/RESEARCH_LOG.md \
  docs/ROADMAP.md \
  docs/SOURCE_COMPLETENESS.generated.md \
  matching/candidates/mathfp.c \
  matching/candidates/mathfp_numtest.c \
  matching/candidates/mathfp_numtest.S

git diff --cached --check
git diff --cached --stat
git commit -m "Match complete EE mathfp corridor"
git push origin HEAD:main
```

`make match-mathfp-listing` usa os bytes do disassembly versionado e não
precisa do executável original. A verificação formal contra o ELF completo
continua opcional e exige uma cópia legal em `original/SNES_EMU.ELF`:

```bash
make reference
make match-mathfp EE_CC="$EE_CC"
```

O `numtestf` tem duas representações intencionais: o arquivo `.c` é o modelo
legível do comportamento e o `.S` reproduz os 128 bytes observados. O assembly
é identificado como reconstrução e não como suposto fonte original.
