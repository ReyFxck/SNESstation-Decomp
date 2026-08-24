# Isolated matching candidates

This directory contains compiler experiments and exact gate candidates. It is
not the canonical recovered program source tree.

- `candidates/` holds focused C or assembly inputs used to reproduce a specific
  target corridor.
- `ee_abi_compat/` supplies minimal historical EE ABI headers for isolated
  compilation.

Readable behavioral reconstruction belongs under `src/`. Exact assembly used
only to prove bytes must stay clearly labelled as reconstruction evidence.
