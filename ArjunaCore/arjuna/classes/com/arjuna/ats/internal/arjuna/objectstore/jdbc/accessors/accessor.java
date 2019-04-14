/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: accessor.java,v 1.1 2003/06/19 10:50:35 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore.jdbc.accessors;

import com.arjuna.ats.arjuna.objectstore.jdbc.JDBCAccess;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.Environment;
import com.arjuna.ats.arjuna.gandiva.ObjectName;

import java.sql.*;

import com.arjuna.ats.arjuna.exceptions.FatalError;
import java.sql.SQLException;

/**
 * Do not return a connection which participates within the
 * transaction 2-phase commit protocol! All connections will have
 * auto-commit set to true, or we will not be able to use them.
 * So don't return an Arjuna JDBC 1.0 or 2.x connection.
 *
 * @since JTS 2.1.
 */

public class accessor implements JDBCAccess
{

    public accessor ()
    {
	_tableName = null;
	_dropTable = false;
	_url = null;
    }
    
    public Connection getConnection () throws SQLException
    {
	return DriverManager.getConnection(_url, null);
    }
    
    public String tableName ()
    {
	return _tableName;
    }
    
    public boolean dropTable ()
    {
	return _dropTable;
    }

    public void initialise (ObjectName objName)
    {
	try
	{
	    _url = objName.getStringAttribute(ArjunaNames.Implementation_ObjectStore_JDBC_url());
	    _tableName = objName.getStringAttribute(ArjunaNames.Implementation_ObjectStore_JDBC_tableName());

	    long drop = objName.getLongAttribute(ArjunaNames.Implementation_ObjectStore_JDBC_dropTable());

	    if (drop == 1)
		_dropTable = true;
	}
	catch (Exception ex)
	{
	    throw new FatalError(toString()+" : "+ex);
	}

	if (_url == null)
	    throw new FatalError(toString()+" : invalid ObjectName parameter!");
    }

    private String  _tableName;
    private boolean _dropTable;
    private String  _url;
    
}
