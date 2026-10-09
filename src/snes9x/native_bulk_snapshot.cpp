/*******************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2003 Gary Henderson (gary.henderson@ntlworld.com) and
                            Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2003 Matthew Kendora and
                            Brad Jorsch (anomie@users.sourceforge.net)



  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003 zsKnight (zsknight@zsnes.com),
                            _Demo_ (_demo_@zsnes.com), and
                            Nach (n-a-c-h@users.sourceforge.net)

  C4 C++ code
  (c) Copyright 2003 Brad Jorsch

  DSP-1 emulator code
  (c) Copyright 1998 - 2003 Ivar (ivar@snes9x.com), _Demo_, Gary Henderson,
                            John Weidman (jweidman@slip.net),
                            neviksti (neviksti@hotmail.com), and
                            Kris Bleakley (stinkfish@bigpond.com)

  DSP-2 emulator code
  (c) Copyright 2003 Kris Bleakley, John Weidman, neviksti, Matthew Kendora, and
                     Lord Nightmare (lord_nightmare@users.sourceforge.net

  OBC1 emulator code
  (c) Copyright 2001 - 2003 zsKnight, pagefault (pagefault@zsnes.com)
  Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002 Matthew Kendora with research by
                     zsKnight, John Weidman, and Dark Force

  S-RTC C emulator code
  (c) Copyright 2001 John Weidman

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003 zsKnight, _Demo_, and pagefault

  Super FX C emulator code
  (c) Copyright 1997 - 1999 Ivar and Gary Henderson.




  Specific ports contains the works of other authors. See headers in
  individual files.

  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and distribute Snes9x in both binary and
  source form, for non-commercial purposes, is hereby granted without fee,
  providing that this license information and copyright notice appear with
  all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes
  charging money for Snes9x or software derived from Snes9x.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
*******************************************************************************/

/* Native bulk proof: analysis/functions/native_bulk_snapshot_exact_5368.tsv
 * 10 routines / 5368 complete linked historical instruction bytes.
 * Reviewed native entry points:
 * 0x0019c5cc Snapshot (28 bytes)
 * 0x001711e8 S9xFreezeGame (72 bytes)
 * 0x00171348 _Z6FreezePv (952 bytes)
 * 0x0017124c S9xUnfreezeGame (252 bytes)
 * 0x00171700 _Z8UnfreezePv (1768 bytes)
 * 0x00171e0c _Z12FreezeStructPvPcS_P10FreezeDatai (872 bytes)
 * 0x001725ac _Z13UnfreezeBlockPvPcPhi (320 bytes)
 * 0x001721ec _Z14UnfreezeStructPvPcS_P10FreezeDatai (960 bytes)
 * 0x00173c24 _Z12S9xFixCyclesv (108 bytes)
 * 0x00171de8 _Z10FreezeSizeii (36 bytes)
 */
/* Pinned Snes9x native snapshot recovery; original declarations/macros and bodies expanded with the historical EE profile. */
extern "C" {
typedef int __int32_t;
typedef unsigned int __uint32_t;
extern "C" {
typedef long _off_t;
typedef long _ssize_t;
typedef __uint32_t __ULong;


struct _glue
{
  struct _glue *_next;
  int _niobs;
  struct __sFILE *_iobs;
};

struct _Bigint
{
  struct _Bigint *_next;
  int _k, _maxwds, _sign, _wds;
  __ULong _x[1];
};


struct __tm
{
  int __tm_sec;
  int __tm_min;
  int __tm_hour;
  int __tm_mday;
  int __tm_mon;
  int __tm_year;
  int __tm_wday;
  int __tm_yday;
  int __tm_isdst;
};







struct _atexit {
        struct _atexit *_next;
        int _ind;
        void (*_fns[32])(void);
};
struct __sbuf {
        unsigned char *_base;
        int _size;
};






typedef long _fpos_t;
struct __sFILE {
  unsigned char *_p;
  int _r;
  int _w;
  short _flags;
  short _file;
  struct __sbuf _bf;
  int _lbfsize;


  void * _cookie;

  int (*_read) (void * _cookie, char *_buf, int _n);
  int (*_write) (void * _cookie, const char *_buf, int _n);

  _fpos_t (*_seek) (void * _cookie, _fpos_t _offset, int _whence);
  int (*_close) (void * _cookie);


  struct __sbuf _ub;
  unsigned char *_up;
  int _ur;


  unsigned char _ubuf[3];
  unsigned char _nbuf[1];


  struct __sbuf _lb;


  int _blksize;
  int _offset;

  struct _reent *_data;
};
struct _rand48 {
  unsigned short _seed[3];
  unsigned short _mult[3];
  unsigned short _add;
};
struct _reent
{

  int _errno;




  struct __sFILE *_stdin, *_stdout, *_stderr;

  int _inc;
  char _emergency[25];

  int _current_category;
  const char *_current_locale;

  int __sdidinit;

  void (*__cleanup) (struct _reent *);


  struct _Bigint *_result;
  int _result_k;
  struct _Bigint *_p5s;
  struct _Bigint **_freelist;


  int _cvtlen;
  char *_cvtbuf;

  union
    {
      struct
        {
          unsigned int _unused_rand;
          char * _strtok_last;
          char _asctime_buf[26];
          struct __tm _localtime_buf;
          int _gamma_signgam;
          __extension__ unsigned long long _rand_next;
          struct _rand48 _r48;
        } _reent;



      struct
        {

          unsigned char * _nextf[30];
          unsigned int _nmalloc[30];
        } _unused;
    } _new;


  struct _atexit *_atexit;
  struct _atexit _atexit0;


  void (**(_sig_func))(int);




  struct _glue __sglue;
  struct __sFILE __sf[3];
};
extern struct _reent *_impure_ptr ;

void _reclaim_reent (struct _reent *);
}
typedef long unsigned int size_t;
void * snes_hidden_memchr (const void *, int, size_t);
int snes_hidden_memcmp (const void *, const void *, size_t);
void * snes_hidden_memcpy (void *, const void *, size_t);
void * snes_hidden_memmove (void *, const void *, size_t);
void * snes_hidden_memset (void *, int, size_t);
char *snes_hidden_strcat (char *, const char *);
char *snes_hidden_strchr (const char *, int);
int snes_hidden_strcmp (const char *, const char *);
int strcoll (const char *, const char *);
char *snes_hidden_strcpy (char *, const char *);
size_t strcspn (const char *, const char *);
char *strerror (int);
size_t snes_hidden_strlen (const char *);
char *snes_hidden_strncat (char *, const char *, size_t);
int snes_hidden_strncmp (const char *, const char *, size_t);
char *snes_hidden_strncpy (char *, const char *, size_t);
char *strpbrk (const char *, const char *);
char *snes_hidden_strrchr (const char *, int);
size_t strspn (const char *, const char *);
char *strstr (const char *, const char *);


char *strtok (char *, const char *);


size_t strxfrm (char *, const char *, size_t);


char *strtok_r (char *, const char *, char **);

int bcmp (const char *, const char *, size_t);
void bcopy (const char *, char *, size_t);
void bzero (char *, size_t);
int ffs (int);
char *index (const char *, int);
void * memccpy (void *, const void *, int, size_t);
char *rindex (const char *, int);
int strcasecmp (const char *, const char *);
char *strdup (const char *);
char *_strdup_r (struct _reent *, const char *);
int strncasecmp (const char *, const char *, size_t);
char *strsep (char **, const char *);
char *strlwr (char *);
char *strupr (char *);
}
extern "C" {
void *memchr(const void *, int, unsigned int);
int memcmp(const void *, const void *, unsigned int);
void *memcpy(void *, const void *, unsigned int);
void *memmove(void *, const void *, unsigned int);
void *memset(void *, int, unsigned int);
char *strcat(char *, const char *);
char *strchr(const char *, int);
int strcmp(const char *, const char *);
char *strcpy(char *, const char *);
unsigned int strlen(const char *);
char *strncat(char *, const char *, unsigned int);
int strncmp(const char *, const char *, unsigned int);
char *strncpy(char *, const char *, unsigned int);
char *strrchr(const char *, int);
}







extern "C" {





int isalnum (int __c);
int isalpha (int __c);
int iscntrl (int __c);
int isdigit (int __c);
int isgraph (int __c);
int islower (int __c);
int isprint (int __c);
int ispunct (int __c);
int isspace (int __c);
int isupper (int __c);
int isxdigit (int __c);
int tolower (int __c);
int toupper (int __c);


int isascii (int __c);
int toascii (int __c);
int _tolower (int __c);
int _toupper (int __c);
extern const char _ctype_[];
}
extern "C" {
typedef struct
{
  int quot;
  int rem;
} div_t;

typedef struct
{
  long quot;
  long rem;
} ldiv_t;
extern int __mb_cur_max;



void abort (void) __attribute__ ((noreturn));
int abs (int);
int atexit (void (*__func)(void));
double atof (const char *__nptr);

float atoff (const char *__nptr);

int atoi (const char *__nptr);
long atol (const char *__nptr);
void * bsearch (const void * __key, const void * __base, size_t __nmemb, size_t __size, int (* _compar) (const void *, const void *));




void * calloc (size_t __nmemb, size_t __size);
div_t div (int __numer, int __denom);
void exit (int __status) __attribute__ ((noreturn));
void free (void *);
char * getenv (const char *__string);
char * _getenv_r (struct _reent *, const char *__string);
char * _findenv (const char *, int *);
char * _findenv_r (struct _reent *, const char *, int *);
long labs (long);
ldiv_t ldiv (long __numer, long __denom);
void * malloc (size_t __size);
int mblen (const char *, size_t);
int _mblen_r (struct _reent *, const char *, size_t, int *);
int mbtowc (wchar_t *, const char *, size_t);
int _mbtowc_r (struct _reent *, wchar_t *, const char *, size_t, int *);
int wctomb (char *, wchar_t);
int _wctomb_r (struct _reent *, char *, wchar_t, int *);
size_t mbstowcs (wchar_t *, const char *, size_t);
size_t _mbstowcs_r (struct _reent *, wchar_t *, const char *, size_t, int *);
size_t wcstombs (char *, const wchar_t *, size_t);
size_t _wcstombs_r (struct _reent *, char *, const wchar_t *, size_t, int *);


int mkstemp (char *);
char * mktemp (char *);


void qsort (void * __base, size_t __nmemb, size_t __size, int(*_compar)(const void *, const void *));
int rand (void);
void * realloc (void * __r, size_t __size);
void srand (unsigned __seed);
double strtod (const char *__n, char **__end_PTR);
double _strtod_r (struct _reent *,const char *__n, char **__end_PTR);

float strtodf (const char *__n, char **__end_PTR);

long strtol (const char *__n, char **__end_PTR, int __base);
long _strtol_r (struct _reent *,const char *__n, char **__end_PTR, int __base);
unsigned long strtoul (const char *__n, char **__end_PTR, int __base);
unsigned long _strtoul_r (struct _reent *,const char *__n, char **__end_PTR, int __base);

int system (const char *__string);


int putenv (const char *__string);
int _putenv_r (struct _reent *, const char *__string);
int setenv (const char *__string, const char *__value, int __overwrite);
int _setenv_r (struct _reent *, const char *__string, const char *__value, int __overwrite);

char * gcvt (double,int,char *);
char * gcvtf (float,int,char *);
char * fcvt (double,int,int *,int *);
char * fcvtf (float,int,int *,int *);
char * ecvt (double,int,int *,int *);
char * ecvtbuf (double, int, int*, int*, char *);
char * fcvtbuf (double, int, int*, int*, char *);
char * ecvtf (float,int,int *,int *);
char * dtoa (double, int, int, int *, int*, char**);
int rand_r (unsigned *__seed);

double drand48 (void);
double _drand48_r (struct _reent *);
double erand48 (unsigned short [3]);
double _erand48_r (struct _reent *, unsigned short [3]);
long jrand48 (unsigned short [3]);
long _jrand48_r (struct _reent *, unsigned short [3]);
void lcong48 (unsigned short [7]);
void _lcong48_r (struct _reent *, unsigned short [7]);
long lrand48 (void);
long _lrand48_r (struct _reent *);
long mrand48 (void);
long _mrand48_r (struct _reent *);
long nrand48 (unsigned short [3]);
long _nrand48_r (struct _reent *, unsigned short [3]);
unsigned short *
       seed48 (unsigned short [3]);
unsigned short *
       _seed48_r (struct _reent *, unsigned short [3]);
void srand48 (long);
void _srand48_r (struct _reent *, long);
long long strtoll (const char *__n, char **__end_PTR, int __base);
long long _strtoll_r (struct _reent *, const char *__n, char **__end_PTR, int __base);
unsigned long long strtoull (const char *__n, char **__end_PTR, int __base);
unsigned long long _strtoull_r (struct _reent *, const char *__n, char **__end_PTR, int __base);


void cfree (void *);
char * _dtoa_r (struct _reent *, double, int, int, int *, int*, char**);
void * _malloc_r (struct _reent *, size_t);
void * _calloc_r (struct _reent *, size_t, size_t);
void _free_r (struct _reent *, void *);
void * _realloc_r (struct _reent *, void *, size_t);
void _mstats_r (struct _reent *, char *);
int _system_r (struct _reent *, const char *);

void __eprintf (const char *, const char *, unsigned int, const char *);


}
extern "C" {
typedef __builtin_va_list __gnuc_va_list;
typedef _fpos_t fpos_t;

typedef struct __sFILE FILE;
FILE * tmpfile (void);
char * tmpnam (char *);
int fclose (FILE *);
int fflush (FILE *);
FILE * freopen (const char *, const char *, FILE *);
void setbuf (FILE *, char *);
int setvbuf (FILE *, char *, int, size_t);
int fprintf (FILE *, const char *, ...);
int fscanf (FILE *, const char *, ...);
int printf (const char *, ...);
int scanf (const char *, ...);
int sscanf (const char *, const char *, ...);
int vfprintf (FILE *, const char *, __gnuc_va_list);
int vprintf (const char *, __gnuc_va_list);
int vsprintf (char *, const char *, __gnuc_va_list);
int fgetc (FILE *);
char * fgets (char *, int, FILE *);
int fputc (int, FILE *);
int fputs (const char *, FILE *);
int getc (FILE *);
int getchar (void);
char * gets (char *);
int putc (int, FILE *);
int putchar (int);
int puts (const char *);
int ungetc (int, FILE *);
size_t snes_hidden_fread (void *, size_t _size, size_t _n, FILE *);
size_t snes_hidden_fwrite (const void * , size_t _size, size_t _n, FILE *);
int fgetpos (FILE *, fpos_t *);
int fseek (FILE *, long, int);
int fsetpos (FILE *, const fpos_t *);
long ftell ( FILE *);
void rewind (FILE *);
void clearerr (FILE *);
int feof (FILE *);
int ferror (FILE *);
void perror (const char *);

FILE * fopen (const char *_name, const char *_type);
int sprintf (char *, const char *, ...);
int remove (const char *);
int rename (const char *, const char *);


int vfiprintf (FILE *, const char *, __gnuc_va_list);
int iprintf (const char *, ...);
int fiprintf (FILE *, const char *, ...);
int siprintf (char *, const char *, ...);
char * tempnam (const char *, const char *);
int vsnprintf (char *, size_t, const char *, __gnuc_va_list);
int vfscanf (FILE *, const char *, __gnuc_va_list);
int vscanf (const char *, __gnuc_va_list);
int vsscanf (const char *, const char *, __gnuc_va_list);

int snprintf (char *, size_t, const char *, ...);
FILE * fdopen (int, const char *);

int fileno (FILE *);
int getw (FILE *);
int pclose (FILE *);
FILE * popen (const char *, const char *);
int putw (int, FILE *);
void setbuffer (FILE *, char *, int);
int setlinebuf (FILE *);






FILE * _fdopen_r (struct _reent *, int, const char *);
FILE * _fopen_r (struct _reent *, const char *, const char *);
int _fscanf_r (struct _reent *, FILE *, const char *, ...);
int _getchar_r (struct _reent *);
char * _gets_r (struct _reent *, char *);
int _iprintf_r (struct _reent *, const char *, ...);
int _mkstemp_r (struct _reent *, char *);
char * _mktemp_r (struct _reent *, char *);
void _perror_r (struct _reent *, const char *);
int _printf_r (struct _reent *, const char *, ...);
int _putchar_r (struct _reent *, int);
int _puts_r (struct _reent *, const char *);
int _remove_r (struct _reent *, const char *);
int _rename_r (struct _reent *, const char *_old, const char *_new);

int _scanf_r (struct _reent *, const char *, ...);
int _sprintf_r (struct _reent *, char *, const char *, ...);
int _snprintf_r (struct _reent *, char *, size_t, const char *, ...);
int _sscanf_r (struct _reent *, const char *, const char *, ...);
char * _tempnam_r (struct _reent *, const char *, const char *);
FILE * _tmpfile_r (struct _reent *);
char * _tmpnam_r (struct _reent *, char *);
int _vfprintf_r (struct _reent *, FILE *, const char *, __gnuc_va_list);
int _vprintf_r (struct _reent *, const char *, __gnuc_va_list);
int _vsprintf_r (struct _reent *, char *, const char *, __gnuc_va_list);
int _vsnprintf_r (struct _reent *, char *, size_t, const char *, __gnuc_va_list);
int _vfscanf_r (struct _reent *, FILE *, const char *, __gnuc_va_list);
int _vscanf_r (struct _reent *, const char *, __gnuc_va_list);
int _vsscanf_r (struct _reent *, const char *, const char *, __gnuc_va_list);





int __srget (FILE *);
int __swbuf (int, FILE *);






FILE *funopen (const void * _cookie, int (*readfn)(void * _cookie, char *_buf, int _n), int (*writefn)(void * _cookie, const char *_buf, int _n), fpos_t (*seekfn)(void * _cookie, fpos_t _off, int _whence), int (*closefn)(void * _cookie));
}




