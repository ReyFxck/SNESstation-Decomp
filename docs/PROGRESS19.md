# Progress 19 - frontend sync and path helpers

This checkpoint promotes three small Progress-16 frontend targets from raw
R5900 pseudocode into typed behavioral C.

- `0x00105898` is strongly identified as the port's `S9xSyncSpeed` callback.
  The target compares `Settings.SkipFrames` with Snes9x `AUTO_FRAMERATE`
  (`200`) and updates the `IPPU.RenderThisFrame`, `IPPU.SkippedFrames` and
  fixed-frame-skip state in the expected callback pattern.  The trailing audio
  service calls remain address-bound/opaque until their owning state is typed.
- `0x001059cc` is the PS2 device-aware `_makepath` helper.  It accepts complete
  device tokens (`mc0`, `cdfs`, etc.), appends `:`, normalizes the directory
  separator, appends the filename and conditionally appends `.` + extension.
- `0x00105ae8` is the complementary device-aware `_splitpath` helper.  The
  reconstructed flow preserves drive extraction at `:`, final slash/dot
  selection, root-directory handling and filename/extension splitting.  The
  repeated slash lookup visible in the target is retained in source.

These are source-model promotions, not relocation-normalized machine-code
matches.  Matching promotion stays gated on an EE object comparison.
