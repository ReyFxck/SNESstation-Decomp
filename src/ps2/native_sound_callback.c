/* Native SNES Station PS2 frontend callback.
 * Snes9x 1.41-1 port.h declares this entry with a C ABI and no arguments.
 * The original PS2 callback performs no work; its eight target bytes are
 * independently frozen in hunt1000plus-v41-validated-28.tsv.
 */

/* 0x00101904: original frontend S9xGenerateSound entry. */
void S9xGenerateSound(void) {}
