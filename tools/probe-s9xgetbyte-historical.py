#!/usr/bin/env python3
from __future__ import annotations

import hashlib
import io
import re
import tarfile
import urllib.request

URL = "https://www.lysator.liu.se/snes9x/1.40/snes9x-1.40-src-2.tar.gz"
SHA256 = "9f5f28a72642c3bf667423b839f89bd3c80a6dc96c6c6c8751a216a3e67169af"

with urllib.request.urlopen(URL, timeout=60) as response:
    raw = response.read()
actual = hashlib.sha256(raw).hexdigest()
if actual != SHA256:
    raise SystemExit(f"archive sha mismatch: {actual}")

with tarfile.open(fileobj=io.BytesIO(raw), mode="r:gz") as archive:
    candidates = [m for m in archive.getmembers()
                  if m.isfile() and m.name.lower().endswith("/getset.h")]
    if len(candidates) != 1:
        raise SystemExit(f"expected one getset.h, found {[m.name for m in candidates]}")
    source = archive.extractfile(candidates[0]).read().decode("latin1")

match = re.search(r"(?:INLINE\s+)?uint8\s+S9xGetByte\s*\(\s*uint32\s+Address\s*\)\s*\{", source)
if not match:
    raise SystemExit("S9xGetByte body not found")
start = match.start()
brace = source.find("{", match.start())
depth = 0
end = None
for pos in range(brace, len(source)):
    ch = source[pos]
    if ch == "{":
        depth += 1
    elif ch == "}":
        depth -= 1
        if depth == 0:
            end = pos + 1
            break
if end is None:
    raise SystemExit("unterminated S9xGetByte body")
print("=== S9XGETBYTE_140_BEGIN ===")
print(source[start:end])
print("=== S9XGETBYTE_140_END ===")
