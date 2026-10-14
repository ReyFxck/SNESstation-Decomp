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

/* Native bulk proof: analysis/functions/native_bulk_apu_exact_3152.tsv
 * 7 routines / 3152 complete linked historical instruction bytes.
 * Reviewed native entry points:
 * 0x0010a840 S9xInitAPU (124 bytes)
 * 0x0010a8bc S9xDeinitAPU (120 bytes)
 * 0x0010ad48 S9xSetAPUDSP (1576 bytes)
 * 0x0010b370 _Z14S9xFixEnvelopeihhh (500 bytes)
 * 0x0010b564 S9xSetAPUControl (456 bytes)
 * 0x0010b72c S9xSetAPUTimer (204 bytes)
 * 0x0010b7f8 S9xGetAPUDSP (172 bytes)
 */
/* Pinned Snes9x native apu recovery; original declarations/macros and bodies expanded with the historical EE profile. */
extern "C" {
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long unsigned int size_t;
typedef __builtin_va_list __gnuc_va_list;
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

extern "C" struct SRegisters Registers;
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
extern struct SCPUState CPU;
extern struct SSNESGameFixes SNESGameFixes;
extern char String [513];

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
extern int32 S9xAPUCycles [256] __asm__("g_S9xAPUCycles_003f44a8");
extern int32 S9xAPUCycleLengths [256] __asm__("DAT_003f40a8");
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
void S9xSetSoundFrequency (int channel, int hertz) __asm__("S9xSetSoundFrequency");
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
void S9xSetEchoEnable (uint8 byte) __asm__("S9xSetEchoEnable");
void S9xSetEchoDelay (int byte) __asm__("S9xSetEchoDelay");
void S9xSetEchoWriteEnable (uint8 byte) __asm__("S9xSetEchoWriteEnable");
void S9xSetFilterCoefficient (int tap, int value);
void S9xSetFrequencyModulationEnable (uint8 byte);
void S9xSetEnvelopeRate (int channel, unsigned long rate, int direction,
                         int target);
bool8 S9xSetSoundMode (int channel, int mode);
int S9xGetEnvelopeHeight (int channel);
void S9xResetSound (bool8 full) __asm__("S9xResetSound");
void S9xFixSoundAfterSnapshotLoad ();
void S9xPlaybackSoundSetting (int channel);
void S9xPlaySample (int channel);
void S9xFixEnvelope (int channel, uint8 gain, uint8 adsr1, uint8 adsr2);
void S9xStartSample (int channel);

extern "C" void S9xMixSamples (uint8 *buffer, int sample_count);
extern "C" void S9xMixSamplesO (uint8 *buffer, int sample_count, int byte_offset);
bool8 S9xOpenSoundDevice (int, bool8, int);
void S9xSetPlaybackRate (uint32 rate);
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

extern struct SPPU PPU;
extern struct SDMA DMA [8];
extern struct InternalPPU IPPU;
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
extern CMemory Memory;
extern uint8 *SRAM;
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
extern struct SICPU ICPU;
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
bool8 S9xFreezeGame (const char *filename);
bool8 S9xUnfreezeGame (const char *filename);
bool8 Snapshot (const char *filename);
bool8 S9xLoadSnapshot (const char *filename);
bool8 S9xSPCDump (const char *filename);
}



extern "C" {const char *S9xGetFilenameInc (const char *);}

int spc_is_dumping=0;
int spc_is_dumping_temp;
uint8 spc_dump_dsp[0x100];

extern int NoiseFreq [32] __asm__("DAT_003f3fc0");





bool8 S9xInitAPU ()
{
    IAPU.RAM = (uint8 *) malloc (0x10000);
    IAPU.ShadowRAM = (uint8 *) malloc (0x10000);
    IAPU.CachedSamples = (uint8 *) malloc (0x40000);

    if (!IAPU.RAM || !IAPU.ShadowRAM || !IAPU.CachedSamples)
    {
                S9xDeinitAPU ();
                return (0);
    }

    return (1);
}

void S9xDeinitAPU ()
{
    if (IAPU.RAM)
    {
                free ((char *) IAPU.RAM);
                IAPU.RAM = __null;
    }
    if (IAPU.ShadowRAM)
    {
                free ((char *) IAPU.ShadowRAM);
                IAPU.ShadowRAM = __null;
    }
    if (IAPU.CachedSamples)
    {
                free ((char *) IAPU.CachedSamples);
                IAPU.CachedSamples = __null;
    }
}

