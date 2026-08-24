# HUNT1000+ V42 — profile-matrix saturation check

V42 ran the complete deterministic **16-profile × 103-translation-unit** matrix
after V41. Of 1,648 compile jobs, 1,647 succeeded. The only failure was a
repeatable GCC 3.2.2 `-O1` internal compiler error in
`src/snes9x/progress13_runtime_more_recovered.c`; it is now cached as a failure
instead of being retried on every run.

The full sweep found the same 28 addresses already selected by the three-profile
default and no additional strict match. This is the reason the normal workflow
uses `o2`, `os`, and `o2-nosched1`: it performs 309 jobs instead of 1,648 while
preserving all observed discoveries. The complete matrix remains available via
`make match-miner-full` for source or toolchain changes.

This saturation result is an optimization finding, not a matching promotion;
the repository therefore keeps no duplicate 28-row evidence report for V42.
