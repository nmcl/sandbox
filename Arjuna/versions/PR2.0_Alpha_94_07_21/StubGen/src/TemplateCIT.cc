/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TemplateCIT.cc,v 1.1 1993/06/17 15:09:55 ngdp Exp $
 */

/*
 * Stub generator template class type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef TEMPLATECIT_H_
#  include "TemplateCIT.h"
#endif

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLLIST_H_
#  include "DeclList.h"
#endif

#ifndef DECLLISTI_H_
#  include "DeclListI.h"
#endif

#ifndef TMPLCNTXT_H_
#  include "TmplCntxt.h"
#endif

static const char RCSid[] = "$Id: TemplateCIT.cc,v 1.1 1993/06/17 15:09:55 ngdp Exp $";

/*
 * PUBLIC operations
 */

TemplateClassInstType::TemplateClassInstType ( const String& name, 
					       ClassType *isInstanceOf,
					       DeclarationList *args,
					       DeclContext *parent )
                                             : TemplateClassType(name,
								 CLASS_TYPE,
								 0,
								 args,
								 parent),
					       templateMaster(isInstanceOf)
{
#ifdef DEBUG
    debugStream << "TemplateClassInstType::TemplateClassInstType(" << (void *)this 
		<< "," << name << "," << (void *)args << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);

#endif

}

TemplateClassInstType::~TemplateClassInstType ()
{
}

Boolean TemplateClassInstType::checkClassProps ( StubModifier sm)
{
    return templateMaster->checkClassProps(sm);
}

DeclContext *TemplateClassInstType::getMemberContext () const
{
    return templateMaster->getMemberContext();
}
