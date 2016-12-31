/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2DynamicRecoveryConnection.java,v 1.2.4.2.4.2 2000/07/28 14:41:44 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.xa.*;

/*
 * This class is responsible for maintaining connection information
 * in such a manner that we can recover the connection to the XA
 * database in the event of a failure.
 */

public class ArjunaJDBC2DynamicRecoveryConnection implements JTA_XAConnectionRecovery, ConnectionInfo
{

public ArjunaJDBC2DynamicRecoveryConnection () throws SQLException
    {
	_dbName = null;
	_user = null;
	_passwd = null;
	_dynamic = null;
	_theConnection = null;
	_theDataSource = null;
	_dynamicConnection = null;
	_connections = 0;
    }
    
public ArjunaJDBC2DynamicRecoveryConnection (String dbName, String user,
					     String passwd, String dynamic) throws SQLException
    {
	_dbName = dbName;
	_user = user;
	_passwd = passwd;
	_dynamic = dynamic;
	_theConnection = null;
	_theDataSource = null;
	_dynamicConnection = null;
	_connections = 0;
    }
    
public boolean packInto (OutputObjectState os)
    {
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

public XAResource getResource () throws SQLException
    {
	try
	{
	    return _theConnection.getXAResource();
	}
	catch (Exception e)
	{
	    throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection.getResource - caught "+e);
	}
    }
    
public XAConnection getConnection () throws SQLException
    {
	if (_connections == 0)
	    initialise();

	return _theConnection;
    }
    
public XADataSource getDataSource () throws SQLException
    {
	return _theDataSource;
    }

public synchronized int connections ()
    {
	return _connections;
    }

public synchronized void addConnection () throws SQLException
    {
	_connections++;
    }
    
public synchronized void removeConnection () throws SQLException
    {
	_connections--;

	if (_connections < 0)
	{
	    _connections = 0;
	
	    throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection - removeConnection makes negative connections!");
	}
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

private void initialise () throws SQLException
    {
	if (_dynamic == null)
	    throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection - no dynamic class specified.");
	else
	{
	    try
	    {
		Class c = Class.forName(_dynamic);

		_dynamicConnection = (ArjunaJDBC2DynamicClass) c.newInstance();
		_theDataSource = _dynamicConnection.getDataSource(_dbName);

		if ((_user == null) && (_passwd == null))
		    _theConnection = _theDataSource.getXAConnection();
		else
		    _theConnection = _theDataSource.getXAConnection(_user, _passwd);
	    }
	    catch (Exception e)
	    {
		throw new SQLException("ArjunaJDBC2DynamicRecoveryConnection.initialise() - caught: "+e);
	    }
	}
    }
    
private String                  _dbName;
private String                  _user;
private String                  _passwd;
private String                  _dynamic;
private XAConnection            _theConnection;
private XADataSource            _theDataSource;
private ArjunaJDBC2DynamicClass _dynamicConnection;
private int                     _connections;
    
};
