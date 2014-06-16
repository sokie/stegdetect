/*
 * file.h - definitions for file(1) program
 * @(#)$Id: file.h,v 1.6 2001/12/20 23:12:13 provos Exp $
 *
 * Copyright (c) Ian F. Darwin, 1987.
 * Written by Ian F. Darwin.
 *
 * This software is not subject to any license of the American Telephone
 * and Telegraph Company or of the Regents of the University of California.
 *
 * Permission is granted to anyone to use this software for any purpose on
 * any computer system, and to alter it and redistribute it freely, subject
 * to the following restrictions:
 *
 * 1. The author is not responsible for the consequences of use of this
 *    software, no matter how awful, even if they arise from flaws in it.
 *
 * 2. The origin of this software must not be misrepresented, either by
 *    explicit claim or by omission.  Since few users ever read sources,
 *    credits must appear in the documentation.
 *
 * 3. Altered versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.  Since few users
 *    ever read sources, credits must appear in the documentation.
 *
 * 4. This notice may not be removed or altered.
 */

#ifndef __file_h__
#define __file_h__

extern int   apprentice_memory(unsigned char *buf, unsigned int size);
extern int   file_init(void);
extern int   file_process(unsigned char *, int);

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#if defined(PACKAGE)

typedef int int32;
typedef unsigned int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef char int8;
typedef unsigned char uint8;

#ifndef HOWMANY
# define HOWMANY 16384		/* how much of the file to look at */
#endif
#define MAXMAGIS 1000		/* max entries in /etc/magic */
#define MAXDESC	50		/* max leng of text description */
#define MAXstring 32		/* max leng of "string" types */

#define MAGICNO		0xF11E041C
#define VERSIONNO	1

#define CHECK	1
#define COMPILE	2

struct magic {
	uint16 cont_level;/* level of ">" */
	uint8 nospflag;	/* supress space character */
	uint8 flag;
#define INDIR	1		/* if '>(...)' appears,  */
#define	UNSIGNED 2		/* comparison is unsigned */
#define OFFADD	4		/* if '>&' appears,  */
	uint8 reln;		/* relation (0=eq, '>'=gt, etc) */
	uint8 vallen;		/* length of string value, if any */
	uint8 type;		/* int, short, long or string. */
	uint8 in_type;		/* type of indirrection */
#define 			BYTE	1
#define				SHORT	2
#define				LONG	4
#define				STRING	5
#define				DATE	6
#define				BESHORT	7
#define				BELONG	8
#define				BEDATE	9
#define				LESHORT	10
#define				LELONG	11
#define				LEDATE	12
#define				PSTRING	13
#define				LDATE	14
#define				BELDATE	15
#define				LELDATE	16
	uint8 in_op;		/* operator for indirection */
	uint8 mask_op;		/* operator for mask */
#define				OPAND	1
#define				OPOR	2
#define				OPXOR	3
#define				OPADD	4
#define				OPMINUS	5
#define				OPMULTIPLY	6
#define				OPDIVIDE	7
#define				OPMODULO	8
#define				OPINVERSE	0x80
	int32 offset;		/* offset to magic number */
	int32 in_offset;	/* offset from indirection */
	union VALUETYPE {
		unsigned char b;
		unsigned short h;
		uint32 l;
		char s[MAXstring];
		unsigned char hs[2];	/* 2 bytes of a fixed-endian "short" */
		unsigned char hl[4];	/* 4 bytes of a fixed-endian "long" */
	} value;		/* either number or string */
	uint32 mask;	/* mask before comparison with value */
	char desc[MAXDESC];	/* description */
};

#define BIT(A)   (1 << (A))
#define STRING_IGNORE_LOWERCASE		BIT(0)
#define STRING_COMPACT_BLANK		BIT(1)
#define STRING_COMPACT_OPTIONAL_BLANK	BIT(2)
#define CHAR_IGNORE_LOWERCASE		'c'
#define CHAR_COMPACT_BLANK		'B'
#define CHAR_COMPACT_OPTIONAL_BLANK	'b'


/* list of magic entries */
struct mlist {
	struct magic *magic;		/* array of magic entries */
	uint32 nmagic;			/* number of entries in array */
	struct mlist *next, *prev;
};

#include <stdio.h>	/* Include that here, to make sure __P gets defined */
#include <errno.h>

#ifndef __P
# if defined(__STDC__) || defined(__cplusplus)
#  define __P(a) a
# else
#  define __P(a) ()
#  define const
# endif
#endif

extern int   apprentice		__P((const char *, int));
extern int   ascmagic		__P((unsigned char *, int));
extern void  error		__P((const char *, ...));
extern void  ckfputs		__P((const char *, FILE *));
struct stat;
extern int   fsmagic		__P((const char *, struct stat *));
extern char *fmttime		__P((long, int));
extern int   is_compress	__P((const unsigned char *, int *));
extern int   is_tar		__P((unsigned char *, int));
extern void  magwarn		__P((const char *, ...));
extern void  mdump		__P((struct magic *));
extern void  showstr		__P((FILE *, const char *, int));
extern int   softmagic		__P((unsigned char *, int));
extern int   tryit		__P((unsigned char *, int, int));
extern int   zmagic		__P((unsigned char *, int));
extern void  ckfprintf		__P((FILE *, const char *, ...));
extern uint32 signextend	__P((struct magic *, unsigned int32));
extern void tryelf		__P((int, unsigned char *, int));

extern char *progname;		/* the program name 			*/
extern const char *magicfile;	/* name of the magic file		*/
extern int lineno;		/* current line number in magic file	*/

extern struct mlist mlist;	/* list of arrays of magic entries	*/

extern int noprint;
extern int debug;		/* enable debugging?			*/
extern int zflag;		/* process compressed files?		*/
extern int lflag;		/* follow symbolic links?		*/
extern int sflag;		/* read/analyze block special files?	*/
extern int iflag;		/* Output types as mime-types		*/

extern char *magic_mem;

extern int optind;		/* From getopt(3)			*/
extern char *optarg;

#ifndef HAVE_STRERROR
extern int sys_nerr;
extern char *sys_errlist[];
#define strerror(e) \
	(((e) >= 0 && (e) < sys_nerr) ? sys_errlist[(e)] : "Unknown error")
#endif

#ifndef HAVE_STRTOUL
#define strtoul(a, b, c)	strtol(a, b, c)
#endif

#if defined(HAVE_MMAP) && defined(HAVE_SYS_MMAN_H) && !defined(QUICK)
#define QUICK
#endif

#ifdef __STDC__
#define FILE_RCSID(id) \
static const char *rcsid(const char *p) { \
	return rcsid(p = id); \
}
#else
#define FILE_RCSID(id) static char rcsid[] = id;
#endif

#endif /* defined(PACKAGE) */
#endif /* __file_h__ */
