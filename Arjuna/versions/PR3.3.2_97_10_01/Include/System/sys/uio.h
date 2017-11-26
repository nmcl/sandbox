/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: uio.h,v 1.7.6.1 1996/10/10 12:26:08 ngdp Exp $
 */

#ifndef SYS_UIO_H_
#define SYS_UIO_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef WIN32

#include <sys/uio.h>

#if defined(NEED_GCC_PROTOTYPES) || defined (_AIX) || defined(__ultrix)
extern "C"
{
    int readv(int, struct iovec *, int);
    int writev(int, const struct iovec *, int);
}
#endif

#else

#include <winsock.h>

struct iovec
{
    caddr_t iov_base;
    int	    iov_len;
};

#endif

#ifdef __LINUX__
#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN 16
#endif
#endif

#endif
