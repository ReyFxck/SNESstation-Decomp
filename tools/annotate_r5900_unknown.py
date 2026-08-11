#!/usr/bin/env python3
"""Annotate R5900 instructions that LLVM's generic MIPS decoder prints unknown.

This is intentionally conservative: only encodings verified in the SNES
Station EE code are rewritten. Original bytes/address remain untouched.
"""
import argparse, re

REG = ["zero","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7",
       "s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","fp","ra"]
LINE = re.compile(r'^(\s*[0-9a-f]+:\s+)((?:[0-9a-f]{2} ){3}[0-9a-f]{2})(\s+)<unknown>(.*)$', re.I)

def s16(x):
    return x - 0x10000 if x & 0x8000 else x

def decode(w):
    op=(w>>26)&0x3f; rs=(w>>21)&31; rt=(w>>16)&31; rd=(w>>11)&31; sa=(w>>6)&31; fn=w&63
    if op == 0 and fn == 0x18 and rd != 0:
        return f"mult\t${REG[rd]}, ${REG[rs]}, ${REG[rt]}  # R5900 3-operand"
    if w == 0x42000038: return "ei\t# R5900"
    if w == 0x42000039: return "di\t# R5900"
    if op == 0x1e:
        return f"lq\t${REG[rt]}, {s16(w & 0xffff):#x}(${REG[rs]})"
    if op == 0x1f:
        return f"sq\t${REG[rt]}, {s16(w & 0xffff):#x}(${REG[rs]})"
    if op == 0x1c:
        if fn == 0x12: return f"mflo1\t${REG[rd]}"
        if fn == 0x13: return f"mtlo1\t${REG[rs]}"
        if fn == 0x19: return f"multu1\t${REG[rs]}, ${REG[rt]}"
        if fn == 0x1b: return f"divu1\t${REG[rs]}, ${REG[rt]}"
    if op == 0x11 and rs == 0x10 and fn == 0x04:
        # Unary COP1: source is bits 20..16, destination is bits 10..6.
        return f"sqrt.s\t$f{sa}, $f{rt}"
    return None

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('input')
    ap.add_argument('output')
    ap.add_argument('--start', type=lambda x:int(x,0))
    ap.add_argument('--end', type=lambda x:int(x,0))
    args=ap.parse_args()
    count=0
    with open(args.input, errors='replace') as fi, open(args.output,'w') as fo:
        for line in fi:
            m=LINE.match(line.rstrip('\n'))
            if not m:
                fo.write(line); continue
            addr=int(m.group(1).split(':')[0].strip(),16)
            if args.start is not None and addr < args.start:
                fo.write(line); continue
            if args.end is not None and addr >= args.end:
                fo.write(line); continue
            raw=bytes.fromhex(m.group(2)); w=int.from_bytes(raw,'little')
            text=decode(w)
            if text:
                fo.write(m.group(1)+m.group(2)+m.group(3)+text+m.group(4)+'\n'); count += 1
            else:
                fo.write(line)
    print(f"annotated {count} instructions")

if __name__ == '__main__': main()
