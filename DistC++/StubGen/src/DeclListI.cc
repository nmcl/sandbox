/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclListI.cc,v 1.1 1997/09/25 15:30:49 nmcl Exp $
 */

/*
 * Declaration list iterator class. Iterates over a given list of
 * declarations returning one at a time.
 *
 */ 


#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLLIST_H_
#  include "DeclList.h"
#endif

#ifndef DECLLISTI_H_
#  include "DeclListI.h"
#endif

static const char RCSid[] = "$Id: DeclListI.cc,v 1.1 1997/09/25 15:30:49 nmcl Exp $";

DeclarationListIterator::DeclarationListIterator ( DeclarationList *l )
                                                 : next(l?l->head:0),
						   currentList(l)
{
}

/*
 * return the next entry from the current list. If there are no more
 * entries, return 0.
 */

Declaration *DeclarationListIterator::operator()()
{ 
    DeclarationListEntry *current = next;
    if (current == 0)
    {
	return 0;
    }

    next = current->getLink();
    return current->getEntry();
}

