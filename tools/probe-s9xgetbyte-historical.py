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


def extract_member(archive, suffix, needles):
    matches = [m for m in archive.getmembers()
               if m.isfile() and m.name.lower().endswith(suffix.lower())]
    if not matches:
        raise SystemExit(f"missing {suffix}")
    text = archive.extractfile(matches[0]).read().decode("latin1")
    for needle in needles:
        pos = text.find(needle)
        if pos >= 0:
            print(f"=== {suffix}:{needle} ===")
            print(text[max(0, pos-1200):min(len(text), pos+9000)])
            return
    raise SystemExit(f"none of {needles} found in {suffix}")

with tarfile.open(fileobj=io.BytesIO(raw), mode="r:gz") as archive:
    extract_member(archive, "/memmap.h", ["class CMemory", "struct CMemory"])
    found = False
    for member in archive.getmembers():
        if not member.isfile() or not member.name.lower().endswith((".h", ".hpp")):
            continue
        text = archive.extractfile(member).read().decode("latin1")
        pos = text.find("PCAtOpcodeStart")
        if pos >= 0:
            print(f"=== {member.name}:PCAtOpcodeStart ===")
            print(text[max(0, pos-4500):min(len(text), pos+4500)])
            found = True
            break
    if not found:
        raise SystemExit("PCAtOpcodeStart layout not found")


with tarfile.open(fileobj=io.BytesIO(raw), mode="r:gz") as archive:
    helper_names = (
        "S9xGetPPU", "S9xGetCPU", "S9xGetDSP", "S9xGetC4",
        "S9xGetSPC7110Byte", "S9xGetSPC7110", "GetOBC1",
        "S9xGetSetaDSP", "S9xGetST018", "ONE_CYCLE", "SLOW_ONE_CYCLE",
    )
    for helper in helper_names:
        print(f"=== LOOKUP {helper} ===")
        hits = 0
        for member in archive.getmembers():
            if not member.isfile() or not member.name.lower().endswith((".h", ".cpp", ".c")):
                continue
            text = archive.extractfile(member).read().decode("latin1")
            for line in text.splitlines():
                if helper in line and (
                    helper.endswith("CYCLE") or
                    re.search(r"\b" + re.escape(helper) + r"\s*\(", line)
                ):
                    print(member.name + ": " + line.strip())
                    hits += 1
                    if hits >= 8:
                        break
            if hits >= 8:
                break
        if not hits:
            print("(no hits)")
