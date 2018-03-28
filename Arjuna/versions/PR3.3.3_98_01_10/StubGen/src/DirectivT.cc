/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DirectivT.cc,v 1.2.6.1 1996/10/10 12:26:56 ngdp Exp $
 */

/*
 * Stub generator cpp directive handler
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef DIRECTIVET_H_
#  include "DirectiveT.h"
#endif

static const char RCSid[] = "$Id: DirectivT.cc,v 1.2.6.1 1996/10/10 12:26:56 ngdp Exp $";

/*
 * PUBLIC operations:
 */

Directive::Directive ( const String& dname  ) 
                     : DummyType(),
		       directive(dname)
{
#ifdef SG_DEBUG
    debugStream << "Directive::Directive(" << dname << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Directive::~Directive ()
{
#ifdef SG_DEBUG
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
