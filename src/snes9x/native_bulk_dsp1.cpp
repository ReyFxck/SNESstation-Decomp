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

/* Native bulk proof: analysis/functions/native_bulk_dsp1_exact_10892.tsv
 * 35 routines / 10892 complete linked historical instruction bytes.
 * Reviewed native entry points:
 * 0x0012c13c _Z7DSPOp00v (36 bytes)
 * 0x0012c160 _Z7DSPOp20v (40 bytes)
 * 0x0012c188 _Z12DSP1_InversessPsS_ (284 bytes)
 * 0x0012c2a4 _Z7DSPOp10v (56 bytes)
 * 0x0012c2dc _Z6SinInts (188 bytes)
 * 0x0012c398 _Z6CosInts (172 bytes)
 * 0x0012c444 _Z7DSPOp04v (100 bytes)
 * 0x0012c4a8 _Z7DSPOp0Cv (176 bytes)
 * 0x0012ce18 _Z7DSPOp0Av (580 bytes)
 * 0x0012d334 _Z7DSPOp01v (376 bytes)
 * 0x0012d4ac _Z7DSPOp11v (376 bytes)
 * 0x0012d624 _Z7DSPOp21v (376 bytes)
 * 0x0012d79c _Z7DSPOp0Dv (192 bytes)
 * 0x0012d85c _Z7DSPOp1Dv (192 bytes)
 * 0x0012d91c _Z7DSPOp2Dv (192 bytes)
 * 0x0012d9dc _Z7DSPOp03v (192 bytes)
 * 0x0012da9c _Z7DSPOp13v (192 bytes)
 * 0x0012db5c _Z7DSPOp23v (192 bytes)
 * 0x0012dc1c _Z7DSPOp14v (452 bytes)
 * 0x0012dde0 _Z7DSPOp0Ev (128 bytes)
 * 0x0012de60 _Z7DSPOp0Bv (80 bytes)
 * 0x0012deb0 _Z7DSPOp1Bv (80 bytes)
 * 0x0012df00 _Z7DSPOp2Bv (80 bytes)
 * 0x0012df50 _Z7DSPOp08v (96 bytes)
 * 0x0012dfb0 _Z7DSPOp18v (76 bytes)
 * 0x0012dffc _Z7DSPOp38v (80 bytes)
 * 0x0012e0cc _Z7DSPOp1Cv (504 bytes)
 * 0x0012e2c4 _Z7DSPOp0Fv (12 bytes)
 * 0x0012e2d0 _Z7DSPOp2Fv (16 bytes)
 * 0x0012e2e0 _Z9DSP2_Op05v (148 bytes)
 * 0x0012e374 _Z9DSP2_Op01v (500 bytes)
 * 0x0012e568 _Z9DSP2_Op06v (80 bytes)
 * 0x0012e5b8 _Z9DSP2_Op0Dv (208 bytes)
 * 0x0012e750 _Z11DSP1SetByteht (4084 bytes)
 * 0x0012f744 _Z11DSP1GetBytet (356 bytes)
 */
/* Pinned Snes9x native dsp1 recovery; original declarations/macros and bodies expanded with the historical EE profile. */
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
extern struct SSettings Settings;
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
extern void (*SetDSP)(uint8, uint16);
extern uint8 (*GetDSP)(uint16);

void DSP1SetByte(uint8 byte, uint16 address);
uint8 DSP1GetByte(uint16 address);

void DSP2SetByte(uint8 byte, uint16 address);
uint8 DSP2GetByte(uint16 address);

void DSP3SetByte(uint8 byte, uint16 address);
uint8 DSP3GetByte(uint16 address);

void DSP4SetByte(uint8 byte, uint16 address);
uint8 DSP4GetByte(uint16 address);



typedef double MATRIX[3][3];
typedef double VECTOR[3];

enum AttitudeMatrix { MatrixA, MatrixB, MatrixC };

struct SDSP1 {
    bool8 waiting4command;
    bool8 first_parameter;
    uint8 command;
    uint32 in_count;
    uint32 in_index;
    uint32 out_count;
    uint32 out_index;
    uint8 parameters [512];

    uint8 output [512];


    MATRIX vMa;
    MATRIX vMb;
    MATRIX vMc;




    MATRIX vM;
    VECTOR vT;


    double vFov;


    double vPlaneD;


    double vHorizon;


    void ScreenToGround(VECTOR &v, double X2d, double Y2d);

    MATRIX &GetMatrix( AttitudeMatrix Matrix );
};




struct DSP1_Parameter
{
    DSP1_Parameter( int16 Fx, int16 Fy, int16 Fz,
                      uint16 Lfe, uint16 Les,
                      int8 Aas, int8 Azs );


    int16 Vof;



    int16 Vva;




    int16 Cx;
    int16 Cy;
};


struct DSP1_Raster
{
    DSP1_Raster( int16 Vs );



    int16 An;
    int16 Bn;
    int16 Cn;
    int16 Dn;
};


struct DSP1_Project
{
    DSP1_Project( int16 x, int16 y, int16 z );

    int16 H;
    int16 V;
    int16 M;
};


struct DSP1_Target
{
    DSP1_Target( int16 h, int16 v );

    int16 X;
    int16 Y;
};


struct DSP1_Triangle
{
    DSP1_Triangle (int16 Theta, int16 r );
    int16 S;
    int16 C;
};


struct DSP1_Radius
{
    DSP1_Radius( int16 x, int16 y, int16 z );
    int16 Ll;
    int16 Lh;
};


int16 DSP1_Range( int16 x, int16 y, int16 z, int16 r );


int16 DSP1_Distance( int16 x, int16 y, int16 z );


struct DSP1_Rotate
{
    DSP1_Rotate (int16 A, int16 x1, int16 y1);

    int16 x2;
    int16 y2;
};


struct DSP1_Polar
{
    DSP1_Polar( int8 Za, int8 Xa, int8 Ya, int16 x, int16 y, int16 z );

    int16 X;
    int16 Y;
    int16 Z;
};


void DSP1_Attitude( int16 m, int8 Za, int8 Xa, int8 Ya, AttitudeMatrix Matrix );


struct DSP1_Objective
{
    DSP1_Objective( int16 x, int16 y, int16 z, AttitudeMatrix Matrix );

    int16 F;
    int16 L;
    int16 U;
};


struct DSP1_Subjective
{
    DSP1_Subjective( int16 F, int16 L, int16 U, AttitudeMatrix Matrix );

    int16 X;
    int16 Y;
    int16 Z;
};


int16 DSP1_Scalar( int16 x, int16 y, int16 z, AttitudeMatrix Matrix );


struct DSP1_Gyrate
{
    DSP1_Gyrate( int8 Zi, int8 Xi, int8 Yi,
                 int8 dU, int8 dF, int8 dL );

    int8 Z0;
    int8 X0;
    int8 Y0;
};


int16 DSP1_Multiply( int16 k, int16 I );


struct DSP1_Inverse
{
    DSP1_Inverse( int16 a, int16 b );

    int16 A;
    int16 B;
};

extern "C" {
void S9xResetDSP1 ();
uint8 S9xGetDSP (uint16 Address);
void S9xSetDSP (uint8 Byte, uint16 Address);
}

extern struct SDSP1 DSP1 __asm__("DAT_00345628");
struct HDMA
{
    uint8 used;
    uint8 bbus_address;
    uint8 abus_bank;
    uint16 abus_address;
    uint8 indirect_address;
    uint8 force_table_address_write;
    uint8 force_table_address_read;
    uint8 line_count_write;
    uint8 line_count_read;
};

struct Missing
{
    uint8 emulate6502;
    uint8 decimal_mode;
    uint8 mv_8bit_index;
    uint8 mv_8bit_acc;
    uint8 interlace;
    uint8 lines_239;
    uint8 pseudo_512;
    struct HDMA hdma [8];
    uint8 modes [8];
    uint8 mode7_fx;
    uint8 mode7_flip;
    uint8 mode7_bgmode;
    uint8 direct;
    uint8 matrix_multiply;
    uint8 oam_read;
    uint8 vram_read;
    uint8 cgram_read;
    uint8 wram_read;
    uint8 dma_read;
    uint8 vram_inc;
    uint8 vram_full_graphic_inc;
    uint8 virq;
    uint8 hirq;
    uint16 virq_pos;
    uint16 hirq_pos;
    uint8 h_v_latch;
    uint8 h_counter_read;
    uint8 v_counter_read;
    uint8 fast_rom;
    uint8 window1 [6];
    uint8 window2 [6];
    uint8 sprite_priority_rotation;
    uint8 subscreen;
    uint8 subscreen_add;
    uint8 subscreen_sub;
    uint8 fixed_colour_add;
    uint8 fixed_colour_sub;
    uint8 mosaic;
    uint8 sprite_double_height;
    uint8 dma_channels;
    uint8 dma_this_frame;
    uint8 oam_address_read;
    uint8 bg_offset_read;
    uint8 matrix_read;
    uint8 hdma_channels;
    uint8 hdma_this_frame;
    uint16 unknownppu_read;
    uint16 unknownppu_write;
    uint16 unknowncpu_read;
    uint16 unknowncpu_write;
    uint16 unknowndsp_read;
    uint16 unknowndsp_write;
};

