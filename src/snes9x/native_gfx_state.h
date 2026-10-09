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

/*****************************************************************************/
/* CPU-S9xOpcodes.CPP                                                                            */
/* This file contains all the opcodes                                                         */
/*****************************************************************************/


/* Original native types and helpers, expanded with the pinned EE ABI. */
#ifndef SNES_NATIVE_GFX_STATE_H
#define SNES_NATIVE_GFX_STATE_H

#define _SNES9X_H_

#define VERSION "1.41-1"



#define HUNT1041_V52_STDIO_H
#define fread snes_hidden_fread
#define fwrite snes_hidden_fwrite
extern "C" {

#define _STDIO_H_
#define _ANSIDECL_H_



#define __SYS_CONFIG_H__
typedef int __int32_t;
typedef unsigned int __uint32_t;



#define _POINTER_INT long






#define __RAND_MAX 0x7fffffff
#define __IMPORT






#define _READ_WRITE_RETURN_TYPE int
#define _HAVE_STDC



#define _PTR void *
#define _AND ,
#define _NOARGS void
#define _CONST const
#define _VOLATILE volatile
#define _SIGNED signed
#define _DOTS , ...
#define _VOID void




#define _EXFUN(name,proto) name proto
#define _EXPARM(name,proto) (* name) proto

#define _DEFUN(name,arglist,args) name(args)
#define _DEFUN_VOID(name) name(_NOARGS)
#define _CAST_VOID (void)

#define _LONG_DOUBLE long double


#define _PARAMS(paramlist) paramlist
#define _ATTRIBUTE(attrs) __attribute__ (attrs)






#define _FSTDIO

#define __need_size_t
#define __size_t__
#define __SIZE_T__
#define _SIZE_T
#define _SYS_SIZE_T_H
#define _T_SIZE_
#define _T_SIZE
#define __SIZE_T
#define _SIZE_T_
#define _BSD_SIZE_T_
#define _SIZE_T_DEFINED_
#define _SIZE_T_DEFINED
#define _BSD_SIZE_T_DEFINED_
#define _SIZE_T_DECLARED
#define ___int_size_t_h
#define _GCC_SIZE_T
#define _SIZET_



#define __size_t





typedef long unsigned int size_t;
#undef __need_size_t
#define __need___va_list
#undef __need___va_list




#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
extern "C" {

#define _SYS_REENT_H_
#define _SYS__TYPES_H

typedef long _off_t;
typedef long _ssize_t;
#define __Long __int32_t
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





#define _ATEXIT_SIZE 32

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
#define _RAND48_SEED_0 (0x330e)
#define _RAND48_SEED_1 (0xabcd)
#define _RAND48_SEED_2 (0x1234)
#define _RAND48_MULT_0 (0xe66d)
#define _RAND48_MULT_1 (0xdeec)
#define _RAND48_MULT_2 (0x0005)
#define _RAND48_ADD (0x000b)
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
#define _N_LISTS 30
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

#define _NULL 0

#define _REENT_INIT(var) { 0, &var.__sf[0], &var.__sf[1], &var.__sf[2], 0, "", 0, "C", 0, _NULL, _NULL, 0, _NULL, _NULL, 0, _NULL, { {0, _NULL, "", { 0,0,0,0,0,0,0,0}, 0, 1, {{_RAND48_SEED_0, _RAND48_SEED_1, _RAND48_SEED_2}, {_RAND48_MULT_0, _RAND48_MULT_1, _RAND48_MULT_2}, _RAND48_ADD}} } }
#define __ATTRIBUTE_IMPURE_PTR__


extern struct _reent *_impure_ptr ;

void _reclaim_reent (struct _reent *);




#define _REENT _impure_ptr



}



typedef _fpos_t fpos_t;

typedef struct __sFILE FILE;

#define __SLBF 0x0001
#define __SNBF 0x0002
#define __SRD 0x0004
#define __SWR 0x0008

#define __SRW 0x0010
#define __SEOF 0x0020
#define __SERR 0x0040
#define __SMBF 0x0080
#define __SAPP 0x0100
#define __SSTR 0x0200
#define __SOPT 0x0400
#define __SNPT 0x0800
#define __SOFF 0x1000
#define __SMOD 0x2000
#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2


#define NULL 0


#define EOF (-1)




#define BUFSIZ 1024





#define FOPEN_MAX 20





#define FILENAME_MAX 1024





#define L_tmpnam FILENAME_MAX



#define P_tmpdir "/tmp"



#define SEEK_SET 0


#define SEEK_CUR 1


#define SEEK_END 2


#define TMP_MAX 26

#define stdin (_impure_ptr->_stdin)
#define stdout (_impure_ptr->_stdout)
#define stderr (_impure_ptr->_stderr)

#define _stdin_r(x) ((x)->_stdin)
#define _stdout_r(x) ((x)->_stdout)
#define _stderr_r(x) ((x)->_stderr)






#define __VALIST __gnuc_va_list




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





#define fropen(cookie,fn) funopen(cookie, fn, (int (*)())0, (fpos_t (*)())0, (int (*)())0)
#define fwopen(cookie,fn) funopen(cookie, (int (*)())0, fn, (fpos_t (*)())0, (int (*)())0)






#define __sgetc_raw(p) (--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))
#define __sgetc(p) __sgetc_raw(p)
#define __sputc_raw(c,p) (--(p)->_w < 0 ? (p)->_w >= (p)->_lbfsize ? (*(p)->_p = (c)), *(p)->_p != '\n' ? (int)*(p)->_p++ : __swbuf('\n', p) : __swbuf((int)(c), p) : (*(p)->_p = (c), (int)*(p)->_p++))
#define __sputc(c,p) __sputc_raw(c, p)



#define __sfeof(p) (((p)->_flags & __SEOF) != 0)
#define __sferror(p) (((p)->_flags & __SERR) != 0)
#define __sclearerr(p) ((void)((p)->_flags &= ~(__SERR|__SEOF)))
#define __sfileno(p) ((p)->_file)

#define feof(p) __sfeof(p)
#define ferror(p) __sferror(p)
#define clearerr(p) __sclearerr(p)







#define getc(fp) __sgetc(fp)
#define putc(x,fp) __sputc(x, fp)



#define getchar() getc(stdin)
#define putchar(x) putc(x, stdout)



#define fast_putc(x,p) (--(p)->_w < 0 ? __swbuf((int)(x), p) == EOF : (*(p)->_p = (x), (p)->_p++, 0))


#define L_cuserid 9






}


