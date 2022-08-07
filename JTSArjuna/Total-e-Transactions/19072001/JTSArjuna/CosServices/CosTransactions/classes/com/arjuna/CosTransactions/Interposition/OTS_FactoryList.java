/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_FactoryList.java,v 1.1.8.2.24.1.2.1.24.2 2001/05/02 14:50:02 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Common.Mutex;
import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_InterpositionCreator;
import com.arjuna.CosTransactions.Interposition.Strict.OTS_StrictInterpositionCreator;
import com.arjuna.CosTransactions.Interposition.Restricted.OTS_RestrictedInterpositionCreator;
import com.arjuna.CosTransactions.Interposition.OSI.OTS_OSIInterpositionCreator;
import com.arjuna.CosTransactions.ArjunaOTS.Arjuna;
import com.arjuna.CosTransactions.OTS_Control;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;

import org.omg.CORBA.SystemException;

/*
 * Default visibility.
 */

class FactoryElement
{
    
public FactoryElement (OTS_FactoryCreator create, int formatID)
    {
	_create = create;
	_formatID = formatID;
	_next = null;
    }

public void finalize ()
    {
	if (_next != null)
	    _next = null;
    }

public OTS_Control recreateLocal (PropagationContext ctx) throws SystemException
    {
	return _create.recreateLocal(ctx);
    }

public Control recreate (PropagationContext ctx) throws SystemException
    {
	return recreateLocal(ctx).getControl();
    }

public int            _formatID;
public FactoryElement _next;

private OTS_FactoryCreator _create;
    
};

/*
 * Add the Arjuna OTS transaction recreator as a minimum, i.e., no
 * special action is required to know about Arjuna transactions.
 */

/**
 * Maintains the list of known transaction interposition factories.
 */

public class OTS_FactoryList
{
    
public OTS_FactoryList ()
    {
	OTS_FactoryList.add(new OTS_InterpositionCreator(), Arjuna.XID());
	OTS_FactoryList.add(new OTS_StrictInterpositionCreator(), Arjuna.strictXID());
	OTS_FactoryList.add(new OTS_RestrictedInterpositionCreator(), Arjuna.restrictedXID());
	OTS_FactoryList.add(new OTS_OSIInterpositionCreator(), 0);  // 0 is OSI TP!
	OTS_FactoryList.addDefault(new OTS_OSIInterpositionCreator(), 0);  // 0 is OSI TP!
    }

public static OTS_Control recreateLocal (PropagationContext ctx, int formatID) throws SystemException
    {
	OTS_Control toReturn = null;
    
	FactoryElement ptr = find(formatID);

	if (ptr != null)
	{
	    toReturn = ptr.recreateLocal(ctx);
	}

	return toReturn;
    }
    
public static Control recreate (PropagationContext ctx, int formatID) throws SystemException
    {
	Control toReturn = null;
    
	FactoryElement ptr = find(formatID);

	if (ptr != null)
	{
	    toReturn = ptr.recreate(ctx);
	}

	return toReturn;
    }

public static void add (OTS_FactoryCreator create, int formatID)
    {
	FactoryElement ptr = find(formatID);

	_lock.lock();

	if (ptr == null)     // assume that the create and id always match
	{
	    ptr = new FactoryElement(create, formatID);
	    ptr._next = _list;
	    _list = ptr;
	}

	_lock.unlock();
    }

public static void remove (int formatID)
    {
	_lock.lock();
	
	FactoryElement ptr = _list;
	FactoryElement trail = null;
	boolean found = false;
    
	while ((ptr != null) && (!found))
	{
	    if (ptr._formatID == formatID)
		found = true;
	    else
	    {
		trail = ptr;
		ptr = ptr._next;
	    }
	}

	if (found)
	{
	    if (_list == ptr)
		_list = ptr._next;
	    else
	    {
		if (trail != null)
		    trail._next = ptr._next;
	    }
	
	    ptr._next = null;

	    ptr = null;
	}

	_lock.unlock();
    }
    
    /*
     * Only allow a default to be added once!
     */
    
public static boolean addDefault (OTS_FactoryCreator create, int formatID)
    {
	boolean res = false;

	_lock.lock();
	
	if (OTS_FactoryList._default == null)
	{
	    OTS_FactoryList._default = new FactoryElement(create, formatID);
	    res = true;
	}
	else
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_FactoryList.addDefault - default already set!");

	_lock.unlock();
	
	return res;
    }

public static boolean removeDefault ()
    {
	boolean found = false;

	_lock.lock();
	
	if (OTS_FactoryList._default != null)
	{
	    OTS_FactoryList._default = null;
	    found = true;
	}

	_lock.unlock();
	
	return found;
    }
    
protected static FactoryElement find (int formatID)
    {
	FactoryElement ptr = _list;
	FactoryElement toReturn = null;

	_lock.lock();

	while ((ptr != null) && (toReturn == null))
	{
	    if (ptr._formatID == formatID)
		toReturn = ptr;
	    else
		ptr = ptr._next;
	}

	if (toReturn == null)
	{
	    /*
	     * No ID matches, so use default.
	     */

	    toReturn = OTS_FactoryList._default;
	}
	
	_lock.unlock();

	return toReturn;
    }

private static FactoryElement _list = null;
private static FactoryElement _default = null;  // used if no formatID values match.
private static Mutex _lock = new Mutex();
    
};