extern "C" struct Missing missing;
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
extern "C" {
union __dmath
{
  __uint32_t i[2];
  double d;
};




extern const union __dmath __infinity[];
extern double atan (double);
extern double cos (double);
extern double sin (double);
extern double tan (double);
extern double tanh (double);
extern double frexp (double, int *);
extern double modf (double, double *);
extern double ceil (double);
extern double fabs (double);
extern double floor (double);






extern double acos (double);
extern double asin (double);
extern double atan2 (double, double);
extern double cosh (double);
extern double sinh (double);
extern double exp (double);
extern double ldexp (double, int);
extern double log (double);
extern double log10 (double);
extern double pow (double, double);
extern double sqrt (double);
extern double fmod (double, double);







extern double infinity (void);
extern double nan (void);
extern int isnan (double);
extern int isinf (double);
extern int finite (double);
extern double copysign (double, double);
extern int ilogb (double);

extern double asinh (double);
extern double cbrt (double);
extern double nextafter (double, double);
extern double rint (double);
extern double scalbn (double, int);


extern double log1p (double);
extern double expm1 (double);



extern double acosh (double);
extern double atanh (double);
extern double remainder (double, double);
extern double gamma (double);
extern double gamma_r (double, int *);
extern double lgamma (double);
extern double lgamma_r (double, int *);
extern double erf (double);
extern double erfc (double);
extern double y0 (double);
extern double y1 (double);
extern double yn (int, double);
extern double j0 (double);
extern double j1 (double);
extern double jn (int, double);



extern double hypot (double, double);


extern double cabs();
extern double drem (double, double);
extern float atanf (float);
extern float cosf (float);
extern float sinf (float);
extern float tanf (float);
extern float tanhf (float);
extern float frexpf (float, int *);
extern float modff (float, float *);
extern float ceilf (float);
extern float fabsf (float);
extern float floorf (float);


extern float acosf (float);
extern float asinf (float);
extern float atan2f (float, float);
extern float coshf (float);
extern float sinhf (float);
extern float expf (float);
extern float ldexpf (float, int);
extern float logf (float);
extern float log10f (float);
extern float powf (float, float);
extern float sqrtf (float);
extern float fmodf (float, float);
extern float infinityf (void);
extern float nanf (void);
extern int isnanf (float);
extern int isinff (float);
extern int finitef (float);
extern float copysignf (float, float);
extern int ilogbf (float);

extern float asinhf (float);
extern float cbrtf (float);
extern float nextafterf (float, float);
extern float rintf (float);
extern float scalbnf (float, int);
extern float log1pf (float);
extern float expm1f (float);


extern float acoshf (float);
extern float atanhf (float);
extern float remainderf (float, float);
extern float gammaf (float);
extern float gammaf_r (float, int *);
extern float lgammaf (float);
extern float lgammaf_r (float, int *);
extern float erff (float);
extern float erfcf (float);
extern float y0f (float);
extern float y1f (float);
extern float ynf (int, float);
extern float j0f (float);
extern float j1f (float);
extern float jnf (int, float);

extern float hypotf (float, float);

extern float cabsf();
extern float dremf (float, float);






extern int *__signgam (void);







struct __exception



{
  int type;
  char *name;
  double arg1;
  double arg2;
  double retval;
  int err;
};


extern int matherr (struct __exception *e);
enum __fdlibm_version
{
  __fdlibm_ieee = -1,
  __fdlibm_svid,
  __fdlibm_xopen,
  __fdlibm_posix
};




extern const enum __fdlibm_version __fdlib_version;
}
typedef __gnuc_va_list va_list;
const unsigned short DSP1ROM[1024] __attribute__((section(".rodata.native_dsp1_rom"))) = {
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020,
        0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000,
        0x4000, 0x7fff, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200,
        0x0100, 0x0080, 0x0040, 0x0020, 0x0001, 0x0008, 0x0004, 0x0002,
        0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x8000, 0xffe5, 0x0100, 0x7fff, 0x7f02, 0x7e08,
        0x7d12, 0x7c1f, 0x7b30, 0x7a45, 0x795d, 0x7878, 0x7797, 0x76ba,
        0x75df, 0x7507, 0x7433, 0x7361, 0x7293, 0x71c7, 0x70fe, 0x7038,
        0x6f75, 0x6eb4, 0x6df6, 0x6d3a, 0x6c81, 0x6bca, 0x6b16, 0x6a64,
        0x69b4, 0x6907, 0x685b, 0x67b2, 0x670b, 0x6666, 0x65c4, 0x6523,
        0x6484, 0x63e7, 0x634c, 0x62b3, 0x621c, 0x6186, 0x60f2, 0x6060,
        0x5fd0, 0x5f41, 0x5eb5, 0x5e29, 0x5d9f, 0x5d17, 0x5c91, 0x5c0c,
        0x5b88, 0x5b06, 0x5a85, 0x5a06, 0x5988, 0x590b, 0x5890, 0x5816,
        0x579d, 0x5726, 0x56b0, 0x563b, 0x55c8, 0x5555, 0x54e4, 0x5474,
        0x5405, 0x5398, 0x532b, 0x52bf, 0x5255, 0x51ec, 0x5183, 0x511c,
        0x50b6, 0x5050, 0x4fec, 0x4f89, 0x4f26, 0x4ec5, 0x4e64, 0x4e05,
        0x4da6, 0x4d48, 0x4cec, 0x4c90, 0x4c34, 0x4bda, 0x4b81, 0x4b28,
        0x4ad0, 0x4a79, 0x4a23, 0x49cd, 0x4979, 0x4925, 0x48d1, 0x487f,
        0x482d, 0x47dc, 0x478c, 0x473c, 0x46ed, 0x469f, 0x4651, 0x4604,
        0x45b8, 0x456c, 0x4521, 0x44d7, 0x448d, 0x4444, 0x43fc, 0x43b4,
        0x436d, 0x4326, 0x42e0, 0x429a, 0x4255, 0x4211, 0x41cd, 0x4189,
        0x4146, 0x4104, 0x40c2, 0x4081, 0x4040, 0x3fff, 0x41f7, 0x43e1,
        0x45bd, 0x478d, 0x4951, 0x4b0b, 0x4cbb, 0x4e61, 0x4fff, 0x5194,
        0x5322, 0x54a9, 0x5628, 0x57a2, 0x5914, 0x5a81, 0x5be9, 0x5d4a,
        0x5ea7, 0x5fff, 0x6152, 0x62a0, 0x63ea, 0x6530, 0x6672, 0x67b0,
        0x68ea, 0x6a20, 0x6b53, 0x6c83, 0x6daf, 0x6ed9, 0x6fff, 0x7122,
        0x7242, 0x735f, 0x747a, 0x7592, 0x76a7, 0x77ba, 0x78cb, 0x79d9,
        0x7ae5, 0x7bee, 0x7cf5, 0x7dfa, 0x7efe, 0x7fff, 0x0000, 0x0324,
        0x0647, 0x096a, 0x0c8b, 0x0fab, 0x12c8, 0x15e2, 0x18f8, 0x1c0b,
        0x1f19, 0x2223, 0x2528, 0x2826, 0x2b1f, 0x2e11, 0x30fb, 0x33de,
        0x36ba, 0x398c, 0x3c56, 0x3f17, 0x41ce, 0x447a, 0x471c, 0x49b4,
        0x4c3f, 0x4ebf, 0x5133, 0x539b, 0x55f5, 0x5842, 0x5a82, 0x5cb4,
        0x5ed7, 0x60ec, 0x62f2, 0x64e8, 0x66cf, 0x68a6, 0x6a6d, 0x6c24,
        0x6dca, 0x6f5f, 0x70e2, 0x7255, 0x73b5, 0x7504, 0x7641, 0x776c,
        0x7884, 0x798a, 0x7a7d, 0x7b5d, 0x7c29, 0x7ce3, 0x7d8a, 0x7e1d,
        0x7e9d, 0x7f09, 0x7f62, 0x7fa7, 0x7fd8, 0x7ff6, 0x7fff, 0x7ff6,
        0x7fd8, 0x7fa7, 0x7f62, 0x7f09, 0x7e9d, 0x7e1d, 0x7d8a, 0x7ce3,
        0x7c29, 0x7b5d, 0x7a7d, 0x798a, 0x7884, 0x776c, 0x7641, 0x7504,
        0x73b5, 0x7255, 0x70e2, 0x6f5f, 0x6dca, 0x6c24, 0x6a6d, 0x68a6,
        0x66cf, 0x64e8, 0x62f2, 0x60ec, 0x5ed7, 0x5cb4, 0x5a82, 0x5842,
        0x55f5, 0x539b, 0x5133, 0x4ebf, 0x4c3f, 0x49b4, 0x471c, 0x447a,
        0x41ce, 0x3f17, 0x3c56, 0x398c, 0x36ba, 0x33de, 0x30fb, 0x2e11,
        0x2b1f, 0x2826, 0x2528, 0x2223, 0x1f19, 0x1c0b, 0x18f8, 0x15e2,
        0x12c8, 0x0fab, 0x0c8b, 0x096a, 0x0647, 0x0324, 0x7fff, 0x7ff6,
        0x7fd8, 0x7fa7, 0x7f62, 0x7f09, 0x7e9d, 0x7e1d, 0x7d8a, 0x7ce3,
        0x7c29, 0x7b5d, 0x7a7d, 0x798a, 0x7884, 0x776c, 0x7641, 0x7504,
        0x73b5, 0x7255, 0x70e2, 0x6f5f, 0x6dca, 0x6c24, 0x6a6d, 0x68a6,
        0x66cf, 0x64e8, 0x62f2, 0x60ec, 0x5ed7, 0x5cb4, 0x5a82, 0x5842,
        0x55f5, 0x539b, 0x5133, 0x4ebf, 0x4c3f, 0x49b4, 0x471c, 0x447a,
        0x41ce, 0x3f17, 0x3c56, 0x398c, 0x36ba, 0x33de, 0x30fb, 0x2e11,
        0x2b1f, 0x2826, 0x2528, 0x2223, 0x1f19, 0x1c0b, 0x18f8, 0x15e2,
        0x12c8, 0x0fab, 0x0c8b, 0x096a, 0x0647, 0x0324, 0x0000, 0xfcdc,
        0xf9b9, 0xf696, 0xf375, 0xf055, 0xed38, 0xea1e, 0xe708, 0xe3f5,
        0xe0e7, 0xdddd, 0xdad8, 0xd7da, 0xd4e1, 0xd1ef, 0xcf05, 0xcc22,
        0xc946, 0xc674, 0xc3aa, 0xc0e9, 0xbe32, 0xbb86, 0xb8e4, 0xb64c,
        0xb3c1, 0xb141, 0xaecd, 0xac65, 0xaa0b, 0xa7be, 0xa57e, 0xa34c,
        0xa129, 0x9f14, 0x9d0e, 0x9b18, 0x9931, 0x975a, 0x9593, 0x93dc,
        0x9236, 0x90a1, 0x8f1e, 0x8dab, 0x8c4b, 0x8afc, 0x89bf, 0x8894,
        0x877c, 0x8676, 0x8583, 0x84a3, 0x83d7, 0x831d, 0x8276, 0x81e3,
        0x8163, 0x80f7, 0x809e, 0x8059, 0x8028, 0x800a, 0x6488, 0x0080,
        0x03ff, 0x0116, 0x0002, 0x0080, 0x4000, 0x3fd7, 0x3faf, 0x3f86,
        0x3f5d, 0x3f34, 0x3f0c, 0x3ee3, 0x3eba, 0x3e91, 0x3e68, 0x3e40,
        0x3e17, 0x3dee, 0x3dc5, 0x3d9c, 0x3d74, 0x3d4b, 0x3d22, 0x3cf9,
        0x3cd0, 0x3ca7, 0x3c7f, 0x3c56, 0x3c2d, 0x3c04, 0x3bdb, 0x3bb2,
        0x3b89, 0x3b60, 0x3b37, 0x3b0e, 0x3ae5, 0x3abc, 0x3a93, 0x3a69,
        0x3a40, 0x3a17, 0x39ee, 0x39c5, 0x399c, 0x3972, 0x3949, 0x3920,
        0x38f6, 0x38cd, 0x38a4, 0x387a, 0x3851, 0x3827, 0x37fe, 0x37d4,
        0x37aa, 0x3781, 0x3757, 0x372d, 0x3704, 0x36da, 0x36b0, 0x3686,
        0x365c, 0x3632, 0x3609, 0x35df, 0x35b4, 0x358a, 0x3560, 0x3536,
        0x350c, 0x34e1, 0x34b7, 0x348d, 0x3462, 0x3438, 0x340d, 0x33e3,
        0x33b8, 0x338d, 0x3363, 0x3338, 0x330d, 0x32e2, 0x32b7, 0x328c,
        0x3261, 0x3236, 0x320b, 0x31df, 0x31b4, 0x3188, 0x315d, 0x3131,
        0x3106, 0x30da, 0x30ae, 0x3083, 0x3057, 0x302b, 0x2fff, 0x2fd2,
        0x2fa6, 0x2f7a, 0x2f4d, 0x2f21, 0x2ef4, 0x2ec8, 0x2e9b, 0x2e6e,
        0x2e41, 0x2e14, 0x2de7, 0x2dba, 0x2d8d, 0x2d60, 0x2d32, 0x2d05,
        0x2cd7, 0x2ca9, 0x2c7b, 0x2c4d, 0x2c1f, 0x2bf1, 0x2bc3, 0x2b94,
        0x2b66, 0x2b37, 0x2b09, 0x2ada, 0x2aab, 0x2a7c, 0x2a4c, 0x2a1d,
        0x29ed, 0x29be, 0x298e, 0x295e, 0x292e, 0x28fe, 0x28ce, 0x289d,
        0x286d, 0x283c, 0x280b, 0x27da, 0x27a9, 0x2777, 0x2746, 0x2714,
        0x26e2, 0x26b0, 0x267e, 0x264c, 0x2619, 0x25e7, 0x25b4, 0x2581,
        0x254d, 0x251a, 0x24e6, 0x24b2, 0x247e, 0x244a, 0x2415, 0x23e1,
        0x23ac, 0x2376, 0x2341, 0x230b, 0x22d6, 0x229f, 0x2269, 0x2232,
        0x21fc, 0x21c4, 0x218d, 0x2155, 0x211d, 0x20e5, 0x20ad, 0x2074,
        0x203b, 0x2001, 0x1fc7, 0x1f8d, 0x1f53, 0x1f18, 0x1edd, 0x1ea1,
        0x1e66, 0x1e29, 0x1ded, 0x1db0, 0x1d72, 0x1d35, 0x1cf6, 0x1cb8,
        0x1c79, 0x1c39, 0x1bf9, 0x1bb8, 0x1b77, 0x1b36, 0x1af4, 0x1ab1,
        0x1a6e, 0x1a2a, 0x19e6, 0x19a1, 0x195c, 0x1915, 0x18ce, 0x1887,
        0x183f, 0x17f5, 0x17ac, 0x1761, 0x1715, 0x16c9, 0x167c, 0x162e,
        0x15df, 0x158e, 0x153d, 0x14eb, 0x1497, 0x1442, 0x13ec, 0x1395,
        0x133c, 0x12e2, 0x1286, 0x1228, 0x11c9, 0x1167, 0x1104, 0x109e,
        0x1036, 0x0fcc, 0x0f5f, 0x0eef, 0x0e7b, 0x0e04, 0x0d89, 0x0d0a,
        0x0c86, 0x0bfd, 0x0b6d, 0x0ad6, 0x0a36, 0x098d, 0x08d7, 0x0811,
        0x0736, 0x063e, 0x0519, 0x039a, 0x0000, 0x7fff, 0x0100, 0x0080,
        0x021d, 0x00c8, 0x00ce, 0x0048, 0x0a26, 0x277a, 0x00ce, 0x6488,
        0x14ac, 0x0001, 0x00f9, 0x00fc, 0x00ff, 0x00fc, 0x00f9, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
        0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff};
