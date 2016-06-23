/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ErrorListI.cc,v 1.1 1997/09/25 15:25:28 nmcl Exp $
 */

/*
 *
 * Error iterator class
 *
 */ 

#ifndef ERRORLIST_H_
#  include <Common/ErrorList.h>
#endif

#ifndef ERRORLISTI_H_
#  include <Common/ErrorListI.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

static const char RCSid[] = "$Id: ErrorListI.cc,v 1.1 1997/09/25 15:25:28 nmcl Exp $";

/*
 * Public constructors and destructors 
 */

ErrorListI::ErrorListI ( const ErrorList& L )
		       : currentList(L),
			 next(L.head)
{ 
}

ErrorListI::~ErrorListI ()
{
}

const Error *const ErrorListI::operator() ()
{
    Error *current = next;
    if (current == 0)
    {
	return 0;
    }
    else
	next = current->getLink();

    return current;
}

#ifdef NO_INLINES
#  define ERRORLISTI_CC_
#  include <Common/ErrorListI.n>
#  undef ERRORLISTI_CC_
#endif
