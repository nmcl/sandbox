/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BinExpr.cc,v 1.1 1997/09/25 15:30:38 nmcl Exp $
 */

/*
 * Stub generator utility routines for simple expression handling
 *
 */

#include <iostream.h>

#ifndef BINEXPR_H_
#  include "BinExpr.h"
#endif

static const char RCSid[] = "$Id: BinExpr.cc,v 1.1 1997/09/25 15:30:38 nmcl Exp $";

ostream& BinaryExpression::printExpr ( ostream &s ) const
{
    if (leftOperand)
	s << *leftOperand;
    s << exprOperator;
    if (rightOperand)
	s << *rightOperand;
    
    return s;
}

#ifdef NO_INLINES
#  define BINEXPR_CC_
#  include "BinExpr.n"
#  undef BINEXPR_CC_
#endif