double CosTable2[2048];
double SinTable2[2048];


double Atan(double x)
;
void InitDSP(void)
;


short Op00Multiplicand;
short Op00Multiplier;
short Op00Result;

void DSPOp00()
{



   Op00Result=Op00Multiplicand*Op00Multiplier >> 15;



}

short Op20Multiplicand;
short Op20Multiplier;
short Op20Result;

void DSPOp20()
{



   Op20Result=Op20Multiplicand*Op20Multiplier >> 15;
   Op20Result++;



}

signed short Op10Coefficient;
signed short Op10Exponent;
signed short Op10CoefficientR;
signed short Op10ExponentR;


short InvTable[128] = {
        0x7fff, 0x7f02, 0x7e08, 0x7d12, 0x7c1f, 0x7b30, 0x7a45, 0x795d,
        0x7878, 0x7797, 0x76ba, 0x75df, 0x7507, 0x7433, 0x7361, 0x7293,
        0x71c7, 0x70fe, 0x7038, 0x6f75, 0x6eb4, 0x6df6, 0x6d3a, 0x6c81,
        0x6bca, 0x6b16, 0x6a64, 0x69b4, 0x6907, 0x685b, 0x67b2, 0x670b,
        0x6666, 0x65c4, 0x6523, 0x6484, 0x63e7, 0x634c, 0x62b3, 0x621c,
        0x6186, 0x60f2, 0x6060, 0x5fd0, 0x5f41, 0x5eb5, 0x5e29, 0x5d9f,
        0x5d17, 0x5c91, 0x5c0c, 0x5b88, 0x5b06, 0x5a85, 0x5a06, 0x5988,
        0x590b, 0x5890, 0x5816, 0x579d, 0x5726, 0x56b0, 0x563b, 0x55c8,
        0x5555, 0x54e4, 0x5474, 0x5405, 0x5398, 0x532b, 0x52bf, 0x5255,
        0x51ec, 0x5183, 0x511c, 0x50b6, 0x5050, 0x4fec, 0x4f89, 0x4f26,
        0x4ec5, 0x4e64, 0x4e05, 0x4da6, 0x4d48, 0x4cec, 0x4c90, 0x4c34,
        0x4bda, 0x4b81, 0x4b28, 0x4ad0, 0x4a79, 0x4a23, 0x49cd, 0x4979,
        0x4925, 0x48d1, 0x487f, 0x482d, 0x47dc, 0x478c, 0x473c, 0x46ed,
        0x469f, 0x4651, 0x4604, 0x45b8, 0x456c, 0x4521, 0x44d7, 0x448d,
        0x4444, 0x43fc, 0x43b4, 0x436d, 0x4326, 0x42e0, 0x429a, 0x4255,
        0x4211, 0x41cd, 0x4189, 0x4146, 0x4104, 0x40c2, 0x4081, 0x4040};

void DSP1_Inverse(short Coefficient, short Exponent, short *iCoefficient, short *iExponent)
{

        if (Coefficient == 0x0000)
        {
                *iCoefficient = 0x7fff;
                *iExponent = 0x002f;
        }
        else
        {
                short Sign = 1;


                if (Coefficient < 0)
                {
                        if (Coefficient < -32767) Coefficient = -32767;
                        Coefficient = -Coefficient;
                        Sign = -1;
                }


                while (Coefficient < 0x4000)
                {
                        Coefficient <<= 1;
                        Exponent--;
                }


                if (Coefficient == 0x4000)
                        if (Sign == 1) *iCoefficient = 0x7fff;
                        else {
                                *iCoefficient = -0x4000;
                                Exponent--;
                        }
                else {

                        short i = InvTable[(Coefficient - 0x4000) >> 7];


                        i = (i << 1) + (((-i * ((Coefficient * i) >> 15)) >> 15) << 1);
                        i = (i << 1) + (((-i * ((Coefficient * i) >> 15)) >> 15) << 1);

                        *iCoefficient = i * Sign;
                }

                *iExponent = 1 - Exponent;
        }
}


