/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclList.cc,v 1.5 1993/03/22 09:30:52 ngdp Exp $
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


#ifndef DECLLIST_H_
#  include "DeclList.h"
#endif

static const char RCSid[] = "$Id: DeclList.cc,v 1.5 1993/03/22 09:30:52 ngdp Exp $";

DeclarationList::~DeclarationList ()
{
    DeclarationListEntry *tmp = head;
    while (tmp != 0)
    {
	head = tmp->get_link();
	delete tmp;
	tmp = head;
    }
}

/* 
 * Append a single Declaration to the list
 */

void DeclarationList::append_decl ( Declaration *d )
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

    tail->set_link(t);			/* set tail */
    tail = t;
}

/* 
 * Append an entire DeclarationList to the end of this list.
 * Clears argument list to empty
 */

void DeclarationList::append_list ( DeclarationList *dl )
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

    tail->set_link(dl->head);		/* link lists togethor */
    tail = dl->tail;			/* move tail */
    dl->head = dl->tail = 0;		/* clear incoming list */
}


/*
 * Inverse of append - push Declaration on to front of list. Same criterion
 * apply
 */

void DeclarationList::push_decl ( Declaration *d )
{
    DeclarationListEntry *temp;

    if (d == 0)
	return;

    temp = new DeclarationListEntry(d);
    temp->set_link(head);
    head = temp;
}
#if 0
void DeclarationList::push_list ( DeclarationList *dl )
{
    if ((dl == 0) || (dl->head == 0))
	return;

    dl->tail->set_link(head);
    dl->tail = tail;
}
#endif
#ifdef NO_INLINES
#  define DECLLIST_CC_
#  include "DeclList.n"
#  undef DECLLIST_CC_
#endif
