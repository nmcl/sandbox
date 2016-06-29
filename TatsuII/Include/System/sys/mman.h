/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: mman.h,v 1.1 1997/06/09 19:52:01 nmcl Exp $
 */

#ifndef SYS_MMAN_H_
#define SYS_MMAN_H_

#ifdef STUB
#  pragma @NoRename
#endif

#if defined(NEED_GCC_PROTOTYPES)
#define mmap __mmap
#endif

#ifdef HAVE_MMAP
#  ifdef hpux
extern "C" {
#  endif
#  include <sys/mman.h>
#  ifdef hpux
}
#  endif
#endif

#ifndef MAP_FAILED
#  define MAP_FAILED ((void *) -1)
#endif

#ifndef MS_SYNC
#  define MS_SYNC 0
#endif

#if defined(NEED_GCC_PROTOTYPES)
#undef mmap

extern "C" {
    caddr_t mmap(caddr_t, size_t, int, int, int, off_t);
    int munmap(caddr_t, size_t);
    int msync(caddr_t, size_t, int);
    int mprotect(caddr_t, int, int);
}

#endif
#endif
