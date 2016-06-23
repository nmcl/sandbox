/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArrayExpr.h,v 1.1 1997/09/25 15:29:27 nmcl Exp $
 */

#ifndef ARRAYEXPR_H_
#define ARRAYEXPR_H_

/*
 *
 * Simple expression handling. Represent array reference expressions
 *
 */

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
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
