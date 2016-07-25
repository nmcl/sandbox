/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclListE.h,v 1.5 1993/06/17 15:07:23 ngdp Exp $
 */

#ifndef DECLLISTE_H_
#define DECLLISTE_H_

/*
 *
 * Stub generator declaration list entry class definition.
 *
 */

class Declaration;

class DeclarationListEntry
{
    friend class DeclarationListIterator;
    friend class DeclarationList;

public:
    DeclarationListEntry (Declaration *d);

    Declaration *getEntry () const;
    DeclarationListEntry *getLink () const;
    void setLink (DeclarationListEntry *);

private:
    Declaration *entry;			/* the real entry */
    DeclarationListEntry *link;		/* link to next elem */


};

#ifndef NO_INLINES
#  include "DeclListE.n"
#endif

#endif
