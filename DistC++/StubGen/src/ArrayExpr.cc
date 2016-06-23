/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArrayExpr.cc,v 1.1 1997/09/25 15:30:36 nmcl Exp $
 */

/*
 * Stub generator utility routines for simple array expression handling
 *
 */

#include <iostream.h>

#ifndef ARRAYEXPR_H_
#  include "ArrayExpr.h"
#endif

static const char RCSid[] = "$Id: ArrayExpr.cc,v 1.1 1997/09/25 15:30:36 nmcl Exp $";

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

