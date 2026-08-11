# Dependency and toolchain version ledger

This ledger records every version-bearing component currently proven in the
SNES Station v0.23 target, plus every important component whose exact revision
is still unknown.  An explicit **unknown** is intentional: it prevents a future
matching attempt from silently substituting a modern library.

## Evidence levels

| Level | Meaning |
|---|---|
| Exact in target | A version/date string or immutable blob hash is present in the reference binary. |
| Strong fingerprint | Constants, object boundaries, public-symbol offsets and control flow identify a historical source/runtime revision, but no target version string exists. |
| Family only | The ABI/RPC protocol and implementation generation are known; the precise commit or archive revision is not. |
| Unknown | The target proves the component is present, but exposes no reliable semantic version. |

## Program and emulation core

| Component | Version / revision | Evidence | Matching implication |
|---|---|---|---|
| SNES Station | `0.23 WIP`, 24 January 2004 | Exact in target and official README | Freeze all work to the packed ELF hash below; later `0.2.4/0.2.5/0.2.6` mods are different targets. |
| Snes9x | `1.41` (not `1.41-1`) | Exact `Snes9x` + `1.41` strings; renderer, DSP1 and `CMemory` ordering agree with the 1.41 archive | Use the 1.41 source archive as the baseline. The `1.41-1` archive identifies itself as `1.41-1` and changes `CPUEXEC.CPP`, so it is not interchangeable. |
| zlib | `1.1.3` | Exact `1.1.3` and `deflate 1.1.3 Copyright 1995-1998 Jean-loup Gailly` strings | Use historical zlib 1.1.3, while retaining the target's PS2 file-I/O adaptations. |
| Gilles Vollant unzip API | `0.15` (1998) | Exact `unzip 0.15 Copyright 1998 Gilles Vollant` target string | Do not replace with a newer minizip API/layout. The target also keeps Shrink, Reduce and Implode support. |

## PS2 and embedded IOP components

| Component | Version / revision | Evidence | Matching implication |
|---|---|---|---|
| Hiryu CDVD filesystem IOP module | `1.13` | Exact embedded string `CDVD: CDVD Filesystem v1.13`; carved IRX SHA-256 below | Pin the IOP blob and the corresponding early Hiryu/Sjeep EE RPC command set, including command 8. |
| SjPCM IOP module | `2.0` | Exact embedded string `SjPCM v2.0 - by Sjeep`; carved IRX SHA-256 below | Use the v2.0 RPC interface and its historical `libsd` path. |
| AmigaMod IOP module | version unknown; build credits `Vzzrzzn` | Exact `AmigaMod: by Vzzrzzn` string, intact symbols/`.mdebug`, and carved IRX SHA-256 | Treat the embedded blob as the version identity. Do not assign a guessed release number. |
| Hiryu `gsLib` | unversioned early snapshot | Target class sizes, method order and historical quirks; official README credit | Match against the early Hiryu implementation represented by the target, not a later gsKit replacement. |
| EE `libcdvd` client | early Hiryu/Sjeep family paired with CDVD FS 1.13 | RPC IDs, commands and target control flow | Exact source revision remains unpinned even though the embedded server version is exact. |
| `libpad` | NEW/XPADMAN generation; exact revision unknown | Binds `0x80000100/0x80000101`, uses commands `0x06..0x12`, loads `rom0:XPADMAN` | The module itself comes from the console ROM, so its version can vary by PS2 model. Match the client ABI, not a modern PS2SDK library. |
| `libmtap` | NEW/XMTAPMAN generation; exact revision unknown | Loads `rom0:XMTAPMAN` and uses the matching RPC family | Keep the target RPC layout and polling behavior. |
| `libmc` | old XMCMAN/XMCSERV client; exact revision unknown | Loads `rom0:XMCMAN`/`rom0:XMCSERV`; client predates the later reboot-count prelude | Later PS2SDK `libmc` is useful only as a structural reference. |
| `libkernel`, SIF RPC/CMD, FileIO and loadfile | old PS2DEV/PS2LIB family; exact revision unknown | Archive boundaries, 32-bit structure geometry and old RPC behavior | Reproduce the old PS2LIB corridor; current PS2SDK is not a matching substitute. |
| `rom0:XSIO2MAN`, `XPADMAN`, `XMTAPMAN`, `XMCMAN`, `XMCSERV`, `LIBSD` | console-ROM supplied; no fixed target version | Exact module names in startup code | Record the test console/ROM revision when hardware matching begins. |
| PS2 kernel/client base | Gustavo Scotti-era PS2DEV lineage; exact revision unknown | Official SNES Station README credit and recovered old libkernel ABI | Keep this as provenance, not a compiler-version claim. |
| Joypad library | `pukko` lineage; exact revision unknown | Official README credit; NEW/XPADMAN client behavior in target | The author credit does not identify a unique source snapshot. |

## Compiler, C library and packer

