/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubMods.cc,v 1.1 1993/11/03 14:48:15 nmcl Exp $
 */

/*
 * Stub generator utility routines for StubModifier varaibles
 *
 */

#include <strstream.h>

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

static const char RCSid[] = "$Id: StubMods.cc,v 1.1 1993/11/03 14:48:15 nmcl Exp $";

ostream& operator<< ( ostream& s, StubModifier sm )
{
    switch (sm)
    {
    case STUB_AUTOMARSHALL:
	s << "@AutoMarshall";
	break;
    case STUB_USERMARSHALL:
	s << "@UserMarshall";
	break;
    case STUB_NOMARSHALL:
	s << "@NoMarshall";
	break;
    case STUB_REMOTE:
	s << "@Remote";
	break;
    case STUB_NOREMOTE:
	s << "@NoRemote";
	break;
    case STUB_NOCLIENT:
	s << "@NoClient";
	break;
    case STUB_ARG_IN:
	s << "@In";
	break;
    case STUB_ARG_OUT:
	s << "@Out";
	break;
    case STUB_ARG_INOUT:
	s << "@InOut";
	break;
    case STUB_ARG_NEITHER:
	s << "@Neither";
	break;
    case STUB_NORENAME:
	s << "@NoRename";
	break;
    case STUB_PENDING:
	s << "@Pending";
	break;
    case STUB_DEFAULT:
	s << "@Default";
	break;
    case STUB_IGNORE:
	s << "@Ignore";
	break;
    case STUB_NOWARNINGS:
	s << "@NoWarnings";
	break;
    }
    
    return s;
}
