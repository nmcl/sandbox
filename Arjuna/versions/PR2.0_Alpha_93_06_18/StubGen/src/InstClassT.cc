/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InstClassT.cc,v 1.1 1993/06/18 07:42:16 ngdp Exp $
 */

/*
 * Stub generator template class type instantiation handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef INSTCLASST_H_
#  include "InstClassT.h"
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

static const char RCSid[] = "$Id: InstClassT.cc,v 1.1 1993/06/18 07:42:16 ngdp Exp $";

/*
 * PUBLIC operations
 */

InstantiatedClassType::InstantiatedClassType ( const String& name, 
					       ClassType *isInstanceOf, 
					       DeclarationList *args,
					       DeclContext *parent )
                                             : ClassType(name,
							 CLASS_TYPE,
							 0,
							 parent),
					       templateArgs(0)
{
#ifdef DEBUG
    debugStream << "InstantiatedClassType::InstantiatedClassType(" 
		<< (void *)this 
		<< "," << name << "," << (void *)args << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);

#endif

    DeclContext *classContext = ClassType::getMemberContext();
    DeclarationListIterator next(args);
    Declaration *nextArg;
    
    templateArgs = new TemplateContext(classContext);
    
    while (nextArg = next())
    {
	templateArgs->putIn(nextArg);
    }
    templateArgs->freeze(); 
    combineTypes(isInstanceOf);
}

InstantiatedClassType::~InstantiatedClassType ()
{
}

String InstantiatedClassType::preName ( DeclStyle ds, Boolean elab, 
				    Boolean noConst ) const
{
    String root = ClassType::preName(ds,elab,noConst);
    
    root += constructArgList();
    
    return root;
}

String InstantiatedClassType::constructArgList () const
{
    Declaration *decl = 0;
    int i = 0;
    int limit = templateArgs->getEntryCount();
    String root = "<";

    while ((decl = templateArgs->getTemplateEntry(i++)) != 0)
    {
	char buffer[1024];
	ostrstream os(buffer,1024);

	decl->printDecl(os, NORMAL_DECL, "");
	os << ends;
	
	root += buffer;
	
	if (i < limit)
	    root += ", ";
	
    }
    
    root += ">";

    return root;
}
