/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBCResource.javatmpl,v 1.9.4.2.4.1.6.2.6.1.14.1.2.2.4.1 2000/12/21 11:21:33 nmcl Exp $
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
































































package com.arjuna.JDBC;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import com.arjuna.CosTransactions.ArjunaOTS.TopLevelTransaction;
import com.arjuna.ArjunaCommon.Common.*;
import org.omg.CosTransactions.*;
import java.sql.*;
import java.io.*;
import java.util.Properties;

import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NoTransaction;
import org.omg.CORBA  .SystemException;
import java.sql.SQLException;

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





/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;





/**
 * JDBC does not use a two-phase commit protocol, and does not allow
 * nested transactions. So, we do not require an implementation of
 * SubtransactionAwareResource, and we could potentially have to raise
 * heuristics.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JDBCResource.javatmpl,v 1.9.4.2.4.1.6.2.6.1.14.1.2.2.4.1 2000/12/21 11:21:33 nmcl Exp $
 * @since JTS 1.2.4.
 */

/*
 * We need to know the various XOpen JDBC error codes which are inserted
 * within a SQLException. Then we can determine the reason for the
 * exception, and base better actions on that.
 */

/*
 * dataDefinitionCausesTransactionCommit and friends. (meta data class).
 */

public class JDBCResource extends StateManager implements  org.omg.CosTransactions .ResourceOperations 
{

public JDBCResource (JDBCConnection connection, boolean readOnly,
		     String url, Properties info)
    {
	this(connection, readOnly, url, info, ObjectType.RECOVERABLE);
    }
 
public JDBCResource (JDBCConnection connection, boolean readOnly,
		     String url, Properties info, int ot)
    {
	super(ot);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "JDBCResource.JDBCResource ( "+connection+", "+readOnly+", "+url+" )");
	}

	_theTransaction = null;
	_otsConn = connection;
	_prepared = false;
	_heuristic = JDBCResource.NONE;
	_url = url;
	_info = info;
	_theConnection = connection.getConnection();
	_readOnly = readOnly;
	_theReference = null;
	_theImpl = null;
	
	try
	{
	    createState();
	}
	catch (JDBCException e1)
	{
	    _otsConn.removeResource();
	}
    }

/**
 * Crash recovery constructor.
 */

public JDBCResource (Uid id) throws SQLException, JDBCException
    {
	super(id);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "JDBCResource.JDBCResource ( "+id+" )");
	}

	_theTransaction = null;
	_otsConn = null;
	_prepared = true;     // must be true for recovery to be here
	_heuristic = JDBCResource.NONE;
	_theConnection = null;
	_theReference = null;
	_theImpl = null;
	
	if (!super.activate())
	    throw new JDBCException("JDBCResource - could not load recovery state.");
	else
	{
	    _otsConn = new JDBCConnection(_url, _info);
	    _theConnection = _otsConn.getConnection();
	}
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "JDBCResource.finalize ()");
	}

	terminate();
	
	if (_theConnection != null)
	{
	    /*
	     * If the transaction is set, then we have potentially been
	     * used.
	     */
	    
	    if (_theTransaction != null)
	    {
		/*
		 * Should have been committed/rolledback by now, so the
		 * reference should be null!
		 */

		ErrorStream.stream(ErrorStream.WARNING).println("JDBCResource - non-null connection attribute at destruction!");

		org.omg.CosTransactions.Status status;
	    
		try
		{
		    status = _theTransaction.get_status();
		}
		catch (Exception e)
		{
		    status = org.omg.CosTransactions.Status.StatusUnknown;
		}

		/*
		 * Is transaction still running?
		 */
		
		if ((status == org.omg.CosTransactions.Status.StatusActive) ||
		    (status == org.omg.CosTransactions.Status.StatusPreparing))
		{
		    /*
		     * Rollback transaction.
		     */

		    try
		    {
			_theTransaction.rollback_only();
		    }
		    catch (Exception e)
		    {
			ErrorStream.stream(ErrorStream.WARNING).println("JDBCResource attempt to rollback transaction failed: "+e);
		    }
		}
		else
		{
		    /*
		     * Something strange happened (is happening).
		     *
		     * We are going out of scope either:
		     *
		     * (i)  after the transaction terminated, in which case
		     *      why weren't	we called during commit/rollback?
		     * (ii) while the transaction is terminating, in which
		     *      case why doesn't it have a reference to us to keep
		     *      us alive?
		     *
		     * Either way, there's not a lot we can do about it!
		     */
		}

		_theTransaction = null;
	    }

	    _otsConn.removeResource();
	}
    }

