/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubMods.cc,v 1.1 1997/06/09 19:52:24 nmcl Exp $
 */

/*
 * Stub generator utility routines for StubModifier varaibles
 *
 */

#include <strstream>

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

static const char RCSid[] = "$Id: StubMods.cc,v 1.1 1997/06/09 19:52:24 nmcl Exp $";

void printStubModifier ( ostream& s, StubModifier sm, DeclStyle ds )

{
    if (ds != IDL_DECL)
	s << "@";
    
    switch (sm)
    {
    case STUB_AUTOMARSHALL:
	s << "AutoMarshall";
	break;
    case STUB_USERMARSHALL:
	s << "UserMarshall";
	break;
    case STUB_NOMARSHALL:
	s << "NoMarshall";
	break;
    case STUB_REMOTE:
	s << "Remote";
	break;
    case STUB_NOREMOTE:
	s << "NoRemote";
	break;
    case STUB_NOCLIENT:
	s << "NoClient";
	break;
    case STUB_ARG_IN:
	s << "in";
	break;
    case STUB_ARG_OUT:
	s << "out";
	break;
    case STUB_ARG_INOUT:
	s << "inout";
	break;
    case STUB_ARG_NEITHER:
//	s << "Neither";
	break;
    case STUB_FREEMEMORY:
	s << "FreeMemory";
	break;
    case STUB_NODELETE:
	s << "NoDelete";
	break;
    case STUB_DELETE:
	s << "Delete";
	break;
    case STUB_NORENAME:
//	s << "NoRename";
	break;
    case STUB_PENDING:
	s << "Pending";
	break;
    case STUB_DEFAULT:
	s << "Default";
	break;
    case STUB_IGNORE:
//	s << "Ignore";
	break;
    case STUB_NOWARNINGS:
	s << "NoWarnings";
	break;
    }
}

ostream& operator<< ( ostream& s, StubModifier sm )
{
    printStubModifier(s,sm, NORMAL_DECL);
    return s;
}
