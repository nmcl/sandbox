/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: uio.h,v 1.7 1995/08/16 13:16:43 ngdp Exp $
 */

#ifndef SYS_UIO_H_
#define SYS_UIO_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <sys/uio.h>

#if defined(NEED_GCC_PROTOTYPES) || defined(_AIX) || defined(__ultrix)
extern "C"
{
    int readv(int, struct iovec *, int);
    int writev(int, const struct iovec *, int);
}
#endif

#ifdef __LINUX__
#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN 16
#endif

#endif

#endif
