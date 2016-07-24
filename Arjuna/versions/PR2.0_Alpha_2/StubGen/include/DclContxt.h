/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DclContxt.h,v 1.1 1993/11/03 14:44:27 nmcl Exp $
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

#ifndef DECLSTYLE_H_
#  include "DeclStyle.h"
#endif

class ostream;

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

    DeclContext *getEnclosingContext () const;
    Declaration *getOneMember (int) const;
    int noOfEntries () const;

    /* virtual interface */

    virtual void freeze ();
    virtual Boolean isApplicable (StubModifier) const;
    virtual Declaration *lookFor (const String&, Type, Context) const;
    virtual Boolean putIn (Declaration *);

    virtual ostream& printOn (ostream&, DeclStyle = NORMAL_DECL, 
			      char* = ", ") const;

protected:
    Declaration *find (const String&, Type, Context) const;
    DeclarationList **getSymtable () const;
    long hash (const String&) const;	/* hashing function */
    

private:
    /* private state */

    DeclContext *enclosingContext;	/* Surrounding context */
    int entryCount;			/* count of entries */
    Boolean contextFrozen;
    int maxEntries;
    Declaration **order;		/* tracks order symbols inserted */
    int prime;				/* used in hash calculation */
    DeclarationList **symtable;		/* the actual table */
    int tablesize;			/* size of hash table */
};

inline ostream& operator<< ( ostream& s, const DeclContext& dc )
{
    return dc.printOn(s);
}

#include "DclContxt.n"

#endif
