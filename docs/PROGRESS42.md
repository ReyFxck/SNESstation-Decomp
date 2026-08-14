# Progress 42 — historical GSLIB source + stale-object-proof runner

The previous 4/7 measurement did not show an EE compile command and therefore
reused an existing `gslib_hw.o`.  It was not a measurement of the newly
recovered historical `hw.c` source.

This progress:

- restores the surviving historical GSLIB source idioms for the seven-function
  corridor;
- restores the measured manifest boundaries, excluding alignment padding;
- adds `tools/run-gslib-frontier.sh`, which always deletes the candidate object
  before compiling;
- prints only the matching summary while retaining the full compiler/listing
  output in `build/matching/gslib_hw/frontier-run.log`;
- automatically runs the strict local listing gate when 7/7 is reached.

Historical source evidence:
`ps2homebrew/gslib/source/hw.c`,
commit `d9e623a351627e53420f44b00d494346cee5d5a2`.
