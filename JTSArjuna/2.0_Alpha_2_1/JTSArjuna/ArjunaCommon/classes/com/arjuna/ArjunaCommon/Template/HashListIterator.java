/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: HashListIterator.java,v 1.1 2000/02/25 14:02:55 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

public class HashListIterator
{

public HashListIterator (HashList list)
    {
	theList = list;
	iter = null;
	lastBucket = -1;
    }

public void finalize ()
    {
	iter = null;
    }

public synchronized ListElement iterate ()
    {
	ListElement th = null;

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
	    iter = new BasicListIterator(theList.buckets[lastBucket]);

	if (iter != null)
	    th = iter.iterate();
    
	return th;
    }

private HashList theList;
private BasicListIterator iter;
private int lastBucket;

};
