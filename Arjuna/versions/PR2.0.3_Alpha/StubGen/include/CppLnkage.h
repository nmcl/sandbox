/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CppLnkage.h,v 1.5 1993/06/17 15:07:07 ngdp Exp $
 */

#ifndef CPPLNKAGE_H_
#define CPPLNKAGE_H_

/*
 *
 * Stub generator linkage class. Provides name mangling routines for
 * overloading purposes - C++ version
 *
 */

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef LINKAGE_H_
#  include "Linkage.h"
#endif

class TypeDescriptor;

class CppLinkage : public Linkage
{
public:
    /* virtual function for mangling names */
    
    virtual String mangle (TypeDescriptor *) const;

    virtual ostream& printLinkage (ostream&);
};

extern CppLinkage *cppLinkage;

#endif
