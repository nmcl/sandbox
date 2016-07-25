/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ADclCntxt.cc,v 1.7 1993/06/17 15:08:45 ngdp Exp $
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
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


static const char RCSid[] = "$Id: ADclCntxt.cc,v 1.7 1993/06/17 15:08:45 ngdp Exp $";

/*
 * Public constructor and destructor
 */

/*
 * Virtual public operations
 */

Boolean ArgDeclContext::isApplicable ( StubModifier sm ) const
{
    switch (sm)
    {
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOMARSHALL:
    case STUB_AUTOMARSHALL:
    case STUB_USERMARSHALL:
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
