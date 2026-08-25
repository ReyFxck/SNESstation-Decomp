# V82 unpacked-layout oracle

V82 closes the first measurable Stage-3 subgate: the private unpacked image now
has a deterministic, public, byte-free layout oracle and rebuilt candidates can
be stopped at their exact first divergent byte.

This does **not** claim that program data, archives, relocations or the final
link already match. It supplies the measurement boundary needed to close those
items in small, independently checked batches.

## Frozen target geometry

| Measurement | Result |
|---|---:|
| Packed reference | 726,968 bytes · SHA-256 `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| SJCRUNCH2 header | packed offset `0x00002f00` |
| Entry | `0x00100008` |
| Initialized load range | `0x00100000..0x00426de8` · 3,304,936 bytes (`0x326de8`) |
| Zero-fill/BSS tail | `0x00426de8..0x00450c18` · 171,568 bytes (`0x29e30`) |
| Unpacked reference | SHA-256 `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |
| SJCRUNCH2 sections | 1 |
| SJCRUNCH2 blocks | 13: twelve `0x40000` blocks plus one `0x26de8` tail |
| Comparison windows | 51: fifty 64 KiB windows plus one `0x6de8` tail |

The sole section declares 714,132 compressed data bytes. Its block stream spans
714,236 bytes including the thirteen 8-byte block headers, followed by four
zero alignment bytes. The public container boundary is packed offset
`0x000b151c`; 668 outer-ELF bytes follow it.

Every section, decompressed block and 64 KiB window has its own SHA-256 in
[`analysis/link_identity/unpacked_layout.json`](../../analysis/link_identity/unpacked_layout.json).
That lets later batches prove progress by address range instead of repeatedly
scanning an unstructured 3.3 MiB image.

## Gates and commands

Repository-only schema and coverage validation:

```bash
make layout-oracle-public-check
```

Private verification from `original/SNES_EMU.ELF`:

```bash
make layout-oracle
```

Compare a rebuilt unpacked image:

```bash
make compare-unpacked CANDIDATE_RAW=/absolute/path/to/rebuilt.bin
```

An exact candidate exits successfully. A mismatch exits nonzero after writing
`build/layout-oracle/comparison.json` with:

- matching prefix and suffix sizes;
- first divergent image offset and virtual address;
- expected/candidate byte at that private local comparison point;
- differing-byte count;
- every mismatching 64 KiB window and its hashes.

Refreshing the frozen public oracle is a separate reviewed operation:

```bash
make layout-oracle-refresh
```

## Privacy boundary

The committed manifest contains integers, layout relationships and SHA-256
values only. The original ELF, unpacked image, comparison candidates and local
reports remain below ignored `original/` or `build/` paths. No executable byte
stream, asset, Base64 payload or machine-local path is committed.

## Honest next gate

V82 measures link progress but creates no replacement bytes. The next small
batch is resolving the 337 source-address aliases already anchored by the
closed function/source gates; program data, runtime archives, final object
order and the unpacked-image hash remain open.
