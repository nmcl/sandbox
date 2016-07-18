/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntT.h,v 1.8 1993/04/14 13:45:46 ngdp Exp $
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

    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual String signature () const;
    virtual String typename (DeclStyle) const;
};

#endif
