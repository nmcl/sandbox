/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvT.h,v 1.12 1993/04/14 13:45:35 ngdp Exp $
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

    virtual void add_type_modifier (TypeModifier);
    virtual void add_cv_qualifier (CvQualifier);
    virtual void add_stub_modifier (StubModifier);

    virtual TypeDescriptor *combine_types (TypeDescriptor *);

    virtual StubModifier default_par_transfer () const;

    virtual TypeDescriptor *deref () const;

    virtual void freeze_type ();

    virtual Boolean is_constant () const;
    virtual Boolean is_fundamental () const;
    virtual Boolean is_transmissible () const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual int priority () const;

    virtual void print_marshalling_code ( ostream&, const String&,
					  const  String&);
    virtual void print_unmarshalling_code ( ostream&, const String&,
					    const  String&);

    virtual String signature () const;
    virtual String typename (DeclStyle) const;

protected:
    Boolean is_const;
    Boolean is_volatile;

    TypeDescriptor *cv_type;
};

#endif
