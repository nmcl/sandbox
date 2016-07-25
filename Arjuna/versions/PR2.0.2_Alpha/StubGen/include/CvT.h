/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvT.h,v 1.13 1993/06/17 15:07:09 ngdp Exp $
 */

#ifndef CVT_H_
#define CVT_H_

/*
 *
 * Stub generator const/volatile type class. Collects togethor qualifiers for
 * types (const etc.). Behaves as a type if no real type
 * (char, int, etc) is detected in a declaration.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef CVQUALS_H_
#  include "CvQuals.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPEMODS_H_
#  include "TypeMods.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;
     
class CvType : public TypeDescriptor
{
public:
    CvType (CvQualifier);
    CvType (TypeDescriptor *);
    CvType ();
    virtual ~CvType ();

    virtual Boolean addCvQualifier (CvQualifier);
    virtual Boolean addStubModifier (StubModifier);
    virtual Boolean addTypeModifier (TypeModifier);

    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
    virtual TypeDescriptor *deref () const;
    virtual Boolean freezeType ();

    virtual Boolean isConstant () const;
    virtual Boolean isFundamental () const;

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;
    virtual String signature () const;

    virtual StubModifier defaultTransfer () const;
    virtual Boolean isTransferable () const;


protected:
    Boolean isConst;
    Boolean isVolatile;

    TypeDescriptor *cvType;
};

#endif
