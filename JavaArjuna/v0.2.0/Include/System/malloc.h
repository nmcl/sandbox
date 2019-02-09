/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: malloc.h,v 1.5 1995/08/15 14:13:35 nmcl Exp $
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
