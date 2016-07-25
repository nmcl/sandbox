/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecList.cc,v 1.7 1993/03/22 09:20:06 ngdp Exp $
 */

/*
 *
 * Record List Class
 *
 * This class manages instances of the classes derived from AbstractRecord
 * in the form of an ordered doubly-linked list.
 * The ordering and insertion criteria
 * are not particularly standard - see the comment on 'insert' for the
 * actual algorithm used in insertion. The algorithm assumes that one or
 * more different record type instances (LockRecords, RecoveryRecords, etc.)
 * will be inserted into the list at different times. Each such record
 * contains specific information managing certain properties of any
 * particular object. As execution progresses newly created records may
 * need to be merged with, replace entirely, or be added to existing
 * records that relate to an object.
 *
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef RECLIST_H_
#  include "RecList.h"
#endif

static const char RCSid[] = "$Id: RecList.cc,v 1.7 1993/03/22 09:20:06 ngdp Exp $";

/*
 * Public constructors and destructor
 */

RecordList::RecordList ()
                       : listHead(0),
			 listTail(0),
			 noEntries(0)
{

#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "RecordList::RecordList()\n" << flush;
#endif

}

RecordList::~RecordList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "RecordList::~RecordList()\n" << flush;
#endif

    AbstractRecord *temp;
    int count = noEntries;

    for (int i = 0; i < count; i++)
    {
	temp = getFront();
	delete temp;
    }
}

/* 
 * Public non-virtual functions 
 */

/* 
 * remove and return the element at the front of the list
 */

AbstractRecord *RecordList::getFront ()
{
    AbstractRecord *temp = listHead;

    if (noEntries == 1)
    {
	listHead = listTail = 0;
	noEntries = 0;
    }
    else if (noEntries > 1)
    {
	listHead = listHead->getNext();
	listHead->setPrevious(0);
	temp->setNext(0);
	temp->setPrevious(0);
	noEntries--;
    }

    return temp;
}

/*
 * remove and return the element at the tail of the list
 */

AbstractRecord *RecordList::getRear ()
{
    AbstractRecord *temp = listTail;

    if (noEntries == 1)
    {
	listHead = listTail = 0;
	noEntries = 0;
    }
    else if (noEntries > 1)
    {
	listTail = listTail->getPrevious();
	listTail->setNext(0);
	temp->setPrevious(0);
	noEntries--;
    }

    return temp;
}

Boolean RecordList::insert ( AbstractRecord *newRecord )
{
    /* Do the insert starting at the head of the list */

    return insert(newRecord, listHead);
}

ostream& RecordList::print (ostream& strm ) const
{
    AbstractRecord *arp = listHead;

    for (int i = 0; i < noEntries; i++)
    {
	strm << *arp;
	arp = arp->getNext();
    }
    return strm;
}

/*
 * explicit push onto front of list
 */

void RecordList::putFront ( AbstractRecord *newRecord )
{
    if (listHead == 0)
    {
	listHead = listTail = newRecord;
	newRecord->setNext(0);
	newRecord->setPrevious(0);
    }
    else
    {
	listHead->setPrevious(newRecord);
	newRecord->setPrevious(0);
	newRecord->setNext(listHead);
	listHead = newRecord;
    }
    noEntries++;
}

/*
 * explicit push onto rear of list
 */

void RecordList::putRear ( AbstractRecord *newRecord )
{
    if (listTail == 0)
    {
	listHead = listTail = newRecord;
	newRecord->setNext(0);
	newRecord->setPrevious(0);
    }
    else
    {
	listTail->setNext(newRecord);
	newRecord->setPrevious(listTail);
	newRecord->setNext(0);
	listTail = newRecord;
    }
    noEntries++;
}

int RecordList::size () const
{
    return noEntries;
}

/*
 * Private non-virtual functions
 */

/*
 * This is an variation on ordered insertion.
 * Insertion obeys the following algorithm
 *
 * Starting at the record indicated by 'startat' examine each
 * entry in the list in turn and perform the following code
 * 1) If the new record should be merged with the old, call
 *    nr->merge passing the old record as an argument and then
 *    INSERT the new record IN PLACE OF the old and exit
 * 2) If the new record should replace the old then
 *    INSERT the new record IN PLACE OF the old and exit
 * 3) If the new record should be added in addition to the old then
 *    INSERT the new record BEFORE the old and exit
 * 4) If the two records are the same (determined by the 
 *    == operator) simply exit
 * 5) Otherwise determine if the new record should be added here
 *    regardless due to the ordering constraints on the list and if so
 *    add and exit, otherwise step on to the next element and repeat
 *    all the steps.
 *
 * Steps 1-4 effectively ensure that information maintained in any two
 * records for the same object is current either by merging in new
 * information, replacing the old with new, adding in new, or leaving the
 * old alone.
 * Step 5 ensures that if no existing record exists insertion takes
 * place at the correct point
 *
 * Returns TRUE if insertion/replacement took place, FALSE otherwise
 */


Boolean RecordList::insert ( AbstractRecord *newRecord, 
			     AbstractRecord *startAt )
{
    AbstractRecord *current = startAt;

    /*
     * Step through the existing list one record at a time
     */

    while (current != 0)
    {
	if (newRecord->should_merge(current))
	{
	    newRecord->merge(current);
	    replace(newRecord, current);
	    return TRUE;
	}
	else
	    if (newRecord->should_replace(current))
	    {
		replace(newRecord, current);
		return TRUE;
	    }
	    else
		if (newRecord->should_add(current))
		{
		    insertBefore(newRecord, current);
		    return TRUE;
		}
		else
		{
		    if (newRecord->should_alter(current))
	                newRecord->alter(current);

		    if (*newRecord == current)
		    {
			return FALSE;
		    }
		    else if (*newRecord < current)
		    {
			insertBefore(newRecord, current);
			return TRUE;
		    }
		    current = current->getNext();
		}
    }
    if (current == 0)
    {
	putRear(newRecord);
    }
    else
	insertBefore(newRecord, current);

    return TRUE;
}

/*
 * insert the first parameter before the second in the list
 */

void RecordList::insertBefore ( AbstractRecord *newRecord, 
				 AbstractRecord *before )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PRIVATE;
    debug_stream << "RecordList::insertBefore - " << newRecord->ordering()
	         << ", " << before->ordering() << "\n" << flush;
#endif

    /* first do the main link chaining */

    newRecord->setPrevious(before->getPrevious());
    newRecord->setNext(before);
    before->setPrevious(newRecord);

    /* determine if insert was at list head */

    if (newRecord->getPrevious())
	(newRecord->getPrevious())->setNext(newRecord);
    else
	/* must be pointing to the head of the list  */
	    listHead = newRecord;
    noEntries++;
}

void RecordList::replace ( AbstractRecord *newRecord, 
			   AbstractRecord *oldRecord )
{
    newRecord->setPrevious(oldRecord->getPrevious());
    newRecord->setNext(oldRecord->getNext());
    
    if (newRecord->getPrevious())
	(newRecord->getPrevious())->setNext(newRecord);
    else
	listHead = newRecord;
    
    if (newRecord->getNext())
	(newRecord->getNext())->setPrevious(newRecord);
    else
	listTail = newRecord;
    
    delete oldRecord;
}

#ifdef NO_INLINES
#  define RECLIST_CC_
#  include "RecList.n"
#  undef RECLIST_CC_
#endif
