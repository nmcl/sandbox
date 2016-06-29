/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id
 */

/*
 * Stub generator utility routines for StorageSpecifier type variables
 *
 */

#include <strstream.h>

#ifndef STORESPEC_H_
#  include "StoreSpec.h"
#endif

static const char RCSid[] = "$Id: StoreSpec.cc,v 1.1 1997/06/09 19:52:24 nmcl Exp $";

ostream& operator<< ( ostream& s, StorageSpecifier sm )
{
    switch (sm)
    {
    case SC_OVERLOAD:
        s << "overload ";
        break;
    case SC_FRIEND:
        s << "friend ";
        break;
    case SC_TYPEDEF:
        s << "typedef ";
        break;
    case SC_AUTO:
        s << "auto ";
        break;
    case SC_EXTERN:
        s << "extern ";
        break;
    case SC_REGISTER:
        s << "register ";
        break;
    case SC_STATIC:
        s << "static ";
    case SC_DEFAULT:
        break;
    }
    return s;
}
