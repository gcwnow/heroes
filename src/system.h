/*------------------------------------------------------------------------.
| Copyright 2000, 2001  Alexandre Duret-Lutz <duret_g@epita.fr>           |
|                                                                         |
| This file is part of Heroes.                                            |
|                                                                         |
| Heroes is free software; you can redistribute it and/or modify it under |
| the terms of the GNU General Public License version 2 as published by   |
| the Free Software Foundation.                                           |
|                                                                         |
| Heroes is distributed in the hope that it will be useful, but WITHOUT   |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   |
| for more details.                                                       |
|                                                                         |
| You should have received a copy of the GNU General Public License along |
| with this program; if not, write to the Free Software Foundation, Inc., |
| 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   |
`------------------------------------------------------------------------*/

/*
 * Every .c file SHOULD include this file as the FIRST.
 *
 * It defines macros needed for portability, and includes system
 * headers that may be needed by some parts of the game.  This means
 * that all .c files will therefore include all these system headers,
 * even if they don't need it; but since most of these headers can
 * have different names or may not be needed at all on some systems,
 * its safer if the selection is done in one unique place.
 *
 * Since this file is always included first, local .h files can assume
 * that this file has already been included.
 *
 * Do NOT include local headers (except config.h), only system
 * headers are included here.
 */

#ifndef HEROES__SYSTEM__H
#define HEROES__SYSTEM__H

#include "config.h"

#include <stdio.h>
#include <sys/types.h>

#if STDC_HEADERS
# include <stdlib.h>
#endif

#include <ctype.h>
#include <sys/stat.h>
#if HAVE_FCNTL_H
# include <fcntl.h>
#else
# include <sys/file.h>
#endif
#include <assert.h>
#include <math.h>
#include <errno.h>

#if HAVE_STRING_H
# if HAVE_MEMORY_H && ! STDC_HEADERS
#  include <memory.h>
# endif
# include <string.h>
#else
# if HAVE_STRINGS_H
#  include <strings.h>
# endif
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if HAVE_DIRENT_H
# include <dirent.h>
#else
# define dirent direct
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#if HAVE_DIRECT_H
# include <direct.h>
#endif

#if HAVE_IO_H
# include <io.h>
#endif

#if HAVE_VPRINTF || HAVE_DOPRNT
# if STDC_HEADERS
#  include <stdarg.h>
#  define VA_START(args, lastarg) va_start(args, lastarg)
# else
#  include <varargs.h>
#  define VA_START(args, lastarg) va_start(args)
# endif
#else
# define va_alist a1, a2, a3, a4, a5, a6, a7, a8
# define va_dcl char *a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8;
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#if HAVE_MMAP
# include <sys/mman.h>
#endif

#if HAVE_GETOPT_H && HAVE_GETOPT_LONG
# include <getopt.h>
#else
/* Getopt is not available on this system so we use the embedded
   getopt.h header.  However, this header uses variables named __argc
   and __argv which are the name of macros defined in MinGW's
   "stdlib.h" and used in many other MinGW headers.

   We could modify getopt.h, but this files comes from the GNU libc
   and changing it locally would make updates harder.  So instead we
   just disable __argc and __argv for the time getopt.h is included.
   Puke, puke, puke.  */
# ifdef __argc
#   define HEROES__saved__argc __argc
#   undef __argc
# endif
# ifdef __argv
#   define HEROES__saved__argv __argv
#   undef __argv
# endif
# include "getopt.h"
# ifdef HEROES__saved__argc
#   define __argc HEROES__saved__argc
#   undef HEROES__saved__argc
# endif
# ifdef HEROES__saved__argv
#   define __argv HEROES__saved__argv
#   undef HEROES__saved__argv
# endif
#endif

/* Take care of NLS matters.  */

#if HAVE_LOCALE_H
# include <locale.h>
#endif
#if !HAVE_SETLOCALE
# define setlocale(Category, Locale) /* empty */
#endif

#if ENABLE_NLS
# include <libintl.h>
# define _(Text) gettext (Text)
#else
# undef bindtextdomain
# define bindtextdomain(Domain, Directory) /* empty */
# undef textdomain
# define textdomain(Domain) /* empty */
# define _(Text) Text
# define ngettext(msg1, msg2, n) ((n) == 1 ? (msg1) : (msg2))
#endif
#define N_(Text) Text

