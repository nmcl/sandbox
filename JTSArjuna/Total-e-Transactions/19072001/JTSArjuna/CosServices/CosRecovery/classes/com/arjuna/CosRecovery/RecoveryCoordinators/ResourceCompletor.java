/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ResourceCompletor.java,v 1.1.2.1.2.1 2001/02/15 21:38:35 pfurniss Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators;

import com.arjuna.ArjunaCommon.Common.*;
import org.omg.CosTransactions.*;

/**
 * Threaded object used to terminate a prepared Resource after a
 * failure.  Normally failed resources are terminated by replaying the
 * transaction in which the Resource was participating. However, if
 * the transaction rolled back then we won't have any persistent
 * record of the transaction so it can't be reactivated. Resources (and
 * subordinate coordinators) may use the replay_completion response to
 * trigger their own rollback.
 * 
 */

public class ResourceCompletor extends Thread
{
    public static final int ROLLBACK = 0;
    public static final int COMMIT = 1;

    public ResourceCompletor( Resource res, int action )
    {
	_res = res;
	_action = action;
    }
    
    public void finalize () throws Throwable
    {
	super.finalize();
	_res = null;
    }

    public final void run ()
    {
	if (_action == ROLLBACK)
	    rollback();
    }

    private final void rollback ()
    {
	try
	{
	    if (DebugController.enabled()) 
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			       FacilityCode.FAC_CRASH_RECOVERY,
			       "ResourceCompletor.rollback()");
	    }
	    _res.rollback();
	}
	catch (Exception e)
	{
	    if (DebugController.enabled()) 
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						   FacilityCode.FAC_CRASH_RECOVERY,
						   "ResourceCompletor.rollback() - rollback failed: "+e);
	    }
	}
    }

    private Resource _res = null;
    private int      _action = 0;
}

    
    
