/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Expr.cc,v 1.1 1993/11/03 14:47:40 nmcl Exp $
 */

/*
 * Stub generator utility routines for simple expression handling
 *
 */

#include <strstream.h>

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

static const char RCSid[] = "$Id: Expr.cc,v 1.1 1993/11/03 14:47:40 nmcl Exp $";

#ifdef __GNUG__
// g++ does not seem to like conversion operators.

String Expression::asString () const
{
    char buffer[1024];
    ostrstream strm(buffer, 1024);
    
    strm << *this << ends;
    
    return buffer;
}
#else

Expression::operator String ()
{
    char buffer[1024];
    ostrstream strm(buffer, 1024);
    
    strm << *this << ends;
    
    return buffer;
}
#endif

#ifdef NO_INLINES
#  define EXPRESSION_CC_
#  include "Expression.n"
#  undef EXPRESSION_CC_
#endif
