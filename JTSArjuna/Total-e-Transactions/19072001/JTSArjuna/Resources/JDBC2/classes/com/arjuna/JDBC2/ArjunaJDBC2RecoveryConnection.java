/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2RecoveryConnection.java,v 1.2.4.1.2.2.8.4.8.2.2.2.2.1.2.1.2.1 2001/02/22 11:19:16 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.*;
import javax.transaction.xa.*;
import javax.naming.*;

/**
 * This class is responsible for maintaining connection information
 * in such a manner that we can recover the connection to the XA
 * database in the event of a failure.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaJDBC2RecoveryConnection.java,v 1.2.4.1.2.2.8.4.8.2.2.2.2.1.2.1.2.1 2001/02/22 11:19:16 nmcl Exp $
 * @since JTS 2.0.
 */

public class ArjunaJDBC2RecoveryConnection implements JTA_XAConnectionRecovery, ConnectionInfo
{

public ArjunaJDBC2RecoveryConnection () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2RecoveryConnection.ArjunaJDBC2RecoveryConnection ()");
	}

	_dbName = null;
	_user = null;
	_passwd = null;
	_theConnections = new Hashtable();
	_theDataSource = null;
	_jdbc2Connection = null;
    }
    
public ArjunaJDBC2RecoveryConnection (String dbName, String user,
				      String passwd,
				      ArjunaJDBC2Connection conn) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2RecoveryConnection.ArjunaJDBC2RecoveryConnection ( "+dbName+", "+user+", "+passwd+" )");
	}

	_dbName = dbName;
	_user = user;
	_passwd = passwd;
	_theConnections = new Hashtable();
	_theDataSource = null;
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
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2RecoveryConnection.packInto ()");
	}

	try
	{
	    os.packString(_dbName);
	    os.packString(_user);
	    os.packString(_passwd);

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
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2RecoveryConnection.unpackFrom ()");
	}

	try
	{
	    _dbName = os.unpackString();
	    _user = os.unpackString();
	    _passwd = os.unpackString();
	    
	    return true;
	}
	catch (Exception e)
	{
	    return false;
	}
    }

public String getDatabaseName ()
    {
	return _dbName;
    }
    
public XAResource getResource () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2RecoveryConnection.getResource ()");
	}

	try
	{
	    return getConnection().getXAResource();
	}
	catch (Exception e)
	{
	    throw new SQLException("ArjunaJDBC2RecoveryConnection.getResource() - caught "+e);
	}
    }

    /*
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
		tx = ArjunaJDBC2RecoveryConnection.nullTransaction;

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
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2RecoveryConnection.getConnection ()");
	}

	try
	{
	    TransactionManager tm = JTA_TransactionManager.transactionManager();
	    JTA_Transaction tx = (JTA_Transaction) tm.getTransaction();

	    if (tx == null)
		tx = ArjunaJDBC2RecoveryConnection.nullTransaction;

	    XAConnection conn = (XAConnection) _theConnections.get(tx);

	    if (conn == null)
	    {
		conn = createConnection(tx);

		/*
		 * Any requirement for branding?
		 */

		JDBC2Branding theBrander = ArjunaJDBC2BrandingFactory.getBranding(dataSourceName());
		
		if (theBrander == null)  // try the dbName, just in case!
		    theBrander = ArjunaJDBC2BrandingFactory.getBranding(_dbName);

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
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2RecoveryConnection.getDataSource ()");
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
	return "";
    }

public String dataSourceName ()
    {
	if (_theDataSource != null)
	    return _theDataSource.toString();
	else
	    return "";
    }

private XAConnection createConnection (JTA_Transaction tx) throws SQLException
    {
	checkTransaction();
	
	try
	{
	    if (_theDataSource == null)
	    {
		Hashtable env = new Hashtable();
		Enumeration e = PropertyManager.propertyNames();
		
		/*
		 * Look through the properties for any Context related
		 * stuff.
		 */

		while (e.hasMoreElements())
		{
		    String name = (String) e.nextElement();
		    
		    if (name.startsWith("Context."))
			env.put(translate(name), PropertyManager.getProperty(name));
		}

		if (env.size() > 0)
		{
		    Context ctx = new InitialContext(env);
		    _theDataSource = (XADataSource) ctx.lookup(_dbName);
		}
		else
		    _theDataSource = null;

		if (_theDataSource == null)
		    throw new SQLException("Could not resolve JNDI XADataSource");
	    }

	    XAConnection conn = null;
	    
	    if ((_user == null) && (_passwd == null))
		conn = _theDataSource.getXAConnection();
	    else
		conn = _theDataSource.getXAConnection(_user, _passwd);

	    _theConnections.put(tx, conn);

	    return conn;	    
	}
	catch (SQLException ex)
	{
	    throw ex;
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    throw new SQLException("ArjunaJDBC2RecoveryConnection.initialise() caught exception: "+e);
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
		
	if (!ArjunaJDBC2RecoveryConnection.multiplexConnection)
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

    /**
     * Warning: roadworks ahead!!
     *
     * For some reasons JNDI uses different property names internally for
     * specifying things like the initial context to those it expects
     * users to manipulate. Why?! There are some really stupid people
     * at Sun!!
     */

private final String translate (String name)
    {
	try
	{
	    if (name.equals("Context.APPLET"))
		return Context.APPLET;
	    if (name.equals("Context.AUTHORITATIVE"))
		return Context.AUTHORITATIVE;
	    if (name.equals("Context.BATCHSIZE"))
		return Context.BATCHSIZE;
	    if (name.equals("Context.DNS_URL"))
		return Context.DNS_URL;
	    if (name.equals("Context.INITIAL_CONTEXT_FACTORY"))
		return Context.INITIAL_CONTEXT_FACTORY;
	    if (name.equals("Context.LANGUAGE"))
		return Context.LANGUAGE;
	    if (name.equals("Context.OBJECT_FACTORIES"))
		return Context.OBJECT_FACTORIES;
	    if (name.equals("Context.PROVIDER_URL"))
		return Context.PROVIDER_URL;
	    if (name.equals("Context.REFERRAL"))
		return Context.REFERRAL;
	    if (name.equals("Context.SECURITY_AUTHENTICATION"))
		return Context.SECURITY_AUTHENTICATION;
	    if (name.equals("Context.SECURITY_CREDENTIALS"))
		return Context.SECURITY_CREDENTIALS;
	    if (name.equals("Context.SECURITY_PRINCIPAL"))
		return Context.SECURITY_PRINCIPAL;
	    if (name.equals("Context.SECURITY_PROTOCOL"))
		return Context.SECURITY_PROTOCOL;
	    if (name.equals("Context.STATE_FACTORIES"))
		return Context.STATE_FACTORIES;
	    if (name.equals("Context.URL_PKG_PREFIXES"))
		return Context.URL_PKG_PREFIXES;
	}
	catch (NullPointerException ex)
	{
	    // name is null
	}
	
	return name;
    }

private String                _dbName;
private String                _user;
private String                _passwd;
private Hashtable             _theConnections;
private XADataSource          _theDataSource;
private ArjunaJDBC2Connection _jdbc2Connection;
    
private static final JTA_Transaction nullTransaction = JTA_Transaction.create(null);

private static boolean multiplexConnection = true;

    static
	{
	    String mc = PropertyManager.getProperty(JDBC2Environment.JDBC2_MULTIPLEX_CONNECTION);

	    if (mc != null)
	    {
		if (mc.equals("NO"))
		    ArjunaJDBC2RecoveryConnection.multiplexConnection = false;
	    }
	}
    
}