extern "C" uint8 APUROM [64] __asm__("DAT_003f4068");

void S9xResetAPU ()
;

void S9xSetAPUDSP (uint8 byte)
{
    uint8 reg = IAPU.RAM [0xf2];
        static uint8 KeyOn;
        static uint8 KeyOnPrev;
    int i;

        spc_dump_dsp[reg] = byte;

    switch (reg)
    {
    case 0x6c:
                if (byte & 0x80)
                {
                        APU.DSP [reg] = 0x40 | 0x20 | (byte & 0x1f);
                        APU.DSP [0x7c] = 0;
                        APU.DSP [0x5c] = 0;
                        APU.DSP [0x4c] = 0;
                        S9xSetEchoWriteEnable (0);





                        S9xResetSound (0);
                }
                else
                {
                        S9xSetEchoWriteEnable (!(byte & 0x20));
                        if (byte & 0x40)
                        {




                                S9xSetSoundMute (1);
                        }
                        else
                                S9xSetSoundMute (0);

                        SoundData.noise_hertz = NoiseFreq [byte & 0x1f];
                        for (i = 0; i < 8; i++)
                        {
                                if (SoundData.channels [i].type == SOUND_NOISE)
                                        S9xSetSoundFrequency (i, SoundData.noise_hertz);
                        }
                }
                break;
    case 0x3d:
                if (byte != APU.DSP [0x3d])
                {




                        uint8 mask = 1;
                        for (int c = 0; c < 8; c++, mask <<= 1)
                        {
                                int type;
                                if (byte & mask)
                                {
                                        type = SOUND_NOISE;
                                }
                                else
                                {
                                        type = SOUND_SAMPLE;







                                }
                                S9xSetSoundType (c, type);
                        }




                }
                break;
    case 0x0c:
                if (byte != APU.DSP [0x0c])
                {





                        S9xSetMasterVolume ((signed char) byte,
                                (signed char) APU.DSP [0x1c]);
                }
                break;
    case 0x1c:
                if (byte != APU.DSP [0x1c])
                {





                        S9xSetMasterVolume ((signed char) APU.DSP [0x0c],
                                (signed char) byte);
                }
                break;
    case 0x2c:
                if (byte != APU.DSP [0x2c])
                {





                        S9xSetEchoVolume ((signed char) byte,
                                (signed char) APU.DSP [0x3c]);
                }
                break;
    case 0x3c:
                if (byte != APU.DSP [0x3c])
                {





                        S9xSetEchoVolume ((signed char) APU.DSP [0x2c],
                                (signed char) byte);
                }
                break;
    case 0x7c:




                byte = 0;
                break;

    case 0x5c:

                {
                        uint8 mask = 1;




                        for (int c = 0; c < 8; c++, mask <<= 1)
                        {
                                if ((byte & mask) != 0)
                                {





                                        if (APU.KeyedChannels & mask)
                                        {
                                                {
                                                        KeyOnPrev&=~mask;
                                                        APU.KeyedChannels &= ~mask;
                                                        APU.DSP [0x4c] &= ~mask;

                                                        S9xSetSoundKeyOff (c);
                                                }
                                        }
                                }
                                else if((KeyOnPrev&mask)!=0)
                                {
                                        KeyOnPrev&=~mask;
                                        APU.KeyedChannels |= mask;

                                        APU.DSP [0x5c] &= ~mask;
                                        APU.DSP [0x7c] &= ~mask;
                                        S9xPlaySample (c);
                                }
                        }




                }

                APU.DSP [0x5c] = byte;
                return;
    case 0x4c:
                if (spc_is_dumping)
                {
                        if (byte & ~spc_is_dumping_temp)
                        {
                                APURegisters.PC = IAPU.PC - IAPU.RAM;
                                S9xAPUPackStatus();
                                S9xSPCDump (S9xGetFilenameInc (".spc"));
                                spc_is_dumping = 0;
                        }
                }
                if (byte)
                {
                        uint8 mask = 1;





                        for (int c = 0; c < 8; c++, mask <<= 1)
                        {
                                if ((byte & mask) != 0)
                                {






                                        if((APU.DSP [0x5c] & mask) ==0)
                                        {
                                                KeyOnPrev&=~mask;
                                                APU.KeyedChannels |= mask;


                                                APU.DSP [0x7c] &= ~mask;
                                                S9xPlaySample (c);
                                        }
                                        else KeyOn|=mask;
                                }
                        }




                }
                spc_is_dumping_temp = byte;
                return;

    case 0x00 + 0x00:
    case 0x00 + 0x10:
    case 0x00 + 0x20:
    case 0x00 + 0x30:
    case 0x00 + 0x40:
    case 0x00 + 0x50:
    case 0x00 + 0x60:
    case 0x00 + 0x70:


                {





                        S9xSetSoundVolume (reg >> 4, (signed char) byte,
                                (signed char) APU.DSP [reg + 1]);
                }
                break;
    case 0x01 + 0x00:
    case 0x01 + 0x10:
    case 0x01 + 0x20:
    case 0x01 + 0x30:
    case 0x01 + 0x40:
    case 0x01 + 0x50:
    case 0x01 + 0x60:
    case 0x01 + 0x70:


                {





                        S9xSetSoundVolume (reg >> 4, (signed char) APU.DSP [reg - 1],
                                (signed char) byte);
                }
                break;

    case 0x02 + 0x00:
    case 0x02 + 0x10:
    case 0x02 + 0x20:
    case 0x02 + 0x30:
    case 0x02 + 0x40:
    case 0x02 + 0x50:
    case 0x02 + 0x60:
    case 0x02 + 0x70:





                S9xSetSoundHertz (reg >> 4, ((byte + (APU.DSP [reg + 1] << 8)) & 0x3fff) * 8);
                break;

    case 0x03 + 0x00:
    case 0x03 + 0x10:
    case 0x03 + 0x20:
    case 0x03 + 0x30:
    case 0x03 + 0x40:
    case 0x03 + 0x50:
    case 0x03 + 0x60:
    case 0x03 + 0x70:





                S9xSetSoundHertz (reg >> 4,
                        (((byte << 8) + APU.DSP [reg - 1]) & 0x3fff) * 8);
                break;

    case 0x04 + 0x00:
    case 0x04 + 0x10:
    case 0x04 + 0x20:
    case 0x04 + 0x30:
    case 0x04 + 0x40:
    case 0x04 + 0x50:
    case 0x04 + 0x60:
    case 0x04 + 0x70:
                if (byte != APU.DSP [reg])
                {





                        S9xSetSoundSample (reg >> 4, byte);
                }
                break;

    case 0x05 + 0x00:
    case 0x05 + 0x10:
    case 0x05 + 0x20:
    case 0x05 + 0x30:
    case 0x05 + 0x40:
    case 0x05 + 0x50:
    case 0x05 + 0x60:
    case 0x05 + 0x70:
                if (byte != APU.DSP [reg])
                {





                        {
                                S9xFixEnvelope (reg >> 4, APU.DSP [reg + 2], byte,
                                        APU.DSP [reg + 1]);
                        }
                }
                break;

    case 0x06 + 0x00:
    case 0x06 + 0x10:
    case 0x06 + 0x20:
    case 0x06 + 0x30:
    case 0x06 + 0x40:
    case 0x06 + 0x50:
    case 0x06 + 0x60:
    case 0x06 + 0x70:
                if (byte != APU.DSP [reg])
                {





                        {
                                S9xFixEnvelope (reg >> 4, APU.DSP [reg + 1], APU.DSP [reg - 1],
                                        byte);
                        }
                }
                break;

    case 0x07 + 0x00:
    case 0x07 + 0x10:
    case 0x07 + 0x20:
    case 0x07 + 0x30:
    case 0x07 + 0x40:
    case 0x07 + 0x50:
    case 0x07 + 0x60:
    case 0x07 + 0x70:
                if (byte != APU.DSP [reg])
                {





                        {
                                S9xFixEnvelope (reg >> 4, byte, APU.DSP [reg - 2],
                                        APU.DSP [reg - 1]);
                        }
                }
                break;

    case 0x08 + 0x00:
    case 0x08 + 0x10:
    case 0x08 + 0x20:
    case 0x08 + 0x30:
    case 0x08 + 0x40:
    case 0x08 + 0x50:
    case 0x08 + 0x60:
    case 0x08 + 0x70:
                break;

    case 0x09 + 0x00:
    case 0x09 + 0x10:
    case 0x09 + 0x20:
    case 0x09 + 0x30:
    case 0x09 + 0x40:
    case 0x09 + 0x50:
    case 0x09 + 0x60:
    case 0x09 + 0x70:
                break;

    case 0x5d:





                break;

    case 0x2d:
                if (byte != APU.DSP [0x2d])
                {
                        S9xSetFrequencyModulationEnable (byte);
                }
                break;

    case 0x4d:
                if (byte != APU.DSP [0x4d])
                {
                        S9xSetEchoEnable (byte);
                }
                break;

    case 0x0d:
                S9xSetEchoFeedback ((signed char) byte);
                break;

    case 0x6d:
                break;

    case 0x7d:
                S9xSetEchoDelay (byte & 0xf);
                break;

    case 0x0f:
    case 0x1f:
    case 0x2f:
    case 0x3f:
    case 0x4f:
    case 0x5f:
    case 0x6f:
    case 0x7f:
                S9xSetFilterCoefficient (reg >> 4, (signed char) byte);
                break;
    default:


                break;
    }

        KeyOnPrev|=KeyOn;
        KeyOn=0;

    if (reg < 0x80)
                APU.DSP [reg] = byte;
}

