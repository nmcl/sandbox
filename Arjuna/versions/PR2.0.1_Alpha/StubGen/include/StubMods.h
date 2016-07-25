/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StubMods.h,v 1.1 1993/11/03 14:45:52 nmcl Exp $
 */

#ifndef STUBMODS_H_
#define STUBMODS_H_

/*
 *
 * Enumeration for stub specific modifiers.
 *     
 */


class ostream;

enum StubModifier { STUB_AUTOMARSHALL, STUB_USERMARSHALL, STUB_NOMARSHALL,
		    STUB_REMOTE, STUB_NOREMOTE, STUB_NOCLIENT,
		    STUB_ARG_IN, STUB_ARG_OUT, STUB_ARG_INOUT, STUB_ARG_NEITHER,
		    STUB_NORENAME, STUB_PENDING, STUB_DEFAULT, 
		    STUB_NOWARNINGS, STUB_IGNORE };

extern ostream& operator<< (ostream&, StubModifier);

#endif
