/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TDesc.h,v 1.14 1993/04/14 13:45:52 ngdp Exp $
 */

#ifndef TDESC_H_
#define TDESC_H_

/*
 *
 * Stub generator basic type attribute class. Used ONLY as a base class for
 * the 'real' type defining classes. Provides the basic definitions of all
 * the functions required in stub generation.
 * Operations are virtual and should be (in general) redifined in all the
 * derived types appropriately.
 * Instances of this class cannot be created explicitly - its only
 * constructor is protected to enforce this. In addition some operations
 * are declared pure virtual where the default behaviour is likely to
 * be problematical to define.
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

#ifndef FUNCMODS_H_
#  include "FuncMods.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPE_H_
#  include "Type.h"
#endif

#ifndef TYPEMODS_H_
#  include "TypeMods.h"
#endif

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

#ifndef DIRECTION_H_
#  include "Direction.h"
#endif

class Expression;
class ostream;

class TypeDescriptor
{   
public:
    virtual ~TypeDescriptor ();

    /* Non-virtual functions */

    void marshall(ostream&, Direction, DeclStyle, const String&, const String&);

    /* public virtual interface */

    virtual void add_body (Expression *);
    virtual void add_func_modifier (FuncModifier);
    virtual void add_name(const String&);
    virtual void add_stub_modifier (StubModifier);
    virtual void add_type_modifier (TypeModifier);
    virtual void add_cv_qualifier (CvQualifier);
    virtual void attach_base_type (TypeDescriptor *);

    virtual void change_visibility (VisibilitySpec);
    virtual TypeDescriptor *combine_types (TypeDescriptor *) = 0;

    virtual StubModifier default_location () const;
    virtual StubModifier default_marshall () const;
    virtual StubModifier default_par_transfer () const;
    virtual TypeDescriptor *deref () const;

    virtual void freeze_type ();

    virtual Boolean is_constant () const;
    virtual Boolean is_fundamental () const;
    virtual Boolean is_pure () const;
    virtual Boolean is_transmissible () const = 0;

    virtual Type kind () const;

    virtual void make_complete ();
    virtual void make_pure ();
    virtual String massage_name (const String&, DeclStyle) const;

    virtual Boolean needs_sep () const;

    virtual void postname (ostream&, DeclStyle) = 0;
    virtual void prename (ostream&, DeclStyle) = 0;
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean) = 0;

    virtual Boolean print_code (ostream&, DeclStyle);
    virtual void printDefaultInit (ostream&) const;

    virtual void print_instance (ostream&, const String&, DeclStyle,
				 Boolean, Boolean);
    virtual void print_marshalling_code (ostream&, const String&,
					 const String&);
    virtual Boolean print_marshall_defs (ostream&, DeclStyle);
    virtual void print_typedef (ostream&, DeclStyle);
    virtual void print_unmarshalling_code (ostream&, const String&,
					   const String&);

    virtual void produce_marshall_defs (ostream&);
    virtual Boolean produce_stub (ostream&, DeclStyle);
    virtual void produce_stubs (ostream&);

    virtual String signature () const = 0;
    virtual Boolean stub_possible () const;
    virtual String typename (DeclStyle) const = 0;
    
protected:
    TypeDescriptor (Type);

private:
    Type is_kind_of;
};

#include "TDesc.n"

#endif