extern "C" {
unsigned int fread(void *, unsigned int, unsigned int, FILE *);
unsigned int fwrite(const void *, unsigned int, unsigned int, FILE *);
}
typedef long int ptrdiff_t;
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;



typedef unsigned short ushort;
typedef unsigned int uint;



typedef unsigned long clock_t;




typedef long time_t;




struct timespec {
  time_t tv_sec;
  long tv_nsec;
};

struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;
};


typedef long daddr_t;
typedef char * caddr_t;







typedef unsigned short ino_t;
typedef short dev_t;


typedef long off_t;

typedef unsigned short uid_t;
typedef unsigned short gid_t;
typedef int pid_t;
typedef long key_t;
typedef long ssize_t;
typedef unsigned int mode_t __attribute__ ((__mode__ (__SI__)));



typedef unsigned short nlink_t;
typedef long fd_mask;







typedef struct _types_fd_set {
        fd_mask fds_bits[(((64)+(((sizeof (fd_mask) * 8))-1))/((sizeof (fd_mask) * 8)))];
} _types_fd_set;
typedef unsigned char bool8;


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef signed char int8;
typedef short int16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
void _makepath (char *path, const char *drive, const char *dir,
                const char *fname, const char *ext);
void _splitpath (const char *path, char *drive, char *dir, char *fname,
                 char *ext);





extern "C" void S9xGenerateSound ();
typedef union
{

    struct { uint8 l,h; } B;



    uint16 W;
} pair;

struct SRegisters{
    uint8 PB;
    uint8 DB;
    pair P;
    pair A;
    pair D;
    pair S;
    pair X;
    pair Y;
    uint16 PC;
};

extern "C" struct SRegisters Registers __asm__("DAT_003453a8");
enum {
    S9X_TRACE,
    S9X_DEBUG,
    S9X_WARNING,
    S9X_INFO,
    S9X_ERROR,
    S9X_FATAL_ERROR
};


enum {
    S9X_ROM_INFO,
    S9X_HEADERS_INFO,
    S9X_ROM_CONFUSING_FORMAT_INFO,
    S9X_ROM_INTERLEAVED_INFO,
    S9X_SOUND_DEVICE_OPEN_FAILED,
    S9X_APU_STOPPED,
    S9X_USAGE,
    S9X_GAME_GENIE_CODE_ERROR,
    S9X_ACTION_REPLY_CODE_ERROR,
    S9X_GOLD_FINGER_CODE_ERROR,
    S9X_DEBUG_OUTPUT,
    S9X_DMA_TRACE,
    S9X_HDMA_TRACE,
    S9X_WRONG_FORMAT,
    S9X_WRONG_VERSION,
    S9X_ROM_NOT_FOUND,
    S9X_FREEZE_FILE_NOT_FOUND,
    S9X_PPU_TRACE,
    S9X_TRACE_DSP1,
    S9X_FREEZE_ROM_NAME,
    S9X_HEADER_WARNING,
    S9X_NETPLAY_NOT_SERVER,
    S9X_FREEZE_FILE_INFO,
    S9X_TURBO_MODE
};
typedef unsigned char Byte;


typedef unsigned int uInt;
typedef unsigned long uLong;






   typedef Byte Bytef;

typedef char charf;
typedef int intf;
typedef uInt uIntf;
typedef uLong uLongf;


   typedef void *voidpf;
   typedef void *voidp;
