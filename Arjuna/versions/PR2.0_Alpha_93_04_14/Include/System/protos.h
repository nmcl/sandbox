/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: protos.h,v 1.10 1993/04/27 14:45:05 ngdp Exp $
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

extern "C" {
    int fchmod (int, mode_t);
    void closelog ();
    int  fsync (int);
#ifndef hpux
    int  gethostname (char *, int);
#else
    int  gethostname (char *, size_t);
#endif

#ifdef SYSV
    int kill(pid_t, int);
#endif

    int  rename (const char *, const char *);
    void syslog (int, const char* ...);

    pid_t vfork();

#ifndef hpux
    long sysconf(int);
#endif
}

#endif
