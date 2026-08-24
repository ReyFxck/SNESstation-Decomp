SNES Station Decomp — Progress 16 overlay

Base:
  main at commit 731e40f
  reconstructed=802/1137 = 70.54%
  mapped=827/1137 = 72.74%

After tools/apply_progress16.py:
  reconstructed=967/1137 = 85.05%
  mapped=992/1137 = 87.25%
  matching=0

Apply from the repository root:
  python3 -m zipfile -e /storage/emulated/0/Download/SNESstation-Decomp-Progress16-85.05-overlay.zip .
  python3 tools/apply_progress16.py

Then inspect and publish:
  git status --short
  git diff --check
  git add -A
  git commit -m "Reach 85.05 percent reconstructed decompilation"
  git push origin HEAD:main

Progress 16 adds 165 address-labelled R5900 structural decompiles. It does not
claim compiler matching; matching remains 0.00%.
