/* Historical Snes9x cheat-patch parser with the SNES Station fio adapter. */

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned char bool8;

enum { PATH_BUFFER_SIZE = 1024 };
enum { S9X_ERROR = 4, S9X_GAME_GENIE_CODE_ERROR = 7 };

extern "C" {
void split_path_00105ae8(const char *, char *, char *, char *, char *);
void _makepath(char *, const char *, const char *, const char *, const char *);
int fioOpen(const char *, int);
char *fioGets(int, char *, int);
int fioClose(int);
char *strrchr(const char *, int);
}
extern const char patch_extension[] __asm__("DAT_001b83e0");

const char *S9xGameGenieToRaw(const char *, uint32 &, uint8 &);
const char *S9xProActionReplayToRaw(const char *, uint32 &, uint8 &);
const char *S9xGoldFingerToRaw(const char *, uint32 &, bool8 &, uint8 &,
                              uint8 *);
void S9xAddCheat(bool8, bool8, uint32, uint8);
void S9xMessage(int, int, const char *);

void S9xParseCheatsFile(const char *rom_filename)
{
    int f;
    char dir[PATH_BUFFER_SIZE];
    char drive[1];
    char name[PATH_BUFFER_SIZE];
    char ext[PATH_BUFFER_SIZE];
    char fname[PATH_BUFFER_SIZE];
    char buf[80];
    uint32 address;
    uint8 byte;
    uint8 bytes[3];
    bool8 sram;
    uint8 num_bytes;
    const char *error;
    char *p;

    split_path_00105ae8(rom_filename, drive, dir, name, ext);
    _makepath(fname, drive, dir, name, patch_extension);

    if ((f = fioOpen(fname, 1)) >= 0)
    {
        while (fioGets(f, buf, 80) != 0)
        {
            if ((p = strrchr(buf, '\n')) != 0)
                *p = '\0';
            if (((error = S9xGameGenieToRaw(buf, address, byte)) == 0) ||
                ((error = S9xProActionReplayToRaw(buf, address, byte)) == 0))
            {
                S9xAddCheat(1, 0, address, byte);
            }
            else if ((error = S9xGoldFingerToRaw(
                          buf, address, sram, num_bytes, bytes)) == 0)
            {
                for (int c = 0; c < num_bytes; c++)
                    S9xAddCheat(1, 0, address + c, bytes[c]);
            }
            else
            {
                S9xMessage(S9X_ERROR, S9X_GAME_GENIE_CODE_ERROR, error);
            }
        }
        fioClose(f);
    }
}
