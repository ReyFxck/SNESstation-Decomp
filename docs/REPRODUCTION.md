# Reproducing SNES Station v0.23

The maintained interface is the root `Makefile`. Public checks use only
committed source and hash manifests. Private checks additionally use a legally
obtained `SNES_EMU.ELF`; generated target bytes remain under ignored `build/`.

## Requirements

- Python 3.12 is used in CI; the proof tools use the standard library.
- GNU Make and a host C compiler are required for repository checks.
- The exact-code gates use the isolated EE GCC 3.2.2 compiler built by the
  repository.
- Private comparison requires the reference ELF whose hash is listed below.

Tool and dependency versions are recorded in [`TOOLS.md`](TOOLS.md) and
[`DEPENDENCY_VERSIONS.md`](DEPENDENCY_VERSIONS.md).

## Public verification

```bash
make check
```

This checks generated files, links, manifests, host syntax, unit tests, the
1,041-entry function proof, source ownership, link contracts and every
public-only whole-image contract. It does not read the original ELF.

Useful public commands:

| Command | Result |
|---|---|
| `make status` | Print the current function counts |
| `make docs` | Regenerate current status from authoritative manifests |
| `make checkpoint-1041-check` | Re-run the frozen 1,041/1,041 public proof |
| `make decompdev-report` | Generate and validate the public Objdiff Report v2 |
| `make bootstrap-ee-stage1` | Build binutils 2.14 and the EE GCC 3.2.2 C compiler |
| `make bootstrap-ee-cxx-stage1` | Build the corresponding C/C++ compiler |

## Private reference verification

Copy a legally obtained target into the ignored input directory:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make reference
```

The accepted target is:

| Form | Size | SHA-256 |
|---|---:|---|
| Packed ELF | 726,968 bytes | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Unpacked image | 3,304,936 bytes | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |

Run all currently implemented private gates with:

```bash
make reproduce-check
```

## What the pipeline proves

| Proof area | Current result |
|---|---:|
| Function entries | 1,041/1,041 |
| EE source units | 97/97 |
| Runtime contracts | 53/53 |
| Program-data address identities | 1,265/1,265 |
| Exact startup | 276 bytes, 3 functions, 27 relocations |
| Exact unpacked-image windows | 45/51 |
| Remaining unpacked-image differences | 263,765 bytes |
| Packed replacement ELF | Not yet |

The exact windows are **1–6 and 12–50**. Windows **0 and 7–11** remain. A
window is counted only when all 65,536 bytes match; partly equal windows add
zero to this count.

## Single-command pipeline

```bash
make reproduce
```

This command runs the maintained sequence:

1. verify and unpack the reference;
2. validate the audited function and source manifests;
3. build the historical EE source aggregate;
4. resolve source, runtime and program-data providers;
5. integrate exact startup, public historical data, runtime metadata and
   hash-verified private media;
6. build the current whole-image candidate and compare all 51 windows;
7. stop at the unfinished exact-link boundary.

The final stop is intentional. Function equality alone cannot recover archive
order, section placement, global relocation results or packer parameters.

## Public/private boundary

The repository may commit:

- readable source recovery;
- public historical source and immutable source hashes;
- target addresses, sizes and hashes;
- exact assembly that is clearly labelled as reconstruction evidence;
- deterministic tools that verify a user-supplied reference.

The repository must not commit:

- the original ELF or embedded IRX/media payloads, except the narrowly
  documented README logo in [`LEGAL.md`](LEGAL.md);
- unpacked or extracted target ranges;
- generated objects containing private payload bytes;
- a fabricated final executable presented as exact.

Private products are written only below ignored `build/`. The manifest checks
also reject host-specific absolute paths so local proof data cannot leak into a
commit.

## Final identity requirements

The project is complete only when all of the following agree with the target:

- all 51 unpacked-image windows;
- entry point and program headers;
- section and segment layout;
- object, archive and library order;
- symbol binding and every relocation result;
- uninitialized storage geometry;
- SJCRUNCH2/LZO stub, block layout and packed bytes;
- packed and unpacked SHA-256 values.

Current priorities are listed in [`ROADMAP.md`](ROADMAP.md). Historical details
about recovered source and compiler candidates are in
[`RECOVERY_HISTORY.md`](RECOVERY_HISTORY.md).
