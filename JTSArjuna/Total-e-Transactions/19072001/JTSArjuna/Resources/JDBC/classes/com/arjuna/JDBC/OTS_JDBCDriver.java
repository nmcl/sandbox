/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_JDBCDriver.java,v 1.8.4.3.4.1.24.1.4.1 2000/12/21 11:21:34 nmcl Exp $
 */

package com.arjuna.JDBC;

import java.util.*;
import java.sql.*;

import java.sql.SQLException;

/**
 * The transactional JDBC driver. This hands out transactional JDBC connections
 * when required.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_JDBCDriver.java,v 1.8.4.3.4.1.24.1.4.1 2000/12/21 11:21:34 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class OTS_JDBCDriver implements java.sql.Driver
{

public static String arjunaDriver = "jdbc:arjuna_jdbc1:";
public static String jdbcDriver = "jdbc:";

public OTS_JDBCDriver ()
    {
    }

public Connection connect (String url, Properties info) throws SQLException
    {
	if (!url.startsWith(OTS_JDBCDriver.arjunaDriver))
	    return null;
	else
	    return JDBCConnectionManager.create(jdbcDriver+url.substring(OTS_JDBCDriver.arjunaDriver.length()), info);
    }
    
public boolean acceptsURL (String url) throws SQLException
    {
	if (url != null)
	{
	    if (url.indexOf(OTS_JDBCDriver.arjunaDriver) == -1)
		return false;
	    else
		return true;
	}
	else
	    return false;
    }

public int getMajorVersion ()
    {
	return 2;
    }
    
public int getMinorVersion ()
    {
	return 1;
    }
    
public boolean jdbcCompliant ()
    {
	return true;
    }

public DriverPropertyInfo[] getPropertyInfo (String url, Properties info) throws SQLException
    {
	int index = url.indexOf(OTS_JDBCDriver.arjunaDriver);

	if (index == -1)
	    return null;
	else
	{
	    String theUrl = url.substring(index+OTS_JDBCDriver.arjunaDriver.length());
	    Driver d = DriverManager.getDriver(theUrl);

	    if (d != null)
		return d.getPropertyInfo(theUrl, info);
	    else
		return null;
	}
    }
    
    static
    {
	try
	{
	    DriverManager.registerDriver(new OTS_JDBCDriver());
	}
	catch (Exception e)
	{
	    throw new ExceptionInInitializerError(e);
	}
    }
    
};