#undef fread
#undef fwrite
extern "C" {
unsigned int fread(void *, unsigned int, unsigned int, FILE *);
unsigned int fwrite(const void *, unsigned int, unsigned int, FILE *);
}
extern "C" {

#define _STDLIB_H_



#define __need_size_t
#define __need_wchar_t
#undef __need_size_t
#define __wchar_t__
#define __WCHAR_T__
#define _WCHAR_T
#define _T_WCHAR_
#define _T_WCHAR
#define __WCHAR_T
#define _WCHAR_T_
#define _BSD_WCHAR_T_
#define _WCHAR_T_DEFINED_
#define _WCHAR_T_DEFINED
#define _WCHAR_T_H
#define ___int_wchar_t_h
#define __INT_WCHAR_T_H
#define _GCC_WCHAR_T
#define _WCHAR_T_DECLARED
#undef _BSD_WCHAR_T_
#undef __need_wchar_t
#define _NEWLIB_ALLOCA_H





#define alloca(size) __builtin_alloca(size)







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





#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX __RAND_MAX

extern int __mb_cur_max;

#define MB_CUR_MAX __mb_cur_max

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
#define _PORT_H_



#define _LIMITS_H___



#define CHAR_BIT 8



#define MB_LEN_MAX 1




#define SCHAR_MIN (-128)

#define SCHAR_MAX 127



#define UCHAR_MAX 255
#define CHAR_MIN (-128)

#define CHAR_MAX 127



#define __SHRT_MAX__ 32767




#define SHRT_MIN (-SHRT_MAX-1)

#define SHRT_MAX __SHRT_MAX__



#define __INT_MAX__ 2147483647


#define INT_MIN (-INT_MAX-1)

#define INT_MAX __INT_MAX__






#define USHRT_MAX (SHRT_MAX * 2 + 1)




#define UINT_MAX (INT_MAX * 2U + 1)
#define LONG_MIN (-LONG_MAX-1)

#define LONG_MAX __LONG_MAX__



#define ULONG_MAX (LONG_MAX * 2UL + 1)


#define __LONG_LONG_MAX__ 9223372036854775807LL
#define LONG_LONG_MIN (-LONG_LONG_MAX-1)

#define LONG_LONG_MAX __LONG_LONG_MAX__



#define ULONG_LONG_MAX (LONG_LONG_MAX * 2ULL + 1)







#define HUNT1041_V52_MEMORY_H


#define HUNT1041_V52_STRING_H
#define memchr snes_hidden_memchr
#define memcmp snes_hidden_memcmp
#define memcpy snes_hidden_memcpy
#define memmove snes_hidden_memmove
#define memset snes_hidden_memset
#define strcat snes_hidden_strcat
#define strchr snes_hidden_strchr
#define strcmp snes_hidden_strcmp
#define strcpy snes_hidden_strcpy
#define strlen snes_hidden_strlen
#define strncat snes_hidden_strncat
#define strncmp snes_hidden_strncmp
#define strncpy snes_hidden_strncpy
#define strrchr snes_hidden_strrchr
#define _STRING_H_


extern "C" {





#define __need_size_t
#undef __need_size_t
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
#define strcmpi strcasecmp


#define stricmp strcasecmp


#define strncmpi strncasecmp


#define strnicmp strncasecmp





}


#undef memchr
#undef memcmp
#undef memcpy
#undef memmove
#undef memset
#undef strcat
#undef strchr
#undef strcmp
#undef strcpy
#undef strlen
#undef strncat
#undef strncmp
#undef strncpy
#undef strrchr
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
#define ACCEPT_SIZE_T unsigned int
#define _SYS_TYPES_H
#define _STDDEF_H
#define _STDDEF_H_

#define _ANSI_STDDEF_H

#define __STDDEF_H__
#define _PTRDIFF_T
#define _T_PTRDIFF_
#define _T_PTRDIFF
#define __PTRDIFF_T
#define _PTRDIFF_T_
#define _BSD_PTRDIFF_T_
#define ___int_ptrdiff_t_h
#define _GCC_PTRDIFF_T



typedef long int ptrdiff_t;
#undef NULL

#define NULL __null
#define offsetof(TYPE,MEMBER) ((size_t) &((TYPE *)0)->MEMBER)







#define _MACHTYPES_H_

#define _CLOCK_T_ unsigned long
#define _TIME_T_ long
#define _CLOCKID_T_ unsigned long
#define _TIMER_T_ unsigned long
#define _ST_INT32 __attribute__ ((__mode__ (__SI__)))






#define physadr physadr_t
#define quad quad_t



typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
#define _BSDTYPES_DEFINED


typedef unsigned short ushort;
typedef unsigned int uint;



typedef unsigned long clock_t;
#define __clock_t_defined



typedef long time_t;
#define __time_t_defined



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
#define _SYS_TYPES_FD_SET
#define NBBY 8







#define FD_SETSIZE 64


typedef long fd_mask;
#define NFDBITS (sizeof (fd_mask) * NBBY)

#define howmany(x,y) (((x)+((y)-1))/(y))




typedef struct _types_fd_set {
        fd_mask fds_bits[(((64)+(((sizeof (fd_mask) * 8))-1))/((sizeof (fd_mask) * 8)))];
} _types_fd_set;

#define fd_set _types_fd_set

#define FD_SET(n,p) ((p)->fds_bits[(n)/NFDBITS] |= (1L << ((n) % NFDBITS)))
#define FD_CLR(n,p) ((p)->fds_bits[(n)/NFDBITS] &= ~(1L << ((n) % NFDBITS)))
#define FD_ISSET(n,p) ((p)->fds_bits[(n)/NFDBITS] & (1L << ((n) % NFDBITS)))
#define FD_ZERO(p) (__extension__ (void)({ size_t __i; char *__tmp = (char *)p; for (__i = 0; __i < sizeof (*(p)); ++__i) *__tmp++ = 0; }))
#undef _ST_INT32
#define PIXEL_FORMAT BGR555
#define snes9x_types_defined

typedef unsigned char bool8;


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef signed char int8;
typedef short int16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
#define _PIXFORM_H_
#define BUILD_PIXEL_RGB565(R,G,B) (((int) (R) << 11) | ((int) (G) << 6) | (int) (B))
#define BUILD_PIXEL2_RGB565(R,G,B) (((int) (R) << 11) | ((int) (G) << 5) | (int) (B))
#define DECOMPOSE_PIXEL_RGB565(PIX,R,G,B) {(R) = (PIX) >> 11; (G) = ((PIX) >> 6) & 0x1f; (B) = (PIX) & 0x1f; }
#define SPARE_RGB_BIT_MASK_RGB565 (1 << 5)

#define MAX_RED_RGB565 31
#define MAX_GREEN_RGB565 63
#define MAX_BLUE_RGB565 31
#define RED_LOW_BIT_MASK_RGB565 0x0800
#define GREEN_LOW_BIT_MASK_RGB565 0x0020
#define BLUE_LOW_BIT_MASK_RGB565 0x0001
#define RED_HI_BIT_MASK_RGB565 0x8000
#define GREEN_HI_BIT_MASK_RGB565 0x0400
#define BLUE_HI_BIT_MASK_RGB565 0x0010
#define FIRST_COLOR_MASK_RGB565 0xF800
#define SECOND_COLOR_MASK_RGB565 0x07E0
#define THIRD_COLOR_MASK_RGB565 0x001F
#define ALPHA_BITS_MASK_RGB565 0x0000


#define BUILD_PIXEL_RGB555(R,G,B) (((int) (R) << 10) | ((int) (G) << 5) | (int) (B))
#define BUILD_PIXEL2_RGB555(R,G,B) (((int) (R) << 10) | ((int) (G) << 5) | (int) (B))
#define DECOMPOSE_PIXEL_RGB555(PIX,R,G,B) {(R) = (PIX) >> 10; (G) = ((PIX) >> 5) & 0x1f; (B) = (PIX) & 0x1f; }
#define SPARE_RGB_BIT_MASK_RGB555 (1 << 15)

#define MAX_RED_RGB555 31
#define MAX_GREEN_RGB555 31
#define MAX_BLUE_RGB555 31
#define RED_LOW_BIT_MASK_RGB555 0x0400
#define GREEN_LOW_BIT_MASK_RGB555 0x0020
#define BLUE_LOW_BIT_MASK_RGB555 0x0001
#define RED_HI_BIT_MASK_RGB555 0x4000
#define GREEN_HI_BIT_MASK_RGB555 0x0200
#define BLUE_HI_BIT_MASK_RGB555 0x0010
#define FIRST_COLOR_MASK_RGB555 0x7C00
#define SECOND_COLOR_MASK_RGB555 0x03E0
#define THIRD_COLOR_MASK_RGB555 0x001F
#define ALPHA_BITS_MASK_RGB555 0x0000


#define BUILD_PIXEL_BGR565(R,G,B) (((int) (B) << 11) | ((int) (G) << 6) | (int) (R))
#define BUILD_PIXEL2_BGR565(R,G,B) (((int) (B) << 11) | ((int) (G) << 5) | (int) (R))
#define DECOMPOSE_PIXEL_BGR565(PIX,R,G,B) {(B) = (PIX) >> 11; (G) = ((PIX) >> 6) & 0x1f; (R) = (PIX) & 0x1f; }
#define SPARE_RGB_BIT_MASK_BGR565 (1 << 5)

#define MAX_RED_BGR565 31
#define MAX_GREEN_BGR565 63
#define MAX_BLUE_BGR565 31
#define RED_LOW_BIT_MASK_BGR565 0x0001
#define GREEN_LOW_BIT_MASK_BGR565 0x0040
#define BLUE_LOW_BIT_MASK_BGR565 0x0800
#define RED_HI_BIT_MASK_BGR565 0x0010
#define GREEN_HI_BIT_MASK_BGR565 0x0400
#define BLUE_HI_BIT_MASK_BGR565 0x8000
#define FIRST_COLOR_MASK_BGR565 0xF800
#define SECOND_COLOR_MASK_BGR565 0x07E0
#define THIRD_COLOR_MASK_BGR565 0x001F
#define ALPHA_BITS_MASK_BGR565 0x0000


#define BUILD_PIXEL_BGR555(R,G,B) (((int) (B) << 10) | ((int) (G) << 5) | (int) (R))
#define BUILD_PIXEL2_BGR555(R,G,B) (((int) (B) << 10) | ((int) (G) << 5) | (int) (R))
#define DECOMPOSE_PIXEL_BGR555(PIX,R,G,B) {(B) = (PIX) >> 10; (G) = ((PIX) >> 5) & 0x1f; (R) = (PIX) & 0x1f; }
#define SPARE_RGB_BIT_MASK_BGR555 (1 << 15)

#define MAX_RED_BGR555 31
#define MAX_GREEN_BGR555 31
#define MAX_BLUE_BGR555 31
#define RED_LOW_BIT_MASK_BGR555 0x0001
#define GREEN_LOW_BIT_MASK_BGR555 0x0020
#define BLUE_LOW_BIT_MASK_BGR555 0x0400
#define RED_HI_BIT_MASK_BGR555 0x0010
#define GREEN_HI_BIT_MASK_BGR555 0x0200
#define BLUE_HI_BIT_MASK_BGR555 0x4000
#define FIRST_COLOR_MASK_BGR555 0x7C00
#define SECOND_COLOR_MASK_BGR555 0x03E0
#define THIRD_COLOR_MASK_BGR555 0x001F
#define ALPHA_BITS_MASK_BGR555 0x0000


#define BUILD_PIXEL_GBR565(R,G,B) (((int) (G) << 11) | ((int) (B) << 6) | (int) (R))
#define BUILD_PIXEL2_GBR565(R,G,B) (((int) (G) << 11) | ((int) (B) << 5) | (int) (R))
#define DECOMPOSE_PIXEL_GBR565(PIX,R,G,B) {(G) = (PIX) >> 11; (B) = ((PIX) >> 6) & 0x1f; (R) = (PIX) & 0x1f; }
#define SPARE_RGB_BIT_MASK_GBR565 (1 << 5)

#define MAX_RED_GBR565 31
#define MAX_BLUE_GBR565 63
#define MAX_GREEN_GBR565 31
#define RED_LOW_BIT_MASK_GBR565 0x0001
#define BLUE_LOW_BIT_MASK_GBR565 0x0040
#define GREEN_LOW_BIT_MASK_GBR565 0x0800
#define RED_HI_BIT_MASK_GBR565 0x0010
#define BLUE_HI_BIT_MASK_GBR565 0x0400
#define GREEN_HI_BIT_MASK_GBR565 0x8000
#define FIRST_COLOR_MASK_GBR565 0xF800
#define SECOND_COLOR_MASK_GBR565 0x07E0
#define THIRD_COLOR_MASK_GBR565 0x001F
#define ALPHA_BITS_MASK_GBR565 0x0000


#define BUILD_PIXEL_GBR555(R,G,B) (((int) (G) << 10) | ((int) (B) << 5) | (int) (R))
#define BUILD_PIXEL2_GBR555(R,G,B) (((int) (G) << 10) | ((int) (B) << 5) | (int) (R))
#define DECOMPOSE_PIXEL_GBR555(PIX,R,G,B) {(G) = (PIX) >> 10; (B) = ((PIX) >> 5) & 0x1f; (R) = (PIX) & 0x1f; }
#define SPARE_RGB_BIT_MASK_GBR555 (1 << 15)

#define MAX_RED_GBR555 31
#define MAX_BLUE_GBR555 31
#define MAX_GREEN_GBR555 31
#define RED_LOW_BIT_MASK_GBR555 0x0001
#define BLUE_LOW_BIT_MASK_GBR555 0x0020
#define GREEN_LOW_BIT_MASK_GBR555 0x0400
#define RED_HI_BIT_MASK_GBR555 0x0010
#define BLUE_HI_BIT_MASK_GBR555 0x0200
#define GREEN_HI_BIT_MASK_GBR555 0x4000
#define FIRST_COLOR_MASK_GBR555 0x7C00
#define SECOND_COLOR_MASK_GBR555 0x03E0
#define THIRD_COLOR_MASK_GBR555 0x001F
#define ALPHA_BITS_MASK_GBR555 0x0000


#define BUILD_PIXEL_RGB5551(R,G,B) (((int) (R) << 11) | ((int) (G) << 6) | (int) ((B) << 1) | 1)
#define BUILD_PIXEL2_RGB5551(R,G,B) (((int) (R) << 11) | ((int) (G) << 6) | (int) ((B) << 1) | 1)
#define DECOMPOSE_PIXEL_RGB5551(PIX,R,G,B) {(R) = (PIX) >> 11; (G) = ((PIX) >> 6) & 0x1f; (B) = ((PIX) >> 1) & 0x1f; }
#define SPARE_RGB_BIT_MASK_RGB5551 (1)

#define MAX_RED_RGB5551 31
#define MAX_GREEN_RGB5551 31
#define MAX_BLUE_RGB5551 31
#define RED_LOW_BIT_MASK_RGB5551 0x0800
#define GREEN_LOW_BIT_MASK_RGB5551 0x0040
#define BLUE_LOW_BIT_MASK_RGB5551 0x0002
#define RED_HI_BIT_MASK_RGB5551 0x8000
#define GREEN_HI_BIT_MASK_RGB5551 0x0400
#define BLUE_HI_BIT_MASK_RGB5551 0x0020
#define FIRST_COLOR_MASK_RGB5551 0xf800
#define SECOND_COLOR_MASK_RGB5551 0x07c0
#define THIRD_COLOR_MASK_RGB5551 0x003e
#define ALPHA_BITS_MASK_RGB5551 0x0001


#define CONCAT(X,Y) X ##Y



#define BUILD_PIXEL_D(F,R,G,B) CONCAT(BUILD_PIXEL_,F) (R,G,B)
#define BUILD_PIXEL2_D(F,R,G,B) CONCAT(BUILD_PIXEL2_,F) (R,G,B)
#define DECOMPOSE_PIXEL_D(F,PIX,R,G,B) CONCAT(DECOMPOSE_PIXEL_,F) (PIX,R,G,B)

#define BUILD_PIXEL(R,G,B) BUILD_PIXEL_D(PIXEL_FORMAT,R,G,B)
#define BUILD_PIXEL2(R,G,B) BUILD_PIXEL2_D(PIXEL_FORMAT,R,G,B)
#define DECOMPOSE_PIXEL(PIX,R,G,B) DECOMPOSE_PIXEL_D(PIXEL_FORMAT,PIX,R,G,B)

#define MAX_RED_D(F) CONCAT(MAX_RED_,F)
#define MAX_BLUE_D(F) CONCAT(MAX_BLUE_,F)
#define MAX_GREEN_D(F) CONCAT(MAX_GREEN_,F)
#define RED_LOW_BIT_MASK_D(F) CONCAT(RED_LOW_BIT_MASK_,F)
#define BLUE_LOW_BIT_MASK_D(F) CONCAT(BLUE_LOW_BIT_MASK_,F)
#define GREEN_LOW_BIT_MASK_D(F) CONCAT(GREEN_LOW_BIT_MASK_,F)
#define RED_HI_BIT_MASK_D(F) CONCAT(RED_HI_BIT_MASK_,F)
#define BLUE_HI_BIT_MASK_D(F) CONCAT(BLUE_HI_BIT_MASK_,F)
#define GREEN_HI_BIT_MASK_D(F) CONCAT(GREEN_HI_BIT_MASK_,F)
#define FIRST_COLOR_MASK_D(F) CONCAT(FIRST_COLOR_MASK_,F)
#define SECOND_COLOR_MASK_D(F) CONCAT(SECOND_COLOR_MASK_,F)
#define THIRD_COLOR_MASK_D(F) CONCAT(THIRD_COLOR_MASK_,F)
#define ALPHA_BITS_MASK_D(F) CONCAT(ALPHA_BITS_MASK_,F)

#define MAX_RED MAX_RED_D(PIXEL_FORMAT)
#define MAX_BLUE MAX_BLUE_D(PIXEL_FORMAT)
#define MAX_GREEN MAX_GREEN_D(PIXEL_FORMAT)
#define RED_LOW_BIT_MASK RED_LOW_BIT_MASK_D(PIXEL_FORMAT)
#define BLUE_LOW_BIT_MASK BLUE_LOW_BIT_MASK_D(PIXEL_FORMAT)
#define GREEN_LOW_BIT_MASK GREEN_LOW_BIT_MASK_D(PIXEL_FORMAT)
#define RED_HI_BIT_MASK RED_HI_BIT_MASK_D(PIXEL_FORMAT)
#define BLUE_HI_BIT_MASK BLUE_HI_BIT_MASK_D(PIXEL_FORMAT)
#define GREEN_HI_BIT_MASK GREEN_HI_BIT_MASK_D(PIXEL_FORMAT)
#define FIRST_COLOR_MASK FIRST_COLOR_MASK_D(PIXEL_FORMAT)
#define SECOND_COLOR_MASK SECOND_COLOR_MASK_D(PIXEL_FORMAT)
#define THIRD_COLOR_MASK THIRD_COLOR_MASK_D(PIXEL_FORMAT)
#define ALPHA_BITS_MASK ALPHA_BITS_MASK_D(PIXEL_FORMAT)

#define GREEN_HI_BIT ((MAX_GREEN + 1) >> 1)
#define RGB_LOW_BITS_MASK (RED_LOW_BIT_MASK | GREEN_LOW_BIT_MASK | BLUE_LOW_BIT_MASK)

#define RGB_HI_BITS_MASK (RED_HI_BIT_MASK | GREEN_HI_BIT_MASK | BLUE_HI_BIT_MASK)

#define RGB_HI_BITS_MASKx2 ((RED_HI_BIT_MASK | GREEN_HI_BIT_MASK | BLUE_HI_BIT_MASK) << 1)

#define RGB_REMOVE_LOW_BITS_MASK (~RGB_LOW_BITS_MASK)
#define FIRST_THIRD_COLOR_MASK (FIRST_COLOR_MASK | THIRD_COLOR_MASK)
#define TWO_LOW_BITS_MASK (RGB_LOW_BITS_MASK | (RGB_LOW_BITS_MASK << 1))
#define HIGH_BITS_SHIFTED_TWO_MASK (( (FIRST_COLOR_MASK | SECOND_COLOR_MASK | THIRD_COLOR_MASK) & ~TWO_LOW_BITS_MASK ) >> 2)






#define TRUE 1



#define FALSE 0
#define EXTERN_C extern "C"
#define START_EXTERN_C extern "C" {
#define END_EXTERN_C }
#define PATH_MAX 1024


#define _MAX_DIR PATH_MAX
#define _MAX_DRIVE 1
#define _MAX_FNAME PATH_MAX
#define _MAX_EXT PATH_MAX
#define _MAX_PATH PATH_MAX

#define ZeroMemory(a,b) memset((a),0,(b))

void _makepath (char *path, const char *drive, const char *dir,
                const char *fname, const char *ext);
void _splitpath (const char *path, char *drive, char *dir, char *fname,
                 char *ext);





extern "C" void S9xGenerateSound ();







#define CHECK_SOUND()






#define SLASH_STR "/"
#define SLASH_CHAR '/'
#define SIG_PF void(*)(int)







#define MSB_FIRST
#define TITLE "Snes9x"






#define STATIC static
#define _65c816_h_

#define AL A.B.l
#define AH A.B.h
#define XL X.B.l
#define XH X.B.h
#define YL Y.B.l
#define YH Y.B.h
#define SL S.B.l
#define SH S.B.h
#define DL D.B.l
#define DH D.B.h
#define PL P.B.l
#define PH P.B.h

#define Carry 1
#define Zero 2
#define IRQ 4
#define Decimal 8
#define IndexFlag 16
#define MemoryFlag 32
#define Overflow 64
#define Negative 128
#define Emulation 256

#define ClearCarry() (ICPU._Carry = 0)
#define SetCarry() (ICPU._Carry = 1)
#define SetZero() (ICPU._Zero = 0)
#define ClearZero() (ICPU._Zero = 1)
#define SetIRQ() (Registers.PL |= IRQ)
#define ClearIRQ() (Registers.PL &= ~IRQ)
#define SetDecimal() (Registers.PL |= Decimal)
#define ClearDecimal() (Registers.PL &= ~Decimal)
#define SetIndex() (Registers.PL |= IndexFlag)
#define ClearIndex() (Registers.PL &= ~IndexFlag)
#define SetMemory() (Registers.PL |= MemoryFlag)
#define ClearMemory() (Registers.PL &= ~MemoryFlag)
#define SetOverflow() (ICPU._Overflow = 1)
#define ClearOverflow() (ICPU._Overflow = 0)
#define SetNegative() (ICPU._Negative = 0x80)
#define ClearNegative() (ICPU._Negative = 0)

#define CheckZero() (ICPU._Zero == 0)
#define CheckCarry() (ICPU._Carry)
#define CheckIRQ() (Registers.PL & IRQ)
#define CheckDecimal() (Registers.PL & Decimal)
#define CheckIndex() (Registers.PL & IndexFlag)
#define CheckMemory() (Registers.PL & MemoryFlag)
#define CheckOverflow() (ICPU._Overflow)
#define CheckNegative() (ICPU._Negative & 0x80)
#define CheckEmulation() (Registers.P.W & Emulation)

#define ClearFlags(f) (Registers.P.W &= ~(f))
#define SetFlags(f) (Registers.P.W |= (f))
#define CheckFlag(f) (Registers.PL & (f))

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
#define _messages_h_


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







#define ROM_NAME_LEN 23
#define _ZLIB_H
#define _ZCONF_H
#define __32BIT__
#define STDC
#define MAX_MEM_LEVEL 9
#define MAX_WBITS 15
#define OF(args) args
#define ZEXPORT


#define ZEXPORTVA


#define ZEXTERN extern



#define FAR



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
#define z_off_t long
extern "C" {


#define ZLIB_VERSION "1.1.3"
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
#define Z_NO_FLUSH 0
#define Z_PARTIAL_FLUSH 1
#define Z_SYNC_FLUSH 2
#define Z_FULL_FLUSH 3
#define Z_FINISH 4


#define Z_OK 0
#define Z_STREAM_END 1
#define Z_NEED_DICT 2
#define Z_ERRNO (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR (-3)
#define Z_MEM_ERROR (-4)
#define Z_BUF_ERROR (-5)
#define Z_VERSION_ERROR (-6)




#define Z_NO_COMPRESSION 0
#define Z_BEST_SPEED 1
#define Z_BEST_COMPRESSION 9
#define Z_DEFAULT_COMPRESSION (-1)


#define Z_FILTERED 1
#define Z_HUFFMAN_ONLY 2
#define Z_DEFAULT_STRATEGY 0


#define Z_BINARY 0
#define Z_ASCII 1
#define Z_UNKNOWN 2


#define Z_DEFLATED 8


#define Z_NULL 0

#define zlib_version zlibVersion()




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

#define deflateInit(strm,level) deflateInit_((strm), (level), ZLIB_VERSION, sizeof(z_stream))

#define inflateInit(strm) inflateInit_((strm), ZLIB_VERSION, sizeof(z_stream))

#define deflateInit2(strm,level,method,windowBits,memLevel,strategy) deflateInit2_((strm),(level),(method),(windowBits),(memLevel), (strategy), ZLIB_VERSION, sizeof(z_stream))


#define inflateInit2(strm,windowBits) inflateInit2_((strm), (windowBits), ZLIB_VERSION, sizeof(z_stream))




    struct internal_state {int dummy;};


extern const char * zError (int err);
extern int inflateSyncPoint (z_streamp z);
extern const uLongf * get_crc_table (void);


}




#define STREAM gzFile
#define READ_STREAM(p,l,s) gzread (s,p,l)
#define WRITE_STREAM(p,l,s) gzwrite (s,p,l)
#define OPEN_STREAM(f,m) gzopen (f,m)
#define FIND_STREAM(f) gztell(f)
#define REVERT_STREAM(f,o,s) gzseek(f,o,s)
#define CLOSE_STREAM(s) gzclose (s)
#define SNES_WIDTH 256
#define SNES_HEIGHT 224
#define SNES_HEIGHT_EXTENDED 239
#define IMAGE_WIDTH (Settings.SupportHiRes ? SNES_WIDTH * 2 : SNES_WIDTH)
#define IMAGE_HEIGHT (Settings.SupportHiRes ? SNES_HEIGHT_EXTENDED * 2 : SNES_HEIGHT_EXTENDED)

#define SNES_MAX_NTSC_VCOUNTER 262
#define SNES_MAX_PAL_VCOUNTER 312
#define SNES_HCOUNTER_MAX 342
#define SPC700_TO_65C816_RATIO 2
#define AUTO_FRAMERATE 200
#define SNES_SCANLINE_TIME (63.695e-6)
#define SNES_CLOCK_SPEED (3579545)

#define SNES_CLOCK_LEN (1.0 / SNES_CLOCK_SPEED)


#define SNES_CYCLES_PER_SCANLINE ((uint32) ((SNES_SCANLINE_TIME / SNES_CLOCK_LEN) * 6 + 0.5))




#define SNES_TR_MASK (1 << 4)
#define SNES_TL_MASK (1 << 5)
#define SNES_X_MASK (1 << 6)
#define SNES_A_MASK (1 << 7)
#define SNES_RIGHT_MASK (1 << 8)
#define SNES_LEFT_MASK (1 << 9)
#define SNES_DOWN_MASK (1 << 10)
#define SNES_UP_MASK (1 << 11)
#define SNES_START_MASK (1 << 12)
#define SNES_SELECT_MASK (1 << 13)
#define SNES_Y_MASK (1 << 14)
#define SNES_B_MASK (1 << 15)

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

#define DEBUG_MODE_FLAG (1 << 0)
#define TRACE_FLAG (1 << 1)
#define SINGLE_STEP_FLAG (1 << 2)
#define BREAK_FLAG (1 << 3)
#define SCAN_KEYS_FLAG (1 << 4)
#define SAVE_SNAPSHOT_FLAG (1 << 5)
#define DELAYED_NMI_FLAG (1 << 6)
#define NMI_FLAG (1 << 7)
#define PROCESS_SOUND_FLAG (1 << 8)
#define FRAME_ADVANCE_FLAG (1 << 9)
#define DELAYED_NMI_FLAG2 (1 << 10)
#define IRQ_PENDING_FLAG (1 << 11)


#define ONE_CYCLE 6
#define SLOW_ONE_CYCLE 8
#define TWO_CYCLES 12






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

#define HBLANK_START_EVENT 0
#define HBLANK_END_EVENT 1
#define HTIMER_BEFORE_EVENT 2
#define HTIMER_AFTER_EVENT 3
#define NO_EVENT 4

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
#define _memmap_h_
#define READ_WORD(s) ( *(uint8 *) (s) | (*((uint8 *) (s) + 1) << 8))

#define READ_DWORD(s) ( *(uint8 *) (s) | (*((uint8 *) (s) + 1) << 8) | (*((uint8 *) (s) + 2) << 16) | (*((uint8 *) (s) + 3) << 24))



#define WRITE_WORD(s,d) *(uint8 *) (s) = (d), *((uint8 *) (s) + 1) = (d) >> 8

#define WRITE_DWORD(s,d) *(uint8 *) (s) = (uint8) (d), *((uint8 *) (s) + 1) = (uint8) ((d) >> 8), *((uint8 *) (s) + 2) = (uint8) ((d) >> 16), *((uint8 *) (s) + 3) = (uint8) ((d) >> 24)



#define WRITE_3WORD(s,d) *(uint8 *) (s) = (uint8) (d), *((uint8 *) (s) + 1) = (uint8) ((d) >> 8), *((uint8 *) (s) + 2) = (uint8) ((d) >> 16)


#define READ_3WORD(s) ( *(uint8 *) (s) | (*((uint8 *) (s) + 1) << 8) | (*((uint8 *) (s) + 2) << 16))




#define MEMMAP_BLOCK_SIZE (0x1000)
#define MEMMAP_NUM_BLOCKS (0x1000000 / MEMMAP_BLOCK_SIZE)
#define MEMMAP_BLOCKS_PER_BANK (0x10000 / MEMMAP_BLOCK_SIZE)
#define MEMMAP_SHIFT 12
#define MEMMAP_MASK (MEMMAP_BLOCK_SIZE - 1)
#define MEMMAP_MAX_SDD1_LOGGED_ENTRIES (0x10000 / 8)

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
#define _PPU_H_

#define FIRST_VISIBLE_LINE 1

extern uint8 GetBank;
extern uint16 SignExtend [2];

#define TILE_2BIT 0
#define TILE_4BIT 1
#define TILE_8BIT 2

#define MAX_2BIT_TILES 4096
#define MAX_4BIT_TILES 2048
#define MAX_8BIT_TILES 1024

#define PPU_H_BEAM_IRQ_SOURCE (1 << 0)
#define PPU_V_BEAM_IRQ_SOURCE (1 << 1)
#define GSU_IRQ_SOURCE (1 << 2)
#define SA1_IRQ_SOURCE (1 << 7)
#define SA1_DMA_IRQ_SOURCE (1 << 5)

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

#define CLIP_OR 0
#define CLIP_AND 1
#define CLIP_XOR 2
#define CLIP_XNOR 3

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
extern struct SDMA DMA [8];
extern struct InternalPPU IPPU __asm__("DAT_0035c268");
}
#define _GFX_H_




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

