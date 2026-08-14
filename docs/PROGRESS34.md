# Progress 34 — second EE stage-one compatibility batch

Progress 33 moved the historical EE scan from 6/101 to 69/101 passing
translation units while preserving the 7/7 local libgcc-unwind matching gate.

The remaining diagnostics exposed two environment/compiler-era classes:

1. the header-less stage-one compiler still lacks libc interfaces used by the
   source models (`stdio.h`, `stdlib.h`, `errno.h`, `math.h`);
2. GCC 3.2.2 predates C11 `_Static_assert`, while the recovered source uses
   static assertions to prove target layouts during modern host checks.

This batch extends only `include/ee_stage1_compat`. The `_Static_assert`
bridge is intentionally a no-op in the historical scan because `make check`
continues to compile the same sources as C11 on the host and therefore still
evaluates the real assertions.

It also adds `UINT64_MAX` and the stdio seek constants observed in the scan.

No matching `EE_CFLAGS` are changed. The 7/7 unwind object remains isolated
from these scan-only headers.
