/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Linkage.h,v 1.1 1993/11/03 14:45:29 nmcl Exp $
 */

#ifndef LINKAGE_H_
#define LINKAGE_H_

/*
 *
 * Stub generator linkage class. Provides name mangling routines for
 * overloading purposes
 *
 */

class ostream;

class String;
class TypeDescriptor;

class Linkage
{
public:
    virtual String mangle (TypeDescriptor *) const = 0;
    virtual ostream& printLinkage (ostream&) = 0;
};

#include "Linkage.n"

#endif