extern "C" {
typedef voidpf (*alloc_func) (voidpf opaque, uInt items, uInt size);
typedef void (*free_func) (voidpf opaque, voidpf address);

struct internal_state;

typedef struct z_stream_s {
    Bytef *next_in;
    uInt avail_in;
    uLong total_in;

    Bytef *next_out;
    uInt avail_out;
    uLong total_out;

    char *msg;
    struct internal_state *state;

    alloc_func zalloc;
    free_func zfree;
    voidpf opaque;

    int data_type;
    uLong adler;
    uLong reserved;
} z_stream;

typedef z_stream *z_streamp;
extern const char * zlibVersion (void);
extern int deflate (z_streamp strm, int flush);
extern int deflateEnd (z_streamp strm);
extern int inflate (z_streamp strm, int flush);
extern int inflateEnd (z_streamp strm);
extern int deflateSetDictionary (z_streamp strm, const Bytef *dictionary, uInt dictLength);
extern int deflateCopy (z_streamp dest, z_streamp source);
extern int deflateReset (z_streamp strm);
extern int deflateParams (z_streamp strm, int level, int strategy);
extern int inflateSetDictionary (z_streamp strm, const Bytef *dictionary, uInt dictLength);
extern int inflateSync (z_streamp strm);
extern int inflateReset (z_streamp strm);
extern int compress (Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
extern int compress2 (Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level);
extern int uncompress (Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
typedef voidp gzFile;

extern gzFile gzopen (const char *path, const char *mode);
extern gzFile gzdopen (int fd, const char *mode);
extern int gzsetparams (gzFile file, int level, int strategy);







extern int gzread (gzFile file, voidp buf, unsigned len);







extern int gzwrite (gzFile file, const voidp buf, unsigned len);







extern int gzprintf (gzFile file, const char *format, ...);






extern int gzputs (gzFile file, const char *s);






extern char * gzgets (gzFile file, char *buf, int len);
extern int gzputc (gzFile file, int c);





extern int gzgetc (gzFile file);





extern int gzflush (gzFile file, int flush);
extern long gzseek (gzFile file, long offset, int whence);
extern int gzrewind (gzFile file);






extern long gztell (gzFile file);
extern int gzeof (gzFile file);





extern int gzclose (gzFile file);






extern const char * gzerror (gzFile file, int *errnum);
extern uLong adler32 (uLong adler, const Bytef *buf, uInt len);
extern uLong crc32 (uLong crc, const Bytef *buf, uInt len);
extern int deflateInit_ (z_streamp strm, int level, const char *version, int stream_size);

extern int inflateInit_ (z_streamp strm, const char *version, int stream_size);

extern int deflateInit2_ (z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char *version, int stream_size);



extern int inflateInit2_ (z_streamp strm, int windowBits, const char *version, int stream_size);
    struct internal_state {int dummy;};


extern const char * zError (int err);
extern int inflateSyncPoint (z_streamp z);
extern const uLongf * get_crc_table (void);


}
enum {
    SNES_MULTIPLAYER5,
    SNES_JOYPAD,
    SNES_MOUSE_SWAPPED,
    SNES_MOUSE,
    SNES_SUPERSCOPE,
        SNES_JUSTIFIER,
        SNES_JUSTIFIER_2,
    SNES_MAX_CONTROLLER_OPTIONS
};
struct SCPUState{
    uint32 Flags;
    bool8 BranchSkip;
    bool8 NMIActive;
    bool8 IRQActive;
    bool8 WaitingForInterrupt;
    bool8 InDMA;
    uint8 WhichEvent;
    uint8 *PC;
    uint8 *PCBase;
    uint8 *PCAtOpcodeStart;
    uint8 *WaitAddress;
    uint32 WaitCounter;
    long Cycles;
    long NextEvent;
    long V_Counter;
    long MemSpeed;
    long MemSpeedx2;
    long FastROMSpeed;
    uint32 AutoSaveTimer;
    bool8 SRAMModified;
    uint32 NMITriggerPoint;
    bool8 BRKTriggered;
    bool8 TriedInterleavedMode2;
    uint32 NMICycleCount;
    uint32 IRQCycleCount;
};







struct SSettings{

    bool8 APUEnabled;
    bool8 Shutdown;
    uint8 SoundSkipMethod;
    long H_Max;
    long HBlankStart;
    long CyclesPercentage;
    bool8 DisableIRQ;
    bool8 Paused;
    bool8 ForcedPause;
    bool8 StopEmulation;


    bool8 TraceDMA;
    bool8 TraceHDMA;
    bool8 TraceVRAM;
    bool8 TraceUnknownRegisters;
    bool8 TraceDSP;


    bool8 SwapJoypads;
    bool8 JoystickEnabled;


    bool8 ForcePAL;
    bool8 ForceNTSC;
    bool8 PAL;
    uint32 FrameTimePAL;
    uint32 FrameTimeNTSC;
    uint32 FrameTime;
    uint32 SkipFrames;


    bool8 ForceLoROM;
    bool8 ForceHiROM;
    bool8 ForceHeader;
    bool8 ForceNoHeader;
    bool8 ForceInterleaved;
    bool8 ForceInterleaved2;
    bool8 ForceNotInterleaved;


    bool8 ForceSuperFX;
    bool8 ForceNoSuperFX;
    bool8 ForceDSP1;
    bool8 ForceNoDSP1;
    bool8 ForceSA1;
    bool8 ForceNoSA1;
    bool8 ForceC4;
    bool8 ForceNoC4;
    bool8 ForceSDD1;
    bool8 ForceNoSDD1;
    bool8 MultiPlayer5;
    bool8 Mouse;
    bool8 SuperScope;
    bool8 SRTC;
    uint32 ControllerOption;

    bool8 ShutdownMaster;
    bool8 MultiPlayer5Master;
    bool8 SuperScopeMaster;
    bool8 MouseMaster;
    bool8 SuperFX;
    bool8 DSP1Master;
    bool8 SA1;
    bool8 C4;
    bool8 SDD1;
        bool8 SPC7110;
        bool8 SPC7110RTC;
        bool8 OBC1;


    uint32 SoundPlaybackRate;
    bool8 TraceSoundDSP;
    bool8 Stereo;
    bool8 ReverseStereo;
    bool8 SixteenBitSound;
    int SoundBufferSize;
    int SoundMixInterval;
    bool8 SoundEnvelopeHeightReading;
    bool8 DisableSoundEcho;
    bool8 DisableSampleCaching;
    bool8 DisableMasterVolume;
    bool8 SoundSync;
    bool8 InterpolatedSound;
    bool8 ThreadSound;
    bool8 Mute;
    bool8 NextAPUEnabled;
    uint8 AltSampleDecode;
    bool8 FixFrequency;


    bool8 SixteenBit;
    bool8 Transparency;
    bool8 SupportHiRes;
    bool8 Mode7Interpolate;


    bool8 BGLayering;
    bool8 DisableGraphicWindows;
    bool8 ForceTransparency;
    bool8 ForceNoTransparency;
    bool8 DisableHDMA;
    bool8 DisplayFrameRate;
    bool8 DisableRangeTimeOver;


    bool8 NetPlay;
    bool8 NetPlayServer;
    char ServerName [128];
    int Port;
    bool8 GlideEnable;
    bool8 OpenGLEnable;
    int32 AutoSaveDelay;
    bool8 ApplyCheats;
    bool8 TurboMode;
    uint32 TurboSkipFrames;
    uint32 AutoMaxSkipFrames;



    bool8 PS2PortLayoutByte;
    bool8 ChuckRock;
    bool8 StarfoxHack;
    bool8 WinterGold;
    bool8 Dezaemon;

    bool8 BS;
    bool8 DaffyDuck;
    uint8 APURAMInitialValue;
    bool8 SampleCatchup;
        bool8 JustifierMaster;
        bool8 Justifier;
        bool8 SecondJustifier;
        int8 SETA;
    bool8 TakeScreenshot;
    int8 StretchScreenshots;
        uint16 DisplayColor;
    int SoundDriver;
    int AIDOShmId;
};

struct SSNESGameFixes
{
    uint8 NeedInit0x2137;

    uint8 alienVSpredetorFix;
    uint8 APU_OutPorts_ReturnValueFix;



    uint8 SoundEnvelopeHeightReading2;
    uint8 SRAMInitialValue;
        uint8 Uniracers;
        uint8 Flintstones;
};

extern "C" {
extern struct SSettings Settings __asm__("DAT_003454e0");
extern struct SCPUState CPU __asm__("g_CPU_blob");
extern struct SSNESGameFixes SNESGameFixes;
extern char String [513] __asm__("DAT_00345268-0x208");

void S9xExit ();
void S9xMessage (int type, int number, const char *message);
void S9xLoadSDD1Data ();
}

enum {
    PAUSE_NETPLAY_CONNECT = (1 << 0),
    PAUSE_TOGGLE_FULL_SCREEN = (1 << 1),
    PAUSE_EXIT = (1 << 2),
    PAUSE_MENU = (1 << 3),
    PAUSE_INACTIVE_WINDOW = (1 << 4),
    PAUSE_WINDOW_ICONISED = (1 << 5),
    PAUSE_RESTORE_GUI = (1 << 6),
    PAUSE_FREEZE_FILE = (1 << 7)
};
void S9xSetPause (uint32 mask);
void S9xClearPause (uint32 mask);
extern "C" {
bool8 S9xFreezeGame (const char *filename);
bool8 S9xUnfreezeGame (const char *filename);
bool8 Snapshot (const char *filename);
bool8 S9xLoadSnapshot (const char *filename);
bool8 S9xSPCDump (const char *filename);
}
extern "C" bool8 S9xLoadOrigSnapshot (const char *filename);

struct SOrigCPUState{
    uint32 Flags;
    short Cycles_old;
    short NextEvent_old;
    uint8 CurrentFrame;
    uint8 FastROMSpeed_old_old;
    uint16 V_Counter_old;
    bool8 BranchSkip;
    bool8 NMIActive;
    bool8 IRQActive;
    bool8 WaitingForInterrupt;
    bool8 InDMA;
    uint8 WhichEvent;
    uint8 *PC;
    uint8 *PCBase;
    uint16 MemSpeed_old;
    uint16 MemSpeedx2_old;
    uint16 FastROMSpeed_old;
    bool8 FastDP;
    uint8 *PCAtOpcodeStart;
    uint8 *WaitAddress;
    uint32 WaitCounter;
    long Cycles;
    long NextEvent;
    long V_Counter;
    long MemSpeed;
    long MemSpeedx2;
    long FastROMSpeed;
};

struct SOrigAPU
{
    uint32 Cycles;
    bool8 ShowROM;
    uint8 Flags;
    uint8 KeyedChannels;
    uint8 OutPorts [4];
    uint8 DSP [0x80];
    uint8 ExtraRAM [64];
    uint16 Timer [3];
    uint16 TimerTarget [3];
    bool8 TimerEnabled [3];
    bool8 TimerValueWritten [3];
};

typedef union
{

    struct { uint8 A, Y; } B;



    uint16 W;
} OrigYAndA;

struct SOrigAPURegisters{
    uint8 P;
    OrigYAndA YA;
    uint8 X;
    uint8 S;
    uint16 PC;
};




typedef struct {
    int state;
    int type;
    short volume_left;
    short volume_right;
    int frequency;
    int count;
    signed short wave [(1024 * 4)];
    bool8 loop;
    int envx;
    short left_vol_level;
    short right_vol_level;
    short envx_target;
    unsigned long int env_error;
    unsigned long erate;
    int direction;
    unsigned long attack_rate;
    unsigned long decay_rate;
    unsigned long sustain_rate;
    unsigned long release_rate;
    unsigned long sustain_level;
    signed short sample;
    signed short decoded [16];
    signed short previous [2];
    uint16 sample_number;
    bool8 last_block;
    bool8 needs_decode;
    uint32 block_pointer;
    uint32 sample_pointer;
    int *echo_buf_ptr;
    int mode;
    uint32 dummy [8];
} OrigChannel;

typedef struct
{
    short master_volume_left;
    short master_volume_right;
    short echo_volume_left;
    short echo_volume_right;
    int echo_enable;
    int echo_feedback;
    int echo_ptr;
    int echo_buffer_size;
    int echo_write_enabled;
    int echo_channel_enable;
    int pitch_mod;

    uint32 dummy [3];
    OrigChannel channels [8];
} SOrigSoundData;

struct SOrigOBJ
{
    short HPos;
    uint16 VPos;
    uint16 Name;
    uint8 VFlip;
    uint8 HFlip;
    uint8 Priority;
    uint8 Palette;
    uint8 Size;
    uint8 Prev;
    uint8 Next;
};

struct SOrigPPU {
    uint8 BGMode;
    uint8 BG3Priority;
    uint8 Brightness;

    struct {
        bool8 High;
        uint8 Increment;
        uint16 Address;
        uint16 Mask1;
        uint16 FullGraphicCount;
        uint16 Shift;
    } VMA;

    struct {
        uint8 TileSize;
        uint16 TileAddress;
        uint8 Width;
        uint8 Height;
        uint16 SCBase;
        uint16 VOffset;
        uint16 HOffset;
        bool8 ThroughMain;
        bool8 ThroughSub;
        uint8 BGSize;
        uint16 NameBase;
        uint16 SCSize;
        bool8 Addition;
    } BG [4];

    bool8 CGFLIP;
    uint16 CGDATA [256];
    uint8 FirstSprite;
    uint8 LastSprite;
    struct SOrigOBJ OBJ [129];
    uint8 OAMPriorityRotation;
    uint16 OAMAddr;

    uint8 OAMFlip;
    uint16 OAMTileAddress;
    uint16 IRQVBeamPos;
    uint16 IRQHBeamPos;
    uint16 VBeamPosLatched;
    uint16 HBeamPosLatched;

    uint8 HBeamFlip;
    uint8 VBeamFlip;
    uint8 HVBeamCounterLatched;

    short MatrixA;
    short MatrixB;
    short MatrixC;
    short MatrixD;
    short CentreX;
    short CentreY;
    uint8 Joypad1ButtonReadPos;
    uint8 Joypad2ButtonReadPos;

    uint8 CGADD;
    uint8 FixedColourRed;
    uint8 FixedColourGreen;
    uint8 FixedColourBlue;
    uint16 SavedOAMAddr;
    uint16 ScreenHeight;
    uint32 WRAM;
    uint8 BG_Forced;
    bool8 ForcedBlanking;
    bool8 OBJThroughMain;
    bool8 OBJThroughSub;
    uint8 OBJSizeSelect;
    uint8 OBJNameSelect_old;
    uint16 OBJNameBase;
    bool8 OBJAddition;
    uint8 OAMReadFlip;
    uint8 OAMData [512 + 32];
    bool8 VTimerEnabled;
    bool8 HTimerEnabled;
    short HTimerPosition;
    uint8 Mosaic;
    bool8 BGMosaic [4];
    bool8 Mode7HFlip;
    bool8 Mode7VFlip;
    uint8 Mode7Repeat;
    uint8 Window1Left;
    uint8 Window1Right;
    uint8 Window2Left;
    uint8 Window2Right;
    uint8 ClipCounts [6];
    uint8 ClipLeftEdges [3][6];
    uint8 ClipRightEdges [3][6];
    uint8 ClipWindowOverlapLogic [6];
    uint8 ClipWindow1Enable [6];
    uint8 ClipWindow2Enable [6];
    bool8 ClipWindow1Inside [6];
    bool8 ClipWindow2Inside [6];
    bool8 RecomputeClipWindows;
    uint8 CGFLIPRead;
    uint16 OBJNameSelect;
    bool8 Need16x8Mulitply;
    uint8 Joypad3ButtonReadPos;
    uint8 MouseSpeed[2];
};

struct SOrigDMA {
    bool8 TransferDirection;
    bool8 AAddressFixed;
    bool8 AAddressDecrement;
    uint8 TransferMode;

    uint8 ABank;
    uint16 AAddress;
    uint16 Address;
    uint8 BAddress;


    uint16 TransferBytes;


    bool8 HDMAIndirectAddressing;
    uint16 IndirectAddress;
    uint8 IndirectBank;
    uint8 Repeat;
    uint8 LineCount;
    uint8 FirstLine;
    bool8 JustStarted;
};

typedef union
{

    struct { uint8 l,h; } B;



    uint16 W;
} OrigPair;

struct SOrigRegisters{
    uint8 PB;
    uint8 DB;
    OrigPair P;
    OrigPair A;
    OrigPair D;
    OrigPair S;
    OrigPair X;
    OrigPair Y;
    uint16 PC;
};
class CMemory {
public:
    bool8 LoadROM (const char *);
    void InitROM (bool8);
    bool8 LoadSRAM (const char *);
    bool8 SaveSRAM (const char *);
    bool8 Init ();
    void Deinit ();
    void FreeSDD1Data ();

    void WriteProtectROM ();
    void FixROMSpeed ();
    void MapRAM ();
    void MapExtraRAM ();
    char *Safe (const char *);

        void BSLoROMMap();
        void JumboLoROMMap ();
    void LoROMMap ();
    void LoROM24MBSMap ();
    void SRAM512KLoROMMap ();

    void SufamiTurboLoROMMap ();
    void HiROMMap ();
    void SuperFXROMMap ();
    void TalesROMMap (bool8);
    void AlphaROMMap ();
    void SA1ROMMap ();
    void BSHiROMMap ();
        void SPC7110HiROMMap();
        void SPC7110Sram(uint8);
    bool8 AllASCII (uint8 *b, int size);
    int ScoreHiROM (bool8 skip_header);
    int ScoreLoROM (bool8 skip_header);
    void ApplyROMFixes ();
    void CheckForIPSPatch (const char *rom_filename, bool8 header,
                           int32 &rom_size);

    const char *TVStandard ();
    const char *Speed ();
    const char *StaticRAMSize ();
    const char *MapType ();
    const char *MapMode ();
    const char *KartContents ();
    const char *Size ();
    const char *Headers ();
    const char *ROMID ();
    const char *CompanyID ();
    enum {
        MAP_PPU, MAP_CPU, MAP_DSP, MAP_LOROM_SRAM, MAP_HIROM_SRAM,
        MAP_NONE, MAP_DEBUG, MAP_C4, MAP_BWRAM, MAP_BWRAM_BITMAP,
        MAP_BWRAM_BITMAP2, MAP_SA1RAM, MAP_SPC7110_ROM, MAP_SPC7110_DRAM,
        MAP_RONLY_SRAM, MAP_OBC_RAM, MAP_SETA_DSP, MAP_SETA_RISC, MAP_LAST
    };
    enum { MAX_ROM_SIZE = 0x800000 };

    uint8 *RAM;
    uint8 *ROM;
    uint8 *VRAM;
    uint8 *SRAM;
    uint8 *BWRAM;
    uint8 *FillRAM;
    uint8 *C4RAM;
    bool8 HiROM;
    bool8 LoROM;
    uint32 SRAMMask;
    uint8 SRAMSize;
    uint8 *Map [(0x1000000 / (0x1000))];
    uint8 *WriteMap [(0x1000000 / (0x1000))];
    uint8 MemorySpeed [(0x1000000 / (0x1000))];
    uint8 BlockIsRAM [(0x1000000 / (0x1000))];
    uint8 BlockIsROM [(0x1000000 / (0x1000))];
    char ROMName [23];
    char ROMId [5];
    char CompanyId [3];
    uint8 ROMSpeed;
    uint8 ROMType;
    uint8 ROMSize;
    int32 ROMFramesPerSecond;
    int32 HeaderCount;
    uint32 CalculatedSize;
    uint32 CalculatedChecksum;
    uint32 ROMChecksum;
    uint32 ROMComplementChecksum;
    uint8 *SDD1Index;
    uint8 *SDD1Data;
    uint32 SDD1Entries;
    uint32 SDD1LoggedDataCountPrev;
    uint32 SDD1LoggedDataCount;
    uint8 SDD1LoggedData [(0x10000 / 8)];
    char ROMFilename [1024];
        uint8 ROMRegion;
    uint32 ROMCRC32;
        uint8 *BSRAM;



};

extern "C" {
extern CMemory Memory __asm__("DAT_0034e2b0");
extern uint8 *SRAM __asm__("DAT_0034e298");
extern uint8 *ROM;
extern uint8 *RegRAM;
void S9xDeinterleaveMode2 ();
}

void S9xAutoSaveSRAM ();


uint8 S9xGetByte (uint32 Address);
uint16 S9xGetWord (uint32 Address);
void S9xSetByte (uint8 Byte, uint32 Address);
void S9xSetWord (uint16 Byte, uint32 Address);
void S9xSetPCBase (uint32 Address);
uint8 *S9xGetMemPointer (uint32 Address);
uint8 *GetBasePointer (uint32 Address);

extern "C"{
extern uint8 OpenBus;
}
extern uint8 GetBank;
extern uint16 SignExtend [2];
struct ClipData {
    uint32 Count [6];
    uint32 Left [6][6];
    uint32 Right [6][6];
};

struct InternalPPU {
    bool8 ColorsChanged;
    uint8 HDMA;
    bool8 HDMAStarted;
    uint8 MaxBrightness;
    bool8 LatchedBlanking;
    bool8 OBJChanged;
    bool8 RenderThisFrame;
    bool8 DirectColourMapsNeedRebuild;
    uint32 FrameCount;
    uint32 RenderedFramesCount;
    uint32 DisplayedRenderedFrameCount;
    uint32 SkippedFrames;
    uint32 FrameSkip;
    uint8 *TileCache [3];
    uint8 *TileCached [3];
    bool8 FirstVRAMRead;
    bool8 DoubleHeightPixels;
    bool8 Interlace;
    bool8 InterlaceSprites;
    bool8 DoubleWidthPixels;
    int RenderedScreenHeight;
    int RenderedScreenWidth;
    uint32 Red [256];
    uint32 Green [256];
    uint32 Blue [256];
    uint8 *XB;
    uint16 ScreenColors [256];
    int PreviousLine;
    int CurrentLine;
    int Controller;
    uint32 Joypads[5];
    uint32 SuperScope;
    uint32 Mouse[2];
    int PrevMouseX[2];
    int PrevMouseY[2];
    struct ClipData Clip [2];
};

struct SOBJ
{
    short HPos;
    uint16 VPos;
    uint16 Name;
    uint8 VFlip;
    uint8 HFlip;
    uint8 Priority;
    uint8 Palette;
    uint8 Size;
};

struct SPPU {
    uint8 BGMode;
    uint8 BG3Priority;
    uint8 Brightness;

    struct {
        bool8 High;
        uint8 Increment;
        uint16 Address;
        uint16 Mask1;
        uint16 FullGraphicCount;
        uint16 Shift;
    } VMA;

    struct {
        uint16 SCBase;
        uint16 VOffset;
        uint16 HOffset;
        uint8 BGSize;
        uint16 NameBase;
        uint16 SCSize;
    } BG [4];

    bool8 CGFLIP;
    uint16 CGDATA [256];
    uint8 FirstSprite;
    uint8 LastSprite;
    struct SOBJ OBJ [128];
    uint8 OAMPriorityRotation;
    uint16 OAMAddr;
    uint8 RangeTimeOver;

    uint8 OAMFlip;
    uint16 OAMTileAddress;
    uint16 IRQVBeamPos;
    uint16 IRQHBeamPos;
    uint16 VBeamPosLatched;
    uint16 HBeamPosLatched;

    uint8 HBeamFlip;
    uint8 VBeamFlip;
    uint8 HVBeamCounterLatched;

    short MatrixA;
    short MatrixB;
    short MatrixC;
    short MatrixD;
    short CentreX;
    short CentreY;
    uint8 Joypad1ButtonReadPos;
    uint8 Joypad2ButtonReadPos;

    uint8 CGADD;
    uint8 FixedColourRed;
    uint8 FixedColourGreen;
    uint8 FixedColourBlue;
    uint16 SavedOAMAddr;
    uint16 ScreenHeight;
    uint32 WRAM;
    uint8 BG_Forced;
    bool8 ForcedBlanking;
    bool8 OBJThroughMain;
    bool8 OBJThroughSub;
    uint8 OBJSizeSelect;
    uint16 OBJNameBase;
    bool8 OBJAddition;
    uint8 OAMReadFlip;
    uint8 OAMData [512 + 32];
    bool8 VTimerEnabled;
    bool8 HTimerEnabled;
    short HTimerPosition;
    uint8 Mosaic;
    bool8 BGMosaic [4];
    bool8 Mode7HFlip;
    bool8 Mode7VFlip;
    uint8 Mode7Repeat;
    uint8 Window1Left;
    uint8 Window1Right;
    uint8 Window2Left;
    uint8 Window2Right;
    uint8 ClipCounts [6];
    uint8 ClipWindowOverlapLogic [6];
    uint8 ClipWindow1Enable [6];
    uint8 ClipWindow2Enable [6];
    bool8 ClipWindow1Inside [6];
    bool8 ClipWindow2Inside [6];
    bool8 RecomputeClipWindows;
    uint8 CGFLIPRead;
    uint16 OBJNameSelect;
    bool8 Need16x8Mulitply;
    uint8 Joypad3ButtonReadPos;
    uint8 MouseSpeed[2];


    uint16 SavedOAMAddr2;
    uint16 OAMWriteRegister;
    uint8 BGnxOFSbyte;
        uint8 OpenBus;
};






struct SDMA {
    bool8 TransferDirection;
    bool8 AAddressFixed;
    bool8 AAddressDecrement;
    uint8 TransferMode;

    uint8 ABank;
    uint16 AAddress;
    uint16 Address;
    uint8 BAddress;


    uint16 TransferBytes;


    bool8 HDMAIndirectAddressing;
    uint16 IndirectAddress;
    uint8 IndirectBank;
    uint8 Repeat;
    uint8 LineCount;
    uint8 FirstLine;
};

extern "C" {
void S9xUpdateScreen ();
void S9xResetPPU ();
void S9xSoftResetPPU ();
void S9xFixColourBrightness ();
void S9xUpdateJoypads ();
void S9xProcessMouse(int which1);
void S9xSuperFXExec ();

void S9xSetPPU (uint8 Byte, uint16 Address);
uint8 S9xGetPPU (uint16 Address);
void S9xSetCPU (uint8 Byte, uint16 Address);
uint8 S9xGetCPU (uint16 Address);

void S9xInitC4 ();
void S9xSetC4 (uint8 Byte, uint16 Address);
uint8 S9xGetC4 (uint16 Address);
void S9xSetC4RAM (uint8 Byte, uint16 Address);
uint8 S9xGetC4RAM (uint16 Address);

extern struct SPPU PPU __asm__("DAT_0035b788");
extern struct SDMA DMA [8] __asm__("DAT_0035d360");
extern struct InternalPPU IPPU __asm__("DAT_0035c268");
}
struct SGFX{

    uint8 *Screen;
    uint8 *SubScreen;
    uint8 *ZBuffer;
    uint8 *SubZBuffer;
    uint32 Pitch;


    int Delta;
    uint16 *X2;
    uint16 *ZERO_OR_X2;
    uint16 *ZERO;
    uint32 RealPitch;
    uint32 Pitch2;
    uint32 ZPitch;
    uint32 PPL;
    uint32 PPLx2;
    uint32 PixSize;
    uint8 *S;
    uint8 *DB;
    uint16 *ScreenColors;
    uint32 DepthDelta;
    uint8 Z1;
    uint8 Z2;
    uint8 ZSprite;
    uint32 FixedColour;
    const char *InfoString;
    uint32 InfoStringTimeout;
    uint32 StartY;
    uint32 EndY;
    struct ClipData *pCurrentClip;
    uint32 Mode7Mask;
    uint32 Mode7PriorityMask;
    int OBJList [129];
    int32 OveredOBJs [239];
    uint32 Sizes [129];
    int8 VisibleTiles [129];
    int VPositions [129];

    uint8 r212c;
    uint8 r212d;
    uint8 r2130;
    uint8 r2131;
    bool8 Pseudo;







};

struct SLineData {
    struct {
        uint16 VOffset;
        uint16 HOffset;
    } BG [4];
};





struct SBG
{
    uint32 TileSize;
    uint32 BitShift;
    uint32 TileShift;
    uint32 TileAddress;
    uint32 NameSelect;
    uint32 SCBase;

    uint32 StartPalette;
    uint32 PaletteShift;
    uint32 PaletteMask;

    uint8 *Buffer;
    uint8 *Buffered;
    bool8 DirectColourMode;
};

struct SLineMatrixData
{
    short MatrixA;
    short MatrixB;
    short MatrixC;
    short MatrixD;
    short CentreX;
    short CentreY;
};

extern uint32 odd_high [4][16];
extern uint32 odd_low [4][16];
extern uint32 even_high [4][16];
extern uint32 even_low [4][16];
extern SBG BG;
extern uint16 DirectColourMaps [8][256];

extern uint8 add32_32 [32][32];
extern uint8 add32_32_half [32][32];
extern uint8 sub32_32 [32][32];
extern uint8 sub32_32_half [32][32];
extern uint8 mul_brightness [16][32];
typedef void (*NormalTileRenderer) (uint32 Tile, uint32 Offset,
                                    uint32 StartLine, uint32 LineCount);
typedef void (*ClippedTileRenderer) (uint32 Tile, uint32 Offset,
                                     uint32 StartPixel, uint32 Width,
                                     uint32 StartLine, uint32 LineCount);
typedef void (*LargePixelRenderer) (uint32 Tile, uint32 Offset,
                                    uint32 StartPixel, uint32 Pixels,
                                    uint32 StartLine, uint32 LineCount);

extern "C" {
void S9xStartScreenRefresh ();
void S9xDrawScanLine (uint8 Line);
void S9xEndScreenRefresh ();
void S9xSetupOBJ (struct SOBJ *);
void S9xUpdateScreen ();
void RenderLine (uint8 line);
void S9xBuildDirectColourMaps ();



extern struct SGFX GFX;

bool8 S9xGraphicsInit ();
void S9xGraphicsDeinit();
bool8 S9xInitUpdate (void);
bool8 S9xDeinitUpdate (int Width, int Height, bool8 sixteen_bit);
void S9xSetPalette ();
void S9xSyncSpeed ();





}
static inline uint8 REGISTER_4212()
{
    GetBank = 0;
    if (CPU.V_Counter >= PPU.ScreenHeight + 1 &&
        CPU.V_Counter < PPU.ScreenHeight + 1 + 3)
        GetBank = 1;

    GetBank |= CPU.Cycles >= Settings.HBlankStart ? 0x40 : 0;
    if (CPU.V_Counter >= PPU.ScreenHeight + 1)
        GetBank |= 0x80;

    return (GetBank);
}

static inline void FLUSH_REDRAW ()
{
    if (IPPU.PreviousLine != IPPU.CurrentLine)
        S9xUpdateScreen ();
}

static inline void REGISTER_2104 (uint8 byte)
{
    if (PPU.OAMAddr & 0x100)
    {
        int addr = ((PPU.OAMAddr & 0x10f) << 1) + (PPU.OAMFlip & 1);
        if (byte != PPU.OAMData [addr]){
            FLUSH_REDRAW ();
            PPU.OAMData [addr] = byte;
            IPPU.OBJChanged = 1;


            struct SOBJ *pObj = &PPU.OBJ [(addr & 0x1f) * 4];

            pObj->HPos = (pObj->HPos & 0xFF) | SignExtend[(byte >> 0) & 1];
            pObj++->Size = byte & 2;
            pObj->HPos = (pObj->HPos & 0xFF) | SignExtend[(byte >> 2) & 1];
            pObj++->Size = byte & 8;
            pObj->HPos = (pObj->HPos & 0xFF) | SignExtend[(byte >> 4) & 1];
            pObj++->Size = byte & 32;
            pObj->HPos = (pObj->HPos & 0xFF) | SignExtend[(byte >> 6) & 1];
            pObj->Size = byte & 128;
        }
        PPU.OAMFlip ^= 1;
        if(!(PPU.OAMFlip & 1)){
            ++PPU.OAMAddr;
            PPU.OAMAddr &= 0x1ff;
        }
    } else if(!(PPU.OAMFlip & 1)){
        PPU.OAMWriteRegister &= 0xff00;
        PPU.OAMWriteRegister |= byte;
        PPU.OAMFlip |= 1;
    } else {
        PPU.OAMWriteRegister &= 0x00ff;
        uint8 lowbyte = (uint8)(PPU.OAMWriteRegister);
        uint8 highbyte = byte;
        PPU.OAMWriteRegister |= byte << 8;

        int addr = (PPU.OAMAddr << 1);

        if (lowbyte != PPU.OAMData [addr] ||
            highbyte != PPU.OAMData [addr+1])
        {
            FLUSH_REDRAW ();
            PPU.OAMData [addr] = lowbyte;
            PPU.OAMData [addr+1] = highbyte;
            IPPU.OBJChanged = 1;
            if (addr & 2)
            {

                PPU.OBJ[addr = PPU.OAMAddr >> 1].Name = PPU.OAMWriteRegister & 0x1ff;


                PPU.OBJ[addr].Palette = (highbyte >> 1) & 7;
                PPU.OBJ[addr].Priority = (highbyte >> 4) & 3;
                PPU.OBJ[addr].HFlip = (highbyte >> 6) & 1;
                PPU.OBJ[addr].VFlip = (highbyte >> 7) & 1;
            }
            else
            {

                PPU.OBJ[addr = PPU.OAMAddr >> 1].HPos &= 0xFF00;
                PPU.OBJ[addr].HPos |= lowbyte;


                PPU.OBJ[addr].VPos = highbyte;
            }
        }
        PPU.OAMFlip &= ~1;
        ++PPU.OAMAddr;
    }

    Memory.FillRAM [0x2104] = byte;
}

static inline void REGISTER_2118 (uint8 Byte)
{
    uint32 address;
    if (PPU.VMA.FullGraphicCount)
    {
        uint32 rem = PPU.VMA.Address & PPU.VMA.Mask1;
        address = (((PPU.VMA.Address & ~PPU.VMA.Mask1) +
                         (rem >> PPU.VMA.Shift) +
                         ((rem & (PPU.VMA.FullGraphicCount - 1)) << 3)) << 1) & 0xffff;
        Memory.VRAM [address] = Byte;
    }
    else
    {
        Memory.VRAM[address = (PPU.VMA.Address << 1) & 0xFFFF] = Byte;
    }
    IPPU.TileCached [0][address >> 4] = 0;
    IPPU.TileCached [1][address >> 5] = 0;
    IPPU.TileCached [2][address >> 6] = 0;
    if (!PPU.VMA.High)
    {
        PPU.VMA.Address += PPU.VMA.Increment;
    }

}

static inline void REGISTER_2118_tile (uint8 Byte)
{
    uint32 address;
    uint32 rem = PPU.VMA.Address & PPU.VMA.Mask1;
    address = (((PPU.VMA.Address & ~PPU.VMA.Mask1) +
                 (rem >> PPU.VMA.Shift) +
                 ((rem & (PPU.VMA.FullGraphicCount - 1)) << 3)) << 1) & 0xffff;
    Memory.VRAM [address] = Byte;
    IPPU.TileCached [0][address >> 4] = 0;
    IPPU.TileCached [1][address >> 5] = 0;
    IPPU.TileCached [2][address >> 6] = 0;
    if (!PPU.VMA.High)
        PPU.VMA.Address += PPU.VMA.Increment;

}

static inline void REGISTER_2118_linear (uint8 Byte)
{
    uint32 address;
    Memory.VRAM[address = (PPU.VMA.Address << 1) & 0xFFFF] = Byte;
    IPPU.TileCached [0][address >> 4] = 0;
    IPPU.TileCached [1][address >> 5] = 0;
    IPPU.TileCached [2][address >> 6] = 0;
    if (!PPU.VMA.High)
        PPU.VMA.Address += PPU.VMA.Increment;

}

static inline void REGISTER_2119 (uint8 Byte)
{
    uint32 address;
    if (PPU.VMA.FullGraphicCount)
    {
        uint32 rem = PPU.VMA.Address & PPU.VMA.Mask1;
        address = ((((PPU.VMA.Address & ~PPU.VMA.Mask1) +
                    (rem >> PPU.VMA.Shift) +
                    ((rem & (PPU.VMA.FullGraphicCount - 1)) << 3)) << 1) + 1) & 0xFFFF;
        Memory.VRAM [address] = Byte;
    }
    else
    {
        Memory.VRAM[address = ((PPU.VMA.Address << 1) + 1) & 0xFFFF] = Byte;
    }
    IPPU.TileCached [0][address >> 4] = 0;
    IPPU.TileCached [1][address >> 5] = 0;
    IPPU.TileCached [2][address >> 6] = 0;
    if (PPU.VMA.High)
    {
        PPU.VMA.Address += PPU.VMA.Increment;
    }

}

static inline void REGISTER_2119_tile (uint8 Byte)
{
    uint32 rem = PPU.VMA.Address & PPU.VMA.Mask1;
    uint32 address = ((((PPU.VMA.Address & ~PPU.VMA.Mask1) +
                    (rem >> PPU.VMA.Shift) +
                    ((rem & (PPU.VMA.FullGraphicCount - 1)) << 3)) << 1) + 1) & 0xFFFF;
    Memory.VRAM [address] = Byte;
    IPPU.TileCached [0][address >> 4] = 0;
    IPPU.TileCached [1][address >> 5] = 0;
    IPPU.TileCached [2][address >> 6] = 0;
    if (PPU.VMA.High)
        PPU.VMA.Address += PPU.VMA.Increment;

}

static inline void REGISTER_2119_linear (uint8 Byte)
{
    uint32 address;
    Memory.VRAM[address = ((PPU.VMA.Address << 1) + 1) & 0xFFFF] = Byte;
    IPPU.TileCached [0][address >> 4] = 0;
    IPPU.TileCached [1][address >> 5] = 0;
    IPPU.TileCached [2][address >> 6] = 0;
    if (PPU.VMA.High)
        PPU.VMA.Address += PPU.VMA.Increment;

}

static inline void REGISTER_2122(uint8 Byte)
{


    if (PPU.CGFLIP)
    {
        if ((Byte & 0x7f) != (PPU.CGDATA[PPU.CGADD] >> 8))
        {
            if (Settings.SixteenBit)
                FLUSH_REDRAW ();
            PPU.CGDATA[PPU.CGADD] &= 0x00FF;
            PPU.CGDATA[PPU.CGADD] |= (Byte & 0x7f) << 8;
            IPPU.ColorsChanged = 1;
            if (Settings.SixteenBit)
            {
                IPPU.Blue [PPU.CGADD] = IPPU.XB [(Byte >> 2) & 0x1f];
                IPPU.Green [PPU.CGADD] = IPPU.XB [(PPU.CGDATA[PPU.CGADD] >> 5) & 0x1f];
                IPPU.ScreenColors [PPU.CGADD] = (uint16) (((int) (IPPU.Blue [PPU.CGADD]) << 10) | ((int) (IPPU.Green [PPU.CGADD]) << 5) | (int) (IPPU.Red [PPU.CGADD]));


            }
        }
        PPU.CGADD++;
    }
    else
    {
        if (Byte != (uint8) (PPU.CGDATA[PPU.CGADD] & 0xff))
        {
            if (Settings.SixteenBit)
                FLUSH_REDRAW ();
            PPU.CGDATA[PPU.CGADD] &= 0x7F00;
            PPU.CGDATA[PPU.CGADD] |= Byte;
            IPPU.ColorsChanged = 1;
            if (Settings.SixteenBit)
            {
                IPPU.Red [PPU.CGADD] = IPPU.XB [Byte & 0x1f];
                IPPU.Green [PPU.CGADD] = IPPU.XB [(PPU.CGDATA[PPU.CGADD] >> 5) & 0x1f];
                IPPU.ScreenColors [PPU.CGADD] = (uint16) (((int) (IPPU.Blue [PPU.CGADD]) << 10) | ((int) (IPPU.Green [PPU.CGADD]) << 5) | (int) (IPPU.Red [PPU.CGADD]));


            }
        }
    }
    PPU.CGFLIP ^= 1;

}

static inline void REGISTER_2180(uint8 Byte)
{
    Memory.RAM[PPU.WRAM++] = Byte;
    PPU.WRAM &= 0x1FFFF;
    Memory.FillRAM [0x2180] = Byte;
}



void JustifierButtons(uint32&);
bool JustifierOffscreen();
struct SOpcodes {



        void (*S9xOpcode)( void);

};

struct SICPU
{
    uint8 *Speed;
    struct SOpcodes *S9xOpcodes;
    uint8 _Carry;
    uint8 _Zero;
    uint8 _Negative;
    uint8 _Overflow;
    bool8 CPUExecuting;
    uint32 ShiftedPB;
    uint32 ShiftedDB;
    uint32 Frame;
    uint32 Scanline;
    uint32 FrameAdvanceCount;
};

extern "C" {
void S9xMainLoop (void);
void S9xReset (void);
void S9xSoftReset (void);
void S9xDoHBlankProcessing ();
void S9xClearIRQ (uint32);
void S9xSetIRQ (uint32);

extern struct SOpcodes S9xOpcodesM1X1 [256];
extern struct SOpcodes S9xOpcodesM1X0 [256];
extern struct SOpcodes S9xOpcodesM0X1 [256];
extern struct SOpcodes S9xOpcodesM0X0 [256];
extern struct SICPU ICPU __asm__("g_ICPU_00345318");
}

static inline void S9xUnpackStatus()
{
    ICPU._Zero = (Registers.P.B.l & 2) == 0;
    ICPU._Negative = (Registers.P.B.l & 128);
    ICPU._Carry = (Registers.P.B.l & 1);
    ICPU._Overflow = (Registers.P.B.l & 64) >> 6;
}

static inline void S9xPackStatus()
{
    Registers.P.B.l &= ~(2 | 128 | 1 | 64);
    Registers.P.B.l |= ICPU._Carry | ((ICPU._Zero == 0) << 1) |
                    (ICPU._Negative & 0x80) | (ICPU._Overflow << 6);
}

static inline void CLEAR_IRQ_SOURCE (uint32 M)
{
    CPU.IRQActive &= ~M;
    if (!CPU.IRQActive)
        CPU.Flags &= ~(1 << 11);
}

static inline void S9xFixCycles ()
{
    if ((Registers.P.W & 256))
    {



        ICPU.S9xOpcodes = S9xOpcodesM1X1;
    }
    else
    if ((Registers.P.B.l & 32))
    {
        if ((Registers.P.B.l & 16))
        {



            ICPU.S9xOpcodes = S9xOpcodesM1X1;
        }
        else
        {



            ICPU.S9xOpcodes = S9xOpcodesM1X0;
        }
    }
    else
    {
        if ((Registers.P.B.l & 16))
        {



            ICPU.S9xOpcodes = S9xOpcodesM0X1;
        }
        else
        {



            ICPU.S9xOpcodes = S9xOpcodesM0X0;
        }
    }
}

static inline void S9xReschedule ()
{
    uint8 which;
    long max;

    if (CPU.WhichEvent == 0 ||
        CPU.WhichEvent == 3)
    {
        which = 1;
        max = Settings.H_Max;
    }
    else
    {
        which = 0;
        max = Settings.HBlankStart;
    }

    if (PPU.HTimerEnabled &&
        (long) PPU.HTimerPosition < max &&
        (long) PPU.HTimerPosition > CPU.NextEvent &&
        (!PPU.VTimerEnabled ||
         (PPU.VTimerEnabled && CPU.V_Counter == PPU.IRQVBeamPos)))
    {
        which = (long) PPU.HTimerPosition < Settings.HBlankStart ?
                        2 : 3;
        max = PPU.HTimerPosition;
    }
    CPU.NextEvent = max;
    CPU.WhichEvent = which;
}
extern "C" {

void S9xSetPalette ();
void S9xTextMode ();
void S9xGraphicsMode ();
char *S9xParseArgs (char **argv, int argc);
void S9xParseArg (char **argv, int &index, int argc);
void S9xExtraUsage ();
uint32 S9xReadJoypad (int which1_0_to_4);
bool8 S9xReadMousePosition (int which1_0_to_1, int &x, int &y, uint32 &buttons);
bool8 S9xReadSuperScopePosition (int &x, int &y, uint32 &buttons);

void S9xUsage ();
void S9xInitDisplay (int argc, char **argv);
void S9xDeinitDisplay ();
void S9xInitInputDevices ();
void S9xSetTitle (const char *title);
void S9xProcessEvents (bool8 block);
void S9xPutImage (int width, int height);
void S9xParseDisplayArg (char **argv, int &index, int argc);
void S9xToggleSoundChannel (int channel);
void S9xSetInfoString (const char *string);
int S9xMinCommandLineArgs ();
void S9xNextController ();
bool8 S9xLoadROMImage (const char *string);
const char *S9xSelectFilename (const char *def, const char *dir,
                               const char *ext, const char *title);

const char *S9xChooseFilename (bool8 read_only);
bool8 S9xOpenSnapshotFile (const char *base, bool8 read_only, gzFile *file);
void S9xCloseSnapshotFile (gzFile file);

const char *S9xBasename (const char *filename);

int S9xFStrcmp (FILE *, const char *);
const char *S9xGetHomeDirectory ();
const char *S9xGetSnapshotDirectory ();
const char *S9xGetROMDirectory ();
const char *S9xGetSRAMFilename ();
const char *S9xGetFilename (const char *extension);
const char *S9xGetFilenameInc (const char *);
}
typedef union
{

    struct { uint8 A, Y; } B;



    uint16 W;
} YAndA;

struct SAPURegisters{
    uint8 P;
    YAndA YA;
    uint8 X;
    uint8 S;
    uint16 PC;
};

extern "C" struct SAPURegisters APURegisters __asm__("DAT_003454d8");
struct SIAPU
{
    uint8 *PC;
    uint8 *RAM;
    uint8 *DirectPage;
    bool8 APUExecuting;
    uint8 Bit;
    uint32 Address;
    uint8 *WaitAddress1;
    uint8 *WaitAddress2;
    uint32 WaitCounter;
    uint8 *ShadowRAM;
    uint8 *CachedSamples;
    uint8 _Carry;
    uint8 _Zero;
    uint8 _Overflow;
    uint32 TimerErrorCounter;
    uint32 Scanline;
    int32 OneCycle;
    int32 TwoCycles;
};

struct SAPU
{
    int32 Cycles;
    bool8 ShowROM;
    uint8 Flags;
    uint8 KeyedChannels;
    uint8 OutPorts [4];
    uint8 DSP [0x80];
    uint8 ExtraRAM [64];
    uint16 Timer [3];
    uint16 TimerTarget [3];
    bool8 TimerEnabled [3];
    bool8 TimerValueWritten [3];
};

extern "C" struct SAPU APU __asm__("g_APU_003453b8");
extern "C" struct SIAPU IAPU __asm__("DAT_00345498");
extern int spc_is_dumping;
extern int spc_is_dumping_temp;
extern uint8 spc_dump_dsp[0x100];
static inline void S9xAPUUnpackStatus()
{
    IAPU._Zero = ((APURegisters.P & 2) == 0) | (APURegisters.P & 128);
    IAPU._Carry = (APURegisters.P & 1);
    IAPU._Overflow = (APURegisters.P & 64) >> 6;
}

static inline void S9xAPUPackStatus()
{
    APURegisters.P &= ~(2 | 128 | 1 | 64);
    APURegisters.P |= IAPU._Carry | ((IAPU._Zero == 0) << 1) |
                      (IAPU._Zero & 0x80) | (IAPU._Overflow << 6);
}

extern "C" {
void S9xResetAPU (void);
bool8 S9xInitAPU ();
void S9xDeinitAPU ();
void S9xDecacheSamples ();
int S9xTraceAPU ();
int S9xAPUOPrint (char *buffer, uint16 Address);
void S9xSetAPUControl (uint8 byte);
void S9xSetAPUDSP (uint8 byte);
uint8 S9xGetAPUDSP ();
void S9xSetAPUTimer (uint16 Address, uint8 byte);
bool8 S9xInitSound (int quality, bool8 stereo, int buffer_size);
void S9xOpenCloseSoundTracingFile (bool8);
void S9xPrintAPUState ();
extern int32 S9xAPUCycles [256];
extern int32 S9xAPUCycleLengths [256];
extern void (*S9xApuOpcodes [256]) (void);
}
enum { SOUND_SAMPLE = 0, SOUND_NOISE, SOUND_EXTRA_NOISE, SOUND_MUTE };
enum { SOUND_SILENT, SOUND_ATTACK, SOUND_DECAY, SOUND_SUSTAIN,
       SOUND_RELEASE, SOUND_GAIN, SOUND_INCREASE_LINEAR,
       SOUND_INCREASE_BENT_LINE, SOUND_DECREASE_LINEAR,
       SOUND_DECREASE_EXPONENTIAL};

enum { MODE_NONE = SOUND_SILENT, MODE_ADSR, MODE_RELEASE = SOUND_RELEASE,
       MODE_GAIN, MODE_INCREASE_LINEAR, MODE_INCREASE_BENT_LINE,
       MODE_DECREASE_LINEAR, MODE_DECREASE_EXPONENTIAL};
typedef struct {
    int sound_fd;
    int sound_switch;
    int playback_rate;
    int buffer_size;
    int noise_gen;
    bool8 mute_sound;
    int stereo;
    bool8 sixteen_bit;
    bool8 encoded;






    int32 samples_mixed_so_far;
    int32 play_position;
    uint32 err_counter;
    uint32 err_rate;
} SoundStatus;

extern "C" volatile SoundStatus so;

typedef struct {
    int state;
    int type;
    short volume_left;
    short volume_right;
    uint32 hertz;
    uint32 frequency;
    uint32 count;
    bool8 loop;
    int envx;
    short left_vol_level;
    short right_vol_level;
    short envx_target;
    unsigned long int env_error;
    unsigned long erate;
    int direction;
    unsigned long attack_rate;
    unsigned long decay_rate;
    unsigned long sustain_rate;
    unsigned long release_rate;
    unsigned long sustain_level;
    signed short sample;
    signed short decoded [16];
    signed short previous16 [2];
    signed short *block;
    uint16 sample_number;
    bool8 last_block;
    bool8 needs_decode;
    uint32 block_pointer;
    uint32 sample_pointer;
    int *echo_buf_ptr;
    int mode;
    int32 envxx;
    signed short next_sample;
    int32 interpolate;
    int32 previous [2];

    uint32 dummy [8];

} Channel;

typedef struct
{
    short master_volume_left;
    short master_volume_right;
    short echo_volume_left;
    short echo_volume_right;
    int echo_enable;
    int echo_feedback;
    int echo_ptr;
    int echo_buffer_size;
    int echo_write_enabled;
    int echo_channel_enable;
    int pitch_mod;

    uint32 dummy [3];
    Channel channels [8];
    bool8 no_filter;
    int master_volume [2];
    int echo_volume [2];
    int noise_hertz;
} SSoundData;

extern "C" SSoundData SoundData __asm__("DAT_0034db50");

void S9xSetSoundVolume (int channel, short volume_left, short volume_right);
void S9xSetSoundFrequency (int channel, int hertz);
void S9xSetSoundHertz (int channel, int hertz);
void S9xSetSoundType (int channel, int type_of_sound);
void S9xSetMasterVolume (short master_volume_left, short master_volume_right);
void S9xSetEchoVolume (short echo_volume_left, short echo_volume_right);
void S9xSetSoundControl (int sound_switch);
bool8 S9xSetSoundMute (bool8 mute);
void S9xSetEnvelopeHeight (int channel, int height);
void S9xSetSoundADSR (int channel, int attack, int decay, int sustain,
                      int sustain_level, int release);
void S9xSetSoundKeyOff (int channel);
void S9xSetSoundDecayMode (int channel);
void S9xSetSoundAttachMode (int channel);
void S9xSoundStartEnvelope (Channel *);
void S9xSetSoundSample (int channel, uint16 sample_number);
void S9xSetEchoFeedback (int echo_feedback);
void S9xSetEchoEnable (uint8 byte);
void S9xSetEchoDelay (int byte);
void S9xSetEchoWriteEnable (uint8 byte);
void S9xSetFilterCoefficient (int tap, int value);
void S9xSetFrequencyModulationEnable (uint8 byte);
void S9xSetEnvelopeRate (int channel, unsigned long rate, int direction,
                         int target);
bool8 S9xSetSoundMode (int channel, int mode);
int S9xGetEnvelopeHeight (int channel);
void S9xResetSound (bool8 full);
void S9xFixSoundAfterSnapshotLoad ();
void S9xPlaybackSoundSetting (int channel);
void S9xPlaySample (int channel);
void S9xFixEnvelope (int channel, uint8 gain, uint8 adsr1, uint8 adsr2);
void S9xStartSample (int channel);

extern "C" void S9xMixSamples (uint8 *buffer, int sample_count);
extern "C" void S9xMixSamplesO (uint8 *buffer, int sample_count, int byte_offset);
bool8 S9xOpenSoundDevice (int, bool8, int);
void S9xSetPlaybackRate (uint32 rate);
struct SSA1Registers {
    uint8 PB;
    uint8 DB;
    pair P;
    pair A;
    pair D;
    pair S;
    pair X;
    pair Y;
    uint16 PC;
};

struct SSA1 {
    struct SOpcodes *S9xOpcodes;
    uint8 _Carry;
    uint8 _Zero;
    uint8 _Negative;
    uint8 _Overflow;
    bool8 CPUExecuting;
    uint32 ShiftedPB;
    uint32 ShiftedDB;
    uint32 Flags;
    bool8 Executing;
    bool8 NMIActive;
    bool8 IRQActive;
    bool8 WaitingForInterrupt;
    bool8 Waiting;

    uint8 *PC;
    uint8 *PCBase;
    uint8 *BWRAM;
    uint8 *PCAtOpcodeStart;
    uint8 *WaitAddress;
    uint32 WaitCounter;
    uint8 *WaitByteAddress1;
    uint8 *WaitByteAddress2;



    uint8 *Map [(0x1000000 / (0x1000))];
    uint8 *WriteMap [(0x1000000 / (0x1000))];
    int16 op1;
    int16 op2;
    int arithmetic_op;
    int64 sum;
    bool8 overflow;
    uint8 VirtualBitmapFormat;
    bool8 in_char_dma;
    uint8 variable_bit_pos;
};
extern "C" {
uint8 S9xSA1GetByte (uint32);
uint16 S9xSA1GetWord (uint32);
void S9xSA1SetByte (uint8, uint32);
void S9xSA1SetWord (uint16, uint32);
void S9xSA1SetPCBase (uint32);
uint8 S9xGetSA1 (uint32);
void S9xSetSA1 (uint8, uint32);

extern struct SOpcodes S9xSA1OpcodesM1X1 [256];
extern struct SOpcodes S9xSA1OpcodesM1X0 [256];
extern struct SOpcodes S9xSA1OpcodesM0X1 [256];
extern struct SOpcodes S9xSA1OpcodesM0X0 [256];
extern struct SSA1Registers SA1Registers __asm__("DAT_00345ae8");
extern struct SSA1 SA1 __asm__("DAT_00345af8");

void S9xSA1MainLoop ();
void S9xSA1Init ();
void S9xFixSA1AfterSnapshotLoad ();
void S9xSA1ExecuteDuringSleep ();
}





static inline void S9xSA1UnpackStatus()
{
    SA1._Zero = (SA1Registers.P.B.l & 2) == 0;
    SA1._Negative = (SA1Registers.P.B.l & 128);
    SA1._Carry = (SA1Registers.P.B.l & 1);
    SA1._Overflow = (SA1Registers.P.B.l & 64) >> 6;
}

static inline void S9xSA1PackStatus()
{
    SA1Registers.P.B.l &= ~(2 | 128 | 1 | 64);
    SA1Registers.P.B.l |= SA1._Carry | ((SA1._Zero == 0) << 1) |
                       (SA1._Negative & 0x80) | (SA1._Overflow << 6);
}

static inline void S9xSA1FixCycles ()
{
    if ((SA1Registers.P.W & 256))
        SA1.S9xOpcodes = S9xSA1OpcodesM1X1;
    else
    if ((SA1Registers.P.B.l & 32))
    {
        if ((SA1Registers.P.B.l & 16))
            SA1.S9xOpcodes = S9xSA1OpcodesM1X1;
        else
            SA1.S9xOpcodes = S9xSA1OpcodesM1X0;
    }
    else
    {
        if ((SA1Registers.P.B.l & 16))
            SA1.S9xOpcodes = S9xSA1OpcodesM0X1;
        else
            SA1.S9xOpcodes = S9xSA1OpcodesM0X0;
    }
}
extern "C" {
struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

clock_t clock (void);
double difftime (time_t _time2, time_t _time1);
time_t mktime (struct tm *_timeptr);
time_t time (time_t *_timer);

char *asctime (const struct tm *_tblock);
char *ctime (const time_t *_time);
struct tm *gmtime (const time_t *_timer);
struct tm *localtime (const time_t *_timer);

size_t strftime (char *_s, size_t _maxsize, const char *_fmt, const struct tm *_t);

char *asctime_r (const struct tm *, char *);
char *ctime_r (const time_t *, char *);
struct tm *gmtime_r (const time_t *, struct tm *);
struct tm *localtime_r (const time_t *, struct tm *);
extern "C" {
}
}
typedef struct
{
    bool8 needs_init;
    bool8 count_enable;
    uint8 data [0xC +1];
    int8 index;
    uint8 mode;

    time_t system_timestamp;
    uint32 pad;
} SRTC_DATA;

extern SRTC_DATA rtc;

void S9xUpdateSrtcTime ();
void S9xSetSRTC (uint8 data, uint16 Address);
uint8 S9xGetSRTC (uint16 Address);
void S9xSRTCPreSaveState ();
void S9xSRTCPostLoadState ();
void S9xResetSRTC ();
void S9xHardResetSRTC ();
void S9xSetSDD1MemoryMap (uint32 bank, uint32 value);
void S9xResetSDD1 ();
void S9xSDD1PostLoadState ();
void S9xSDD1SaveLoggedData ();
void S9xSDD1LoadLoggedData ();
extern void (*LoadUp7110)(char*);
extern void (*CleanUp7110)(void);
extern void (*Copy7110)(void);

extern uint16 cacheMegs;

void Del7110Gfx(void);
void Close7110Gfx(void);
void Drop7110Gfx(void);
extern "C"{
uint8 S9xGetSPC7110(uint16 Address);
uint8 S9xGetSPC7110Byte(uint32 Address);
uint8* Get7110BasePtr(uint32);
}
void S9xSetSPC7110 (uint8 data, uint16 Address);
void S9xSpc7110Init();
uint8* Get7110BasePtr(uint32);
void S9xSpc7110Reset();
void S9xUpdateRTC ();
void Do7110Logging();
int S9xRTCDaysInMonth( int month, int year );



void SPC7110Load(char*);
void SPC7110Open(char*);
void SPC7110Grab(char*);

typedef struct SPC7110RTC
{
        unsigned char reg[16];
        short index;
        uint8 control;
        bool init;
        time_t last_used;
} S7RTC;

typedef struct SPC7110EmuVars
{
        unsigned char reg4800;
        unsigned char reg4801;
        unsigned char reg4802;
        unsigned char reg4803;
        unsigned char reg4804;
        unsigned char reg4805;
        unsigned char reg4806;
        unsigned char reg4807;
        unsigned char reg4808;
        unsigned char reg4809;
        unsigned char reg480A;
        unsigned char reg480B;
        unsigned char reg480C;
        unsigned char reg4811;
        unsigned char reg4812;
        unsigned char reg4813;
        unsigned char reg4814;
        unsigned char reg4815;
        unsigned char reg4816;
        unsigned char reg4817;
        unsigned char reg4818;
        unsigned char reg4820;
        unsigned char reg4821;
        unsigned char reg4822;
        unsigned char reg4823;
        unsigned char reg4824;
        unsigned char reg4825;
        unsigned char reg4826;
        unsigned char reg4827;
        unsigned char reg4828;
        unsigned char reg4829;
        unsigned char reg482A;
        unsigned char reg482B;
        unsigned char reg482C;
        unsigned char reg482D;
        unsigned char reg482E;
        unsigned char reg482F;
        unsigned char reg4830;
        unsigned char reg4831;
        unsigned char reg4832;
        unsigned char reg4833;
        unsigned char reg4834;
        unsigned char reg4840;
        unsigned char reg4841;
        unsigned char reg4842;
        uint8 AlignBy;
        uint8 written;
        uint8 offset_add;
        uint32 DataRomOffset;
        uint32 DataRomSize;
        uint32 bank50Internal;
        uint8 bank50[0x10000];

} SPC7110Regs;
extern SPC7110Regs s7r __asm__("DAT_00413508");
extern S7RTC rtc_f9 __asm__("DAT_00423548");

bool8 S9xSaveSPC7110RTC (S7RTC *rtc_f9);
bool8 S9xLoadSPC7110RTC (S7RTC *rtc_f9);



extern uint8 *SRAM __asm__("DAT_0034e298");
bool8 S9xUnfreezeZSNES (const char *filename);

typedef struct {
    int offset;
    int size;
    int type;
} FreezeData;

enum {
    INT_V, uint8_ARRAY_V, uint16_ARRAY_V, uint32_ARRAY_V
};
static FreezeData SnapCPU [] = {
    {((int) (((char *) (&(((struct SCPUState *)__null)->Flags))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->BranchSkip))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->NMIActive))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->IRQActive))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->WaitingForInterrupt))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->WhichEvent))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->Cycles))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->NextEvent))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->V_Counter))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->MemSpeed))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->MemSpeedx2))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SCPUState *)__null)->FastROMSpeed))) - ((char *) __null))), 4, INT_V}
};