/**
 * Since JDBC 1.0 does not support 2-phase commit, we always
 * return VoteCommit for prepare, and hope we can when commit
 * called!
 */


public org.omg.CosTransactions.Vote prepare () throws SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_JDBC, "JDBCResource.prepare");
	}

	_prepared = true;

	if ((_theConnection == null) || _readOnly)
	{
	    destroyState();

	    return Vote.VoteReadOnly;
	}
	else
	{
	    if (_theTransaction == null)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("JDBCResource.prepare: no transaction specified.");

		destroyState();
		
		return Vote.VoteRollback;
	    }
	    else
		return Vote.VoteCommit;
	}
    }

/**
 * Must remove prepare decision, if any.
 *
 * We record the heuristic decision all the time, although technically
 * we need only do this if rollback is called after prepare has been
 * invoked.
 */

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_JDBC, "JDBCResource.rollback");
	}
	
	if (!_readOnly)
	{
	    switch (_heuristic)
	    {
	    case JDBCResource.HAZARD:
		throw new HeuristicHazard();
	    default:
		break;
	    }
	    
	    if (_theConnection != null)
	    {
		if (_theTransaction == null)
		    ErrorStream.stream(ErrorStream.WARNING).println("JDBCResource.rollback: no transaction specified.");

		try
		{
		    _theConnection.rollback();

		    _otsConn.removeResource();
		    
		    destroyState();
		}
		catch (SQLException e)
		{
		    /*
		     * Can't say what happened.
		     */

		    _otsConn.removeResource();
		    
		    updateState(JDBCResource.HAZARD);
		    
		    throw new HeuristicHazard();
		}
	    }
	}
    }

/**
 * Must remove prepare decision, if any, and record any
 * heuristic decision we may make.
 */

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_JDBC, "JDBCResource.commit");
	}

	if (!_prepared)
	    throw new NotPrepared();

	if (!_readOnly)
	{
	    switch (_heuristic)
	    {
	    case JDBCResource.ROLLBACK:
		throw new HeuristicRollback();
	    case JDBCResource.HAZARD:
		throw new HeuristicHazard();
	    case JDBCResource.NONE:
		break;
	    }
	    
	    if (_theConnection != null)
	    {
		if (_theTransaction == null)
		    ErrorStream.stream(ErrorStream.WARNING).println("JDBCResource.commit: no transaction specified.");

		try
		{
		    _theConnection.commit();

		    _otsConn.removeResource();
		    
		    destroyState();
		}
		catch (SQLException e1)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println(e1);

		    /*
		     * Try to rollback then.
		     */

		    try
		    {
			_theConnection.rollback();

			_otsConn.removeResource();
			
			updateState(JDBCResource.ROLLBACK);
			
			throw new HeuristicRollback();
		    }
		    catch (SQLException e2)
		    {
			updateState(JDBCResource.HAZARD);
		    }

		    /*
		     * Can't say what happened.
		     */
		    
		    throw new HeuristicHazard();
		}
	    }
	}
    }

/**
 * We need to remember what type of heuristic (if any) we
 * threw.
 */

public void forget () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_JDBC, "JDBCResource.forget");
	}

	super.destroy();
    }

