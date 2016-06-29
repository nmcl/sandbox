/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: protos.h,v 1.1 1997/06/09 19:51:59 nmcl Exp $
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
#  include <System/sys/types.h>
#endif

#ifdef _AIX
#  ifndef SYS_UIO_H_
#    include <sys/uio.h>    
#  endif
#endif


extern "C" {
#ifndef WIN32
    int fchmod (int, mode_t);
#endif
    void closelog ();
    int fsync (int);

#if defined(NEED_GCC_PROTOTYPES) || defined(__ultrix)
    void bzero (void *, int);
#endif

#if defined(sun) || defined(__ultrix)
    int gethostname (char *, int);
    int ftruncate (int, off_t);
#endif

#ifdef SYSV
    int kill(pid_t, int);
#endif

    int rename (const char *, const char *);

#ifndef _AIX
    void syslog (int, const char* ...);
#endif

#ifndef WIN32
    pid_t vfork();
#endif

#ifndef hpux
    long sysconf(int);
#endif
}

#endif
