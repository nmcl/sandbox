/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: uio.h,v 1.1 1993/11/03 14:34:48 nmcl Exp $
 */

#ifndef SYS_UIO_H_
#define SYS_UIO_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifdef __GNUG__
extern "C"
{
#ifndef __linux__
    int readv(int, struct iovec *, int);
    int writev(int, const struct iovec *, int);
#else
    int readv(int, const struct iovec *, unsigned int);
    int writev(int, const struct iovec *, unsigned int);
#endif
}
#endif

#include <sys/uio.h>

#endif
