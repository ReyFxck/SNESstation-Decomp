# Object-native linkage

The exact-image pipeline and the clean object link are tracked separately.

Stage-3P proves all **720,620/720,620 code bytes** and the complete
replacement ELF. Twenty-eight ELF inputs provide **445,612 code bytes** directly
through 161 placed code sections. Twenty-three are historical candidate objects,
including `2XSAI.o`, `fxemu.o`, `fxinst.o`, `SA1CPU.o`, `ppu-short.o`,
`CPUEXEC.o`, `dma.o`, `gfx-short.o`, `CPUOPS.o` and eight original runtime objects.
Where exact, original data and read-only sections replace constructed providers
at their historical addresses. The `dma.o` read-only dispatch table is linked
from the object; its 260-byte data section has one nonrelocation byte mismatch,
so the proved semantic data provider remains. The other 74 candidate objects
remain evidence sources. `CPUOPS.o` supplies its original code and the exact
20,876-byte data prefix; its 172-byte CFI tail differs outside relocations and
continues to use the proved semantic provider.

The remaining **275,008 code bytes** pass through generated `.incbin` sections
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
| Bytes linked directly from producer objects | **445,612/720,620 (61.84%)** |
| Direct ELF object inputs | **28** (including **23** historical candidate objects) |
| Direct placed sections | **161** |
| Bytes remaining behind generated payload | **275,008** |
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
