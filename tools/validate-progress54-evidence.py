#!/usr/bin/env python3
from __future__ import annotations
import csv, importlib.util, re, sys
from pathlib import Path

ROOT = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
TARGET_BASE = 0x00100000
SCREEN = ROOT / "build/matching/progress54-200plus-screen2/matches.tsv"
HIST = ROOT / "build/matching/progress54-historical-per-object/new_matches.tsv"
OUT = ROOT / "build/matching/progress54-validation"
OUT.mkdir(parents=True, exist_ok=True)
EXPECTED = set(["0x001057fc", "0x0010a8bc", "0x00151330", "0x0018339c", "0x0018e2e0", "0x0018f240", "0x0018f638", "0x0018f654", "0x001907cc", "0x001908bc", "0x00193564", "0x00193580", "0x001935d8", "0x0019371c", "0x00193cd8", "0x00193d0c", "0x00193dbc", "0x00193f08", "0x001940ac", "0x00194378", "0x00194398", "0x001943c0", "0x00194498", "0x0019532c", "0x0019533c", "0x00196980", "0x00196988", "0x00198aa4", "0x00198d38", "0x00198d58", "0x001990f8", "0x00199178", "0x00199198", "0x001991a0", "0x001991a8", "0x001991b0", "0x001991e8", "0x001991f8", "0x00199208", "0x00199290", "0x00199700", "0x00199720", "0x00199830", "0x00199848", "0x00199898", "0x001998b8", "0x0019a580", "0x0019bf00", "0x0019c0d0", "0x0019c128", "0x0019c190", "0x0019c2ac", "0x0019c304", "0x0019c688", "0x0019c7b0", "0x0019c960", "0x0019c978", "0x0019ca0c", "0x0019ca54", "0x0019cb08", "0x0019cba8", "0x0019cd4c", "0x0019cd70", "0x0019ce2c", "0x0019cfc0", "0x0019d090", "0x0019d120", "0x0019d360", "0x0019d534", "0x0019d600", "0x0019d620", "0x0019e274", "0x0019e860", "0x0019eaa4", "0x0019edac", "0x0019eddc", "0x0019ee0c", "0x0019ee20", "0x0019ee34", "0x0019ee80", "0x0019ee94", "0x0019eee0", "0x0019eefc", "0x0019ef3c", "0x0019ef5c", "0x0019efac", "0x0019f138", "0x0019f264", "0x0019f2a0", "0x0019f2dc", "0x0019f2e8", "0x0019f510", "0x0019f544", "0x0019faa8", "0x0019fb00", "0x0019fbf0", "0x001a10d0", "0x001a1af4"])

def die(msg):
    raise SystemExit("Progress54 evidence validator: " + msg)

if not SCREEN.exists() or not HIST.exists():
    die("missing probe evidence; run the successful Progress54 probes first")

with open(ROOT/"analysis/progress_targets.csv", newline="", encoding="utf-8") as f:
    progress = {int(r["address"],16): r for r in csv.DictReader(f)}
with SCREEN.open(newline="", encoding="utf-8") as f:
    srows = list(csv.DictReader(f, delimiter="\t"))
with HIST.open(newline="", encoding="utf-8") as f:
    hrows = list(csv.DictReader(f, delimiter="\t"))

if len(srows) != 82:
    die(f"expected 82 Screen2 rows, found {len(srows)}")
if len(hrows) != 16:
    die(f"expected 16 historical rows, found {len(hrows)}")
got = {r["address"].lower() for r in srows+hrows}
if got != EXPECTED:
    die(f"address-set mismatch; missing={sorted(EXPECTED-got)} extra={sorted(got-EXPECTED)}")

spec = importlib.util.spec_from_file_location("cmp_p54", ROOT/"tools/compare_elf_functions.py")
mod = importlib.util.module_from_spec(spec)
sys.modules["cmp_p54"] = mod
spec.loader.exec_module(mod)
ELFFile = mod.ELFFile
compare_function = mod.compare_function

insn = re.compile(
    r"^\s*([0-9A-Fa-f]+):\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
)
bm = {}
conflicts = set()
for listing in sorted((ROOT/"analysis/functions").glob("*.asm")):
    for line in listing.read_text(encoding="utf-8", errors="replace").splitlines():
        m = insn.match(line)
        if not m: continue
        a = int(m.group(1),16)
        bs = bytes(int(m.group(i),16) for i in range(2,6))
        for i,b in enumerate(bs):
            p=a+i
            if p in bm and bm[p] != b: conflicts.add(p)
            else: bm[p]=b
for p in conflicts: bm.pop(p,None)
end=max(bm)+1
target=bytearray(end-TARGET_BASE)
for a,b in bm.items():
    if a>=TARGET_BASE: target[a-TARGET_BASE]=b
target=bytes(target)

def safe(s): return re.sub(r"[^A-Za-z0-9_.-]+","_",s)
def screen_obj(row):
    stem=safe(row["source"].replace("/","__"))
    return ROOT/"build/matching/progress54-200plus-screen2/objects"/f'{stem}.{row["compiler"]}.{row["profile"]}.o'
def hist_obj(row):
    return ROOT/"build/matching/progress54-historical-per-object/objects"/f'{row["group"]}__{row["macro"]}__{row["profile"]}.o'

validated=[]
for kind, rows, path_fn in (("screen2",srows,screen_obj),("historical-per-object",hrows,hist_obj)):
    for row in rows:
        addr=int(row["address"],16)
        obj=path_fn(row)
        if not obj.exists(): die(f"missing object for {row['address']}: {obj}")
        elf=ELFFile(obj)
        sym_name=row["object_symbol"]
        sym=elf.find_symbol(sym_name)
        size=int(row["object_size"])
        if sym.size != size:
            die(f"object size changed for {row['address']}: evidence={size} object={sym.size}")
        comp=compare_function(target, addr-TARGET_BASE, size, elf, sym_name)
        if not comp.matching or comp.unknown_relocation_types:
            die(f"strict revalidation failed for {row['address']}")
        validated.append({
            "address":f"0x{addr:08x}",
            "name":progress[addr]["name"],
            "area":progress[addr]["area"],
            "gate":kind,
            "source":row.get("source","historical PS2DEV per-object"),
            "profile":row["profile"],
            "object_symbol":sym_name,
            "object_size":size,
            "boundary":row.get("boundary",""),
        })

if len(validated)!=98 or len({r["address"] for r in validated})!=98:
    die("validated set is not exactly 98 unique functions")

fields=["address","name","area","gate","source","profile","object_symbol","object_size","boundary"]
with (OUT/"validated-98.tsv").open("w",newline="",encoding="utf-8") as f:
    w=csv.DictWriter(f,fieldnames=fields,delimiter="\t",lineterminator="\n")
    w.writeheader()
    w.writerows(sorted(validated,key=lambda r:int(r["address"],16)))

print("Progress54 evidence validator: OK (98/98)")
print("  Screen2: 82/82")
print("  historical per-object: 16/16")
print("  checkpoint if promoted: 200/1041")
