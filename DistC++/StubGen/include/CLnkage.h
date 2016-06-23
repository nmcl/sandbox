/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CLnkage.h,v 1.1 1997/09/25 15:29:38 nmcl Exp $
 */

#ifndef CLNKAGE_H_
#define CLNKAGE_H_

/*
 *
 * Stub generator linkage class. Provides name mangling routines for
 * overloading purposes - C version
 *
 */

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef LINKAGE_H_
#  include "Linkage.h"
#endif

class Declaration;

class CLinkage : public Linkage
{
public:
    /* virtual function for mangling names */
    
    virtual String mangle (TypeDescriptor *) const;
    virtual ostream& printLinkage (ostream&);
};

extern CLinkage *cLinkage;

#endif
