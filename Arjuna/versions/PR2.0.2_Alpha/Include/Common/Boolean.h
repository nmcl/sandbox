/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Boolean.h,v 1.9 1993/03/18 14:30:25 nsmw Exp $
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

typedef unsigned short Boolean;

extern const Boolean TRUE;
extern const Boolean FALSE;

#endif
