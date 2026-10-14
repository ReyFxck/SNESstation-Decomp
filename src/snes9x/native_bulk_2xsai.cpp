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

/* Native bulk proof: analysis/functions/native_bulk_2xsai_exact_6888.tsv
 * 10 routines / 6888 complete linked historical instruction bytes.
 * Frozen audit entry points:
 * 0x0010a7fc _Z9GetResultjjjj (68 bytes)
 * 0x0010a768 _Z10GetResult1jjjjj (68 bytes)
 * 0x0010a7ac _Z10GetResult2jjjjj (80 bytes)
 * 0x0010a18c _Z8Bilinearjjj (216 bytes)
 * 0x0010a264 _Z9Bilinear4jjjjjj (372 bytes)
 */
/* Pinned Snes9x native 2xsai recovery; original declarations/macros and bodies expanded with the historical EE profile. */
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
extern "C"
{
}

bool8 cpu_mmx = 1;

static uint32 colorMask = 0xF7DEF7DE;
static uint32 lowPixelMask = 0x08210821;
static uint32 qcolorMask = 0xE79CE79C;
static uint32 qlowpixelMask = 0x18631863;
static uint32 redblueMask = 0xF81F;
static uint32 greenMask = 0x7E0;

int Init_2xSaI (uint32 BitFormat)
{
    if (BitFormat == 565)
    {
        colorMask = 0xF7DEF7DE;
        lowPixelMask = 0x08210821;
        qcolorMask = 0xE79CE79C;
        qlowpixelMask = 0x18631863;
        redblueMask = 0xF81F;
        greenMask = 0x7E0;
    }
    else if (BitFormat == 555)
    {
        colorMask = 0x7BDE7BDE;
        lowPixelMask = 0x04210421;
        qcolorMask = 0x739C739C;
        qlowpixelMask = 0x0C630C63;
        redblueMask = 0x7C1F;
        greenMask = 0x3E0;
    }
    else
    {
        return 0;
    }





    return 1;
}

static inline int GetResult1 (uint32 A, uint32 B, uint32 C, uint32 D,
                              uint32 )
{
    int x = 0;
    int y = 0;
    int r = 0;

    if (A == C)
        x += 1;
    else if (B == C)
        y += 1;
    if (A == D)
        x += 1;
    else if (B == D)
        y += 1;
    if (x <= 1)
        r += 1;
    if (y <= 1)
        r -= 1;
    return r;
}

static inline int GetResult2 (uint32 A, uint32 B, uint32 C, uint32 D,
                              uint32 )
{
    int x = 0;
    int y = 0;
    int r = 0;

    if (A == C)
        x += 1;
    else if (B == C)
        y += 1;
    if (A == D)
        x += 1;
    else if (B == D)
        y += 1;
    if (x <= 1)
        r -= 1;
    if (y <= 1)
        r += 1;
    return r;
}

static inline int GetResult (uint32 A, uint32 B, uint32 C, uint32 D)
{
    int x = 0;
    int y = 0;
    int r = 0;

    if (A == C)
        x += 1;
    else if (B == C)
        y += 1;
    if (A == D)
        x += 1;
    else if (B == D)
        y += 1;
    if (x <= 1)
        r += 1;
    if (y <= 1)
        r -= 1;
    return r;
}

static inline uint32 INTERPOLATE (uint32 A, uint32 B)
{
    if (A != B)
    {
        return (((A & colorMask) >> 1) + ((B & colorMask) >> 1) +
                (A & B & lowPixelMask));
    }
    else
        return A;
}

