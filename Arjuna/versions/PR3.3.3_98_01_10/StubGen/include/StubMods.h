/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubMods.h,v 1.6 1995/03/03 13:46:26 ngdp Exp $
 */

#ifndef STUBMODS_H_
#define STUBMODS_H_

/*
 *
 * Enumeration for stub specific modifiers.
 *     
 */


#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;

enum StubModifier { STUB_NOMARSHALL, STUB_AUTOMARSHALL, STUB_USERMARSHALL,
		    STUB_NOREMOTE, STUB_REMOTE, STUB_NOCLIENT,
		    STUB_ARG_IN, STUB_ARG_OUT, STUB_ARG_INOUT, STUB_ARG_NEITHER,
		    STUB_FREEMEMORY, STUB_NODELETE, STUB_DELETE,
		    STUB_NORENAME, STUB_PENDING, STUB_DEFAULT, 
		    STUB_NOWARNINGS, STUB_IGNORE };

extern ostream& operator<< (ostream&, StubModifier);
extern void printStubModifier (ostream&, StubModifier, DeclStyle);

#endif
