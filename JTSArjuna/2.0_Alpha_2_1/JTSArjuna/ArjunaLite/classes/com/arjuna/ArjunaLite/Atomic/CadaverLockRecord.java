/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CadaverLockRecord.java,v 1.1 2000/02/25 14:03:45 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaLite.Interface.LockStore;
import java.io.PrintStream;

/*
 *
 * Cadaver Lock Record Class Implementation
 *
 * Instances of this record class are created by LockManager if the
 * object goes out of scope prior to the end of a manipulating action.
 * The intention is that the operations of this class will clean up
 * those locks that get left set as the object goes out of scope but
 * which must remain held until the action ends otherwise serialisability
 * is compromised
 *
 */ 

class CadaverLockRecord extends LockRecord
{

public CadaverLockRecord (LockStore store, LockManager lm)
    {
	super(lm);
	
	cadaverLockStore = store;
	objectTypeName = new String(lm.type());
	
	if (((StateManagerAttribute)lm.attributes()).objectModel == ObjectModel.SINGLE)
	{
	    doRelease = false;
	}
	else
	    doRelease = true;
    }

    /*
     * Public virtual functions. These are all re-implementations of inherited
     * functions 
     */

public boolean propagateOnAbort ()
    {
	return true;
    }

    /*
     * Atomic action controlled functions. These functions create an instance
     * of CadaverLockManager to handle the lock manipulation that is needed and
     * then throw it away when done.
     */

public int nestedAbort ()
    {
	if (doRelease)
	{
	    CadaverLockManager manager = new CadaverLockManager(order(), objectTypeName);

	    return (manager.releaseAll(BasicAction.Current().get_uid()) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
	}
	else
	    return PrepareOutcome.FINISH_OK;
    }

public int nestedCommit ()
    {
	if (doRelease)
	{
	    /*
	     * Need to change the owner of the locks from the current
	     * committing action to its parent. Since no genuine LockManager
	     * exists at this time create one to take care of this.
	     */
    
	    BasicAction curr = BasicAction.Current();
    
	    CadaverLockManager manager = new CadaverLockManager(order(), objectTypeName);
	    
	    return (manager.propagate(curr.get_uid(), curr.parent().get_uid()) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
	}
	else
	    return PrepareOutcome.FINISH_OK;
    }

public int topLevelAbort ()
    {
	if (doRelease)
	{
	    CadaverLockManager manager = new CadaverLockManager(order(), objectTypeName);

	    return (manager.releaseAll(BasicAction.Current().get_uid()) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
	}
	else
	    return PrepareOutcome.FINISH_OK;
    }

public int topLevelCommit ()
    {
	if (doRelease)
	{
	    CadaverLockManager manager = new CadaverLockManager(order(), objectTypeName);

	    return (manager.releaseAll(BasicAction.Current().get_uid()) ? PrepareOutcome.FINISH_OK : PrepareOutcome.FINISH_ERROR);
	}
	else
	    return PrepareOutcome.FINISH_OK;
    }

public void print (PrintStream strm)
    {
	strm.println("CadaverLockRecord : ");
	super.print(strm);
    }

public String type ()
    {
	return "/StateManager/AbstractRecord/LockRecord/CadaverLockRecord";
    }
    
public boolean shouldReplace (AbstractRecord ar)
    {
	return (((order().equals(ar.order())) &&
		 ar.typeIs() == RecordType.LOCK ) ? true : false);
    }
    
    /*
     * Already determined that ar is a LockRecord, otherwise replace would
     * not have been called.
     * So, get the type from it before it is deleted!
     */
    
public void replace (AbstractRecord ar)
    {
	LockRecord lr = (LockRecord) ar;

	objectTypeName = lr.lockType();
    }
    
protected CadaverLockRecord ()
    {
	super();
	
	cadaverLockStore = null;
	objectTypeName = null;
	doRelease = false;
    }

private LockStore cadaverLockStore;
private String objectTypeName;
private boolean doRelease;
    
}
