# V109 — Embedded-media integration

Stage 3M integrates the six remaining embedded-media containers and closes all
twenty 64 KiB image windows from 15 through 34. The cumulative diagnostic
jumps from 18/51 to 38/51 exact windows.

## Integrated containers

| Container | Address | Asset bytes | Verified trailer |
|---|---:|---:|---:|
| Frontend background IIF | `0x001fafd0` | 614,416 | 32-bit size word |
| Frontend logo IIF | `0x00290ff0` | 106,976 | 32-bit size word |
| Panel-corner IIF | `0x002ab1e0` | 2,320 | 32-bit size word |
| Frontend BFNT font | `0x002abb00` | 262,432 | 32-bit size word |
| Azazel ProTracker module | `0x002ec540` | 222,220 | 32-bit size word |
| Memory Card icon | `0x00322980` | 76,024 | 32-bit size word |

The catalog already derived each asset boundary from its format and froze its
SHA-256. V109 additionally verifies the six adjacent size words and links the
complete 1,284,412-byte envelopes. Two older one-word/one-byte fixed sections
inside those envelopes are preserved as absolute aliases instead of duplicate
storage.

The private reference is required only by the full integration gate. Generated
assembly uses `.incbin` below ignored `build/`; graphics, font, music, icon and
original executable bytes are not committed. The public manifest contains only
names, geometry, hashes, metrics and explicit non-completion claims.

## Result

- Exact windows: **38/51**.
- Newly exact: **15–34** (20 consecutive windows).
- Remaining windows: **13**.
- Differing bytes: **661,433**.
- Differences removed: **1,184,204**.
- Complete replacement ELF: **not yet**.

Public contract only:

```sh
make media-assets-public-check
```

Private rebuild and byte gate:

```sh
make media-assets
```

The remaining differences are confined to windows 0–11 and 35. Exact
application/runtime implementation selection and packed-ELF reproduction are
still open.
