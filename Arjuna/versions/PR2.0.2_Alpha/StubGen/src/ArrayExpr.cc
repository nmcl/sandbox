/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArrayExpr.cc,v 1.3 1993/05/06 14:49:56 nsmw Exp $
 */

/*
 * Stub generator utility routines for simple array expression handling
 *
 */


#ifndef ARRAYEXPR_H_
#  include "ArrayExpr.h"
#endif

static const char RCSid[] = "$Id: ArrayExpr.cc,v 1.3 1993/05/06 14:49:56 nsmw Exp $";

ostream& ArrayExpression::printExpr ( ostream &s )
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

