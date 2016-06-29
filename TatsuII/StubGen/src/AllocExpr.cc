/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AllocExpr.cc,v 1.1 1997/06/09 19:52:19 nmcl Exp $
 */

/*
 * Stub generator utility routines for simple expression handling
 *
 */

#include <iostream.h>

#ifndef ALLOCEXPR_H_
#  include "AllocExpr.h"
#endif

static const char RCSid[] = "$Id: AllocExpr.cc,v 1.1 1997/06/09 19:52:19 nmcl Exp $";

ostream& AllocExpression::printExpr ( ostream &s ) const
{
    if (global)
	s << "::";

    s << "new ";

    if (placement)
	s << *placement;
    
    s << *newOn;

    if (init)
	s << *init;

    return s;
}

#ifdef NO_INLINES
#  define ALLOCEXPR_CC_
#  include "AllocExpr.n"
#  undef ALLOCEXPR_CC_
#endif
