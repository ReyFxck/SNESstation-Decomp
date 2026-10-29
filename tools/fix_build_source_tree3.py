#!/usr/bin/env python3
"""
Fix the build_source_tree.py - memory_cleanup.c missing value.
"""

from pathlib import Path

ROOT = Path("/root/SNESstation-Decomp")
py_path = ROOT / "tools/build_source_tree.py"

content = py_path.read_text()
lines = content.splitlines()

# Fix line 108 (0-indexed: 107) - memory_cleanup.c missing value
# It should have SNES9X flags
snes9x_flags = "    'src/snes9x/memory_cleanup.c': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-fno-builtin', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections'),"

if lines[107] == "    'src/snes9x/memory_cleanup.c':":
    lines[107] = snes9x_flags
    print("Fixed memory_cleanup.c entry")

# Write fixed content
new_content = "\n".join(lines) + "\n"
py_path.write_text(new_content)
print("Fixed build_source_tree.py")

# Verify syntax
import ast
try:
    ast.parse(new_content)
    print("Syntax OK")
except SyntaxError as e:
    print(f"Syntax error: {e}")
    print(f"Line {e.lineno}: {e.text}")

# Verify no _. patterns remain in keys
for i, line in enumerate(lines):
    if "'src/" in line and "_." in line and ":" in line:
        print(f"WARNING: Still has _. pattern at line {i+1}: {line.strip()}")

# Check for any lines ending with just ':' (missing values)
for i, line in enumerate(lines):
    stripped = line.strip()
    if stripped.endswith("':") and not stripped.endswith("'),"):
        print(f"WARNING: Line {i+1} might be missing value: {line.strip()}")