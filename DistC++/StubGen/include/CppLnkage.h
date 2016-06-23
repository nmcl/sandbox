/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CppLnkage.h,v 1.1 1997/09/25 15:29:43 nmcl Exp $
 */

#ifndef CPPLNKAGE_H_
#define CPPLNKAGE_H_

/*
 *
 * Stub generator linkage class. Provides name mangling routines for
 * overloading purposes - C++ version
 *
 */

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
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
