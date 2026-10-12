# Snes9x 1.41-1 historical source closure

These six files preserve every original source body behind the final 36
unresolved entries in `analysis/functions/native_bulk_census.tsv`. They come
from the pinned `snes9x-1.41-1-src.tar.gz` archive (SHA-256
`5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b`).
The two remaining census entries are the checked-in comparator bodies in
`src/snes9x/native_clip.cpp`.

The files are reference source, not canonical linked translation units. Their
presence closes the source-body inventory without claiming target instruction
identity where no complete frozen target witness exists. Four MEMMAP/DSP
bodies also retain their recorded failed full-link comparison status.

The archive stores the sources as Latin-1. Repository copies use UTF-8 so they
remain readable and transport safely; encoding each checked-in file back to
Latin-1 reproduces the exact pinned archive bytes. For five ASCII-only files,
the repository and archive SHA-256 values are identical.

Run `python3 tools/run-native-pending-archive-source-recovery.py` to verify the
archive pin, byte-for-byte round trip, all 36 historical compiler symbols and
sizes, the two checked-in clip helpers, and the zero-gap census classification.
