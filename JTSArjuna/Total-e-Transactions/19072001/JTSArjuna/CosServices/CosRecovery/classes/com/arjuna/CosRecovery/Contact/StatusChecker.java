/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: StatusChecker.javatmpl,v 1.1.2.2.2.2.2.1.12.1.2.2.26.1 2001/08/02 12:06:00 nmcl Exp $
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
































































package com.arjuna.CosRecovery.Contact;

import java.util.Hashtable;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.ArjunaCore.Atomic.InitAction;
import com.arjuna.OrbCommon.event.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import com.arjuna.CosTransactions.OTS_Utility;
import com.arjuna.CosTransactions.OTS;

import org.omg.CosTransactions.*;
import org.omg.CORBA.NO_IMPLEMENT;
import org.omg.CORBA.OBJECT_NOT_EXIST;
import org.omg.CORBA.COMM_FAILURE;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.TRANSIENT;
import org.omg.CORBA.SystemException;
import org.omg.CosTransactions.Inactive;

/**
 * Checks the status of a transaction as known to the original process that
 * created it - assuming the transaction still exists.
 *
 *
 * (relies on the fact (true for 2.1) that any ArjunaFactory can be used to 
 * find the status of any transaction.
 *
 * Singleton class 
 */

public class StatusChecker
{
  /* the interface of this class is intended to allow for possible 
   * future extension to do statuschecking by other (non-corba) means
   *
   * On the other hand, there is some nasty interlinking between bits of this
   * class - some redesign into separate classes would make things easier to 
   * follow.
   */
   
