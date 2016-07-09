/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: EDclCntxt.cc,v 1.1 1997/06/09 19:52:22 nmcl Exp $
 */

#ifndef STRSTREAM_H_
#  include "strstream"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

#ifndef EDCLCNTXT_H_
#  include "EDclCntxt.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLLIST_H_
#  include "DeclList.h"
#endif

#ifndef DECLLISTI_H_
#  include "DeclListI.h"
#endif


static const char RCSid[] = "$Id: EDclCntxt.cc,v 1.1 1997/06/09 19:52:22 nmcl Exp $";

/*
 * Public constructor and destructor
 */

/*
 * Virtual public operations
 */

Boolean ExceptionDeclContext::isApplicable ( StubModifier sm ) const
{
    switch (sm)
    {
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOMARSHALL:
    case STUB_AUTOMARSHALL:
    case STUB_USERMARSHALL:
    case STUB_PENDING:
    case STUB_DEFAULT:
    case STUB_ARG_IN:
    case STUB_ARG_INOUT:
	return FALSE;
    default:
	return TRUE;
    }
}

#ifdef NO_INLINES
#  define EDCLCNTXT_CC_
#  include "EDclCntxt.n"
#  undef EDCLCNTXT_CC_
#endif
