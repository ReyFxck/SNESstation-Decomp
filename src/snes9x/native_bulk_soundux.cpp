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

/* Native bulk proof: analysis/functions/native_bulk_soundux_exact_4076.tsv
 * 22 routines / 4076 complete linked historical instruction bytes.
 * Frozen audit entry points:
 * 0x00173dfc _Z20S9xAPUSetEndOfSampleiP7Channel (80 bytes)
 * 0x00173e4c _Z13S9xAPUSetEndXi (32 bytes)
 * 0x00173e6c _Z13S9xSetEnvRateP7Channelmii (184 bytes)
 * 0x00173f24 _Z18S9xSetEnvelopeRateimii (44 bytes)
 * 0x00173f50 _Z17S9xSetSoundVolumeiss (160 bytes)
 * 0x00173ff0 _Z18S9xSetMasterVolumess (172 bytes)
 * 0x0017409c _Z16S9xSetEchoVolumess (132 bytes)
 * 0x00174204 _Z18S9xSetEchoFeedbacki (40 bytes)
 * 0x001742e8 _Z31S9xSetFrequencyModulationEnableh (16 bytes)
 * 0x001742f8 _Z17S9xSetSoundKeyOffi (88 bytes)
 * 0x00174350 _Z28S9xFixSoundAfterSnapshotLoadv (384 bytes)
 * 0x001744d0 _Z23S9xSetFilterCoefficientii (148 bytes)
 * 0x00174564 _Z15S9xSetSoundADSRiiiiii (180 bytes)
 * 0x00174618 _Z20S9xSetEnvelopeHeightii (140 bytes)
 * 0x001746a4 _Z20S9xGetEnvelopeHeighti (124 bytes)
 * 0x00174720 _Z17S9xSetSoundSampleit (8 bytes)
 * 0x00174830 _Z16S9xSetSoundHertzii (48 bytes)
 * 0x00174860 _Z15S9xSetSoundTypeii (28 bytes)
 * 0x0017487c _Z15S9xSetSoundMuteh (28 bytes)
 * 0x00174898 _Z14AltDecodeBlockP7Channel (764 bytes)
 * 0x00174b94 _Z15AltDecodeBlock2P7Channel (1104 bytes)
 * 0x00177db0 _Z15S9xSetSoundModeii (172 bytes)
 */
