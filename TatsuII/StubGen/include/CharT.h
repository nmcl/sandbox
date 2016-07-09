/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CharT.h,v 1.1 1997/06/09 19:52:08 nmcl Exp $
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

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif


class CharType : public ModifiedType
{
public:
    CharType (Boolean= FALSE);
    virtual ~CharType ();

    virtual Boolean addTypeModifier (TypeModifier);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);

    virtual String preName (DeclStyle  = NORMAL_DECL, 
			    Boolean = FALSE, Boolean = FALSE) const;
    virtual String signature () const;
private:
    Boolean wide;
    String charTypeName;
};

#endif
