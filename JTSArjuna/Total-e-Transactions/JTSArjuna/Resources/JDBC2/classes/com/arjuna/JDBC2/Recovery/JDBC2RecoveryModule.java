/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBC2RecoveryModule.java,v 1.1.2.4.2.2.2.1.4.2.2.5.2.1.2.2 2001/03/07 15:23:51 nmcl Exp $
 */

package com.arjuna.JDBC2.Recovery;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.jta.*;
import com.arjuna.JDBC2.*;
import com.arjuna.CosRecovery.RecoveryModule;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.*;
import javax.transaction.xa.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.io.IOException;
import java.sql.SQLException;
import javax.transaction.xa.XAException;

public class JDBC2RecoveryModule implements RecoveryModule
{

public static final String XARecoveryPropertyNamePrefix = "XAConnectionRecovery";

public JDBC2RecoveryModule ()
    {
	_xaRecoverers = new Vector();

	Properties props = PropertyManager.getProperties();

	if (props != null)
	{
	    Enumeration names = props.propertyNames();

	    while (names.hasMoreElements())
	    {
		String propName = (String) names.nextElement();

		if (propName.startsWith(JDBC2RecoveryModule.XARecoveryPropertyNamePrefix))
		{
		    /*
		     * Given the recovery string, create the class it refers to
		     * and store it.
		     */

		    String theClassAndParameter = PropertyManager.getProperty(propName);

		    // see if there is a string parameter

		    int breakPosition = theClassAndParameter.indexOf(BREAKCHARACTER);
		    
		    String theClass = null;
		    String theParameter = null;
		    
		    if (breakPosition != -1)
		    {
			theClass = theClassAndParameter.substring(0,breakPosition);
			theParameter = theClassAndParameter.substring(breakPosition+1);
		    }
		    else
		    {
			theClass = theClassAndParameter;
		    }

		    if (DebugController.enabled()) 
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_RECOVERY_NORMAL,
				     "JDBC2RecoveryModule loading " + theClass
				     + (( theParameter != null) ? theParameter : "" ) + ")" );
		    }

		    if (theClass == null)
		    {
			ErrorStream.warning().println("JDBC2RecoveryModule - could not get class name for "+propName);
		    }
		    else
		    {
			try
			{
			    Class c = Class.forName(theClass);
			    XAConnectionRecovery ri = (XAConnectionRecovery) c.newInstance();

			    if (theParameter != null)
				ri.initialise(theParameter);
			    
			    _xaRecoverers.addElement(ri);
			}
			catch (Exception e)
			{
			    ErrorStream.warning().println("Caught exception: "+e+" for "+theClass);
			}
		    }
		}
	    }
	}
	
	_objStore = new ObjectStore();
    }
    
public void periodicWorkFirstPass ()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "JDBC2RecoveryModule - first pass");
	}

	_uids = new InputObjectState();

	/*
	 * Scan for resources in the object store.
	 */

	try
	{
	    if (!_objStore.allObjUids(_type, _uids))
	    {
		ErrorStream.warning().println(id()+" could not get all object Uids.");

	    }
	}
	catch (ObjectStoreException e)
	{
	    ErrorStream.warning().println(e);

	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("JDBC2RecoveryModule.periodicWorkFirstPass exception " + e);
	    e.printStackTrace(ErrorStream.stream());
	}
    }

public void periodicWorkSecondPass ()
    {
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "JDBC2RecoveryModule - second pass");
	}
	try
	{
	    // do the recovery on anything from the scan in first pass
	    transactionInitiatedRecovery();

	    if (DebugController.enabled()) 
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			 FacilityCode.FAC_CRASH_RECOVERY,
			 "JDBC2RecoveryModule.transactionInitiatedRecovery completed");
	    }

	    /*
	     * See the comment about this routine!!
	     */

	    resourceInitiatedRecovery();

	    if (DebugController.enabled()) 
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			 FacilityCode.FAC_CRASH_RECOVERY,
			 "JDBC2RecoveryModule.resourceInitiatedRecovery completed");
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("JDBC2RecoveryModule.periodicWorkSecondPass exception " + e);
	    e.printStackTrace(ErrorStream.stream());
	}
	
	clearAllFailures();
    }

public String id ()
    {
	return "JDBC2RecoveryModule";
    }

private ObjectStore      _objStore = new ObjectStore();
private InputObjectState _uids = new InputObjectState();

private static String _type = JTA_ResourceRecord.typeName();

