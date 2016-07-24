/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: protos.h,v 1.3 1993/11/30 14:47:50 nmcl Exp $
 */

#ifndef PROTOS_H_
#define PROTOS_H_

/*
 * Prototype definitions for those routines that are not
 * already prototyped in 'standard' header files. This is likely 
 * to be the most machine dependent part of the system
 */

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef SYS_TYPES_H_
#  include <System\sys\types.h>
#endif

#ifdef _AIX
#  ifndef SYS_UIO_H_
#    include <sys\uio.h>
#  endif
#endif


extern "C" {
    int fchmod (int, mode_t);
    void closelog ();
    int fsync (int);
#if !defined(OS2) && !defined(MSDOS)
    int gethostname (char *, size_t);
#endif
#ifdef sun
    int ftruncate (int, off_t);
#endif

#ifdef SYSV
    int kill(pid_t, int);
#endif

    int rename (const char *, const char *);

#ifndef _AIX
    void syslog (int, const char* ...);
#endif

#if defined(OS2) || defined(MSDOS)
    unsigned int usleep (unsigned int);
#endif

#ifdef sun
#ifdef __GNUG__
    unsigned int usleep (unsigned int);
#endif
#endif

#if !defined(OS2) && !defined(MSDOS)
    pid_t vfork();
#endif

#ifndef hpux
    long sysconf(int);
#endif

#ifdef _AIX
    int writev (int, struct iovec *, int);
    int readv  (int, struct iovec *, int);
#endif
}

#endif
