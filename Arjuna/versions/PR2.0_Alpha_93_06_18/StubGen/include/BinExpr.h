/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BinExpr.h,v 1.2 1993/03/22 09:26:44 ngdp Exp $
 */

#ifndef BINEXPR_H_
#define BINEXPR_H_

/*
 *
 * Simple expression handling. Used as the basis for initial values.
 * This is a binary expression handler. Used for unary expressions too
 * by setting either left or right to Null.
 *
 */

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

class ostream;

class BinaryExpression : public Expression
{
public:
    BinaryExpression (const String&, Expression*, Expression*);

    virtual ostream& printExpr (ostream&);
private:
    String exprOperator;
    Expression *leftOperand;
    Expression *rightOperand;

};

#include "BinExpr.n"

#endif
