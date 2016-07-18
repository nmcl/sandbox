/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VoidT.h,v 1.8 1993/04/14 13:45:56 ngdp Exp $
 */

#ifndef VOIDT_H_
#define VOIDT_H_

/*
 *
 * Stub generator void type class. Redefines inherited operations 
 * as appropriate for voids.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;

class VoidType : public TypeDescriptor
{
public:
    VoidType ();
    virtual ~VoidType ();

    virtual TypeDescriptor *combine_types (TypeDescriptor *);
    virtual Boolean is_transmissible () const;
    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual String signature () const;
    virtual String typename (DeclStyle) const;
};

#endif
