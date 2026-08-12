# Aplicar este checkpoint e enviar ao GitHub

O ZIP desta fase é um **overlay de código**, não uma cópia dos arquivos
gerados em `build/`. Ele pode ser extraído por cima do ZIP anterior no
repositório atual; não apague `build/`.

```bash
set -e

cd ~/SNESstation-Decomp
git status --short
git pull --ff-only origin main

python3 -m zipfile -e \
  /storage/emulated/0/Download/SNESstation-Decomp-MatchingPhase4-mathfp-match2-arm64fix.zip \
  .

make check
git diff --check

# A compilação é retomável. EE_BUILD_JOBS=2 é conservador para celular.
# Se a versão anterior parou no config.guess, não apague build/: este comando
# aplica a correção AArch64 e retoma exatamente o estágio que falhou.
make bootstrap-ee-stage1 EE_BUILD_JOBS=2
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make toolchain-probe EE_CC="$EE_CC"
make -B build/matching/get_tree/get_tree.o EE_CC="$EE_CC"
make -B build/matching/mathfp/mathfp.o EE_CC="$EE_CC"
make match-mathfp-listing EE_CC="$EE_CC"

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
  docs/ROADMAP.md \
  docs/SOURCE_COMPLETENESS.generated.md \
  matching/candidates/mathfp.c \
  matching/candidates/mathfp_numtest.c \
  tools/bootstrap_ee_gcc_stage1.py \
  tools/objdump_listing_to_binary.py \
  tools/patches/gcc-3.2.2-aarch64-host.patch \
  tools/patches/gcc-3.2.2-modern-host.patch \
  tools/patches/gnu-config-aarch64.patch \
  tools/test_bootstrap_ee_gcc_stage1.py \
  tools/test_objdump_listing_to_binary.py

git diff --cached --check
git diff --cached --stat
git commit -m "Match first EE mathfp leaves and enable ARM64 bootstrap"
git push origin HEAD:main
```

O comando `make match-mathfp-listing` usa os bytes do disassembly já
versionado e não precisa do executável original. A verificação formal contra o
ELF completo continua opcional e exige uma cópia legal em
`original/SNES_EMU.ELF`:

```bash
make reference
make match-mathfp EE_CC="$EE_CC"
```
