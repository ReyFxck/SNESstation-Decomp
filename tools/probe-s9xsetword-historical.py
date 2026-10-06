#!/usr/bin/env python3
# Canonical recovery target: S9xSetWord @ 0x001ac190 (next TU after S9xSetPCBase).
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
    candidates = [m for m in archive.getmembers()
                  if m.isfile() and m.name.lower().endswith("/getset.h")]
    if len(candidates) != 1:
        raise SystemExit(f"expected one getset.h, found {[m.name for m in candidates]}")
    source = archive.extractfile(candidates[0]).read().decode("latin1")

match = re.search(
    r"(?:INLINE\s+)?void\s+S9xSetWord\s*\(\s*uint16\s+Word\s*,\s*uint32\s+Address\s*\)\s*\{",
    source,
)
if not match:
    raise SystemExit("S9xSetWord body not found")
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
    raise SystemExit("unterminated S9xSetWord body")

print("=== S9XSETWORD_1411_BEGIN ===")
print(source[start:end])
print("=== S9XSETWORD_1411_END ===")

def emit_context(archive, suffix, needles, before=2500, after=12000):
    matches = [m for m in archive.getmembers()
               if m.isfile() and m.name.lower().endswith(suffix.lower())]
    if not matches:
        print(f"=== OPTIONAL_CONTEXT_MISSING {suffix} ===")
        return
    text = archive.extractfile(matches[0]).read().decode("latin1")
    for needle in needles:
        pos = text.find(needle)
        if pos >= 0:
            print(f"=== {suffix}:{needle} ===")
            print(text[max(0, pos-before):min(len(text), pos+after)])
            return
    print(f"=== OPTIONAL_CONTEXT_MISSING {suffix}:{needles} ===")

with tarfile.open(fileobj=io.BytesIO(raw), mode="r:gz") as archive:
    emit_context(archive, "/memmap.h", ["class CMemory", "struct CMemory"])
    for needle in ("SRAMModified", "MemSpeedx2", "PCAtOpcodeStart"):
        found = False
        for member in archive.getmembers():
            if not member.isfile() or not member.name.lower().endswith((".h", ".hpp")):
                continue
            text = archive.extractfile(member).read().decode("latin1")
            pos = text.find(needle)
            if pos >= 0:
                print(f"=== {member.name}:{needle} ===")
                print(text[max(0, pos-4000):min(len(text), pos+6000)])
                found = True
                break
        if not found:
            print(f"=== OPTIONAL_CONTEXT_MISSING {needle} ===")
    emit_context(archive, "/sa1.h", ["WaitByteAddress1", "struct SSA1"])
    emit_context(archive, "/spc7110.h", ["bank50", "struct SPC7110"])
    emit_context(archive, "/getset.h", ["S9xSetByte (uint8 Byte", "S9xSetByte(uint8"])
