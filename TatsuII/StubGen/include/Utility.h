/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Utility.h,v 1.1 1997/06/09 19:52:18 nmcl Exp $
 */

#ifndef UTILITY_H_
#define UTILITY_H_

/*
 * Forward references to classes we don't want to know about fully (yet)
 */

class iostream;				/* standard system io */
class String;				/* reference-counted strings */

/*
 * Prototypes for some generally useful routines
 */

extern char *const locateProg(const char *);
extern String constructName (const String&, long);
extern long hashIt (const String&);

#endif