 // lookup the relevant factory according to the uid of the FactoryContactItem
private Hashtable _itemFromUid;

private static StatusChecker _checker;

/**
 * create a static singleton to do the work for the static methods
 */
static 
{
    _checker = new StatusChecker();
}


/**
 * get the status in the original process, given the uid of the contact
 * item (which is the uid of the process)
 */
public static Status get_status(Uid transactionUid, Uid itemUid) throws Inactive
{
    return _checker.checkOriginalStatus(transactionUid, itemUid);
}

/**
 * why isn't this private
 */
public StatusChecker()
{
    _itemFromUid = new Hashtable();
}


/**
 * Check the status of a transaction when the contact item uid is known.
 * This method *must* only be called from replay_completion, since it
 * relies upon this fact to differentiate between a committed or rolled back
 * transaction in the event of finding no intentions list in the object
 * store.
 *
 * @return the status of the transaction as known in the original process.
 * @throws Inactive if the original process is no longer active.
 */

public Status checkOriginalStatus (Uid transactionUid, Uid itemUid) throws Inactive
{
    FactoryContactItem item = getItem(itemUid);

    if (item != null)
    {
	return getStatus(transactionUid, item);
    }
    else
    {
	// null item implies long-dead process

	throw new Inactive();
    }
}


/**
 *  try to get the status from a factory and convert to our way.
 *  factory must not be null
 *  itemUid is the store id as in _itemFromUid
 */

private Status getStatus (Uid transactionUid, FactoryContactItem item) throws Inactive
{
    ArjunaFactory factory = item.getFactory();

    if (factory != null)
    {
	Status otsStatus = Status.StatusUnknown;
	boolean originalDead = false;

	try
	{
	    otid_t otid = OTS_Utility.uidToOtid(transactionUid);
	    
	    otsStatus = factory.getCurrentStatus(otid);

	    /*
	     * If the factory doesn't know about the transaction, then
	     * check the object store for the intentions list. If not
	     * present, then the transaction must have rolled back.
	     * If present, then we don't know what's going on, since the
	     * factory should still have a reference to the transaction!
	     */

	    if (otsStatus == Status.StatusNoTransaction)
	    {
		otsStatus = factory.getStatus(otid);

		if ((otsStatus == Status.StatusNoTransaction) ||
		    (otsStatus == Status.StatusUnknown))
		{
		    return otsStatus;
		}
		else
		{
		    /*
		     * We got an answer! This probably means that the factory
		     * has just finished with the transaction, but the state
		     * hasn't been removed by the file system yet - we don't
		     * sync the removal to improve performance.
		     */

		    ErrorStream.warning().println("StatusChecked.getStatus - found intentions list for apparently unknown transaction: "+transactionUid);
		    
		    otsStatus = Status.StatusUnknown;
		}
	    }
	    
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "StatusChecker.getStatus("+transactionUid+") - status = "+OTS_Utility.printStatus(otsStatus));
	    }
	    item.markAsAlive();
	} catch ( NO_IMPLEMENT ex_noimp) {
	    // the original application has died
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "StatusChecker.getStatus("+transactionUid+") - NO_IMPLEMENT = dead");
	    }
	    originalDead = true;

	// orbix seems to count unreachable as transient. Over infinite time, all
	// addresses are valid
	} catch ( TRANSIENT ex_trans) {
	    // the original application has (probably) died
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "StatusChecker.getStatus("+transactionUid+") - TRANSIENT = dead");
	    }
	    originalDead = true;

	} catch ( COMM_FAILURE ex_comm) {
	    /*
	     * Probably the original application has died, but only just - do not mark
	     * either way.
	     */

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "StatusChecker.getStatus("+transactionUid+") - COMM_FAILURE = live");
	    }
	} catch ( OBJECT_NOT_EXIST ex_noobj) {
	    // the original process must have gone away, and another one
	    // come up in the same place
	    // (or, just possibly, the original closed the ots)
	    originalDead = true;
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "StatusChecker.getStatus("+transactionUid+") - OBJECT_NOT_EXIST = dead");
	    }
	} catch ( BAD_PARAM ex_badparam) {
	    ErrorStream.warning().println("BAD_PARAM exception on trying to contact original process");
	    // the transactionUid is invalid !
	} catch ( NoTransaction ex_notran) {
	    ErrorStream.warning().println("NoTransaction exception on trying to contact original process");
	    // the transactionUid is invalid !
	    // no transaction 
	} catch ( SystemException ex_corba ) {
	    // why did this happen ?
	    ErrorStream.warning().println("CORBA exception on trying to contact original process " + ex_corba);
	} catch ( Exception ex_other) {
	    // this really shouldn't happen
	    ErrorStream.warning().println("Exception on trying to contact original process " +ex_other);
	    ex_other.printStackTrace(ErrorStream.stream());
	}
    
    
	if (originalDead)
	{
	    item.markAsDead();

	    // use Inactive as an indication that the parent process
	    // has gone
	    
	    throw new Inactive();
	}
	else
	{
	    return otsStatus;
	}
    }
    else
    {
	// factory in item is null - process already dead
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_CRASH_RECOVERY,
			     "StatusChecker.getStatus("+transactionUid+") - no factory, process previously dead");
	}

	/*
	 * In which case we can use the current, in process local factory, to
	 * look at the object store and get the status from that. At present
	 * all factories can look at the entire object store on a machine, so
	 * this will work. If a factory is limited to only a portion of the object
	 * store then we may need to create an explicit factory that has "global"
	 * knowledge.
	 */

	try
	{
	    return OTS.factory().getStatus(transactionUid);
	}
	catch (NoTransaction e1)
	{
	    return Status.StatusNoTransaction;
	}
	catch (SystemException e2)
	{
	    return Status.StatusUnknown;
	}
    }
}


/**
 * find the IOR for the ArjunaFactory whose FactoryContactItem was saved with 
 * this uid. It is possible this Uid was created after the last scan, so if
 * it isn't in the hashtable, look for real directly
 */
 
private FactoryContactItem getItem (Uid uid)
{
    FactoryContactItem theItem = null;
    theItem = getKnownItem(uid);
    if (theItem == null) {
	// not previously known - see if it exists now
	theItem = getNewItem(uid);
	if (theItem == null) {
	    
	    // if it's still null, either something has gone wrong 
	    // - how did it get in the recoverycoordkey when the 
	    //   factory was unknown
	    // or it's very old and been fully deleted
	    ErrorStream.warning().println("no known contactitem for " + uid);
	    // treat as long-dead process - return null
	}
    }

    return theItem;
}

private FactoryContactItem getKnownItem(Uid uid)
{
    FactoryContactItem theItem = null;
    try {
	theItem = (FactoryContactItem) _itemFromUid.get(uid);
	return theItem;
    } catch (ClassCastException ex) {
	ErrorStream.warning().println("surprise item in StatusChecker list for "
	     + uid + ", " + ex);
	return null;
    }
}

private FactoryContactItem getNewItem(Uid uid)
{
    FactoryContactItem item = FactoryContactItem.recreate(uid);
    if (item != null) {
	// enter in the uid hashtable
	_itemFromUid.put(uid,item);
    }
    return item;			
}


}
