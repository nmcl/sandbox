/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverLockManager.java,v 1.3 1998/07/06 13:26:16 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.Common.*;

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
