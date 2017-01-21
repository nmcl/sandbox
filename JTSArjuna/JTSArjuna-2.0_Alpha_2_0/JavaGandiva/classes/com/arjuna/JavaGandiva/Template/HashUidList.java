/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashUidList.java,v 1.5 1998/07/06 13:31:05 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

import com.arjuna.JavaGandiva.Common.Uid;

public class HashUidList
{

public HashUidList (int hashBucket)
    {
	maxBucket = hashBucket;
	buckets = new BasicUidList[hashBucket];
    
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
    
public synchronized boolean add (UidListElement th)
    {
	if (th != null)
	{
	    int entry = th.get_uid().hashCode() % maxBucket;

	    if (buckets[entry] == null)
		buckets[entry] = new BasicUidList();

	    return buckets[entry].add(th);
	}
    
	return false;
    }

    /*
     * Insert is like add execpt that it only adds once. i.e. if the entry
     * already exists it returns false;
     */

public synchronized boolean insert (UidListElement th)
    {
	if (th != null)
	{
	    int entry = th.get_uid().hashCode() % maxBucket;

	    if (buckets[entry] != null)
		return buckets[entry].insert(th);
	    else
	    {
		buckets[entry] = new BasicUidList();
		return buckets[entry].add(th);
	    }
	}
    
	return false;
    }

public synchronized boolean remove (UidListElement findMe)
    {
	if (findMe != null)
	{
	    int entry = findMe.get_uid().hashCode() % maxBucket;

	    if (buckets[entry] != null)
		return buckets[entry].remove(findMe);
	}
	
	return false;
    }

public synchronized UidListElement remove (Uid findMe)
    {
	int entry = findMe.hashCode() % maxBucket;
    
	if (buckets[entry] != null)
	    return buckets[entry].remove(findMe);

	return null;
    }

public synchronized UidListElement lookFor (Uid findMe)
    {
	int entry = findMe.hashCode() % maxBucket;
	
	if (buckets[entry] != null)
	    return buckets[entry].lookFor(findMe);

	return null;
    }

public synchronized UidListElement orderedPop ()
    {
	UidListElement th = null;
    
	for (int i = 0; (i < maxBucket) && (th != null); i++)
	{
	    if ((buckets[i] != null) && (buckets[i].size() > 0))
	    {
		th = buckets[i].orderedPop();
	    }
	}
    
	return th;
    }

protected BasicUidList[] buckets;
protected int maxBucket;

};
