/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TmplCntxt.cc,v 1.2 1993/11/02 12:37:02 ngdp Exp $
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

#ifndef TMPLCNTXT_H_
#  include "TmplCntxt.h"
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


static const char RCSid[] = "$Id: TmplCntxt.cc,v 1.2 1993/11/02 12:37:02 ngdp Exp $";

/*
 * Public constructor and destructor
 */

TemplateContext::TemplateContext ( DeclContext *mainCntxt,
				   int templContextSize )
                                 : DeclContext(0,templContextSize),
				   argsFrozen(FALSE),
				   mainContext(mainCntxt)
{
    
}

TemplateContext::~TemplateContext ()
{
}


Declaration *TemplateContext::getTemplateEntry ( int entryWanted ) const
{
    return getOneMember(entryWanted);
}

int TemplateContext::getEntryCount () const
{
    return noOfEntries();  
}

void TemplateContext::setContext ( DeclContext *context ) 
{
    mainContext = context;  
}

/*
 * Virtual public operations
 */

void TemplateContext::freeze ()
{
    if (!argsFrozen)
    {
	argsFrozen = TRUE;
	DeclContext::freeze();
    }
    else
	mainContext->freeze();
    
}

/*
 * Pretend that all stub options are allowed. Hopefully the parser
 * will catch any funnies
 */

Boolean TemplateContext::isApplicable ( StubModifier ) const
{
    return TRUE;
}

Declaration *TemplateContext::lookFor ( const String& name, Type type, 
					Context ctx) const
{
    Declaration *d = find(name, type, CURRENT_CONTEXT);
    
    if (d == 0)
	d = mainContext->lookFor(name, type, ctx);

    return d;
}

Boolean TemplateContext::putIn ( Declaration *d )
{
    if (!argsFrozen)
	return DeclContext::putIn(d);
    else
	return mainContext->putIn(d);
}


ostream& TemplateContext::printOn ( ostream& s, DeclStyle ds, char* sep ) const
{    

    s << "template <";
    
    DeclContext::printOn(s,ds,sep);
    
    s << "> ";
    
    
    return s;
}
