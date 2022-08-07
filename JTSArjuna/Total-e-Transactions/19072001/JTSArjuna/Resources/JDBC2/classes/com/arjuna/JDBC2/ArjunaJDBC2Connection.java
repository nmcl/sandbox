/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2Connection.javatmpl,v 1.3.4.2.4.2.4.5.4.5.8.1.2.1.2.2.8.1.2.1 2001/03/07 15:22:34 nmcl Exp $
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
































































package com.arjuna.JDBC2;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import com.arjuna.CosTransactions.ArjunaOTS.AtomicTransaction;
import com.arjuna.CosTransactions.ArjunaOTS.TopLevelTransaction;
import com.arjuna.jta.*;
import org.omg.CosTransactions.*;
import javax.transaction.*;
import javax.transaction.xa.*;
import javax.sql.*;
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
import javax.transaction.RollbackException;
import java.sql.SQLException;

/**
 * A transactional JDBC 2.0 connection. This wraps the real connection and
 * registers it with the transaction at appropriate times to ensure that
 * all worked performed by it may be committed or rolled back.
 *
 * Applications must not use this class directly.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaJDBC2Connection.javatmpl,v 1.3.4.2.4.2.4.5.4.5.8.1.2.1.2.2.8.1.2.1 2001/03/07 15:22:34 nmcl Exp $
 * @since JTS 2.0.
 */

public class ArjunaJDBC2Connection implements java.sql.Connection
{

public ArjunaJDBC2Connection (String dbName, Properties info) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Connection.ArjunaJDBC2Connection ( "+dbName+" )");
	}

	String user = info.getProperty(ArjunaJDBC2Driver.userName);
	String passwd = info.getProperty(ArjunaJDBC2Driver.password);
	String dynamic = info.getProperty(ArjunaJDBC2Driver.dynamicClass);

	if ((dynamic == null) || (dynamic.equals("")))
	{
	    _recoveryConnection = new ArjunaJDBC2RecoveryConnection(dbName, user, passwd, this);
	}
	else
	{
	    _recoveryConnection = new ArjunaJDBC2DynamicRecoveryConnection(dbName, user, passwd, dynamic, this);
	}

	/*
	 * Is there any "modifier" we are required to work with?
	 */

	_theModifier = ArjunaJDBC2ModifierFactory.getModifier(((ConnectionInfo)_recoveryConnection).dataSourceName());

	_theConnections = new Hashtable();
    }

public ArjunaJDBC2Connection (String dbName, String user, String passwd) throws SQLException
    {
	this(dbName, user, passwd, null);
    }
    
public ArjunaJDBC2Connection (String dbName, String user, String passwd, String dynamic) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Connection.ArjunaJDBC2Connection ( "+dbName+", "+user+", "+passwd+", "+dynamic+" )");
	}

	if ((dynamic == null) || (dynamic.equals("")))
	    _recoveryConnection = new ArjunaJDBC2RecoveryConnection(dbName, user, passwd, this);
	else
	    _recoveryConnection = new ArjunaJDBC2DynamicRecoveryConnection(dbName, user, passwd, dynamic, this);

	/*
	 * Any "modifier" required to work with?
	 */

	_theModifier = ArjunaJDBC2ModifierFactory.getModifier(((ConnectionInfo)_recoveryConnection).dataSourceName());

	_theConnections = new Hashtable();
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Connection.finalize ()");
	}

	try
	{
	    endConnection();
	}
	catch (Exception ex)
	{
	    ErrorStream.warning().println("ArjunaJDBC2Connection.finalize caught: "+ex);
	}

	_recoveryConnection = null;

	Enumeration e = _theConnections.elements();

	while (e.hasMoreElements())
	{
	    try
	    {
		Connection c = (Connection) e.nextElement();
		
		c.close();
	    }
	    catch (Exception ex)
	    {
	    }
	}

	_theConnections = null;
    }

public Statement createStatement () throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}
	
	return getConnection().createStatement();
    }

public Statement createStatement (int rs, int rc) throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}
	
	return getConnection().createStatement(rs, rc);
    }    
    
