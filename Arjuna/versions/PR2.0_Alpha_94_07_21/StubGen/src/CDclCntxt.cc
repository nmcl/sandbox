/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CDclCntxt.cc,v 1.8 1993/06/17 15:08:49 ngdp Exp $
 */

/*
 * ClassDeclaration context class for stub generator. Holds all 
 * symbols declared within a particular class.
 * ClassDeclContexts are chained togethor to allow base class members
 * to be found.
 *
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

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

#ifndef CDCLCNTXT_H_
#  include "CDclCntxt.h"
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


static const char RCSid[] = "$Id: CDclCntxt.cc,v 1.8 1993/06/17 15:08:49 ngdp Exp $";

/*
 * Public constructor and destructor
 */

ClassDeclContext::ClassDeclContext ( ClassType *myclass,
				     DeclContext *parent,
				     BaseClass *bclasses,
				     int initialsize )
                                   : DeclContext(parent, initialsize),
				     inheritedContext(bclasses),
				     owner(myclass)
{
}

ClassDeclContext::~ClassDeclContext ()
{
}

Boolean ClassDeclContext::isApplicable ( StubModifier sm ) const
{
    switch (sm)
    {
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
    case STUB_NORENAME:
	return FALSE;
    default:
	return TRUE;
    }
}

/*
 * Virtual public operations
 */

/*
 * Look up an entry in a class. We look for an exact match on name and
 * type unless type is ANY_TYPE when the first matching name is used. If
 * no match exists in this class the search may be propagated to the
 * base classes (if any exists) until a match is found in which case 
 * return NULL. Whether this is done or not
 * is controlled by the setting of the context parameter
 */

Declaration *ClassDeclContext::lookFor ( const String& name,
				         Type type,
				         Context lookin ) const
{
    BaseClass *nextBase = inheritedContext;
    Declaration *decl = 0;

    /* Look in 'this' first of all providing not told otherwise */

    if (lookin != INHERITED_CONTEXT)
	decl = find (name, type, CURRENT_CONTEXT);

    /* If only looking in this context return result without further ado */

    if (lookin == CURRENT_CONTEXT)
     return decl;

    /* If not found do breadth first search of all base classes */

    while (decl == 0 && nextBase != 0)
    {
	decl = nextBase->getClass()->lookFor(name, type, CURRENT_CONTEXT);
	nextBase = nextBase->getNext();
    }

    /* If still not found in direct base classes look in indirect */

    nextBase = inheritedContext;

    while (decl == 0 && nextBase != 0)
    {
	nextBase->getClass()->lookFor(name, type, INHERITED_CONTEXT);
	nextBase = nextBase->getNext();
    }

    if ((decl == 0) &&
	(lookin == ANY_CONTEXT) &&
	(getEnclosingContext() != 0))
        return (getEnclosingContext()->lookFor(name, type, lookin));
    else
        return decl;
}

Boolean ClassDeclContext::putIn ( Declaration *d )
{
    d->setVisibility(owner->getVisibility());
    return DeclContext::putIn(d);
}


