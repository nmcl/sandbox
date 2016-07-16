/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ModifiedT.h,v 1.1 1997/09/25 15:30:18 nmcl Exp $
 */

#ifndef MODIFIEDT_H_
#define MODIFIEDT_H_

/*
 *
 * Stub generator modified type class. Collects togethor modifiers for
 * basic types (unsigned etc.). Behaves as a type if no real type
 * (char, int, etc) is detected in a declaration.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPEMODS_H_
#  include "TypeMods.h"
#endif

#ifndef TYPE_H_
#  include "Type.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ModifiedType : public TypeDescriptor
{
public:
    ModifiedType (TypeModifier);
    ModifiedType (Type = MODIFIED_TYPE);
    virtual ~ModifiedType ();

    virtual Boolean addStubModifier (StubModifier);
    virtual Boolean addTypeModifier (TypeModifier);

    virtual TypeDescriptor *combineTypes (TypeDescriptor *);

    virtual Boolean canDiscard () const;
    virtual String defaultInit () const;
    virtual Boolean overrideStubModifiers (StubModifier);
    
    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, 
			    Boolean = FALSE, Boolean = FALSE) const;
    virtual String signature () const;

    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;
    virtual Boolean isTransferable () const;

protected:
    Boolean isLong[2];
    Boolean isShort;
    Boolean isSigned;
    Boolean isUnsigned;
};

#endif