private final boolean transactionInitiatedRecovery ()
    {
	Uid theUid = new Uid();

	while (theUid.notEquals(Uid.nullUid()))
	{
	    try
	    {
		theUid.unpack(_uids);

		if (theUid.notEquals(Uid.nullUid()))
		{
		    boolean problem = false;
		    JDBC2RecoveryResource record = null;

		    try
		    {
			record = new JDBC2RecoveryResource(theUid);

			problem = true;

			if (record.recoverable())
			{
			    if (DebugController.enabled()) 
			    {
				DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					 FacilityCode.FAC_CRASH_RECOVERY,
					 "JDBC2Recovery attempting recovery of " + theUid);
			    }

			    if (!record.recover())
			    {
				ErrorStream.warning().println("JTA failed to recover "+theUid);
			    }
			    else
				problem = false;
			}
			else
			{
			    if (DebugController.enabled()) 
			    {
				DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								     FacilityCode.FAC_CRASH_RECOVERY,
								     "JDBC2Recovery " + theUid + " is non-recoverable");
			    }
			}
		    }
		    catch (Exception e)
		    {
			problem = true;

			ErrorStream.warning().println(e);
		    }

		    if (problem && (record != null))
		    {
			addFailure(record.getXid(),record.get_uid());
		    }
		}
	    }
	    catch (IOException e)
	    {
		theUid = Uid.nullUid();
	    }
	}

	return true;
    }

    /*
     * Now check for any outstanding transactions. If
     * we didn't fail to recover them, then roll them
     * back - if they'd got through prepare we would have an entry
     * within the object store.
     *
     * Rely upon _xaRecoverers being set up properly (via
     * properties).
     *
     * We cannot just remember the JTA_ResourceRecords we
     * used (if any) to cache the JDBC connection information and use that
     * since we may never have had any such records!
     *
     * IMPORTANT: resourceInitiatedRecovery may rollback transactions which are
     * inflight: just because we have no entry for a transaction in the
     * object store does not mean it does not exist - it may be *about* to
     * write its intentions list. To try to reduce this probability we
     * remember potential rollback-ees at this scan, and wait for the next
     * scan before actually rolling them back.
     *
     * Note we cannot use the method that works with Transctions and
     * TransactionalObjects, of checking with original process that created
     * the transaction, because we don't know which process it was.
     */

private final boolean resourceInitiatedRecovery ()
    {
	/*
	 * Now any additional connections we may need to create.
	 * Relies upon information provided by the application.
	 */

	if (_xaRecoverers.size() > 0)
	{
	    for (int i = 0; i < _xaRecoverers.size(); i++)
	    {
		XAResource resource = null;
		XAConnectionRecovery ri = (XAConnectionRecovery) _xaRecoverers.elementAt(i);

		try
		{
		    while (ri.hasMoreConnections())
		    {
			try
			{
			    resource = ri.getConnection().recoveryConnection().getResource();
			    
			    xaRecovery(resource);
			}
			catch (Exception exp)
			{
			    ErrorStream.warning().println("Caught: "+exp);
			}
		    }
		}
		catch (Exception ex)
		{
		    ErrorStream.warning().println(ex);
		}
	    }
	}

	return true;
    }

private final boolean xaRecovery (XAResource xares)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
		     FacilityCode.FAC_CRASH_RECOVERY,
		     "xarecovery of " + xares);
	}

	try
	{
	    Xid[] trans = null;

	    try
	    {
		trans = xares.recover(XAResource.TMSTARTRSCAN);

		if (DebugController.enabled()) 
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_CRASH_RECOVERY,
							 "found " + trans.length + " xids in doubt");
		}
	    }
	    catch (XAException e)
	    {
		ErrorStream.warning().println("JDBC2RecoveryModule.xaRecovery " + e  + ", " +
					      JTA_Utility.printXAErrorCode(e));

		e.printStackTrace(ErrorStream.stream());

		try
		{
		    xares.recover(XAResource.TMENDRSCAN);
		}
		catch (Exception e1)
		{
		}

		return false;
	    }

	    /*
	     * Rather than recover now, wait until the next scan to
	     * give any transactions a chance to finish.
	     */

	    if (_scanN == null)
	    {
		_scanN = trans;
	    }
	    else
	    {
		_scanN = _scanM;
		_scanM = trans;
	    }

	    Object[] xids = toRecover();

	    if (xids != null)
	    {
		for (int j = 0; j < xids.length; j++)
		{
		    boolean doForget = false;

		    /*
		     * Check if in failure list.
		     */

		    Uid recordUid = null;

		    do
		    {
			// is the xid known to be one that couldn't be recovered
			recordUid = previousFailure((Xid) xids[j]);

			if (recordUid == null)
			{
			    /*
			     * It wasn't an xid that we couldn't recover, so
			     * the RM knows about it, but we don't. Therefore
			     * it should be rolled back.
			     */

			    try
			    {
				if (DebugController.enabled()) 
				{
				    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					     FacilityCode.FAC_RECOVERY_NORMAL,
					     "Rolling back " + JTA_Utility.xidToString((Xid) xids[j]));
				}

				xares.rollback((Xid) xids[j]);
			    }
			    catch (XAException e1)
			    {
				switch (e1.errorCode)
				{
				case XAException.XAER_RMERR:
				    break;
				case XAException.XA_HEURHAZ:
				case XAException.XA_HEURCOM:
				case XAException.XA_HEURMIX:
				case XAException.XA_HEURRB:
				case XAException.XA_RBROLLBACK:
				    {
					if (!doForget)  // already done?
					    doForget = true;
				    }
				    break;
				default:
				    break;
				}
			    }
			    catch (Exception e2)
			    {
				ErrorStream.warning().println("JDBC2RecoveryModule.xaRecovery " + e2);
				e2.printStackTrace(ErrorStream.stream());
			    }
			}
			else
			{
			    /*
			     * In the failures list so it may be that
			     * we just need another XAResource to be
			     * able to recover this.
			     */

			    JDBC2RecoveryResource record = new JDBC2RecoveryResource(recordUid, xares);

			    if (!record.recover())
				ErrorStream.warning().println("JDBC2RecoveryModule - failed to recover XAResource.");

			    removeFailure(record.getXid(), record.get_uid());
			}

			if (doForget)
			{
			    try
			    {
				xares.forget((Xid) xids[j]);
			    }
			    catch (Exception e)
			    {
				ErrorStream.warning().println("JDBC2RecoveryModule.xaRecovery " + e);
				e.printStackTrace(ErrorStream.stream());
			    }
			}

		    } while (recordUid != null);
		}
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("JDBC2RecoveryModule.xaRecovery " + e);
	    e.printStackTrace(ErrorStream.stream());
	}

	try
	{
	    if (xares != null)
		xares.recover(XAResource.TMENDRSCAN);
	}
	catch (XAException e)
	{
	    ErrorStream.warning().println("JDBC2RecoveryModule.xaRecovery " + e);
	    e.printStackTrace(ErrorStream.stream());
	}

	return true;
    }

    /*
     * Go through the current list of inflight transactions and look for
     * the same entries in the previous scan. If we find them, then we will
     * try to recover them. Then move the current list into the old list
     * for next time. We could prune out those transactions we manage to
     * recover, but this will happen automatically at the next scan since
     * they won't appear in the next new list of inflight transactions.
     */