void S9xFixEnvelope (int channel, uint8 gain, uint8 adsr1, uint8 adsr2)
{
    if (adsr1 & 0x80)
    {

                static unsigned long AttackRate [16] = {
                        4100, 2600, 1500, 1000, 640, 380, 260, 160,
                                96, 64, 40, 24, 16, 10, 6, 1
                };
                static unsigned long DecayRate [8] = {
                        1200, 740, 440, 290, 180, 110, 74, 37
                };
                static unsigned long SustainRate [32] = {
                        ~0, 38000, 28000, 24000, 19000, 14000, 12000, 9400,
                                7100, 5900, 4700, 3500, 2900, 2400, 1800, 1500,
                                1200, 880, 740, 590, 440, 370, 290, 220,
                                180, 150, 110, 92, 74, 55, 37, 18
                };



                if (S9xSetSoundMode (channel, MODE_ADSR))
                {



                        int attack = AttackRate [adsr1 & 0xf];

                        if (attack == 1 && (!Settings.SoundSync



                ))
                                attack = 0;

                        S9xSetSoundADSR (channel, attack,
                                DecayRate [(adsr1 >> 4) & 7],
                                SustainRate [adsr2 & 0x1f],
                                (adsr2 >> 5) & 7, 8);
                }
    }
    else
    {

                if ((gain & 0x80) == 0)
                {
                        if (S9xSetSoundMode (channel, MODE_GAIN))
                        {
                                S9xSetEnvelopeRate (channel, 0, 0, gain & 0x7f);
                                S9xSetEnvelopeHeight (channel, gain & 0x7f);
                        }
                }
                else
                {
                        static unsigned long IncreaseRate [32] = {
                                ~0, 4100, 3100, 2600, 2000, 1500, 1300, 1000,
                                        770, 640, 510, 380, 320, 260, 190, 160,
                                        130, 96, 80, 64, 48, 40, 32, 24,
                                        20, 16, 12, 10, 8, 6, 4, 2
                        };
                        static unsigned long DecreaseRateExp [32] = {
                                ~0, 38000, 28000, 24000, 19000, 14000, 12000, 9400,
                                        7100, 5900, 4700, 3500, 2900, 2400, 1800, 1500,
                                        1200, 880, 740, 590, 440, 370, 290, 220,
                                        180, 150, 110, 92, 74, 55, 37, 18
                        };
                        if (gain & 0x40)
                        {

                                if (S9xSetSoundMode (channel, (gain & 0x20) ?
MODE_INCREASE_BENT_LINE :
                                MODE_INCREASE_LINEAR))
                                {
                                        S9xSetEnvelopeRate (channel, IncreaseRate [gain & 0x1f],
                                                1, 127);
                                }
                        }
                        else
                        {
                                uint32 rate = (gain & 0x20) ? DecreaseRateExp [gain & 0x1f] / 2 :
                        IncreaseRate [gain & 0x1f];
                        int mode = (gain & 0x20) ? MODE_DECREASE_EXPONENTIAL
                                : MODE_DECREASE_LINEAR;

                        if (S9xSetSoundMode (channel, mode))
                                S9xSetEnvelopeRate (channel, rate, -1, 0);
                        }
                }
    }
}

