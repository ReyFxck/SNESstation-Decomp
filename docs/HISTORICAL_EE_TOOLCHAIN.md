# Historical EE toolchain candidate

This checkpoint pins the first public, source-available GCC 3.2.2 PS2DEV build
recipe that can be tested against SNES Station. It does **not** claim that the
recipe is already the exact compiler used for the target.

## What is actually proven

The neighboring SNESticle build tree calls its compiler installation
`3.2.2-b1`, invokes `ee-gcc`, and preserves GCC-generated listings whose
`.ident` is `GCC: (GNU) 3.2.2`. Its link map also exposes an installation path
for the `i686-pc-cygwin` host. The `-b1` text is therefore an external build
label; GCC's own `-dumpversion` output cannot prove that patch level.

The official PS2DEV toolchain repository retains this immutable root commit:

```text
repository  https://github.com/ps2dev/ps2toolchain.git
commit      16a47184b3a5fdf4aea45fcc8fee082d3c4d4183
date        2004-12-31
```

That commit records the following build stack and order:

| Layer | Candidate version | Historical target/prefix |
|---|---|---|
| GNU binutils | `2.14` + PS2 patch | `--target=ee`, tools named `ee-*` |
| GCC | `3.2.2` + PS2 patch | C stage 1, then C/C++ stage 2 |
| Newlib | `1.10.0` + PS2 patch | `--target=ee`, built with `CPPFLAGS=-G0` |
| EE processor | Toshiba R5900, little-endian | canonical alias `mips64r5900el-scei-elf` |

The exact files are verified after download:

| File | SHA-256 |
|---|---|
| `README.TXT` | `514c2b9bffdba7e64ef9f436fc45eb63c881b0ba0dd05d2ba2ec24e5c519b714` |
| `toolchain.sh` | `3962bf7c32209b84db543b59273dd46c78c49387397d88c625156aeba3a7b9ff` |
| `binutils-2.14.patch` | `f63a6d656d51e9ed74bd26d7cde4fb237d6552569980f00c979ef73f52ff3cda` |
| `gcc-3.2.2.patch` | `803395ac6345d71ebdcdf6bd4a8981863e64b0cfb36cfb48c607c59d433c5b9a` |
| `newlib-1.10.0.patch` | `618634ff422e17aa517445308a2acbf8bfba95f8fee8b0bd5cbc123a0d69db38` |

Fetch only this recipe and its patches with:

```bash
make fetch-ee-toolchain-recipe
```

The command writes under ignored `build/upstream/`, checks the commit and all
five hashes, and does not install anything or download compiler binaries.

## Reproducible stage-one bootstrap

The repository can now build the smallest useful compiler layer locally:

```bash
make bootstrap-ee-stage1
```

This downloads and verifies the two upstream GNU release archives, applies the
pinned PS2DEV patches, builds only EE binutils and the C-only GCC stage one,
then runs the compiler contract probe.

| Archive | SHA-256 |
|---|---|
| `binutils-2.14.tar.gz` | `ba91202a1aefca79f5eeb534e6c4235c874b220a2975725296712e42e6b91df1` |
| `gcc-3.2.2.tar.gz` | `a0a626b10be8f793349a5309dd054a224c00772c83207d0354499c17e8deb187` |

Everything is written below the ignored directory
`build/toolchains/ee-gcc-3.2.2-stage1/`; the command does not use `sudo`, copy
files into `/usr`, or alter the login environment. Downloads, source trees,
logs and completed-step stamps are retained so an interrupted phone build can
resume. The validated x86_64 tree occupied 486 MiB; allow about 600 MiB and keep
at least 1 GiB free while building. Override conservative parallelism when
needed with, for example, `make bootstrap-ee-stage1 EE_BUILD_JOBS=2`.

On Debian/Ubuntu the host prerequisites are:

```bash
apt install git patch make gcc binutils python3
```

Four host compatibility changes are deliberately separated from the historical
PS2 backend patch:

- the old host-side tools are compiled with `_FORTIFY_SOURCE` disabled because
  binutils 2.14 intentionally formats fixed-width archive fields across
  adjacent structure members, which Ubuntu's modern fortified `sprintf`
  rejects at runtime;
- GCC receives the two small `obstack.h` and `collect2.c` fixes preserved in a
  later PS2DEV patch. They repair modern host-C compilation and the required
  `open(..., O_CREAT, mode)` call; neither changes R5900 code generation.
- the 2003 GNU `config.guess` and `config.sub` scripts receive only the missing
  AArch64 Linux recognition needed to identify a modern ARM64 host;
- GCC 3.2.2's `config.gcc` accepts AArch64 as a build/host system while still
  rejecting it as a code-generation target. The compiler target remains the
  historical `mips64r5900el-scei-elf` EE backend.

