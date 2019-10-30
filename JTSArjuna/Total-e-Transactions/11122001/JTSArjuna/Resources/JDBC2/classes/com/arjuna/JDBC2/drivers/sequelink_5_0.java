/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: sequelink_5_0.java,v 1.1.6.1.10.2.8.1.2.1.46.1 2001/07/19 14:16:08 nmcl Exp $
 */

package com.arjuna.JDBC2.drivers;

import com.arjuna.JDBC2.ArjunaJDBC2DynamicClass;
import com.arjuna.JDBC2.ArjunaJDBC2BrandingFactory;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import com.merant.sequelink.jdbcx.datasource.SequeLinkDataSource;
import javax.sql.XADataSource;

import java.sql.SQLException;

/*
 * This is a stateless class to allow us access to the Oracle
 * specific API without hardwiring the code into the generic
 * JDBC2 driver.
 */

public class sequelink_5_0 implements ArjunaJDBC2DynamicClass
{

public sequelink_5_0 ()
    {
    }
    
public XADataSource getDataSource (String dbName) throws SQLException
    {
	return getDataSource(dbName, true);
    }
    
public synchronized XADataSource getDataSource (String dbName, boolean create) throws SQLException
    {
	try
	{
	    SequeLinkDataSource xads = new SequeLinkDataSource();
	    int index1 = dbName.indexOf(sequelink_5_0.driverName);
	    
	    if (index1 == -1)
		throw new SQLException("sequelink_5_0.getDataSource - db "+dbName+" not for Merant");
	    else
	    {
		/*
		 * Strip off any spurious parameters.
		 */

		int index2 = dbName.indexOf(sequelink_5_0.semicolon);
		String theUrl = null;

		if (index2 == -1)
		{
		    theUrl = dbName.substring(index1+sequelink_5_0.driverName.length());
		}
		else
		{
		    theUrl = dbName.substring(index1+sequelink_5_0.driverName.length(), index2);
		}

		/*
		 * From the string, get the database name, the server name,
		 * the port, etc.
		 */

		int thePort = 0;
		String theServer = null;
		String theDbName = null;

		index1 = dbName.indexOf(sequelink_5_0.databaseName);
		    
		if (index1 != -1)
		{
		    index2 = dbName.indexOf(sequelink_5_0.semicolon, index1);
		    
		    if (index2 == -1)
		    {
			theDbName = dbName.substring(index1+sequelink_5_0.databaseName.length());
		    }
		    else
		    {
			theDbName = dbName.substring(index1+sequelink_5_0.databaseName.length(), index2);
		    }
		}

		if (theDbName != null)
		    xads.setDatabaseName(theDbName);

		index1 = theUrl.indexOf(sequelink_5_0.colon);
		
		if (index1 != -1)
		{
		    try
		    {
			Integer i = new Integer(theUrl.substring(index1+1));
			
			thePort = i.intValue();
		    }
		    catch (Exception e)
		    {
			throw new SQLException(e.toString());
		    }

		    theServer = theUrl.substring(0, index1);
		}
		else
		{
		    theServer = theUrl;
		}
		
		xads.setServerName(theServer);
		xads.setPortNumber(thePort);

		return xads;
	    }
	}
	catch (SQLException e1)
	{
	    throw e1;
	}
	catch (Exception e2)
	{
	    throw new SQLException("sequelink_5_0 caught: " +e2);
	}
    }
    
public synchronized void shutdownDataSource (XADataSource ds) throws SQLException
    {
    }

private static final String driverName = "sequelink://";
private static final String semicolon = ";";
private static final String colon = ":";
private static final String databaseName = "databaseName=";

}
