/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CadaverLockManager.java,v 1.1 2000/02/25 14:03:44 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaCommon.Common.*;

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
