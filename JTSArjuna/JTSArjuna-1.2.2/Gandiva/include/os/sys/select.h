/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: select.h,v 1.4 1996/03/28 15:57:16 nmcl Exp $
 */

#ifndef OS_SYS_SELECT_H_
#define OS_SYS_SELECT_H_

#ifdef HAVE_SELECT_H_
#  include <sys/select.h>
#endif

extern "C"
{
#if defined(NEED_SELECT_PROTO) || defined(NEED_GCC_PROTOTYPES)
    int select(int, fd_set *, fd_set*, fd_set*, struct timeval*);
#endif
}
#endif
