/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: uio.h,v 1.9 1996/04/16 09:28:19 nmcl Exp $
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