static FreezeData SnapRegisters [] = {
    {((int) (((char *) (&(((struct SRegisters *)__null)->PB))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SRegisters *)__null)->DB))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SRegisters *)__null)->P.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SRegisters *)__null)->A.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SRegisters *)__null)->D.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SRegisters *)__null)->S.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SRegisters *)__null)->X.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SRegisters *)__null)->Y.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SRegisters *)__null)->PC))) - ((char *) __null))), 2, INT_V}
};




static FreezeData SnapPPU [] = {
    {((int) (((char *) (&(((struct SPPU *)__null)->BGMode))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG3Priority))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Brightness))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VMA.High))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VMA.Increment))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VMA.Address))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VMA.Mask1))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VMA.FullGraphicCount))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VMA.Shift))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[0].SCBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[0].VOffset))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[0].HOffset))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[0].BGSize))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[0].NameBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[0].SCSize))) - ((char *) __null))), 2, INT_V},

    {((int) (((char *) (&(((struct SPPU *)__null)->BG[1].SCBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[1].VOffset))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[1].HOffset))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[1].BGSize))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[1].NameBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[1].SCSize))) - ((char *) __null))), 2, INT_V},

    {((int) (((char *) (&(((struct SPPU *)__null)->BG[2].SCBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[2].VOffset))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[2].HOffset))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[2].BGSize))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[2].NameBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[2].SCSize))) - ((char *) __null))), 2, INT_V},

    {((int) (((char *) (&(((struct SPPU *)__null)->BG[3].SCBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[3].VOffset))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[3].HOffset))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[3].BGSize))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[3].NameBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BG[3].SCSize))) - ((char *) __null))), 2, INT_V},

    {((int) (((char *) (&(((struct SPPU *)__null)->CGFLIP))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->CGDATA))) - ((char *) __null))), 256, uint16_ARRAY_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->FirstSprite))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[0].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[0].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[0].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[0].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[0].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[0].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[0].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[0].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[1].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[1].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[1].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[1].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[1].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[1].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[1].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[1].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[2].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[2].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[2].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[2].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[2].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[2].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[2].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[2].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[3].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[3].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[3].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[3].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[3].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[3].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[3].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[3].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[4].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[4].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[4].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[4].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[4].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[4].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[4].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[4].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[5].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[5].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[5].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[5].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[5].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[5].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[5].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[5].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[6].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[6].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[6].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[6].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[6].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[6].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[6].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[6].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[7].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[7].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[7].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[7].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[7].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[7].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[7].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[7].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[8].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[8].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[8].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[8].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[8].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[8].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[8].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[8].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[9].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[9].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[9].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[9].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[9].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[9].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[9].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[9].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[10].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[10].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[10].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[10].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[10].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[10].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[10].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[10].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[11].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[11].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[11].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[11].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[11].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[11].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[11].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[11].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[12].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[12].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[12].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[12].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[12].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[12].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[12].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[12].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[13].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[13].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[13].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[13].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[13].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[13].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[13].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[13].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[14].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[14].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[14].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[14].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[14].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[14].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[14].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[14].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[15].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[15].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[15].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[15].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[15].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[15].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[15].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[15].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[16].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[16].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[16].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[16].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[16].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[16].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[16].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[16].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[17].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[17].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[17].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[17].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[17].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[17].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[17].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[17].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[18].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[18].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[18].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[18].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[18].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[18].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[18].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[18].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[19].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[19].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[19].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[19].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[19].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[19].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[19].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[19].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[20].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[20].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[20].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[20].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[20].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[20].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[20].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[20].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[21].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[21].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[21].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[21].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[21].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[21].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[21].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[21].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[22].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[22].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[22].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[22].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[22].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[22].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[22].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[22].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[23].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[23].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[23].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[23].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[23].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[23].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[23].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[23].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[24].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[24].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[24].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[24].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[24].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[24].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[24].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[24].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[25].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[25].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[25].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[25].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[25].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[25].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[25].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[25].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[26].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[26].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[26].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[26].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[26].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[26].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[26].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[26].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[27].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[27].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[27].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[27].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[27].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[27].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[27].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[27].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[28].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[28].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[28].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[28].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[28].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[28].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[28].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[28].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[29].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[29].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[29].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[29].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[29].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[29].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[29].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[29].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[30].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[30].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[30].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[30].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[30].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[30].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[30].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[30].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[31].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[31].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[31].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[31].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[31].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[31].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[31].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[31].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[32].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[32].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[32].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[32].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[32].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[32].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[32].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[32].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[33].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[33].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[33].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[33].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[33].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[33].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[33].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[33].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[34].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[34].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[34].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[34].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[34].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[34].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[34].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[34].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[35].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[35].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[35].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[35].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[35].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[35].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[35].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[35].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[36].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[36].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[36].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[36].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[36].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[36].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[36].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[36].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[37].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[37].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[37].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[37].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[37].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[37].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[37].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[37].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[38].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[38].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[38].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[38].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[38].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[38].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[38].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[38].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[39].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[39].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[39].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[39].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[39].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[39].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[39].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[39].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[40].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[40].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[40].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[40].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[40].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[40].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[40].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[40].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[41].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[41].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[41].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[41].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[41].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[41].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[41].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[41].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[42].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[42].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[42].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[42].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[42].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[42].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[42].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[42].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[43].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[43].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[43].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[43].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[43].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[43].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[43].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[43].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[44].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[44].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[44].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[44].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[44].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[44].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[44].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[44].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[45].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[45].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[45].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[45].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[45].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[45].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[45].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[45].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[46].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[46].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[46].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[46].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[46].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[46].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[46].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[46].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[47].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[47].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[47].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[47].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[47].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[47].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[47].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[47].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[48].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[48].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[48].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[48].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[48].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[48].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[48].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[48].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[49].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[49].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[49].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[49].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[49].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[49].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[49].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[49].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[50].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[50].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[50].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[50].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[50].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[50].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[50].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[50].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[51].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[51].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[51].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[51].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[51].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[51].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[51].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[51].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[52].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[52].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[52].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[52].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[52].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[52].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[52].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[52].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[53].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[53].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[53].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[53].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[53].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[53].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[53].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[53].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[54].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[54].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[54].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[54].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[54].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[54].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[54].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[54].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[55].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[55].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[55].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[55].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[55].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[55].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[55].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[55].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[56].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[56].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[56].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[56].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[56].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[56].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[56].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[56].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[57].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[57].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[57].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[57].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[57].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[57].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[57].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[57].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[58].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[58].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[58].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[58].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[58].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[58].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[58].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[58].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[59].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[59].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[59].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[59].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[59].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[59].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[59].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[59].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[60].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[60].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[60].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[60].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[60].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[60].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[60].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[60].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[61].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[61].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[61].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[61].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[61].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[61].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[61].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[61].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[62].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[62].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[62].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[62].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[62].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[62].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[62].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[62].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[63].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[63].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[63].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[63].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[63].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[63].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[63].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[63].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[64].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[64].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[64].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[64].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[64].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[64].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[64].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[64].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[65].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[65].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[65].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[65].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[65].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[65].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[65].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[65].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[66].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[66].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[66].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[66].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[66].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[66].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[66].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[66].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[67].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[67].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[67].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[67].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[67].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[67].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[67].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[67].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[68].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[68].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[68].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[68].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[68].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[68].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[68].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[68].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[69].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[69].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[69].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[69].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[69].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[69].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[69].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[69].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[70].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[70].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[70].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[70].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[70].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[70].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[70].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[70].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[71].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[71].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[71].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[71].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[71].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[71].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[71].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[71].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[72].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[72].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[72].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[72].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[72].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[72].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[72].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[72].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[73].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[73].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[73].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[73].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[73].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[73].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[73].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[73].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[74].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[74].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[74].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[74].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[74].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[74].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[74].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[74].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[75].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[75].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[75].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[75].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[75].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[75].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[75].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[75].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[76].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[76].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[76].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[76].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[76].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[76].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[76].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[76].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[77].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[77].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[77].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[77].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[77].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[77].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[77].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[77].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[78].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[78].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[78].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[78].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[78].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[78].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[78].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[78].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[79].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[79].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[79].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[79].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[79].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[79].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[79].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[79].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[80].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[80].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[80].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[80].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[80].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[80].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[80].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[80].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[81].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[81].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[81].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[81].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[81].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[81].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[81].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[81].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[82].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[82].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[82].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[82].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[82].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[82].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[82].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[82].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[83].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[83].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[83].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[83].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[83].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[83].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[83].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[83].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[84].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[84].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[84].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[84].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[84].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[84].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[84].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[84].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[85].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[85].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[85].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[85].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[85].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[85].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[85].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[85].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[86].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[86].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[86].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[86].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[86].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[86].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[86].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[86].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[87].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[87].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[87].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[87].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[87].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[87].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[87].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[87].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[88].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[88].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[88].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[88].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[88].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[88].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[88].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[88].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[89].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[89].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[89].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[89].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[89].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[89].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[89].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[89].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[90].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[90].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[90].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[90].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[90].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[90].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[90].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[90].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[91].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[91].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[91].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[91].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[91].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[91].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[91].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[91].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[92].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[92].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[92].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[92].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[92].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[92].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[92].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[92].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[93].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[93].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[93].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[93].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[93].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[93].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[93].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[93].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[94].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[94].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[94].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[94].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[94].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[94].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[94].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[94].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[95].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[95].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[95].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[95].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[95].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[95].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[95].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[95].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[96].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[96].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[96].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[96].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[96].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[96].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[96].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[96].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[97].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[97].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[97].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[97].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[97].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[97].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[97].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[97].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[98].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[98].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[98].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[98].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[98].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[98].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[98].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[98].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[99].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[99].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[99].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[99].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[99].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[99].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[99].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[99].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[100].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[100].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[100].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[100].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[100].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[100].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[100].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[100].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[101].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[101].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[101].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[101].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[101].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[101].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[101].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[101].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[102].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[102].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[102].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[102].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[102].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[102].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[102].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[102].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[103].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[103].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[103].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[103].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[103].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[103].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[103].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[103].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[104].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[104].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[104].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[104].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[104].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[104].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[104].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[104].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[105].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[105].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[105].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[105].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[105].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[105].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[105].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[105].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[106].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[106].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[106].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[106].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[106].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[106].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[106].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[106].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[107].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[107].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[107].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[107].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[107].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[107].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[107].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[107].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[108].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[108].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[108].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[108].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[108].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[108].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[108].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[108].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[109].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[109].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[109].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[109].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[109].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[109].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[109].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[109].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[110].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[110].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[110].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[110].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[110].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[110].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[110].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[110].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[111].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[111].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[111].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[111].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[111].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[111].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[111].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[111].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[112].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[112].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[112].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[112].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[112].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[112].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[112].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[112].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[113].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[113].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[113].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[113].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[113].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[113].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[113].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[113].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[114].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[114].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[114].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[114].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[114].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[114].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[114].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[114].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[115].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[115].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[115].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[115].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[115].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[115].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[115].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[115].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[116].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[116].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[116].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[116].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[116].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[116].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[116].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[116].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[117].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[117].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[117].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[117].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[117].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[117].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[117].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[117].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[118].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[118].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[118].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[118].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[118].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[118].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[118].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[118].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[119].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[119].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[119].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[119].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[119].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[119].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[119].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[119].Size))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[120].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[120].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[120].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[120].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[120].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[120].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[120].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[120].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[121].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[121].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[121].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[121].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[121].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[121].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[121].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[121].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[122].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[122].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[122].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[122].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[122].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[122].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[122].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[122].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[123].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[123].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[123].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[123].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[123].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[123].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[123].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[123].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[124].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[124].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[124].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[124].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[124].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[124].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[124].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[124].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[125].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[125].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[125].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[125].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[125].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[125].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[125].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[125].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[126].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[126].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[126].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[126].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[126].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[126].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[126].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[126].Size))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[127].HPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[127].VPos))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[127].Name))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[127].VFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[127].HFlip))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[127].Priority))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[127].Palette))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->OBJ[127].Size))) - ((char *) __null))), 1, INT_V},

    {((int) (((char *) (&(((struct SPPU *)__null)->OAMPriorityRotation))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OAMAddr))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OAMFlip))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OAMTileAddress))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->IRQVBeamPos))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->IRQHBeamPos))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VBeamPosLatched))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->HBeamPosLatched))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->HBeamFlip))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VBeamFlip))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->HVBeamCounterLatched))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->MatrixA))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->MatrixB))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->MatrixC))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->MatrixD))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->CentreX))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->CentreY))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Joypad1ButtonReadPos))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Joypad2ButtonReadPos))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Joypad3ButtonReadPos))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->CGADD))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->FixedColourRed))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->FixedColourGreen))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->FixedColourBlue))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->SavedOAMAddr))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->ScreenHeight))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->WRAM))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->ForcedBlanking))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJNameSelect))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJSizeSelect))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OBJNameBase))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OAMReadFlip))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->VTimerEnabled))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->HTimerEnabled))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->HTimerPosition))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Mosaic))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Mode7HFlip))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Mode7VFlip))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Mode7Repeat))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Window1Left))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Window1Right))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Window2Left))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Window2Right))) - ((char *) __null))), 1, INT_V},







    {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindowOverlapLogic[0]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Enable[0]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Enable[0]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Inside[0]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Inside[0]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindowOverlapLogic[1]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Enable[1]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Enable[1]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Inside[1]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Inside[1]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindowOverlapLogic[2]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Enable[2]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Enable[2]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Inside[2]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Inside[2]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindowOverlapLogic[3]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Enable[3]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Enable[3]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Inside[3]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Inside[3]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindowOverlapLogic[4]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Enable[4]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Enable[4]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Inside[4]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Inside[4]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindowOverlapLogic[5]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Enable[5]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Enable[5]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow1Inside[5]))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((struct SPPU *)__null)->ClipWindow2Inside[5]))) - ((char *) __null))), 1, INT_V},



    {((int) (((char *) (&(((struct SPPU *)__null)->CGFLIPRead))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Need16x8Mulitply))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->BGMosaic))) - ((char *) __null))), 4, uint8_ARRAY_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->OAMData))) - ((char *) __null))), 512 + 32, uint8_ARRAY_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->Need16x8Mulitply))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPPU *)__null)->MouseSpeed))) - ((char *) __null))), 2, uint8_ARRAY_V}
};




