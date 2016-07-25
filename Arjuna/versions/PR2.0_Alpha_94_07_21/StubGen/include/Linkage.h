/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Linkage.h,v 1.5 1993/06/17 15:08:06 ngdp Exp $
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
