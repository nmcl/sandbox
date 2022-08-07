/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTA_ResourceRecord.javatmpl,v 1.4.2.1.4.1.2.1.2.4.4.6.6.1.2.4.2.2.2.2.4.3 2001/01/26 19:15:07 pfurniss Exp $
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
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCore.Interface.*;
import com.arjuna.CosTransactions.tx.*;
import com.arjuna.CosTransactions.OTS_Utility;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import org.omg.CosTransactions.*;
import javax.transaction.*;
import javax.transaction.xa.*;
import java.io.*;

import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.HeuristicRollback;
import java.io.IOException;
import org.omg.CORBA.OBJECT_NOT_EXIST;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.SystemException;
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
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JTA_ResourceRecord.javatmpl,v 1.4.2.1.4.1.2.1.2.4.4.6.6.1.2.4.2.2.2.2.4.3 2001/01/26 19:15:07 pfurniss Exp $
 * @since JTS 1.2.4.
 */

public class JTA_ResourceRecord extends  org.omg.CosTransactions . ResourcePOA 
{

public static final int XACONNECTION = 0;
    
/**
 * The params represent specific parameters we need to recreate the
 * connection to the database in the event of a failure. If they're
 * not set then recovery is out of our control.
 *
 * Could also use it to pass other information, such as the readonly
 * flag.
 */

public JTA_ResourceRecord (XAResource res, Xid xid, Object[] params)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_ResourceRecord.JTA_ResourceRecord ( "+xid+" )");
	}

	_theXAResource = res;
	_recoveryObject = null;
	_tranID = xid;	

	_valid = true;

	if (params != null)
	{
	    if (params.length >= XACONNECTION)
	    {
		if (params[XACONNECTION] instanceof JTA_XAConnectionRecovery)
		    _recoveryObject = (JTA_XAConnectionRecovery) params[XACONNECTION];
	    }
	}

	_prepared = false;
	_heuristic = JTA_ResourceRecord.NONE;
	_objStore = null;
	_theUid = new Uid();
	_theReference = null;
	_recoveryCoordinator = null;

	TransactionManager tm = JTA_TransactionManager.transactionManager();

	try
	{
	    _theTransaction = (JTA_Transaction) tm.getTransaction();
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e.toString());

	    _theTransaction = null;
	}
    }

public void finalize ()
    {
	if (_theTransaction != null)
	{
	    JTA_Transaction.removeTransaction(_theTransaction);	
	    _theTransaction = null;
	}
    }

public final Uid get_uid ()
    {
	return _theUid;
    }
 
public final synchronized org.omg.CosTransactions.Resource getResource ()
    {
	if (_theReference == null)
	{
	    OAInterface.objectIsReady( this ) ;

	    _theReference =  org.omg.CosTransactions.ResourceHelper.narrow(OAInterface.corbaReference( this )) ;
	}

	return _theReference;
    }

public final Xid getXid ()
    {
	return _tranID;
    }
 


public org.omg.CosTransactions.Vote prepare () throws HeuristicMixed, HeuristicHazard, org.omg.CORBA.SystemException


    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_ResourceRecord.prepare for "+_tranID);
	}

	if (!_valid || (_theXAResource == null))
	{
	    removeConnection();
	    
	    return Vote.VoteReadOnly;
	}

	if (_tranID == null)
	{
	    ErrorStream.warning().println("JTA_ResourceRecord.prepare - null transaction!");
	    
	    removeConnection();
	    
	    return Vote.VoteRollback;
	}

	try
	{
	    if (createState())
	    {
		if (endAssociation())
		    _theXAResource.end(_tranID, XAResource.TMSUCCESS);
		
		if (_theXAResource.prepare(_tranID) == XAResource.XA_RDONLY)
		{
		    destroyState();
		
		    return Vote.VoteReadOnly;
		}
		else
		    return Vote.VoteCommit;
	    }
	    else
		return Vote.VoteRollback;
	}
	catch (XAException e1)
	{
	    e1.printStackTrace();
	    
	    /*
	     * XA_RB*, XAER_RMERR, XAER_RMFAIL,
	     * XAER_NOTA, XAER_INVAL, or XAER_PROTO.
	     */

	    removeConnection();
	    
	    return Vote.VoteRollback;
	}
	catch (Exception e2)
	{
	    /*
	     * XA_RB*, XAER_RMERR, XAER_RMFAIL,
	     * XAER_NOTA, XAER_INVAL, or XAER_PROTO.
	     */

	    removeConnection();
	    
	    return Vote.VoteRollback;
	}
    }

