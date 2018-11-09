/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Lock.java,v 1.9.2.1 1999/02/05 09:52:38 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

import com.arjuna.JavaGandiva.Common.*;
import java.io.PrintStream;

import java.io.IOException;

public class Lock extends StateManager
{

public Lock ()
    {
	super(ObjectType.NEITHER);

	currentStatus = LockStatus.LOCKFREE;
	nextLock = null;
	lMode = LockMode.WRITE;
	owners = new ActionHierarchy(0);
    }

    /*
     * Constructor : create a new Lock object and initialise it. Mode is
     * based upon argument. The value of BasicAction.Current determines the 
     * values of the remainder of the fields.
     * If there is no action running the owner field is set to be the
     * application uid created when the application starts.
     */

public Lock (int lm)
    {
	super(ObjectType.NEITHER);

	currentStatus = LockStatus.LOCKFREE;
	nextLock = null;
	lMode = lm;
	owners = new ActionHierarchy(0);

	BasicAction curr = BasicAction.Current();
    
	if (curr == null)
	{
	    int currentPid = 0;   // ::getpid();
	    ActionHierarchy ah = new ActionHierarchy(1); /* max depth of 1 */

	    if (applicUid == null)
	    {
		applicUid = new Uid();
	    }
	    
	    if (applicPid != currentPid)
	    {
		try
		{
		    Uid temp = new Uid();
	    
		    /*
		     * Process id change probably due to a fork(). Get new pid 
		     * and generate a new Applic_Uid
		     */

		    applicPid = currentPid;
		    applicUid.copy(temp);
		}
		catch (UidException e)
		{
		    System.err.println(e.getMessage());
		}
	    }

	    ah.add(applicUid);
	    owners.copy(ah);
	}
	else
	{
	    owners.copy(curr.getHierarchy());
	}
    }

    /*
     * Constructor: This is used when re-initialising a Lock after
     * retreival from the object store.
     */

public Lock (Uid storeUid)
    {
	super(storeUid, ObjectType.NEITHER);

	currentStatus = LockStatus.LOCKFREE;
	nextLock = null;
	lMode = LockMode.WRITE;
	owners = new ActionHierarchy(0);
    }

    /*
     * Destructor: General clean up as Lock is deleted.
     */

public void finalize ()
    {
	super.terminate();
    
	owners = null;
    }

    /*
     * Public utility operations. Most are sufficiently simple as to be self
     * explanatory!
     */

public final int getLockMode ()
    {
	return lMode;
    }

public final Uid getCurrentOwner ()
    {
	return owners.getDeepestActionUid();
    }

public final ActionHierarchy getAllOwners ()
    {
	return owners;
    }

public final int getCurrentStatus ()
    {
	return currentStatus;
    }

public final void changeHierarchy (ActionHierarchy newOwner)
    {
	owners.copy(newOwner);

	if (currentStatus == LockStatus.LOCKFREE)
	    currentStatus = LockStatus.LOCKHELD;
    }

public final void propagate ()
    {
	owners.forgetDeepest();
	currentStatus = LockStatus.LOCKRETAINED;
    }

public boolean modifiesObject ()
    {
	return ((lMode == LockMode.WRITE) ? true : false);
    }

    /*
     * Implementation of Lock conflict check. Returns TRUE if there is
     * conflict FALSE otherwise. Does not take account of relationship in
     * the atomic action hierarchy since this is a function of
     * LockManager.
     */

public boolean conflictsWith (Lock otherLock)
    {
	if (!(getCurrentOwner().equals(otherLock.getCurrentOwner())))
	    switch (lMode)
	    {
	    case LockMode.WRITE:
		return true;		/* WRITE conflicts always */

	    case LockMode.READ:
		if (otherLock.getLockMode() != LockMode.READ)
		    return true;
		break;
	    }
	
	return false;			/* no conflict between these locks */
    }

public boolean equals (Lock otherLock)
    {
	if ((lMode == otherLock.lMode) &&
	    (owners.equals(otherLock.owners)) &&
	    (currentStatus == otherLock.currentStatus))
	    return true;

	return false;
    }

    /*
     * functions inherited from StateManager
     */
    
public void print (PrintStream strm)
    {
	strm.println("Lock object : ");
	strm.println("\tunique id is : "+get_uid());
	
	strm.print("\tcurrent_status : ");
	LockStatus.print(strm, currentStatus);
	
	strm.print("\n\tMode : ");
	LockMode.print(strm, lMode);
    
	strm.println("\n\tOwner List : ");
	owners.print(strm);
    }

    /*
     * Carefully restore the state of a Lock.
     */

public boolean restore_state (InputObjectState os, int ot)
    {
	ActionHierarchy ah = new ActionHierarchy(0);

	try
	{
	    currentStatus = os.unpackInt();
	    lMode = os.unpackInt();
	    ah.unpack(os);
	    owners = ah;

	    return true;
	}
	catch (IOException e)
	{
	    return false;
	}
    }

    /*
     * Save the state of a lock object.
     */

public boolean save_state (OutputObjectState os, int ot)
    {
	try
	{
	    os.packInt(currentStatus);
	    os.packInt(lMode);
	    owners.pack(os);

	    return os.valid();
	}
	catch (IOException e)
	{
	    return false;
	}
    }

public String type ()
    {
	return "/StateManager/Lock";
    }

protected Lock getLink ()
    {
	return nextLock;
    }

protected void setLink (Lock pointTo)
    {
	nextLock = pointTo;
    }

private int currentStatus;	/* Current status of lock */
private Lock nextLock;
private int lMode;		/* Typically READ or WRITE */
private ActionHierarchy owners;	/* Uid of owner action (faked if none) */
    
private static Uid applicUid = null;	/* In case lock set outside AA */
private static int applicPid = 0;     /* ::getpid();	process id */
    
}
