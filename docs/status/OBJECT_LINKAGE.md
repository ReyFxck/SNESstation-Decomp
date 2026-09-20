# Object-native linkage

The exact-image pipeline and the clean object link are tracked separately.

Stage-3P proves all **720,620/720,620 code bytes** and the complete
replacement ELF. Thirty-two ELF inputs provide **497,020 code bytes** directly
through 185 placed code sections. Twenty-seven are historical candidate objects,
including `2XSAI.o`, `fxemu.o`, `fxinst.o`, `SA1CPU.o`, `ppu-short.o`,
`CPU.o`, `seta.o`, `CPUEXEC.o`, `dma.o`, `gfx-short.o`, `CPUOPS.o`, `tile.o`,
`apu-short.o` and eight original runtime objects.
Where exact, original data and read-only sections replace constructed providers
at their historical addresses. The `dma.o` read-only dispatch table is linked
from the object; its 260-byte data section has one nonrelocation byte mismatch,
so the proved semantic data provider remains. The other 70 candidate objects
remain evidence sources. `CPUOPS.o` supplies its original code and the exact
20,876-byte data prefix; its 172-byte CFI tail differs outside relocations and
continues to use the proved semantic provider. `tile.o` contributes 33,568 bytes
of main code, 20 selected inline renderer sections and its original 2,220-byte
unwind data. Four additional inline renderer definitions lie outside the selected
code region; their calls resolve to independently verified target addresses.
`CPU.o` contributes its complete 956-byte code section; its original unwind
container stays with the existing proved semantic provider. `seta.o` adds its
complete 72-byte code section; its two local data references bind to proved
addresses, leaving the existing data provider in place. `apu-short.o` adds its
complete 4,196-byte code section; original data and read-only references bind
to proved addresses while the existing semantic data providers remain. Its
derived object's eight BSS relocation addends are adjusted to the original
address (the candidate's local BSS starts one byte later), without changing
target code bytes or committing private reference data.

The remaining **223,600 code bytes** pass through generated `.incbin` sections
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
| Bytes linked directly from producer objects | **497,020/720,620 (68.97%)** |
| Direct ELF object inputs | **32** (including **27** historical candidate objects) |
| Direct placed sections | **185** |
| Bytes remaining behind generated payload | **223,600** |
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
