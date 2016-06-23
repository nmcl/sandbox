/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NestExpr.h,v 1.1 1997/09/25 15:30:19 nmcl Exp $
 */

#ifndef NESTEXPR_H_
#define NESTEXPR_H_

/*
 *
 * Simple expression handling. Represent nested expressions
 *
 */

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

class ostream;

class NestedExpression : public Expression
{
public:
    NestedExpression (Expression *, const char = '(', const char = ')');

    virtual ostream& printExpr (ostream&) const;
private:
    Expression *nested;
    char left, right;
};

#include "NestExpr.n"

#endif
