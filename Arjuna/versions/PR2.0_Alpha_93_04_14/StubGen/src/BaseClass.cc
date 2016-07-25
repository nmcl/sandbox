/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BaseClass.cc,v 1.10 1993/04/14 13:45:58 ngdp Exp $
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

static const char RCSid[] = "$Id: BaseClass.cc,v 1.10 1993/04/14 13:45:58 ngdp Exp $";

/*
 * PUBLIC operations
 */

Boolean BaseClass::append ( BaseClass *new_base )
{
    if (base_class)
	if (new_base->get_class()->get_class_hashval() == base_class->get_class_hashval())
	    return FALSE;

    if (next_base)
	return next_base->append(new_base);
    else
	next_base = new_base;

    return TRUE;
}

void BaseClass::freeze ()
{
    if (base_class)
	base_class->freeze_type();

    if (next_base)
	next_base->freeze();
}

ostream& BaseClass::print_baseclass ( ostream& s, DeclStyle ds )
{
    if (virtual_base)
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

    switch (ds)
    {
    case CLIENT_DECL:
	s << base_class->get_clientname();
	break;
    case SERVER_DECL:
	s << base_class->get_servername();
	break;
    case SERVERIMPLEMENTATION_DECL:
	s << base_class->get_implname();
	break;
    case NORMAL_DECL:
	s << base_class->typename(ds);
    }

    return s;
}

#ifdef NO_INLINES
#  define BASECLASS_CC_
#  include "BaseClass.n"
#  undef BASECLASS_CC_
#endif
