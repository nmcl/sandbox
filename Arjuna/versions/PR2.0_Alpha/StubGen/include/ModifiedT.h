/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ModifiedT.h,v 1.10 1993/04/14 13:45:47 ngdp Exp $
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

#ifndef STRING_H_
#  include "String.h"
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

class ostream;
     
class ModifiedType : public TypeDescriptor
{
public:
    ModifiedType (TypeModifier);
    ModifiedType (Type = INTEGRAL_TYPE);
    virtual ~ModifiedType ();

    virtual void add_type_modifier (TypeModifier);
    virtual void add_stub_modifier (StubModifier);
    virtual TypeDescriptor *combine_types (TypeDescriptor *);
    virtual Boolean is_transmissible () const;

    virtual void postname (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual void printDefaultInit (ostream&) const;
    virtual void prename (ostream&, DeclStyle);
    virtual void print_marshalling_code ( ostream&, const String&,
					  const  String&);
    virtual void print_unmarshalling_code ( ostream&, const String&,
					    const  String&);

    virtual String signature () const;
    virtual String typename (DeclStyle) const;

protected:
    Boolean is_long;
    Boolean is_short;
    Boolean is_signed;
    Boolean is_unsigned;
};

#endif
