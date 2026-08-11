#!/usr/bin/env python3
import csv, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ASM = ROOT/'asm/full.asm'
STRINGS = ROOT/'notes/strings.txt'
OUT = ROOT/'notes/string_xrefs.csv'
BASE = 0x00100000

# strings -a -t x output: hex_offset whitespace text
strings = []
for line in STRINGS.read_text(errors='replace').splitlines():
    m = re.match(r'\s*([0-9a-fA-F]+)\s+(.*)$', line)
    if not m: continue
    off = int(m.group(1), 16)
    s = m.group(2)
    if len(s) < 3: continue
    strings.append((BASE+off, s))
strings.sort()
by_addr = {a:s for a,s in strings}

regs = {}
rows=[]
insn_re = re.compile(r'^\s*([0-9a-fA-F]+):\s+(?:[0-9a-fA-F]{2}\s+){0,8}\s*([^\s]+)\s*(.*)$')
reg_re = re.compile(r'\$(?:zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra|[0-9]+)')

names_num = {
    '$0':'$zero','$2':'$v0','$3':'$v1','$4':'$a0','$5':'$a1','$6':'$a2','$7':'$a3',
    '$8':'$t0','$9':'$t1','$10':'$t2','$11':'$t3','$12':'$t4','$13':'$t5','$14':'$t6','$15':'$t7',
    '$16':'$s0','$17':'$s1','$18':'$s2','$19':'$s3','$20':'$s4','$21':'$s5','$22':'$s6','$23':'$s7',
    '$24':'$t8','$25':'$t9','$28':'$gp','$29':'$sp','$30':'$fp','$31':'$ra'
}
def canon(r): return names_num.get(r,r)
def imm(x):
    x=x.strip()
    neg=x.startswith('-')
    if neg: x=x[1:]
    v=int(x,0)
    return -v if neg else v

def maybe_string(val):
    # Exact starts only; enough to find literal loads.
    return by_addr.get(val)

for line in ASM.read_text(errors='replace').splitlines():
    m=insn_re.match(line)
    if not m: continue
    pc=int(m.group(1),16); op=m.group(2); args=m.group(3).strip()
    # report known string-valued source registers before mutation
    seen=set()
    for r0 in reg_re.findall(args):
        r=canon(r0)
        if r in seen: continue
        seen.add(r)
        val=regs.get(r)
        if val is not None:
            s=maybe_string(val & 0xffffffff)
            if s is not None:
                rows.append((pc,r,val & 0xffffffff,op,s))

    # Minimal constant propagation
    parts=[p.strip() for p in args.split(',')] if args else []
    try:
        if op=='lui' and len(parts)>=2:
            rd=canon(parts[0]); regs[rd]=(imm(parts[1]) & 0xffff)<<16
        elif op in ('addiu','addi') and len(parts)>=3:
            rd,rs=canon(parts[0]),canon(parts[1]); iv=imm(parts[2])
            if rs in regs:
                regs[rd]=(regs[rs]+iv)&0xffffffff
            else: regs.pop(rd,None)
        elif op=='ori' and len(parts)>=3:
            rd,rs=canon(parts[0]),canon(parts[1]); iv=imm(parts[2])
            if rs in regs: regs[rd]=(regs[rs]|(iv&0xffff))&0xffffffff
            else: regs.pop(rd,None)
        elif op=='move' and len(parts)>=2:
            rd,rs=canon(parts[0]),canon(parts[1]);
            if rs=='$zero': regs[rd]=0
            elif rs in regs: regs[rd]=regs[rs]
            else: regs.pop(rd,None)
        elif op in ('addu','or') and len(parts)>=3:
            rd,rs,rt=canon(parts[0]),canon(parts[1]),canon(parts[2])
            if op=='addu' and rs in regs and rt in regs: regs[rd]=(regs[rs]+regs[rt])&0xffffffff
            elif op=='or' and rs in regs and rt=='$zero': regs[rd]=regs[rs]
            elif op=='or' and rt in regs and rs=='$zero': regs[rd]=regs[rt]
            else: regs.pop(rd,None)
        else:
            # Kill destination for common instructions that write first operand.
            if parts and op not in ('sw','sb','sh','sd','beq','bne','beqz','bnez','beql','bnel','b','j','jal','jr','syscall','nop'):
                rd=canon(parts[0])
                if rd.startswith('$') and rd not in ('$zero',): regs.pop(rd,None)
    except Exception:
        pass

# de-duplicate exact observations
seen=set(); uniq=[]
for row in rows:
    key=row[:4]
    if key not in seen:
        seen.add(key); uniq.append(row)

with OUT.open('w',newline='') as f:
    w=csv.writer(f); w.writerow(['pc','register','string_address','instruction','string'])
    for pc,r,a,op,s in uniq:
        w.writerow([f'0x{pc:08x}',r,f'0x{a:08x}',op,s])
print(f'wrote {len(uniq)} xrefs to {OUT}')