#if HAVE_WINDOWS_H
# include <windows.h>
# if ! HAVE_READDIR
   /* should be included after io.h */
#  include "w_dirent.h"
# endif
# if ! HAVE_SLEEP
#  define sleep(x) (Sleep ((x) * 1000))
# endif
#endif

/* boolean type */

#if HAVE_STDBOOL_H
# include <stdbool.h>
#else
typedef enum {false = 0, true = 1} bool;
#endif

#ifndef M_PI
# define M_PI	3.14159265358979323846  /* pi */
#endif
#ifndef M_PI_2
# define M_PI_2	1.57079632679489661923  /* pi/2 */
#endif
#ifndef M_PI_4
# define M_PI_4 0.78539816339744830962  /* pi/4 */
#endif

/* common integer sizes */

#if HAVE_STDINT_H
# include <stdint.h>
typedef uint32_t	a_u32;
typedef uint16_t	a_u16;
typedef uint8_t		a_u8;
typedef int32_t		a_s32;
typedef int16_t		a_s16;
typedef int8_t		a_s8;
#define U16_MAX		UINT16_MAX
#define U32_MAX		UINT32_MAX
#else
typedef unsigned int		a_u32;
typedef unsigned short int	a_u16;
typedef unsigned char		a_u8;
typedef signed int		a_s32;
typedef signed short int	a_s16;
typedef signed char		a_s8;
#define U16_MAX		(0xffffU)
#define U32_MAX		(0xffffffffU)
#endif

#if ! HAVE_SSIZE_T
typedef int ssize_t;
#endif

typedef a_u32 a_keycode; /* FIXME: move to media/ */

#define UCHAR(c) ((unsigned char) (c))

/* xalloc.h (from fileutils.h) define __attribute__ only for gcc >= 2.8
   (according to the ChangeLog this is needed for OPENStep 4.2 cc)
   while we want to allow __attribute__ for gcc >= 2.7 in Heroes
   (because 2.7.2 is still widely used and __attribute__((packed)) is
   mandatory).  So we need to undefine __attribute__ after xalloc has been
   included. */
#ifndef __attribute__
/* remember that __attribute has to be undefined after inclusion */
# define undefine__attribute__
#endif
#include "xalloc.h"
#ifdef undefine__attribute__
# undef __attribute__
# undef undefine__attribute__
#endif

/* Like XFREE, but also zeroes Var */
#define XFREE0(Var)				\
  do {						\
    if (Var) {					\
      free (Var);				\
      (Var) = 0;				\
    }						\
  } while (0)

#define XCALLOC_ARRAY(Array, N_items) \
Array = xcalloc (sizeof (*(Array)), (N_items))

#define XMALLOC_ARRAY(Array, N_items) \
Array = xmalloc (sizeof (*(Array)) * (N_items))

#define XREALLOC_ARRAY(Array, N_items) \
Array = xrealloc ((Array), sizeof (*(Array)) * (N_items))

#define XMALLOC_VAR(Var) XMALLOC_ARRAY ((Var), 1)
#define XCALLOC_VAR(Var) XCALLOC_ARRAY ((Var), 1)

/* Like XMALLOC_ARRAY but also performs a memSet */
#define XSALLOC_ARRAY(Array, N_items, Val)			\
  do {								\
    XMALLOC_ARRAY ((Array), (N_items));				\
    memset ((Array), (Val), sizeof (*(Array)) * (N_items));	\
  } while (0)

#if HAVE_MTRACE
# include <mcheck.h>
#else
# define mtrace()
# define muntrace()
#endif

/* keep this header at the end of the include list, because it may
   define some macros to change the declaration of malloc functions */
#if HAVE_DMALLOC_H
# define DMALLOC_FUNC_CHECK
# include <dmalloc.h>
#endif
#if HAVE_MPATROL_H
# include <mpatrol.h>
#endif

/* miscellaneous prototypes for replacement functions */

#if ! HAVE_DECL_STRCASECMP
int strcasecmp (const char *s1, const char *s2);
#endif

#if ! HAVE_DECL_STPCPY
# ifndef stpcpy
char *stpcpy (const char *s1, const char *s2);
# endif
#endif

#if HAVE_MKDIR
# if MKDIR_TAKES_ONE_ARG
#  define mkdir(a,b) mkdir(a)
# endif
#else
# if HAVE__MKDIR
#  define mkdir(a,b) _mkdir(a)
# else
#  error "Don't know how to create a directory on this system."
# endif
#endif

