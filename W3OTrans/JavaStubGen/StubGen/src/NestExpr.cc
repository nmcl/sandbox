/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NestExpr.cc,v 1.2 1994/07/25 13:27:19 ngdp Exp $
 */

/*
 * Stub generator utility routines for simple expression handling
 *
 */

#include <iostream.h>

#ifndef NESTEXPR_H_
#  include "NestExpr.h"
#endif

static const char RCSid[] = "$Id: NestExpr.cc,v 1.2 1994/07/25 13:27:19 ngdp Exp $";

ostream& NestedExpression::printExpr ( ostream &s ) const
{
    s << left << *nested << right;

    return s;
}

#ifdef NO_INLINES
#  define NESTEXPR_CC_
#  include "NestExpr.n"
#  undef NESTEXPR_CC_
#endif
