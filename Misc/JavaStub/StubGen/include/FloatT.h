/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FloatT.h,v 1.2 1994/01/06 12:06:45 ngdp Exp $
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

    virtual Boolean addTypeModifier (TypeModifier);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);

    virtual String preName (DeclStyle = NORMAL_DECL,
			    Boolean = FALSE, Boolean = FALSE) const;
    virtual String signature () const;

private:
    Boolean isDouble;
};

#endif
