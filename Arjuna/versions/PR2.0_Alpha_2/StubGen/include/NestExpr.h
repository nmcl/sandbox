/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NestExpr.h,v 1.1 1993/11/03 14:45:40 nmcl Exp $
 */

#ifndef NESTEXPR_H_
#define NESTEXPR_H_

/*
 *
 * Simple expression handling. Represent nested expressions
 *
 */

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

class ostream;

class NestedExpression : public Expression
{
public:
    NestedExpression (Expression *, const char = '(', const char = ')');

    virtual ostream& printExpr (ostream&);
private:
    Expression *nested;
    char left, right;
};

#include "NestExpr.n"

#endif
