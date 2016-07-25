/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BaseClass.cc,v 1.11 1993/06/17 15:08:47 ngdp Exp $
 */

/*
 * Stub generator definition handler for base class lists
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
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

static const char RCSid[] = "$Id: BaseClass.cc,v 1.11 1993/06/17 15:08:47 ngdp Exp $";

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

    if (visibility != CLASS_DEFAULT)
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

#ifdef NO_INLINES
#  define BASECLASS_CC_
#  include "BaseClass.n"
#  undef BASECLASS_CC_
#endif
