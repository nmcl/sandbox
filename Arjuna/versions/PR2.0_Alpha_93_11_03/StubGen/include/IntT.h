/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntT.h,v 1.1 1993/11/03 14:45:23 nmcl Exp $
 */

#ifndef INTT_H_
#define INTT_H_

/*
 *
 * Stub generator integer type class. Redefines inherited operations 
 * as appropriate for ints.
 *
 */

#ifndef INTEGRALT_H_
#  include "IntegralT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;
class TypeDescriptor;

class IntType : public IntegralType
{
public:
    IntType ();
    virtual ~IntType ();

    virtual String preName (DeclStyle  = NORMAL_DECL, 
			    Boolean = FALSE, Boolean = FALSE) const;
    virtual String signature () const;
};

#endif
