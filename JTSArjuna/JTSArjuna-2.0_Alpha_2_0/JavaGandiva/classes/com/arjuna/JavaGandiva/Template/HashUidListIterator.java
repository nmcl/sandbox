/*
 * Copyright (C) 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashUidListIterator.java,v 1.7 1998/07/06 13:31:06 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

public class HashUidListIterator
{

public HashUidListIterator (HashUidList list)
    {
	theList = list;
	iter = null;
	lastBucket = -1;
    }

public void finalize ()
    {
	iter = null;
    }

public synchronized UidListElement iterate ()
    {
	UidListElement th = null;

	/*
	 * Assume that receiving a null from the current iterator means
	 * we have come to the end of that list, so move on to next.
	 */

	if (iter != null)
	    th = iter.iterate();

	if (th != null)
	    return th;
	else
	    iter = null;

	lastBucket++;  // move on to next bucket!
	
	while ((lastBucket < theList.maxBucket) &&
	       ((theList.buckets[lastBucket] == null) ||
		(theList.buckets[lastBucket].size() == 0)))
	    lastBucket++;

	if (lastBucket < theList.maxBucket)
	    iter = new BasicUidListIterator(theList.buckets[lastBucket]);

	if (iter != null)
	    th = iter.iterate();
    
	return th;
    }

private HashUidList theList;
private BasicUidListIterator iter;
private int lastBucket;

};
