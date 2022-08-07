/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Hewlett Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTA_Transaction.javatmpl,v 1.3.4.1.4.4.2.1.2.2.4.6.8.3.2.5.2.4.8.1.2.1.20.1.4.1.4.1 2001/07/31 12:40:51 nmcl Exp $
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































































/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/




package com.arjuna.jta;

import com.arjuna.ArjunaCommon.Common.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.*;
import javax.transaction.*;
import javax.transaction.xa.*;
import java.util.Hashtable;
import java.util.Enumeration;

import javax.transaction.NotSupportedException;
import javax.transaction.RollbackException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.HeuristicRollbackException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.SystemException;
import java.lang.SecurityException;
import java.lang.IllegalStateException;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NoTransaction;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.InvalidControl;
import org.omg.CORBA.NO_PERMISSION;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.UNKNOWN;

/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.1.2.1.28.1.14.1 2001/03/20 16:24:46 nmcl Exp $
 */






/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;




/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_Exceptions.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * The Minor version numbers for some system exceptions
 * we may raise.
 */




/*
 * UNKNOWN
 */







/*
 * BAD_OPERATION
 */









/*
 * BAD_PARAM
 */




/*
 * TRANSACTION_ROLLEDBACK
 */






/*
 * INVALID_TRANSACTION
 */









/*
 * WRONG_TRANSACTION
 */





/*
 * TRANSACTION_REQUIRED
 */







/**
 * An implementation of javax.transaction.Transaction.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JTA_Transaction.javatmpl,v 1.3.4.1.4.4.2.1.2.2.4.6.8.3.2.5.2.4.8.1.2.1.20.1.4.1.4.1 2001/07/31 12:40:51 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class JTA_Transaction implements javax.transaction.Transaction
{

public static final int XACONNECTION = 0;
public static final int XAMODIFIER = 1;

public static JTA_Transaction create (org.omg.CosTransactions.Control ctx)
    {
	return new JTA_Transaction(new OTS_ControlWrapper(ctx));
    }

public static JTA_Transaction create (org.omg.CosTransactions.Control ctx, Uid u)
    {
	return new JTA_Transaction(ctx, u);
    }
    
    /**
     * Overloads Object.equals()
     */
    
public boolean equals (Object obj)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.OPERATORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Transaction.equals");
	}

	if (obj == null)
	    return false;

	if (obj == this)
	    return true;
	
	if (obj instanceof JTA_Transaction)
	{
	    /*
	     * If we can't get either coordinator to compare,
	     * then assume transactions are different.
	     */

	    try
	    {
		JTA_Transaction tx = (JTA_Transaction) obj;
		OTS_ControlWrapper txControl = tx.getControlWrapper();

		if ((_theControl == null) && (txControl == null))
		    return true;
		else
		    return _theControl.equals(txControl);
	    }
	    catch (Exception e)
	    {
	    }
	}
	
	return false;
    }

    /**
     * Return -1 if we fail.
     */

public int hashCode ()
    {
	try
	{
	    return _theControl.hash_transaction();
	}
	catch (Exception e)
	{
	    return -1;
	}
    }
    
public OTS_ControlWrapper getControlWrapper ()
    {
	return _theControl;
    }

    /**
     * The JTA specification is vague on whether the calling thread
     * can have any transaction associated with it. It does say that
     * it need not have the same transaction as this one. We could
     * call suspend prior to making these calls, but for now we do
     * nothing, and simply treat it like a Control.
     */
    
    /**
     * We will never throw a HeuristicRollbackException because if we
     * get a HeuristicRollback from a resource, and can successfully
     * rollback the other resources, this is then the same as having
     * simply been forced to rollback the transaction during phase 1.
     * The OTS interfaces do not allow a differentiation.
     */
    
