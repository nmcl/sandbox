/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Expr.cc,v 1.1 1997/09/25 15:30:56 nmcl Exp $
 */

/*
 * Stub generator utility routines for simple expression handling
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

static const char RCSid[] = "$Id: Expr.cc,v 1.1 1997/09/25 15:30:56 nmcl Exp $";

#ifdef BROKEN_CONVERSION_OPERS

String Expression::asString () const
{
    char buffer[1024];
    ostrstream strm(buffer, 1024);
    
    strm << *this << ends;
    
    return buffer;
}
#else

Expression::operator String () const
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
