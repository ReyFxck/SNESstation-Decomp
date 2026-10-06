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

match = re.search(
    r"(?:INLINE\s+)?void\s+S9xSetByte\s*\(\s*uint8\s+Byte\s*,\s*uint32\s+Address\s*\)\s*\{",
    source,
)
if not match:
    raise SystemExit("S9xSetByte body not found")
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
    raise SystemExit("unterminated S9xSetByte body")

print("=== S9XSETBYTE_140_BEGIN ===")
print(source[start:end])
print("=== S9XSETBYTE_140_END ===")


def emit_context(archive, suffix, needles, before=2500, after=10000):
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
    for suffix, needles in [
        ("/cpuexec.h", ["struct SCPUState", "SRAMModified", "PCAtOpcodeStart"]),
        ("/sa1.h", ["struct SSA1", "WaitByteAddress1", "S9xOpcodes"]),
        ("/spc7110.h", ["bank50", "S9xSetSPC7110"]),
        ("/obc1.h", ["SetOBC1", "OBC1"]),
        ("/seta.h", ["S9xSetSetaDSP", "S9xSetST018"]),
    ]:
        try:
            emit_context(archive, suffix, needles)
        except SystemExit as exc:
            print(f"=== OPTIONAL_CONTEXT_MISSING {suffix}: {exc} ===")
