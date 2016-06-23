/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DclContxt.cc,v 1.1 1997/09/25 15:30:45 nmcl Exp $
 */

/*
 * Declaration context class for stub generator. A context holds all 
 * symbols declared within a particular scope (eg file, class, function, etc.)
 * Contexts are chained togethor with file level representing the outermost
 * level.
 * If a name cannot be found in the current context the lookup may be
 * continued in the enclosing one.
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#include <string.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
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

static const char RCSid[] = "$Id: DclContxt.cc,v 1.1 1997/09/25 15:30:45 nmcl Exp $";
     
/*
 * Public constructor and destructor
 */

DeclContext::DeclContext ( DeclContext *parent, int initialsize )
                         : enclosingContext(parent),
			   entryCount(0),
			   contextFrozen(FALSE),
			   maxEntries(initialsize),
			   prime(initialsize),
			   tablesize(initialsize)
{
    symtable = new DeclarationList *[tablesize];
    order = new Declaration *[tablesize];
    for (int i = 0; i < tablesize; i++)
    {
	symtable[i] = 0;
	order[i] = 0;
    }
}

DeclContext::~DeclContext ()
{
    for (int i = 0; i < tablesize; i++)
    {
	delete symtable[i];
    }
    delete [] symtable;
    delete [] order;
}

String DeclContext::getContextName ( DeclStyle ds ) const
{
    String s1;

    if (enclosingContext)
	s1 = enclosingContext->getContextName(ds);

    s1 += getSimpleContextName(ds);

    if (s1 != NullString)
	s1 += "::";

    return s1;
}

/*
 * Virtual public operations
 */


void DeclContext::freeze ()
{
    contextFrozen = TRUE;
}

String DeclContext::getSimpleContextName ( DeclStyle ) const
{
    return NullString;
}

Boolean DeclContext::isApplicable ( StubModifier sm ) const
{
    switch (sm)
    {
    case STUB_NORENAME:
    case STUB_REMOTE:
    case STUB_NOREMOTE:
	return TRUE;
    default:
	return FALSE;
    }
}

Declaration *DeclContext::lookFor ( const String& name,
				    Type type,
				    Context lookin ) const
{
    return find(name, type, lookin);
}

Boolean DeclContext::putIn ( Declaration *d )
{
    if (contextFrozen)
	return FALSE;
    
    if (d != 0)
    {
	register long hashcode;
	String name;

	name = d->getName();

#ifdef SG_DEBUG
	debugStream << "DeclContext::puTin(" << (void *)this;
	debugStream << ", " << name << ")\n";
	_debug.dbgFlush(FUNCTIONS, FAC_SYMTAB, VIS_PUBLIC);
#endif

	d->setContext(this);

	hashcode = hash(name);
	if (symtable[hashcode] == 0)
	{
	    symtable[hashcode] = new DeclarationList(d);
	}
	else
	    symtable[hashcode]->pushDecl(d);

	order[entryCount++] = d;

	/*
	 * If order table is full re-allocate a bigger one, and 
	 * copy them
         */

	if (entryCount == maxEntries)
	{
	    Declaration **temp = new Declaration *[maxEntries*=2];

	    for (int i = 0; i < maxEntries; i++)
		if (i < entryCount)
		    temp[i] = order[i];
		else
		    temp[i] = 0;

	    delete [] order;
	    order = temp;
	}
	return TRUE;
    }
    return FALSE;
}

ostream& DeclContext::printOn ( ostream& s, DeclStyle ds, Boolean noName,
				Boolean noDefault, Boolean noConst,
				const char* sep ) const
{    
    Declaration *nextArg;
    int i = 0;
    
    while ((nextArg = order[i++]))
    {
	if ( i < entryCount)
	    nextArg->printDecl(s, ds, sep, noName, noDefault, noConst);
	else
	    nextArg->printDecl(s, ds, "", noName, noDefault, noConst);
    }
    
    return s;
}

/*
 * Protected operations
 */

/*
 * Look up an entry in a context. We look for an exact match on name and
 * type unless type is ANY_TYPE when the first matching name is used. If
 * no match exists in this scope the search may be propagated to the
 * enclosing context (if one exists) until a match is found or no outer
 * scope exists in which case return NULL. Whether this is done or not
 * is controlled by the setting of the context parameter
 */

Declaration *DeclContext::find ( const String& name,
				 Type type,
				 Context lookin ) const
{
#ifdef SG_DEBUG
    debugStream << "DeclContext::find(" << (void *)this
		<< ", " << name << ", " << type
		<< ", " << lookin << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    register long hashcode = hash(name);
    register DeclarationList *list = symtable[hashcode];
    
    if (list != 0)
    {
	DeclarationListIterator next(list);
	register Declaration *entry;
	String listName;
	
	while ((entry = next()))
	{
	    listName = entry->getName();
	    if ((listName == name) &&
		((type == ANY_TYPE) || (entry->kind() == type)))
		    return entry;
	}
    }

    if ( (lookin == ANY_CONTEXT) && (enclosingContext != 0))
	return (enclosingContext->lookFor(name, type, lookin));
    else
	return 0;
}

long DeclContext::hash ( const String& name ) const
{
    return hashIt(name) % prime;
}

#ifdef NO_INLINES
#  define DCLCONTXT_CC_
#  include "DclContxt.n"
#  undef DCLCONTXT_CC_
#endif
