/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncMods.cc,v 1.1 1997/06/09 19:52:22 nmcl Exp $
 */

/*
 * Stub generator utility routines for FuncModifier varaibles
 *
 */

#include <strstream>

#ifndef FUNCMODS_H_
#  include "FuncMods.h"
#endif

static const char RCSid[] = "$Id: FuncMods.cc,v 1.1 1997/06/09 19:52:22 nmcl Exp $";

ostream& operator<< ( ostream& s , FuncModifier fm )
{
    switch (fm)
    {
    case FUNC_INLINE:
	s << "inline ";
	break;
    case FUNC_VIRTUAL:
	s << "virtual ";
    }

    return s;
}
