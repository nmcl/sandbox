/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AAStatus.java,v 1.3 1998/07/06 10:49:47 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AAStatus;

import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;

/*
 * Class: AAStatus
 * Used to check for the existence of AtomicAction states in the object store
 */

public class AAStatus
{

public static final int UNRESOLVED_AA = 0;
public static final int COMMITTED_AA = 1;
    
public AAStatus (int res)  // should be a return value
    {
	res = 1;
    }

public int isCommitted (Uid u, boolean checkProcess)
    {
	int result = ActionCondition.ACTION_STATUS_UNKNOWN;
	AtomicAction aa = new AtomicAction(u);

	if ( aa.activate() )
	{
	    /*
	     * No way (short of native methods) to get the
	     * creator pid, so we fake it.
	     */
	    
	    int aaPid = 0; // aa.getCreatorPid();
	    
	    if (pingProcess(aaPid))
	    {
		result = ActionCondition.ACTION_ATTACHED;
	    }
	    else
	    {
		result = ActionCondition.ACTION_COMMITTED;
	    }
	}
	else
	{
	    result = ActionCondition.ACTION_ABORTED;
	}
	
	return result;
    }

private boolean pingProcess (int pid)
    {
	// need native method
	
	return true;
    }

}