#define H_FLIP 0x4000
#define V_FLIP 0x8000
#define BLANK_TILE 2

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

extern uint32 odd_high [4][16] __asm__("DAT_0035f99c+0x4");
extern uint32 odd_low [4][16] __asm__("DAT_0035f99c+0x104");
extern uint32 even_high [4][16] __asm__("DAT_0035fda0-0x200");
extern uint32 even_low [4][16] __asm__("DAT_0035fda0-0x100");
extern SBG BG __asm__("DAT_0035d450");
extern uint16 DirectColourMaps [8][256] __asm__("DAT_003f2f80");

extern uint8 add32_32 [32][32];
extern uint8 add32_32_half [32][32];
extern uint8 sub32_32 [32][32];
extern uint8 sub32_32_half [32][32];
extern uint8 mul_brightness [16][32];


#define SWAP_DWORD(dw) dw = ((dw & 0xff) << 24) | ((dw & 0xff00) << 8) | ((dw & 0xff0000) >> 8) | ((dw & 0xff000000) >> 24)







#define READ_2BYTES(s) (*(uint8 *) (s) | (*((uint8 *) (s) + 1) << 8))
#define WRITE_2BYTES(s,d) *(uint8 *) (s) = (d), *((uint8 *) (s) + 1) = (d) >> 8
#define SUB_SCREEN_DEPTH 0
#define MAIN_SCREEN_DEPTH 32







