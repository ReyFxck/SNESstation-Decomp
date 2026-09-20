# Object-native linkage

The exact-image pipeline and the clean object link are tracked separately.

Stage-3P proves all **720,620/720,620 code bytes** and the complete
replacement ELF. Sixty ELF inputs provide **632,936 code bytes** directly
through 202 placed code sections. Fifty-five inputs come from historical
candidate objects (the same `pgen-gzio.o` contributes two separate sections),
including `2XSAI.o`, `fxemu.o`, `fxinst.o`, `SA1CPU.o`, `ppu-short.o`,
`CPU.o`, `seta.o`, `CPUEXEC.o`, `dma.o`, `gfx-short.o`, `CPUOPS.o`, `tile.o`,
`apu-short.o`, `c4.o`, `obc1.o`, `snes-sa1.o`, `snes-CHEATS.o`,
`deflate-1.41.o`, `unzip.o`, `xprintf.o`, `libmc.o`, `unwind-dw2-fde.o`,
`snaporig-short.o`, `memmap-short.o`, `DSP1.o`, `spc7110.o`,
`snapshot-short.o`, `sound-normal.o`, `gsFont.o` and eight original runtime objects.
Where exact, original data and read-only sections replace constructed providers
at their historical addresses. The `dma.o` read-only dispatch table is linked
from the object; its 260-byte data section has one nonrelocation byte mismatch,
so the proved semantic data provider remains. The other 43 candidate objects
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
target code bytes or committing private reference data. `c4.o` contributes its
complete 2,652-byte code section, `obc1.o` its complete 1,276-byte code
section and `snes-sa1.o` its complete 6,364-byte code section, with their
relocations checked against proved data providers. The final 108 bytes in
`snes-sa1.o` also match an earlier CPUOPS candidate slice. `snes-CHEATS.o`
supplies the verified 23,936-byte suffix of its code section. Its first 1,024
bytes differ outside relocations, so the derived ELF excludes that prefix;
the linker still receives the original producer's proven suffix and only its
15 selected relocations. Six further candidate objects contribute their
complete historical code sections: `xprintf.o` (2,840 bytes),
`unwind-dw2-fde.o` (6,336 bytes), `deflate-1.41.o` (7,792 bytes), `unzip.o`
(5,872 bytes), `libmc.o` (5,044 bytes) and `snaporig-short.o` (4,000 bytes).
Their original data and read-only section references resolve to existing
proved providers. Eleven old residual assembly sections (496 bytes) are
covered by the verified `unwind-dw2-fde.o` section and removed from the
derived residual linker input. That earlier batch increased strict linkage by
31,388 bytes.

Eighteen further historical ELF inputs add **70,300 directly linked code
bytes**. The largest comes from a 29,040-byte internal corridor of
`memmap-short.o`; `DSP1.o`, `spc7110.o`, `snapshot-short.o`, `pgen-gzio.o` and
`sound-normal.o` provide more exact corridors. Smaller verified sections come
from `snes-srtc.o`, `pgen-unshrink.o`, `pgen-inffast.o`, `libmtap.o`,
`malloc.o`, `gsFont.o`, `loadzip.o`, `sjpcm.o`, `explode.o`, `seta010.o` and
`cheats2.o`; `pgen-gzio.o` also supplies a separate 548-byte suffix. Derived
objects trim unproved prefixes and tails and redirect their original
relocations to proved addresses. The private reference verifies relocation
results and whole-image identity; its instruction payload is not committed.

The remaining **87,684 code bytes** pass through generated `.incbin` sections
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
| Bytes linked directly from producer objects | **632,936/720,620 (87.83%)** |
| Direct ELF object inputs | **60** (including **55** historical candidate inputs) |
| Direct placed sections | **202** |
| Bytes remaining behind generated payload | **87,684** |
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
