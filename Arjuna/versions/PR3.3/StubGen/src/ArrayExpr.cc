/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArrayExpr.cc,v 1.3 1995/02/10 12:10:28 ngdp Exp $
 */

/*
 * Stub generator utility routines for simple array expression handling
 *
 */

#include <iostream.h>

#ifndef ARRAYEXPR_H_
#  include "ArrayExpr.h"
#endif

static const char RCSid[] = "$Id: ArrayExpr.cc,v 1.3 1995/02/10 12:10:28 ngdp Exp $";

ostream& ArrayExpression::printExpr ( ostream &s ) const
{
    if (arrayOf)
	s << *arrayOf;
    
    s << "[";

    if (indexInto)
	s << *indexInto;

    s << "]";

    return s;
}

#ifdef NO_INLINES
#  define ARRAYEXPR_CC_
#  include "ArrayExpr.n"
#  undef ARRAYEXPR_CC_
#endif

