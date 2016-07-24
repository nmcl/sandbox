/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Expression.h,v 1.1 1993/11/03 14:45:02 nmcl Exp $
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

/*
 *
 * Simple expression handling. Used as the base class for handling of
 * all basic expressions
 */

#ifndef STRING_H_
#  include "String.h"
#endif

class ostream;

class Expression
{
public:
    Expression ();

    virtual ostream& printExpr (ostream&) = 0;

#ifdef __GNUG__
    String asString () const;
#else
    operator String ();
#endif
};

#include "Expression.n"

#endif
