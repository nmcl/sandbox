/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VecT.h,v 1.9 1993/04/14 13:45:55 ngdp Exp $
 */

#ifndef VECT_H_
#define VECT_H_

/*
 *
 * Stub generator vector type class definition.
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


class Expression;
class ostream;

class VecType : public TypeDescriptor
{
public:
    VecType (Expression *, TypeDescriptor *);
    virtual ~VecType ();

    virtual void attach_base_type (TypeDescriptor *);
    virtual TypeDescriptor *combine_types (TypeDescriptor *);

    virtual StubModifier default_par_transfer () const;
    virtual Boolean is_fundamental () const;
    virtual Boolean is_transmissible () const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);

    virtual void print_marshalling_code (ostream&, const String&,
					 const  String&);
    virtual void print_unmarshalling_code (ostream&, const String&,
					   const String&);

    virtual String signature () const;
    virtual String typename (DeclStyle) const;

private:
    String indexname ();		/* generate new index var name */

    Expression *vector_size;		/* basic size of vector */
    TypeDescriptor *vector_of;		/* is a vector of this type */
};

#endif
