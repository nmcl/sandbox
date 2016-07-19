/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: EnumT.h,v 1.9 1993/04/14 13:45:40 ngdp Exp $
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

    virtual TypeDescriptor *combine_types (TypeDescriptor *);

    virtual Boolean is_transmissible () const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual void print_typedef (ostream&, DeclStyle);

    virtual void produce_stubs (ostream&);
    virtual Boolean stub_possible () const;
    
    virtual String signature () const;
    virtual String typename (DeclStyle) const;

private:
    static long enum_count;
    Boolean enum_is_anon;
    Boolean enum_is_complete;
    String enum_name;			/* name of this type */
    DeclarationList *enumerators;	/* list of enumerations */
    

};

#endif
