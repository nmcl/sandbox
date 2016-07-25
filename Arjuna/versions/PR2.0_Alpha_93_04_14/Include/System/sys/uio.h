/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: uio.h,v 1.5 1993/05/05 13:40:49 ngdp Exp $
 */

#ifndef SYS_UIO_H_
#define SYS_UIO_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifdef __GNUG__
extern "C"
{
    int readv(int, struct iovec *, int);
    int writev(int, const struct iovec *, int);
}
#endif

#include <sys/uio.h>

#endif
