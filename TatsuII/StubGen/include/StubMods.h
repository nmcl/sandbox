/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubMods.h,v 1.1 1997/06/09 19:52:17 nmcl Exp $
 */

#ifndef STUBMODS_H_
#define STUBMODS_H_

#include <iostream>

using namespace std;

/*
 *
 * Enumeration for stub specific modifiers.
 *     
 */

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif


enum StubModifier { STUB_NOMARSHALL, STUB_AUTOMARSHALL, STUB_USERMARSHALL,
		    STUB_NOREMOTE, STUB_REMOTE, STUB_NOCLIENT,
		    STUB_ARG_IN, STUB_ARG_OUT, STUB_ARG_INOUT, STUB_ARG_NEITHER,
		    STUB_FREEMEMORY, STUB_NODELETE, STUB_DELETE,
		    STUB_NORENAME, STUB_PENDING, STUB_DEFAULT, 
		    STUB_NOWARNINGS, STUB_IGNORE };

extern ostream& operator<< (ostream&, StubModifier);
extern void printStubModifier (ostream&, StubModifier, DeclStyle);

#endif
