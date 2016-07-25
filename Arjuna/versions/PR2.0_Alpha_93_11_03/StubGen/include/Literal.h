/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Literal.h,v 1.1 1993/11/03 14:45:32 nmcl Exp $
 */

#ifndef LITERAL_H_
#define LITERAL_H_

/*
 *
 * Simple expression handling. Represent literals (strings, int consts
 * etc as expressions so they can handled by the expression code
 *
 */

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

class ostream;

class Literal : public Expression
{
public:
    Literal (const String&);

    virtual ostream& printExpr (ostream&);
private:
    String theLiteral;
};

#include "Literal.n"

#endif
