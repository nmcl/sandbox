/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Expression.h,v 1.6 1993/03/22 09:27:15 ngdp Exp $
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

/*
 *
 * Simple expression handling. Used as the base class for handling of
 * all basic expressions
 */

class ostream;

class Expression
{
public:
    Expression ();

    virtual ostream& printExpr (ostream&) = 0;
};

#include "Expression.n"

#endif