#define COLOR_ADD(C1,C2) (GFX.X2 [((((C1) & RGB_REMOVE_LOW_BITS_MASK) + ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1) + ((C1) & (C2) & RGB_LOW_BITS_MASK)] | (((C1) ^ (C2)) & RGB_LOW_BITS_MASK))






#define COLOR_ADD1_2(C1,C2) (((((C1) & RGB_REMOVE_LOW_BITS_MASK) + ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1) + ((C1) & (C2) & RGB_LOW_BITS_MASK) | ALPHA_BITS_MASK)
#define COLOR_SUB(C1,C2) (GFX.ZERO_OR_X2 [(((C1) | RGB_HI_BITS_MASKx2) - ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1] + ((C1) & RGB_LOW_BITS_MASK) - ((C2) & RGB_LOW_BITS_MASK))





#define COLOR_SUB1_2(C1,C2) GFX.ZERO [(((C1) | RGB_HI_BITS_MASKx2) - ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1]



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



extern struct SGFX GFX __asm__("DAT_0035d480");

bool8 S9xGraphicsInit ();
void S9xGraphicsDeinit();
bool8 S9xInitUpdate (void);
bool8 S9xDeinitUpdate (int Width, int Height, bool8 sixteen_bit);
void S9xSetPalette ();
void S9xSyncSpeed ();





}




