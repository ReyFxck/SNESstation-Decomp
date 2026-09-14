# decomp.dev public report contract

This directory freezes the public inputs and claims used to generate the
Objdiff Report v2 uploaded for decomp.dev. The report is generated with:

```sh
make decompdev-report
```

The output is `build/decompdev/report.json`; it is a generated CI artifact and
is not committed. `report_contract.json` hashes every public manifest consumed
by the generator so a changed project checkpoint cannot silently retain stale
progress figures.

The report deliberately keeps two proof levels separate:

- `function_matching` records the frozen 1,041/1,041 per-function matching
  gate and marks all 90 source/historical code units complete. Every reported
  function lies in the exact linked image and the replacement-ELF gate is closed.
- `whole_image_identity` counts all 51 completely exact 64 KiB windows.
  A partly equal window would contribute zero exact bytes and fail the frozen
  completion contract.

Generation uses only committed manifests. The decomp.dev “fully linked”
percentage is therefore derived from public completion evidence, not forced by
a CI-only override. The report does not read or publish the private original
ELF, the privately reconstructed unpacked image, extracted assets, toolchain
paths or host-specific absolute paths.
