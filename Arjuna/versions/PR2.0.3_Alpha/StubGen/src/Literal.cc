/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Literal.cc,v 1.3 1993/05/06 14:50:05 nsmw Exp $
 */

/*
 * Stub generator utility routines for simple expression handling
 *
 */


#ifndef LITERAL_H_
#  include "Literal.h"
#endif

static const char RCSid[] = "$Id: Literal.cc,v 1.3 1993/05/06 14:50:05 nsmw Exp $";

ostream& Literal::printExpr ( ostream &s )
{
    s << theLiteral;
    
    return s;
}

#ifdef NO_INLINES
#  define LITERAL_CC_
#  include "Literal.n"
#  undef LITERAL_CC_
#endif
