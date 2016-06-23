/*
 * Copyright (C) 1993-1997,
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

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#ifndef STORESPEC_H_
#  include "StoreSpec.h"
#endif

static const char RCSid[] = "$Id: StoreSpec.cc,v 1.1 1997/09/25 15:31:19 nmcl Exp $";

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
