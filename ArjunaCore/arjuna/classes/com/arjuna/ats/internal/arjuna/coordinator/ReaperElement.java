/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ReaperElement.java,v 1.3 2003/08/11 15:04:17 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.coordinator;

import com.arjuna.common.util.logging.*;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.coordinator.Reapable;
import com.arjuna.ats.arjuna.coordinator.ActionStatus;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.ats.internal.arjuna.template.OrderedListElement;

public class ReaperElement implements OrderedListElement
{

    /*
     * Currently, once created the reaper object and thread stay around
     * forever.
     * We could destroy both once the list of transactions is null. Depends
     * upon the relative cost of recreating them over keeping them around.
     */


public ReaperElement (Reapable control, int timeout)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "ReaperElement::ReaperElement ( "+control+", "+timeout+" )");
	}

	_control = control;
	_timeout = timeout;

	/*
	 * Given a timeout period in seconds, calculate its absolute value
	 * from the current time of day in milliseconds.
	 */
	
	_absoluteTimeout = timeout*1000 + System.currentTimeMillis();
    }

public void finalize ()
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "ReaperElement.finalize ()");
	}
	
	_control = null;
    }
    
public final boolean equals (OrderedListElement e)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.OPERATORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "ReaperElement.equals ()");
	}

	if (e instanceof ReaperElement)
	    return (_absoluteTimeout == ((ReaperElement) e)._absoluteTimeout);
	else
	    return false;
    }

public final boolean lessThan (OrderedListElement e)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.OPERATORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "ReaperElement.lessThan ()");
	}
	
	if (e instanceof ReaperElement)
	    return (_absoluteTimeout < ((ReaperElement)e)._absoluteTimeout);
	else
	    return false;
    }

public final boolean greaterThan (OrderedListElement e)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.OPERATORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_ATOMIC_ACTION, "ReaperElement.greaterThan ()");
	}
	
	if (e instanceof ReaperElement)
	    return (_absoluteTimeout > ((ReaperElement)e)._absoluteTimeout);
	else
	    return false;
    }

    public Reapable _control;
    public long     _absoluteTimeout;
    public int      _timeout;
    
}



