/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StCommonT.h,v 1.1 1997/06/09 19:52:17 nmcl Exp $
 */

#ifndef STCOMMONT_H_
#define STCOMMONT_H_

/*
 *
 * Stub generator common type definitions.
 *
 */

/* first check for TRUE and FALSE definitions - if so then undefine them */

#ifdef TRUE
#  undef TRUE
#  undef FALSE
#endif

/* For C++ usage */

#ifndef HAVE_BOOLEAN_TYPE
enum Boolean { FALSE = 0, TRUE };

class ostream;

extern ostream& operator<< (ostream&, Boolean);

#else

typedef bool Boolean;

#  define TRUE true
#  define FALSE false
#endif

#endif

