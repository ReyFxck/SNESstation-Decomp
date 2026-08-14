# Progress 31 — close the committed libgcc unwind leaf probe

Progress 30 produced the first trustworthy historical EE source scan:
6 translation units pass GCC 3.2.2 and 95 require compatibility cleanup.  It
also reduced the local libgcc probe to the seven functions whose target bytes
are actually present in the committed listing.

## 1. Signed switch decision trees

The target `size_of_encoded_value` and `base_of_encoded_value` use `slti` in
the compiler-generated switch trees.  The Progress-30 candidate used unsigned
mask literals, which made GCC emit `sltu`.  Progress 31 keeps the mask expression
as `int`, matching the promotion used by the historical unwind source.

## 2. LEB128 shift width

The target ULEB/SLEB loops use 32-bit `sllv` for `(byte & 0x7f) << shift`, then
merge that value into the 64-bit unwind word.  Casting the byte to the unwind
word before the shift forced `dsllv` and changed the generated loop.  The cast
is now deliberately omitted.  The SLEB sign-extension expression remains a
64-bit shift, as in the target.

## 3. Function size versus alignment padding

The target has four bytes of `.p2align` padding after `size_of_encoded_value`
and `read_uleb128`.  Those bytes sit before the next aligned function but are
not part of the candidate ELF symbol `st_size`.  The local listing manifest now
ends those functions at `0x001a3e2c` and `0x001a3f24` respectively.

## 4. EE scan triage

`tools/summarize_ee_scan.py` groups the 95 old-GCC failures by their first real
diagnostic.  This turns the next source-cleanup phase into batches rather than
editing translation units one by one blindly.

Run:

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make check
make match-libgcc-unwind-listing EE_CC="$EE_CC"
python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv
```

The 12-function formal gate still requires the user's legally obtained packed
reference at `original/SNES_EMU.ELF` and remains the only promotion authority.
