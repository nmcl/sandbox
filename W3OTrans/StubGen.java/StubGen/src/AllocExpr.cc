/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AllocExpr.cc,v 1.3 1995/02/10 12:10:28 ngdp Exp $
 */

/*
 * Stub generator utility routines for simple expression handling
 *
 */

#include <iostream.h>

#ifndef ALLOCEXPR_H_
#  include "AllocExpr.h"
#endif

static const char RCSid[] = "$Id: AllocExpr.cc,v 1.3 1995/02/10 12:10:28 ngdp Exp $";

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
