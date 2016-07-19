/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FloatT.h,v 1.8 1993/04/14 13:45:42 ngdp Exp $
 */

#ifndef FLOATT_H_
#define FLOATT_H_

/*
 *
 * Stub generator float & double type class. Redefines inherited operations 
 * as appropriate for floats.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef TYPEMODS_H_
#  include "TypeMods.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;

class FloatType : public ModifiedType
{
public:
    FloatType (Boolean = FALSE);
    virtual ~FloatType ();

    virtual void add_type_modifier (TypeModifier);

    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual String signature () const;
    virtual String typename (DeclStyle) const;

private:
    Boolean is_double;
};

#endif
