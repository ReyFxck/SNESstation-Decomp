# Object-native linkage

The exact-image pipeline and the clean object link are tracked separately.

The current Stage-3P gate proves all **720,620/720,620 code bytes** and the
complete replacement ELF. It rebuilds and checks 97 candidate ELF objects, but
then selects verified slices and transports the completed code windows through
one generated `code-windows.o` whose sections use `.incbin`. Those candidate
objects are therefore evidence providers, not the final code inputs to
`ee-ld`.

For the strict project definition, completion requires each selected code range
to be linked from the ELF object that produced it. A consolidated binary payload
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
| Bytes linked directly from selected producer objects | **0/720,620** |
| Strict object-native linkage | **In progress** |

The construction buckets sum to the exact 720,620-byte code region. They
describe provenance used to assemble the exact image; they do not imply that the
corresponding producer object is already a direct linker input.

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
