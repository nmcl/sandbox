/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Expression.h,v 1.1 1997/09/25 15:30:00 nmcl Exp $
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

/*
 *
 * Simple expression handling. Used as the base class for handling of
 * all basic expressions
 */

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

class ostream;

class Expression
{
public:
    Expression ();

    virtual ostream& printExpr (ostream&) const = 0;

#ifdef BROKEN_CONVERSION_OPERS
    String asString () const;
#else
    operator String () const;
#endif
};

#include "Expression.n"

#endif