/* For systems that distinguish between text and binary I/O.
   O_BINARY is usually declared in fcntl.h  */
#if !defined O_BINARY && defined _O_BINARY
  /* For MSC-compatible compilers.  */
# define O_BINARY _O_BINARY
# define O_TEXT _O_TEXT
#endif

#ifdef __BEOS__
  /* BeOS 5 has O_BINARY and O_TEXT, but they have no effect.  */
# undef O_BINARY
# undef O_TEXT
#endif

#ifndef O_BINARY
  /* On reasonable systems, binary I/O is the default.  */
# define O_BINARY 0
# define O_TEXT 0
#endif

/* Define S_ISDIR if it isn't already defined in sys/stat.h */
#if defined(S_IFDIR) && !defined(S_ISDIR)
# define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif

/* Re-define S_ISDIR if the native implementation is broken. According to
   the autoconf info-file this is the case for Tektronix UTekV, Amdahl UTS
   and Motorola System V/88 */
#if defined(STAT_MACROS_BROKEN)
# undef S_ISDIR
# define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif


#if !S_IRUSR && S_IREAD
# define S_IRUSR S_IREAD
#endif
#if !S_IRUSR
# define S_IRUSR 00400
#endif
#if !S_IRGRP
# define S_IRGRP (S_IRUSR >> 3)
#endif
#if !S_IROTH
# define S_IROTH (S_IRUSR >> 6)
#endif

#if !S_IWUSR && S_IWRITE
# define S_IWUSR S_IWRITE
#endif
#if !S_IWUSR
# define S_IWUSR 00200
#endif
#if !S_IWGRP
# define S_IWGRP (S_IWUSR >> 3)
#endif
#if !S_IWOTH
# define S_IWOTH (S_IWUSR >> 6)
#endif

#if !S_IXUSR && S_IEXEC
# define S_IXUSR S_IEXEC
#endif
#if !S_IXUSR
# define S_IXUSR 00100
#endif
#if !S_IXGRP
# define S_IXGRP (S_IXUSR >> 3)
#endif
#if !S_IXOTH
# define S_IXOTH (S_IXUSR >> 6)
#endif

#if !S_IRWXU
# define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#endif
#if !S_IRWXG
# define S_IRWXG (S_IRGRP | S_IWGRP | S_IXGRP)
#endif
#if !S_IRWXO
# define S_IRWXO (S_IROTH | S_IWOTH | S_IXOTH)
#endif

/* S_IXUGO is a common extension to POSIX.1.  */
#if !S_IXUGO
# define S_IXUGO (S_IXUSR | S_IXGRP | S_IXOTH)
#endif

#ifndef S_IRWXUGO
# define S_IRWXUGO (S_IRWXU | S_IRWXG | S_IRWXO)
#endif

#ifndef S_IRUGO
# define S_IRUGO (S_IRUSR | S_IRGRP | S_IROTH)
#endif

#ifndef S_IWUGO
# define S_IWUGO (S_IWUSR | S_IWGRP | S_IWOTH)
#endif

#ifndef S_IRWUGO
# define S_IRWUGO (S_IRUGO | S_IWUGO)
#endif

/* ===== The following ctype definition are stolen from textutils ===== */

/* Jim Meyering writes:

   "... Some ctype macros are valid only for character codes that
   isascii says are ASCII (SGI's IRIX-4.0.5 is one such system --when
   using /bin/cc or gcc but without giving an ansi option).  So, all
   ctype uses should be through macros like ISPRINT...  If
   STDC_HEADERS is defined, then autoconf has verified that the ctype
   macros don't need to be guarded with references to isascii. ...
   Defining isascii to 1 should let any compiler worth its salt
   eliminate the && through constant folding."

   Bruno Haible adds:

   "... Furthermore, isupper(c) etc. have an undefined result if c is
   outside the range -1 <= c <= 255. One is tempted to write isupper(c)
   with c being of type `char', but this is wrong if c is an 8-bit
   character >= 128 which gets sign-extended to a negative value.
   The macro ISUPPER protects against this as well."  */

#if STDC_HEADERS || (!defined (isascii) && !HAVE_ISASCII)
# define IN_CTYPE_DOMAIN(c) 1
#else
# define IN_CTYPE_DOMAIN(c) isascii(c)
#endif

