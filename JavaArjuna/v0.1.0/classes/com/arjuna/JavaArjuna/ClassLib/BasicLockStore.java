/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicLockStore.java,v 1.4 1998/07/06 13:26:40 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.LockStore;

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaArjunaLite.Implementation.LockStoreImple;
import com.arjuna.JavaGandiva.Common.*;

import com.arjuna.JavaArjunaLite.Common.LockStoreException;

public class BasicLockStore extends LockStoreImple
{

    /*
     * This implementation is for purely local-applet locks, so
     * we don't need the key.
     */
    
public BasicLockStore (String key)
    {
    }

public InputObjectState read_state (Uid u, String tName) throws LockStoreException
    {
	return segmentStore.read_state(u, tName);
    }
    
public boolean remove_state (Uid u, String tName)
    {
	return segmentStore.remove_state(u, tName);
    }
    
public boolean write_committed (Uid u, String tName, OutputObjectState state)
    {
	return segmentStore.write_committed(u, tName, state);
    }

public ClassName className ()
    {
	return JavaArjunaLiteNames.Implementation_LockStore_BasicLockStore();
    }

public static ClassName name ()
    {
	return JavaArjunaLiteNames.Implementation_LockStore_BasicLockStore();
    }

public static final BasicLockStore create (Object[] param)
    {
	if (param == null)
	    return null;
	
	String key = (String) param[0];

	return new BasicLockStore(key);
    }
    
private static LockStoreSegment segmentStore = new LockStoreSegment();
    
};


class LockStoreSegment
{

public LockStoreSegment ()
    {
	headOfList = null;
    }

    /*
     * https://github.com/nmcl/sandbox/issues/75
public void finalize ()
    {
	headOfList = null;
    }
    */

public synchronized boolean write_committed (Uid u, String tName, OutputObjectState state)
    {
	if (state.size() <= 0)
	    return false;
	
	LockStoreList ptr = find(tName);

	if (ptr == null)
	{
	    ptr = new LockStoreList(tName);
	    ptr.setNext(headOfList);

	    headOfList = ptr;
	}

	ptr.add(u, state);

	return true;
    }

public synchronized InputObjectState read_state (Uid u, String tName) throws LockStoreException
    {
	LockStoreList ptr = find(tName);

	if (ptr == null)
	    return null;

	return ptr.get(u);
    }

public synchronized boolean remove_state (Uid u, String tName)
    {
	boolean found = false;
	LockStoreList ptr = headOfList;

	while ((!found) && (ptr != null))
	{
	    if (ptr.name().compareTo(tName) == 0)
		found = true;
	    else
		ptr = ptr.getNext();
	}

	if (!found)
	    return true;
	else
	    ptr.remove(u);

	return true;
    }

public synchronized boolean remove_segment (String tName)
    {
	boolean found = false;
	LockStoreList ptr = headOfList;
	LockStoreList trail = null;

	while ((!found) && (ptr != null))
	{
	    if (ptr.name().compareTo(tName) == 0)
		found = true;
	    else
	    {
		trail = ptr;
		ptr = ptr.getNext();
	    }
	}

	if (!found)
	    return true;
	else
	{
	    if (trail == null)  // remove headOfList
		headOfList = ptr.getNext();
	    else
		trail.setNext(ptr.getNext());

	    ptr = null;
	}

	return true;
    }    
    
private LockStoreList find (String tName)
    {
	boolean found = false;
	LockStoreList ptr = headOfList;

	while ((!found) && (ptr != null))
	{
	    if (ptr.name().compareTo(tName) == 0)
		found = true;
	    else
		ptr = ptr.getNext();
	}

	return ptr;
    }

private LockStoreList headOfList;
    
};


class LockStoreList
{
    
public LockStoreList (String tName)
    {
	_name = tName;
	_next = null;
    }

public String name ()
    {
	return _name;
    }

public void setNext (LockStoreList n)
    {
	_next = n;
    }

public LockStoreList getNext ()
    {
	return _next;
    }

public boolean add (Uid u, OutputObjectState state)
    {
	LockStateStore ptr = find(u);

	if (ptr == null)
	{
	    ptr = new LockStateStore(u, state);
	    ptr._next = headOfList;
	    headOfList = ptr;
	}
	else
	    ptr._state = state;

	return true;
    }

public InputObjectState get (Uid u) throws LockStoreException
    {
	LockStateStore ptr = find(u);

	if (ptr == null)
	    return null;
	else
	    return new InputObjectState(ptr._state);
    }

public boolean remove (Uid u)
    {
	boolean found = false;
	LockStateStore ptr = headOfList;
	LockStateStore trail = null;

	while ((!found) && (ptr != null))
	{
	    if (ptr._id.equals(u))
		found = true;
	    else
	    {
		trail = ptr;
		ptr = ptr._next;
	    }
	}

	if (!found)
	    return false;
	else
	{
	    if (trail == null)  // remove headOfList
		headOfList = ptr._next;
	    else
		trail._next = ptr._next;
	}

	return true;
    }

private LockStateStore find (Uid u)
    {
	boolean found = false;
	LockStateStore ptr = headOfList;

	while ((!found) && (ptr != null))
	{
	    if (ptr._id.equals(u))
		found = true;
	    else
		ptr = ptr._next;
	}

	return ptr;
    }
	
private LockStoreList _next;
private String _name;
private LockStateStore headOfList;
    
}

class LockStateStore
{

public LockStateStore (Uid u, OutputObjectState s)
    {
	_id = u;
	_state = s;
	_next = null;
    }
	
public Uid _id;
public OutputObjectState _state;
public LockStateStore _next;
    
};
