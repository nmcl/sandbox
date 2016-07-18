/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QualT.h,v 1.9 1993/04/14 13:45:51 ngdp Exp $
 */

#ifndef QUALT_H_
#define QUALT_H_

/*
 *
 * Stub generator class for qualified types (eg x::y)
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

class QualifiedType : public TypeDescriptor
{
public:
    QualifiedType (TypeDescriptor *, TypeDescriptor *);
    virtual ~QualifiedType ();

    virtual void add_body (Expression *);
    virtual void add_func_modifier (FuncModifier);
    virtual TypeDescriptor *combine_types (TypeDescriptor *);
    virtual TypeDescriptor *deref () const;
    virtual Boolean is_transmissible () const;
    virtual String massage_name (const String&, DeclStyle) const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual void print_typedef (ostream&, DeclStyle);
    virtual void print_unmarshalling_code (ostream&,
					   const String&, const String&);

    virtual String signature () const;
    virtual String typename (DeclStyle) const;
private:
    TypeDescriptor *qualified_type;
    TypeDescriptor *qualifying_type;
};

#endif
 
