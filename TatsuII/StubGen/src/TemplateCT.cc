/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TemplateCT.cc,v 1.1 1997/06/09 19:52:24 nmcl Exp $
 */

/*
 * Stub generator template class type definition handler
 *
 */

#include <strstream>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef TEMPLATECT_H_
#  include "TemplateCT.h"
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

static const char RCSid[] = "$Id: TemplateCT.cc,v 1.1 1997/06/09 19:52:24 nmcl Exp $";

/*
 * PUBLIC operations
 */

TemplateClassType::TemplateClassType ( const String& name, ClassSpecifier ckind,
				       BaseClass *bases, TemplateContext *args,
				       DeclContext *parent,
				       Boolean nameless, Boolean willBeDefn )
                                     : ClassType(name,ckind,bases,
						 parent,
						 nameless,willBeDefn),
				       templateArgs(args)
{
#ifdef SG_DEBUG
    debugStream << "TemplateClassType::TemplateClassType(" << (void *)this 
		<< "," << name << "," << (void *)args << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);

#endif

    DeclContext *classContext = ClassType::getMemberContext();
    
    templateArgs->setContext(classContext);
    
}

TemplateClassType::~TemplateClassType ()
{
}

DeclContext *TemplateClassType::getMemberContext () const
{
    return templateArgs;
}

String TemplateClassType::preName ( DeclStyle ds, Boolean elab, 
				    Boolean noConst ) const
{
    String root = ClassType::preName(ds,elab,noConst);
    
    root += constructArgList();
    
    return root;
}

String TemplateClassType::simpleTypeName ( DeclStyle ds ) const
{
    String root = ClassType::simpleTypeName(ds);
    
//    root += constructArgList();
    
    return root;
}

String TemplateClassType::typeName ( DeclStyle ds, Boolean elab, 
				     Boolean qualify ) const
{
    String root = ClassType::preName(ds,elab,qualify);
    
    root += constructArgList();
    
    return root;
}

ostream& TemplateClassType::printTemplatePrefix ( ostream& s )
{
    Declaration *nextArg;
    int limit = templateArgs->getEntryCount();
    int i = 0;
    
    s << "template <" ;

    while ((nextArg = templateArgs->getTemplateEntry(i++)))
    {
	const char *sep = ( i < limit ? ", " : "");
	nextArg->printDecl(s, NORMAL_DECL, sep, FALSE, FALSE, TRUE);
    }
    
    s << "> ";

    return s;
}

String TemplateClassType::constructArgList () const
{
    Declaration *decl = 0;
    int i = 0;
    int limit = templateArgs->getEntryCount();
    String root = "<";

    while ((decl = templateArgs->getTemplateEntry(i++)) != 0)
    {
	root += decl->getName();
	if (i < limit)
	    root += ", ";
	
    }
    
    root += ">";

    return root;
}