static FreezeData SnapDMA [] = {
    {((int) (((char *) (&(((struct SDMA *)__null)->TransferDirection))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressFixed))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressDecrement))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferMode))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->ABank))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddress))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Address))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->BAddress))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferBytes))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->HDMAIndirectAddressing))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectAddress))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectBank))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Repeat))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->LineCount))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->FirstLine))) - ((char *) __null))) + 0 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferDirection))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressFixed))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressDecrement))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferMode))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->ABank))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddress))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Address))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->BAddress))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferBytes))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->HDMAIndirectAddressing))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectAddress))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectBank))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Repeat))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->LineCount))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->FirstLine))) - ((char *) __null))) + 1 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferDirection))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressFixed))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressDecrement))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferMode))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->ABank))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddress))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Address))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->BAddress))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferBytes))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->HDMAIndirectAddressing))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectAddress))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectBank))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Repeat))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->LineCount))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->FirstLine))) - ((char *) __null))) + 2 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferDirection))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressFixed))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressDecrement))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferMode))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->ABank))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddress))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Address))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->BAddress))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferBytes))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->HDMAIndirectAddressing))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectAddress))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectBank))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Repeat))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->LineCount))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->FirstLine))) - ((char *) __null))) + 3 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferDirection))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressFixed))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressDecrement))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferMode))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->ABank))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddress))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Address))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->BAddress))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferBytes))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->HDMAIndirectAddressing))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectAddress))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectBank))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Repeat))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->LineCount))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->FirstLine))) - ((char *) __null))) + 4 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferDirection))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressFixed))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressDecrement))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferMode))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->ABank))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddress))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Address))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->BAddress))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferBytes))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->HDMAIndirectAddressing))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectAddress))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectBank))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Repeat))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->LineCount))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->FirstLine))) - ((char *) __null))) + 5 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferDirection))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressFixed))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressDecrement))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferMode))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->ABank))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddress))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Address))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->BAddress))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferBytes))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->HDMAIndirectAddressing))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectAddress))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectBank))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Repeat))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->LineCount))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->FirstLine))) - ((char *) __null))) + 6 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferDirection))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressFixed))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddressDecrement))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferMode))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->ABank))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->AAddress))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Address))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->BAddress))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->TransferBytes))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->HDMAIndirectAddressing))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectAddress))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 2, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->IndirectBank))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->Repeat))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->LineCount))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}, {((int) (((char *) (&(((struct SDMA *)__null)->FirstLine))) - ((char *) __null))) + 7 * sizeof (struct SDMA), 1, INT_V}

};




