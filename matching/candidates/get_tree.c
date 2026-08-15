/*
 * Historical-source model for get_tree @ 0x0018c124.
 *
 * This deliberately preserves the 1992 K&R definition and expression order
 * found in iaddis/SNESticle commit
 * 9590ebf3bf768424ebd6cb018f322e724a7aade3, path
 * Gep/Source/common/unzip/explode.c.
 *
 * The paired surviving SNESticle release_EE3.2.2-b1/explode.lst proves that
 * this historical C shape compiles to a 208-byte get_tree with that compiler
 * build.  SNES Station's target is 212 bytes and uses a different register
 * allocation / bytebuf-base lifetime.  Do not deform this C to compensate for
 * the compiler fingerprint: matching/candidates/get_tree.S records the exact
 * target instructions for the formal matcher.
 *
 * This C file remains the readable provenance/source model; the .S file is a
 * clearly labelled matching reconstruction, not Hiryu's claimed original asm.
 */

typedef unsigned short get_tree_uword;

extern get_tree_uword bytebuf;
extern int ReadByte(get_tree_uword *value);

int get_tree_candidate(l, n)
unsigned *l;
unsigned n;
{
    unsigned i;
    unsigned k;
    unsigned j;
    unsigned b;

    ReadByte(&bytebuf);
    i = bytebuf + 1;
    k = 0;
    do {
        ReadByte(&bytebuf);
        b = ((j = bytebuf) & 0xf) + 1;
        j = ((j & 0xf0) >> 4) + 1;
        if (k + j > n)
            return 4;
        do {
            l[k++] = b;
        } while (--j);
    } while (--i);
    return k != n ? 4 : 0;
}
