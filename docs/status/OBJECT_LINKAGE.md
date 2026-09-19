# Object-native linkage

The exact-image pipeline and the clean object link are tracked separately.

Stage-3P currently proves all **720,620/720,620 code bytes** and the complete
replacement ELF. Six ELF object inputs contribute **123,132 bytes** directly
through 137 explicitly placed sections: the split residual assembly, exact
V80, V81 and C4 proof sources, a compiled ROM cleanup function, and the
historical PS2SDK `SyncDCache.o`. The remaining **597,488 bytes** still pass
through the generated `code-windows.o` transport and its `.incbin` sections.
The 97 rebuilt candidate objects form the evidence inventory for the byte
matches. One whole historical candidate object, `SyncDCache.o`, is now also a
final link input; the other candidates remain proof sources.

For the strict project definition, completion requires every selected code range
to be linked from an ELF object that produced it. A consolidated binary payload
does not count, even when its final bytes are exact.

## Current baseline

| Measure | Result |
|---|---:|
| Exact code-window bytes | **720,620/720,620** |
| Candidate ELF objects rebuilt and checked | **97** |
| Candidate-object slices selected | **432** |
| Candidate-object evidence bytes | **611,088** |
| Historical/recovered object construction bucket | **525,460 bytes** |
| Earlier exact-assembly construction bucket | **85,628 bytes** |
| Explicit residual assembly | **36,340 bytes** |
| Public listing construction bucket | **73,192 bytes** |
| Bytes linked directly from producer objects | **123,132/720,620** |
| Direct ELF object inputs | **6** (including **1** historical candidate object) |
| Direct placed sections | **137** |
| Bytes remaining behind generated payload | **597,488** |
| Strict object-native linkage | **In progress** |

The provenance buckets sum to the exact 720,620-byte code region. The direct
object and generated-payload rows describe how that proven region is transported
into the final link, so they intentionally overlap those provenance buckets.

## Gates

```bash
make object-linkage-status
make object-linkage-public-check
make object-linkage-required
```

The first two commands validate and display the frozen public baseline.
`object-linkage-required` is intentionally red until all 720,620 bytes use
direct object inputs and the generated `.incbin` transport is gone.

Private byte comparison remains in `make reproduce-check`. The object-linkage
manifest contains counts and paths only; it does not contain original target
bytes.
