/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Boolean.h,v 1.2 1998/08/29 11:47:31 nmcl Exp $
 */

#ifndef BOOLEAN_H_
#define BOOLEAN_H_

#ifdef STUB
#  pragma @NoRename
#endif

/* first check for TRUE and FALSE definitions - if so then undefine them */

#ifdef TRUE
#  undef TRUE
#  undef FALSE
#endif

/* For C++ usage */

#ifndef HAVE_BOOLEAN_TYPE
typedef unsigned char Boolean;

extern const Boolean TRUE;
extern const Boolean FALSE;
#else

typedef bool Boolean;

#  define TRUE true
#  define FALSE false
#endif

#endif