void DSPOp10()
{
        DSP1_Inverse(Op10Coefficient, Op10Exponent, &Op10CoefficientR, &Op10ExponentR);



}

short Op04Angle;
short Op04Radius;
short Op04Sin;
short Op04Cos;

short MulTable[256] = {
        0, 3, 6, 9, 12, 15, 18, 21,
        25, 28, 31, 34, 37, 40, 43, 47,
        50, 53, 56, 59, 62, 65, 69, 72,
        75, 78, 81, 84, 87, 91, 94, 97,
        100, 103, 106, 109, 113, 116, 119, 122,
        125, 128, 131, 135, 138, 141, 144, 147,
        150, 153, 157, 160, 163, 166, 169, 172,
        175, 179, 182, 185, 188, 191, 194, 197,
        201, 204, 207, 210, 213, 216, 219, 223,
        226, 229, 232, 235, 238, 241, 245, 248,
        251, 254, 257, 260, 263, 267, 270, 273,
        276, 279, 282, 285, 289, 292, 295, 298,
        301, 304, 307, 311, 314, 317, 320, 323,
        326, 329, 333, 336, 339, 342, 345, 348,
        351, 355, 358, 361, 364, 367, 370, 373,
        376, 380, 383, 386, 389, 392, 395, 398,
        402, 405, 408, 411, 414, 417, 420, 424,
        427, 430, 433, 436, 439, 442, 446, 449,
        452, 455, 458, 461, 464, 468, 471, 474,
        477, 480, 483, 486, 490, 493, 496, 499,
        502, 505, 508, 512, 515, 518, 521, 524,
        527, 530, 534, 537, 540, 543, 546, 549,
        552, 556, 559, 562, 565, 568, 571, 574,
        578, 581, 584, 587, 590, 593, 596, 600,
        603, 606, 609, 612, 615, 618, 622, 625,
        628, 631, 634, 637, 640, 644, 647, 650,
        653, 656, 659, 662, 666, 669, 672, 675,
        678, 681, 684, 688, 691, 694, 697, 700,
        703, 706, 710, 713, 716, 719, 722, 725,
        728, 731, 735, 738, 741, 744, 747, 750,
        753, 757, 760, 763, 766, 769, 772, 775,
        779, 782, 785, 788, 791, 794, 797, 801};

short DSPSinTable[256] = {
        0, 804, 1607, 2410, 3211, 4011, 4808, 5602,
        6392, 7179, 7961, 8739, 9512, 10278, 11039, 11793,
        12539, 13278, 14010, 14732, 15446, 16151, 16846, 17530,
        18204, 18868, 19519, 20159, 20787, 21403, 22005, 22594,
        23170, 23732, 24279, 24812, 25330, 25832, 26319, 26790,
        27245, 27684, 28106, 28511, 28898, 29269, 29621, 29956,
        30273, 30572, 30852, 31114, 31357, 31581, 31785, 31971,
        32138, 32285, 32413, 32521, 32610, 32679, 32728, 32758,
        32767, 32758, 32728, 32679, 32610, 32521, 32413, 32285,
        32138, 31971, 31785, 31581, 31357, 31114, 30852, 30572,
        30273, 29956, 29621, 29269, 28898, 28511, 28106, 27684,
        27245, 26790, 26319, 25832, 25330, 24812, 24279, 23732,
        23170, 22594, 22005, 21403, 20787, 20159, 19519, 18868,
        18204, 17530, 16846, 16151, 15446, 14732, 14010, 13278,
        12539, 11793, 11039, 10278, 9512, 8739, 7961, 7179,
        6392, 5602, 4808, 4011, 3211, 2410, 1607, 804,
        0, -804, -1607, -2410, -3211, -4011, -4808, -5602,
        -6392, -7179, -7961, -8739, -9512, -10278, -11039, -11793,
        -12539, -13278, -14010, -14732, -15446, -16151, -16846, -17530,
        -18204, -18868, -19519, -20159, -20787, -21403, -22005, -22594,
        -23170, -23732, -24279, -24812, -25330, -25832, -26319, -26790,
        -27245, -27684, -28106, -28511, -28898, -29269, -29621, -29956,
        -30273, -30572, -30852, -31114, -31357, -31581, -31785, -31971,
        -32138, -32285, -32413, -32521, -32610, -32679, -32728, -32758,
        -32767, -32758, -32728, -32679, -32610, -32521, -32413, -32285,
        -32138, -31971, -31785, -31581, -31357, -31114, -30852, -30572,
        -30273, -29956, -29621, -29269, -28898, -28511, -28106, -27684,
        -27245, -26790, -26319, -25832, -25330, -24812, -24279, -23732,
        -23170, -22594, -22005, -21403, -20787, -20159, -19519, -18868,
        -18204, -17530, -16846, -16151, -15446, -14732, -14010, -13278,
        -12539, -11793, -11039, -10278, -9512, -8739, -7961, -7179,
        -6392, -5602, -4808, -4011, -3211, -2410, -1607, -804};





short SinInt(short Angle)
{
        if (Angle == -32768)
                return 0;

        if (Angle < 0)
                return -SinInt(-Angle);
        else
        {
                int S = DSPSinTable[(Angle >> 8)] + ((MulTable[(Angle & 0xff)] * DSPSinTable[((Angle + 0x4000) >> 8)]) >> 15);
                if (S > 32767)
                        S = 32767;
                if (S < -32768)
                        S = -32767;
                return (short) S;
        }
}

short CosInt(short Angle)
{
    int S;
        if (Angle == -32768) return -32768;

        if (Angle < 0) Angle = -Angle;

        S = DSPSinTable[((Angle + 0x4000) >> 8)] - ((MulTable[(Angle & 0xff)] * (-DSPSinTable[((Angle + 0x8000) >> 8)])) >> 15);
        if (S > 32767)
                S = 32767;
        if (S < -32768)
                S = -32767;
        return (short) S;
}

void DSPOp04()
{
        Op04Sin = SinInt(Op04Angle) * Op04Radius >> 15;
        Op04Cos = CosInt(Op04Angle) * Op04Radius >> 15;
}

short Op0CA;
short Op0CX1;
short Op0CY1;
short Op0CX2;
short Op0CY2;


void DSPOp0C()
{
        Op0CX2 = (Op0CY1 * SinInt(Op0CA) >> 15) + (Op0CX1 * CosInt(Op0CA) >> 15);
        Op0CY2 = (Op0CY1 * CosInt(Op0CA) >> 15) - (Op0CX1 * SinInt(Op0CA) >> 15);
}


short Op02FX;
short Op02FY;
short Op02FZ;
short Op02LFE;
short Op02LES;
unsigned short Op02AAS;
unsigned short Op02AZS;
unsigned short Op02VOF;
unsigned short Op02VVA;

short Op02CX;
short Op02CY;
double Op02CXF;
double Op02CYF;
double ViewerX0;
double ViewerY0;
double ViewerZ0;
double ViewerX1;
double ViewerY1;
double ViewerZ1;
double ViewerX;
double ViewerY;
double ViewerZ;
int ViewerAX;
int ViewerAY;
int ViewerAZ;
double NumberOfSlope;
double ScreenX;
double ScreenY;
double ScreenZ;
double TopLeftScreenX;
double TopLeftScreenY;
double TopLeftScreenZ;
double BottomRightScreenX;
double BottomRightScreenY;
double BottomRightScreenZ;
double Ready;
double RasterLX;
double RasterLY;
double RasterLZ;
double ScreenLX1;
double ScreenLY1;
double ScreenLZ1;
int ReversedLES;
short Op02LESb;
double NAzsB,NAasB;
double ViewerXc;
double ViewerYc;
double ViewerZc;
double CenterX,CenterY;
short Op02CYSup,Op02CXSup;
double CXdistance;



short TValDebug,TValDebug2;
short ScrDispl;



void DSPOp02()
;
short Op0AVS;
short Op0AA;
short Op0AB;
short Op0AC;
short Op0AD;

double RasterRX;
double RasterRY;
double RasterRZ;
double RasterLSlopeX;
double RasterLSlopeY;
double RasterLSlopeZ;
double RasterRSlopeX;
double RasterRSlopeY;
double RasterRSlopeZ;
double GroundLX;
double GroundLY;
double GroundRX;
double GroundRY;
double Distance;

double NAzs,NAas;
double RVPos,RHPos,RXRes,RYRes;


void GetRXYPos();

void DSPOp0A()
{
  double x2,y2,x3,y3,x4,y4,m,ypos;


   if(Op0AVS==0) {Op0AVS++; return;}
   ypos=Op0AVS-ScrDispl;


   RVPos = ypos; RHPos = 0;
   GetRXYPos(); x2 = RXRes; y2 = RYRes;

   RVPos = ypos; RHPos = -128;
   GetRXYPos(); x3 = RXRes; y3 = RYRes;

   RVPos = ypos; RHPos = 127;
   GetRXYPos(); x4 = RXRes; y4 = RYRes;


   m = (x4-x3)/256*256; if (m>32767) m=32767; if (m<-32768) m=-32768;
   Op0AA = (short)(m);

   m = (y4-y3)/256*256; if (m>32767) m=32767; if (m<-32768) m=-32768;
   Op0AC = (short)(m);
   if (ypos==0){
     Op0AB = 0;
     Op0AD = 0;
   }
   else {

     m = (x2-CenterX)/ypos*256; if (m>32767) m=32767; if (m<-32768) m=-32768;
     Op0AB = (short)(m);

     m = (y2-CenterY)/ypos*256; if (m>32767) m=32767; if (m<-32768) m=-32768;
     Op0AD = (short)(m);
   }

   Op0AVS+=1;
}

