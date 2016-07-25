/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TemplateCIT.h,v 1.1 1993/06/17 15:08:25 ngdp Exp $
 */

#ifndef TEMPLATECIT_H_
#define TEMPLATECIT_H_

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
#  include "TemplateCT.h"
#endif

class TemplateContext;

class TemplateClassInstType : public TemplateClassType
{
public:
    TemplateClassInstType (const String&, ClassType *, DeclarationList *, 
			   DeclContext * );
    
    ~TemplateClassInstType ();

    virtual Boolean checkClassProps (StubModifier);
    virtual DeclContext *getMemberContext () const;

private:

    ClassType *templateMaster;
};

#endif
