/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BinExpr.cc,v 1.3 1993/05/06 14:49:58 nsmw Exp $
 */

/*
 * Stub generator utility routines for simple expression handling
 *
 */


#ifndef BINEXPR_H_
#  include "BinExpr.h"
#endif

static const char RCSid[] = "$Id: BinExpr.cc,v 1.3 1993/05/06 14:49:58 nsmw Exp $";

ostream& BinaryExpression::printExpr ( ostream &s )
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
