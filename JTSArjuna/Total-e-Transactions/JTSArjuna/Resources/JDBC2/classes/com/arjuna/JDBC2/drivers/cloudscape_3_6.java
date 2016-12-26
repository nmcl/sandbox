/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: cloudscape_3_6.java,v 1.1.2.1 2001/08/01 14:19:57 jcoleman Exp $
 */

package com.arjuna.JDBC2.drivers;

import com.arjuna.JDBC2.ArjunaJDBC2DynamicClass;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import COM.cloudscape.core.DataSourceFactory;
import COM.cloudscape.core.XaDataSource;
import javax.sql.XADataSource;

import java.sql.SQLException;

/*
 * This is a stateless class to allow us access to the Cloudscape
 * specific API without hardwiring the code into the generic
 * JDBC2 driver.
 */

public class cloudscape_3_6 implements ArjunaJDBC2DynamicClass
{

public XADataSource getDataSource (String dbName) throws SQLException
    {
	return getDataSource(dbName, true);
    }
    
public synchronized XADataSource getDataSource (String dbName, boolean create) throws SQLException
    {
	try
	{
	    XaDataSource xads = (COM.cloudscape.core.XaDataSource) DataSourceFactory.getXADataSource();
	    int index1 = dbName.indexOf(cloudscape_3_6.driverName);

	    if (index1 == -1)
		throw new SQLException("cloudscape_3_6.getDataSource - db not for Cloudscape");
	    else
	    {
		/*
		 * Strip off any spurious parameters.
		 */

		int index2 = dbName.indexOf(cloudscape_3_6.semicolon);
		String theDbName = null;

		if (index2 == -1)
		{
		    theDbName = dbName.substring(index1+cloudscape_3_6.driverName.length());
		}
		else
		{
		    theDbName = dbName.substring(index1+cloudscape_3_6.driverName.length(),index2);
		}

		/*
		 * At present cloudscape does not allow remote
		 * XA connections. So, we know the thing after the :
		 * in the 'url' must be the database name. If this
		 * restriction changes, we'll need to determine the
		 * server name, port, and database name some other
		 * way.
		 */

		xads.setDatabaseName(theDbName);

		if (create)
		    xads.setCreateDatabase("create");
	    
		return (XADataSource) xads;
	    }
	}
	catch (SQLException e1)
	{
	    throw e1;
	}
	catch (Exception e2)
	{
	    throw new SQLException("cloudscape_3_6 caught: " +e2);
	}
    }
    
public synchronized void shutdownDataSource (XADataSource ds) throws SQLException
    {
	try
	{
	    XaDataSource xads = (COM.cloudscape.core.XaDataSource) ds;

	    xads.setShutdownDatabase("shutdown");
	}
	catch (Exception e)
	{
	    throw new SQLException("cloudscape_3_6 caught: "+e);
	}
    }

private static final String driverName = "cloudscape:";
private static final String semicolon = ";";
    
};
