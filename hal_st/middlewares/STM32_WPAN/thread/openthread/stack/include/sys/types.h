#if defined(CORE_CM0PLUS) || defined(STM32WBA55xx)

/*-
 * Copyright (c) 1982, 1986, 1991, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *  @(#)types.h 8.1 (Berkeley) 6/2/93
 */

#ifndef _TYPES_H_
#define _TYPES_H_

/* Machine type dependent parameters. */
//#include <machine/endian.h>

#ifndef _POSIX_SOURCE
typedef unsigned char u_char;
typedef unsigned short  u_short;
typedef unsigned int  u_int;
typedef unsigned long u_long;
typedef unsigned short  ushort;   /* Sys V compatibility */
typedef unsigned int  uint;   /* Sys V compatibility */
#endif

typedef unsigned long long u_quad_t;  /* quads */
typedef long long quad_t;
typedef quad_t *  qaddr_t;

typedef char *    caddr_t;  /* core address */
typedef long    daddr_t;  /* disk address */
typedef unsigned long dev_t;    /* device number */
typedef unsigned long fixpt_t;  /* fixed point number */
typedef unsigned long gid_t;    /* group id */
typedef unsigned long ino_t;    /* inode number */
typedef unsigned short  mode_t;   /* permissions */
typedef unsigned short  nlink_t;  /* link count */
typedef quad_t    off_t;    /* file offset */
typedef long    pid_t;    /* process id */
typedef long    segsz_t;  /* segment size */
typedef long    swblk_t;  /* swap offset */
typedef unsigned long uid_t;    /* user id */

/*
 * This belongs in unistd.h, but is placed here to ensure that programs
 * casting the second parameter of lseek to off_t will get the correct
 * version of lseek.
 */
//#ifndef KERNEL
//__BEGIN_DECLS
//off_t  lseek __P((int, off_t, int));
//__END_DECLS
//#endif

#ifndef _POSIX_SOURCE
#define major(x)  ((int)(((u_int)(x) >> 8)&0xff)) /* major number */
#define minor(x)  ((int)((x)&0xff))   /* minor number */
#define makedev(x,y)  ((dev_t)(((x)<<8) | (y))) /* create dev_t */
#endif

//#include <machine/ansi.h>
//#include <machine/types.h>

#ifdef  _BSD_CLOCK_T_
typedef _BSD_CLOCK_T_ clock_t;
#undef  _BSD_CLOCK_T_
#endif

#ifdef  _BSD_SIZE_T_
typedef _BSD_SIZE_T_  size_t;
#undef  _BSD_SIZE_T_
#endif

#ifdef  _BSD_SSIZE_T_
typedef _BSD_SSIZE_T_ ssize_t;
#undef  _BSD_SSIZE_T_
#endif

#ifdef  _BSD_TIME_T_
typedef _BSD_TIME_T_  time_t;
#undef  _BSD_TIME_T_
#endif

#ifndef _POSIX_SOURCE
#define NBBY  8   /* number of bits in a byte */

/*
 * Select uses bit masks of file descriptors in longs.  These macros
 * manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here should
 * be enough for most uses.
 */
#ifndef FD_SETSIZE
#define FD_SETSIZE  256
#endif

typedef long  fd_mask;
#define NFDBITS (sizeof(fd_mask) * NBBY)  /* bits per mask */

#ifndef howmany
#define howmany(x, y) (((x)+((y)-1))/(y))
#endif

