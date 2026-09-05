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
  gate, while leaving all 90 source/historical units incomplete because the
  final linked implementation selection is still open.
- `whole_image_identity` counts only the 12 completely exact 64-KiB windows
  from the V102 diagnostic link. A partly equal window contributes zero exact
  bytes, and the report does not claim either target hash.

Generation uses only committed manifests. It does not read or publish the
private original ELF, the privately reconstructed unpacked image, extracted
assets, toolchain paths or host-specific absolute paths.