public void commit () throws javax.transaction.RollbackException,
	                     javax.transaction.HeuristicMixedException,
	                     javax.transaction.HeuristicRollbackException, java.lang.SecurityException,
                             javax.transaction.SystemException, java.lang.IllegalStateException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Transaction.commit");
	}

	if (_theControl != null)
	{
	    try
	    {
		_theControl.commit(true);
	    }
	    catch (org.omg.CosTransactions.Unavailable e1)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }
	    catch (org.omg.CosTransactions.HeuristicMixed e2)
	    {
		throw new javax.transaction.HeuristicMixedException("JTA_HeuristicType.HEURISTIC_MIXED");
	    }
	    catch (org.omg.CosTransactions.HeuristicHazard e3)
	    {
		throw new javax.transaction.HeuristicMixedException("JTA_HeuristicType.HEURISTIC_HAZARD");
	    }
	    catch (TRANSACTION_ROLLEDBACK  e4)
	    {
		throw new RollbackException();
	    }
	    catch (org.omg.CORBA.NO_PERMISSION e5)
	    {
		throw new SecurityException();
	    }

	    catch (INVALID_TRANSACTION  e6)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }

	    catch (org.omg.CORBA.SystemException e7)
	    {
		throw new javax.transaction.SystemException();
	    }
	}
	else
	    throw new IllegalStateException("Transaction is not active");
    }

public void rollback () throws java.lang.IllegalStateException, java.lang.SecurityException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Transaction.rollback");
	}

	if (_theControl != null)
	{
	    try
	    {
		_theControl.rollback();
	    }	
	    catch (org.omg.CosTransactions.Unavailable e1)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }
	    catch (org.omg.CORBA.NO_PERMISSION e2)
	    {
		throw new SecurityException();
	    }

	    catch (INVALID_TRANSACTION  e3)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }

	    catch (NoTransaction e4)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }
	    catch (org.omg.CORBA.SystemException e5)
	    {
		throw new javax.transaction.SystemException();
	    }
	}
	else
	    throw new IllegalStateException("Transaction is not active");
    }

public void setRollbackOnly () throws java.lang.IllegalStateException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Transaction.setRollbackOnly");
	}

	if (_theControl != null)
	{
	    try
	    {
		_theControl.rollback_only();
	    }
	    catch (org.omg.CosTransactions.Inactive e1)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }
	    catch (org.omg.CosTransactions.Unavailable e2)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }
	    catch (org.omg.CosTransactions.NoTransaction e3)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }
	    catch (org.omg.CORBA.SystemException e4)
	    {
		throw new javax.transaction.SystemException();
	    }
	}
	else
	    throw new IllegalStateException("Transaction is not active");
    }

public int getStatus () throws javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Transaction.getStatus");
	}

	int status = javax.transaction.Status.STATUS_NO_TRANSACTION;
	
	if (_theControl != null)
	{
	    try
	    {
		return JTA_Utility.convertStatus(_theControl.get_status());
	    }
	    catch (org.omg.CORBA.SystemException e2)
	    {
		throw new javax.transaction.SystemException();
	    }
	}

	return status;
    }

public void registerSynchronization (javax.transaction.Synchronization sync) throws javax.transaction.RollbackException, java.lang.IllegalStateException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Transaction.registerSynchronization");
	}

	if (sync == null)
	    throw new javax.transaction.SystemException("JTA_Transaction.registerSynchronization - null parameter!");

	if (_theControl != null)
	{
	    try
	    {
		JTA_Synchronization s = new JTA_Synchronization(sync);

		_theControl.register_synchronization(s.getSynchronization());
	    }

	    catch (TRANSACTION_ROLLEDBACK  e2)
	    {
		throw new javax.transaction.RollbackException();
	    }

	    catch (org.omg.CosTransactions.Inactive e3)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }
	    catch (org.omg.CosTransactions.SynchronizationUnavailable e4)
	    {
		throw new IllegalStateException("Synchronizations are not allowed!");
	    }
	    catch (INVALID_TRANSACTION  e5)
	    {
		throw new IllegalStateException("Transaction is not active");
	    }
	    catch (org.omg.CORBA.SystemException e6)
	    {
		throw new javax.transaction.SystemException();
	    }
	}
	else
	    throw new IllegalStateException("Transaction is not active");
    }

public boolean enlistResource (XAResource xaRes) throws RollbackException, IllegalStateException, javax.transaction.SystemException
    {
	return enlistResource(xaRes, null);
    }
    
