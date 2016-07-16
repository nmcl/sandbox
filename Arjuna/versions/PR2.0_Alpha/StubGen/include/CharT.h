/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CharT.h,v 1.8 1993/04/14 13:45:32 ngdp Exp $
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

    virtual void add_type_modifier (TypeModifier);

    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual String signature () const;
    virtual String typename (DeclStyle) const;
};

#endif
