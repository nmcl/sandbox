/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2Driver.java,v 1.3.4.2.4.1.10.1.16.2.2.3.2.1.4.1.8.1 2001/04/06 12:04:18 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.ArjunaCommon.Common.*;
import java.util.*;
import java.sql.*;

import java.sql.SQLException;
import java.lang.ExceptionInInitializerError;

/**
 * Transactional JDBC 2.0 driver. This returns transactional JDBC connections
 * when required.
 *
 * We try to make this look as much like a standard
 * JDBC driver as possible.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaJDBC2Driver.java,v 1.3.4.2.4.1.10.1.16.2.2.3.2.1.4.1.8.1 2001/04/06 12:04:18 nmcl Exp $
 * @since JTS 2.0.
 */

public class ArjunaJDBC2Driver implements java.sql.Driver
{

public static final String arjunaDriver = "jdbc:arjuna:";
public static final String jdbc = "jdbc:";
public static final String userName = "user";
public static final String password = "password";
public static final String dynamicClass = "DYNAMIC_CLASS";
public static final String createDb = "CREATE_DB";

public ArjunaJDBC2Driver ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Driver.ArjunaJDBC2Driver ()");
	}
    }

public Connection connect (String url, Properties info) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Driver.connect ( "+url+" )");
	}

	if (!url.startsWith(ArjunaJDBC2Driver.arjunaDriver))
	{
	    return null;
	}
	else
	{
	    return JDBC2ConnectionManager.create(url.substring(ArjunaJDBC2Driver.arjunaDriver.length()), info);
	}
    }
    
public boolean acceptsURL (String url) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Driver.acceptsURL ( "+url+" )");
	}

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
	return 1;
    }
    
public boolean jdbcCompliant ()
    {
	return true;
    }

public DriverPropertyInfo[] getPropertyInfo (String url, Properties info) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "ArjunaJDBC2Driver.getPropertyInfo ( "+url+" )");
	}

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

