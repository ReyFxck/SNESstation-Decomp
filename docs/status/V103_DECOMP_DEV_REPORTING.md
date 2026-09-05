# V103 — public decomp.dev reporting

V103 adds a GitHub Actions report that decomp.dev can discover without putting
the original SNES Station executable or any extracted private payload in the
repository. The base for this checkpoint is `ccf94f6b3c17b64d70b761883b43cbaf18605c2c`
(V102).

## Why registration previously failed

decomp.dev registers a repository from a completed GitHub Actions **push** run
on its default branch. It searches that run for an artifact whose name follows
the `<version>_report` convention and then parses an Objdiff Report v2 inside
it. Before V103 the repository did not publish such an artifact, so the form
correctly displayed `No workflow runs containing reports found.`

The new `.github/workflows/decomp-dev.yml` runs after a push to `main`, creates
`build/decompdev/report.json` and uploads it as `SNES_EMU_report`. The report is
also reproducible locally:

```sh
make decompdev-report
```

## Reported proof levels

| Category | Proven progress | Completion deliberately withheld |
|---|---:|---:|
| Function matching | 1,041/1,041 functions; 722,892 address-span bytes | 0/90 linked source/historical units |
| Whole-image identity | 12/51 fully exact 64-KiB windows; 786,432 exact-window bytes | final unpacked hash and replacement ELF |

The V102 diagnostic has 1,421,069 equal bytes in total, but many of those are
inside windows that also differ. V103 reports the more conservative 786,432
bytes belonging to the 12 wholly exact windows. This prevents partial equality
from being presented as completed image regions.

The report generator reads only committed public manifests. Its frozen privacy
contract rejects binary payloads, private-reference requirements and absolute
local paths. The packed and unpacked target fingerprints remain documentation
and verification oracles; neither private binary is uploaded to GitHub Actions
or decomp.dev.

## Registration order

1. Apply V103, commit it and push it to `main`.
2. Open the repository's **Actions** tab and wait for **decomp.dev progress
   report** to finish with a green check.
3. Return to <https://decomp.dev/manage/new> and reload the page.
4. Select `ReyFxck/SNESstation-Decomp`, enter `SNES Station v0.23` as the game
   name, `SNES Station` as the short name and `PlayStation 2` as the platform.
5. Press **Add**. The repository is now discoverable because a completed push
   run contains the expected report artifact.

If the workflow is still running, or failed, the same red registration message
is expected. Open the run first; do not upload the original ELF to fix it.

The report format follows the
[decomp.dev project guide](https://decomp.wiki/tools/decomp-dev) and Objdiff
Report v2. This checkpoint improves public observability only. The remaining
core work is still exact startup/object selection, complete layout and
relocation identity, the unpacked hash, and finally SJCRUNCH2/LZO packing and
the packed hash.
