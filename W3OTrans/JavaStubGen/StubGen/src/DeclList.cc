/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclList.cc,v 1.2 1994/04/20 09:02:03 ngdp Exp $
 */

/*
 * Stub generator declaration list manipulation
 *
 * Lists have a very simple structure: a pointer to the head and tail
 * elements of the list. Individual elements on the list maintain
 * the required link pointers.
 *
 * Note that we don't use 'const' parameters here despite the fact
 * Declaration instances are never modified. This is because as they get
 * put on the list the pointer gets copied creating an alias which the
 * compiler assumes might be used to subvert the const property. Grrr.
 *
 */

#include <iostream.h>

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLLIST_H_
#  include "DeclList.h"
#endif

#ifndef DECLLISTI_H_
#  include "DeclListI.h"
#endif

static const char RCSid[] = "$Id: DeclList.cc,v 1.2 1994/04/20 09:02:03 ngdp Exp $";

DeclarationList::~DeclarationList ()
{
    DeclarationListEntry *tmp = head;
    while (tmp != 0)
    {
	head = tmp->getLink();
	delete tmp;
	tmp = head;
    }
}

/* 
 * Append a single Declaration to the list
 */

void DeclarationList::appendDecl ( Declaration *d )
{
    DeclarationListEntry *t;

    if (d == 0)
	return;				/* nothing to do */

    t = new DeclarationListEntry(d);
    if (head == 0)			/* simple insert */
    {
	tail = head = t;
	return;
    }

    tail->setLink(t);			/* set tail */
    tail = t;
}

/* 
 * Append an entire DeclarationList to the end of this list.
 * Clears argument list to empty
 */

void DeclarationList::appendList ( DeclarationList *dl )
{
    if ((dl == 0) || (dl->head == 0))
	return;				/* nothing to do */

    if (head == 0)			/* simple insert */
    {
	head = dl->head;
	tail = dl->tail;
	dl->head = dl->tail = 0;
	return;
    }

    tail->setLink(dl->head);		/* link lists togethor */
    tail = dl->tail;			/* move tail */
    dl->head = dl->tail = 0;		/* clear incoming list */
}


/*
 * Inverse of append - push Declaration on to front of list. Same criterion
 * apply
 */

void DeclarationList::pushDecl ( Declaration *d )
{
    DeclarationListEntry *temp;

    if (d == 0)
	return;

    temp = new DeclarationListEntry(d);
    temp->setLink(head);
    head = temp;
}

ostream& operator<< ( ostream& s, DeclarationList *dl )
{
    DeclarationListIterator next(dl);
    Declaration *d = next();

    while (d)
    {
	d->printDecl(s, NORMAL_DECL, "");
	d = next();
	if (d)
	    s << ", ";
    }
    return s;
}
    
#ifdef NO_INLINES
#  define DECLLIST_CC_
#  include "DeclList.n"
#  undef DECLLIST_CC_
#endif
