/* Original CMemory::Safe implementation with its persistent storage owned by
 * the canonical program-data image. */
class CMemory
{
public:
    char *Safe(const char *);
};

extern char *memory_safe_buffer __asm__("DAT_0042e820");
extern int memory_safe_length __asm__("DAT_003f4910");
extern "C" unsigned int strlen(const char *);
extern "C" void free(void *);
extern "C" void *malloc(unsigned int);

char *CMemory::Safe(const char *s)
{
    if (s == 0)
    {
        if (memory_safe_buffer != 0)
        {
            free(memory_safe_buffer);
            memory_safe_buffer = 0;
        }
        return 0;
    }

    int len = strlen(s);
    if (!memory_safe_buffer || len + 1 > memory_safe_length)
    {
        if (memory_safe_buffer)
            free(memory_safe_buffer);
        memory_safe_buffer = (char *) malloc(memory_safe_length = len + 1);
    }

    for (int i = 0; i < len; i++)
    {
        if (s[i] >= 32 && s[i] < 127)
            memory_safe_buffer[i] = s[i];
        else
            memory_safe_buffer[i] = '?';
    }
    memory_safe_buffer[len] = 0;
    return memory_safe_buffer;
}
