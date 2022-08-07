/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCConnectionManager.java,v 1.1.2.1.10.1 2000/08/30 13:42:59 nmcl Exp $
 */

package com.arjuna.JDBC;

import com.arjuna.ArjunaCommon.Common.*;
import java.util.*;

import java.sql.SQLException;

class JDBCConnectionManager
{

public static OTS_JDBCConnection create (String dbUrl, Properties info) throws SQLException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "JDBCConnectionManager.create ( "+dbUrl+" )");
	}

	OTS_JDBCConnection conn = (OTS_JDBCConnection) _connections.get(dbUrl);
	
	if (conn != null)
	    return conn;
	else
	    conn = new OTS_JDBCConnection(dbUrl, info);
	    
	_connections.put(conn, conn);

	return conn;
    }

public static void remove (OTS_JDBCConnection conn)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_JDBC, "JDBCConnectionManager.remove ()");
	}

	_connections.remove(conn);
    }

private static Hashtable _connections = new Hashtable();
    
};
