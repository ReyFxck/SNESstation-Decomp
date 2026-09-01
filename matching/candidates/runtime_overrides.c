/*
 * Target-selected runtime overrides, reconstructed from the V41/V47 bodies.
 * These are not claims about the original source filename or archive origin.
 * The private gate proves the historical incoming named relocations and links
 * both complete bodies at their target addresses, including fioWrite's JAL.
 *
 * Do not substitute standard libc semantics: target puts does not append a
 * newline, ignores the write result, and returns the measured string length.
 */
extern int fioWrite(int fd, void *buffer, int size);

/* 0x00107578: selected by fourteen abort relocations in the unwind members. */
void abort(void)
{
    for (;;) {}
}

/* 0x0019e414: selected by the preserved weak PS2LIB termination body. */
int puts(const char *text)
{
    const char *cursor = text;
    int length = 0;
    while (*cursor++)
        length++;
    fioWrite(1, (void *)text, length);
    return length;
}
