# Reference binary

Place your own reference copy here as:

```text
original/SNES_EMU.ELF
```

Expected SHA-256 for the currently selected v0.23 WIP target:

```text
4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487
```

The executable is intentionally not distributed by this repository.

After placing the private file, run:

```bash
make reference
```

The unpacked image and all extracted assets are written under ignored `build/`
storage. The public repository tracks only fingerprints and deterministic
verification logic.
