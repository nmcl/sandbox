/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_FactoryList.java,v 1.5 1999/03/29 13:45:12 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions;

import com.arjuna.JavaCosTransactions.Interposition.Arjuna.OTS_InterpositionCreator;
import com.arjuna.JavaCosTransactions.Interposition.Strict.OTS_StrictInterpositionCreator;
import com.arjuna.JavaCosTransactions.ArjunaOTS.Arjuna;
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

class OTS_FactoryList
{
    
public OTS_FactoryList ()
    {
	OTS_FactoryList.add(new OTS_InterpositionCreator(), Arjuna.XID());
	OTS_FactoryList.add(new OTS_StrictInterpositionCreator(), Arjuna.strictXID());
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

	return null;
    }

private static FactoryElement _list = null;
 
};