static inline uint32 Q_INTERPOLATE (uint32 A, uint32 B, uint32 C, uint32 D)
{
    register uint32 x = ((A & qcolorMask) >> 2) +
        ((B & qcolorMask) >> 2) +
        ((C & qcolorMask) >> 2) + ((D & qcolorMask) >> 2);
    register uint32 y = (A & qlowpixelMask) +
        (B & qlowpixelMask) + (C & qlowpixelMask) + (D & qlowpixelMask);

    y = (y >> 2) & qlowpixelMask;
    return x + y;
}
void Super2xSaI (uint8 *srcPtr, uint32 srcPitch,
                 uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch,
                 int width, int height)
{
    uint16 *bP;
    uint8 *dP;
    uint32 inc_bP;
    {
        uint32 Nextline = srcPitch >> 1;
        inc_bP = 1;

        while (height--)
        {
            bP = (uint16 *) srcPtr;
            dP = (uint8 *) dstPtr;

            for (uint32 finish = width; finish; finish -= inc_bP)
            {
                uint32 color4, color5, color6;
                uint32 color1, color2, color3;
                uint32 colorA0, colorA1, colorA2, colorA3,
                    colorB0, colorB1, colorB2, colorB3, colorS1, colorS2;
                uint32 product1a, product1b, product2a, product2b;






                colorB0 = *(bP - Nextline - 1);
                colorB1 = *(bP - Nextline);
                colorB2 = *(bP - Nextline + 1);
                colorB3 = *(bP - Nextline + 2);

                color4 = *(bP - 1);
                color5 = *(bP);
                color6 = *(bP + 1);
                colorS2 = *(bP + 2);

                color1 = *(bP + Nextline - 1);
                color2 = *(bP + Nextline);
                color3 = *(bP + Nextline + 1);
                colorS1 = *(bP + Nextline + 2);

                colorA0 = *(bP + Nextline + Nextline - 1);
                colorA1 = *(bP + Nextline + Nextline);
                colorA2 = *(bP + Nextline + Nextline + 1);
                colorA3 = *(bP + Nextline + Nextline + 2);


                if (color2 == color6 && color5 != color3)
                {
                    product2b = product1b = color2;
                }
                else if (color5 == color3 && color2 != color6)
                {
                    product2b = product1b = color5;
                }
                else if (color5 == color3 && color2 == color6)
                {
                    register int r = 0;

                    r += GetResult (color6, color5, color1, colorA1);
                    r += GetResult (color6, color5, color4, colorB1);
                    r += GetResult (color6, color5, colorA2, colorS1);
                    r += GetResult (color6, color5, colorB2, colorS2);

                    if (r > 0)
                        product2b = product1b = color6;
                    else if (r < 0)
                        product2b = product1b = color5;
                    else
                    {
                        product2b = product1b = INTERPOLATE (color5, color6);
                    }
                }
                else
                {
                    if (color6 == color3 && color3 == colorA1
                            && color2 != colorA2 && color3 != colorA0)
                        product2b =
                            Q_INTERPOLATE (color3, color3, color3, color2);
                    else if (color5 == color2 && color2 == colorA2
                             && colorA1 != color3 && color2 != colorA3)
                        product2b =
                            Q_INTERPOLATE (color2, color2, color2, color3);
                    else
                        product2b = INTERPOLATE (color2, color3);

                    if (color6 == color3 && color6 == colorB1
                            && color5 != colorB2 && color6 != colorB0)
                        product1b =
                            Q_INTERPOLATE (color6, color6, color6, color5);
                    else if (color5 == color2 && color5 == colorB2
                             && colorB1 != color6 && color5 != colorB3)
                        product1b =
                            Q_INTERPOLATE (color6, color5, color5, color5);
                    else
                        product1b = INTERPOLATE (color5, color6);
                }

                if (color5 == color3 && color2 != color6 && color4 == color5
                        && color5 != colorA2)
                    product2a = INTERPOLATE (color2, color5);
                else
                    if (color5 == color1 && color6 == color5
                        && color4 != color2 && color5 != colorA0)
                    product2a = INTERPOLATE (color2, color5);
                else
                    product2a = color2;

                if (color2 == color6 && color5 != color3 && color1 == color2
                        && color2 != colorB2)
                    product1a = INTERPOLATE (color2, color5);
                else
                    if (color4 == color2 && color3 == color2
                        && color1 != color5 && color2 != colorB0)
                    product1a = INTERPOLATE (color2, color5);
                else
                    product1a = color5;

                product1a = product1a | (product1b << 16);
                product2a = product2a | (product2b << 16);

                *((uint32 *) dP) = product1a;
                *((uint32 *) (dP + dstPitch)) = product2a;

                bP += inc_bP;
                dP += sizeof (uint32);
            }

            srcPtr += srcPitch;
            dstPtr += dstPitch * 2;
            deltaPtr += srcPitch;
        }
    }
}

