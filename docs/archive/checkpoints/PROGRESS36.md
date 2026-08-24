# Progress 36 — correct the EE int128 compatibility probe

Progress 35 accidentally redeclared GCC's internal `__uint128_t` and
`__int128_t` names in the scan-only `stdint.h` shim. The historical EE
compiler immediately proved that those type names already exist: 94
translation units failed with `conflicting types for __uint128_t`.

The real compatibility gap is narrower. GCC 3.2.2 accepts the internal
128-bit type name used by the Progress-28 structural lift, but predates the
modern `__SIZEOF_INT128__` predefined macro used as its feature test.

Progress 36 therefore removes both typedefs and supplies only the missing
`__SIZEOF_INT128__` feature macro. Matching flags remain unchanged.
