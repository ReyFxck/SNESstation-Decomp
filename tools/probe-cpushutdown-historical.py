#!/usr/bin/env python3
# Canonical recovery target: CPUShutdown @ 0x001ac604.
from __future__ import annotations

import hashlib
import io
import re
import tarfile
import urllib.request

URL = "https://www.lysator.liu.se/snes9x/1.42/snes9x-1.42-src.tar.gz"
SHA256 = "136c7c9bf826bf9dba91073aae14e4b315ab78e67eee189322dec2637e949ffb"

with urllib.request.urlopen(URL, timeout=60) as response:
    raw = response.read()
actual = hashlib.sha256(raw).hexdigest()
if actual != SHA256:
    raise SystemExit(f"archive sha mismatch: {actual}")

with tarfile.open(fileobj=io.BytesIO(raw), mode="r:gz") as archive:
    candidates = [m for m in archive.getmembers()
                  if m.isfile() and m.name.lower().endswith("/cpuops.cpp")]
    if len(candidates) != 1:
        raise SystemExit(f"expected one cpuops.cpp, found {[m.name for m in candidates]}")
    source = archive.extractfile(candidates[0]).read().decode("latin1")

match = re.search(r"(?:inline\s+)?void\s+CPUShutdown\s*\(\s*\)\s*\{", source)
if not match:
    raise SystemExit("CPUShutdown body not found")
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
    raise SystemExit("unterminated CPUShutdown body")

print("=== CPUSHUTDOWN_142_BEGIN ===")
print(source[start:end])
print("=== CPUSHUTDOWN_142_END ===")

def emit_hits(archive, needle, suffixes=(".h", ".hpp", ".cpp"), before=3500, after=9000):
    for member in archive.getmembers():
        if not member.isfile() or not member.name.lower().endswith(tuple(s.lower() for s in suffixes)):
            continue
        text = archive.extractfile(member).read().decode("latin1")
        pos = text.find(needle)
        if pos >= 0:
            print(f"=== CONTEXT {member.name}:{needle} ===")
            print(text[max(0,pos-before):min(len(text),pos+after)])
            return
    print(f"=== OPTIONAL_CONTEXT_MISSING {needle} ===")

with tarfile.open(fileobj=io.BytesIO(raw), mode="r:gz") as archive:
    for needle in (
        "struct SCPUState",
        "struct SSettings",
        "struct SIAPU",
        "struct SAPU",
        "struct SICPU",
        "S9xUpdateAPUTimer",
        "S9xSA1ExecuteDuringSleep",
        "IRQ_PENDING_FLAG",
        "NMI_FLAG",
        "APU_EXECUTE1",
    ):
        emit_hits(archive, needle)
