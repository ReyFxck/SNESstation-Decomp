# Progress 45 — Exact historical libcdvd source/header fingerprint

Progress 44 showed that O3 is not the missing key.  The best O2+builtins and O3
profiles both produced zero exact matches and the same total size delta.

This progress removes another approximation from the experiment.

The previous candidate used a padded stand-in type whose only purpose was to
place `SifRpcClientData_t.server` at +0x24.  A surviving historical SIF RPC
header confirms the complete client layout and the same +0x24 server offset.
The candidate now uses that historical field structure, exact global names and
declaration order, and the original function names/source expression shapes.

The source candidate is also separated from the recovered project source:

- `matching/candidates/cdvd_rpc.c`
- `matching/ee_abi_compat/cdvd_legacy_compat.h`

The manifest points at the exact historical symbols.

The frontier runner now ranks candidates by relocation-normalized differing
bytes, not only by function size.  This makes compiler fingerprint experiments
meaningful even while the exact-match count remains 0/8.

It tests:

- strict/default/no strict-aliasing;
- first and second instruction schedulers independently;
- delayed-branch scheduling;
- several individual O2 passes;
- O1/O3 controls;
- the surviving SNESticle freestanding profile.

No profile is promoted merely for winning the matrix.  Only the strict byte
gate can close the corridor.
