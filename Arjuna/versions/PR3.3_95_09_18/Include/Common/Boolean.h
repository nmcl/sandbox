/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Boolean.h,v 1.2 1994/08/30 08:06:28 ngdp Exp $
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
typedef unsigned short Boolean;

extern const Boolean TRUE;
extern const Boolean FALSE;
#else

typedef bool Boolean;

#  define TRUE true
#  define FALSE false
#endif

#endif