public void rollback () throws org.omg.CORBA.SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_ResourceRecord.rollback for "+_tranID);
	}

	if (!_valid)
	    return;
	
	if (_tranID == null)
	{
	    ErrorStream.warning().println("JTA_ResourceRecord.rollback - null transaction!");
	}
	else
	{
	    if (_theXAResource != null)
	    {
		switch (_heuristic)
		{
		case JTA_ResourceRecord.HAZARD:
		    throw new org.omg.CosTransactions.HeuristicHazard();
		case JTA_ResourceRecord.MIXED:
		    throw new org.omg.CosTransactions.HeuristicMixed();
		default:
		    break;
		}
		
		try
		{
		    if (!_prepared)
		    {
			if (endAssociation())
			    _theXAResource.end(_tranID, XAResource.TMSUCCESS);
		    }
		    
		    _theXAResource.rollback(_tranID);

		    if (_prepared)
			destroyState();  // race condition
		    else
			removeConnection();
		}
		catch (XAException e1)
		{
		    if (notAProblem(e1,false)) { 
			// some other thread got there first (probably)
			if (_prepared)
			    destroyState();  // race condition
			else
			    removeConnection();
		    }
		    else
		    {
			ErrorStream.warning().println("JTA_ResourceRecord.rollback - xa error " +
				 JTA_Utility.printXAErrorCode(e1));
			 e1.printStackTrace(ErrorStream.stream());
			switch (e1.errorCode)
			{
			case XAException.XAER_RMERR:
			case XAException.XA_HEURHAZ:
			    updateState(JTA_ResourceRecord.HAZARD);
			    
			    throw new org.omg.CosTransactions.HeuristicHazard();
			case XAException.XA_HEURCOM:
			    updateState(JTA_ResourceRecord.COMMIT);
			    
			    throw new org.omg.CosTransactions.HeuristicCommit();
			case XAException.XA_HEURMIX:
			    updateState(JTA_ResourceRecord.MIXED);
			    
			    throw new org.omg.CosTransactions.HeuristicMixed();
			case XAException.XA_HEURRB:
			case XAException.XA_RBROLLBACK:
			    destroyState();
			    break;
			default:
			    destroyState();
			    
			    throw new UNKNOWN();
			}
		    }
		}
		catch (Exception e2)
		{
		    e2.printStackTrace();
		    
		    removeConnection();

		    throw new UNKNOWN();
		}
	    }
	}
    }

