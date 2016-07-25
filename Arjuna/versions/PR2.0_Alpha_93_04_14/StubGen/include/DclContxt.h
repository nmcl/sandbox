/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DclContxt.h,v 1.6 1993/03/22 09:26:55 ngdp Exp $
 */

#ifndef DCLCONTXT_H_
#define DCLCONTXT_H_

/*
 *
 * This class implements a flat scoped symbol table suitable for the 
 * global tables and the tables needed in classes and structs.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STUBMODS_H_
#  include "StubMods.h"
#endif

#ifndef TYPE_H_
#  include "Type.h"
#endif

class Declaration;
class DeclarationList;
class String;

enum Context { CURRENT_CONTEXT, CLASS_CONTEXT, INHERITED_CONTEXT,
	       ANY_CONTEXT };

class DeclContext
{
public:
    /* Constructors and destructors */

    DeclContext (DeclContext * = 0, int = 211);
    virtual ~DeclContext ();

    /* non-virtual interface */

    DeclContext *get_enclosing_context () const;
    Declaration *get_member (int) const;
    int get_no_of_entries () const;

    /* virtual interface */

    virtual Boolean is_applicable (StubModifier) const;
    virtual Declaration *lookfor (const String&, Type, Context) const;
    virtual Boolean putin (Declaration *);

protected:
    Declaration *find (const String&, Type, Context) const;
    DeclarationList **get_symtable () const;
    long hash (const String&) const;	/* hashing function */
    

private:
    /* private state */

    DeclContext *enclosing_context;	/* Surrounding context */
    int entrycount;			/* count of entries */
    int maxentries;
    Declaration **order;		/* tracks order symbols inserted */
    int prime;				/* used in hash calculation */
    DeclarationList **symtable;		/* the actual table */
    int tablesize;			/* size of hash table */
};

#include "DclContxt.n"

#endif