public synchronized boolean enlistResource (XAResource xaRes, Object[] params) throws RollbackException, IllegalStateException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Transaction.enlistResource ( "+xaRes+" )");
	}

	if (xaRes == null)
	    throw new javax.transaction.SystemException("JTA_Transaction.enlistResource - resource is null!");

	int status = getStatus();

	switch (status)
	{
	case javax.transaction.Status.STATUS_MARKED_ROLLBACK:
	    throw new RollbackException();
	case javax.transaction.Status.STATUS_ACTIVE:
	    break;
	default:
	    throw new IllegalStateException("Transaction is not active");
	}

	JTA_XAModifier theModifier = null;

	if (params != null)
	{
	    if (params.length >= XAMODIFIER+1)
	    {
		if (params[XAMODIFIER] instanceof JTA_XAModifier)
		{
		    theModifier = (JTA_XAModifier) params[XAMODIFIER];
		}
	    }
	}
	    
	try
	{
	    /*
	     * For each transaction we maintain a list of resources
	     * registered with it. Each element on this list also
	     * contains a list of threads which have registered this
	     * resource, and what their XID was for that registration.
	     */

	    TxInfo info = null;

	    /*
	     * Have we seen this specific resource instance before?
	     * Do this trawl first before checking the RM instance
	     * later. Saves time.
	     */

	    try
	    {
		info = (TxInfo) _resources.get(xaRes);
		
		if (info == null)
		{
		    /*
		     * Null info means it's not in the main resources list, but
		     * may be in the duplicates.
		     */

		    info = (TxInfo) _duplicateResources.get(xaRes);
		}
		
		if (info != null)
		{
		    switch (info.getState())
		    {
		    case TxInfo.ASSOCIATION_SUSPENDED:
			{
			    /*
			     * Have seen resource before, so do a resume.
			     * The Resource instance will still be registered
			     * with the transaction though.
			     */

			    xaRes.start(info.xid(), XAResource.TMRESUME);

			    info.setState(TxInfo.ASSOCIATED);
			
			    return true;  // already registered resource with this transaction!
			}
		    case TxInfo.ASSOCIATED:
			{
			    /*
			     * Already active on this transaction.
			     */
			    
			    return true;
			}
		    case TxInfo.NOT_ASSOCIATED:
			{
			    /*
			     * Resource was associated, but was presumably
			     * delisted.
			     */

			    xaRes.start(info.xid(), XAResource.TMJOIN);
			    
			    info.setState(TxInfo.ASSOCIATED);
			
			    return true;
			}
		    default:
			{
			    // Note: this exception will be caught by our catch block

			    throw new IllegalStateException("JTA_Transaction.enlistResource - illegal resource state: "+info.getState());
			}
		    }
		}
	    }
	    catch (IllegalStateException ex)
	    {
		throw ex;  // we threw it in the first place
	    }
	    catch (XAException exp)
	    {
		if (info != null)
		    info.setState(TxInfo.FAILED);

		ErrorStream.warning().println("JTA_Transaction.enlistResource - caught: "+JTA_Utility.printXAErrorCode(exp));
		
		return false;
	    }

	    //	    if (threadIsActive(xaRes))
	    //		return true;  // this thread has already registered a resource for this db
	    
	    /*
	     * We definitely haven't seen this specific resource instance
	     * before, but that doesn't mean that we haven't seen the RM it
	     * is connected to.
	     */

	    Xid xid = null;

	    if (isNewRM(xaRes))
	    {
		/*
		 * New RM, so create xid with new branch.
		 */

		boolean branchRequired = true;
		
		if (_resources.size() == 0)// first ever, so no need for branch
		{
		    //		    branchRequired = false;
		    branchRequired = true;
		}

		xid = createXid(branchRequired, theModifier);

		boolean associatedWork = false;
		int retry = 20;

		/*
		 * If another process has (or is about to) create the same
		 * transaction association then we will probably get a failure
		 * during start with XAER_DUPID. We know this must be due to
		 * another server, since we keep track of our own
		 * registrations.
		 * So, if this happens we create a new transaction branch and
		 * try again.
		 *
		 * To save time we could always just create branches by
		 * default.
		 *
		 * Is there a benefit to a zero branch?
		 */

		while (!associatedWork)
		{
		    try
		    {
			xaRes.start(xid, XAResource.TMNOFLAGS);

			associatedWork = true;

			_resources.put(xaRes, new TxInfo(xid));
		    }
		    catch (XAException e)
		    {
			// transaction already created by another server

			if (e.errorCode == XAException.XAER_DUPID)
			{
			    if (retry > 0)
				xid = createXid(true, theModifier);

			    retry--;
			}
			else
			{
			    /*
			     * Can't do start, so set transaction to
			     * rollback only.
			     */

			    ErrorStream.warning().println("JTA_Transaction.enlistResource - XAResource.start returned: "+JTA_Utility.printXAErrorCode(e));

			    markRollbackOnly();

			    throw e;
			}
		    
			if (retry < 0)
			{
			    ErrorStream.warning().println("JTA_Transaction.enlistResource - XAResource.start could not register transaction!");

			    markRollbackOnly();
			    
			    throw new UNKNOWN();
			}
		    }
		}
	    }
	    else
	    {
		/*
		 * Have seen this RM before, so ignore this instance.
		 * The first registered RM instance will be used to drive
		 * the transaction completion.
		 * We add it to the duplicateResource list so we can delist
		 * it correctly later though.
		 */

		/*
		 * Re-create xid with zero branch.
		 */

		xid = new JTA_XID(_theControl, false);

		if (theModifier != null)
		{
		    try
		    {
			xid = theModifier.createXid((JTA_XID) xid);
		    }
		    catch (Exception e)
		    {
			ErrorStream.warning().println("JTA_Transaction.enlistResource - could not create XID: "+e);
			
			return false;
		    }
		}

		try
		{
		    xaRes.start(xid, XAResource.TMJOIN);
		}
		catch (XAException ex)
		{
		    ErrorStream.warning().println("JTA_Transaction.enlistResource - xa_start: "+JTA_Utility.printXAErrorCode(ex));
		    
		    markRollbackOnly();

		    throw ex;
		}
		
		/*
		 * Add to duplicate resources list so we can keep
		 * track of it (particularly if we later have to
		 * delist).
		 */
    
		_duplicateResources.put(xaRes, new TxInfo(xid));

		return true;
	    }

	    /*
	     * Control and Coordinator should be set, or we would not
	     * have gotten this far!
	     */
	    
	    JTA_ResourceRecord res = new JTA_ResourceRecord(xaRes, xid, params);
	    RecoveryCoordinator recCoord = _theControl.register_resource(res.getResource());

	    //	    if (recCoord == null)
	    //		throw new BAD_OPERATION();
	    
	    res.setRecoveryCoordinator(recCoord);
	    
	    res = null;
	    
	    return true;
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    /*
	     * Some exceptional condition arose and we probably could not
	     * enlist the resouce. So, for safety mark the transaction as
	     * rollback only.
	     */

	    markRollbackOnly();
	    
	    return false;
	}
    }

    /*
     * Do we have to unregister resources? Assume not as it would not
     * make much sense otherwise!
     */
    
