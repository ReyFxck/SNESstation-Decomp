# V106 runtime-tail source integration

V106 takes the smallest evidence-backed part of the last mismatching image
window. It rebuilds `TILE.CPP` from the pinned Snes9x 1.41-1 source and eight
selected GCC 3.2.2 `libsupc++` translation units with the historical EE C++
compiler. No target payload is committed.

## Verified result

- 14 source sections and 3,868 bytes are exact after linking.
- 56 compiler-emitted CIE/FDE records are retained.
- 73 `R_MIPS_32` relocation results are checked against the private reference;
  every byte outside those relocation words already matches the rebuilt public
  source object.
- The selected ranges cover `TILE.CPP` unwind data, the `eh_personality`,
  `eh_terminate`, `eh_throw`, `new_op`, `new_opv`, `tinfo`, `eh_alloc` and
  `eh_catch` unwind containers, the new-handler slot, and four exact LSDA
  ranges.
- Whole-image differing bytes fall from 1,859,772 to **1,857,419**.
- Window 50 falls from 5,526 to **3,173** differing bytes.
- The exact-window count remains **16/51**; 35 windows remain.

The lack of a newly closed window is intentional: a 64 KiB window counts only
when every byte matches. V106 removes 2,353 proved differences from window 50
without claiming that its unresolved gaps are known.

## Reproduction

Public-only validation:

```sh
python3 tools/runtime_tail_data.py validate
```

Private reconstruction after the V105 dependency chain and historical EE C++
bootstrap:

```sh
make runtime-tail-data-check
```

The build writes `build/runtime-tail-data/stage3j-runtime-tail-integrated.elf`
and its padded raw diagnostic. Build payloads containing resolved relocation
words stay under ignored `build/` and are never source-controlled.

## Honest boundary and next target

This is not a replacement ELF. The initialized-image SHA-256 is still not
`739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`,
and the packed SHA-256 is not claimed. Window 50 still contains 3,173
differences, primarily the earlier SPC7110 unwind container and unresolved
runtime tables. The remaining application windows 0–11 and 15–36 still need
exact source selection, archive ordering, relocations and final layout before
SJCRUNCH2 packing can be attempted.
