/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Tyne and Wear,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBCConnection.javatmpl,v 1.1.2.1.4.1.6.2.6.3.6.1.2.2.2.1.2.1 2000/12/21 11:21:33 nmcl Exp $
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
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.ArjunaOTS.AtomicTransaction;
import com.arjuna.CosTransactions.ArjunaOTS.TopLevelTransaction;
import org.omg.CosTransactions.*;
import java.util.*;
import java.sql.*;

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





/*
 * JDBC 1.0 has no notion of assigning a txid to a given operation,
 * so all accesses through the same connection are implicitly associated
 * with the same transaction in the db.
 */

/**
 * A JDBC 1.0 connection. This wraps all accesses to the real JDBC connection
 * and ensures that transaction enlistment/delistment occurs at the right
 * time. Basically makes the real connection transactional.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JDBCConnection.javatmpl,v 1.1.2.1.4.1.6.2.6.3.6.1.2.2.2.1.2.1 2000/12/21 11:21:33 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class JDBCConnection implements java.sql.Connection
{

    /**
     * Create a new connection using the specified url and properties.
     * Delegate all connection invocations to the provided connection
     * implementation.
     */

public JDBCConnection (String url, Properties info, java.sql.Connection conn) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "JDBCConnection.JDBCConnection ( "+url+" )");
	}

	_autoCommit = true;
	_readOnly = false;
	_oldIsolationLevel = -1;
	_theResource = null;
	_theConnection = conn;
	_url = url;
	_info = info;
    }

    /**
     * Create a new connection using the specified url and properties.
     */

public JDBCConnection (String url, Properties info) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "JDBCConnection.JDBCConnection ( "+url+" )");
	}

	_autoCommit = true;
	_readOnly = false;
	_oldIsolationLevel = -1;
	_theResource = null;
	_theConnection = DriverManager.getConnection(url, info);

	/*
	 * Any requirement for branding?
	 */

	JDBCBranding theBrander = JDBCBrandingFactory.getBranding(url);
		
	if (theBrander != null)
	    theBrander.brand(_theConnection);
	
	_url = url;
	_info = info;
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "JDBCConnection.finalize ()");
	}

	try
	{
	    endConnection();
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("JDBCConnection.finalize: "+e);
	}
	
	_theResource = null;
	_theConnection = null;
    }
    
public Statement createStatement () throws SQLException
    {
	checkConnection();
	
	registerDatabase();
	
	return _theConnection.createStatement();
    }


public Statement createStatement (int rs, int rc) throws SQLException
    {
	checkConnection();
	
	registerDatabase();
	
	return _theConnection.createStatement(rs, rc);
    }    

    
public PreparedStatement prepareStatement (String sql) throws SQLException
    {
	checkConnection();
	
	registerDatabase();

	return _theConnection.prepareStatement(sql);
    }


public PreparedStatement prepareStatement (String sql, int rs, int rc) throws SQLException
    {
	checkConnection();
	
	registerDatabase();
	
	return _theConnection.prepareStatement(sql, rs, rc);
    }

    
public CallableStatement prepareCall (String sql) throws SQLException
    {
	checkConnection();
	
	registerDatabase();

	return _theConnection.prepareCall(sql);
    }


public CallableStatement prepareCall (String sql, int rs, int rc) throws SQLException
    {
	checkConnection();
	
	registerDatabase();
	
	return _theConnection.prepareCall(sql, rs, rc);
    }


public String nativeSQL (String sql) throws SQLException
    {
	checkConnection();
	
	registerDatabase();

	return _theConnection.nativeSQL(sql);
    }


public Map getTypeMap () throws SQLException
    {
	checkConnection();

	return _theConnection.getTypeMap();
    }

public void setTypeMap (Map map) throws SQLException
    {
	checkConnection();

	_theConnection.setTypeMap(map);
    }

    
    /**
     * Not allowed.
     */
    
public void setAutoCommit (boolean autoCommit) throws SQLException
    {
	checkConnection();

	if (autoCommit)
	    throw new SQLException("Autocommit not allowed by OTS.");

	_theConnection.setAutoCommit(autoCommit);
    }

public boolean getAutoCommit () throws SQLException
    {
	checkConnection();

	return _autoCommit;
    }

public void commit () throws SQLException
    {
	checkConnection();

	throw new SQLException("Commit not allowed by OTS.");
    }

public void rollback () throws SQLException
    {
	checkConnection();

	throw new SQLException("Rollback not allowed by OTS.");
    }

    /**
     * If there is a transaction running when close is called then we throw
     * an exception and mark it as rollback only.
     */
    
public void close () throws SQLException
    {
	checkConnection();

	if (_theResource != null)
	    throw new SQLException("Transaction running when close called!");

	try
	{
	    endConnection();
	}
	catch (SQLException e)
	{
	    markRollbackOnly();

	    throw e;
	}
	
	_theConnection = null;
    }

public boolean isClosed () throws SQLException
    {
	checkConnection();
	
	if (_theConnection != null)
	    return _theConnection.isClosed();
	else
	    return true;
    }
    
public DatabaseMetaData getMetaData () throws SQLException
    {
	checkConnection();

	return _theConnection.getMetaData();
    }

    /**
     * Can only set readonly before we use the connection in a
     * given transaction!
     */
    
public void setReadOnly (boolean ro) throws SQLException
    {
	checkConnection();

	if (_theResource == null)
	    _readOnly = ro;
	else
	    throw new SQLException("Cannot set readonly when within a transaction!");
    }
    
