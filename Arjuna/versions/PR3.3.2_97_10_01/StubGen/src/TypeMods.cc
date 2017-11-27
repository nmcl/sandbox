/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeMods.cc,v 1.1 1993/11/03 14:48:24 nmcl Exp $
 */

/*
 * Stub generator utility routines for TypeModifier variables
 *
 */

#include <strstream.h>

#ifndef TYPEMODS_H_
#  include "TypeMods.h"
#endif

static const char RCSid[] = "$Id: TypeMods.cc,v 1.1 1993/11/03 14:48:24 nmcl Exp $";

ostream& operator<< ( ostream& s , TypeModifier tm )
{
    switch (tm)
    {
    case MOD_SHORT:
	s << "short ";
	break;
    case MOD_LONG:
	s << "long ";
	break;
    case MOD_SIGNED:
	s << "signed ";
	break;
    case MOD_UNSIGNED:
	s << "unsigned ";
    }

    return s;
}