public synchronized boolean delistResource (XAResource xaRes, int flags) throws IllegalStateException, javax.transaction.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_Transaction.delistResource ( "+xaRes+" )");
	}

	if (xaRes == null)
	    throw new javax.transaction.SystemException("JTA_Transaction.delistResource - resource is null!");
	
	int status = getStatus();

	switch (status)
	{
	case javax.transaction.Status.STATUS_ACTIVE:
	    break;
	case javax.transaction.Status.STATUS_MARKED_ROLLBACK:
	    break;
	default:
	    throw new IllegalStateException("Transaction is not active");
	}

	TxInfo info = null;

	try
	{
	    info = (TxInfo) _resources.get(xaRes);

	    if (info == null)
		info = (TxInfo) _duplicateResources.get(xaRes);
	    
	    if (info == null)
	    {
		ErrorStream.warning().println("JTA_Transaction.delistResource - unknown resource");
		
		return false;
	    }
	    else
	    {
		try
		{
		    /*
		     * If we know the transaction is going to rollback, then
		     * we can try to rollback the RM now. Just an optimisation.
		     */

		    if (status == javax.transaction.Status.STATUS_MARKED_ROLLBACK)
		    {
			xaRes.rollback(info.xid());
		    }
		}
		catch (Exception e)
		{
		    // failed, so try again when transaction does rollback
		}
		
		switch (info.getState())
		{
		case TxInfo.ASSOCIATED:
		    {
			if ((flags & XAResource.TMSUCCESS) != 0)
			{
			    xaRes.end(info.xid(), XAResource.TMSUCCESS);
			    info.setState(TxInfo.NOT_ASSOCIATED);
			}
			else
			{
			    if ((flags & XAResource.TMSUSPEND) != 0)
			    {
				xaRes.end(info.xid(), XAResource.TMSUSPEND);
				info.setState(TxInfo.ASSOCIATION_SUSPENDED);
			    }
			    else
			    {
				xaRes.end(info.xid(), XAResource.TMFAIL);
				info.setState(TxInfo.FAILED);
			    }
			}
		    }
		    break;
		case TxInfo.ASSOCIATION_SUSPENDED:
		    {
			if ((flags & XAResource.TMSUCCESS) != 0)
			{
			    xaRes.end(info.xid(), XAResource.TMSUCCESS);
			    info.setState(TxInfo.NOT_ASSOCIATED);
			}
			else
			{
			    if ((flags & XAResource.TMSUSPEND) != 0)
			    {
				// Note: this exception will be caught by our catch block

				throw new IllegalStateException("JTA_Transaction.delistResource - resource already suspended!");
			    }
			    else
			    {
				xaRes.end(info.xid(), XAResource.TMFAIL);
				info.setState(TxInfo.FAILED);
			    }
			}
		    }
		    break;
		default:
		    throw new IllegalStateException("JTA_Transaction.delistResource - illegal resource state: "+info.getState());
		}
		
		info = null;

		return true;
	    }
	}
	catch (IllegalStateException ex)
	{
	    throw ex;
	}
	catch (XAException exp)
	{
	    if (info != null)
		info.setState(TxInfo.FAILED);

	    /*
	     * For safety mark the transaction as rollback only.
	     */

	    markRollbackOnly();
	    
	    ErrorStream.warning().println("JTA_Transaction.delistResource - caught: "+JTA_Utility.printXAErrorCode(exp));
	    
	    return false;
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e);

	    /*
	     * Some exception occurred and we probably could not delist the
	     * resource. So, for safety mark the transaction as rollback only.
	     */

	    markRollbackOnly();
	    
	    return false;
	}
    }