| Component | Version / revision | Evidence | Matching implication |
|---|---|---|---|
| EE GCC application compiler | `3.2.2-b1` is the strongest candidate | Shared PS2 release listings, R5900 code shape and many exact archive-member sizes | Candidate only until a reproducible compile compares bytes. Test compiler patch level and flags per translation unit. |
| EE binutils assembler/linker | exact release unknown | Target section placement and archive/member order are mapped, but carry no unique version signature | Do not infer binutils from GCC's version. Linker and assembler revisions must be tested independently. |
| linked `libgcc` / unwind runtime | GCC `3.2.2-b1` layout fingerprint | `_udivdi3`, `_umoddi3`, `unwind-dw2`, FDE and soft-float object sizes plus public offsets agree | Stronger than the global compiler claim; still mark functions `RECONSTRUCTED`, not `MATCHING`. |
| linked `libsupc++` / C++ EH | GCC 3.2.2-era ABI/runtime family | RTTI vtables, EH globals, personality and exception-object layout | Use the historical Itanium ABI implementation; modern libstdc++ is structurally different. |
| EE libc / allocator / stdio | old PS2LIB/Newlib-era snapshot; exact bundle unknown | Recovered `malloc`, formatter, string/ctype and syscall behavior matches the early PS2DEV corridor, but no global libc version string exists | Treat each archive family separately; the `mathfp` fingerprint below does not prove that every libc object came from Newlib 1.10.0. |
| Newlib `mathfp` | `1.10.0` source fingerprint | Polynomial tables, function ordering and EE-specific leaf substitutions; official archive hash pinned below | Strong fingerprint, not an embedded version string. Preserve `-mlong64` quirks and compare the target's hardware `sqrtf`/`fabsf` leaves instead of assuming generic upstream bodies. |
| executable packer | SJCRUNCH2 container; packer revision unknown | Deterministic 13-block layout at file offset `0x2f00`; unpacker-stub string `Jul 12 2002` | Pin the packed ELF hash. The date identifies the stub build, not an asserted SJCRUNCH semantic version. |
| LZO decompressor used by SJCRUNCH2 | exact LZO revision unknown | Container/block behavior and successful `lzo1x_decompress` reproduction | Host `liblzo2` is an analysis dependency only and does not prove the historical LZO release. |
| IOP-module compiler | GCC 2-family, exact release unknown | `gcc2_compiled.` / `__gnu_compiled_c` symbols in intact embedded IRXs | Preserve each IRX blob by hash until its original toolchain is independently recovered. |

## Version-bearing target strings

Offsets below are file offsets in the unpacked image, except for the final
SJCRUNCH row, which belongs to the packed ELF stub.

| Offset | Exact string evidence |
|---:|---|
| `0x0b0f20` / `0x0b0f28` | `Snes9x` / `1.41` |
| `0x0b88a0` | `unzip 0.15 Copyright 1998 Gilles Vollant` |
| `0x0b8910` | `deflate 1.1.3 Copyright 1995-1998 Jean-loup Gailly` |
| `0x0b9448` | `inflate 1.1.3 Copyright 1995-1998 Mark Adler` |
| `0x0ee940` | `CDVD: CDVD Filesystem v1.13` |
| `0x0f5340` | `SjPCM v2.0 - by Sjeep` |
| `0x0f8248` | `AmigaMod: by Vzzrzzn` (no version number) |
| packed `0x002ea0` | `Jul 12 2002` (SJCRUNCH2 stub build date) |

## Immutable fingerprints

| Artifact | Size | Unpacked-image offset | SHA-256 |
|---|---:|---:|---|
| Packed `SNES_EMU.ELF` | 726,968 bytes | — | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Unpacked EE image | 3,304,936 bytes | — | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |
| Embedded CDVD FS 1.13 IRX | 32,212 bytes (`0x7dd4`) | `0x0ec300` | `0dbf147d0f0cb2a49c7d734e92df0570b079c52dde19478bc94b5283345050de` |
| Embedded SjPCM 2.0 IRX | 8,133 bytes (`0x1fc5`) | `0x0f4100` | `690d69decfd2abaed46a06c402be0b835d3329bb49a1012ecc29a7a4a9ad579f` |
| Embedded AmigaMod IRX | 20,061 bytes (`0x4e5d`) | `0x0f6140` | `25d8b8b8e0a9ec1a28ff944eb2a21f53b87125d4f7f74ddb5f93d4621005a3e3` |
| Historical Snes9x 1.41 source archive used for validation | 1,012,496 bytes | — | `f24e5761fd91078c124241e8631370a6bd182b8dde9661d24e9761898d1838f3` |
| Official Newlib 1.10.0 source archive | downloaded by `tools/fetch_upstream.py` | — | `69b62ad4c746a9acaf4f898772549f6da49f228f83a95efce7e88ae1d88c5a84` |

The IRX blobs and original ELF are deliberately not distributed by this
repository. Their hashes, sizes and unpacked-image offsets are sufficient to
verify a legally obtained copy.

## Analysis-tool versions for reproducibility

These tools are not target dependencies, but recording them makes structural
recovery deterministic:

| Tool | Version / processor |
|---|---|
| Ghidra | `10.4 PUBLIC` |
| [Ghidra Emotion Engine: Reloaded](https://github.com/chaoticgd/ghidra-emotionengine-reloaded) | `2.1.10` |
| Ghidra language | `r5900:LE:32:default` |
| LLVM objdump used for the generic pass | `20` |
| Progress generator | repository `tools/update_progress.py`; audited universe `1,041` (`1,137` raw JAL targets − `292` data patterns + `196` non-JAL entries) |

Primary historical references are the official [Snes9x source archive
index](https://www.lysator.liu.se/snes9x/), the preserved [SNES Station v0.23
release](https://archive.org/details/snes_0_2_3_20040124), and the public
[SNESticle PS2 source/build tree](https://github.com/iaddis/SNESticle).
The exact Makefile used for the neighboring toolchain fingerprint is pinned to
SNESticle commit
[`9590ebf3bf768424ebd6cb018f322e724a7aade3`](https://github.com/iaddis/SNESticle/blob/9590ebf3bf768424ebd6cb018f322e724a7aade3/SNESticle/Project/ps2/Makefile).
