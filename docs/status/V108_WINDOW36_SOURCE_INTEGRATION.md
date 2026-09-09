# V108 — Window 36 source integration

Stage 3L closes image window 36 with reproducible historical source and
explicit unwind semantics. It does not store the private ELF or claim a
replacement executable.

## Integrated containers

- `DSP1.CPP`: 18,412-byte initialized prefix, including the two indirect DSP
  entry pointers.
- `CPUOPS.CPP`: four typed 256-entry opcode tables (4,096 bytes).
- `fxemu.cpp`: complete 2,280-byte `.data` section.
- `fxinst.cpp`: complete 6,672-byte `.data` section.
- GCC 3.2.2 CFI: 17 DSP1 and 10 renderer FDEs, reconstructed from explicit
  DWARF operations and linked function extents.

The four source ranges total 31,460 bytes and contain 2,083 `R_MIPS_32`
relocations. Private target data may resolve only those relocation-controlled
words; every byte outside the relocation masks must already match the rebuilt
historical object. The two indirect-only DSP functions outside the JAL-derived
manifest are separately normalized against their complete 4,084-byte and
356-byte historical source symbols.

Forty-three older fixed slices are absorbed by the larger source containers,
while their exported names remain absolute aliases. The resulting diagnostic
has 18/51 exact windows, 33 mismatching windows, and 1,845,637 differing bytes.

## Verification

Public contract only:

```sh
make window36-data-public-check
```

Historical rebuild plus private byte gate:

```sh
make window36-data
```

The result remains a linked diagnostic. Exact implementation selection for the
other 33 windows and reproduction of the packed ELF are still open.
