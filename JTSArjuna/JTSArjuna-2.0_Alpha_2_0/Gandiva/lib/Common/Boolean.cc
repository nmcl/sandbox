/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Boolean.cc,v 1.2 1996/01/02 13:14:02 nsmw Exp $
 */

#include <os/iostream.h>

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef __GNUG__
ostream &operator<< (ostream &ostrm, const Boolean &boolean)
{
    if (boolean)
        ostrm << "TRUE";
    else
        ostrm << "FALSE";

    return ostrm;
}
#endif