short Op06X;
short Op06Y;
short Op06Z;
short Op06H;
short Op06V;
unsigned short Op06S;

double ObjPX;
double ObjPY;
double ObjPZ;
double ObjPX1;
double ObjPY1;
double ObjPZ1;
double ObjPX2;
double ObjPY2;
double ObjPZ2;
double DivideOp06;
int Temp;
int tanval2;


void DSPOp06()
;
short matrixC[3][3];
short matrixB[3][3];
short matrixA[3][3];

short Op01m;
short Op01Zr;
short Op01Xr;
short Op01Yr;
short Op11m;
short Op11Zr;
short Op11Xr;
short Op11Yr;
short Op21m;
short Op21Zr;
short Op21Xr;
short Op21Yr;
double sc,sc2,sc3;

void DSPOp01 ()
{
        short SinAz = SinInt(Op01Zr);
        short CosAz = CosInt(Op01Zr);
        short SinAy = SinInt(Op01Yr);
        short CosAy = CosInt(Op01Yr);
        short SinAx = SinInt(Op01Xr);
        short CosAx = CosInt(Op01Xr);

        Op01m >>= 1;

        matrixA[0][0] = (Op01m * CosAz >> 15) * CosAy >> 15;
        matrixA[0][1] = -((Op01m * SinAz >> 15) * CosAy >> 15);
        matrixA[0][2] = Op01m * SinAy >> 15;

        matrixA[1][0] = ((Op01m * SinAz >> 15) * CosAx >> 15) + (((Op01m * CosAz >> 15) * SinAx >> 15) * SinAy >> 15);
        matrixA[1][1] = ((Op01m * CosAz >> 15) * CosAx >> 15) - (((Op01m * SinAz >> 15) * SinAx >> 15) * SinAy >> 15);
        matrixA[1][2] = -((Op01m * SinAx >> 15) * CosAy >> 15);

        matrixA[2][0] = ((Op01m * SinAz >> 15) * SinAx >> 15) - (((Op01m * CosAz >> 15) * CosAx >> 15) * SinAy >> 15);
        matrixA[2][1] = ((Op01m * CosAz >> 15) * SinAx >> 15) + (((Op01m * SinAz >> 15) * CosAx >> 15) * SinAy >> 15);
        matrixA[2][2] = (Op01m * CosAx >> 15) * CosAy >> 15;
}

void DSPOp11()
{
        short SinAz = SinInt(Op11Zr);
        short CosAz = CosInt(Op11Zr);
        short SinAy = SinInt(Op11Yr);
        short CosAy = CosInt(Op11Yr);
        short SinAx = SinInt(Op11Xr);
        short CosAx = CosInt(Op11Xr);

        Op11m >>= 1;

        matrixB[0][0] = (Op11m * CosAz >> 15) * CosAy >> 15;
        matrixB[0][1] = -((Op11m * SinAz >> 15) * CosAy >> 15);
        matrixB[0][2] = Op11m * SinAy >> 15;

        matrixB[1][0] = ((Op11m * SinAz >> 15) * CosAx >> 15) + (((Op11m * CosAz >> 15) * SinAx >> 15) * SinAy >> 15);
        matrixB[1][1] = ((Op11m * CosAz >> 15) * CosAx >> 15) - (((Op11m * SinAz >> 15) * SinAx >> 15) * SinAy >> 15);
        matrixB[1][2] = -((Op11m * SinAx >> 15) * CosAy >> 15);

        matrixB[2][0] = ((Op11m * SinAz >> 15) * SinAx >> 15) - (((Op11m * CosAz >> 15) * CosAx >> 15) * SinAy >> 15);
        matrixB[2][1] = ((Op11m * CosAz >> 15) * SinAx >> 15) + (((Op11m * SinAz >> 15) * CosAx >> 15) * SinAy >> 15);
        matrixB[2][2] = (Op11m * CosAx >> 15) * CosAy >> 15;
}

void DSPOp21()
{
        short SinAz = SinInt(Op21Zr);
        short CosAz = CosInt(Op21Zr);
        short SinAy = SinInt(Op21Yr);
        short CosAy = CosInt(Op21Yr);
        short SinAx = SinInt(Op21Xr);
        short CosAx = CosInt(Op21Xr);

        Op21m >>= 1;

        matrixC[0][0] = (Op21m * CosAz >> 15) * CosAy >> 15;
        matrixC[0][1] = -((Op21m * SinAz >> 15) * CosAy >> 15);
        matrixC[0][2] = Op21m * SinAy >> 15;

        matrixC[1][0] = ((Op21m * SinAz >> 15) * CosAx >> 15) + (((Op21m * CosAz >> 15) * SinAx >> 15) * SinAy >> 15);
        matrixC[1][1] = ((Op21m * CosAz >> 15) * CosAx >> 15) - (((Op21m * SinAz >> 15) * SinAx >> 15) * SinAy >> 15);
        matrixC[1][2] = -((Op21m * SinAx >> 15) * CosAy >> 15);

        matrixC[2][0] = ((Op21m * SinAz >> 15) * SinAx >> 15) - (((Op21m * CosAz >> 15) * CosAx >> 15) * SinAy >> 15);
        matrixC[2][1] = ((Op21m * CosAz >> 15) * SinAx >> 15) + (((Op21m * SinAz >> 15) * CosAx >> 15) * SinAy >> 15);
        matrixC[2][2] = (Op21m * CosAx >> 15) * CosAy >> 15;
}

short Op0DX;
short Op0DY;
short Op0DZ;
short Op0DF;
short Op0DL;
short Op0DU;
short Op1DX;
short Op1DY;
short Op1DZ;
short Op1DF;
short Op1DL;
short Op1DU;
short Op2DX;
short Op2DY;
short Op2DZ;
short Op2DF;
short Op2DL;
short Op2DU;

void DSPOp0D()
{
        Op0DF = (Op0DX * matrixA[0][0] >> 15) + (Op0DY * matrixA[0][1] >> 15) + (Op0DZ * matrixA[0][2] >> 15);
        Op0DL = (Op0DX * matrixA[1][0] >> 15) + (Op0DY * matrixA[1][1] >> 15) + (Op0DZ * matrixA[1][2] >> 15);
        Op0DU = (Op0DX * matrixA[2][0] >> 15) + (Op0DY * matrixA[2][1] >> 15) + (Op0DZ * matrixA[2][2] >> 15);




}

void DSPOp1D()
{
        Op1DF = (Op1DX * matrixB[0][0] >> 15) + (Op1DY * matrixB[0][1] >> 15) + (Op1DZ * matrixB[0][2] >> 15);
        Op1DL = (Op1DX * matrixB[1][0] >> 15) + (Op1DY * matrixB[1][1] >> 15) + (Op1DZ * matrixB[1][2] >> 15);
        Op1DU = (Op1DX * matrixB[2][0] >> 15) + (Op1DY * matrixB[2][1] >> 15) + (Op1DZ * matrixB[2][2] >> 15);




}

void DSPOp2D()
{
        Op2DF = (Op2DX * matrixC[0][0] >> 15) + (Op2DY * matrixC[0][1] >> 15) + (Op2DZ * matrixC[0][2] >> 15);
        Op2DL = (Op2DX * matrixC[1][0] >> 15) + (Op2DY * matrixC[1][1] >> 15) + (Op2DZ * matrixC[1][2] >> 15);
        Op2DU = (Op2DX * matrixC[2][0] >> 15) + (Op2DY * matrixC[2][1] >> 15) + (Op2DZ * matrixC[2][2] >> 15);



}

short Op03F;
short Op03L;
short Op03U;
short Op03X;
short Op03Y;
short Op03Z;
short Op13F;
short Op13L;
short Op13U;
short Op13X;
short Op13Y;
short Op13Z;
short Op23F;
short Op23L;
short Op23U;
short Op23X;
short Op23Y;
short Op23Z;

void DSPOp03()
{
        Op03X = (Op03F * matrixA[0][0] >> 15) + (Op03L * matrixA[1][0] >> 15) + (Op03U * matrixA[2][0] >> 15);
        Op03Y = (Op03F * matrixA[0][1] >> 15) + (Op03L * matrixA[1][1] >> 15) + (Op03U * matrixA[2][1] >> 15);
        Op03Z = (Op03F * matrixA[0][2] >> 15) + (Op03L * matrixA[1][2] >> 15) + (Op03U * matrixA[2][2] >> 15);




}

void DSPOp13()
{
        Op13X = (Op13F * matrixB[0][0] >> 15) + (Op13L * matrixB[1][0] >> 15) + (Op13U * matrixB[2][0] >> 15);
        Op13Y = (Op13F * matrixB[0][1] >> 15) + (Op13L * matrixB[1][1] >> 15) + (Op13U * matrixB[2][1] >> 15);
        Op13Z = (Op13F * matrixB[0][2] >> 15) + (Op13L * matrixB[1][2] >> 15) + (Op13U * matrixB[2][2] >> 15);



}

void DSPOp23()
{
        Op23X = (Op23F * matrixC[0][0] >> 15) + (Op23L * matrixC[1][0] >> 15) + (Op23U * matrixC[2][0] >> 15);
        Op23Y = (Op23F * matrixC[0][1] >> 15) + (Op23L * matrixC[1][1] >> 15) + (Op23U * matrixC[2][1] >> 15);
        Op23Z = (Op23F * matrixC[0][2] >> 15) + (Op23L * matrixC[1][2] >> 15) + (Op23U * matrixC[2][2] >> 15);



}

short Op14Zr;
short Op14Xr;
short Op14Yr;
short Op14U;
short Op14F;
short Op14L;
short Op14Zrr;
short Op14Xrr;
short Op14Yrr;