typedef struct fd_set {
  fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

#define FD_SET(n, p)  ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define FD_CLR(n, p)  ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_COPY(f, t) bcopy(f, t, sizeof(*(f)))
#define FD_ZERO(p)  bzero(p, sizeof(*(p)))

#if defined(__STDC__) && defined(KERNEL)
/*
 * Forward structure declarations for function prototypes.  We include the
 * common structures that cross subsystem boundaries here; others are mostly
 * used in the same place that the structure is defined.
 */
struct  proc;
struct  pgrp;
struct  ucred;
struct  rusage;
struct  file;
struct  buf;
struct  tty;
struct  uio;
#endif

#endif /* !_POSIX_SOURCE */
#endif /* !_TYPES_H_ */


#else

/* unified sys/types.h: 
   start with sef's sysvi386 version.
   merge go32 version -- a few ifdefs.
   h8300hms, h8300xray, and sysvnecv70 disagree on the following types:

   typedef int gid_t;
   typedef int uid_t;
   typedef int dev_t;
   typedef int ino_t;
   typedef int mode_t;
   typedef int caddr_t;

   however, these aren't "reasonable" values, the sysvi386 ones make far 
   more sense, and should work sufficiently well (in particular, h8300 
   doesn't have a stat, and the necv70 doesn't matter.) -- eichin
 */

#ifndef _SYS_TYPES_H

#include <_ansi.h>
#include <sys/cdefs.h>
#include <machine/_types.h>

/* BSD types permitted by POSIX and always exposed as in Glibc.  Only provided
   for backward compatibility with BSD code.  The uintN_t standard types should
   be preferred in new code. */
#if ___int8_t_defined
typedef __uint8_t u_int8_t;
#endif
#if ___int16_t_defined
typedef __uint16_t  u_int16_t;
#endif 
#if ___int32_t_defined
typedef __uint32_t  u_int32_t;
#endif
#if ___int64_t_defined
typedef __uint64_t  u_int64_t;
#endif
typedef __intptr_t register_t;
#define __BIT_TYPES_DEFINED__ 1

#ifndef __need_inttypes

#define _SYS_TYPES_H
#include <sys/_types.h>
#include <sys/_stdint.h>

#if __BSD_VISIBLE
#include <machine/endian.h>
#include <sys/select.h>
#  define physadr   physadr_t
#  define quad    quad_t

#ifndef _IN_ADDR_T_DECLARED
typedef __uint32_t  in_addr_t;  /* base type for internet address */
#define _IN_ADDR_T_DECLARED
#endif

#ifndef _IN_PORT_T_DECLARED
typedef __uint16_t  in_port_t;
#define _IN_PORT_T_DECLARED
#endif

typedef __uintptr_t u_register_t;
#endif /* __BSD_VISIBLE */

#if __MISC_VISIBLE
#ifndef _BSDTYPES_DEFINED
/* also defined in mingw/gmon.h and in w32api/winsock[2].h */
#ifndef __u_char_defined
typedef unsigned char u_char;
#define __u_char_defined
#endif
#ifndef __u_short_defined
typedef unsigned short  u_short;
#define __u_short_defined
#endif
#ifndef __u_int_defined
typedef unsigned int  u_int;
#define __u_int_defined
#endif
#ifndef __u_long_defined
typedef unsigned long u_long;
#define __u_long_defined
#endif
#define _BSDTYPES_DEFINED
#endif
#endif /* __MISC_VISIBLE */

#if __MISC_VISIBLE
typedef unsigned short  ushort;   /* System V compatibility */
typedef unsigned int  uint;   /* System V compatibility */
typedef unsigned long ulong;    /* System V compatibility */
#endif

#ifndef _BLKCNT_T_DECLARED
typedef __blkcnt_t  blkcnt_t;
#define _BLKCNT_T_DECLARED
#endif

#ifndef _BLKSIZE_T_DECLARED
typedef __blksize_t blksize_t;
#define _BLKSIZE_T_DECLARED
#endif

#if !defined(__clock_t_defined) && !defined(_CLOCK_T_DECLARED)
typedef _CLOCK_T_ clock_t;
#define __clock_t_defined
#define _CLOCK_T_DECLARED
#endif

#if !defined(__time_t_defined) && !defined(_TIME_T_DECLARED)
typedef _TIME_T_  time_t;
#define __time_t_defined
#define _TIME_T_DECLARED
#endif

#ifndef __daddr_t_defined
typedef long  daddr_t;
#define __daddr_t_defined
#endif
#ifndef __caddr_t_defined
typedef char *  caddr_t;
#define __caddr_t_defined
#endif

#ifndef _FSBLKCNT_T_DECLARED    /* for statvfs() */
typedef __fsblkcnt_t  fsblkcnt_t;
typedef __fsfilcnt_t  fsfilcnt_t;
#define _FSBLKCNT_T_DECLARED
#endif

#ifndef _ID_T_DECLARED
typedef __id_t    id_t;   /* can hold a uid_t or pid_t */
#define _ID_T_DECLARED
#endif

#ifndef _INO_T_DECLARED
typedef __ino_t   ino_t;    /* inode number */
#define _INO_T_DECLARED
#endif

#if defined(__i386__) && (defined(GO32) || defined(__MSDOS__))
typedef char *    addr_t;
typedef unsigned long vm_offset_t;
typedef unsigned long vm_size_t;
#endif /* __i386__ && (GO32 || __MSDOS__) */

/*
 * All these should be machine specific - right now they are all broken.
 * However, for all of Cygnus' embedded targets, we want them to all be
 * the same.  Otherwise things like sizeof (struct stat) might depend on
 * how the file was compiled (e.g. -mint16 vs -mint32, etc.).
 */

#ifndef _OFF_T_DECLARED
typedef __off_t   off_t;    /* file offset */
#define _OFF_T_DECLARED
#endif
#ifndef _DEV_T_DECLARED
typedef __dev_t   dev_t;    /* device number or struct cdev */
#define _DEV_T_DECLARED
#endif
#ifndef _UID_T_DECLARED
typedef __uid_t   uid_t;    /* user id */
#define _UID_T_DECLARED
#endif
#ifndef _GID_T_DECLARED
typedef __gid_t   gid_t;    /* group id */
#define _GID_T_DECLARED
#endif

#ifndef _PID_T_DECLARED
typedef __pid_t   pid_t;    /* process id */
#define _PID_T_DECLARED
#endif

#ifndef _KEY_T_DECLARED
typedef __key_t   key_t;    /* IPC key */
#define _KEY_T_DECLARED
#endif

#ifndef _SSIZE_T_DECLARED
typedef _ssize_t ssize_t;
#define _SSIZE_T_DECLARED
#endif

#ifndef _MODE_T_DECLARED
typedef __mode_t  mode_t;   /* permissions */
#define _MODE_T_DECLARED
#endif

#ifndef _NLINK_T_DECLARED
typedef __nlink_t nlink_t;  /* link count */
#define _NLINK_T_DECLARED
#endif

#if !defined(__clockid_t_defined) && !defined(_CLOCKID_T_DECLARED)
typedef __clockid_t clockid_t;
#define __clockid_t_defined
#define _CLOCKID_T_DECLARED
#endif

#if !defined(__timer_t_defined) && !defined(_TIMER_T_DECLARED)
typedef __timer_t timer_t;
#define __timer_t_defined
#define _TIMER_T_DECLARED
#endif

#ifndef _USECONDS_T_DECLARED
typedef __useconds_t  useconds_t; /* microseconds (unsigned) */
#define _USECONDS_T_DECLARED
#endif

#ifndef _SUSECONDS_T_DECLARED
typedef __suseconds_t suseconds_t;
#define _SUSECONDS_T_DECLARED
#endif

typedef __int64_t sbintime_t;

#include <sys/features.h>
#include <sys/_pthreadtypes.h>
#include <machine/types.h>

#endif  /* !__need_inttypes */

#undef __need_inttypes

#endif  /* _SYS_TYPES_H */

#endif //CORE_CM0PLUS
