/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCConnectionManager.java,v 1.1.2.1 2000/07/17 08:30:11 nmcl Exp $
 */

package com.arjuna.JDBC;

import java.util.*;

import java.sql.SQLException;

class JDBCConnectionManager
{

public static OTS_JDBCConnection create (String dbUrl, Properties info) throws SQLException
    {
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
	_connections.remove(conn);
    }

private static Hashtable _connections = new Hashtable();
    
};
