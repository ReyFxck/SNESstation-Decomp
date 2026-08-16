# Progress 67 libpad object-layout gate

Progress 67 reuses the historical `libpad-newpadman` source-lineage object
compiled with the pinned EE GCC 3.2.2 `-Os` profile.

Dominant target/object layout base:

`0x001a83e0`

The layout is anchored by 18 already-MATCHING libpad symbols. The strict sweep
found five RECONSTRUCTED functions whose complete object symbol bytes match the
target after applying that layout.

Three manifest starts were corrected:

- `padStateInt2String`: `0x001a896c` -> `0x001a8964`
- `padReqStateInt2String`: `0x001a89ac` -> `0x001a89a4`
- `padSetActDirect`: `0x001a8fe4` -> `0x001a8fe0`

Two additional functions already had the correct manifest start:

- `padSetReqState` at `0x001a8938`
- `padSetActAlign` at `0x001a8f08`

All five have:

- strict relocation-aware `MATCH`
- `differing_bytes=0`
- `normalized_equal=True`
- no unknown relocation types
- `object-layout-exact-next` boundary proof

The `.c.txt` suffix is intentional so root `host-syntax` does not compile the
historical PS2SDK candidate as an independent host C translation unit.

Frozen source SHA-256: `ea6129f89fb58f682d24244c5a37ad9a2b17dc59aa7f9d4220243f3e2f0e2d2b`
