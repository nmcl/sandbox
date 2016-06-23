/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: select.h,v 1.1 1997/09/25 15:27:39 nmcl Exp $
 */

#ifndef SYS_SELECT_H_
#define SYS_SELECT_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifdef HAVE_SELECT_H_
#  include <sys/select.h>
#endif

#ifdef NEED_FD_SET

/* Some systems (HP-RT for example) seem to hide FD_SET! */

#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
#endif

#ifndef NBBY            /* number of bits per byte */
#define NBBY 8
#endif

typedef long    fd_mask;
#define NFDBITS (sizeof (fd_mask) * NBBY)       /* bits per mask */
#ifndef howmany
#define howmany(x, y)   (((x)+((y)-1))/(y))
#endif

typedef struct fd_set {
        fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

#define FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= \
                            ((unsigned)1 << ((n) % NFDBITS)))

#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= \
                            ~((unsigned)1 << ((n) % NFDBITS)))

#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & \
                            ((unsigned)1 << ((n) % NFDBITS)))

#define FD_ZERO(p)      memset((char *)(p), 0, sizeof (*(p)))
#endif

extern "C"
{
#if defined(NEED_SELECT_PROTO) || defined(NEED_GCC_PROTOTYPES)
    int select(int, fd_set *, fd_set*, fd_set*, struct timeval*);
#endif
}
#endif
