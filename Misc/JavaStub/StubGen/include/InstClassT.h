/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InstClassT.h,v 1.3 1995/03/20 15:14:06 ngdp Exp $
 */

#ifndef INSTCLASST_H_
#define INSTCLASST_H_

/*
 *
 * Stub generator template class instantiation handler.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

class TemplateContext;

class InstantiatedClassType : public ClassType
{
public:
    InstantiatedClassType (const String&, ClassType *,
			   DeclarationList *, DeclContext *);
    ~InstantiatedClassType ();

    virtual DeclContext *getMemberContext () const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;
    virtual String typeName (DeclStyle, Boolean = FALSE, Boolean = FALSE) const;

private:

    String constructArgList () const;
    
    TemplateContext *templateArgs;
};

#endif