#!/usr/bin/env python3
from __future__ import annotations

import hashlib
import io
import re
import tarfile
import urllib.request

URL = "https://www.lysator.liu.se/snes9x/1.41-1/snes9x-1.41-1-src.tar.gz"
SHA256 = "5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b"

with urllib.request.urlopen(URL, timeout=60) as response:
    raw = response.read()
actual = hashlib.sha256(raw).hexdigest()
if actual != SHA256:
    raise SystemExit(f"archive sha mismatch: {actual}")

with tarfile.open(fileobj=io.BytesIO(raw), mode="r:gz") as archive:
    members = [m for m in archive.getmembers() if m.isfile()]
    candidates = [m for m in members if m.name.lower().endswith("/getset.h")]
    if len(candidates) != 1:
        raise SystemExit(f"expected one getset.h, found {[m.name for m in candidates]}")
    source = archive.extractfile(candidates[0]).read().decode("latin1")

match = re.search(
    r"(?:INLINE\s+)?uint16\s+S9xGetWord\s*\(\s*uint32\s+Address\s*\)\s*\{",
    source,
)
if not match:
    raise SystemExit("S9xGetWord body not found")
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
    raise SystemExit("unterminated S9xGetWord body")

print("=== S9XGETWORD_1411_BEGIN ===")
print(source[start:end])
print("=== S9XGETWORD_1411_END ===")

def emit_context(archive, suffix, needles, before=1800, after=10000):
    matches = [m for m in archive.getmembers()
               if m.isfile() and m.name.lower().endswith(suffix.lower())]
    if not matches:
        raise SystemExit(f"missing {suffix}")
    text = archive.extractfile(matches[0]).read().decode("latin1")
    for needle in needles:
        pos = text.find(needle)
        if pos >= 0:
            print(f"=== {suffix}:{needle} ===")
            print(text[max(0, pos-before):min(len(text), pos+after)])
            return
    raise SystemExit(f"none of {needles} found in {suffix}")

with tarfile.open(fileobj=io.BytesIO(raw), mode="r:gz") as archive:
    emit_context(archive, "/memmap.h", ["class CMemory", "struct CMemory"])
    emit_context(archive, "/cpuexec.h", ["struct SCPUState", "PCAtOpcodeStart"])
    emit_context(archive, "/getset.h", ["INLINE uint8 S9xGetByte", "S9xGetByte (uint32 Address)"])
