/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityCompletionData.javatmpl,v 1.1 2001/07/23 13:44:53 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.Activity.ActivityCoordinator;

import org.omg.CosActivity.CompletionStatus;
import org.omg.CORBA.*;

class ActivityCompletionData
{

public ActivityCompletionData ()
    {
	_theStatus = org.omg.CosActivity.CompletionStatus.CompletionStatusFail;
	_signalSetName = null;  // valid to remain null
    }

public synchronized boolean setCompletionStatus (org.omg.CosActivity.CompletionStatus cs)
    {
	boolean res = false;
	
	/*
	 * Once we are at CompletionStatusFailOnly then we cannot
	 * allow any further state changes.
	 */

	switch ( _theStatus.value() )
	{
	case  org.omg.CosActivity.CompletionStatus ._CompletionStatusSuccess  :
	case  org.omg.CosActivity.CompletionStatus ._CompletionStatusFail  :
	    {
		_theStatus = cs;
		res = true;
	    }
	    break;
	case  org.omg.CosActivity.CompletionStatus ._CompletionStatusFailOnly  :
	default:
	    {
		res = false;
	    }
	    break;
	}

	return res;
    }

public synchronized org.omg.CosActivity.CompletionStatus getCompletionStatus ()
    {
	return _theStatus;
    }

public synchronized void setCompletionSignalSet (String signalSetName)
    {
	_signalSetName = signalSetName;
    }
    
public synchronized String getCompletionSignalSet ()
    {
	return _signalSetName;
    }
    
private org.omg.CosActivity.CompletionStatus _theStatus;
private String                               _signalSetName;
 
};