#define MAX_5C77_VERSION 0x01
#define MAX_5C78_VERSION 0x03
#define MAX_5A22_VERSION 0x02

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
#define _CPUEXEC_H_




#define DO_HBLANK_CHECK() if (CPU.Cycles >= CPU.NextEvent) S9xDoHBlankProcessing ();



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
#define _DISPLAY_H_

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
#define _apu_h_
#define _SPC700_H_
#define Carry 1
#define Zero 2
#define Interrupt 4
#define HalfCarry 8
#define BreakFlag 16
#define DirectPageFlag 32
#define Overflow 64
#define Negative 128

#define APUClearCarry() (IAPU._Carry = 0)
#define APUSetCarry() (IAPU._Carry = 1)
#define APUSetInterrupt() (APURegisters.P |= Interrupt)
#define APUClearInterrupt() (APURegisters.P &= ~Interrupt)
#define APUSetHalfCarry() (APURegisters.P |= HalfCarry)
#define APUClearHalfCarry() (APURegisters.P &= ~HalfCarry)
#define APUSetBreak() (APURegisters.P |= BreakFlag)
#define APUClearBreak() (APURegisters.P &= ~BreakFlag)
#define APUSetDirectPage() (APURegisters.P |= DirectPageFlag)
#define APUClearDirectPage() (APURegisters.P &= ~DirectPageFlag)
#define APUSetOverflow() (IAPU._Overflow = 1)
#define APUClearOverflow() (IAPU._Overflow = 0)