public void commit () throws org.omg.CORBA.SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_ResourceRecord.commit for "+_tranID);
	}

	if (!_prepared)
	    throw new NotPrepared();
	
	if (_tranID == null)
	{
	    ErrorStream.warning().println("JTA_ResourceRecord.commit - null transaction!");
	}
	else
	{
	    if (_theXAResource != null)
	    {
		switch (_heuristic)
		{
		case JTA_ResourceRecord.HAZARD:
		    throw new org.omg.CosTransactions.HeuristicHazard();
		case JTA_ResourceRecord.MIXED:
		    throw new org.omg.CosTransactions.HeuristicMixed();
		case JTA_ResourceRecord.ROLLBACK:
		    throw new org.omg.CosTransactions.HeuristicRollback();
		default:
		    break;
		}
		
		try
		{
		    _theXAResource.commit(_tranID, false);

		    destroyState();
		}
		catch (XAException e1)
		{
		    if (notAProblem(e1,true)) { 
			// some other thread got there first (probably)
			destroyState();
		    }
		    else 
		    {
			ErrorStream.warning().println("JTA_ResourceRecord.commit - xa error " +
				 JTA_Utility.printXAErrorCode(e1));
			 e1.printStackTrace(ErrorStream.stream());
			
		    
			/*
			 * XA_HEURHAZ, XA_HEURCOM, XA_HEURRB,
			 * XA_HEURMIX, XAER_RMERR, XAER_RMFAIL,
			 * XAER_NOTA, XAER_INVAL, or XAER_PROTO.
			 */
    
			switch (e1.errorCode)
			{
			case XAException.XAER_RMERR:
			case XAException.XA_HEURHAZ:
			    updateState(JTA_ResourceRecord.HAZARD);
			    
			    throw new org.omg.CosTransactions.HeuristicHazard();
			case XAException.XA_HEURCOM:
			    destroyState();
			    break;
			case XAException.XA_HEURRB:
			    updateState(JTA_ResourceRecord.ROLLBACK);
			    
			    throw new org.omg.CosTransactions.HeuristicRollback();
			case XAException.XA_HEURMIX:
			    updateState(JTA_ResourceRecord.MIXED);
			    
			    throw new org.omg.CosTransactions.HeuristicMixed();
			case XAException.XAER_NOTA:
			case XAException.XAER_INVAL:
			case XAException.XAER_PROTO:
			case XAException.XAER_RMFAIL:  // resource manager failed, did it rollback?
			    removeConnection();
    
			    throw new UNKNOWN();
			default:
			    destroyState();
    
			    throw new UNKNOWN();
			}
		    }
		}
		catch (Exception e2)
		{
		    removeConnection();

		    throw new UNKNOWN();
		}
	    }
	}
    }


/**
 *  Is the XAException a non-error when received in reply to commit or rollback ?
 *   it normally is, but may be overridden in recovery
 */
protected boolean notAProblem(XAException ex, boolean commit)
{
    return false;
}

/**
 * For commit_one_phase we can do whatever we want since the
 * transaction outcome is whatever we want. Therefore, we do
 * not need to save any additional recoverable state, such as
 * a reference to the transaction coordinator, since it will not
 * have an intentions list anyway.
 */

public void commit_one_phase () throws HeuristicHazard, org.omg.CORBA.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_ResourceRecord.commit_one_phase for "+_tranID);
	}

	if (_tranID == null)
	{
	    ErrorStream.warning().println("JTA_ResourceRecord.commit - null transaction!");
	}
	else
	{
	    if (_theXAResource != null)
	    {
		switch (_heuristic)
		{
		case JTA_ResourceRecord.HAZARD:
		    throw new org.omg.CosTransactions.HeuristicHazard();
		default:
		    break;
		}
		
		try
		{
		    if (endAssociation())
			_theXAResource.end(_tranID, XAResource.TMSUCCESS);

		    try
		    {
			_theXAResource.commit(_tranID, true);

			removeConnection();
		    }
		    catch (XAException xaex)
		    {
			ErrorStream.warning().println("JTA_ResourceRecord.commit_one_phase caught: "+xaex);

			_theXAResource.rollback(_tranID);

			throw xaex;
		    }
		}
		catch (XAException e1)
		{
		    e1.printStackTrace();
		    
		    /*
		     * XA_HEURHAZ, XA_HEURCOM, XA_HEURRB,
		     * XA_HEURMIX, XAER_RMERR, XAER_RMFAIL,
		     * XAER_NOTA, XAER_INVAL, or XAER_PROTO.
		     * XA_RB*
		     */

		    if ((e1.errorCode > XAException.XA_RBBASE) &&
			(e1.errorCode < XAException.XA_RBEND))
		    {
			removeConnection();
			
			throw new TRANSACTION_ROLLEDBACK ();
		    }
		    
		    switch (e1.errorCode)
		    {
		    case XAException.XAER_RMERR:
		    case XAException.XA_HEURHAZ:
		    case XAException.XA_HEURMIX:
			updateState(JTA_ResourceRecord.HAZARD);
			
			throw new org.omg.CosTransactions.HeuristicHazard();
		    case XAException.XA_HEURCOM:
			break;
		    case XAException.XA_HEURRB:
			throw new TRANSACTION_ROLLEDBACK ();
		    case XAException.XAER_NOTA:
		    case XAException.XAER_INVAL:
		    case XAException.XAER_PROTO:
		    case XAException.XAER_RMFAIL:  // resource manager failed, did it rollback?
			removeConnection();

			throw new UNKNOWN();
		    default:
			removeConnection();

			throw new UNKNOWN();
		    }
		}
		catch (Exception e2)
		{
		    removeConnection();

		    throw new UNKNOWN();
		}
	    }
	}
    }
 
