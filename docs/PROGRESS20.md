# Progress 20 - typed promotions and GS frame-present helper

This checkpoint fixes the source-completeness accounting so a historical
Progress-16/17 pseudocode snapshot can remain immutable while a later typed
behavioral reconstruction is promoted out of the pseudocode-only backlog.

`analysis/source_promotions.csv` is now the explicit promotion registry. The
audit validates that each promoted address belongs to a historical P16/P17
checkpoint, that its typed source and evidence files exist, and that the source
file carries the address token used for traceability.

The first registry contains the four already recovered frontend/path entries
`0x001057fc`, `0x00105898`, `0x001059cc`, and `0x00105ae8`, plus one new small
Progress-16 recovery:

- `0x00101e8c`: flush the current GS pipe, acknowledge/wait for GS CSR bit 3
  (`VSINT` in the target flow), then call `gsDriver_swapBuffers_001991b0`.
  The typed reconstruction follows the committed R5900 pseudocode and the
  recovered GSLIB method map.

After `make audit-source`, the expected readiness accounting is:

- structural representation: 1,041 / 1,041;
- behavioral/source model: 807 / 1,041 (77.52%);
- pseudocode-only backlog: 234 / 1,041 (22.48%);
- remaining Progress-16 pseudocode: 160;
- remaining Progress-17 pseudocode: 74;
- historical snapshots retained: 165 P16 and 74 P17 entries;
- relocation-normalized matching remains 7 / 1,041.

No new machine-code matching claim is made by this checkpoint.