void DSPOp14()
{
        short Coefficient;
        short Exponent;
        int Rtmp;

        DSP1_Inverse(CosInt(Op14Xr), 0, &Coefficient, &Exponent);


        Rtmp = ((Op14U * CosInt(Op14Yr)) >> 15) - ((Op14F * SinInt(Op14Yr)) >> 15);
        Rtmp = (Coefficient * (short) Rtmp) >> (15 - Exponent);
        if (Rtmp > 32767)
                Rtmp = 32767;
        if (Rtmp < -32768)
                Rtmp = -32767;
        Op14Zrr = Op14Zr + Rtmp;


        Op14Xrr = Op14Xr + ((Op14U * SinInt(Op14Yr)) >> 15) + ((Op14F * CosInt(Op14Yr)) >> 15);


        Rtmp = ((Op14U * CosInt(Op14Yr)) >> 15) + ((Op14F * SinInt(Op14Yr)) >> 15);
        Rtmp = (((Coefficient * (short) Rtmp) >> 15) * SinInt(Op14Xr)) >> (15 - Exponent);
        if (Rtmp > 32767)
                Rtmp = 32767;
        if (Rtmp < -32768)
                Rtmp = -32767;
        Op14Yrr = Op14Yr - Rtmp + Op14L;
}

short Op0EH;
short Op0EV;
short Op0EX;
short Op0EY;

void DSPOp0E()
{


   RVPos = Op0EV;
   RHPos = Op0EH;
   GetRXYPos();
   Op0EX = (short)(RXRes);
   Op0EY = (short)(RYRes);




}

short Op0BX;
short Op0BY;
short Op0BZ;
short Op0BS;
short Op1BX;
short Op1BY;
short Op1BZ;
short Op1BS;
short Op2BX;
short Op2BY;
short Op2BZ;
short Op2BS;

void DSPOp0B()
{
    Op0BS = ((Op0BX*matrixA[0][0]+Op0BY*matrixA[0][1]+Op0BZ*matrixA[0][2])>>15);



}

void DSPOp1B()
{
    Op1BS = ((Op1BX*matrixB[0][0]+Op1BY*matrixB[0][1]+Op1BZ*matrixB[0][2])>>15);





}

void DSPOp2B()
{
    Op2BS = (short)((Op2BX*matrixC[0][0]+Op2BY*matrixC[0][1]+Op2BZ*matrixC[0][2])>>15);



}

short Op08X,Op08Y,Op08Z,Op08Ll,Op08Lh;
long Op08Size;

void DSPOp08()
{


   Op08Size=(Op08X*Op08X+Op08Y*Op08Y+Op08Z*Op08Z)*2;
   Op08Ll = Op08Size&0xFFFF;
   Op08Lh = (Op08Size>>16) & 0xFFFF;




}

short Op18X,Op18Y,Op18Z,Op18R,Op18D;

void DSPOp18()
{


   int x,y,z,r;
   x=Op18X; y=Op18Y; z=Op18Z; r=Op18R;
   r = (x*x+y*y+z*z-r*r);
   Op18D=(short) (r >> 15);



}

short Op38X,Op38Y,Op38Z,Op38R,Op38D;

void DSPOp38()
{


   int x,y,z,r;
   x=Op38X; y=Op38Y; z=Op38Z; r=Op38R;
   r = (x*x+y*y+z*z-r*r);
   Op38D=(short) (r >> 15);
   Op38D++;



}


short Op28X;
short Op28Y;
short Op28Z;
short Op28R;

void DSPOp28()
;

short Op1CX,Op1CY,Op1CZ;
short Op1CXBR,Op1CYBR,Op1CZBR,Op1CXAR,Op1CYAR,Op1CZAR;
short Op1CX1;
short Op1CY1;
short Op1CZ1;
short Op1CX2;
short Op1CY2;
short Op1CZ2;

void DSPOp1C()
{

   Op1CX1=((Op1CXBR*CosInt(Op1CZ))>>15)+((Op1CYBR*SinInt(Op1CZ))>>15);
   Op1CY1=-(((Op1CXBR*SinInt(Op1CZ))>>15)+((Op1CYBR*CosInt(Op1CZ))>>15));
   Op1CZ1=Op1CZBR;

   Op1CX2=((Op1CX1*CosInt(Op1CY))>>15)-((Op1CZ1*SinInt(Op1CY))>>15);
   Op1CY2=Op1CY1;
   Op1CZ2=((Op1CX1*SinInt(Op1CY))>>15)+((Op1CZ1*CosInt(Op1CY))>>15);

   Op1CXAR=Op1CX2;
   Op1CYAR=((Op1CY2*CosInt(Op1CX))>>15)+((Op1CZ2*SinInt(Op1CX))>>15);
   Op1CZAR=-(((Op1CY2*-SinInt(Op1CX))>>15)+((Op1CZ2*CosInt(Op1CX))>>15));




}



unsigned short Op0FRamsize;
unsigned short Op0FPass;

void DSPOp0F()
{

   Op0FPass = 0x0000;
   return;




}

short Op2FUnknown;
short Op2FSize;

void DSPOp2F()
{
        Op2FSize=0x100;
}
uint16 DSP2Op09Word1=0;
uint16 DSP2Op09Word2=0;
bool DSP2Op05HasLen=false;
int DSP2Op05Len=0;
bool DSP2Op06HasLen=false;
int DSP2Op06Len=0;
uint8 DSP2Op05Transparent=0;

void DSP2_Op05 ()
{
        uint8 color;
        int n;
        unsigned char c1;
        unsigned char c2;
        unsigned char *p1 = DSP1.parameters;
        unsigned char *p2 = &DSP1.parameters[DSP2Op05Len];
        unsigned char *p3 = DSP1.output;

        color = DSP2Op05Transparent&0x0f;

        for( n = 0; n < DSP2Op05Len; n++ )
        {
                c1 = *p1++;
                c2 = *p2++;
                *p3++ = ( ((c2 >> 4) == color ) ? c1 & 0xf0: c2 & 0xf0 ) |
                        ( ((c2 & 0x0f)==color) ? c1 & 0x0f: c2 & 0x0f );
        }
}

void DSP2_Op01 ()
{



        int j;
        unsigned char c0, c1, c2, c3;
        unsigned char *p1 = DSP1.parameters;
        unsigned char *p2a = DSP1.output;
        unsigned char *p2b = &DSP1.output[16];



        for ( j = 0; j < 8; j++ )
        {
                c0 = *p1++;
                c1 = *p1++;
                c2 = *p1++;
                c3 = *p1++;

                *p2a++ = (c0 & 0x10) << 3 |
                             (c0 & 0x01) << 6 |
                             (c1 & 0x10) << 1 |
                             (c1 & 0x01) << 4 |
                             (c2 & 0x10) >> 1 |
                             (c2 & 0x01) << 2 |
                             (c3 & 0x10) >> 3 |
                             (c3 & 0x01);

                *p2a++ = (c0 & 0x20) << 2 |
                             (c0 & 0x02) << 5 |
                             (c1 & 0x20) |
                             (c1 & 0x02) << 3 |
                             (c2 & 0x20) >> 2 |
                             (c2 & 0x02) << 1 |
                             (c3 & 0x20) >> 4 |
                             (c3 & 0x02) >> 1;

                *p2b++ = (c0 & 0x40) << 1 |
                             (c0 & 0x04) << 4 |
                             (c1 & 0x40) >> 1 |
                             (c1 & 0x04) << 2 |
                             (c2 & 0x40) >> 3 |
                             (c2 & 0x04) |
                             (c3 & 0x40) >> 5 |
                             (c3 & 0x04) >> 2;


                *p2b++ = (c0 & 0x80) |
                             (c0 & 0x08) << 3 |
                             (c1 & 0x80) >> 2 |
                             (c1 & 0x08) << 1 |
                             (c2 & 0x80) >> 4 |
                             (c2 & 0x08) >> 1 |
                             (c3 & 0x80) >> 6 |
                             (c3 & 0x08) >> 3;
        }
        return;
}

void DSP2_Op06 ()
{




        int i, j;

        for ( i = 0, j = DSP2Op06Len - 1; i < DSP2Op06Len; i++, j-- )
        {
                DSP1.output[j] = (DSP1.parameters[i] << 4) | (DSP1.parameters[i] >> 4);
        }
}

bool DSP2Op0DHasLen=false;
int DSP2Op0DOutLen=0;
int DSP2Op0DInLen=0;





void DSP2_Op0D()
{
        int i;
        int pixel_offset;
        uint8 pixelarray[512];

        for(i=0; i<DSP2Op0DOutLen*2; i++)
        {
                pixel_offset = (i * DSP2Op0DInLen) / DSP2Op0DOutLen;
                if ( (pixel_offset&1) == 0 )
                        pixelarray[i] = DSP1.parameters[pixel_offset>>1] >> 4;
                else
                        pixelarray[i] = DSP1.parameters[pixel_offset>>1] & 0x0f;
        }

        for ( i=0; i < DSP2Op0DOutLen; i++ )
                DSP1.output[i] = ( pixelarray[i<<1] << 4 ) | pixelarray[(i<<1)+1];
}
void (*SetDSP)(uint8, uint16)=&DSP1SetByte;
uint8 (*GetDSP)(uint16)=&DSP1GetByte;

void S9xInitDSP1 ()
;

void S9xResetDSP1 ()
;

uint8 S9xGetDSP (uint16 address)
;

void S9xSetDSP (uint8 byte, uint16 address)
;


