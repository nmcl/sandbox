/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ADclCntxt.cc,v 1.1 1997/09/25 15:30:35 nmcl Exp $
 */

#ifndef WIN32
#  include "strstream.h"
#else
#  include "strstrea.h"
#endif

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

#ifndef FUNCT_H_
#  include "FuncT.h"
#endif

#ifndef ADCLCNTXT_H_
#  include "ADclCntxt.h"
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

static const char RCSid[] = "$Id: ADclCntxt.cc,v 1.1 1997/09/25 15:30:35 nmcl Exp $";

/*
 * Public constructor and destructor
 */

/*
 * Virtual public operations
 */

String ArgDeclContext::getSimpleContextName ( DeclStyle ds ) const
{
    if (funcArgsAreFor)
	return funcArgsAreFor->massageName(NullString,ds);
    else
	return NullString;
}

Boolean ArgDeclContext::isApplicable ( StubModifier sm ) const
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
	return FALSE;
    default:
	return TRUE;
    }
}

#ifdef NO_INLINES
#  define ADCLCNTXT_CC_
#  include "ADclCntxt.n"
#  undef ADCLCNTXT_CC_
#endif
