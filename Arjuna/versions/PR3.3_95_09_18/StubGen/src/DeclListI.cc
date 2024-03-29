/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DeclListI.cc,v 1.2 1994/11/10 11:29:50 ngdp Exp $
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

static const char RCSid[] = "$Id: DeclListI.cc,v 1.2 1994/11/10 11:29:50 ngdp Exp $";

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

