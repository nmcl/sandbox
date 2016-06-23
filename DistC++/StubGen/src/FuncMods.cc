/*
 * Copyright (C) 1993-1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncMods.cc,v 1.1 1997/09/25 15:31:00 nmcl Exp $
 */

/*
 * Stub generator utility routines for FuncModifier varaibles
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#ifndef FUNCMODS_H_
#  include "FuncMods.h"
#endif

static const char RCSid[] = "$Id: FuncMods.cc,v 1.1 1997/09/25 15:31:00 nmcl Exp $";

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