#define APUCheckZero() (IAPU._Zero == 0)
#define APUCheckCarry() (IAPU._Carry)
#define APUCheckInterrupt() (APURegisters.P & Interrupt)
#define APUCheckHalfCarry() (APURegisters.P & HalfCarry)
#define APUCheckBreak() (APURegisters.P & BreakFlag)
#define APUCheckDirectPage() (APURegisters.P & DirectPageFlag)
#define APUCheckOverflow() (IAPU._Overflow)
#define APUCheckNegative() (IAPU._Zero & 0x80)

#define APUClearFlags(f) (APURegisters.P &= ~(f))
#define APUSetFlags(f) (APURegisters.P |= (f))
#define APUCheckFlag(f) (APURegisters.P & (f))

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



#define ONE_APU_CYCLE 21



#define ONE_APU_CYCLE_HUMAN 21
#define APU_EXECUTE1() { APU.Cycles += S9xAPUCycles [*IAPU.PC]; (*S9xApuOpcodes[*IAPU.PC]) (); }






#define APU_EXECUTE() if (IAPU.APUExecuting) { while (APU.Cycles <= CPU.Cycles) APU_EXECUTE1(); }
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

extern "C" struct SAPU APU;
extern "C" struct SIAPU IAPU;
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


#define APU_VOL_LEFT 0x00
#define APU_VOL_RIGHT 0x01
#define APU_P_LOW 0x02
#define APU_P_HIGH 0x03
#define APU_SRCN 0x04
#define APU_ADSR1 0x05
#define APU_ADSR2 0x06
#define APU_GAIN 0x07
#define APU_ENVX 0x08
#define APU_OUTX 0x09