void S9xSetAPUControl (uint8 byte)
{


    if ((byte & 1) != 0 && !APU.TimerEnabled [0])
    {
                APU.Timer [0] = 0;
                IAPU.RAM [0xfd] = 0;
                if ((APU.TimerTarget [0] = IAPU.RAM [0xfa]) == 0)
                        APU.TimerTarget [0] = 0x100;
    }
    if ((byte & 2) != 0 && !APU.TimerEnabled [1])
    {
                APU.Timer [1] = 0;
                IAPU.RAM [0xfe] = 0;
                if ((APU.TimerTarget [1] = IAPU.RAM [0xfb]) == 0)
                        APU.TimerTarget [1] = 0x100;
    }
    if ((byte & 4) != 0 && !APU.TimerEnabled [2])
    {
                APU.Timer [2] = 0;
                IAPU.RAM [0xff] = 0;
                if ((APU.TimerTarget [2] = IAPU.RAM [0xfc]) == 0)
                        APU.TimerTarget [2] = 0x100;
    }
    APU.TimerEnabled [0] = byte & 1;
    APU.TimerEnabled [1] = (byte & 2) >> 1;
    APU.TimerEnabled [2] = (byte & 4) >> 2;

    if (byte & 0x10)
                IAPU.RAM [0xF4] = IAPU.RAM [0xF5] = 0;

    if (byte & 0x20)
                IAPU.RAM [0xF6] = IAPU.RAM [0xF7] = 0;

    if (byte & 0x80)
    {
                if (!APU.ShowROM)
                {
                        memmove (&IAPU.RAM [0xffc0], APUROM, sizeof (APUROM));
                        APU.ShowROM = 1;
                }
    }
    else
    {
                if (APU.ShowROM)
                {
                        APU.ShowROM = 0;
                        memmove (&IAPU.RAM [0xffc0], APU.ExtraRAM, sizeof (APUROM));
                }
    }
    IAPU.RAM [0xf1] = byte;
}