static FreezeData SnapAPU [] = {
    {((int) (((char *) (&(((struct SAPU *)__null)->Cycles))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->ShowROM))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->Flags))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->KeyedChannels))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->OutPorts))) - ((char *) __null))), 4, uint8_ARRAY_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->DSP))) - ((char *) __null))), 0x80, uint8_ARRAY_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->ExtraRAM))) - ((char *) __null))), 64, uint8_ARRAY_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->Timer))) - ((char *) __null))), 3, uint16_ARRAY_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->TimerTarget))) - ((char *) __null))), 3, uint16_ARRAY_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->TimerEnabled))) - ((char *) __null))), 3, uint8_ARRAY_V},
    {((int) (((char *) (&(((struct SAPU *)__null)->TimerValueWritten))) - ((char *) __null))), 3, uint8_ARRAY_V}
};




static FreezeData SnapAPURegisters [] = {
    {((int) (((char *) (&(((struct SAPURegisters *)__null)->P))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SAPURegisters *)__null)->YA.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SAPURegisters *)__null)->X))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SAPURegisters *)__null)->S))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SAPURegisters *)__null)->PC))) - ((char *) __null))), 2, INT_V},
};




static FreezeData SnapSoundData [] = {
    {((int) (((char *) (&(((SSoundData *)__null)->master_volume_left))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->master_volume_right))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->echo_volume_left))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->echo_volume_right))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->echo_enable))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->echo_feedback))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->echo_ptr))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->echo_buffer_size))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->echo_write_enabled))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->echo_channel_enable))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->pitch_mod))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((SSoundData *)__null)->dummy))) - ((char *) __null))), 3, uint32_ARRAY_V},
    {((int) (((char *) (&(((SSoundData *)__null)->channels [0].state))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].type))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].volume_left))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].volume_right))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].hertz))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].count))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].loop))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].envx))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].left_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].right_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].envx_target))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].env_error))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].erate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].direction))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].attack_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].decay_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].sustain_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].release_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].sustain_level))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].sample))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].decoded))) - ((char *) __null))), 16, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].previous16))) - ((char *) __null))), 2, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].sample_number))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].last_block))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].needs_decode))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].block_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].sample_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [0].mode))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].state))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].type))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].volume_left))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].volume_right))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].hertz))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].count))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].loop))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].envx))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].left_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].right_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].envx_target))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].env_error))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].erate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].direction))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].attack_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].decay_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].sustain_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].release_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].sustain_level))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].sample))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].decoded))) - ((char *) __null))), 16, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].previous16))) - ((char *) __null))), 2, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].sample_number))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].last_block))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].needs_decode))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].block_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].sample_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [1].mode))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].state))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].type))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].volume_left))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].volume_right))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].hertz))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].count))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].loop))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].envx))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].left_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].right_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].envx_target))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].env_error))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].erate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].direction))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].attack_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].decay_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].sustain_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].release_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].sustain_level))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].sample))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].decoded))) - ((char *) __null))), 16, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].previous16))) - ((char *) __null))), 2, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].sample_number))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].last_block))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].needs_decode))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].block_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].sample_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [2].mode))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].state))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].type))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].volume_left))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].volume_right))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].hertz))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].count))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].loop))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].envx))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].left_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].right_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].envx_target))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].env_error))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].erate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].direction))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].attack_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].decay_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].sustain_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].release_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].sustain_level))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].sample))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].decoded))) - ((char *) __null))), 16, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].previous16))) - ((char *) __null))), 2, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].sample_number))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].last_block))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].needs_decode))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].block_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].sample_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [3].mode))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].state))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].type))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].volume_left))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].volume_right))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].hertz))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].count))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].loop))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].envx))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].left_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].right_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].envx_target))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].env_error))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].erate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].direction))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].attack_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].decay_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].sustain_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].release_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].sustain_level))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].sample))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].decoded))) - ((char *) __null))), 16, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].previous16))) - ((char *) __null))), 2, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].sample_number))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].last_block))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].needs_decode))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].block_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].sample_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [4].mode))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].state))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].type))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].volume_left))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].volume_right))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].hertz))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].count))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].loop))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].envx))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].left_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].right_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].envx_target))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].env_error))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].erate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].direction))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].attack_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].decay_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].sustain_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].release_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].sustain_level))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].sample))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].decoded))) - ((char *) __null))), 16, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].previous16))) - ((char *) __null))), 2, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].sample_number))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].last_block))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].needs_decode))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].block_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].sample_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [5].mode))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].state))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].type))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].volume_left))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].volume_right))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].hertz))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].count))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].loop))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].envx))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].left_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].right_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].envx_target))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].env_error))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].erate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].direction))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].attack_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].decay_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].sustain_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].release_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].sustain_level))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].sample))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].decoded))) - ((char *) __null))), 16, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].previous16))) - ((char *) __null))), 2, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].sample_number))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].last_block))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].needs_decode))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].block_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].sample_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [6].mode))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].state))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].type))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].volume_left))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].volume_right))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].hertz))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].count))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].loop))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].envx))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].left_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].right_vol_level))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].envx_target))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].env_error))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].erate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].direction))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].attack_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].decay_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].sustain_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].release_rate))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].sustain_level))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].sample))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].decoded))) - ((char *) __null))), 16, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].previous16))) - ((char *) __null))), 2, uint16_ARRAY_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].sample_number))) - ((char *) __null))), 2, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].last_block))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].needs_decode))) - ((char *) __null))), 1, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].block_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].sample_pointer))) - ((char *) __null))), 4, INT_V}, {((int) (((char *) (&(((SSoundData *)__null)->channels [7].mode))) - ((char *) __null))), 4, INT_V}

};