public void commit_one_phase () throws HeuristicHazard, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_JDBC, "JDBCResource.commit_one_phase");
	}

	if (!_readOnly)
	{
	    switch (_heuristic)
	    {
	    case JDBCResource.HAZARD:
		throw new HeuristicHazard();
	    default:
		break;
	    }

	    if (_theConnection != null)
	    {
		if (_theTransaction == null)
		    ErrorStream.stream(ErrorStream.WARNING).println("JDBCResource.commit_one_phase: no transaction specified.");

		try
		{
		    _theConnection.commit();

		    _otsConn.removeResource();
		    
		    destroyState();
		}
		catch (SQLException e)
		{
		    updateState(JDBCResource.HAZARD);
		    
		    throw new HeuristicHazard();
		}
	    }
	}
    }

public void setTransaction (Coordinator tran) throws INVALID_TRANSACTION 
    {
	/*
	 * Only allow to be set once.
	 */
	
	if (_theTransaction == null)
	    _theTransaction = tran;
	else
	    throw new INVALID_TRANSACTION ();
    }

public Coordinator getTransaction () throws Unavailable
    {
	if (_theTransaction == null)
	    throw new Unavailable();
	else
	    return _theTransaction;
    }

public boolean save_state (OutputObjectState os, int ot)
    {
	boolean res = false;

	try
	{
	    os.packInt(_heuristic);
	    os.packBoolean(_readOnly);
	    
	    os.packString(_url);

	    ByteArrayOutputStream stream = new ByteArrayOutputStream();

	    _info.store(stream, get_uid().toString());

	    os.packBytes(stream.toByteArray());
	    
	    res = true;
	}
	catch (Exception e)
	{
	    res = false;
	}
	
	return res;
    }

public boolean restore_state (InputObjectState os, int ot)
    {
	boolean res = false;

	try
	{
	    _heuristic = os.unpackInt();
	    _readOnly = os.unpackBoolean();
	    
	    _url = os.unpackString();

	    ByteArrayInputStream stream = new ByteArrayInputStream(os.unpackBytes());

	    _info = new Properties();
	    _info.load(stream);

	    res = true;
	}
	catch (Exception e)
	{
	    res = false;
	}

	return res;
    }

public String type ()
    {
	return JDBCResource.typeName();
    }

public static String typeName ()
    {
	return new String("/StateManager/JDBCResource");
    }

protected boolean recover ()
    {
	// nothing we can really do with JDBC 1.0!

	if (super.activate())
	{
	    if (_heuristic != JDBCResource.NONE)
		ErrorStream.warning().println("JDBCResource "+get_uid()+" threw Heuristic"+((_heuristic == JDBCResource.ROLLBACK) ? "Rollback" : "Hazard"));
	    else
		super.destroy();
	    
	    return true;
	}
	else
	    return false;
    }

synchronized org.omg.CosTransactions.Resource getReference ()
    {
	if (_theReference == null)
	{
	    _theImpl = new  org.omg.CosTransactions. ResourcePOATie (this);

	    OAInterface.objectIsReady( _theImpl ) ;

	    _theReference =   org.omg.CosTransactions.ResourceHelper.narrow(OAInterface.corbaReference( _theImpl )) ;
	}
	
	return _theReference;
    }
 
private void createState () throws JDBCException
    {
	super.activate();
	
	super.modified();

	if (!super.deactivate())
	    throw new JDBCException("JDBCResource - could not create state.");
    }

private void updateState (int h)
    {
	super.activate();
	
	_heuristic = h;

	super.modified();

	if (!super.deactivate())
	    ErrorStream.stream(ErrorStream.WARNING).println("JDBCResource - could not update state.");
    }

private void destroyState ()
    {
	// first get rid of the CORBA reference if we have one

	if (_theReference != null)
	{
	    OAInterface.shutdownObject( _theImpl ) ;
	    
	    _theImpl = null;
	    _theReference = null;
	}
	
	super.destroy();
    }

private static final int NONE = 0;
private static final int ROLLBACK = 1;
private static final int HAZARD = 2;

private Coordinator    _theTransaction;
private JDBCConnection _otsConn;
private boolean        _prepared;
private int            _heuristic;
private String         _url;
private Properties     _info;
private Connection     _theConnection;
private boolean        _readOnly;
private Resource       _theReference;
private  org.omg.CosTransactions. ResourcePOATie  _theImpl;

}