public void forget () throws org.omg.CORBA.SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_ResourceRecord.forget for "+_tranID);
	}

	if ((_theXAResource != null) && (_tranID != null))
	{
	    try
	    {
		_theXAResource.forget(_tranID);
	    }
	    catch (Exception e)
	    {
	    }
	}
		
	destroyState();

	removeConnection();
    }

public boolean saveState (OutputObjectState os)
    {
	boolean res = false;

	try
	{
	    os.packInt(_heuristic);

	    /*
	     * Since we don't know what type of Xid we are
	     * using, leave it up to JTA_XID to pack.
	     */

	    JTA_XID.pack(os, _tranID);

	    /*
	     * If no recovery object set then rely upon object
	     * serialisation!
	     */

	    if (_recoveryObject == null)
	    {
		os.packInt(JTA_XAConnectionRecovery.OBJECT_RECOVERY);

		try
		{
		    ByteArrayOutputStream s = new ByteArrayOutputStream();
		    ObjectOutputStream o = new ObjectOutputStream(s);

		    o.writeObject(_theXAResource);
		    o.close();

		    os.packBoolean(true);
		    
		    os.packBytes(s.toByteArray());
		}
		catch (NotSerializableException ex)
		{
		    // have to rely upon XAResource.recover!

		    os.packBoolean(false);
		}
	    }
	    else
	    {
		os.packInt(JTA_XAConnectionRecovery.AUTO_RECOVERY);
		os.packString(_recoveryObject.getClass().getName());
		
		_recoveryObject.packInto(os);
	    }

	    if (_recoveryCoordinator == null)
		os.packBoolean(false);
	    else
	    {
		os.packBoolean(true);
		
		String ior = ORBInterface.orb().object_to_string(_recoveryCoordinator);
	    
		os.packString(ior);
	    
		ior = null;
	    }

	    res = true;
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    res = false;
	}

	return res;
    }

public boolean restoreState (InputObjectState os)
    {
	boolean res = false;

	try
	{
	    _heuristic = os.unpackInt();
	    _tranID = JTA_XID.unpack(os);

	    _theXAResource = null;
	    _recoveryObject = null;
	    
	    if (os.unpackInt() == JTA_XAConnectionRecovery.OBJECT_RECOVERY)
	    {
		boolean haveXAResource = os.unpackBoolean();
		
		if (haveXAResource)
		{
		    try
		    {
			byte[] b = os.unpackBytes();
	    
			ByteArrayInputStream s = new ByteArrayInputStream(b);
			ObjectInputStream o = new ObjectInputStream(s);
	    
			_theXAResource = (XAResource) o.readObject();
			o.close();

			if (DebugController.enabled())
			{
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								 FacilityCode.FAC_JTA, "JTA_ResourceRecord.restore_state - XAResource de-serialized");
			}
		    }
		    catch (Exception ex)
		    {
			// not serializable in the first place!

			ErrorStream.warning().println("Exception on attempting to restore XAResource " + ex);
			ex.printStackTrace(ErrorStream.stream());
		    }
		}
	    }
	    else
	    {
		String creatorName = os.unpackString();
		Class c = Class.forName(creatorName);

		_recoveryObject = (JTA_XAConnectionRecovery) c.newInstance();
		
		_recoveryObject.unpackFrom(os);
		_theXAResource = _recoveryObject.getResource();

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS,
							 VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_JTA, "JTA_ResourceRecord.restore_state - XAResource got from " + creatorName);
		}
	    }

	    boolean haveRecCoord = os.unpackBoolean();

	    if (haveRecCoord)
	    {
		String ior = os.unpackString();
	    
		if (ior == null)
		    return false;
		else
		{
		    org.omg.CORBA.Object objRef = ORBInterface.orb().string_to_object(ior);
		
		    _recoveryCoordinator = RecoveryCoordinatorHelper.narrow(objRef);
		}
	    }
	    else
		_recoveryCoordinator = null;

	    res = true;
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    res = false;
	}

	return res;
    }

