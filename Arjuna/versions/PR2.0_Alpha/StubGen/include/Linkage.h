/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Linkage.h,v 1.4 1993/03/22 09:27:31 ngdp Exp $
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
    virtual ostream& print_linkage (ostream&) = 0;
};

#include "Linkage.n"

#endif
