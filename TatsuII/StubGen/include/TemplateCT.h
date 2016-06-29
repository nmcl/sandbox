/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TemplateCT.h,v 1.1 1997/06/09 19:52:17 nmcl Exp $
 */

#ifndef TEMPLATECT_H_
#define TEMPLATECT_H_

/*
 *
 * Stub generator template class definition handler.
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

class TemplateClassType : public ClassType
{
public:
    TemplateClassType (const String&, ClassSpecifier, BaseClass *,
		       TemplateContext *, DeclContext *, 
		       Boolean = FALSE, Boolean = FALSE);
    ~TemplateClassType ();

    virtual DeclContext *getMemberContext () const;

    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;
    virtual String simpleTypeName (DeclStyle) const;
    virtual String typeName (DeclStyle, Boolean = FALSE, Boolean = FALSE) const;

protected:
    virtual ostream& printTemplatePrefix (ostream&);

private:

    String constructArgList () const;
    
    TemplateContext *templateArgs;
};

#endif
