/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VoidT.h,v 1.9 1993/06/17 15:08:38 ngdp Exp $
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

    virtual TypeDescriptor *combineTypes (TypeDescriptor *);

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, 
			    Boolean = FALSE, Boolean = FALSE) const;

    virtual String signature () const;

    virtual Boolean isTransferable () const;

};

#endif
