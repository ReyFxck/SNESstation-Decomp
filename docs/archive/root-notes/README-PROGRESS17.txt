SNES Station Decomp — Progress 17 overlay

Base:
  main at commit 74ad6e0
  reconstructed=967/1137 = 85.05% on the former raw-JAL proxy
  mapped=992/1137 = 87.25% on the former raw-JAL proxy

After tools/apply_progress17.py:
  validated universe=1137 raw JAL - 292 data patterns + 196 non-JAL = 1041
  reconstructed=1041/1041 = 100.00%
  mapped=1041/1041 = 100.00%
  matching=0

Apply from the repository root:
  python3 -m zipfile -e /storage/emulated/0/Download/SNESstation-Decomp-Progress17-100-overlay.zip .
  python3 tools/apply_progress17.py

Then inspect and publish:
  git status --short
  git diff --check
  git add -A
  git commit -m "Reach 100 percent audited structural reconstruction"
  git push origin HEAD:main

Progress 17 adds 49 real code targets, promotes 25 mapped targets, and records
all 74 R5900 structural decompiles with pinned hashes and warning evidence.
The 100% figure is audited structural coverage, not compiler matching; matching
remains 0.00%.