void SuperEagle (uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr,
                 uint8 *dstPtr, uint32 dstPitch, int width, int height)
{
    uint8 *dP;
    uint16 *bP;
    uint16 *xP;
    uint32 inc_bP;
    {
        inc_bP = 1;

        uint32 Nextline = srcPitch >> 1;

        while (height--)
        {
            bP = (uint16 *) srcPtr;
            xP = (uint16 *) deltaPtr;
            dP = dstPtr;
            for (uint32 finish = width; finish; finish -= inc_bP)
            {
                uint32 color4, color5, color6;
                uint32 color1, color2, color3;
                uint32 colorA1, colorA2, colorB1, colorB2, colorS1, colorS2;
                uint32 product1a, product1b, product2a, product2b;

                colorB1 = *(bP - Nextline);
                colorB2 = *(bP - Nextline + 1);

                color4 = *(bP - 1);
                color5 = *(bP);
                color6 = *(bP + 1);
                colorS2 = *(bP + 2);

                color1 = *(bP + Nextline - 1);
                color2 = *(bP + Nextline);
                color3 = *(bP + Nextline + 1);
                colorS1 = *(bP + Nextline + 2);

                colorA1 = *(bP + Nextline + Nextline);
                colorA2 = *(bP + Nextline + Nextline + 1);


                if (color2 == color6 && color5 != color3)
                {
                    product1b = product2a = color2;
                    if ((color1 == color2) || (color6 == colorB2))
                    {
                        product1a = INTERPOLATE (color2, color5);
                        product1a = INTERPOLATE (color2, product1a);

                    }
                    else
                    {
                        product1a = INTERPOLATE (color5, color6);
                    }

                    if ((color6 == colorS2) || (color2 == colorA1))
                    {
                        product2b = INTERPOLATE (color2, color3);
                        product2b = INTERPOLATE (color2, product2b);

                    }
                    else
                    {
                        product2b = INTERPOLATE (color2, color3);
                    }
                }
                else if (color5 == color3 && color2 != color6)
                {
                    product2b = product1a = color5;

                    if ((colorB1 == color5) || (color3 == colorS1))
                    {
                        product1b = INTERPOLATE (color5, color6);
                        product1b = INTERPOLATE (color5, product1b);

                    }
                    else
                    {
                        product1b = INTERPOLATE (color5, color6);
                    }

                    if ((color3 == colorA2) || (color4 == color5))
                    {
                        product2a = INTERPOLATE (color5, color2);
                        product2a = INTERPOLATE (color5, product2a);

                    }
                    else
                    {
                        product2a = INTERPOLATE (color2, color3);
                    }

                }
                else if (color5 == color3 && color2 == color6)
                {
                    register int r = 0;

                    r += GetResult (color6, color5, color1, colorA1);
                    r += GetResult (color6, color5, color4, colorB1);
                    r += GetResult (color6, color5, colorA2, colorS1);
                    r += GetResult (color6, color5, colorB2, colorS2);

                    if (r > 0)
                    {
                        product1b = product2a = color2;
                        product1a = product2b = INTERPOLATE (color5, color6);
                    }
                    else if (r < 0)
                    {
                        product2b = product1a = color5;
                        product1b = product2a = INTERPOLATE (color5, color6);
                    }
                    else
                    {
                        product2b = product1a = color5;
                        product1b = product2a = color2;
                    }
                }
                else
                {
                    product2b = product1a = INTERPOLATE (color2, color6);
                    product2b =
                        Q_INTERPOLATE (color3, color3, color3, product2b);
                    product1a =
                        Q_INTERPOLATE (color5, color5, color5, product1a);

                    product2a = product1b = INTERPOLATE (color5, color3);
                    product2a =
                        Q_INTERPOLATE (color2, color2, color2, product2a);
                    product1b =
                        Q_INTERPOLATE (color6, color6, color6, product1b);





                }
                product1a = product1a | (product1b << 16);
                product2a = product2a | (product2b << 16);

                *((uint32 *) dP) = product1a;
                *((uint32 *) (dP + dstPitch)) = product2a;
                *xP = color5;

                bP += inc_bP;
                xP += inc_bP;
                dP += sizeof (uint32);
            }

            srcPtr += srcPitch;
            dstPtr += dstPitch * 2;
            deltaPtr += srcPitch;
        }
    }
}

