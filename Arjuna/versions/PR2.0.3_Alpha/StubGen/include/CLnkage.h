/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CLnkage.h,v 1.5 1993/06/17 15:07:00 ngdp Exp $
 */

#ifndef CLNKAGE_H_
#define CLNKAGE_H_

/*
 *
 * Stub generator linkage class. Provides name mangling routines for
 * overloading purposes - C version
 *
 */

#ifndef STRING_H_
#  include "String.h"
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
