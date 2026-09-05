# V105 historical C++ tail integration

V105 closes four more whole-image oracle windows by rebuilding data that the
historical EE C++ toolchain placed around the application and Snes9x objects.
The result remains a diagnostic executable, not a replacement ELF.

## Frozen base and result

- Exact base: V104 commit
  `b551153b48fa0d3acf109a0ba0e7c14eefcbeccd`.
- Historical compiler contract: EE GCC 3.2.2, target `ee`.
- Pinned Snes9x 1.41-1 source archive SHA-256:
  `5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b`.
- ELF entry remains exact at `0x00100008`.
- Exact oracle windows advance from 12 in V104 to **16/51**.
- Differing windows fall from 39 to **35**.
- Differing bytes fall from 1,884,142 to **1,859,772**.
- The first difference remains `0x00100114`.
- Padded diagnostic SHA-256:
  `f1b99419c6a4c433b6e3564646aa2a210e6ed64d270f6fb4461c0b241546f0d4`.
- Target unpacked SHA-256 remains
  `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`.

The exact window roster is 12–14 and 37–49. V105 newly closes window 14 and
windows 47–49; a 64 KiB window is a comparison region, not a function count.

## Stage 3H: frontend unwind metadata

The historical compiler emitted writable GCC C++ exception metadata between
the frontend code and embedded assets. V105 reconstructs three CIE/FDE groups
from explicit DWARF semantics:

| Address | Size | FDE relocations |
|---:|---:|---:|
| `0x001ebaf0` | 544 | 13 |
| `0x001ebd60` | 140 | 4 |
| `0x001ec1cc` | 260 | 6 |

Together these groups cover **18 FDEs, 944 exact bytes and 23 relocations**.
Function start/size inputs come from the frozen 1,041/1,041 function ledger.
[`matching/candidates/stage3h_frontend_eh_frames.S`](../../matching/candidates/stage3h_frontend_eh_frames.S)
contains directives for the CFI operations; it contains no extracted target
payload and no `.incbin` directive.

The public, byte-free contract is
[`analysis/link_identity/frontend_eh_frames.json`](../../analysis/link_identity/frontend_eh_frames.json).

## Stage 3I: rebuilt source data and CFI

Six complete or bounded public-source sections provide **123,140 bytes**:

| Provider | Target start | Bytes | `R_MIPS_32` relocations |
|---|---:|---:|---:|
| `ppu.cpp` | `0x003f4bf0` | 648 | 14 |
| `SA1CPU.CPP` | `0x003f5040` | 21,872 | 1,411 |
| `snaporig.cpp` | `0x003fa6c0` | 72,072 | 4 |
| `SNAPSHOT.CPP` prefix | `0x0040c048` | 19,940 | 11 |
| `SOUNDUX.CPP` prefix | `0x00410f00` | 200 | 4 |
| `SPC700.CPP` suffix | `0x00411410` | 8,408 | 179 |

Every non-relocation byte is checked against the private reference. The
reference is then used only as an oracle for the final values of the 1,623
`R_MIPS_32` words. Generated provider payloads live below ignored `build/`;
none are committed.

The source rebuild applies one explicit target-ABI layout correction:
Snes9x's `time_t last_used` is compiled as a 32-bit field. The target's
independently observed field offset is `0x14`; the modern host definition would
move it to `0x18`. This patch is recorded in the manifest and applied only to
the temporary extracted source.

Nine additional semantic groups cover **1,708 bytes**, including **30 FDEs and
36 link relocations**. They are emitted from CFI operations and one typed
`uint16 cacheMegs = 5` source identity, not copied from the target.

Ten smaller V98 fixed sections are absorbed by the larger exact providers.
The linker retains all **157 global names** from those sections as absolute
aliases at their already-proved addresses, so no caller loses ownership and no
duplicate storage is emitted.

The frozen public contract is
[`analysis/link_identity/historical_tail_data.json`](../../analysis/link_identity/historical_tail_data.json).

## Reproduce

Repository-only validation requires neither original image nor EE compiler:

```bash
make frontend-eh-frames-public-check
make historical-tail-data-public-check
```

With the legally obtained reference and both historical compiler drivers:

```bash
make data-backing-check \
  EE_CC=build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc \
  EE_STAGE1_CXX=build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++

make startup-integration-check \
  EE_CC=build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc

make frontend-eh-frames-check \
  EE_CC=build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc

make historical-tail-data-check \
  EE_STAGE1_CXX=build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++
```

The private gates rebuild, relocate and link every new range, verify each
range byte-for-byte, compare all 51 windows and then require the result to
equal the frozen manifests.

## Remaining boundary

V105 does not prove historical whole-archive composition, complete application
implementation selection, all object/array bounds, final relocation/string
pooling order or SJCRUNCH2/LZO packing. The next useful targets are the early
application text/data windows 0–11, the middle windows 15–36 and the final
window 50. Only an unpacked hash match can open the packing gate.
