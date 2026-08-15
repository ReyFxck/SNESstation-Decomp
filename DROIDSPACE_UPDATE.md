# Progress 52

Apply directly over either clean main or the already-applied/uncommitted Progress 51 tree:
```bash
cd ~/SNESstation-Decomp
rm -rf /tmp/SNESstation-Decomp-Progress52
unzip -q /storage/emulated/0/Download/SNESstation-Decomp-Progress52.zip -d /tmp
bash /tmp/SNESstation-Decomp-Progress52/apply-progress52.sh "$PWD"
```

Commit/push after all strict gates pass:
```bash
cd ~/SNESstation-Decomp
git diff --check
git status
git add -A
git diff --cached --check
git commit -m "Match historical old EE libkernel and libc batch"
git push origin main
```
