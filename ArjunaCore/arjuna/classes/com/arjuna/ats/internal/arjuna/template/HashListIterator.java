/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: HashListIterator.java,v 1.1 2003/06/19 10:50:38 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.template;

/**
 * An iterator for HashLists.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: HashListIterator.java,v 1.1 2003/06/19 10:50:38 nmcl Exp $
 * @since JTS 1.0.
 */

public class HashListIterator
{

    /**
     * Create a new iterator which will be used to iterate over
     * the specified list.
     */

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

    /**
     * Return the next item in the list.
     */

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

private HashList          theList;
private BasicListIterator iter;
private int               lastBucket;

}