public boolean isReadOnly () throws SQLException
    {
	checkConnection();

	return _readOnly;
    }

public void setCatalog (String cat) throws SQLException
    {
	checkConnection();
	
	registerDatabase();

	_theConnection.setCatalog(cat);
    }
	    
public String getCatalog () throws SQLException
    {
	checkConnection();
	
	registerDatabase();

	return _theConnection.getCatalog();
    }

public void setTransactionIsolation (int iso) throws SQLException
    {
	checkConnection();

	if (iso != Connection.TRANSACTION_SERIALIZABLE)
	    throw new SQLException("Must be Connection.TRANSACTION_SERIALIZABLE");
    }

public int getTransactionIsolation () throws SQLException
    {
	checkConnection();

	return _oldIsolationLevel;
    }

public SQLWarning getWarnings () throws SQLException
    {
	checkConnection();

	return _theConnection.getWarnings();
    }
    
public void clearWarnings () throws SQLException
    {
	checkConnection();

	_theConnection.clearWarnings();
    }

synchronized java.sql.Connection getConnection ()
    {
	return _theConnection;
    }
   
synchronized void removeResource ()
    {
	_theResource = null;
	
	try
	{
	    endConnection();
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("JDBCConnection.removeResource: "+e);
	}
    }

String connectionUrl ()
    {
	return _url;
    }

Properties connectionProperties ()
    {
	return _info;
    }

    /**
     * Mark the transaction as rollback only. This method does not
     * throw any exceptions because it is called when we already have
     * an exception to throw back to the application, and any exceptions
     * throw here will simply be ignored.
     */

private final void markRollbackOnly ()
    {
	try
	{
	    Coordinator coord = OTS.current().get_control().get_coordinator();		
	    coord.rollback_only();
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("JDBCConnection.markRollbackOnly - could not set rollback_only: "+e);
	}
    }
    
private final synchronized void checkConnection () throws SQLException
    {
	if (_theConnection == null)
	    throw new SQLException("Invalid connection.");
    }
    
private final synchronized void setupConnection () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_JDBC, "JDBCConnection.setupConnection ()");
	}

	/*
	 * Try to setup the db connection in a way which makes
	 * sense for the OTS.
	 */

	try
	{
	    _theConnection.setAutoCommit(false);
	    _autoCommit = false;
	}
	catch (SQLException e)
	{
	    /*
	     * Could not set autocommit off. In which case all
	     * calls to the db are separate top-level transactions,
	     * and this resource becomes useless.
	     */

	    ErrorStream.warning().println("JDBCConnection - could not disable autocommit.");
	    
	    _autoCommit = true;

	    markRollbackOnly();

	    throw e;
	}

	try
	{
	    DatabaseMetaData metaData = _theConnection.getMetaData();

	    if (metaData.supportsTransactionIsolationLevel(Connection.TRANSACTION_SERIALIZABLE))
	    {
		try
		{
		    _oldIsolationLevel = _theConnection.getTransactionIsolation();

		    if (_oldIsolationLevel != Connection.TRANSACTION_SERIALIZABLE)
			_theConnection.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
		}
		catch (SQLException e)
		{
		    ErrorStream.warning().println("JDBCConnection - failed to set isolation level: "+e);

		    throw e;
		}
	    }
	}
	catch (SQLException ex)
	{
	    markRollbackOnly();
	    
	    throw ex;
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("JDBCConnection - failed to set isolation level: "+e);

	    markRollbackOnly();
	    
	    throw new SQLException(e.toString());
	}
    }

private final synchronized void endConnection () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_JDBC, "JDBCConnection.endConnection ()");
	}

	if (_theConnection != null)
	{
	    if (_oldIsolationLevel != -1)
	    {
		try
		{
		    _theConnection.setTransactionIsolation(_oldIsolationLevel);
		}
		catch (SQLException e)
		{
		    ErrorStream.warning().print("JDBCResource.endConnection: attempt to reset isolation level ");
		    ErrorStream.stream().println(_oldIsolationLevel+" failed.");

		    _oldIsolationLevel = -1;

		    throw e;
		}

		_oldIsolationLevel = -1;
	    }
	}
    }
    
private final synchronized void registerDatabase () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_JDBC, "JDBCConnection.registerDatabase ()");
	}

	if (_theResource == null)
	{
	    setupConnection();
	    
	    try
	    {
		_theResource = new JDBCResource(this, _readOnly, _url, _info);
	    
		Coordinator coord = OTS.current().get_control().get_coordinator();		

		Resource res = _theResource.getReference();
		RecoveryCoordinator rc = coord.register_resource(res);
	    
		_theResource.setTransaction(coord);

		coord = null;
	    }
	    catch (Unavailable e1)
	    {
		throw new SQLException("JDBCConnection.registerDatabase - "+e1);
	    }
	    catch (Inactive e2)
	    {
		throw new SQLException("JDBCConnection.registerDatabase - "+e2);
	    }
	    catch (SystemException e3)
	    {
		throw new SQLException("JDBCConnection.registerDatabase - "+e3);
	    }
	}
    }

private java.sql.Connection _theConnection;
private boolean             _autoCommit;
private boolean             _readOnly;
private int                 _oldIsolationLevel;
private JDBCResource        _theResource;
private String              _url;
private Properties          _info;
    
};

