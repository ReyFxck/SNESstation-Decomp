# Object-native linkage

The exact-image pipeline and the clean object link are tracked separately.

Stage-3P proves all **720,620/720,620 code bytes** and the complete
replacement ELF. Twenty-seven ELF inputs provide **366,536 code bytes** directly
through 159 placed code sections. Twenty-two are historical candidate objects,
including `2XSAI.o`, `fxemu.o`, `fxinst.o`, `SA1CPU.o`, `ppu-short.o`,
`CPUEXEC.o`, `dma.o`, `gfx-short.o` and eight original runtime objects.
Where exact, original data and read-only sections replace constructed providers
at their historical addresses. The `dma.o` read-only dispatch table is linked
from the object; its 260-byte data section has one nonrelocation byte mismatch,
so the proved semantic data provider remains. The other 75 candidate objects
remain evidence sources.

The remaining **354,084 code bytes** pass through generated `.incbin` sections
in `code-windows.o`. Those bytes match the target but still need to be linked
from their producer objects.

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
| Bytes linked directly from producer objects | **366,536/720,620 (50.86%)** |
| Direct ELF object inputs | **27** (including **22** historical candidate objects) |
| Direct placed sections | **159** |
| Bytes remaining behind generated payload | **354,084** |
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
