/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2DynamicRecoveryConnection.java,v 1.2.4.2.4.2.8.6.6.1.2.2.4.2.4.1.2.1.2.1.36.1.10.5.4.7 2001/11/05 16:50:47 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.*;
import javax.transaction.xa.*;

import com.arjuna.jta.NotImplementedException;

/**
 * This class is responsible for maintaining connection information
 * in such a manner that we can recover the connection to the XA
 * database in the event of a failure.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaJDBC2DynamicRecoveryConnection.java,v 1.2.4.2.4.2.8.6.6.1.2.2.4.2.4.1.2.1.2.1.36.1.10.5.4.7 2001/11/05 16:50:47 nmcl Exp $
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
	_theConnection = null;
	_theDataSource = null;
	_dynamicConnection = null;
	_theXAResource = null;
	_theTransaction = null;
	_theArjunaConnection = null;
	_theModifier = ArjunaJDBC2ModifierFactory.getModifier(dataSourceName());

	if (_theModifier != null)
	    _dbName = _theModifier.initialise(_dbName);
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
	_theConnection = null;
	_theDataSource = null;
	_dynamicConnection = null;
	_theXAResource = null;
	_theTransaction = null;
	_theArjunaConnection = conn;
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
	    if (_theXAResource == null)
		_theXAResource = getConnection().getXAResource();

	    return _theXAResource;
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection.getResource - caught "+e);
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
    
    /**
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
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.getConnection ()");
	}

	try
	{
	    if (_theConnection == null)
	    {
		createConnection();

		/*
		 * Any requirement for branding?
		 */

		JDBC2Branding theBrander = ArjunaJDBC2BrandingFactory.getBranding(_dbName);
		
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
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.getDataSource ()");
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
	return _dynamic;
    }

public String dataSourceName ()
    {
	return _dbName;
    }
    
private final void createConnection () throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2DynamicRecoveryConnection.createConnection");
	}

	if ((_dynamic == null) || (_dynamic.equals("")))
	{
	    throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection - no dynamic class specified.");
	}
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

		if ((_user == null) && (_passwd == null))
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							     FacilityCode.FAC_JDBC,
							     "ArjunaJDBC2DynamicRecoveryConnection - getting connection with no user");
		    }

		    _theConnection = _theDataSource.getXAConnection();
		}
		else
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							     FacilityCode.FAC_JDBC,
							     "ArjunaJDBC2DynamicRecoveryConnection - getting connection for user " + _user);
		    }

		    _theConnection = _theDataSource.getXAConnection(_user, _passwd);
		}
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();
		
		throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection.initialise() - caught: "+e);
	    }
	}
    }

private String		        _dbName;
private String		        _user;
private String		        _passwd;
private String		        _dynamic;
private XAConnection            _theConnection;
private XADataSource	        _theDataSource;
private ArjunaJDBC2DynamicClass _dynamicConnection;
private XAResource              _theXAResource;
private JTA_Transaction         _theTransaction;
private ArjunaJDBC2Connection   _theArjunaConnection;
private JTA_XAModifier          _theModifier;

}

