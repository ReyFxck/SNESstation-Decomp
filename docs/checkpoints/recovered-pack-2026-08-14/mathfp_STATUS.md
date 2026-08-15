# MathFP — MATCHED

**7/7 relocation-normalized matches** against the committed listing.

- 0x0019fddc cosf
- 0x001a0024 sinf
- 0x001a0254 tanf
- 0x001a045c atanf
- 0x001a06a0 sqrtf
- 0x001a06b0 fabsf
- 0x001a06c0 numtestf

The complete historical-expression C candidate remains in the repository as
`matching/candidates/mathfp.c`; `mathfp_numtest.S` is the byte-exact leaf used
for the seventh function because the surviving BETA 3 backend does not emit the
older target instruction selection from readable C.
