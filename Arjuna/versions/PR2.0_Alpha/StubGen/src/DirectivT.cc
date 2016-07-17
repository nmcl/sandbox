/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DirectivT.cc,v 1.6 1993/04/14 13:46:05 ngdp Exp $
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

static const char RCSid[] = "$Id: DirectivT.cc,v 1.6 1993/04/14 13:46:05 ngdp Exp $";

/*
 * PUBLIC operations:
 */

Directive::Directive ( const String& dname  ) 
                     : DummyType(),
		       directive(dname)
{
#ifdef DEBUG
    debug_stream << "Directive::Directive(" << dname << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Directive::~Directive ()
{
#ifdef DEBUG
    debug_stream << "Directive::~Directive(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void Directive::prename ( ostream& s, DeclStyle )
{
    s << directive;
}

void Directive::postname ( ostream& s, DeclStyle )
{
    s <<"\n";
}

String Directive::typename ( DeclStyle ) const
{
    return directive;
}

String Directive::signature () const
{
    return directive;
}
