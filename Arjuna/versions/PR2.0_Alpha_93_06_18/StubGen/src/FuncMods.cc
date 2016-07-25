/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncMods.cc,v 1.4 1993/03/22 09:31:08 ngdp Exp $
 */

/*
 * Stub generator utility routines for FuncModifier varaibles
 *
 */

#include <strstream.h>

#ifndef FUNCMODS_H_
#  include "FuncMods.h"
#endif

static const char RCSid[] = "$Id: FuncMods.cc,v 1.4 1993/03/22 09:31:08 ngdp Exp $";

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