public final Uid get_uid ()
    {
	return _theControl.get_uid();
    }
    
public String toString ()
    {
	if (_theControl == null)
	    return "NoTransaction";
	else
	{
	    try
	    {
		return _theControl.get_transaction_name();
	    }
	    catch (Exception e)
	    {
		return "UnknownTransaction";
	    }
	}
    }

protected JTA_Transaction (org.omg.CosTransactions.Control control, Uid u)
    {
	this(new OTS_ControlWrapper(control, u));
    }

protected JTA_Transaction (OTS_ControlWrapper control)
    {
	_theControl = control;

	if (control != null)
	{
	    _resources = new Hashtable();
	    _duplicateResources = new Hashtable();

	    _theControl.determineLocality();
	}
	else
	{
	    _resources = null;
	    _duplicateResources = null;
	}
    }

final int getXAResourceState (XAResource xaRes)
    {
	int state = TxInfo.UNKNOWN;

	if (xaRes != null)
	{
	    TxInfo info = (TxInfo) _resources.get(xaRes);
	    
	    if (info == null)
	    {
		info = (TxInfo) _duplicateResources.get(xaRes);
	    }
	    
	    if (info != null)
		state = info.getState();
	}

	return state;
    }
    
static final JTA_Transaction getTransaction (Control theControl)
    {
	try
	{
	    if (theControl != null)
	    {
		Uid u = OTS_Utility.getUid(theControl);
		
		if (u.equals(Uid.nullUid()))
		    return getOTSTransaction(theControl);
		else
		    return getArjunaTransaction(u, theControl);
	    }
	    else
		return null;
	}
	catch (Exception e)
	{
	    return null;
	}
    }

    /*
     * Add and remove transactions from list.
     */

static final void putTransaction (JTA_Transaction tx)
    {
	JTA_Transaction._transactions.put(tx.get_uid(), tx);
    }

static final void removeTransaction (JTA_Transaction tx)
    {
	JTA_Transaction._transactions.remove(tx.get_uid());
    }

    /**
     * This is an Arjuna transaction, so we can get its Uid and optimise
     * the lookup. This should be the most common scenario.
     */

private static final JTA_Transaction getArjunaTransaction (Uid theUid,
							   Control theControl)
    {
	JTA_Transaction tx = (JTA_Transaction) _transactions.get(theUid);
	
	if (tx == null)
	{
	    /*
	     * New transaction.
	     */

	    tx = new JTA_Transaction(theControl, theUid);

	    /*
	     * Now add transaction to list of global transactions.
	     * Will be removed when the transaction terminates.
	     */
	    
	    JTA_Transaction.putTransaction(tx);
	}
	
	return tx;
    }

    /**
     * This is a foreign transaction. This is likely to be an infrequent case,
     * so we can afford a slightly slower lookup here to improve performance
     * where we have Arjuna transactions.
     */

