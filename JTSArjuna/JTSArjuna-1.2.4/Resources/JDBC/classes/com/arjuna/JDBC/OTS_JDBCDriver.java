/*
 * Copyright (C) 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_JDBCDriver.java,v 1.1 1999/06/22 15:12:38 nmcl Exp $
 */

package com.arjuna.JDBC;

import java.util.*;
import java.sql.*;

import java.sql.SQLException;

public class OTS_JDBCDriver implements java.sql.Driver
{

public static String arjunaDriver = "arjuna:";

public OTS_JDBCDriver ()
    {
    }

public Connection connect (String url, Properties info) throws SQLException
    {
	int index = url.indexOf(OTS_JDBCDriver.arjunaDriver);

	if (index == -1)
	    return null;
	else
	    return new OTS_JDBCConnection(url.substring(index+OTS_JDBCDriver.arjunaDriver.length()), info);
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
	return 1;
    }
    
public int getMinorVersion ()
    {
	return 0;
    }
    
public boolean jdbcCompliant ()
    {
	return false;
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
    
};