#define APU_MVOL_LEFT 0x0c
#define APU_MVOL_RIGHT 0x1c
#define APU_EVOL_LEFT 0x2c
#define APU_EVOL_RIGHT 0x3c
#define APU_KON 0x4c
#define APU_KOFF 0x5c
#define APU_FLG 0x6c
#define APU_ENDX 0x7c

#define APU_EFB 0x0d
#define APU_PMON 0x2d
#define APU_NON 0x3d
#define APU_EON 0x4d
#define APU_DIR 0x5d
#define APU_ESA 0x6d
#define APU_EDL 0x7d

#define APU_C0 0x0f
#define APU_C1 0x1f
#define APU_C2 0x2f
#define APU_C3 0x3f
#define APU_C4 0x4f
#define APU_C5 0x5f
#define APU_C6 0x6f
#define APU_C7 0x7f

#define APU_SOFT_RESET 0x80
#define APU_MUTE 0x40
#define APU_ECHO_DISABLED 0x20

#define FREQUENCY_MASK 0x3fff
#define _CHEATS_H_

struct SCheat
{
    uint32 address;
    uint8 byte;
    uint8 saved_byte;
    bool8 enabled;
    bool8 saved;
    char name [22];
};

#define MAX_CHEATS 75

struct SCheatData
{
    struct SCheat c [75];
    uint32 num_cheats;
    uint8 CWRAM [0x20000];
    uint8 CSRAM [0x10000];
    uint8 CIRAM [0x2000];
    uint8 *RAM;
    uint8 *FillRAM;
    uint8 *SRAM;
    uint32 WRAM_BITS [0x20000 >> 3];
    uint32 SRAM_BITS [0x10000 >> 3];
    uint32 IRAM_BITS [0x2000 >> 3];
};

typedef enum
{
    S9X_LESS_THAN, S9X_GREATER_THAN, S9X_LESS_THAN_OR_EQUAL,
    S9X_GREATER_THAN_OR_EQUAL, S9X_EQUAL, S9X_NOT_EQUAL
} S9xCheatComparisonType;

typedef enum
{
    S9X_8_BITS, S9X_16_BITS, S9X_24_BITS, S9X_32_BITS
} S9xCheatDataSize;

void S9xInitCheatData ();

const char *S9xGameGenieToRaw (const char *code, uint32 &address, uint8 &byte);
const char *S9xProActionReplayToRaw (const char *code, uint32 &address, uint8 &byte);
const char *S9xGoldFingerToRaw (const char *code, uint32 &address, bool8 &sram,
                                uint8 &num_bytes, uint8 bytes[3]);
void S9xApplyCheats ();
void S9xApplyCheat (uint32 which1);
void S9xRemoveCheats ();
void S9xRemoveCheat (uint32 which1);
void S9xEnableCheat (uint32 which1);
void S9xDisableCheat (uint32 which1);
void S9xAddCheat (bool8 enable, bool8 save_current_value, uint32 address,
                  uint8 byte);
void S9xDeleteCheats ();
void S9xDeleteCheat (uint32 which1);
bool8 S9xLoadCheatFile (const char *filename);
bool8 S9xSaveCheatFile (const char *filename);

void S9xStartCheatSearch (SCheatData *);
void S9xSearchForChange (SCheatData *, S9xCheatComparisonType cmp,
                         S9xCheatDataSize size, bool8 is_signed, bool8 update);
void S9xSearchForValue (SCheatData *, S9xCheatComparisonType cmp,
                        S9xCheatDataSize size, uint32 value,
                        bool8 is_signed, bool8 update);
void S9xOutputCheatSearchResults (SCheatData *);
#define SCREENSHOT_H

bool8 S9xDoScreenshot(int width, int height);



#define M7 19
#define M8 19

void output_png();
void ComputeClipWindows ();
static void S9xDisplayFrameRate ();
static void S9xDisplayString (const char *string);

extern uint8 BitShifts[8][4] __asm__("DAT_003f2eb8+0x20");
extern uint8 TileShifts[8][4] __asm__("DAT_003f2eb8+0x40");
extern uint8 PaletteShifts[8][4] __asm__("DAT_003f2eb8+0x60");
extern uint8 PaletteMasks[8][4] __asm__("DAT_003f2f78-0x40");
extern uint8 Depths[8][4] __asm__("DAT_003f2f78-0x20");
extern uint8 BGSizes [2] __asm__("DAT_003f2f78");

