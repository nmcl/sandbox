/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CharT.h,v 1.1 1993/11/03 14:44:17 nmcl Exp $
 */

#ifndef CHART_H_
#define CHART_H_

/*
 *
 * Stub generator char type class. Redefines inherited operations
 * as appropriate for chars.
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

#ifndef INTEGRALT_H_
#  include "IntegralT.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;

class CharType : public IntegralType
{
public:
    CharType ();
    virtual ~CharType ();

    virtual Boolean addTypeModifier (TypeModifier);

    virtual String preName (DeclStyle  = NORMAL_DECL, 
			    Boolean = FALSE, Boolean = FALSE) const;
    virtual String signature () const;
};

#endif
