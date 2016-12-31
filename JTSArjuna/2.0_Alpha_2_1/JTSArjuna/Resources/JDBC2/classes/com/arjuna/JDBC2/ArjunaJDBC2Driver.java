/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2Driver.java,v 1.3.4.2.4.1 2000/07/17 08:30:13 nmcl Exp $
 */

package com.arjuna.JDBC2;

import java.util.*;
import java.sql.*;

import java.sql.SQLException;
import java.lang.ExceptionInInitializerError;

/*
 * We try to make this look as much like a standard
 * JDBC driver as possible. However, there are some
 * places where this won't work.
 */

public class ArjunaJDBC2Driver implements java.sql.Driver
{

public static final String arjunaDriver = "jdbc:arjuna:";
public static final String jdbc = "jdbc:";
public static final String userName = "USER";
public static final String password = "PASSWORD";
public static final String dynamicClass = "DYNAMIC_CLASS";
public static final String createDb = "CREATE_DB";

public ArjunaJDBC2Driver ()
    {
    }

public Connection connect (String url, Properties info) throws SQLException
    {
	if (!url.startsWith(ArjunaJDBC2Driver.arjunaDriver))
	    return null;
	else
	{
	    return JDBC2ConnectionManager.create(url.substring(ArjunaJDBC2Driver.arjunaDriver.length()), info);
	}
    }
    
public boolean acceptsURL (String url) throws SQLException
    {
	if (url != null)
	{
	    if (url.indexOf(ArjunaJDBC2Driver.arjunaDriver) == -1)
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
	int index = url.indexOf(ArjunaJDBC2Driver.arjunaDriver);

	if (index == -1)
	    return null;
	else
	{
	    String theUrl = url.substring(index+ArjunaJDBC2Driver.arjunaDriver.length());
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
	    DriverManager.registerDriver(new ArjunaJDBC2Driver());
	}
	catch (Exception e)
	{
	    throw new ExceptionInInitializerError(e);
	}
    }
    
};

