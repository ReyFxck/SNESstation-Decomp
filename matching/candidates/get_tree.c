/*
 * First compiler-fingerprint candidate for get_tree @ 0x0018c124.
 *
 * This deliberately preserves the 1992 K&R definition and expression order
 * found in iaddis/SNESticle commit 9590ebf, path
 * Gep/Source/common/unzip/explode.c.  That source is paired with a release
 * EE GCC 3.2.2-b1 GAS listing.  The function is renamed only so this isolated
 * experiment cannot collide with the behavioral reconstruction.
 *
 * It is a matching candidate, not a MATCHING claim.
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
