/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: types.h,v 1.1 1997/06/09 19:52:02 nmcl Exp $
 */

#ifndef SYS_TYPES_H_
#define SYS_TYPES_H_ 

#ifdef STUB
#  pragma @NoRename
#endif

/*
 * InterViews & sys/types.h often clash avoid include if it looks as
 * if the file has already been included
 */

#if defined(os_types) || defined(os_boolean) || defined(iv_os_boolean)
#  include <OS/types.h>
#else
#  include <sys/types.h>
#endif

#ifdef WIN32
typedef char* caddr_t;
typedef int pid_t;
#endif

#endif
