/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2DynamicRecoveryConnection.java,v 1.2.4.2.4.2.8.6.6.1.2.2.4.2.4.1.2.1.2.1.36.1 2001/07/19 14:16:06 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.*;
import javax.transaction.xa.*;

/**
 * This class is responsible for maintaining connection information
 * in such a manner that we can recover the connection to the XA
 * database in the event of a failure.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaJDBC2DynamicRecoveryConnection.java,v 1.2.4.2.4.2.8.6.6.1.2.2.4.2.4.1.2.1.2.1.36.1 2001/07/19 14:16:06 nmcl Exp $
 * @since JTS 2.0.
 */

public class ArjunaJDBC2DynamicRecoveryConnection implements JTA_XAConnectionRecovery, ConnectionInfo
{

public ArjunaJDBC2DynamicRecoveryConnection () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.ArjunaJDBC2DynamicRecoveryConnection()");
	}

	_dbName = null;
	_user = null;
	_passwd = null;
	_dynamic = null;
	_theConnections = new Hashtable();
	_theDataSource = null;
	_dynamicConnection = null;
	_jdbc2Connection = null;
    }
    
public ArjunaJDBC2DynamicRecoveryConnection (String dbName, String user,
					     String passwd, String dynamic,
					     ArjunaJDBC2Connection conn) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.ArjunaJDBC2DynamicRecoveryConnection( "+dbName+", "+user+", "+passwd+", "+dynamic+" )");
	}

	_dbName = dbName;
	_user = user;
	_passwd = passwd;
	_dynamic = dynamic;
	_theConnections = new Hashtable();
	_theDataSource = null;
	_dynamicConnection = null;
	_jdbc2Connection = conn;
    }

public void finalize ()
    {
	try
	{
	    if (_theConnections != null)
	    {
		Enumeration e = _theConnections.elements();

		while (e.hasMoreElements())
		{
		    XAConnection c = (XAConnection) e.nextElement();
					 
		    c.close();

		    c = null;
		}
	    }
	}
	catch (SQLException e)
	{
	    ErrorStream.warning().println(e.toString());
	}
    }
    
public boolean packInto (OutputObjectState os)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.packInto ()");
	}

	try
	{
	    os.packString(_dbName);
	    os.packString(_user);
	    os.packString(_passwd);
	    os.packString(_dynamic);

	    return true;
	}
	catch (Exception e)
	{
	    return false;
	}
    }
    
public boolean unpackFrom (InputObjectState os)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.unpackFrom ()");
	}

	try
	{
	    _dbName = os.unpackString();
	    _user = os.unpackString();
	    _passwd = os.unpackString();
	    _dynamic = os.unpackString();
	    
	    return true;
	}
	catch (Exception e)
	{
	    return false;
	}
    }

    /**
     * @return a new XAResource for this connection.
     */

public XAResource getResource () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.getResource ()");
	}

	try
	{
	    return getConnection().getXAResource();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection.getResource - caught "+e);
	}
    }

    /**
     * If there is a connection then return it. Do not create a
     * new connection otherwise.
     */

public synchronized XAConnection getCurrentConnection () throws SQLException
    {
	XAConnection conn = null;
	
	try
	{
	    TransactionManager tm = JTA_TransactionManager.transactionManager();
	    JTA_Transaction tx = (JTA_Transaction) tm.getTransaction();

	    if (tx == null)
		tx = ArjunaJDBC2DynamicRecoveryConnection.nullTransaction;

	    conn = (XAConnection) _theConnections.get(tx);
	}
	catch (Exception e)
	{
	}
	    
	return conn;
    }
    
public synchronized XAConnection getConnection () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.getConnection ()");
	}

	try
	{
	    TransactionManager tm = JTA_TransactionManager.transactionManager();
	    JTA_Transaction tx = (JTA_Transaction) tm.getTransaction();

	    if (tx == null)
		tx = ArjunaJDBC2DynamicRecoveryConnection.nullTransaction;

	    XAConnection conn = (XAConnection) _theConnections.get(tx);

	    if (conn == null)
	    {
		conn = createConnection(tx);

		/*
		 * Any requirement for branding?
		 */

		JDBC2Branding theBrander = ArjunaJDBC2BrandingFactory.getBranding(_dbName);
		
		if (theBrander != null)
		    theBrander.brand(conn);
	    }
	    
	    return conn;
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    throw new SQLException(e.toString());
	}
    }
    
