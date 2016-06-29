/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: malloc.h,v 1.1 1997/06/09 19:51:59 nmcl Exp $
 */

#ifndef MALLOC_H_
#define MALLOC_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef __ultrix
#  include <malloc.h>
#endif

#if defined(NEED_MALLOC_T)
typedef void* malloc_t;
#endif

#endif
