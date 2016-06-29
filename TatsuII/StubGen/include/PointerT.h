/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PointerT.h,v 1.1 1997/06/09 19:52:17 nmcl Exp $
 */

#ifndef POINTERT_H_
#define POINTERT_H_

/*
 *
 * Stub generator pointer handler.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

class ostream;

class PointerType : public TypeDescriptor
{
public:
    PointerType (Type, TypeDescriptor * = 0, TypeDescriptor * = 0);
    virtual ~PointerType ();

    /* virtual functions */

    virtual Boolean addStubModifier (StubModifier);
    virtual Boolean attachBaseType (TypeDescriptor *);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
    virtual TypeDescriptor *deref ();

    virtual String defaultInit () const;
    virtual Boolean deleteable () const;
    virtual Boolean freezeType ();
    virtual Boolean isFundamental () const;
    virtual Boolean overrideStubModifiers (StubModifier);
    
    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;

    virtual String signature () const;

    /* Stub generation specifics */

    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;
    virtual Boolean isTransferable () const;
    
    virtual Boolean produceMarshallingCode (ostream&, DeclStyle, Direction,
					    const String&, const String&);

private:
    TypeDescriptor *pointerQuals;	/* const/volatile */
    TypeDescriptor *pointerTo;		/* type pointed to */
    StubModifier pointerTransfer;
    Boolean deletePointer;
};

#endif
