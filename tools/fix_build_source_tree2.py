#!/usr/bin/env python3
"""
Fix the build_source_tree.py SOURCE_FIXED_FLAGS dictionary properly.
"""

from pathlib import Path

ROOT = Path("/root/SNESstation-Decomp")
py_path = ROOT / "tools/build_source_tree.py"

content = py_path.read_text()
lines = content.splitlines()

# Fix lines 77-78 (libmtap.c issue)
# Line 77: 'src/ps2/libmtap.c':
# Line 78: ('src/ps2/libmtap.c': (-G0', ...
# Should be: 'src/ps2/libmtap.c': (-G0', '-EL', ...),

# The PS2 flags tuple (copied from other PS2 entries)
ps2_flags = "    'src/ps2/libmtap.c': ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-ffreestanding', '-fno-builtin', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DALIGN_DWORD', '-DCODE_PLATFORM=3', '-Iinclude', '-Iinclude/ee_stage1_compat', '-ffunction-sections'),"

# Replace lines 77-78 (0-indexed: 76-77)
# Remove line 77 (index 76) and line 78 (index 77), insert the fixed line
if lines[76] == "    'src/ps2/libmtap.c':" and lines[77].startswith("    ('src/ps2/libmtap.c':"):
    lines[76] = ps2_flags
    lines.pop(77)  # Remove the malformed line 78
    print("Fixed libmtap.c entry")

# Now fix all remaining _.c, _.cpp, _.S patterns in the keys
new_lines = []
for line in lines:
    # Fix key patterns: 'src/xxx/yyy_.c': -> 'src/xxx/yyy.c':
    # We need to be careful to only fix the key part, not values
    if "'src/" in line and "_." in line and ":" in line:
        # Find the key part before the colon
        colon_idx = line.index(":")
        key_part = line[:colon_idx]
        value_part = line[colon_idx:]

        # Fix the key part
        key_part = key_part.replace("_.c'", ".c'")
        key_part = key_part.replace("_.cpp'", ".cpp'")
        key_part = key_part.replace("_.S'", ".S'")

        line = key_part + value_part

    new_lines.append(line)

# Check for duplicate audio_rpc line
# Look for a line with audio_rpc.c that has two tuples
for i, line in enumerate(new_lines):
    if "'src/ps2/audio_rpc.c':" in line and line.count("('") > 1:
        # This line has duplicate tuples - fix it
        # Find the first closing parenthesis of the tuple
        first_tuple_end = line.find("'-ffunction-sections'),")
        if first_tuple_end != -1:
            line = line[:first_tuple_end + len("'-ffunction-sections'),")]
            new_lines[i] = line
            print(f"Fixed duplicate audio_rpc at line {i}")

# Check for duplicate main_flow line
for i, line in enumerate(new_lines):
    if "'src/app/main_flow.c':" in line and line.count("('") > 1:
        first_tuple_end = line.find("'-ffunction-sections'),")
        if first_tuple_end != -1:
            line = line[:first_tuple_end + len("'-ffunction-sections'),")]
            new_lines[i] = line
            print(f"Fixed duplicate main_flow at line {i}")

# Write fixed content
new_content = "\n".join(new_lines) + "\n"
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
for i, line in enumerate(new_lines):
    if "'src/" in line and "_." in line and ":" in line:
        print(f"WARNING: Still has _. pattern at line {i+1}: {line.strip()}")