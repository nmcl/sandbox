/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Literal.h,v 1.1 1997/09/25 15:30:14 nmcl Exp $
 */

#ifndef LITERAL_H_
#define LITERAL_H_

/*
 *
 * Simple expression handling. Represent literals (strings, int consts
 * etc as expressions so they can handled by the expression code
 *
 */

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

class ostream;

class Literal : public Expression
{
public:
    Literal (const String&);

    virtual ostream& printExpr (ostream&) const;
private:
    String theLiteral;
};

#include "Literal.n"

#endif
