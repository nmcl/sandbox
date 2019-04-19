/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CadaverLockManager.java,v 1.3 2003/08/11 15:04:48 nmcl Exp $
 */

package com.arjuna.ats.txoj;

import com.arjuna.ats.arjuna.coordinator.AbstractRecord;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.common.Uid;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.txoj.common.*;

import com.arjuna.ats.txoj.logging.txojLogger;
import com.arjuna.ats.txoj.logging.FacilityCode;

import com.arjuna.common.util.logging.*;

/*
 *
 * Lock concurrency controller
 *
 * Lock-base concurrency control management system
 * Instances of this class are created by CadaverLockRecord class
 * instances for the sole purpose of lock cleanup due to a locked
 * object going out of scope prior to action termination. 
 * Serialisability prevents locks being released as scope is exited
 * thus they must be cleaned up later.
 *
 */

class CadaverLockManager extends LockManager
{
    
public CadaverLockManager (Uid objUid, String tName)
    {
	super(objUid);

	objectTypeName = new String(tName);

	if (txojLogger.aitLogger.isDebugEnabled())
	{
	    txojLogger.aitLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				       FacilityCode.FAC_CONCURRENCY_CONTROL, 
				       "CadaverLockManager::CadaverLockManager("+objUid+")");
	}
    }

    /*
     * Publically inherited functions
     */

public boolean restore_state (InputObjectState os, int t)
    {
	return false;
    }

public boolean save_state (OutputObjectState os, int t)
    {
	return false;
    }

public String type ()
    {
	return objectTypeName;
    }

private String objectTypeName;
    
}