void DSP1SetByte(uint8 byte, uint16 address)
{
    if( (address & 0xf000) == 0x6000 || (address & 0x7fff) < 0x4000 )
    {


                if((DSP1.command==0x0A||DSP1.command==0x1A)&&DSP1.out_count!=0)
                {
                        DSP1.out_count--;
                        DSP1.out_index++;
                        return;
                }
                else if (DSP1.waiting4command)
                {
                        DSP1.command = byte;
                        DSP1.in_index = 0;
                        DSP1.waiting4command = 0;
                        DSP1.first_parameter = 1;


                        switch (byte)
                        {
                        case 0x00: DSP1.in_count = 2; break;
                        case 0x30:
                        case 0x10: DSP1.in_count = 2; break;
                        case 0x20: DSP1.in_count = 2; break;
                        case 0x24:
                        case 0x04: DSP1.in_count = 2; break;
                        case 0x08: DSP1.in_count = 3; break;
                        case 0x18: DSP1.in_count = 4; break;
                        case 0x28: DSP1.in_count = 3; break;
                        case 0x38: DSP1.in_count = 4; break;
                        case 0x2c:
                        case 0x0c: DSP1.in_count = 3; break;
                        case 0x3c:
                        case 0x1c: DSP1.in_count = 6; break;
                        case 0x32:
                        case 0x22:
                        case 0x12:
                        case 0x02: DSP1.in_count = 7; break;
                        case 0x0a: DSP1.in_count = 1; break;
                        case 0x3a:
                        case 0x2a:
                        case 0x1a:
                                DSP1. command =0x1a;
                                DSP1.in_count = 1; break;
                        case 0x16:
                        case 0x26:
                        case 0x36:
                        case 0x06: DSP1.in_count = 3; break;
                        case 0x1e:
                        case 0x2e:
                        case 0x3e:
                        case 0x0e: DSP1.in_count = 2; break;
                        case 0x05:
                        case 0x35:
                        case 0x31:
                        case 0x01: DSP1.in_count = 4; break;
                        case 0x15:
                        case 0x11: DSP1.in_count = 4; break;
                        case 0x25:
                        case 0x21: DSP1.in_count = 4; break;
                        case 0x09:
                        case 0x39:
                        case 0x3d:
                        case 0x0d: DSP1.in_count = 3; break;
                        case 0x19:
                        case 0x1d: DSP1.in_count = 3; break;
                        case 0x29:
                        case 0x2d: DSP1.in_count = 3; break;
                        case 0x33:
                        case 0x03: DSP1.in_count = 3; break;
                        case 0x13: DSP1.in_count = 3; break;
                        case 0x23: DSP1.in_count = 3; break;
                        case 0x3b:
                        case 0x0b: DSP1.in_count = 3; break;
                        case 0x1b: DSP1.in_count = 3; break;
                        case 0x2b: DSP1.in_count = 3; break;
                        case 0x34:
                        case 0x14: DSP1.in_count = 6; break;
                        case 0x07:
                        case 0x0f: DSP1.in_count = 1; break;
                        case 0x27:
                        case 0x2F: DSP1.in_count=1; break;
                        case 0x17:
                        case 0x37:
                        case 0x3F:
                                DSP1.command=0x1f;
                        case 0x1f: DSP1.in_count = 1; break;

                        default:

                        case 0x80:
                                DSP1.in_count = 0;
                                DSP1.waiting4command = 1;
                                DSP1.first_parameter = 1;
                                break;
                        }
                        DSP1.in_count<<=1;
                }
                else
                {
                        DSP1.parameters [DSP1.in_index] = byte;
                        DSP1.first_parameter = 0;
                        DSP1.in_index++;
                }

                if (DSP1.waiting4command ||
                        (DSP1.first_parameter && byte == 0x80))
                {
                        DSP1.waiting4command = 1;
                        DSP1.first_parameter = 0;
                }
                else if(DSP1.first_parameter && (DSP1.in_count != 0 || (DSP1.in_count==0&&DSP1.in_index==0)))
                {
                }



                else
                {
                        if (DSP1.in_count)
                        {

                                if (--DSP1.in_count == 0)
                                {

                                        DSP1.waiting4command = 1;
                                        DSP1.out_index = 0;
                                        switch (DSP1.command)
                                        {
                                        case 0x1f:
                                                DSP1.out_count=2048;
                                                break;
                                        case 0x00:
                                                Op00Multiplicand = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op00Multiplier = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));

                                                DSPOp00 ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = Op00Result&0xFF;
                                                DSP1.output [1] = (Op00Result>>8)&0xFF;
                                                break;

                                        case 0x20:
                                                Op20Multiplicand = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op20Multiplier = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));

                                                DSPOp20 ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = Op20Result&0xFF;
                                                DSP1.output [1] = (Op20Result>>8)&0xFF;
                                                break;

                                        case 0x30:
                                        case 0x10:
                                                Op10Coefficient = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op10Exponent = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));

                                                DSPOp10 ();

                                                DSP1.out_count = 4;
                                                DSP1.output [0] = (uint8) (((int16) Op10CoefficientR)&0xFF);
                                                DSP1.output [1] = (uint8) ((((int16) Op10CoefficientR)>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (((int16) Op10ExponentR)&0xff);
                                                DSP1.output [3] = (uint8) ((((int16) Op10ExponentR)>>8)&0xff);
                                                break;

                                        case 0x24:
                                        case 0x04:
                                                Op04Angle = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op04Radius = (uint16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));

                                                DSPOp04 ();

                                                DSP1.out_count = 4;
                                                DSP1.output [0] = (uint8) (Op04Sin&0xFF);
                                                DSP1.output [1] = (uint8) ((Op04Sin>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op04Cos&0xFF);
                                                DSP1.output [3] = (uint8) ((Op04Cos>>8)&0xFF);
                                                break;

                                        case 0x08:
                                                Op08X = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op08Y = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op08Z = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp08 ();

                                                DSP1.out_count = 4;
                                                DSP1.output [0] = (uint8) (((int16) Op08Ll)&0xFF);
                                                DSP1.output [1] = (uint8) ((((int16) Op08Ll)>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (((int16) Op08Lh)&0xFF);
                                                DSP1.output [3] = (uint8) ((((int16) Op08Lh)>>8)&0xFF);
                                                break;

                                        case 0x18:

                                                Op18X = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op18Y = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op18Z = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));
                                                Op18R = (int16) (DSP1.parameters [6]|(DSP1.parameters[7]<<8));

                                                DSPOp18 ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = (uint8) (Op18D&0xFF);
                                                DSP1.output [1] = (uint8) ((Op18D>>8)&0xFF);
                                                break;

                                        case 0x38:

                                                Op38X = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op38Y = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op38Z = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));
                                                Op38R = (int16) (DSP1.parameters [6]|(DSP1.parameters[7]<<8));

                                                DSPOp38 ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = (uint8) (Op38D&0xFF);
                                                DSP1.output [1] = (uint8) ((Op38D>>8)&0xFF);
                                                break;

                                        case 0x28:
                                                Op28X = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op28Y = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op28Z = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp28 ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = (uint8) (Op28R&0xFF);
                                                DSP1.output [1] = (uint8) ((Op28R>>8)&0xFF);
                                                break;

                                        case 0x2c:
                                        case 0x0c:
                                                Op0CA = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op0CX1 = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op0CY1 = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp0C ();

                                                DSP1.out_count = 4;
                                                DSP1.output [0] = (uint8) (Op0CX2&0xFF);
                                                DSP1.output [1] = (uint8) ((Op0CX2>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op0CY2&0xFF);
                                                DSP1.output [3] = (uint8) ((Op0CY2>>8)&0xFF);
                                                break;

                                        case 0x3c:
                                        case 0x1c:
                                                Op1CZ = (DSP1.parameters [0]|(DSP1.parameters[1]<<8));

                                                Op1CY = (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op1CX = (DSP1.parameters [4]|(DSP1.parameters[5]<<8));
                                                Op1CXBR = (DSP1.parameters [6]|(DSP1.parameters[7]<<8));
                                                Op1CYBR = (DSP1.parameters [8]|(DSP1.parameters[9]<<8));
                                                Op1CZBR = (DSP1.parameters [10]|(DSP1.parameters[11]<<8));

                                                DSPOp1C ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op1CXAR&0xFF);
                                                DSP1.output [1] = (uint8) ((Op1CXAR>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op1CYAR&0xFF);
                                                DSP1.output [3] = (uint8) ((Op1CYAR>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op1CZAR&0xFF);
                                                DSP1.output [5] = (uint8) ((Op1CZAR>>8)&0xFF);
                                                break;

                                        case 0x32:
                                        case 0x22:
                                        case 0x12:
                                        case 0x02:
                                                Op02FX = (short)(DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op02FY = (short)(DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op02FZ = (short)(DSP1.parameters [4]|(DSP1.parameters[5]<<8));
                                                Op02LFE = (short)(DSP1.parameters [6]|(DSP1.parameters[7]<<8));
                                                Op02LES = (short)(DSP1.parameters [8]|(DSP1.parameters[9]<<8));
                                                Op02AAS = (unsigned short)(DSP1.parameters [10]|(DSP1.parameters[11]<<8));
                                                Op02AZS = (unsigned short)(DSP1.parameters [12]|(DSP1.parameters[13]<<8));

                                                DSPOp02 ();

                                                DSP1.out_count = 8;
                                                DSP1.output [0] = (uint8) (Op02VOF&0xFF);
                                                DSP1.output [1] = (uint8) ((Op02VOF>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op02VVA&0xFF);
                                                DSP1.output [3] = (uint8) ((Op02VVA>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op02CX&0xFF);
                                                DSP1.output [5] = (uint8) ((Op02CX>>8)&0xFF);
                                                DSP1.output [6] = (uint8) (Op02CY&0xFF);
                                                DSP1.output [7] = (uint8) ((Op02CY>>8)&0xFF);
                                                break;

                                        case 0x3a:
                                        case 0x2a:
                                        case 0x1a:
                                        case 0x0a:
                                                Op0AVS = (short)(DSP1.parameters [0]|(DSP1.parameters[1]<<8));

                                                DSPOp0A ();

                                                DSP1.out_count = 8;
                                                DSP1.output [0] = (uint8) (Op0AA&0xFF);
                                                DSP1.output [2] = (uint8) (Op0AB&0xFF);
                                                DSP1.output [4] = (uint8) (Op0AC&0xFF);
                                                DSP1.output [6] = (uint8) (Op0AD&0xFF);
                                                DSP1.output [1] = (uint8) ((Op0AA>>8)&0xFF);
                                                DSP1.output [3] = (uint8) ((Op0AB>>8)&0xFF);
                                                DSP1.output [5] = (uint8) ((Op0AC>>8)&0xFF);
                                                DSP1.output [7] = (uint8) ((Op0AD>>8)&0xFF);
                                                DSP1.in_index=0;
                                                break;

                                        case 0x16:
                                        case 0x26:
                                        case 0x36:
                                        case 0x06:
                                                Op06X = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op06Y = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op06Z = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp06 ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op06H&0xff);
                                                DSP1.output [1] = (uint8) ((Op06H>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op06V&0xFF);
                                                DSP1.output [3] = (uint8) ((Op06V>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op06S&0xFF);
                                                DSP1.output [5] = (uint8) ((Op06S>>8)&0xFF);
                                                break;

                                        case 0x1e:
                                        case 0x2e:
                                        case 0x3e:
                                        case 0x0e:
                                                Op0EH = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op0EV = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));

                                                DSPOp0E ();

                                                DSP1.out_count = 4;
                                                DSP1.output [0] = (uint8) (Op0EX&0xFF);
                                                DSP1.output [1] = (uint8) ((Op0EX>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op0EY&0xFF);
                                                DSP1.output [3] = (uint8) ((Op0EY>>8)&0xFF);
                                                break;


                                        case 0x05:
                                        case 0x35:
                                        case 0x31:
                                        case 0x01:
                                                Op01m = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op01Zr = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op01Yr = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));
                                                Op01Xr = (int16) (DSP1.parameters [6]|(DSP1.parameters[7]<<8));

                                                DSPOp01 ();
                                                break;

                                        case 0x15:
                                        case 0x11:
                                                Op11m = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op11Zr = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op11Yr = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));
                                                Op11Xr = (int16) (DSP1.parameters [7]|(DSP1.parameters[7]<<8));

                                                DSPOp11 ();
                                                break;

                                        case 0x25:
                                        case 0x21:
                                                Op21m = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op21Zr = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op21Yr = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));
                                                Op21Xr = (int16) (DSP1.parameters [6]|(DSP1.parameters[7]<<8));

                                                DSPOp21 ();
                                                break;

                                        case 0x09:
                                        case 0x39:
                                        case 0x3d:
                                        case 0x0d:
                                                Op0DX = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op0DY = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op0DZ = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp0D ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op0DF&0xFF);
                                                DSP1.output [1] = (uint8) ((Op0DF>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op0DL&0xFF);
                                                DSP1.output [3] = (uint8) ((Op0DL>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op0DU&0xFF);
                                                DSP1.output [5] = (uint8) ((Op0DU>>8)&0xFF);
                                                break;

                                        case 0x19:
                                        case 0x1d:
                                                Op1DX = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op1DY = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op1DZ = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp1D ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op1DF&0xFF);
                                                DSP1.output [1] = (uint8) ((Op1DF>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op1DL&0xFF);
                                                DSP1.output [3] = (uint8) ((Op1DL>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op1DU&0xFF);
                                                DSP1.output [5] = (uint8) ((Op1DU>>8)&0xFF);
                                                break;

                                        case 0x29:
                                        case 0x2d:
                                                Op2DX = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op2DY = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op2DZ = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp2D ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op2DF&0xFF);
                                                DSP1.output [1] = (uint8) ((Op2DF>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op2DL&0xFF);
                                                DSP1.output [3] = (uint8) ((Op2DL>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op2DU&0xFF);
                                                DSP1.output [5] = (uint8) ((Op2DU>>8)&0xFF);
                                                break;

                                        case 0x33:
                                        case 0x03:
                                                Op03F = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op03L = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op03U = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp03 ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op03X&0xFF);
                                                DSP1.output [1] = (uint8) ((Op03X>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op03Y&0xFF);
                                                DSP1.output [3] = (uint8) ((Op03Y>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op03Z&0xFF);
                                                DSP1.output [5] = (uint8) ((Op03Z>>8)&0xFF);
                                                break;

                                        case 0x13:
                                                Op13F = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op13L = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op13U = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp13 ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op13X&0xFF);
                                                DSP1.output [1] = (uint8) ((Op13X>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op13Y&0xFF);
                                                DSP1.output [3] = (uint8) ((Op13Y>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op13Z&0xFF);
                                                DSP1.output [5] = (uint8) ((Op13Z>>8)&0xFF);
                                                break;

                                        case 0x23:
                                                Op23F = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op23L = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op23U = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp23 ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op23X&0xFF);
                                                DSP1.output [1] = (uint8) ((Op23X>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op23Y&0xFF);
                                                DSP1.output [3] = (uint8) ((Op23Y>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op23Z&0xFF);
                                                DSP1.output [5] = (uint8) ((Op23Z>>8)&0xFF);
                                                break;

                                        case 0x3b:
                                        case 0x0b:
                                                Op0BX = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op0BY = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op0BZ = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp0B ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = (uint8) (Op0BS&0xFF);
                                                DSP1.output [1] = (uint8) ((Op0BS>>8)&0xFF);
                                                break;

                                        case 0x1b:
                                                Op1BX = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op1BY = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op1BZ = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp1B ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = (uint8) (Op1BS&0xFF);
                                                DSP1.output [1] = (uint8) ((Op1BS>>8)&0xFF);
                                                break;

                                        case 0x2b:
                                                Op2BX = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op2BY = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op2BZ = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));

                                                DSPOp0B ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = (uint8) (Op2BS&0xFF);
                                                DSP1.output [1] = (uint8) ((Op2BS>>8)&0xFF);
                                                break;

                                        case 0x34:
                                        case 0x14:
                                                Op14Zr = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));
                                                Op14Xr = (int16) (DSP1.parameters [2]|(DSP1.parameters[3]<<8));
                                                Op14Yr = (int16) (DSP1.parameters [4]|(DSP1.parameters[5]<<8));
                                                Op14U = (int16) (DSP1.parameters [6]|(DSP1.parameters[7]<<8));
                                                Op14F = (int16) (DSP1.parameters [8]|(DSP1.parameters[9]<<8));
                                                Op14L = (int16) (DSP1.parameters [10]|(DSP1.parameters[11]<<8));

                                                DSPOp14 ();

                                                DSP1.out_count = 6;
                                                DSP1.output [0] = (uint8) (Op14Zrr&0xFF);
                                                DSP1.output [1] = (uint8) ((Op14Zrr>>8)&0xFF);
                                                DSP1.output [2] = (uint8) (Op14Xrr&0xFF);
                                                DSP1.output [3] = (uint8) ((Op14Xrr>>8)&0xFF);
                                                DSP1.output [4] = (uint8) (Op14Yrr&0xFF);
                                                DSP1.output [5] = (uint8) ((Op14Yrr>>8)&0xFF);
                                                break;


                                        case 0x27:
                                        case 0x2F:
                                                Op2FUnknown = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));

                                                DSPOp2F ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = (uint8)(Op2FSize&0xFF);
                                                DSP1.output [1] = (uint8)((Op2FSize>>8)&0xFF);
                                                break;

                                        case 0x07:
                                        case 0x0F:
                                                Op0FRamsize = (int16) (DSP1.parameters [0]|(DSP1.parameters[1]<<8));

                                                DSPOp0F ();

                                                DSP1.out_count = 2;
                                                DSP1.output [0] = (uint8)(Op0FPass&0xFF);
                                                DSP1.output [1] = (uint8)((Op0FPass>>8)&0xFF);
                                                break;

                                        default:
                                                break;
                                        }
                                }
                        }
                }
    }
}

