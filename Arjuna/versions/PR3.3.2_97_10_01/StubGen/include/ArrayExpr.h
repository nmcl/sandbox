/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArrayExpr.h,v 1.2 1994/07/25 13:26:55 ngdp Exp $
 */

#ifndef ARRAYEXPR_H_
#define ARRAYEXPR_H_

/*
 *
 * Simple expression handling. Represent array reference expressions
 *
 */

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

class ostream;

class ArrayExpression : public Expression
{
public:
    ArrayExpression (Expression *, Expression *);

    virtual ostream& printExpr (ostream&) const;
private:
    Expression *arrayOf;
    Expression *indexInto;
};

#include "ArrayExpr.n"

#endif
