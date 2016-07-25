/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeDefT.h,v 1.10 1993/04/14 13:45:53 ngdp Exp $
 */

#ifndef TYPEDEFT_H_
#define TYPEDEFT_H_

/*
 *
 * Stub generator class for forward references and typedefs.
 * Handles declarations of the form:
 *    class fred;
 *    class X { fred af; };
 *    ...
 *    class fred {...};
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

class TypeDefType : public TypeDescriptor
{
public:
    TypeDefType (const String&, TypeDescriptor *);
    virtual ~TypeDefType ();

    virtual TypeDescriptor *combine_types (TypeDescriptor *);
    virtual TypeDescriptor *deref () const;
    virtual Boolean is_fundamental () const;
    virtual Boolean is_transmissible () const;

    virtual String signature () const;
    virtual String typename (DeclStyle) const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual void printDefaultInit (ostream&) const;
    virtual void print_typedef (ostream&, DeclStyle);

    virtual void print_marshalling_code (ostream&, const String&,
					 const  String&);
    virtual void print_unmarshalling_code (ostream&, const String&,
					   const String&);

private:
    TypeDescriptor *real_type;
    String typedefname;
};

#endif