private static final JTA_Transaction getOTSTransaction (Control theControl)
    {
	try
	{
	    if (theControl != null)
	    {
		OTS_ControlWrapper wrapper = new OTS_ControlWrapper(theControl, new Uid());
		JTA_Transaction tx = null;
		Enumeration e = _transactions.elements();
		
		while (e.hasMoreElements())
		{
		    tx = (JTA_Transaction) e.nextElement();

		    if (tx.getControlWrapper().equals(wrapper))
			return tx;
		}
		
		/*
		 * New transaction.
		 */

		tx = new JTA_Transaction(wrapper);

		/*
		 * Now add transaction to list of global transactions.
		 * Will be removed when the transaction terminates.
		 */

		JTA_Transaction.putTransaction(tx);

		return tx;
	    }
	    else
		return null;
	}
	catch (Exception ex)
	{
	    return null;
	}
    }

    /*
     * If this thread has already registered a resource for the
     * same db then don't use this copy. For some databases it would
     * actually be ok for us to use the resource (at least to do an
     * xa_start equivalent on it), but for Oracle 8.1.6 it causes
     * their JDBC driver to crash!
     */

private final boolean threadIsActive (XAResource xaRes)
    {
	Thread t = Thread.currentThread();

	try
	{
	    Enumeration el = _resources.keys();
	    
	    if (el != null)
	    {
		while (el.hasMoreElements())
		{
		    XAResource x = (XAResource) el.nextElement();
		    
		    if (x.isSameRM(xaRes))
		    {
			TxInfo info = (TxInfo) _resources.get(x);
		    
			if (info.thread() == t)
			    return true;
		    }
		}
	    }

	    el = _duplicateResources.keys();

	    if (el != null)
	    {
		while (el.hasMoreElements())
		{
		    XAResource x = (XAResource) el.nextElement();
		    
		    if (x.isSameRM(xaRes))
		    {
			TxInfo info = (TxInfo) _resources.get(x);
		    
			if (info.thread() == t)
			    return true;
		    }
		}
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.FATAL).println(e);

	    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
	}

	return false;
    }

private final boolean isNewRM (XAResource xaRes)
    {
	try
	{
	    Enumeration el = _resources.keys();
	    
	    if (el != null)
	    {
		while (el.hasMoreElements())
		{
		    XAResource x = (XAResource) el.nextElement();
		    
		    if (x.isSameRM(xaRes))
		    {
			return false;
		    }
		}
	    }

	    el = _duplicateResources.keys();

	    if (el != null)
	    {
		while (el.hasMoreElements())
		{
		    if (((XAResource) el.nextElement()).isSameRM(xaRes))
		    {
			return false;
		    }
		}
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.FATAL).println(e);

	    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
	}

	return true;
    }

private final Xid createXid (boolean branch, JTA_XAModifier theModifier)
    {
	Xid xid = new JTA_XID(_theControl, branch);

	if (theModifier != null)
	{
	    try
	    {
		xid = theModifier.createXid((JTA_XID) xid);
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();
	    }
	}

	return xid;
    }

    /*
     * This method calls setRollbackOnly and catches any exceptions
     * it may throw and issues a warning. We use this in places wherew
     * we need to force the outcome of the transaction but already have
     * an exception to throw back to the application, so a failure here
     * will only be masked.
     */

private final void markRollbackOnly ()
    {
	try
	{
	    if (_theControl != null)
	    {
		try
		{
		    _theControl.rollback_only();
		}
		catch (org.omg.CosTransactions.Inactive e1)
		{
		    // ok
		}
		catch (org.omg.CosTransactions.Unavailable e2)
		{
		    // ok
		}
		catch (org.omg.CosTransactions.NoTransaction e3)
		{
		    // ok
		}
		catch (org.omg.CORBA.SystemException e3)
		{
		    throw new javax.transaction.SystemException();
		}
	    }
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	    
	    ErrorStream.warning().println("JTA_Transaction.markRollbackOnly - could not setRollbackOnly: "+ex);
	}
    }
	
private OTS_ControlWrapper _theControl;
private Hashtable          _resources;
private Hashtable          _duplicateResources;
    
private static Hashtable   _transactions = new Hashtable();
 
}