public PreparedStatement prepareStatement (String sql) throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}
	
	return getConnection().prepareStatement(sql);
    }

public PreparedStatement prepareStatement (String sql, int rs, int rc) throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}
	
	return getConnection().prepareStatement(sql, rs, rc);
    }
    
public CallableStatement prepareCall (String sql) throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}
	
	return getConnection().prepareCall(sql);
    }

public CallableStatement prepareCall (String sql, int rs, int rc) throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}
	
	return getConnection().prepareCall(sql, rs, rc);
    }

public String nativeSQL (String sql) throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}
	
	return getConnection().nativeSQL(sql);
    }

public Map getTypeMap () throws SQLException
    {
	return getConnection().getTypeMap();
    }

public void setTypeMap (Map map) throws SQLException
    {
	getConnection().setTypeMap(map);
    }
    
    /**
     * Not allowed if within a transaction.
     */
    
public void setAutoCommit (boolean autoCommit) throws SQLException
    {
	if (transactionRunning())
	{
	    if (autoCommit)
		throw new SQLException("Autocommit not allowed by Transaction Service.");
	}
	else
	{
	    getConnection().setAutoCommit(autoCommit);
	}
    }

public boolean getAutoCommit () throws SQLException
    {
	return getConnection().getAutoCommit();
    }

public void commit () throws SQLException
    {
	/*
	 * If there is a transaction running, then it cannot be terminated
	 * via the driver - the user must go through current.
	 */

	if (transactionRunning())
	{
	    throw new SQLException("Commit not allowed by Transaction Service.");
	}
	else
	    getConnection().commit();
    }

public void rollback () throws SQLException
    {
	if (transactionRunning())
	{
	    throw new SQLException("Rollback not allowed by Transaction Service.");
	}
	else
	    getConnection().rollback();
    }

public void close () throws SQLException
    {
	try
	{
	    /*
	     * Need to know whether this particular connection has
	     * outstanding resources waiting for it. If not then we
	     * can close, otherwise we can't. Currently we do not do
	     * a close if there are *any* transactions associated with
	     * the "connection", and rely upon garbage collection to do
	     * a close later.
	     */

	    if (_recoveryConnection.connections() == 0)
		JDBC2ConnectionManager.remove(this);  // finalize?

	    /*
	     * Delist resource if within a transaction.
	     */

	    TransactionManager tm = JTA_TransactionManager.transactionManager();
	    JTA_Transaction tx = (JTA_Transaction) tm.getTransaction();
		
	    if (tx != null)
	    {
		XAResource xares = _recoveryConnection.getResource();

		if (!tx.delistResource(xares, XAResource.TMSUCCESS))
		    throw new SQLException("Delist of resource failed");
	    }
	
	    // what about connections without xaCon?
	}
	catch (SQLException sqle)
	{
	    throw sqle;
	}
	catch (Exception e1)
	{
	    throw new SQLException("ArjunaJDBC2Connection.close error: "+e1);
	}
    }

    /**
     * If multiplexing is enabled then the connection is
     * never going to be closed since each call will
     * create a new connection if one does not already exist!
     */

public boolean isClosed () throws SQLException
    {
	try
	{
	    XAConnection xaConn = _recoveryConnection.getCurrentConnection();

	    if (xaConn != null)
	    {
		Connection conn = (Connection) _theConnections.get(xaConn);

		return conn.isClosed();
	    }
	    else
	    {
		if (_recoveryConnection.connections() == 0)
		    return true;
		else
		    return false;  // can't tell
	    }
	}
	catch (Exception e)
	{
	    return true; // NullPointerException et al
	}
    }
    
public DatabaseMetaData getMetaData () throws SQLException
    {
	return getConnection().getMetaData();
    }

    /**
     * Can only set readonly before we use the connection in a
     * given transaction!
     */
    
public void setReadOnly (boolean ro) throws SQLException
    {
	if (_recoveryConnection.connections() == 0)
	{
	    getConnection().setReadOnly(ro);
	}
	else
	    throw new SQLException("Cannot set readonly when within a transaction!");
    }
    
