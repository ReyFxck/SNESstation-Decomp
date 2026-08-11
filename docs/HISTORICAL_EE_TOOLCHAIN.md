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
safe route is to rebuild the historical source for the current host or use a
deliberately pinned emulated build environment; neither route is assumed to be
bit-stable until its output is compared.

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

Building this candidate reproducibly on a modern ARM64 host is the next phase;
the historical script itself is evidence, not a safe modern installer.

## Primary historical evidence

- [PS2DEV toolchain root commit](https://github.com/ps2dev/ps2toolchain/tree/16a47184b3a5fdf4aea45fcc8fee082d3c4d4183)
- [GCC 3.2.2 PS2 patch at that commit](https://github.com/ps2dev/ps2toolchain/blob/16a47184b3a5fdf4aea45fcc8fee082d3c4d4183/gcc-3.2.2.patch)
- [SNESticle PS2 Makefile](https://github.com/iaddis/SNESticle/blob/9590ebf3bf768424ebd6cb018f322e724a7aade3/SNESticle/Project/ps2/Makefile)
- [SNESticle link map carrying the Cygwin installation path](https://github.com/iaddis/SNESticle/blob/9590ebf3bf768424ebd6cb018f322e724a7aade3/SNESticle/Project/ps2/release_EE3.2.2-b1/SNESticle.map)