static FreezeData SnapSA1Registers [] = {
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->PB))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->DB))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->P.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->A.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->D.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->S.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->X.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->Y.W))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SSA1Registers *)__null)->PC))) - ((char *) __null))), 2, INT_V}
};




static FreezeData SnapSA1 [] = {
    {((int) (((char *) (&(((struct SSA1 *)__null)->Flags))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SSA1 *)__null)->NMIActive))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SSA1 *)__null)->IRQActive))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SSA1 *)__null)->WaitingForInterrupt))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SSA1 *)__null)->op1))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SSA1 *)__null)->op2))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SSA1 *)__null)->arithmetic_op))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SSA1 *)__null)->sum))) - ((char *) __null))), 8, INT_V},
    {((int) (((char *) (&(((struct SSA1 *)__null)->overflow))) - ((char *) __null))), 1, INT_V}
};




static FreezeData SnapSPC7110 [] = {
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4800))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4801))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4802))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4803))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4804))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4805))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4806))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4807))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4808))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4809))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg480A))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg480B))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg480C))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4811))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4812))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4813))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4814))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4815))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4816))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4817))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4818))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4820))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4821))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4822))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4823))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4824))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4825))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4826))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4827))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4828))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4829))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg482A))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg482B))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg482C))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg482D))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg482E))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg482F))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4830))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4831))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4832))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4833))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4834))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4840))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4841))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->reg4842))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->AlignBy))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->written))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->offset_add))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->DataRomOffset))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->DataRomSize))) - ((char *) __null))), 4, INT_V},
    {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->bank50Internal))) - ((char *) __null))), 4, INT_V},
        {((int) (((char *) (&(((struct SPC7110EmuVars *)__null)->bank50))) - ((char *) __null))), 0x10000, uint8_ARRAY_V}
};




static FreezeData SnapS7RTC [] = {
    {((int) (((char *) (&(((struct SPC7110RTC *)__null)->reg))) - ((char *) __null))), 16, uint8_ARRAY_V},
    {((int) (((char *) (&(((struct SPC7110RTC *)__null)->index))) - ((char *) __null))), 2, INT_V},
    {((int) (((char *) (&(((struct SPC7110RTC *)__null)->control))) - ((char *) __null))), 1, INT_V},
    {((int) (((char *) (&(((struct SPC7110RTC *)__null)->init))) - ((char *) __null))), 1, INT_V},
        {((int) (((char *) (&(((struct SPC7110RTC *)__null)->last_used))) - ((char *) __null))),4,INT_V}
};

static char ROMFilename [1024];


static void Freeze (gzFile);
static int Unfreeze (gzFile);
void FreezeStruct (gzFile stream, char *name, void *base, FreezeData *fields,
                                   int num_fields);
void FreezeBlock (gzFile stream, char *name, uint8 *block, int size);

int UnfreezeStruct (gzFile stream, char *name, void *base, FreezeData *fields,
                                        int num_fields);
int UnfreezeBlock (gzFile stream, char *name, uint8 *block, int size);

bool8 Snapshot (const char *filename)
{
    return (S9xFreezeGame (filename));
}

bool8 S9xFreezeGame (const char *filename)
{
    gzFile stream = __null;

    if (S9xOpenSnapshotFile (filename, 0, &stream))
    {
                Freeze (stream);
                S9xCloseSnapshotFile (stream);
                return (1);
    }
    return (0);
}

bool8 S9xLoadSnapshot (const char *filename)
;

bool8 S9xUnfreezeGame (const char *filename)
{
    if (S9xLoadOrigSnapshot (filename))
                return (1);

    if (S9xUnfreezeZSNES (filename))
                return (1);

    gzFile snapshot = __null;
    if (S9xOpenSnapshotFile (filename, 1, &snapshot))
    {
                int result;
                if ((result = Unfreeze (snapshot)) != 1)
                {
                        switch (result)
                        {
                        case (-1):
                                S9xMessage (S9X_ERROR, S9X_WRONG_FORMAT,
                                        "File not in Snes9x freeze format");
                                break;
                        case (-2):
                                S9xMessage (S9X_ERROR, S9X_WRONG_VERSION,
                                        "Incompatable Snes9x freeze file format version");
                                break;
                        default:
                        case (-3):
                                sprintf (String, "ROM image \"%s\" for freeze file not found",
                                        ROMFilename);
                                S9xMessage (S9X_ERROR, S9X_ROM_NOT_FOUND, String);
                                break;
                        }
                        S9xCloseSnapshotFile (snapshot);
                        return (0);
                }
                S9xCloseSnapshotFile (snapshot);
                return (1);
    }
    return (0);
}

