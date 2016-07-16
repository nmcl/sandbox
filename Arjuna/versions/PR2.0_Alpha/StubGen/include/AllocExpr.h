/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AllocExpr.h,v 1.3 1993/03/22 09:26:40 ngdp Exp $
 */

#ifndef ALLOCEXPR_H_
#define ALLOCEXPR_H_

/*
 *
 * Simple expression handling. Represent allocation expressions
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

class ostream;

class AllocExpression : public Expression
{
public:
    AllocExpression (const Boolean&, Expression *, Expression *, Expression *);

    virtual ostream& printExpr (ostream&);
private:
    Boolean global;
    Expression *init;
    Expression *newOn;
    Expression *placement;

};

#include "AllocExpr.n"

#endif