public boolean isReadOnly () throws SQLException
    {
	return getConnection().isReadOnly();
    }

public void setCatalog (String cat) throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}

	getConnection().setCatalog(cat);
    }
	    
public String getCatalog () throws SQLException
    {
	try
	{
	    checkTransaction();

	    registerDatabase();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction running, so do nothing.
	     */
	}

	return getConnection().getCatalog();
    }

public void setTransactionIsolation (int iso) throws SQLException
    {
	if (iso != Connection.TRANSACTION_SERIALIZABLE)
	    throw new SQLException("Must be Connection.TRANSACTION_SERIALIZABLE");
    }

public int getTransactionIsolation () throws SQLException
    {
	return getConnection().getTransactionIsolation();
    }

public SQLWarning getWarnings () throws SQLException
    {
	return getConnection().getWarnings();
    }
    
public void clearWarnings () throws SQLException
    {
	getConnection().clearWarnings();
    }

    /**
     * @return the Arjuna specific recovery connection information. This
     * should not be used by anything other than Arjuna.
     */

public final JTA_XAConnectionRecovery recoveryConnection ()
    {
	return _recoveryConnection;
    }

    /**
     * @return the XAResource associated with the current XAConnection.
     */

protected final XAResource getXAResource ()
    {
	try
	{
	    return _recoveryConnection.getResource();
	}
	catch (Exception e)
	{
	    return null;
	}
    }

    /*
     * Move the Connection hashtable into the recovery connection code
     * completely, and delegate to that for everything!
     */

final java.sql.Connection getConnection () throws SQLException
    {
	XAConnection xaConn = _recoveryConnection.getConnection();
	Connection conn = (Connection) _theConnections.get(xaConn);

	if (conn == null)
	{
	    conn = xaConn.getConnection();

	    _theConnections.put(xaConn, conn);

	    /*
	     * If we're within a transaction then set autocommit to
	     * false. Always set the connection to serializable though.
	     */

	    try
	    {
		TransactionManager tm = JTA_TransactionManager.transactionManager();
		JTA_Transaction tx = (JTA_Transaction) tm.getTransaction();

		if (tx != null)
		    conn.setAutoCommit(false);
	    }
	    catch (javax.transaction.SystemException ex)
	    {
		/*
		 * Some error occurred in getting the transaction. Better
		 * not allow anything more to happen.
		 */

		ErrorStream.warning().println("ArjunaJDBC2Connection - could not get transaction information: "+ex);

		throw new SQLException("JDBC2 connection error: "+ex.toString());
	    }
	    catch (SQLException e)
	    {
		/*
		 * Could not set autocommit off. In which case all
		 * calls to the db are separate top-level transactions,
		 * and this resource becomes useless.
		 */
		
		ErrorStream.warning().println("ArjunaJDBC2Connection - could not disable autocommit.");
		
		throw e;
	    }

	    try
	    {
		DatabaseMetaData metaData = conn.getMetaData();

		if (metaData.supportsTransactionIsolationLevel(Connection.TRANSACTION_SERIALIZABLE))
		{
		    try
		    {
			if (conn.getTransactionIsolation() != Connection.TRANSACTION_SERIALIZABLE)
			    conn.setTransactionIsolation(Connection.TRANSACTION_SERIALIZABLE);
		    }
		    catch (SQLException e)
		    {
			ErrorStream.warning().println("ArjunaJDBC2Connection - failed to set isolation level: "+e);

			throw e;
		    }
		}
	    }
	    catch (SQLException ex)
	    {
		throw ex;
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("ArjunaJDBC2Connection - failed to get isolation level: "+e);

		throw new SQLException("JDBC2 connection initialisation problem:" +e);
	    }
	}

	return conn;
    }

final ConnectionInfo connectionInfo ()
    {
	return (ConnectionInfo) _recoveryConnection;
    }

final void removeConnection (XAConnection xaConn)
    {
	_theConnections.remove(xaConn);
    }
    