static void Freeze (gzFile stream)
{
    char buffer [1024];
    int i;

    S9xSetSoundMute (1);





        S9xUpdateRTC();
    S9xSRTCPreSaveState ();

    for (i = 0; i < 8; i++)
    {
                SoundData.channels [i].previous16 [0] = (int16) SoundData.channels [i].previous [0];
                SoundData.channels [i].previous16 [1] = (int16) SoundData.channels [i].previous [1];
    }
    sprintf (buffer, "%s:%04d\n", "#!snes9x", 1);
    gzwrite (stream,buffer,strlen (buffer));
    sprintf (buffer, "NAM:%06d:%s%c", strlen (Memory.ROMFilename) + 1,
                Memory.ROMFilename, 0);
    gzwrite (stream,buffer,strlen (buffer) + 1);
    FreezeStruct (stream, "CPU", &CPU, SnapCPU, (sizeof (SnapCPU) / sizeof (SnapCPU[0])));
    FreezeStruct (stream, "REG", &Registers, SnapRegisters, (sizeof (SnapRegisters) / sizeof (SnapRegisters[0])));
    FreezeStruct (stream, "PPU", &PPU, SnapPPU, (sizeof (SnapPPU) / sizeof (SnapPPU[0])));
    FreezeStruct (stream, "DMA", DMA, SnapDMA, (sizeof (SnapDMA) / sizeof (SnapDMA[0])));


    FreezeBlock (stream, "VRA", Memory.VRAM, 0x10000);
    FreezeBlock (stream, "RAM", Memory.RAM, 0x20000);
    FreezeBlock (stream, "SRA", ::SRAM, 0x20000);
    FreezeBlock (stream, "FIL", Memory.FillRAM, 0x8000);
    if (Settings.APUEnabled)
    {

                FreezeStruct (stream, "APU", &APU, SnapAPU, (sizeof (SnapAPU) / sizeof (SnapAPU[0])));
                FreezeStruct (stream, "ARE", &APURegisters, SnapAPURegisters,
                        (sizeof (SnapAPURegisters) / sizeof (SnapAPURegisters[0])));
                FreezeBlock (stream, "ARA", IAPU.RAM, 0x10000);
                FreezeStruct (stream, "SOU", &SoundData, SnapSoundData,
                        (sizeof (SnapSoundData) / sizeof (SnapSoundData[0])));
    }
    if (Settings.SA1)
    {
                SA1Registers.PC = SA1.PC - SA1.PCBase;
                S9xSA1PackStatus ();
                FreezeStruct (stream, "SA1", &SA1, SnapSA1, (sizeof (SnapSA1) / sizeof (SnapSA1[0])));
                FreezeStruct (stream, "SAR", &SA1Registers, SnapSA1Registers,
                        (sizeof (SnapSA1Registers) / sizeof (SnapSA1Registers[0])));
    }

        if (Settings.SPC7110)
    {
                FreezeStruct (stream, "SP7", &s7r, SnapSPC7110, (sizeof (SnapSPC7110) / sizeof (SnapSPC7110[0])));
    }
        if(Settings.SPC7110RTC)
        {
                FreezeStruct (stream, "RTC", &rtc_f9, SnapS7RTC, (sizeof (SnapS7RTC) / sizeof (SnapS7RTC[0])));
        }

    S9xSetSoundMute (0);




}

static int Unfreeze (gzFile stream)
{
    char buffer [1024 + 1];
    char rom_filename [1024 + 1];
    int result;

    int version;
    int len = strlen ("#!snes9x") + 1 + 4 + 1;
    if (gzread (stream,buffer,len) != len)
                return ((-1));
    if (strncmp (buffer, "#!snes9x", strlen ("#!snes9x")) != 0)
                return ((-1));
    if ((version = strtol (&buffer [strlen ("#!snes9x") + 1], __null, 10)) > 1)
                return ((-2));

    if ((result = UnfreezeBlock (stream, "NAM", (uint8 *) rom_filename, 1024)) != 1)
                return (result);

    if (strcasecmp (rom_filename, Memory.ROMFilename) != 0 &&
                strcasecmp (S9xBasename (rom_filename), S9xBasename (Memory.ROMFilename)) != 0)
    {
                S9xMessage (S9X_WARNING, S9X_FREEZE_ROM_NAME,
                        "Current loaded ROM image doesn't match that required by freeze-game file.");
    }

    uint32 old_flags = CPU.Flags;
    uint32 sa1_old_flags = SA1.Flags;
    S9xReset ();
    S9xSetSoundMute (1);

    if ((result = UnfreezeStruct (stream, "CPU", &CPU, SnapCPU,
                (sizeof (SnapCPU) / sizeof (SnapCPU[0])))) != 1)
                return (result);
    Memory.FixROMSpeed ();
    CPU.Flags |= old_flags & ((1 << 0) | (1 << 1) |
                (1 << 2) | (1 << 9));
    if ((result = UnfreezeStruct (stream, "REG", &Registers, SnapRegisters, (sizeof (SnapRegisters) / sizeof (SnapRegisters[0])))) != 1)
                return (result);
    if ((result = UnfreezeStruct (stream, "PPU", &PPU, SnapPPU, (sizeof (SnapPPU) / sizeof (SnapPPU[0])))) != 1)
                return (result);

    IPPU.ColorsChanged = 1;
    IPPU.OBJChanged = 1;
    CPU.InDMA = 0;
    S9xFixColourBrightness ();
    IPPU.RenderThisFrame = 0;

    if ((result = UnfreezeStruct (stream, "DMA", DMA, SnapDMA,
                (sizeof (SnapDMA) / sizeof (SnapDMA[0])))) != 1)
                return (result);
    if ((result = UnfreezeBlock (stream, "VRA", Memory.VRAM, 0x10000)) != 1)
                return (result);
    if ((result = UnfreezeBlock (stream, "RAM", Memory.RAM, 0x20000)) != 1)
                return (result);
    if ((result = UnfreezeBlock (stream, "SRA", ::SRAM, 0x20000)) != 1)
                return (result);
    if ((result = UnfreezeBlock (stream, "FIL", Memory.FillRAM, 0x8000)) != 1)
                return (result);
    if (UnfreezeStruct (stream, "APU", &APU, SnapAPU, (sizeof (SnapAPU) / sizeof (SnapAPU[0]))) == 1)
    {
                if ((result = UnfreezeStruct (stream, "ARE", &APURegisters, SnapAPURegisters,
                        (sizeof (SnapAPURegisters) / sizeof (SnapAPURegisters[0])))) != 1)
                        return (result);
                if ((result = UnfreezeBlock (stream, "ARA", IAPU.RAM, 0x10000)) != 1)
                        return (result);
                if ((result = UnfreezeStruct (stream, "SOU", &SoundData, SnapSoundData,
                        (sizeof (SnapSoundData) / sizeof (SnapSoundData[0])))) != 1)
                        return (result);

                S9xSetSoundMute (0);
                IAPU.PC = IAPU.RAM + APURegisters.PC;
                S9xAPUUnpackStatus ();
                if ((APURegisters.P & 32))
                        IAPU.DirectPage = IAPU.RAM + 0x100;
                else
                        IAPU.DirectPage = IAPU.RAM;
                Settings.APUEnabled = 1;
                IAPU.APUExecuting = 1;
    }
    else
    {
                Settings.APUEnabled = 0;
                IAPU.APUExecuting = 0;
                S9xSetSoundMute (1);
    }
    if ((result = UnfreezeStruct (stream, "SA1", &SA1, SnapSA1,
                (sizeof (SnapSA1) / sizeof (SnapSA1[0])))) == 1)
    {
                if ((result = UnfreezeStruct (stream, "SAR", &SA1Registers,
                        SnapSA1Registers, (sizeof (SnapSA1Registers) / sizeof (SnapSA1Registers[0])))) != 1)
                        return (result);

                S9xFixSA1AfterSnapshotLoad ();
                SA1.Flags |= sa1_old_flags & ((1 << 1));
    }

        if ((result = UnfreezeStruct (stream, "SP7", &s7r, SnapSPC7110,
                (sizeof (SnapSPC7110) / sizeof (SnapSPC7110[0])))) != 1)
    {
                if(Settings.SPC7110)
                        return result;
        }
        if ((result = UnfreezeStruct (stream, "RTC", &rtc_f9,
                        SnapS7RTC, (sizeof (SnapS7RTC) / sizeof (SnapS7RTC[0])))) == 1)
        {
                S9xUpdateRTC();
        }
        else
        {
                if(Settings.SPC7110RTC)
                        return result;
        }
    S9xFixSoundAfterSnapshotLoad ();

        if(!Memory.FillRAM[0x4213]){

                Memory.FillRAM[0x4213]=Memory.FillRAM[0x4201];
                if(!Memory.FillRAM[0x4213])
                        Memory.FillRAM[0x4213]=Memory.FillRAM[0x4201]=0xFF;
        }

    ICPU.ShiftedPB = Registers.PB << 16;
    ICPU.ShiftedDB = Registers.DB << 16;
    S9xSetPCBase (ICPU.ShiftedPB + Registers.PC);
    S9xUnpackStatus ();
    S9xFixCycles ();
    S9xReschedule ();





    S9xSRTCPostLoadState ();
    if (Settings.SDD1)
                S9xSDD1PostLoadState ();

    return (1);
}

int FreezeSize (int size, int type)
{
    switch (type)
    {
    case uint16_ARRAY_V:
                return (size * 2);
    case uint32_ARRAY_V:
                return (size * 4);
    default:
                return (size);
    }
}

void FreezeStruct (gzFile stream, char *name, void *base, FreezeData *fields,
                                   int num_fields)
{

    int len = 0;
    int i;
    int j;

    for (i = 0; i < num_fields; i++)
    {
                if (fields [i].offset + FreezeSize (fields [i].size,
                        fields [i].type) > len)
                        len = fields [i].offset + FreezeSize (fields [i].size,
                        fields [i].type);
    }

    uint8 *block = new uint8 [len];
    uint8 *ptr = block;
    uint16 word;
    uint32 dword;
    int64 qword;


    for (i = 0; i < num_fields; i++)
    {
                switch (fields [i].type)
                {
                case INT_V:
                        switch (fields [i].size)
                        {
                        case 1:
                                *ptr++ = *((uint8 *) base + fields [i].offset);
                                break;
                        case 2:
                                word = *((uint16 *) ((uint8 *) base + fields [i].offset));
                                *ptr++ = (uint8) (word >> 8);
                                *ptr++ = (uint8) word;
                                break;
                        case 4:
                                dword = *((uint32 *) ((uint8 *) base + fields [i].offset));
                                *ptr++ = (uint8) (dword >> 24);
                                *ptr++ = (uint8) (dword >> 16);
                                *ptr++ = (uint8) (dword >> 8);
                                *ptr++ = (uint8) dword;
                                break;
                        case 8:
                                qword = *((int64 *) ((uint8 *) base + fields [i].offset));
                                *ptr++ = (uint8) (qword >> 56);
                                *ptr++ = (uint8) (qword >> 48);
                                *ptr++ = (uint8) (qword >> 40);
                                *ptr++ = (uint8) (qword >> 32);
                                *ptr++ = (uint8) (qword >> 24);
                                *ptr++ = (uint8) (qword >> 16);
                                *ptr++ = (uint8) (qword >> 8);
                                *ptr++ = (uint8) qword;
                                break;
                        }
                        break;
                        case uint8_ARRAY_V:
                                memmove (ptr, (uint8 *) base + fields [i].offset, fields [i].size);
                                ptr += fields [i].size;
                                break;
                        case uint16_ARRAY_V:
                                for (j = 0; j < fields [i].size; j++)
                                {
                                        word = *((uint16 *) ((uint8 *) base + fields [i].offset + j * 2));
                                        *ptr++ = (uint8) (word >> 8);
                                        *ptr++ = (uint8) word;
                                }
                                break;
                        case uint32_ARRAY_V:
                                for (j = 0; j < fields [i].size; j++)
                                {
                                        dword = *((uint32 *) ((uint8 *) base + fields [i].offset + j * 4));
                                        *ptr++ = (uint8) (dword >> 24);
                                        *ptr++ = (uint8) (dword >> 16);
                                        *ptr++ = (uint8) (dword >> 8);
                                        *ptr++ = (uint8) dword;
                                }
                                break;
                }
    }

    FreezeBlock (stream, name, block, len);
    delete[] block;
}

void FreezeBlock (gzFile stream, char *name, uint8 *block, int size)
;

int UnfreezeStruct (gzFile stream, char *name, void *base, FreezeData *fields,
                                        int num_fields)
{

    int len = 0;
    int i;
    int j;

    for (i = 0; i < num_fields; i++)
    {
                if (fields [i].offset + FreezeSize (fields [i].size,
                        fields [i].type) > len)
                        len = fields [i].offset + FreezeSize (fields [i].size,
                        fields [i].type);
    }

    uint8 *block = new uint8 [len];
    uint8 *ptr = block;
    uint16 word;
    uint32 dword;
    int64 qword;
    int result;

    if ((result = UnfreezeBlock (stream, name, block, len)) != 1)
    {
                delete block;
                return (result);
    }


    for (i = 0; i < num_fields; i++)
    {
                switch (fields [i].type)
                {
                case INT_V:
                        switch (fields [i].size)
                        {
                        case 1:
                                *((uint8 *) base + fields [i].offset) = *ptr++;
                                break;
                        case 2:
                                word = *ptr++ << 8;
                                word |= *ptr++;
                                *((uint16 *) ((uint8 *) base + fields [i].offset)) = word;
                                break;
                        case 4:
                                dword = *ptr++ << 24;
                                dword |= *ptr++ << 16;
                                dword |= *ptr++ << 8;
                                dword |= *ptr++;
                                *((uint32 *) ((uint8 *) base + fields [i].offset)) = dword;
                                break;
                        case 8:
                                qword = (int64) *ptr++ << 56;
                                qword |= (int64) *ptr++ << 48;
                                qword |= (int64) *ptr++ << 40;
                                qword |= (int64) *ptr++ << 32;
                                qword |= (int64) *ptr++ << 24;
                                qword |= (int64) *ptr++ << 16;
                                qword |= (int64) *ptr++ << 8;
                                qword |= (int64) *ptr++;
                                *((int64 *) ((uint8 *) base + fields [i].offset)) = qword;
                                break;
                        }
                        break;
                        case uint8_ARRAY_V:
                                memmove ((uint8 *) base + fields [i].offset, ptr, fields [i].size);
                                ptr += fields [i].size;
                                break;
                        case uint16_ARRAY_V:
                                for (j = 0; j < fields [i].size; j++)
                                {
                                        word = *ptr++ << 8;
                                        word |= *ptr++;
                                        *((uint16 *) ((uint8 *) base + fields [i].offset + j * 2)) = word;
                                }
                                break;
                        case uint32_ARRAY_V:
                                for (j = 0; j < fields [i].size; j++)
                                {
                                        dword = *ptr++ << 24;
                                        dword |= *ptr++ << 16;
                                        dword |= *ptr++ << 8;
                                        dword |= *ptr++;
                                        *((uint32 *) ((uint8 *) base + fields [i].offset + j * 4)) = dword;
                                }
                                break;
                }
    }

    delete [] block;
    return (result);
}

int UnfreezeBlock (gzFile stream, char *name, uint8 *block, int size)
{
    char buffer [20];
    int len = 0;
    int rem = 0;
    int rew_len;
    if (gzread (stream,buffer,11) != 11 ||
                strncmp (buffer, name, 3) != 0 || buffer [3] != ':' ||
                (len = strtol (&buffer [4], __null, 10)) == 0)
    {
                gzseek(stream,gztell(stream)-11,0);
                return ((-1));
    }

    if (len > size)
    {
                rem = len - size;
                len = size;
    }
    if ((rew_len=gzread (stream,block,len)) != len)
        {
                gzseek(stream,gztell(stream)-11-rew_len,0);
                return ((-1));
        }
    if (rem)
    {
                char *junk = new char [rem];
                gzread (stream,junk,rem);
                delete [] junk;
    }

    return (1);
}

extern uint8 spc_dump_dsp[0x100];

bool8 S9xSPCDump (const char *filename)
;

bool8 S9xUnfreezeZSNES (const char *filename)
;