#ifdef isblank
# define ISBLANK(c) (IN_CTYPE_DOMAIN (c) && isblank (c))
#else
# define ISBLANK(c) ((c) == ' ' || (c) == '\t')
#endif
#ifdef isgraph
# define ISGRAPH(c) (IN_CTYPE_DOMAIN (c) && isgraph (c))
#else
# define ISGRAPH(c) (IN_CTYPE_DOMAIN (c) && isprint (c) && !isspace (c))
#endif

/* This is defined in <sys/euc.h> on at least Solaris2.6 systems.  */
#undef ISPRINT

#define ISPRINT(c) (IN_CTYPE_DOMAIN (c) && isprint (c))
#define ISALNUM(c) (IN_CTYPE_DOMAIN (c) && isalnum (c))
#define ISALPHA(c) (IN_CTYPE_DOMAIN (c) && isalpha (c))
#define ISCNTRL(c) (IN_CTYPE_DOMAIN (c) && iscntrl (c))
#define ISLOWER(c) (IN_CTYPE_DOMAIN (c) && islower (c))
#define ISPUNCT(c) (IN_CTYPE_DOMAIN (c) && ispunct (c))
#define ISSPACE(c) (IN_CTYPE_DOMAIN (c) && isspace (c))
#define ISUPPER(c) (IN_CTYPE_DOMAIN (c) && isupper (c))
#define ISXDIGIT(c) (IN_CTYPE_DOMAIN (c) && isxdigit (c))
#define ISDIGIT_LOCALE(c) (IN_CTYPE_DOMAIN (c) && isdigit (c))

#if STDC_HEADERS
# define TOLOWER(Ch) tolower (Ch)
# define TOUPPER(Ch) toupper (Ch)
#else
# define TOLOWER(Ch) (ISUPPER (Ch) ? tolower (Ch) : (Ch))
# define TOUPPER(Ch) (ISLOWER (Ch) ? toupper (Ch) : (Ch))
#endif

/* ISDIGIT differs from ISDIGIT_LOCALE, as follows:
   - Its arg may be any int or unsigned int; it need not be an unsigned char.
   - It's guaranteed to evaluate its argument exactly once.
   - It's typically faster.
   Posix 1003.2-1992 section 2.5.2.1 page 50 lines 1556-1558 says that
   only '0' through '9' are digits.  Prefer ISDIGIT to ISDIGIT_LOCALE unless
   it's important to use the locale's definition of `digit' even when the
   host does not conform to Posix.  */
#define ISDIGIT(c) ((unsigned) (c) - '0' <= 9)

/* ===== end of ctype definitions ===== */

#if ! HAVE_SETGID
# if HAVE_SETREGID
#  define setuid(X) setreuid(X,X)
#  define setgid(X) setregid(X,X)
# else
#  define setuid(X)
#  define setgid(X)
# endif
#endif

#if ! HAVE_SETEGID
# if HAVE_SETREGID
#  define seteuid(X) setreuid(-1,X)
#  define setegid(X) setregid(-1,X)
# else
#  define seteuid(x)
#  define setegid(x)
# endif
#endif

#if ! HAVE_SETREGID
# define setreuid(x,y)
# define setregid(x,y)
#endif

#if ! HAVE_GETEGID
# define geteuid() 0
# define getuid()  0
# define getegid() 0
# define getgid()  0
#endif


#ifndef __attribute__
# if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7) || __STRICT_ANSI__
#  define __attribute__(x)
# endif
#endif

#ifndef ATTRIBUTE_NORETURN
# define ATTRIBUTE_NORETURN __attribute__ ((noreturn))
#endif

#ifndef ATTRIBUTE_UNUSED
# define ATTRIBUTE_UNUSED __attribute__ ((unused))
#endif

#ifndef ATTRIBUTE_PACKED
# define ATTRIBUTE_PACKED __attribute__ ((packed))
#endif

#ifndef ATTRIBUTE_PRINTF
# define ATTRIBUTE_PRINTF(str,fst) __attribute__ ((format (printf, str, fst)))
#endif


#ifndef PTR_TO_INT
# define PTR_TO_INT(P) (((char *) P) - (char *) 0)
#endif
#ifndef INT_TO_PTR
# define INT_TO_PTR(P, TYPE) ((P) + ((TYPE) *) 0)
#endif

#endif /* HEROES__SYSTEM__H */
