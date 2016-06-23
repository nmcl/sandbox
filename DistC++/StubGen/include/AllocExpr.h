/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AllocExpr.h,v 1.1 1997/09/25 15:29:25 nmcl Exp $
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

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

class ostream;

class AllocExpression : public Expression
{
public:
    AllocExpression (const Boolean&, Expression *, Expression *, Expression *);

    virtual ostream& printExpr (ostream&) const;
private:
    Boolean global;
    Expression *init;
    Expression *newOn;
    Expression *placement;

};

#include "AllocExpr.n"

#endif