void S9xSetAPUTimer (uint16 Address, uint8 byte)
{
    IAPU.RAM [Address] = byte;

    switch (Address)
    {
    case 0xfa:
                if ((APU.TimerTarget [0] = IAPU.RAM [0xfa]) == 0)
                        APU.TimerTarget [0] = 0x100;
                APU.TimerValueWritten [0] = 1;
                break;
    case 0xfb:
                if ((APU.TimerTarget [1] = IAPU.RAM [0xfb]) == 0)
                        APU.TimerTarget [1] = 0x100;
                APU.TimerValueWritten [1] = 1;
                break;
    case 0xfc:
                if ((APU.TimerTarget [2] = IAPU.RAM [0xfc]) == 0)
                        APU.TimerTarget [2] = 0x100;
                APU.TimerValueWritten [2] = 1;
                break;
    }
}

uint8 S9xGetAPUDSP ()
{
    uint8 reg = IAPU.RAM [0xf2] & 0x7f;
    uint8 byte = APU.DSP [reg];

    switch (reg)
    {
    case 0x4c:
                break;
    case 0x5c:
                break;
    case 0x09 + 0x00:
    case 0x09 + 0x10:
    case 0x09 + 0x20:
    case 0x09 + 0x30:
    case 0x09 + 0x40:
    case 0x09 + 0x50:
    case 0x09 + 0x60:
    case 0x09 + 0x70:
                if (SoundData.channels [reg >> 4].state == SOUND_SILENT)
                        return (0);
                return ((SoundData.channels [reg >> 4].sample >> 8) |
                        (SoundData.channels [reg >> 4].sample & 0xff));

    case 0x08 + 0x00:
    case 0x08 + 0x10:
    case 0x08 + 0x20:
    case 0x08 + 0x30:
    case 0x08 + 0x40:
    case 0x08 + 0x50:
    case 0x08 + 0x60:
    case 0x08 + 0x70:
                return ((uint8) S9xGetEnvelopeHeight (reg >> 4));

    case 0x7c:


                break;
    default:
                break;
    }
    return (byte);
}
