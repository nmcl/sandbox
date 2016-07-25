/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: EnumT.h,v 1.10 1993/06/17 15:07:38 ngdp Exp $
 */

#ifndef ENUMT_H_
#define ENUMT_H_

/*
 *
 * Stub generator enumerated types handler. Holds information
 * about any particular enumerated type.
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

class DeclarationList;
class ostream;

class EnumType : public TypeDescriptor
{
public:
    EnumType (const String&, DeclarationList *, Boolean = TRUE);
    virtual ~EnumType ();

    /* virtual functions */

    virtual Boolean addStubModifier (StubModifier);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
    virtual Boolean freezeType ();

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;
    virtual String signature () const;


    /* Stub generation specifics */

    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;
    virtual Boolean isTransferable () const;
    virtual Boolean produceAllStubs (ostream&);
    virtual Boolean stubIsPossible () const;
    
    /* Type printing */

    virtual ostream& printAsTypedef (ostream&, DeclStyle);

private:
    static long enumCount;
    Boolean enumFrozen;
    Boolean enumIsAnon;
    Boolean enumIsComplete;
    String enumName;			/* name of this type */
    DeclarationList *enumerators;	/* list of enumerations */
    StubModifier marshallOpts;

};

#endif
