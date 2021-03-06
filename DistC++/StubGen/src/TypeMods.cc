/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeMods.cc,v 1.1 1997/09/25 15:31:32 nmcl Exp $
 */

/*
 * Stub generator utility routines for TypeModifier variables
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#ifndef TYPEMODS_H_
#  include "TypeMods.h"
#endif

static const char RCSid[] = "$Id: TypeMods.cc,v 1.1 1997/09/25 15:31:32 nmcl Exp $";

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

