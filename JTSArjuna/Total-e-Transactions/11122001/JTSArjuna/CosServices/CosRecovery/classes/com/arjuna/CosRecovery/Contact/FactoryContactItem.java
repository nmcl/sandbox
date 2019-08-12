/*
 * Copyright (C) 2001
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: FactoryContactItem.java,v 1.1.2.3.20.1.36.1 2001/11/05 12:36:18 nmcl Exp $
 */

package com.arjuna.CosRecovery.Contact;

import java.util.Date;
import java.text.*;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.ArjunaCore.Atomic.InitAction;
import com.arjuna.OrbCommon.event.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Common.*;

/**
 * Create a persistent entry in the ActionStore to allow the RecoveryManager to
 * know which ArjunaFactory is in which JVM.
 *
 * (relies on the fact (true for 2.1) that any ArjunaFactory can be used to 
 * find the status of any transaction.
 *
 * Identifying uid is the processUid
 */

public class FactoryContactItem 
{

private static final String _pseudoTypeName = "/Recovery/FactoryContact";
private static final int    version = 1;
private static ObjectStore  _objectStore = null;

private static FactoryContactItem _theSingularItem = null;

private Date	    _creationTime = null;
private Date	    _aliveTime = null;
private Date	    _deadTime = null;
// if this is null, the parent process is known to be deceased
private ArjunaFactory    _factory = null;
private Uid		_uid = null;

/**
 * create the contact item for the factory in this address space
 * and persist it
 *
 * Used in the original transaction-initiating process
 */
static boolean createAndSave(ArjunaFactory factory)
{
    if (_theSingularItem == null) {
	_theSingularItem = new FactoryContactItem(factory);
	return _theSingularItem.saveMe();
    } else {
	// do it only once
	return true;
    }
}


/**
 * construct the item known by this uid from the ObjectStore (assuming
 * it is there - otherwise return null
 *
 * Used in the RecoveryManager
 */
 
static FactoryContactItem recreate(Uid uid)
{
    FactoryContactItem theItem = new FactoryContactItem(uid);
    
    if (theItem.restoreMe()) {
	return theItem;
    } else {
	return null;
    }
}

/*  provide information for other classes (and ourselves) in this package
 */
static ObjectStore getStore()
{
    if ( _objectStore == null ) {
	_objectStore = InitAction.getStore();
    }
    return _objectStore;
}


static String getTypeName()
{
    return _pseudoTypeName;
}


/**
 * accessor
 */
ArjunaFactory getFactory()
{
    return _factory;
}

/**
 * accessor
 */
Date getCreationTime()
{
    return _creationTime;
}

/**
 * accessor - returns null if contact has not be successfully used
 *	 this run of RecoveryManager
 */
Date getAliveTime()
{
    return _aliveTime;
}

/**
 * accessor - returns null if contact has not failed at some time in the
 *      past (persists)
 */
Date getDeadTime()
{
    return _deadTime;
}

/**
 * accessor
 */
Uid getUid()
{
    return _uid;
}

/**
 * the address space this contact item points to has gone away
 */
void markAsDead()
{
    // ignore if done previously
    if (_factory != null) {
	// the ior won't work any more, so forget it
	_factory = null;
	_deadTime = new Date();
	saveMe();
    }
}

/**
 * the address space this contact item points to has just been contacted
 * this information is NOT persisted
 */
void markAsAlive()
{
    _aliveTime = new Date();
}

/**
 *  Constructor used in normal Arjuna Transaction service application to
 *  identify the (or an) ArjunaFactory in this process
 */
 
private FactoryContactItem(ArjunaFactory factory)
{
    if (DebugController.enabled())
    {
	DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					     FacilityCode.FAC_CRASH_RECOVERY,
					     "FactoryContactItem(factory)");
    }
    // the Uid of this object is the Uid of the process that creates it by
    // this constructor.
    //  full class name needed to disambiguate from java.text.Utility
    _uid   = com.arjuna.ArjunaCommon.Common.Utility.getProcessUid();;
    _factory = factory;
    _creationTime = new Date();
}

/**
 * Constructor used in RecoveryManager to restore a contact item from the 
 * the object store
 */
 
private FactoryContactItem(Uid uid)
{
    _uid = new Uid(uid);
}

/**
  * Although FactoryContactItem is not derived from StateManager, this
  * method has the equivalent signature and purpose (but different access)
  */
private boolean save_state(OutputObjectState objstate)
{
    // convert the information
    try {
    
	objstate.packInt(version);
	objstate.packLong(_creationTime.getTime());

	if (_factory != null) {
	    String iorAsString = ORBInterface.orb().object_to_string(_factory);
	    objstate.packString(iorAsString);
	} else {
	    objstate.packString("");
	    objstate.packLong(_deadTime.getTime());
	}
	return true;
    }
    catch (java.io.IOException ex)
    {
	ErrorStream.warning().println("Problem with storing process/factory link "
				      + ex);
    }
    catch (Exception exp)
    {
	ErrorStream.warning().println("Problem with storing process/factory link "
				      + exp);
    }

    return false;
}

/**
  * Although FactoryContactItem is not derived from StateManager, this
  * method has the equivalent signature and purpose (but different access)
  */
private boolean restore_state(InputObjectState objstate)
{
    // convert the information
    try {
	int oldversion = objstate.unpackInt();
	if (oldversion != version) {
	    ErrorStream.warning().println("Attempted to read FactoryContactItem of different version");
	}
	long oldtime = objstate.unpackLong();
	_creationTime = new Date(oldtime);
	String iorAsString = objstate.unpackString();
	if (iorAsString.length() > 1) {
	    org.omg.CORBA.Object corbject = ORBInterface.orb().string_to_object(iorAsString);
		
	    _factory = ArjunaFactoryHelper.narrow(corbject);
	    if (_factory == null) {
		ErrorStream.warning().println("Stored IOR is not an ArjunaFactory");
	    }
	    _deadTime = null;
	} else {
	    _factory = null;
	    oldtime = objstate.unpackLong();
	    _deadTime = new Date(oldtime);
	}
	_aliveTime = null;
	return true;
    }
    catch (java.io.IOException ex)
    {
	ErrorStream.warning().println("Problem with restoring process/factory link "
				      + ex);
    }
    catch (Exception exp)
    {
	ErrorStream.warning().println("Problem with restoring process/factory link "
				      + exp);	    
    }
    
    return false;
}

private boolean saveMe()
{
    try {
	OutputObjectState objstate = new OutputObjectState();
	if (save_state(objstate)) {
	    getStore().write_committed(_uid, _pseudoTypeName, objstate);
	    return true;
	}
    } catch (ObjectStoreException exo) {
	ErrorStream.warning().println("Problem with storing process/factory link "
		+ exo);
    }
    return false;
}

private boolean restoreMe()
{
    try {
		    
	InputObjectState objstate = getStore().read_committed(_uid, _pseudoTypeName);
	
	if ( restore_state(objstate)) {
	    return true;
	}
	ErrorStream.warning().println("Problem with restoring process/factory link ");
    } catch (ObjectStoreException exo) {
	// this shouldn't happen, because we shouldn't be looking for a factory
	// that was never recorded
	ErrorStream.warning().println("Problem with storing process/factory link "
		+ exo);
    }
    return false;
}

private static boolean removeMe(Uid uid)
{
    try {
	return getStore().remove_committed(uid, _pseudoTypeName);
    } catch (ObjectStoreException exo) {
	// this shouldn't happen, because we shouldn't be looking for a factory
	// that was never recorded
	ErrorStream.warning().println("Problem with removing contact item " 
		+ exo);
    }
    return false;
}

}




