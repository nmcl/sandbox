/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ActionHierarchy.java,v 1.3 1998/07/06 13:28:36 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions;

import com.arjuna.OrbCommon.ORBInterface;
import org.omg.CosTransactions.*;
import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Template.*;
import java.io.PrintStream;

/*
 * Need methods for adding to and removing from the
 * hierarchy as it changes over time. We only want to
 * maintain a single instance of this class for each
 * hierarchy, no matter how it evolves. This means
 * covering branches etc.
 */

class OTS_ActionHierarchy
{
    
public OTS_ActionHierarchy ()
    {
	_hier = new SimpleUidList();
    }

public void finalize ()
    {
	if (_hier != null)
	{
	    UidListElementWrapper ptr = (UidListElementWrapper) _hier.orderedPop();

	    while (ptr != null)
	    {
		ptr = null;
		ptr = (UidListElementWrapper) _hier.orderedPop();
	    }

	    _hier = null;
	}
    }

public final synchronized boolean present (Control control)
    {
	SimpleUidListIterator iter = new SimpleUidListIterator(_hier);
	UidListElementWrapper wrapper = (UidListElementWrapper) iter.iterate();
	Control act = ((wrapper != null) ? (Control) wrapper.value() : null);
	boolean found = false;
	Coordinator theCoordinator = null;
    
	try
	{
	    theCoordinator = control.get_coordinator();
	}
	catch (Exception e)
	{
	    theCoordinator = null;
	}

	if (theCoordinator != null)
	{
	    Coordinator coord = null;

	    while ((act != null) && (!found))
	    {
		try
		{
		    coord = act.get_coordinator();
		    found = theCoordinator.is_same_transaction(coord);
		}
		catch (Exception e)
		{
		}

		coord = null;

		if (!found)
		{
		    wrapper = (UidListElementWrapper) iter.iterate();		    
		
		    act = ((wrapper != null) ? (Control) wrapper.value() : null);
		}
	    }

	    coord = null;
	}

	theCoordinator = null;
    
	return found;
    }

public final synchronized void push (Control control)
    {
	_hier.push(new UidListElementWrapper(control));
    }

public final synchronized Control pop ()
    {
	UidListElementWrapper wrapper = (UidListElementWrapper) _hier.orderedPop();
	Control cont = ((wrapper != null) ? (Control) wrapper.value() : null);
	Control toReturn = null;
    
	if (wrapper != null)
	{
	    wrapper = null;

	    return cont;
	}
	else
	    return null;
    }

public synchronized Control inspect ()
    {
	return null;
    }

private SimpleUidList _hier;
 
};