public String type ()
    {
	return JTA_ResourceRecord.typeName();
    }

public static String typeName ()
    {
	return "/StateManager/JTA_ResourceRecord";
    }

final void setRecoveryCoordinator (RecoveryCoordinator recCoord)
    {
	_recoveryCoordinator = recCoord;
    }

final RecoveryCoordinator getRecoveryCoordinator ()
    {
	return _recoveryCoordinator;
    }
 
protected JTA_ResourceRecord (Uid u)
    {
	_theXAResource = null;
	_recoveryObject = null;
	_tranID = null;
	_prepared = true;
	_heuristic = JTA_ResourceRecord.NONE;
	_theUid = u;
	_objStore = null;
	_valid = false;
	_theReference = null;
	_recoveryCoordinator = null;
	_theTransaction = null;
	
	_valid = loadState();
    } 

/**
 * For those objects where the original XAResource could not be
 * saved.
 */

protected synchronized void setXAResource (XAResource res)
    {
	_theXAResource = res;
    }
 
protected boolean recover ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JTA, "JTA_ResourceRecord.recover");
	}

	if (_valid)
	{
	    org.omg.CosTransactions.Status s = org.omg.CosTransactions.Status.StatusUnknown;
	
	    try
	    {
		s = _recoveryCoordinator.replay_completion(getResource());
	    }
	    catch (OBJECT_NOT_EXIST ex)
	    {
		// no coordinator, so presumed abort.

		s = org.omg.CosTransactions.Status.StatusRolledBack;
	    }

	    catch (NotPrepared ex1)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("JTA_ResourceRecord: caught NotPrepared");

		return false;
	    }
	    catch (java.lang.NullPointerException ne)
	    {
		/*
		 * No recovery coordinator!
		 */
	    }
	    catch (Exception e)
	    {
		/*
		 * Unknown error, so better to do nothing at this
		 * stage.
		 */

		ErrorStream.warning().println("JTA_ResourceRecord: caught unexpected exception: "+e);
		e.printStackTrace(ErrorStream.stream());

		return false;
	    }

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_JTA, "JTA_ResourceRecord.recover got status: "+OTS_Utility.printStatus(s));
	    }

	    boolean doCommit = false;
	
	    switch ( s.value() )
	    {
	    case  org.omg.CosTransactions.Status ._StatusUnknown  :
		// some problem occurred

		return false;
	    case  org.omg.CosTransactions.Status ._StatusMarkedRollback  :
	    case  org.omg.CosTransactions.Status ._StatusRollingBack  :
		// we should be told eventually, so wait
		
		return false;
	    case  org.omg.CosTransactions.Status ._StatusCommitted  :
		doCommit = true;
		break;
	    case  org.omg.CosTransactions.Status ._StatusRolledBack  :
	    case  org.omg.CosTransactions.Status ._StatusNoTransaction  :  // presumed abort
		doCommit = false;
		break;
	    default:
		// wait

		return false;
	    }

	    return doRecovery(doCommit);
	}
	
	return false;
    }

private final void setObjectStore ()
    {
	if (_objStore == null)
	    _objStore = new ObjectStore(null, "");  // interface gets default
    }
 
private final boolean createState ()
    {
	setObjectStore();
	
	if ((_theXAResource != null) && (_tranID != null) && (_objStore != null))
	{
	    OutputObjectState os = new OutputObjectState();
	    
	    if (saveState(os))
	    {
		try
		{
		    _valid = _objStore.write_committed(_theUid, type(), os);
		    _prepared = true;
		}
		catch (Exception e)
		{
		    ErrorStream.warning().println("JTA_ResourceRecord.createState: "+e);
		    
		    _valid = false;
		}
	    }
	    else
		_valid = false;
	}
	else
	    _valid = false;

	return _valid;
    }
 
