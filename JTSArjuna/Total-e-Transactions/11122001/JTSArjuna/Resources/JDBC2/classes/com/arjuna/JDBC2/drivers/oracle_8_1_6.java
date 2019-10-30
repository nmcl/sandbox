/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: oracle_8_1_6.java,v 1.1.14.4.58.1 2001/07/19 14:16:08 nmcl Exp $
 */

package com.arjuna.JDBC2.drivers;

import com.arjuna.JDBC2.ArjunaJDBC2DynamicClass;
import com.arjuna.JDBC2.ArjunaJDBC2Driver;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import oracle.jdbc.xa.client.OracleXADataSource;
import oracle.jdbc.xa.OracleXid;
import oracle.jdbc.pool.*;
import javax.sql.XADataSource;

import java.sql.SQLException;

/*
 * This is a stateless class to allow us access to the Oracle
 * specific API without hardwiring the code into the generic
 * JDBC2 driver.
 */

public class oracle_8_1_6 implements ArjunaJDBC2DynamicClass
{

public oracle_8_1_6 ()
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
	    OracleXADataSource xads = new OracleXADataSource();
	    int index1 = dbName.indexOf(oracle_8_1_6.driverName);
	    
	    if (index1 == -1)
		throw new SQLException("oracle_8_1_6.getDataSource - db not for Oracle");
	    else
	    {
		/*
		 * Strip off any spurious parameters.
		 */

		int index2 = dbName.indexOf(oracle_8_1_6.semicolon);
		String theDbName = null;

		if (index2 == -1)
		{
		    theDbName = dbName.substring(index1+oracle_8_1_6.driverName.length());
		}
		else
		{
		    theDbName = dbName.substring(index1+oracle_8_1_6.driverName.length(),index2);
		}

		/*
		 * Rather than work our way through the string to determine
		 * the server name, the port, etc., we can make use
		 * of the Oracle specific method.
		 */

		xads.setURL(ArjunaJDBC2Driver.jdbc+oracle_8_1_6.driverName+theDbName);

		return xads;
	    }
	}
	catch (SQLException e1)
	{
	    throw e1;
	}
	catch (Exception e2)
	{
	    throw new SQLException("oracle_8_1_6 caught: " +e2);
	}
    }
    
public synchronized void shutdownDataSource (XADataSource ds) throws SQLException
    {
    }

private static final String driverName = "oracle:";
private static final String semicolon = ";";
    
};