private final java.lang.Object[] toRecover ()
    {
	Vector workingVector = new Vector();
	boolean finished = false;

	for (int i = 0; !finished; i++)
	{
	    try
	    {
		if (JTA_Utility.sameXID(_scanN[i], _scanM[i]))
		    workingVector.add(_scanN[i]);
	    }
	    catch (ArrayIndexOutOfBoundsException ex)
	    {
		finished = true;
	    }
	    catch (NullPointerException e)
	    {
		// first scan, so do nothing

		return null;
	    }
	}

	return workingVector.toArray();
    }

    /**
     * Is the Xid is in the failure list, i.e., the list
     * of those transactions we couldn't recover, possibly because of transient
     * failures. If so, return the uid of (one of) the records and remove it
     * from the list. 
     */

private final Uid previousFailure (Xid xid)
    {    
	if (_failures == null) {
	    return null;
	}
	Enumeration e = _failures.keys();

	while (e.hasMoreElements())
	{
	    Xid theXid = (Xid) e.nextElement();

	    if (JTA_Utility.sameXID(xid, theXid))
	    {
		// remove uid from failure list
		Vector failureItem = (Vector) _failures.get(theXid);
		Uid u = (Uid) failureItem.remove(0);

		if (failureItem.size() == 0)
		    _failures.remove(failureItem);

		return u;
	    }
	}

	// not present in the failures list.

	return null;
    }

/* methods to manipulate the failure list */

/** 
 * Add record to failure list
 */
private void addFailure(Xid xid,Uid uid)
{
    if (_failures == null)
	_failures = new Hashtable();

    Vector failureItem = (Vector) _failures.get(xid);

    if (failureItem == null)
    {
	failureItem = new Vector();

	_failures.put(xid, failureItem);
    }

    failureItem.addElement(uid);
}

/** remove record uid from failure list */
private void removeFailure(Xid xid, Uid uid)
{
    // find the failure item for this xid    
    Vector failureItem = (Vector) _failures.get(xid);

    if (failureItem == null)
    {
	ErrorStream.warning().println("JDBC2RecoveryModule - could not remove record for "+xid);
    }
    else
    {
	// remove this record from the item
	failureItem.remove(uid);

	// if that was the last one, remove the item altogether
	if (failureItem.size() == 0)
	    _failures.remove(failureItem);
    }
}

private void clearAllFailures()
{
    if (_failures != null)
    _failures.clear();
}

private Vector    _xaRecoverers = null;
private Hashtable _failures = null;
private Xid[]     _scanN = null;
private Xid[]     _scanM = null;

private static int _backoffPeriod = 0;

private static final int JDBC2_BACKOFF_PERIOD = 20000; // backoff in milliseconds
private static final char BREAKCHARACTER = ';';  // delimiter for xaconnrecov property

    static
    {
	String env = PropertyManager.getProperty(JDBC2Environment.JDBC2_BACKOFF_PERIOD);

	JDBC2RecoveryModule._backoffPeriod = JDBC2_BACKOFF_PERIOD;

	if (env != null)
	{
	    try
	    {
		Integer i = new Integer(env);

		JDBC2RecoveryModule._backoffPeriod = i.intValue();
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println(e.toString());
	    }
	}
    }

}
