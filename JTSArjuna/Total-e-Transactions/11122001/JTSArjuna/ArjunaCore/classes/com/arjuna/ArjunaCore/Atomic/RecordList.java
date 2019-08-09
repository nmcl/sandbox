/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecordList.java,v 1.1.4.1.6.2.4.1 2001/01/26 13:45:08 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintWriter;

/**
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
 * Note, the methods of this class do not need to be synchronized because
 * instances of this class are only used from within synchronized classes.
 * Applications should not use this class.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: RecordList.java,v 1.1.4.1.6.2.4.1 2001/01/26 13:45:08 nmcl Exp $
 * @since JTS 1.0.
 */

public class RecordList
{

public RecordList ()
    {
	listHead = null;
	listTail = null;
	noEntries = 0;
    }

public void finalize ()
    {
	AbstractRecord temp;
	int count = noEntries;

	for (int i = 0; i < count; i++)
	{
	    temp = getFront();
	    temp = null;
	}
    }

    /**
     * Remove and return the element at the front of the list.
     *
     * @return the front element.
     */

public final AbstractRecord getFront ()
    {
	AbstractRecord temp = listHead;

	if (noEntries == 1)
	{
	    listHead = listTail = null;
	    noEntries = 0;
	}
	else if (noEntries > 1)
	{
	    listHead = listHead.getNext();
	    listHead.setPrevious(null);
	    temp.setNext(null);
	    temp.setPrevious(null);
	    noEntries--;
	}

	return temp;
    }

    /**
     * Remove and return the element at the tail of the list.
     *
     * @return the last element.
     */

public final AbstractRecord getRear ()
    {
	AbstractRecord temp = listTail;

	if (noEntries == 1)
	{
	    listHead = listTail = null;
	    noEntries = 0;
	}
	else if (noEntries > 1)
	{
	    listTail = listTail.getPrevious();
	    listTail.setNext(null);
	    temp.setPrevious(null);
	    noEntries--;
	}

	return temp;
    }

    /**
     * Insert the entry at the head of the list.
     */

public final boolean insert (AbstractRecord newRecord)
    {
	/* Do the insert starting at the head of the list */

	return insert(newRecord, listHead);
    }

public final void print (PrintWriter strm)
    {
	AbstractRecord arp = listHead;

	for (int i = 0; i < noEntries; i++)
	{
	    strm.print(arp);
	    arp = arp.getNext();
	}
    }

    /**
     * Explicit push onto front of list.
     */

public final void putFront (AbstractRecord newRecord)
    {
	if (listHead == null)
	{
	    listHead = listTail = newRecord;
	    newRecord.setNext(null);
	    newRecord.setPrevious(null);
	}
	else
	{
	    listHead.setPrevious(newRecord);
	    newRecord.setPrevious(null);
	    newRecord.setNext(listHead);
	    listHead = newRecord;
	}
	
	noEntries++;
    }

    /**
     * Explicit push onto rear of list.
     */

public final void putRear (AbstractRecord newRecord)
    {
	if (listTail == null)
	{
	    listHead = listTail = newRecord;
	    newRecord.setNext(null);
	    newRecord.setPrevious(null);
	}
	else
	{
	    listTail.setNext(newRecord);
	    newRecord.setPrevious(listTail);
	    newRecord.setNext(null);
	    listTail = newRecord;
	}
	
	noEntries++;
    }

    /**
     * @return the number of items in the current list.
     */

public final int size ()
    {
	return noEntries;
    }

    /**
     * This is a variation on ordered insertion.
     * Insertion obeys the following algorithm
     *
     * Starting at the record indicated by 'startat' examine each
     * entry in the list in turn and perform the following code
     * 1) If the new record should be merged with the old, call
     *    nr.merge passing the old record as an argument and then
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
     * @returns <code>true</code> if insertion/replacement took place,
     * <code>false</code> otherwise.
     */

private final boolean insert (AbstractRecord newRecord, AbstractRecord startAt)
    {
	AbstractRecord current = startAt;

	/*
	 * Step through the existing list one record at a time
	 */

	while (current != null)
	{
	    if (newRecord.shouldMerge(current))
	    {
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							 FacilityCode.FAC_ABSTRACT_REC, "RecordList::insert("+this+") : merging "
							 +newRecord.type()+" and "+current.type()+" for "+newRecord.order());
		}

		newRecord.merge(current);
		replace(newRecord, current);
		return true;
	    }
	    else
		if (newRecord.shouldReplace(current))
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							     FacilityCode.FAC_ABSTRACT_REC, "RecordList::insert("+this+") : replacing "
							     +current.type()+" with "+newRecord.type()+" for "+newRecord.order());
		    }

		    replace(newRecord, current);
		    return true;
		}
		else
		    if (newRecord.shouldAdd(current))
		    {
			if (DebugController.enabled())
			{
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
								 FacilityCode.FAC_ABSTRACT_REC, "RecordList::insert("+this+") : adding extra record of type "
								 +newRecord.type()+" before "+current.type()+" for "+newRecord.order());
			}

			insertBefore(newRecord, current);
			return true;
		    }
		    else
		    {
			if (newRecord.shouldAlter(current))
			    newRecord.alter(current);

			if (newRecord.equals(current))
			{
			    return false;
			}
			else if (newRecord.lessThan(current))
			{
			    if (DebugController.enabled())
			    {
				DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
								     FacilityCode.FAC_ABSTRACT_REC, "RecordList::insert("+this+") : inserting "
								     +newRecord.type()+" for "+newRecord.order()+" before "+current.type()+" for "+current.order());
			    }

			    insertBefore(newRecord, current);

			    return true;
			}
			current = current.getNext();
		    }
	}

	if (current == null)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						     FacilityCode.FAC_ABSTRACT_REC, "RecordList::insert("+this+") : appending "
						     +newRecord.type()+" for "+newRecord.order());
	    }

	    putRear(newRecord);
	}
	else
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						     FacilityCode.FAC_ABSTRACT_REC, "RecordList::insert("+this+") : inserting "
						     +newRecord.type()+" for "+newRecord.order()+" before "+current.type()+" for "+current.order());
	    }

	    insertBefore(newRecord, current);
	}
    
	return true;
    }

    /**
     * Insert the first parameter before the second in the list.
     */

private final void insertBefore (AbstractRecord newRecord, AbstractRecord before)
    {
	/* first do the main link chaining */

	newRecord.setPrevious(before.getPrevious());
	newRecord.setNext(before);
	before.setPrevious(newRecord);

	/* determine if insert was at list head */

	if (newRecord.getPrevious() != null)
	    (newRecord.getPrevious()).setNext(newRecord);
	else
	    /* must be pointing to the head of the list  */
	    listHead = newRecord;
	
	noEntries++;
    }

private final void replace (AbstractRecord newRecord, AbstractRecord oldRecord)
    {
	newRecord.setPrevious(oldRecord.getPrevious());
	newRecord.setNext(oldRecord.getNext());
    
	if (newRecord.getPrevious() != null)
	    (newRecord.getPrevious()).setNext(newRecord);
	else
	    listHead = newRecord;
    
	if (newRecord.getNext() != null)
	    (newRecord.getNext()).setPrevious(newRecord);
	else
	    listTail = newRecord;

	oldRecord = null;
    }
    
protected AbstractRecord listHead;
    
private AbstractRecord listTail;
private int            noEntries;

}
