/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockList.cc,v 1.2 1993/11/16 09:55:35 ngdp Exp $
 */

/*
 *
 * Lock list maintainance class implementation
 *
 */ 

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef LOCKLIST_H_
#  include "LockList.h"
#endif

#ifndef LOCKLISTI_H_
#  include "LockListI.h"
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

static const char RCSid[] = "$Id: LockList.cc,v 1.2 1993/11/16 09:55:35 ngdp Exp $";

/*
 * Public constructors and destructor
 */

LockList::LockList ()
                   : head(0),
		     count(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockList::LockList()" << endl;
#endif
}

LockList::~LockList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockList::~LockList()" << endl;
#endif

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
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockList::insert(" << (void *)newlock << ")" << endl;
#endif

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
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockList::pop()" << endl;
#endif

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
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockList::push(" << (void *)newLock << ")" << endl;
#endif

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
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockList::forgetNext(" << (void *)current << ")" << endl;
#endif

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
