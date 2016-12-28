/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HashList.java,v 1.1 2000/02/25 14:02:55 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

import com.arjuna.ArjunaCommon.Common.Uid;

/*
 * This hash table for objects which define get_uid does not use
 * the Java Hashtable class, since we sometimes want to add multiple
 * different values under the same key.
 */

public class HashList
{

public HashList (int hashBucket)
    {
	maxBucket = hashBucket;
	buckets = new BasicList[hashBucket];
    
	for (int i = 0; i < hashBucket; i++)
	    buckets[i] = null;
    }

public void finalize ()
    {
	empty();

	buckets = null;
    }

public synchronized void empty ()
    {
	for (int i = 0; i < maxBucket; i++)
	    if (buckets[i] != null)
		buckets[i] = null;
    }

public synchronized int size ()
    {
	int rVal = 0;
    
	for (int i = 0; i < maxBucket; i++)
	{
	    if (buckets[i] != null)
	    {
		rVal += buckets[i].size();
	    }
	}
    
	return rVal;
    }
    
public synchronized boolean add (ListElement th)
    {
	if (th != null)
	{
	    int entry = th.get_uid().hashCode() % maxBucket;
	    
	    if (buckets[entry] == null)
		buckets[entry] = new BasicList();

	    return buckets[entry].add(th);
	}
    
	return false;
    }

    /*
     * Insert is like add execpt that it only adds once, i.e., if the entry
     * already exists it returns false.
     */

public synchronized boolean insert (ListElement th)
    {
	if (th != null)
	{
	    int entry = th.get_uid().hashCode() % maxBucket;

	    if (buckets[entry] != null)
		return buckets[entry].insert(th);
	    else
	    {
		buckets[entry] = new BasicList();
		return buckets[entry].add(th);
	    }
	}
    
	return false;
    }

public synchronized boolean remove (ListElement findMe)
    {
	if (findMe != null)
	{
	    int entry = findMe.get_uid().hashCode() % maxBucket;

	    if (buckets[entry] != null)
		return buckets[entry].remove(findMe);
	}
	
	return false;
    }

public synchronized ListElement remove (Uid findMe)
    {
	int entry = findMe.hashCode() % maxBucket;
    
	if (buckets[entry] != null)
	    return buckets[entry].remove(findMe);

	return null;
    }

public synchronized ListElement lookFor (Uid findMe)
    {
	int entry = findMe.hashCode() % maxBucket;
	
	if (buckets[entry] != null)
	    return buckets[entry].lookFor(findMe);

	return null;
    }

public synchronized ListElement orderedPop ()
    {
	ListElement th = null;
    
	for (int i = 0; (i < maxBucket) && (th != null); i++)
	{
	    if ((buckets[i] != null) && (buckets[i].size() > 0))
	    {
		th = buckets[i].orderedPop();
	    }
	}
    
	return th;
    }

protected BasicList[] buckets;
protected int maxBucket;

};
