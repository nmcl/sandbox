/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Utility.h,v 1.6 1993/03/22 09:27:51 ngdp Exp $
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
extern String constructname (const String&, long);
extern long hashit (const String&);

#endif