void _2xSaI (uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr,
             uint8 *dstPtr, uint32 dstPitch, int width, int height)
{
    uint8 *dP;
    uint16 *bP;
    uint32 inc_bP;
    {
        inc_bP = 1;

        uint32 Nextline = srcPitch >> 1;

        while (height--)
        {
            bP = (uint16 *) srcPtr;
            dP = dstPtr;

            for (uint32 finish = width; finish; finish -= inc_bP)
            {

                register uint32 colorA, colorB;
                uint32 colorC, colorD,
                    colorE, colorF, colorG, colorH,
                    colorI, colorJ, colorK, colorL,

                    colorM, colorN, colorO, colorP;
                uint32 product, product1, product2;






                colorI = *(bP - Nextline - 1);
                colorE = *(bP - Nextline);
                colorF = *(bP - Nextline + 1);
                colorJ = *(bP - Nextline + 2);

                colorG = *(bP - 1);
                colorA = *(bP);
                colorB = *(bP + 1);
                colorK = *(bP + 2);

                colorH = *(bP + Nextline - 1);
                colorC = *(bP + Nextline);
                colorD = *(bP + Nextline + 1);
                colorL = *(bP + Nextline + 2);

                colorM = *(bP + Nextline + Nextline - 1);
                colorN = *(bP + Nextline + Nextline);
                colorO = *(bP + Nextline + Nextline + 1);
                colorP = *(bP + Nextline + Nextline + 2);

                if ((colorA == colorD) && (colorB != colorC))
                {
                    if (((colorA == colorE) && (colorB == colorL)) ||
                            ((colorA == colorC) && (colorA == colorF)
                             && (colorB != colorE) && (colorB == colorJ)))
                    {
                        product = colorA;
                    }
                    else
                    {
                        product = INTERPOLATE (colorA, colorB);
                    }

                    if (((colorA == colorG) && (colorC == colorO)) ||
                            ((colorA == colorB) && (colorA == colorH)
                             && (colorG != colorC) && (colorC == colorM)))
                    {
                        product1 = colorA;
                    }
                    else
                    {
                        product1 = INTERPOLATE (colorA, colorC);
                    }
                    product2 = colorA;
                }
                else if ((colorB == colorC) && (colorA != colorD))
                {
                    if (((colorB == colorF) && (colorA == colorH)) ||
                            ((colorB == colorE) && (colorB == colorD)
                             && (colorA != colorF) && (colorA == colorI)))
                    {
                        product = colorB;
                    }
                    else
                    {
                        product = INTERPOLATE (colorA, colorB);
                    }

                    if (((colorC == colorH) && (colorA == colorF)) ||
                            ((colorC == colorG) && (colorC == colorD)
                             && (colorA != colorH) && (colorA == colorI)))
                    {
                        product1 = colorC;
                    }
                    else
                    {
                        product1 = INTERPOLATE (colorA, colorC);
                    }
                    product2 = colorB;
                }
                else if ((colorA == colorD) && (colorB == colorC))
                {
                    if (colorA == colorB)
                    {
                        product = colorA;
                        product1 = colorA;
                        product2 = colorA;
                    }
                    else
                    {
                        register int r = 0;

                        product1 = INTERPOLATE (colorA, colorC);
                        product = INTERPOLATE (colorA, colorB);

                        r +=
                            GetResult1 (colorA, colorB, colorG, colorE,
                                        colorI);
                        r +=
                            GetResult2 (colorB, colorA, colorK, colorF,
                                        colorJ);
                        r +=
                            GetResult2 (colorB, colorA, colorH, colorN,
                                        colorM);
                        r +=
                            GetResult1 (colorA, colorB, colorL, colorO,
                                        colorP);

                        if (r > 0)
                            product2 = colorA;
                        else if (r < 0)
                            product2 = colorB;
                        else
                        {
                            product2 =
                                Q_INTERPOLATE (colorA, colorB, colorC,
                                               colorD);
                        }
                    }
                }
                else
                {
                    product2 = Q_INTERPOLATE (colorA, colorB, colorC, colorD);

                    if ((colorA == colorC) && (colorA == colorF)
                            && (colorB != colorE) && (colorB == colorJ))
                    {
                        product = colorA;
                    }
                    else
                        if ((colorB == colorE) && (colorB == colorD)
                            && (colorA != colorF) && (colorA == colorI))
                    {
                        product = colorB;
                    }
                    else
                    {
                        product = INTERPOLATE (colorA, colorB);
                    }

                    if ((colorA == colorB) && (colorA == colorH)
                            && (colorG != colorC) && (colorC == colorM))
                    {
                        product1 = colorA;
                    }
                    else
                        if ((colorC == colorG) && (colorC == colorD)
                            && (colorA != colorH) && (colorA == colorI))
                    {
                        product1 = colorC;
                    }
                    else
                    {
                        product1 = INTERPOLATE (colorA, colorC);
                    }
                }

                product = colorA | (product << 16);
                product1 = product1 | (product2 << 16);
                *((int32 *) dP) = product;
                *((uint32 *) (dP + dstPitch)) = product1;

                bP += inc_bP;
                dP += sizeof (uint32);
            }

            srcPtr += srcPitch;
            dstPtr += dstPitch * 2;
            deltaPtr += srcPitch;
        }
    }
}
static uint32 Bilinear (uint32 A, uint32 B, uint32 x)
{
    unsigned long areaA, areaB;
    unsigned long result;

    if (A == B)
        return A;

    areaB = (x >> 11) & 0x1f;
    areaA = 0x20 - areaB;

    A = (A & redblueMask) | ((A & greenMask) << 16);
    B = (B & redblueMask) | ((B & greenMask) << 16);

    result = ((areaA * A) + (areaB * B)) >> 5;

    return (result & redblueMask) | ((result >> 16) & greenMask);

}

