/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ActionHierarchy.java,v 1.2.26.1.2.1.4.1 2000/12/21 11:21:13 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.OrbCommon.ORBInterface;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Template.*;
import java.io.PrintStream;

/*
 * Need methods for adding to and removing from the
 * hierarchy as it changes over time. We only want to
 * maintain a single instance of this class for each
 * hierarchy, no matter how it evolves. This means
 * covering branches etc.
 */

/**
 * Maintains the OTS transaction hierarchy.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_ActionHierarchy.java,v 1.2.26.1.2.1.4.1 2000/12/21 11:21:13 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_ActionHierarchy
{
    
public OTS_ActionHierarchy ()
    {
	_hier = new SimpleList();
    }

public void finalize ()
    {
	if (_hier != null)
	{
	    ListElementWrapper ptr = (ListElementWrapper) _hier.orderedPop();

	    while (ptr != null)
	    {
		ptr = null;
		ptr = (ListElementWrapper) _hier.orderedPop();
	    }

	    _hier = null;
	}
    }

public final synchronized boolean present (Control control)
    {
	SimpleListIterator iter = new SimpleListIterator(_hier);
	ListElementWrapper wrapper = (ListElementWrapper) iter.iterate();
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
		    wrapper = (ListElementWrapper) iter.iterate();		    
		
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
	_hier.push(new ListElementWrapper(control));
    }

public final synchronized Control pop ()
    {
	ListElementWrapper wrapper = (ListElementWrapper) _hier.orderedPop();
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

SimpleList _hier;
 
};