extern NormalTileRenderer DrawTilePtr __asm__("DAT_0035f98c");
extern ClippedTileRenderer DrawClippedTilePtr __asm__("DAT_0035f990");
extern NormalTileRenderer DrawHiResTilePtr __asm__("DAT_0035f994");
extern ClippedTileRenderer DrawHiResClippedTilePtr __asm__("DAT_0035f998");
extern LargePixelRenderer DrawLargePixelPtr __asm__("DAT_0035f99c");

extern struct SBG BG __asm__("DAT_0035d450");

extern struct SLineData LineData[240] __asm__("DAT_0035df48");
extern struct SLineMatrixData LineMatrixData [240] __asm__("DAT_0035ee48");

extern uint8 Mode7Depths [2] __asm__("DAT_0035f988");

#define CLIP_10_BIT_SIGNED(a) ((a) & ((1 << 10) - 1)) + (((((a) & (1 << 13)) ^ (1 << 13)) - (1 << 13)) >> 3)


#define ON_MAIN(N) (GFX.r212c & (1 << (N)) && !(PPU.BG_Forced & (1 << (N))))



#define SUB_OR_ADD(N) (GFX.r2131 & (1 << (N)))


#define ON_SUB(N) ((GFX.r2130 & 0x30) != 0x30 && (GFX.r2130 & 2) && (GFX.r212d & (1 << N)) && !(PPU.BG_Forced & (1 << (N))))





#define ANYTHING_ON_SUB ((GFX.r2130 & 0x30) != 0x30 && (GFX.r2130 & 2) && (GFX.r212d & 0x1f))




#define ADD_OR_SUB_ON_ANYTHING (GFX.r2131 & 0x3f)


#define FIX_INTERLACE(SCREEN,DO_DEPTH,DEPTH) if (IPPU.DoubleHeightPixels && ((PPU.BGMode != 5 && PPU.BGMode != 6) || !IPPU.Interlace)) for (uint32 y = GFX.StartY; y <= GFX.EndY; y++) { memmove (SCREEN + (y * 2 + 1) * GFX.Pitch2, SCREEN + y * 2 * GFX.Pitch2, GFX.Pitch2); if(DO_DEPTH){ memmove (DEPTH + (y * 2 + 1) * (GFX.PPLx2>>1), DEPTH + y * GFX.PPL, GFX.PPLx2>>1); } }
#define BLACK BUILD_PIXEL(0,0,0)

void DrawTile (uint32 Tile, uint32 Offset, uint32 StartLine,
               uint32 LineCount);
void DrawClippedTile (uint32 Tile, uint32 Offset,
                      uint32 StartPixel, uint32 Width,
                      uint32 StartLine, uint32 LineCount);
void DrawTilex2 (uint32 Tile, uint32 Offset, uint32 StartLine,
                 uint32 LineCount);
void DrawClippedTilex2 (uint32 Tile, uint32 Offset,
                        uint32 StartPixel, uint32 Width,
                        uint32 StartLine, uint32 LineCount);
void DrawTilex2x2 (uint32 Tile, uint32 Offset, uint32 StartLine,
               uint32 LineCount);
void DrawClippedTilex2x2 (uint32 Tile, uint32 Offset,
                          uint32 StartPixel, uint32 Width,
                          uint32 StartLine, uint32 LineCount);
void DrawLargePixel (uint32 Tile, uint32 Offset,
                     uint32 StartPixel, uint32 Pixels,
                     uint32 StartLine, uint32 LineCount);

void DrawTile16 (uint32 Tile, uint32 Offset, uint32 StartLine,
                 uint32 LineCount);
void DrawClippedTile16 (uint32 Tile, uint32 Offset,
                        uint32 StartPixel, uint32 Width,
                        uint32 StartLine, uint32 LineCount);
void DrawTile16x2 (uint32 Tile, uint32 Offset, uint32 StartLine,
                   uint32 LineCount);
void DrawClippedTile16x2 (uint32 Tile, uint32 Offset,
                          uint32 StartPixel, uint32 Width,
                          uint32 StartLine, uint32 LineCount);
void DrawTile16x2x2 (uint32 Tile, uint32 Offset, uint32 StartLine,
                     uint32 LineCount);
void DrawClippedTile16x2x2 (uint32 Tile, uint32 Offset,
                            uint32 StartPixel, uint32 Width,
                            uint32 StartLine, uint32 LineCount);
void DrawLargePixel16 (uint32 Tile, uint32 Offset,
                       uint32 StartPixel, uint32 Pixels,
                       uint32 StartLine, uint32 LineCount);

void DrawTile16Add (uint32 Tile, uint32 Offset, uint32 StartLine,
                    uint32 LineCount);

void DrawClippedTile16Add (uint32 Tile, uint32 Offset,
                           uint32 StartPixel, uint32 Width,
                           uint32 StartLine, uint32 LineCount);

void DrawTile16Add1_2 (uint32 Tile, uint32 Offset, uint32 StartLine,
                       uint32 LineCount);

void DrawClippedTile16Add1_2 (uint32 Tile, uint32 Offset,
                              uint32 StartPixel, uint32 Width,
                              uint32 StartLine, uint32 LineCount);

void DrawTile16FixedAdd1_2 (uint32 Tile, uint32 Offset, uint32 StartLine,
                            uint32 LineCount);

void DrawClippedTile16FixedAdd1_2 (uint32 Tile, uint32 Offset,
                                   uint32 StartPixel, uint32 Width,
                                   uint32 StartLine, uint32 LineCount);

void DrawTile16Sub (uint32 Tile, uint32 Offset, uint32 StartLine,
                    uint32 LineCount);

void DrawClippedTile16Sub (uint32 Tile, uint32 Offset,
                           uint32 StartPixel, uint32 Width,
                           uint32 StartLine, uint32 LineCount);

void DrawTile16Sub1_2 (uint32 Tile, uint32 Offset, uint32 StartLine,
                       uint32 LineCount);

void DrawClippedTile16Sub1_2 (uint32 Tile, uint32 Offset,
                              uint32 StartPixel, uint32 Width,
                              uint32 StartLine, uint32 LineCount);

void DrawTile16FixedSub1_2 (uint32 Tile, uint32 Offset, uint32 StartLine,
                            uint32 LineCount);

void DrawClippedTile16FixedSub1_2 (uint32 Tile, uint32 Offset,
                                   uint32 StartPixel, uint32 Width,
                                   uint32 StartLine, uint32 LineCount);

void DrawLargePixel16Add (uint32 Tile, uint32 Offset,
                          uint32 StartPixel, uint32 Pixels,
                          uint32 StartLine, uint32 LineCount);

void DrawLargePixel16Add1_2 (uint32 Tile, uint32 Offset,
                             uint32 StartPixel, uint32 Pixels,
                             uint32 StartLine, uint32 LineCount);

void DrawLargePixel16Sub (uint32 Tile, uint32 Offset,
                          uint32 StartPixel, uint32 Pixels,
                          uint32 StartLine, uint32 LineCount);

void DrawLargePixel16Sub1_2 (uint32 Tile, uint32 Offset,
                             uint32 StartPixel, uint32 Pixels,
                             uint32 StartLine, uint32 LineCount);


#endif
