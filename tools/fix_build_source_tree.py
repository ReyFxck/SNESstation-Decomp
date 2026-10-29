#!/usr/bin/env python3
"""
Fix the build_source_tree.py SOURCE_FIXED_FLAGS dictionary.
Replace all _.c with .c, _.cpp with .cpp, fix syntax errors.
"""

import re
from pathlib import Path

ROOT = Path("/root/SNESstation-Decomp")
py_path = ROOT / "tools/build_source_tree.py"

content = py_path.read_text()

# Fix all _.c' to .c'
content = re.sub(r"_(\.c)'(?=\s*:)", r"\1'", content)
# Fix all _.cpp' to .cpp'
content = re.sub(r"_(\.cpp)'(?=\s*:)", r"\1'", content)
# Fix all _.S' to .S'
content = re.sub(r"_(\.S)'(?=\s*:)", r"\1'", content)

# Fix the libmtap.c line - it's missing its value
# Find the line with 'src/ps2/libmtap.c': followed by a newline and another key
content = re.sub(
    r"('src/ps2/libmtap\.c':\s*\n)\s*('src/ps2/small_dispatch\.c')",
    r"\1    ('src/ps2/libmtap.c': (-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-ffreestanding', '-fno-builtin', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DALIGN_DWORD', '-DCODE_PLATFORM=3', '-Iinclude', '-Iinclude/ee_stage1_compat', '-ffunction-sections'),\n    \2",
    content
)

# Fix duplicate audio_rpc entry on line 107
# Remove the duplicate tuple at the end of the line
content = re.sub(
    r"('src/ps2/audio_rpc\.c': \(.*?-ffunction-sections\)),\s*\(-G0.*?-ffunction-sections\)",
    r"\1",
    content
)

# Fix duplicate main_flow entry on line 152
content = re.sub(
    r"('src/app/main_flow\.c': \(.*?-ffunction-sections\)),\s*\(-G0.*?-ffunction-sections\)",
    r"\1",
    content
)

# Write fixed content
py_path.write_text(content)
print("Fixed build_source_tree.py")

# Verify by checking for any remaining _.c patterns
if "_." in content:
    print("WARNING: Still found _._ patterns in file")
    for i, line in enumerate(content.splitlines(), 1):
        if "_." in line and "src/" in line:
            print(f"  Line {i}: {line.strip()}")
else:
    print("All _. patterns fixed")

# Verify syntax
import ast
try:
    ast.parse(content)
    print("Syntax OK")
except SyntaxError as e:
    print(f"Syntax error: {e}")
    print(f"Line {e.lineno}: {e.text}")