uint8 DSP1GetByte(uint16 address)
{
        uint8 t;
    if ((address & 0xf000) == 0x6000 ||

                (address&0x7fff) < 0x4000)
    {
                if (DSP1.out_count)
                {

                                t = (uint8) DSP1.output [DSP1.out_index];



                                DSP1.out_index++;
                                if (--DSP1.out_count == 0)
                                {
                                        if (DSP1.command == 0x1a || DSP1.command == 0x0a)
                                        {
                                                DSPOp0A ();
                                                DSP1.out_count = 8;
                                                DSP1.out_index = 0;
                                                DSP1.output [0] = (Op0AA&0xFF);
                                                DSP1.output [1] = (Op0AA>>8)&0xFF;
                                                DSP1.output [2] = (Op0AB&0xFF);
                                                DSP1.output [3] = (Op0AB>>8)&0xFF;
                                                DSP1.output [4] = (Op0AC&0xFF);
                                                DSP1.output [5] = (Op0AC>>8)&0xFF;
                                                DSP1.output [6] = (Op0AD&0xFF);
                                                DSP1.output [7] = (Op0AD>>8)&0xFF;
                                        }
                                        if(DSP1.command==0x1f)
                                        {
                                                if((DSP1.out_index%2)!=0)
                                                {
                                                        t=(uint8)DSP1ROM[DSP1.out_index>>1];
                                                }
                                                else
                                                {
                                                        t=DSP1ROM[DSP1.out_index>>1]>>8;
                                                }
                                        }
                                }
                                DSP1.waiting4command = 1;

                }
                else
                {


                                t = 0xff;


                }
    }
    else t = 0x80;
        return t;
}

void DSP2SetByte(uint8 byte, uint16 address)
;

uint8 DSP2GetByte(uint16 address)
;