The tracked compatibility patch SHA-256 values are:

| Patch | SHA-256 |
|---|---|
| `gcc-3.2.2-modern-host.patch` | `8e799725842a266d8bce883791c7cbf044a9e0753779102ce5d29852496ec8fe` |
| `gnu-config-aarch64.patch` | `9b083b0d9d3cb7cdb2b394e4ff2535877202dad5c81da9dc1fe24a59185682f6` |
| `gcc-3.2.2-aarch64-host.patch` | `4ce10fbb0a1545ff8ba57b6f101d9640c559f497bb2a4c5e858f4c1a3c71ab11` |

The exact commands, host identity, archive hashes, patch hash and resulting
host-specific compiler hash are saved in
`build/toolchains/ee-gcc-3.2.2-stage1/bootstrap-manifest.json`. The complete
bootstrap passed on an x86_64 Ubuntu host with GCC 13 and produced an `ee-gcc`
that passes the R5900/ELF32 smoke probe and compiles the isolated `get_tree`
candidate. An ARM64 DroidSpaces run exposed the original 2003
`config.guess` limitation before compilation. The corrected
`aarch64-unknown-linux-gnu` configure path now completes a full stage-one build
proxy; completion and the compiler probe on the native ARM64 host remain the
decisive pending test.

After it passes, use the printed absolute compiler path:

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make match-get-tree EE_CC="$EE_CC"
```

This is a **compile-only matching toolchain**. It does not yet build Newlib,
C++, PS2SDK, the historical application archives, a replacement ELF or the
SJCRUNCH2 container.

## The chronology warning

The SNES Station reference identifies itself as `0.23 WIP`, 24 January 2004.
The retained PS2DEV commit is from December 2004, and its GCC patch contains
the text `BETA 3 Release 3.2.2-20040214-1`. That public patch snapshot is newer
than the target date.

Consequently:

- GCC 3.2.2/R5900 remains a strong compiler-family hypothesis;
- the December recipe is the first reproducible public candidate, not proof of
  the target's exact patch level;
- only generated-byte comparisons can decide whether this candidate is close,
  exact, or must be rolled back to an earlier missing patch snapshot.

## ARM64 and old prebuilt binaries

The surviving SNESticle map refers to an `i686-pc-cygwin` installation. Such a
binary is not a native compiler for an ARM64 Android/Linux environment. The
bootstrap above instead rebuilds the historical source for the current host.
Its emitted target bytes are not assumed to be exact until they are compared.

Check any compiler before matching:

```bash
make toolchain-probe EE_CC=/absolute/path/to/ee-gcc
```

The probe reports the host, GCC base version, target tuple, acceptance of the
recorded R5900 flags, and whether a smoke compile produces an ELF32
little-endian MIPS relocatable object. A pass proves only the base compiler
contract. It deliberately reports the `-b1` patch level as unproven.

After a pass, the next evidence gate is:

```bash
make match-get-tree EE_CC=/absolute/path/to/ee-gcc
```

On ARM64, run the bootstrap directly rather than attempting to execute the old
Cygwin binary. A successful probe proves that the native host compiler can emit
the expected EE object family; function comparison remains the next evidence
gate.

If an older overlay stopped at `01-binutils-configure` with `unable to guess
system type`, extract the corrected overlay and run the same bootstrap command
again. Downloads and extracted sources under `build/` are retained, the new
patches are applied once, and the failed configure step is retried; deleting
the build directory is not required.

## Primary historical evidence

- [PS2DEV toolchain root commit](https://github.com/ps2dev/ps2toolchain/tree/16a47184b3a5fdf4aea45fcc8fee082d3c4d4183)
- [GCC 3.2.2 PS2 patch at that commit](https://github.com/ps2dev/ps2toolchain/blob/16a47184b3a5fdf4aea45fcc8fee082d3c4d4183/gcc-3.2.2.patch)
- [GNU binutils 2.14 release archive](https://ftp.gnu.org/gnu/binutils/binutils-2.14.tar.gz)
- [GNU GCC 3.2.2 release archive](https://ftp.gnu.org/gnu/gcc/gcc-3.2.2/gcc-3.2.2.tar.gz)
- [SNESticle PS2 Makefile](https://github.com/iaddis/SNESticle/blob/9590ebf3bf768424ebd6cb018f322e724a7aade3/SNESticle/Project/ps2/Makefile)
- [SNESticle link map carrying the Cygwin installation path](https://github.com/iaddis/SNESticle/blob/9590ebf3bf768424ebd6cb018f322e724a7aade3/SNESticle/Project/ps2/release_EE3.2.2-b1/SNESticle.map)
