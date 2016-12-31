/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_FactoryList.java,v 1.1 2000/02/25 14:06:17 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition;

import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_InterpositionCreator;
import com.arjuna.CosTransactions.Interposition.Strict.OTS_StrictInterpositionCreator;
import com.arjuna.CosTransactions.Interposition.Restricted.OTS_RestrictedInterpositionCreator;
import com.arjuna.CosTransactions.Interposition.OSI.OTS_OSIInterpositionCreator;
import com.arjuna.CosTransactions.ArjunaOTS.Arjuna;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;

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

public Control recreate (PropagationContext ctx)
    {
	return _create.recreate(ctx);
    }

public int _formatID;
public FactoryElement _next;

private OTS_FactoryCreator _create;
    
};

/*
 * Add the Arjuna OTS transaction recreator as a minimum, i.e., no
 * special action is required to know about Arjuna transactions.
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

public static synchronized Control recreate (PropagationContext ctx, int formatID)
    {
	Control toReturn = null;
    
	FactoryElement ptr = find(formatID);

	if (ptr != null)
	{
	    toReturn = ptr.recreate(ctx);
	}

	return toReturn;
    }

public static synchronized void add (OTS_FactoryCreator create, int formatID)
    {
	FactoryElement ptr = find(formatID);

	if (ptr == null)     // assume that the create and id always match
	{
	    ptr = new FactoryElement(create, formatID);
	    ptr._next = _list;
	    _list = ptr;
	}
    }

public static synchronized void remove (int formatID)
    {
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
    }
    
    /*
     * Only allow a default to be added once!
     */
    
public static synchronized boolean addDefault (OTS_FactoryCreator create, int formatID)
    {
	boolean res = false;

	if (OTS_FactoryList._default == null)
	{
	    OTS_FactoryList._default = new FactoryElement(create, formatID);
	    res = true;
	}
	else
	    System.err.println("WARNING - OTS_FactoryList.addDefault - default already set!");

	return res;
    }

public static synchronized boolean removeDefault ()
    {
	boolean found = false;

	if (OTS_FactoryList._default != null)
	{
	    OTS_FactoryList._default = null;
	    found = true;
	}

	return found;
    }
    
protected static FactoryElement find (int formatID)
    {
	FactoryElement ptr = _list;

	while (ptr != null)
	{
	    if (ptr._formatID == formatID)
		return ptr;
	    else
		ptr = ptr._next;
	}

	/*
	 * No ID matches, so use default.
	 */

	return OTS_FactoryList._default;
    }

private static FactoryElement _list = null;
private static FactoryElement _default = null;  // used if no formatID values match.
    
};