static uint32 Bilinear4 (uint32 A, uint32 B, uint32 C, uint32 D, uint32 x,
                         uint32 y)
{
    unsigned long areaA, areaB, areaC, areaD;
    unsigned long result, xy;

    x = (x >> 11) & 0x1f;
    y = (y >> 11) & 0x1f;
    xy = (x * y) >> 5;

    A = (A & redblueMask) | ((A & greenMask) << 16);
    B = (B & redblueMask) | ((B & greenMask) << 16);
    C = (C & redblueMask) | ((C & greenMask) << 16);
    D = (D & redblueMask) | ((D & greenMask) << 16);

    areaA = 0x20 + xy - x - y;
    areaB = x - xy;
    areaC = y - xy;
    areaD = xy;

    result = ((areaA * A) + (areaB * B) + (areaC * C) + (areaD * D)) >> 5;

    return (result & redblueMask) | ((result >> 16) & greenMask);
}

void Scale_2xSaI (uint8 *srcPtr, uint32 srcPitch, uint8 * ,
                  uint8 *dstPtr, uint32 dstPitch,
                  uint32 dstWidth, uint32 dstHeight, int width, int height)
{
    uint8 *dP;
    uint16 *bP;

    uint32 w;
    uint32 h;
    uint32 dw;
    uint32 dh;
    uint32 hfinish;
    uint32 wfinish;

    uint32 Nextline = srcPitch >> 1;

    wfinish = (width - 1) << 16;
    dw = wfinish / (dstWidth - 1);
    hfinish = (height - 1) << 16;
    dh = hfinish / (dstHeight - 1);

    for (h = 0; h < hfinish; h += dh)
    {
        uint32 y1, y2;

        y1 = h & 0xffff;
        bP = (uint16 *) (srcPtr + ((h >> 16) * srcPitch));
        dP = dstPtr;
        y2 = 0x10000 - y1;

        w = 0;

        for (; w < wfinish;)
        {
            uint32 A, B, C, D;
            uint32 E, F, G, H;
            uint32 I, J, K, L;
            uint32 x1, x2, a1, f1, f2;
            uint32 position, product1;

            position = w >> 16;
            A = bP[position];
            B = bP[position + 1];
            C = bP[position + Nextline];
            D = bP[position + Nextline + 1];
            E = bP[position - Nextline];
            F = bP[position - Nextline + 1];
            G = bP[position - 1];
            H = bP[position + Nextline - 1];
            I = bP[position + 2];
            J = bP[position + Nextline + 2];
            K = bP[position + Nextline + Nextline];
            L = bP[position + Nextline + Nextline + 1];

            x1 = w & 0xffff;
            x2 = 0x10000 - x1;


            if (A == B && C == D && A == C)
                product1 = A;
            else

            if (A == D && B != C)
            {
                f1 = (x1 >> 1) + (0x10000 >> 2);
                f2 = (y1 >> 1) + (0x10000 >> 2);
                if (y1 <= f1 && A == J && A != E)
                {
                    a1 = f1 - y1;
                    product1 = Bilinear (A, B, a1);
                }
                else if (y1 >= f1 && A == G && A != L)
                {
                    a1 = y1 - f1;
                    product1 = Bilinear (A, C, a1);
                }
                else if (x1 >= f2 && A == E && A != J)
                {
                    a1 = x1 - f2;
                    product1 = Bilinear (A, B, a1);
                }
                else if (x1 <= f2 && A == L && A != G)
                {
                    a1 = f2 - x1;
                    product1 = Bilinear (A, C, a1);
                }
                else if (y1 >= x1)
                {
                    a1 = y1 - x1;
                    product1 = Bilinear (A, C, a1);
                }
                else if (y1 <= x1)
                {
                    a1 = x1 - y1;
                    product1 = Bilinear (A, B, a1);
                }
            }
            else

            if (B == C && A != D)
            {
                f1 = (x1 >> 1) + (0x10000 >> 2);
                f2 = (y1 >> 1) + (0x10000 >> 2);
                if (y2 >= f1 && B == H && B != F)
                {
                    a1 = y2 - f1;
                    product1 = Bilinear (B, A, a1);
                }
                else if (y2 <= f1 && B == I && B != K)
                {
                    a1 = f1 - y2;
                    product1 = Bilinear (B, D, a1);
                }
                else if (x2 >= f2 && B == F && B != H)
                {
                    a1 = x2 - f2;
                    product1 = Bilinear (B, A, a1);
                }
                else if (x2 <= f2 && B == K && B != I)
                {
                    a1 = f2 - x2;
                    product1 = Bilinear (B, D, a1);
                }
                else if (y2 >= x1)
                {
                    a1 = y2 - x1;
                    product1 = Bilinear (B, A, a1);
                }
                else if (y2 <= x1)
                {
                    a1 = x1 - y2;
                    product1 = Bilinear (B, D, a1);
                }
            }

            else
            {
                product1 = Bilinear4 (A, B, C, D, x1, y1);
            }


            *(uint32 *) dP = product1;
            dP += 2;
            w += dw;
        }
        dstPtr += dstPitch;
    }
}
