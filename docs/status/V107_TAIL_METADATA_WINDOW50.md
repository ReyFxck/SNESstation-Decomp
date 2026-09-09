# V107 — final tail metadata closes image window 50

Stage 3K rebuilds the remaining public-source and semantic metadata at the end
of the initialized image. The integrated diagnostic now matches image window
50 byte for byte. This is a link-progress checkpoint, not a replacement ELF.

## Reproducible result

| Metric | Result |
|---|---:|
| Public-source sections | 34 |
| Public-source bytes | 9,428 |
| Reapplied source relocations | 151 |
| Semantic sections | 5 |
| Semantic bytes | 944 |
| SPC7110 FDEs | 14 |
| Window 50 differing bytes | **0** |
| Exact image windows | **17/51** |
| Remaining differing bytes | **1,854,246** |

The new source containers cover Snes9x S-RTC, unzip/zlib tables, PS2LIB data,
GCC 3.2.2 `libgcc` unwind data and `libsupc++` RTTI/vtables. The semantic
containers describe SPC7110 unwind records, runtime LSDA, IniFile RTTI, the
historical `sbrk` initial break and the three selected Newlib MathFP globals.
Three superseded fixed backing fragments are absorbed by the stronger source
or semantic owners.

Only public source, hashes, addresses, relocation counts and semantic recipes
are committed. Final relocation values are reconstructed under ignored
`build/` storage and checked against the legally obtained private reference;
no target payload is stored in the manifest.

## Verify

The public contract needs no private executable:

```bash
make tail-metadata-public-check
```

With `original/SNES_EMU.ELF` available, rebuild and compare the integrated
diagnostic with historical EE GCC 3.2.2:

```bash
make tail-metadata
```

The remaining 34 image windows concern exact implementation-object selection,
placement and historical link composition. Packed and unpacked whole-image
hashes therefore remain explicitly open.
