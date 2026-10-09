#!/usr/bin/env python3
"""Print the frozen Snes9x 1.41-1 C4DrawWireFrame source context."""
from __future__ import annotations
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))
import hunt1000plus_v47_closure as v47

archive = v47.download_archive(v47.SNES_141_1_ARCHIVE, v47.SNES_CACHE)
source_root = v47.safe_extract_archive(
    archive,
    v47.SNES_CACHE / "source-1.41-1",
    v47.SNES_141_1_ARCHIVE.source_directory,
)
path = source_root / "snes9x" / "c4emu.cpp"
text = path.read_text(encoding="latin-1")
needle = "C4DrawWireFrame"
positions = []
cursor = 0
while True:
    pos = text.find(needle, cursor)
    if pos < 0:
        break
    positions.append(pos)
    cursor = pos + len(needle)
print(f"C4DrawWireFrame occurrences={positions}")
for index, pos in enumerate(positions):
    lo = max(0, pos - 1500)
    hi = min(len(text), pos + 7500)
    print(f"=== C4DRAW_CONTEXT_{index}_BEGIN ===")
    print(text[lo:hi])
    print(f"=== C4DRAW_CONTEXT_{index}_END ===")
