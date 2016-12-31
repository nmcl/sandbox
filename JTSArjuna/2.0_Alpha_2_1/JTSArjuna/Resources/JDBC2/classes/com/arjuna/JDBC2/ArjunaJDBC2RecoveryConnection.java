/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2RecoveryConnection.java,v 1.2.4.1.2.2 2000/07/28 14:41:45 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.xa.*;
import javax.naming.*;

/*
 * This class is responsible for maintaining connection information
 * in such a manner that we can recover the connection to the XA
 * database in the event of a failure.
 */

public class ArjunaJDBC2RecoveryConnection implements JTA_XAConnectionRecovery, ConnectionInfo
{

public ArjunaJDBC2RecoveryConnection () throws SQLException    
    {
	_dbName = null;
	_user = null;
	_passwd = null;
	_theConnection = null;
	_theDataSource = null;
	_connections = 0;
    }
    
public ArjunaJDBC2RecoveryConnection (String dbName, String user, String passwd) throws SQLException
    {
	_dbName = dbName;
	_user = user;
	_passwd = passwd;
	_theConnection = null;
	_theDataSource = null;
	_connections = 0;
    }
    
public boolean packInto (OutputObjectState os)
    {
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
	try
	{
	    return _theConnection.getXAResource();
	}
	catch (Exception e)
	{
	    throw new SQLException("ArjunaJDBC2RecoveryConnection.getResource() - caught "+e);
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
	    
	    throw new SQLException("ArjunaJDBC2RecoveryConnection - removeConnection makes negative connections!");
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
	return "";
    }

private void initialise () throws SQLException
    {
	try
	{
	    Context ctx = new InitialContext();
	    
	    _theDataSource = (XADataSource) ctx.lookup(_dbName);

	    if ((_user == null) && (_passwd == null))
		_theConnection = _theDataSource.getXAConnection();
	    else
		_theConnection = _theDataSource.getXAConnection(_user, _passwd);
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    throw new SQLException("ArjunaJDBC2RecoveryConnection.initialise() caught exception: "+e);
	}
    }
    
private String       _dbName;
private String       _user;
private String       _passwd;
private XAConnection _theConnection;
private XADataSource _theDataSource;
private int          _connections;
 
};
