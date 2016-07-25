/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockList.cc,v 1.6 1995/06/26 13:15:56 ngdp Exp $
 */

/*
 *
 * Lock list maintainance class implementation
 *
 */ 

#ifdef sun
/* Get over standard cpp include nesting limit on 4.1.* */
#    ifndef STATEMAN_H_
#      include <Arjuna/StateMan.h>
#    endif
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

#ifndef LOCKLIST_H_
#  include "LockList.h"
#endif

#ifndef LOCKLISTI_H_
#  include "LockListI.h"
#endif

static const char RCSid[] = "$Id: LockList.cc,v 1.6 1995/06/26 13:15:56 ngdp Exp $";

/*
 * Public constructors and destructor
 */

LockList::LockList ()
                   : count(0),
		     head(0)
{
}

LockList::~LockList ()
{
    Lock *temp;

    while ((temp = pop()) != 0)
    {
	delete temp;
    }
}

/*
 * Non virtual public functions 
 */

/*
 * Insert a new Lock. This returns TRUE if the insertion occurred, false
 * otherwise. Insertion fails if a matching lock already exists in the list.
 */

Boolean LockList::insert ( Lock *newlock )
{
    LockListI next(*this);
    Lock *current;

    while ((current = next()) != 0)
    {
	if (*current == *newlock)
	    return FALSE;
    }
    push(newlock);
    return TRUE;
}

/*
 * Pop the first element off the list and return it.
 */

Lock *LockList::pop ()
{
    Lock *current;

    if (count == 0)
	return 0;

    current = (Lock *)head;
    count--;
    head = head->getLink();
    current->setLink(0);
    return current;
}

/*
 *  Push a new element at the head of the list. First set the link
 *  field to be the old head, and then set head to be the new element.
 */

void LockList::push ( Lock *newLock )
{
    newLock->setLink(head);
    head = newLock;
    count++;
}

/*
 * Discard the element following the one pointed at. If it is the
 * first element (current = 0) then simply change the head pointer.
 * Beware if current points at the last element or the list is empty!
 * This probably indicates a bug in the caller.
 */

void LockList::forgetNext ( Lock *current )
{
    if (count > 0)			/* something there to forget */
    {
	if (current == 0)
	    head = head->getLink();
	else
	{
	    Lock *nextOne = current->getLink();
	    
	    /* See if at list end */

	    if (nextOne != 0)
		current->setLink(nextOne->getLink());
	    else
	    {
		/*
		 * Probably an error - being asked to forget element
		 * after end of list
		 */
		count++;
		current->setLink(0);	/* force end of list */
	    }

	}
	count--;
    }
}

#ifdef NO_INLINES
#  define LOCKLIST_CC_
#  include "LockList.n"
#  undef LOCKLIST_CC_
#endif
