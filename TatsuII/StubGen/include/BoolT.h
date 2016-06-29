/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BoolT.h,v 1.1 1997/06/09 19:52:08 nmcl Exp $
 */

#ifndef BOOLT_H_
#define BOOLT_H_

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

class BoolType : public TypeDescriptor
{
public:
    BoolType ();
    virtual ~BoolType ();

    virtual Boolean addStubModifier (StubModifier);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, 
			    Boolean = FALSE, Boolean = FALSE) const;

    virtual String signature () const;
    
    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;
    virtual Boolean isTransferable () const;
};

#endif