public synchronized XADataSource getDataSource () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.getDataSource ()");
	}

	return _theDataSource;
    }

public synchronized int connections ()
    {
	return _theConnections.size();
    }

public synchronized void removeConnection (JTA_Transaction tx) throws SQLException
    {
	XAConnection conn = (XAConnection) _theConnections.remove(tx);

	if (conn != null)
	{
	    conn.close();

	    if (_jdbc2Connection != null)
		_jdbc2Connection.removeConnection(conn);
	    
	    conn = null;
	}
	else
	    throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection.removeConnection - unknown transaction!");
    }

public String user ()
    {
	return _user;
    }
    
public String password ()
    {
	return _passwd;
    }

public String url ()
    {
	return _dbName;
    }
    
public String dynamicClass ()
    {
	return _dynamic;
    }

public String dataSourceName ()
    {
	return _dbName;
    }
    
private XAConnection createConnection (JTA_Transaction tx) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.createConnection (tx)");
	}

	checkTransaction();
	
	if ((_dynamic == null) || (_dynamic.equals("")))
	    throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection - no dynamic class specified.");
	else
	{
	    try
	    {
		if (_theDataSource == null)
		{
		    Class c = Class.forName(_dynamic);

		    _dynamicConnection = (ArjunaJDBC2DynamicClass) c.newInstance();
		    _theDataSource = _dynamicConnection.getDataSource(_dbName);
		}

		XAConnection conn = null;
		
		if ((_user == null) && (_passwd == null))
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
					 FacilityCode.FAC_JDBC,
					 "ArjunaJDBC2DynamicRecoveryConnection - getting connection with no user");
		    }

		    conn = _theDataSource.getXAConnection();
		}
		else
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
					 FacilityCode.FAC_JDBC,
					 "ArjunaJDBC2DynamicRecoveryConnection - getting connection for user " + _user);
		    }

		    conn = _theDataSource.getXAConnection(_user, _passwd);
		}
		
		_theConnections.put(tx, conn);

		return conn;
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();
		
		throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection.initialise() - caught: "+e);
	    }
	}
    }

private void checkTransaction () throws SQLException
    {
	/*
	 * If we are not multiplexing different transactions through
	 * the same connection then this call had better be associated
	 * with the transaction this connection is already associated
	 * with, or else!
	 */
		
	if (!ArjunaJDBC2DynamicRecoveryConnection.multiplexConnection)
	{
	    /*
	     * Is this connection associated with a transaction?
	     */

	    if (_theConnections.size() == 1)
	    {
		try
		{
		    TransactionManager tm = JTA_TransactionManager.transactionManager();
		    JTA_Transaction tx = (JTA_Transaction) tm.getTransaction();
		    Enumeration e = _theConnections.elements();
		    JTA_Transaction currentTran = (JTA_Transaction) e.nextElement();

		    if (!currentTran.equals(tx))
			throw new SQLException("Error - connection already associated with a transaction!");
		}
		catch (SQLException e1)
		{
		    throw e1;
		}
		catch (Exception e2)
		{
		    throw new SQLException(e2.toString());
		}		
	    }
	}
    }
    
private String		  _dbName;
private String		  _user;
private String		  _passwd;
private String		  _dynamic;
private Hashtable	       _theConnections;
private XADataSource	    _theDataSource;
private ArjunaJDBC2DynamicClass _dynamicConnection;
private ArjunaJDBC2Connection   _jdbc2Connection;
    
private static final JTA_Transaction nullTransaction = JTA_Transaction.create(null);
private static boolean multiplexConnection = true;

    static
	{
	    String mc = PropertyManager.getProperty(JDBC2Environment.JDBC2_MULTIPLEX_CONNECTION);

	    if (mc != null)
	    {
		if (mc.equals("NO"))
		    ArjunaJDBC2DynamicRecoveryConnection.multiplexConnection = false;
	    }
	}
    
};
