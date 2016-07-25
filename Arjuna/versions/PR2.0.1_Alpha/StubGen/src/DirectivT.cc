/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DirectivT.cc,v 1.1 1993/11/03 14:47:31 nmcl Exp $
 */

/*
 * Stub generator cpp directive handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef DIRECTIVET_H_
#  include "DirectiveT.h"
#endif

static const char RCSid[] = "$Id: DirectivT.cc,v 1.1 1993/11/03 14:47:31 nmcl Exp $";

/*
 * PUBLIC operations:
 */

Directive::Directive ( const String& dname  ) 
                     : DummyType(),
		       directive(dname)
{
#ifdef DEBUG
    debugStream << "Directive::Directive(" << dname << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Directive::~Directive ()
{
#ifdef DEBUG
    debugStream << "Directive::~Directive(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

String Directive::preName ( DeclStyle, Boolean, Boolean ) const
{
    return directive;
}

String Directive::postName ( DeclStyle ) const
{
    return "\n";
}