private final boolean updateState (int h)
    {
	setObjectStore();

	if (_prepared)  // only need do if we have prepared
	{
	    OutputObjectState os = new OutputObjectState();

	    _heuristic = h;
	    
	    if (saveState(os))
	    {
		try
		{
		    _valid = _objStore.write_committed(_theUid, type(), os);
		}
		catch (Exception e)
		{
		    _valid = false;
		}
	    }
	    else
	    {
		ErrorStream.warning().println("JTA_ResourceRecord - could not update state.");

		_valid = false;
	    }
	}

	return _valid;
    }

private final boolean loadState ()
    {
	setObjectStore();

	InputObjectState os = null;
	
	try
	{
	    os = _objStore.read_committed(_theUid, type());
	}
	catch (Exception e)
	{
	    os = null;
	}
	
	if (os != null)
	{
	    _valid = restoreState(os);
	    
	    os = null;
	}
	else
	    _valid = false;

	return _valid;
    }
 
private final synchronized boolean destroyState ()
    {
	setObjectStore();

	if (_prepared && _valid)
	{
	    try
	    {
		_objStore.remove_committed(_theUid, type());
		_prepared = false;
	    }
	    catch (Exception e)
	    {
		_valid = false;
	    }
	}
	
	if (_recoveryObject != null)
	    removeConnection();

	return _valid;
    }

private final void removeConnection ()
    {
	/*
	 * Should only be called once. Remove the connection so that
	 * user can reuse the driver as though it were fresh (e.g., can
	 * do read only optimisation).
	 */

	if ((_recoveryObject != null) && (_theTransaction != null))
	{
	    try
	    {
		_recoveryObject.removeConnection(_theTransaction);
		_recoveryObject = null;

		JTA_Transaction.removeTransaction(_theTransaction);	
		_theTransaction = null;
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println(e.toString());
	    }
	}

	try
	{
	    if (_theReference != null)
	    {
		OAInterface.shutdownObject( this ) ;
		_theReference = null;
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e.toString());
	}
    }

private final boolean doRecovery (boolean commit)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_JTA, "JTA_ResourceRecord.doRecovery ( "+commit+" )");
	}

	boolean result = false;
	
	if ((_theXAResource != null) && (_tranID != null))
	{
	    try
	    {
		if (commit)
		    commit();
		else
		    rollback();

		// if those succeed, they will have removed any persistent state
		
		result = true;

	    }
	    catch (Exception e2)
	    {
		ErrorStream.warning().println("JTA_ResourceRecord.doRecovery: "+e2);
		e2.printStackTrace(ErrorStream.stream());
	    }
	}

	return result;
    }

/*
 * Ask the transaction whether or not this XAResource is still
 * associated with the thread, i.e., has end already been called
 * on it?
 */

private final boolean endAssociation ()
    {
	boolean doEnd = true;
	
	if (_theTransaction != null)
	{
	    if (_theTransaction.getXAResourceState(_theXAResource) != TxInfo.ASSOCIATED)
	    {
		doEnd = false;
	    }
	}
	
	return doEnd;
    }
 
protected XAResource _theXAResource;
 
private static final int NONE = 0;
private static final int ROLLBACK = 1;
private static final int HAZARD = 2;
private static final int MIXED = 3;
private static final int COMMIT = 4;  
 
private JTA_XAConnectionRecovery _recoveryObject;
private Xid		      _tranID;
private boolean		  _prepared;
private boolean		  _valid;
private int		      _heuristic;
private ObjectStore	      _objStore;
private Uid		      _theUid;
private org.omg.CosTransactions.Resource	    _theReference;
private org.omg.CosTransactions.RecoveryCoordinator _recoveryCoordinator;
private JTA_Transaction		 _theTransaction;

};
