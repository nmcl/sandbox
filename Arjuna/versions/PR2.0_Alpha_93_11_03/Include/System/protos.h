/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: protos.h,v 1.1 1993/11/03 14:32:58 nmcl Exp $
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
    int fchmod (int, mode_t);
    void closelog ();
    int  fsync (int);
    int  gethostname (char *, size_t);

#ifdef SYSV
    int kill(pid_t, int);
#endif

    int  rename (const char *, const char *);

#ifndef _AIX
    void syslog (int, const char* ...);
#endif

#ifdef sun
#ifdef __GNUG__
    unsigned int usleep (unsigned int);
#endif
#endif

    pid_t vfork();

#ifndef hpux
    long sysconf(int);
#endif

#ifdef _AIX
    int writev (int, struct iovec *, int);
    int readv  (int, struct iovec *, int);
#endif
}

#endif
