/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PointerT.h,v 1.11 1993/04/14 13:45:49 ngdp Exp $
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

    virtual void attach_base_type (TypeDescriptor *);
    virtual void add_stub_modifier (StubModifier);
    virtual TypeDescriptor *combine_types (TypeDescriptor *);

    virtual StubModifier default_par_transfer () const;

    virtual TypeDescriptor *deref () const;
    virtual Boolean is_fundamental () const;
    virtual Boolean is_transmissible () const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual void printDefaultInit (ostream&) const;
    virtual void print_marshalling_code (ostream&, const String&,
					 const String&);
    virtual void print_unmarshalling_code (ostream&, const String&,
					   const String&);

    virtual String signature () const;
    virtual String typename (DeclStyle) const;

private:
    TypeDescriptor *pointer_quals;	/* const/volatile */
    TypeDescriptor *pointer_to;		/* type pointed to */

};

#endif
