/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BaseClass.cc,v 1.1 1997/06/09 19:52:19 nmcl Exp $
 */

/*
 * Stub generator definition handler for base class lists
 *
 */

#ifndef WIN32

#include <strstream.h>

#else

#include <strstrea.h>

#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

#ifndef BASECLASS_H_
#  include "BaseClass.h"
#endif

static const char RCSid[] = "$Id: BaseClass.cc,v 1.1 1997/06/09 19:52:19 nmcl Exp $";

/*
 * PUBLIC operations
 */

Boolean BaseClass::append ( BaseClass *newBase )
{
    if (baseClass)
	if (newBase->getClass()->getClassHashval() == baseClass->getClassHashval())
	    return FALSE;

    if (nextBase)
	return nextBase->append(newBase);
    else
	nextBase = newBase;

    return TRUE;
}

void BaseClass::freeze ()
{
    if (baseClass)
	baseClass->freezeType();

    if (nextBase)
	nextBase->freeze();
}

ostream& BaseClass::printBaseClass ( ostream& s, DeclStyle ds )
{
    if (virtualBase)
	s << "virtual ";

    if ((visibility != CLASS_DEFAULT) && (ds != IDL_DECL))
    {
	switch (visibility)
	{
	case CLASS_PRIVATE:
	    s << "private ";
	    break;
	case CLASS_PROTECTED:
	    s << "protected ";
	    break;
	case CLASS_PUBLIC:
	    s << "public ";
	    break;
	default:
	    break;
	}
    }

    s << baseClass->typeName(ds);

    return s;
}

StubModifier BaseClass::defaultLocation () const
{
    const BaseClass *next = this;
    StubModifier bestFound = STUB_REMOTE;
    StubModifier current;

    while (next)
    {
	ClassType *ct = next->getClass();
	if ((current = ct->defaultLocation()) < bestFound)
	{
	    bestFound = current;
	}
	
	next = getNext();
    }

    return bestFound;
}


StubModifier BaseClass::defaultMarshall () const
{
    const BaseClass *next = this;
    StubModifier bestFound = STUB_USERMARSHALL;
    StubModifier current;

    while (next)
    {
	ClassType *ct = next->getClass();
	if ((current = ct->defaultMarshall()) < bestFound)
	{
	    bestFound = current;
	}
	
	next = next->getNext();
    }

    return bestFound;
}



#ifdef NO_INLINES
#  define BASECLASS_CC_
#  include "BaseClass.n"
#  undef BASECLASS_CC_
#endif
