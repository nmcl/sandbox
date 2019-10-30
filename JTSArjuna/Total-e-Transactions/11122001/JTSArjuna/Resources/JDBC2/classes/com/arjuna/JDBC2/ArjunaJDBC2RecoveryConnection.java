/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2RecoveryConnection.java,v 1.2.4.1.2.2.8.4.8.2.2.2.2.1.2.1.2.1.54.4.4.7 2001/11/05 16:50:47 nmcl Exp $
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
 * @version $Id: ArjunaJDBC2RecoveryConnection.java,v 1.2.4.1.2.2.8.4.8.2.2.2.2.1.2.1.2.1.54.4.4.7 2001/11/05 16:50:47 nmcl Exp $
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
	_theConnection = null;
	_theDataSource = null;
	_theXAResource = null;
	_theTransaction = null;
	_theArjunaConnection = null;

	/*
	 * Create a jndi specific modifier first, so that we can then
	 * use this to find out what the end-point datasource really
	 * is.
	 */

	com.arjuna.JDBC2.drivers.modifiers.jndi jndiModifier = new com.arjuna.JDBC2.drivers.modifiers.jndi();

	_dbName = jndiModifier.initialise(_dbName);
	
	createDataSource();
	
	_theModifier = ArjunaJDBC2ModifierFactory.getModifier(dataSourceName());

	if (_theModifier != null)
	    _dbName = _theModifier.initialise(_dbName);
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
	_theConnection = null;
	_theDataSource = null;
	_theXAResource = null;
	_theTransaction = null;
	_theArjunaConnection = conn;

	/*
	 * Create a jndi specific modifier first, so that we can then
	 * use this to find out what the end-point datasource really
	 * is.
	 */

	com.arjuna.JDBC2.drivers.modifiers.jndi jndiModifier = new com.arjuna.JDBC2.drivers.modifiers.jndi();

	_dbName = jndiModifier.initialise(_dbName);	

	createDataSource();

	_theModifier = ArjunaJDBC2ModifierFactory.getModifier(dataSourceName());

	if (_theModifier != null)
	    _dbName = _theModifier.initialise(_dbName);
    }

public void finalize ()
    {
	try
	{
	    if (_theConnection != null)
	    {
		_theConnection.close();
		_theConnection = null;
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
	    if (_theXAResource == null)
		_theXAResource = getConnection().getXAResource();

	    return _theXAResource;
	}
	catch (Exception e)
	{
	    throw new SQLException("ArjunaJDBC2RecoveryConnection.getResource() - caught "+e);
	}
    }

public final synchronized void close ()
    {
	reset();

	if (_theModifier != null)
	{
	    try
	    {
		_theConnection = _theModifier.getConnection(_theConnection);
	    }
	    catch (Exception ex)
	    {
		_theConnection = null;
	    }
	}
	else
	    _theConnection = null;

	/*
	 * If no connection, then we can't reuse it and should let the
	 * ArjunaJDBC2Connection object know so that it releases us.
	 */

	if (_theConnection == null)
	{
	    _theArjunaConnection.reset();

	    //	    JDBC2ConnectionManager.remove(_theArjunaConnection);
	}
    }

public final synchronized void reset ()
    {
	_theXAResource = null;
	_theTransaction = null;
    }

public synchronized boolean setTransaction (JTA_Transaction tx)
    {
	if (_theTransaction == null)
	{
	    _theTransaction = tx;
	    
	    return true;
	}
	else
	{
	    /*
	     * In case we have already set it for this transaction.
	     */

	    if (validTransaction(tx))
		return true;
	    else
		return false;
	}
    }

public synchronized boolean validTransaction (JTA_Transaction tx)
    {
	boolean valid = true;
	
	if (_theTransaction != null)
	    valid = _theTransaction.equals(tx);
	
	return valid;
    }
	
    /*
     * If there is a connection then return it. Do not create a
     * new connection otherwise.
     */

public synchronized XAConnection getCurrentConnection () throws SQLException
    {
	return _theConnection;
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
	    if (_theConnection == null)
	    {
		createConnection();

		/*
		 * Any requirement for branding?
		 */

		JDBC2Branding theBrander = ArjunaJDBC2BrandingFactory.getBranding(dataSourceName());
		
		if (theBrander == null)  // try the dbName, just in case!
		    theBrander = ArjunaJDBC2BrandingFactory.getBranding(_dbName);

		if (theBrander != null)
		    theBrander.brand(_theConnection);
	    }
	    
	    return _theConnection;
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

public synchronized boolean inuse ()
    {
	return (boolean) (_theXAResource != null);
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

private final void createDataSource () throws SQLException
    {
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
    
private final void createConnection () throws SQLException
    {
	try
	{
	    if (_theDataSource == null)
		createDataSource();

	    if ((_user == null) && (_passwd == null))
		_theConnection = _theDataSource.getXAConnection();
	    else
		_theConnection = _theDataSource.getXAConnection(_user, _passwd);
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

private String                 _dbName;
private String                 _user;
private String                 _passwd;
private XAConnection           _theConnection;
private XADataSource           _theDataSource;
private XAResource             _theXAResource;
private JTA_Transaction        _theTransaction;
private ArjunaJDBC2Connection  _theArjunaConnection;
private JTA_XAModifier         _theModifier;
    
}