/* Pinned Snes9x native soundux recovery; original declarations/macros and bodies expanded with the historical EE profile. */
extern "C" {
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long unsigned int size_t;
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
extern "C" {







extern int *__errno (void);




extern const char * const _sys_errlist[];
extern int _sys_nerr;
}







extern "C" {
struct flock {
        short l_type;
        short l_whence;
        long l_start;
        long l_len;
        short l_pid;
        short l_xxx;
};



struct eflock {
        short l_type;
        short l_whence;
        long l_start;
        long l_len;
        short l_pid;
        short l_xxx;
        long l_rpid;
        long l_rsys;
};
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
extern "C" {
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
struct stat
{
  dev_t st_dev;
  ino_t st_ino;
  mode_t st_mode;
  nlink_t st_nlink;
  uid_t st_uid;
  gid_t st_gid;
  dev_t st_rdev;
  off_t st_size;






  time_t st_atime;
  long st_spare1;
  time_t st_mtime;
  long st_spare2;
  time_t st_ctime;
  long st_spare3;
  long st_blksize;
  long st_blocks;
  long st_spare4[2];

};
int chmod ( const char *__path, mode_t __mode );
int fchmod (int __fd, mode_t __mode);
int fstat ( int __fd, struct stat *__sbuf );
int mkdir ( const char *_path, mode_t __mode );
int mkfifo ( const char *__path, mode_t __mode );
int stat ( const char *__path, struct stat *__sbuf );
mode_t umask ( mode_t __mask );
int _fstat ( int __fd, struct stat *__sbuf );
int _stat ( const char *__path, struct stat *__sbuf );



}



extern int open (const char *, int, ...);
extern int creat (const char *, mode_t);
extern int fcntl (int, int, ...);



extern int _open (const char *, int, ...);
extern int _fcntl (int, int, ...);


}
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
extern struct SSNESGameFixes SNESGameFixes __asm__("DAT_0035b738");
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

extern "C" volatile SoundStatus so __asm__("DAT_003ab718");

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
void S9xSetEchoEnable (uint8 byte);
void S9xSetEchoDelay (int byte) __asm__("S9xSetEchoDelay");
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

extern "C" struct SAPURegisters APURegisters;
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



extern int Echo [24000] __asm__("DAT_003ab748");
extern int DummyEchoBuffer [(1024 * 16)] __asm__("DAT_003c2e48");
extern int MixBuffer [(1024 * 16)] __asm__("DAT_003d2e48");
extern int EchoBuffer [(1024 * 16)] __asm__("DAT_003e2e48");
extern int FilterTaps [8] __asm__("DAT_003f2e48");
extern unsigned long Z __asm__("DAT_003f2e68");
extern int Loop [16] __asm__("DAT_003f2e70");

extern long FilterValues[4][2];
extern int NoiseFreq [32] __asm__("DAT_003f3fc0");
extern "C" void DecodeBlockAsm (int8 *, int16 *, int32 *, int32 *);
extern "C" void DecodeBlockAsm2 (int8 *, int16 *, int32 *, int32 *);
static inline uint8 *S9xGetSampleAddress (int sample_number)
{
    uint32 addr = (((APU.DSP[0x5d] << 8) + (sample_number << 2)) & 0xffff);
    return (IAPU.RAM + addr);
}

void S9xAPUSetEndOfSample (int i, Channel *ch)
{
    ch->state = SOUND_SILENT;
    ch->mode = MODE_NONE;
    APU.DSP [0x7c] |= 1 << i;
    APU.DSP [0x4c] &= ~(1 << i);
    APU.DSP [0x5c] &= ~(1 << i);
    APU.KeyedChannels &= ~(1 << i);
}




void S9xAPUSetEndX (int ch)
{
    APU.DSP [0x7c] |= 1 << ch;
}




void S9xSetEnvRate (Channel *ch, unsigned long rate, int direction, int target)
{
    ch->envx_target = target;

    if (rate == ~0UL)
    {
                ch->direction = 0;
                rate = 0;
    }
    else
                ch->direction = direction;

    static int steps [] =
    {

                0, 64, 619, 619, 128, 1, 64, 55, 64, 619
    };

    if (rate == 0 || so.playback_rate == 0)
                ch->erate = 0;
    else
    {
                ch->erate = (unsigned long)
                        (((int64) 0x10000UL * 1000 * steps [ch->state]) /
                        (rate * so.playback_rate));
    }
}





void S9xSetEnvelopeRate (int channel, unsigned long rate, int direction,
                                                 int target)
{
    S9xSetEnvRate (&SoundData.channels [channel], rate, direction, target);
}





void S9xSetSoundVolume (int channel, short volume_left, short volume_right)
{
    Channel *ch = &SoundData.channels[channel];
    if (!so.stereo)
                volume_left = (((volume_right) < 0 ? -(volume_right) : (volume_right)) + ((volume_left) < 0 ? -(volume_left) : (volume_left))) / 2;

    ch->volume_left = volume_left;
    ch->volume_right = volume_right;
    ch-> left_vol_level = (ch->envx * volume_left) / 128;
    ch->right_vol_level = (ch->envx * volume_right) / 128;
}

void S9xSetMasterVolume (short volume_left, short volume_right)
{
    if (Settings.DisableMasterVolume)
    {
                SoundData.master_volume_left = 127;
                SoundData.master_volume_right = 127;
                SoundData.master_volume [0] = SoundData.master_volume [1] = 127;
    }
    else
    {
                if (!so.stereo)
                        volume_left = (((volume_right) < 0 ? -(volume_right) : (volume_right)) + ((volume_left) < 0 ? -(volume_left) : (volume_left))) / 2;
                SoundData.master_volume_left = volume_left;
                SoundData.master_volume_right = volume_right;
                SoundData.master_volume [Settings.ReverseStereo] = volume_left;
                SoundData.master_volume [1 ^ Settings.ReverseStereo] = volume_right;
    }
}

void S9xSetEchoVolume (short volume_left, short volume_right)
{
    if (!so.stereo)
                volume_left = (((volume_right) < 0 ? -(volume_right) : (volume_right)) + ((volume_left) < 0 ? -(volume_left) : (volume_left))) / 2;
    SoundData.echo_volume_left = volume_left;
    SoundData.echo_volume_right = volume_right;
    SoundData.echo_volume [Settings.ReverseStereo] = volume_left;
    SoundData.echo_volume [1 ^ Settings.ReverseStereo] = volume_right;
}

void S9xSetEchoEnable (uint8 byte)
;

void S9xSetEchoFeedback (int feedback)
{
    if ((feedback) < -128) (feedback) = -128; else if ((feedback) > 127) (feedback) = 127;
    SoundData.echo_feedback = feedback;
}

void S9xSetEchoDelay (int delay) __asm__("S9xSetEchoDelay");

void S9xSetEchoWriteEnable (uint8 byte)
;

void S9xSetFrequencyModulationEnable (uint8 byte)
{
    SoundData.pitch_mod = byte & ~1;
}

void S9xSetSoundKeyOff (int channel)
{
    Channel *ch = &SoundData.channels[channel];

    if (ch->state != SOUND_SILENT)
    {
                ch->state = SOUND_RELEASE;
                ch->mode = MODE_RELEASE;
                S9xSetEnvRate (ch, 8, -1, 0);
    }
}

void S9xFixSoundAfterSnapshotLoad ()
{
    SoundData.echo_write_enabled = !(APU.DSP [0x6c] & 0x20);
    SoundData.echo_channel_enable = APU.DSP [0x4d];
    S9xSetEchoDelay (APU.DSP [0x7d] & 0xf);
    S9xSetEchoFeedback ((signed char) APU.DSP [0x0d]);

    S9xSetFilterCoefficient (0, (signed char) APU.DSP [0x0f]);
    S9xSetFilterCoefficient (1, (signed char) APU.DSP [0x1f]);
    S9xSetFilterCoefficient (2, (signed char) APU.DSP [0x2f]);
    S9xSetFilterCoefficient (3, (signed char) APU.DSP [0x3f]);
    S9xSetFilterCoefficient (4, (signed char) APU.DSP [0x4f]);
    S9xSetFilterCoefficient (5, (signed char) APU.DSP [0x5f]);
    S9xSetFilterCoefficient (6, (signed char) APU.DSP [0x6f]);
    S9xSetFilterCoefficient (7, (signed char) APU.DSP [0x7f]);
    for (int i = 0; i < 8; i++)
    {
                SoundData.channels[i].needs_decode = 1;
                S9xSetSoundFrequency (i, SoundData.channels[i].hertz);
                SoundData.channels [i].envxx = SoundData.channels [i].envx << 24;
                SoundData.channels [i].next_sample = 0;
                SoundData.channels [i].interpolate = 0;
                SoundData.channels [i].previous [0] = (int32) SoundData.channels [i].previous16 [0];
                SoundData.channels [i].previous [1] = (int32) SoundData.channels [i].previous16 [1];
    }
    SoundData.master_volume [Settings.ReverseStereo] = SoundData.master_volume_left;
    SoundData.master_volume [1 ^ Settings.ReverseStereo] = SoundData.master_volume_right;
    SoundData.echo_volume [Settings.ReverseStereo] = SoundData.echo_volume_left;
    SoundData.echo_volume [1 ^ Settings.ReverseStereo] = SoundData.echo_volume_right;
    IAPU.Scanline = 0;
}

void S9xSetFilterCoefficient (int tap, int value)
{
    FilterTaps [tap & 7] = value;
    SoundData.no_filter = (FilterTaps [0] == 127 || FilterTaps [0] == 0) &&
                FilterTaps [1] == 0 &&
                FilterTaps [2] == 0 &&
                FilterTaps [3] == 0 &&
                FilterTaps [4] == 0 &&
                FilterTaps [5] == 0 &&
                FilterTaps [6] == 0 &&
                FilterTaps [7] == 0;
}

void S9xSetSoundADSR (int channel, int attack_rate, int decay_rate,
                                          int sustain_rate, int sustain_level, int release_rate)
{
    SoundData.channels[channel].attack_rate = attack_rate;
    SoundData.channels[channel].decay_rate = decay_rate;
    SoundData.channels[channel].sustain_rate = sustain_rate;
    SoundData.channels[channel].release_rate = release_rate;
    SoundData.channels[channel].sustain_level = sustain_level + 1;

    switch (SoundData.channels[channel].state)
    {
    case SOUND_ATTACK:
                S9xSetEnvelopeRate (channel, attack_rate, 1, 127);
                break;

    case SOUND_DECAY:
                S9xSetEnvelopeRate (channel, decay_rate, -1,
                        (127 * (sustain_level + 1)) >> 3);
                break;
    case SOUND_SUSTAIN:
                S9xSetEnvelopeRate (channel, sustain_rate, -1, 0);
                break;
    }
}

void S9xSetEnvelopeHeight (int channel, int level)
{
    Channel *ch = &SoundData.channels[channel];

    ch->envx = level;
    ch->envxx = level << 24;

    ch->left_vol_level = (level * ch->volume_left) / 128;
    ch->right_vol_level = (level * ch->volume_right) / 128;

    if (ch->envx == 0 && ch->state != SOUND_SILENT && ch->state != SOUND_GAIN)
    {
                S9xAPUSetEndOfSample (channel, ch);
    }
}

int S9xGetEnvelopeHeight (int channel)
{
    if ((Settings.SoundEnvelopeHeightReading ||
                SNESGameFixes.SoundEnvelopeHeightReading2) &&
        SoundData.channels[channel].state != SOUND_SILENT &&
        SoundData.channels[channel].state != SOUND_GAIN)
    {
        return (SoundData.channels[channel].envx);
    }


    if (SNESGameFixes.SoundEnvelopeHeightReading2 &&
        SoundData.channels[channel].state != SOUND_SILENT)
    {
        return (SoundData.channels[channel].envx);
    }

    return (0);
}


void S9xSetSoundSample (int, uint16)
{
}
void S9xSetSoundFrequency (int channel, int hertz) __asm__("S9xSetSoundFrequency");

void S9xSetSoundHertz (int channel, int hertz)
{
    SoundData.channels[channel].hertz = hertz;
    S9xSetSoundFrequency (channel, hertz);
}

void S9xSetSoundType (int channel, int type_of_sound)
{
    SoundData.channels[channel].type = type_of_sound;
}

bool8 S9xSetSoundMute (bool8 mute)
{
    bool8 old = so.mute_sound;
    so.mute_sound = mute;
    return (old);
}

void AltDecodeBlock (Channel *ch)
{
    if (ch->block_pointer >= 0x10000 - 9)
    {
                ch->last_block = 1;
                ch->loop = 0;
                ch->block = ch->decoded;
                memset ((void *) ch->decoded, 0, sizeof (int16) * 16);
                return;
    }
    signed char *compressed = (signed char *) &IAPU.RAM [ch->block_pointer];

    unsigned char filter = *compressed;
    if ((ch->last_block = filter & 1))
                ch->loop = (filter & 2) != 0;
    int32 out;
    unsigned char shift;
    signed char sample1, sample2;
    unsigned int i;

    compressed++;
    signed short *raw = ch->block = ch->decoded;

    int32 prev0 = ch->previous [0];
    int32 prev1 = ch->previous [1];
    shift = filter >> 4;

    switch ((filter >> 2) & 3)
    {
    case 0:
                for (i = 8; i != 0; i--)
                {
                        sample1 = *compressed++;
                        sample2 = sample1 << 4;
                        sample2 >>= 4;
                        sample1 >>= 4;
                        *raw++ = ((int32) sample1 << shift);
                        *raw++ = ((int32) sample2 << shift);
                }
                prev1 = *(raw - 2);
                prev0 = *(raw - 1);
                break;
    case 1:
                for (i = 8; i != 0; i--)
                {
                        sample1 = *compressed++;
                        sample2 = sample1 << 4;
                        sample2 >>= 4;
                        sample1 >>= 4;
                        prev0 = (int16) prev0;
                        *raw++ = prev1 = ((int32) sample1 << shift) + prev0 - (prev0 >> 4);
                        prev1 = (int16) prev1;
                        *raw++ = prev0 = ((int32) sample2 << shift) + prev1 - (prev1 >> 4);
                }
                break;
    case 2:
                for (i = 8; i != 0; i--)
                {
                        sample1 = *compressed++;
                        sample2 = sample1 << 4;
                        sample2 >>= 4;
                        sample1 >>= 4;

                        out = (sample1 << shift) - prev1 + (prev1 >> 4);
                        prev1 = (int16) prev0;
                        prev0 &= ~3;
                        *raw++ = prev0 = out + (prev0 << 1) - (prev0 >> 5) -
                                (prev0 >> 4);

                        out = (sample2 << shift) - prev1 + (prev1 >> 4);
                        prev1 = (int16) prev0;
                        prev0 &= ~3;
                        *raw++ = prev0 = out + (prev0 << 1) - (prev0 >> 5) -
                                (prev0 >> 4);
                }
                break;
    case 3:
                for (i = 8; i != 0; i--)
                {
                        sample1 = *compressed++;
                        sample2 = sample1 << 4;
                        sample2 >>= 4;
                        sample1 >>= 4;
                        out = (sample1 << shift);

                        out = out - prev1 + (prev1 >> 3) + (prev1 >> 4);
                        prev1 = (int16) prev0;
                        prev0 &= ~3;
                        *raw++ = prev0 = out + (prev0 << 1) - (prev0 >> 3) -
                                (prev0 >> 4) - (prev1 >> 6);

                        out = (sample2 << shift);
                        out = out - prev1 + (prev1 >> 3) + (prev1 >> 4);
                        prev1 = (int16) prev0;
                        prev0 &= ~3;
                        *raw++ = prev0 = out + (prev0 << 1) - (prev0 >> 3) -
                                (prev0 >> 4) - (prev1 >> 6);
                }
                break;
    }
    ch->previous [0] = prev0;
    ch->previous [1] = prev1;


    ch->block_pointer += 9;
}

void AltDecodeBlock2 (Channel *ch)
{
    int32 out;
    unsigned char filter;
    unsigned char shift;
    signed char sample1, sample2;
    unsigned char i;

    if (ch->block_pointer > 0x10000 - 9)
    {
                ch->last_block = 1;
                ch->loop = 0;
                ch->block = ch->decoded;
                memset ((void *) ch->decoded, 0, sizeof (int16) * 16);
                return;
    }

    signed char *compressed = (signed char *) &IAPU.RAM [ch->block_pointer];

    filter = *compressed;
    if ((ch->last_block = filter & 1))
                ch->loop = (filter & 2) != 0;

    compressed++;
    signed short *raw = ch->block = ch->decoded;

    shift = filter >> 4;
    int32 prev0 = ch->previous [0];
    int32 prev1 = ch->previous [1];

    if(shift > 12)
                shift -= 4;

    switch ((filter >> 2) & 3)
    {
    case 0:
                for (i = 8; i != 0; i--)
                {
                        sample1 = *compressed++;
                        sample2 = sample1 << 4;

                        sample2 >>= 4;

                        sample1 >>= 4;

                        out = (int32)(sample1 << shift);

                        prev1 = prev0;
                        prev0 = out;
                        if ((out) < -32768) (out) = -32768; else if ((out) > 32767) (out) = 32767;
                        *raw++ = (int16)out;

                        out = (int32)(sample2 << shift);

                        prev1 = prev0;
                        prev0 = out;
                        if ((out) < -32768) (out) = -32768; else if ((out) > 32767) (out) = 32767;
                        *raw++ = (int16)out;
                }
                break;
    case 1:
                for (i = 8; i != 0; i--)
                {
                        sample1 = *compressed++;
                        sample2 = sample1 << 4;

                        sample2 >>= 4;

                        sample1 >>= 4;
                        out = (int32)(sample1 << shift);
                        out += (int32)((double)prev0 * 15/16);

                        prev1 = prev0;
                        prev0 = out;
                        if ((out) < -32768) (out) = -32768; else if ((out) > 32767) (out) = 32767;
                        *raw++ = (int16)out;

                        out = (int32)(sample2 << shift);
                        out += (int32)((double)prev0 * 15/16);

                        prev1 = prev0;
                        prev0 = out;
                        if ((out) < -32768) (out) = -32768; else if ((out) > 32767) (out) = 32767;
                        *raw++ = (int16)out;
                }
                break;
    case 2:
                for (i = 8; i != 0; i--)
                {
                        sample1 = *compressed++;
                        sample2 = sample1 << 4;

                        sample2 >>= 4;

                        sample1 >>= 4;

                        out = ((sample1 << shift) * 256 + (prev0 & ~0x2) * 488 - prev1 * 240) >> 8;

                        prev1 = prev0;
                        prev0 = (int16)out;
                        *raw++ = (int16)out;

                        out = ((sample2 << shift) * 256 + (prev0 & ~0x2) * 488 - prev1 * 240) >> 8;

                        prev1 = prev0;
                        prev0 = (int16)out;
                        *raw++ = (int16)out;
                }
                break;

    case 3:
                for (i = 8; i != 0; i--)
                {
                        sample1 = *compressed++;
                        sample2 = sample1 << 4;

                        sample2 >>= 4;

                        sample1 >>= 4;
                        out = (int32)(sample1 << shift);
                        out += (int32)((double)prev0 * 115/64 - (double)prev1 * 13/16);

                        prev1 = prev0;
                        prev0 = out;

                        if ((out) < -32768) (out) = -32768; else if ((out) > 32767) (out) = 32767;
                        *raw++ = (int16)out;

                        out = (int32)(sample2 << shift);
                        out += (int32)((double)prev0 * 115/64 - (double)prev1 * 13/16);

                        prev1 = prev0;
                        prev0 = out;

                        if ((out) < -32768) (out) = -32768; else if ((out) > 32767) (out) = 32767;
                        *raw++ = (int16)out;
                }
                break;
    }
    ch->previous [0] = prev0;
    ch->previous [1] = prev1;
    ch->block_pointer += 9;
}

void DecodeBlock (Channel *ch)
;


void MixStereo (int sample_count)
;





void MixMono (int sample_count)
;
void S9xMixSamples (uint8 *buffer, int sample_count)
;




void S9xMixSamplesO (uint8 *buffer, int sample_count, int byte_offset)
;





void S9xResetSound (bool8 full)
;

void S9xSetPlaybackRate (uint32 playback_rate)
;

bool8 S9xInitSound (int mode, bool8 stereo, int buffer_size)
;

bool8 S9xSetSoundMode (int channel, int mode)
{
    Channel *ch = &SoundData.channels[channel];

    switch (mode)
    {
    case MODE_RELEASE:
                if (ch->mode != MODE_NONE)
                {
                        ch->mode = MODE_RELEASE;
                        return (1);
                }
                break;

    case MODE_DECREASE_LINEAR:
    case MODE_DECREASE_EXPONENTIAL:
    case MODE_GAIN:
                if (ch->mode != MODE_RELEASE)
                {
                        ch->mode = mode;
                        if (ch->state != SOUND_SILENT)
                                ch->state = mode;

                        return (1);
                }
                break;

    case MODE_INCREASE_LINEAR:
    case MODE_INCREASE_BENT_LINE:
                if (ch->mode != MODE_RELEASE)
                {
                        ch->mode = mode;
                        if (ch->state != SOUND_SILENT)
                                ch->state = mode;

                        return (1);
                }
                break;

    case MODE_ADSR:
                if (ch->mode == MODE_NONE || ch->mode == MODE_ADSR)
                {
                        ch->mode = mode;
                        return (1);
                }
    }

    return (0);
}

void S9xSetSoundControl (int sound_switch)
;

void S9xPlaySample (int channel)
;