private final boolean transactionRunning () throws SQLException
    {
	try
	{
	    if (JTA_TransactionManager.transactionManager().getTransaction() != null)
	    {
		return true;
	    }
	    else
	    {
		return false;
	    }
	}
	catch (Exception e)
	{
	    throw new SQLException(e.toString());
	}
    }

private final void endConnection () throws SQLException
    {
	try
	{
	    Enumeration e = _theConnections.elements();
	    
	    while (e.hasMoreElements())
	    {
		Connection conn = (Connection) e.nextElement();
		
		if ((conn != null) && !conn.isClosed())
		    conn.close();

		conn = null;
	    }
	    
	    _theConnections = null;
	    _recoveryConnection = null;
	}
	catch (SQLException e)
	{
	    ErrorStream.warning().println("ArjunaJDBC2Connection.endConnection caught: "+e);
	}
    }

    /*
     * Whenever a JDBC call is invoked on us we get an XAResource and
     * try to register it with the transaction. If the same thread
     * causes this to happen many times within the same transaction then
     * we will currently attempt to get and register many redundant XAResources
     * for it. The JTA implementation will detect this and ignore all but the
     * first for each thread. However, a further optimisation would be to trap
     * such calls here and not do a registration at all. This would require the
     * connection object to be informed whenever a transaction completes so
     * that it could flush its cache of XAResources though.
     */

private final synchronized void registerDatabase () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Connection.registerDatabase ()");
	}

	Connection theConnection = getConnection();
	
	if (theConnection != null)
	{
	    XAResource xares = null;

	    try
	    {
		TransactionManager tm = JTA_TransactionManager.transactionManager();
		JTA_Transaction tx = (JTA_Transaction) tm.getTransaction();

		if (tx == null)
		    return;
		
		Object[] params;
		
		if (_theModifier != null)
		    params = new Object[2];
		else
		    params = new Object[1];

		params[JTA_Transaction.XACONNECTION] = _recoveryConnection;

		if (_theModifier != null)
		    params[JTA_Transaction.XAMODIFIER] = _theModifier;

		/*
		 * Use our extended version of enlistResource.
		 */

		xares = _recoveryConnection.getResource();

		if (!tx.enlistResource(xares, params))
		{
		    /*
		     * Failed to enlist, so mark transaction as rollback only.
		     */

		    try
		    {
			tx.setRollbackOnly();
		    }
		    catch (Exception e)
		    {
			ErrorStream.warning().println("ArjunaJDBC2Connection.registerDatabase - could not mark transaction rollback");
			
			throw new SQLException(e.toString());
		    }

		    throw new SQLException("ArjunaJDBC2Connection.registerDatabase - enlistResource failed");
		}

		params = null;
		xares = null;
		tx = null;
		tm = null;
	    }
	    catch (RollbackException e1)
	    {
		throw new SQLException("ArjunaJDBC2Connection.registerDatabase - "+e1);
	    }
	    catch (SystemException e2)
	    {
		throw new SQLException("ArjunaJDBC2Connection.registerDatabase - "+e2);
	    }
	    catch (SQLException e3)
	    {
		throw e3;
	    }
	    catch (Exception e4)
	    {
		if (DebugController.enabled())
		    e4.printStackTrace();

		throw new SQLException(e4.toString());
	    }
	}
    }

private final void checkTransaction () throws SQLException, Unavailable
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Connection.checkTransaction ()");
	}

	try
	{
	    Control c = OTS.current().get_control();

	    if (c != null)
	    {
		Coordinator coord = c.get_coordinator();
	    
		if (!coord.is_top_level_transaction())
		    throw new SubtransactionsUnavailable();
	
		coord = null;
		c = null;
	    }
	    else
		throw new Unavailable();
	}
	catch (Unavailable e1)
	{
	    throw e1;
	}
	catch (SubtransactionsUnavailable e2)
	{
	    throw new SQLException("Subtransactions unavailable!");
	}
	catch (Exception e3)
	{
	    throw new SQLException("Could not get transaction information.");
	}
    }

private JTA_XAConnectionRecovery _recoveryConnection;
private JTA_XAModifier           _theModifier;
private Hashtable                _theConnections;
    
}

