/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DclContxt.cc,v 1.8 1993/03/22 09:30:48 ngdp Exp $
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

#include <strstream.h>
#include <string.h>

#ifdef DEBUG
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

static const char RCSid[] = "$Id: DclContxt.cc,v 1.8 1993/03/22 09:30:48 ngdp Exp $";
     
/*
 * Public constructor and destructor
 */

DeclContext::DeclContext ( DeclContext *parent, int initialsize )
                         : enclosing_context(parent),
			   entrycount(0),
			   maxentries(initialsize),
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

/*
 * Virtual public operations
 */


Boolean DeclContext::is_applicable ( StubModifier sm ) const
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

Declaration *DeclContext::lookfor ( const String& name,
				    Type type,
				    Context lookin ) const
{
    return find(name, type, lookin);
}

Boolean DeclContext::putin ( Declaration *d )
{
    if (d != 0)
    {
	register long hashcode;
	String name;

	name = d->get_name();

#ifdef DEBUG
	debug_stream << "DeclContext::putin(" << (void *)this;
	debug_stream << ", " << name << ")\n";
	_debug.dbg_flush(FUNCTIONS, FAC_SYMTAB, VIS_PUBLIC);
#endif

	d->set_context(this);

	hashcode = hash(name);
	if (symtable[hashcode] == 0)
	{
	    symtable[hashcode] = new DeclarationList(d);
	}
	else
	    symtable[hashcode]->push_decl(d);

	order[entrycount++] = d;

	/*
	 * If order table is full re-allocate a bigger one, and 
	 * copy them
         */

	if (entrycount == maxentries)
	{
	    Declaration **temp = new Declaration *[maxentries*=2];

	    for (int i = 0; i < maxentries; i++)
		if (i < entrycount)
		    temp[i] = order[i];
		else
		    temp[i] = 0;

	    delete order;
	    order = temp;
	}
	return TRUE;
    }
    return FALSE;
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
    register long hashcode = hash(name);
    register DeclarationList *list = symtable[hashcode];
    
    if (list != 0)
    {
	DeclarationListIterator next(list);
	register Declaration *entry;
	String listname;
	
	while (entry = next())
	{
	    listname = entry->get_name();
	    if ((listname == name) &&
		((type == ANY_TYPE) || (entry->kind() == type)))
		    return entry;
	}
    }

    if ( (lookin == ANY_CONTEXT) && (enclosing_context != 0))
	return (enclosing_context->lookfor(name, type, lookin));
    else
	return 0;
}

long DeclContext::hash ( const String& name ) const
{
    return hashit(name) % prime;
}

#ifdef NO_INLINES
#  define DCLCONTXT_CC_
#  include "DclContxt.n"
#  undef DCLCONTXT_CC_
#endif
