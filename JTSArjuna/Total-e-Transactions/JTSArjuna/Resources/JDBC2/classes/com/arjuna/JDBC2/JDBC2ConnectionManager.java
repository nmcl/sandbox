/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBC2ConnectionManager.java,v 1.1.2.2.8.3.22.1.36.1.10.1.6.3 2001/11/27 14:00:20 nmcl Exp $
 */

package com.arjuna.JDBC2;

import java.util.*;

import java.sql.SQLException;

/*
 * Only ever create a single instance of a given connection, based upon the
 * user/password/url/dynamic_class options. If the connection we have cached
 * has been closed, then create a new one.
 */

class JDBC2ConnectionManager
{

    /*
     * Connections are pooled for the duration of a transaction.
     */

public static synchronized ArjunaJDBC2Connection create (String dbUrl, Properties info) throws SQLException
    {
	String user = info.getProperty(ArjunaJDBC2Driver.userName);
	String passwd = info.getProperty(ArjunaJDBC2Driver.password);
	String dynamic = info.getProperty(ArjunaJDBC2Driver.dynamicClass);
	Enumeration e = _connections.elements();
	ArjunaJDBC2Connection conn = null;

	if (dynamic == null)
	    dynamic = "";
	
	while (e.hasMoreElements())
	{
	    conn = (ArjunaJDBC2Connection) e.nextElement();

	    ConnectionInfo connInfo = conn.connectionInfo();
	    
	    if (connInfo.url().equals(dbUrl) && connInfo.user().equals(user) &&
		connInfo.password().equals(passwd) && connInfo.dynamicClass().equals(dynamic))
	    {
		try
		{
		    /*
		     * A connection may appear closed to a thread if another thread
		     * has bound it to a different transaction. If this is the case
		     * then we'll create a new connection for the new thread.
		     */

		    /*
		     * Should not overload the meaning of closed. Change!
		     */

		    if (!conn.isClosed())
		    {
			return conn;
		    }
		}
		catch (Exception ex)
		{
		    ex.printStackTrace();  // error, so try to create a new instance
		}
	    }
	}

	conn = new ArjunaJDBC2Connection(dbUrl, info);
	
	/*
	 * Will replace any old (closed) connection which had the
	 * same connection information.
	 */

	_connections.put(conn, conn);
	
	return conn;
    }

public static synchronized void remove (ArjunaJDBC2Connection conn)
    {
	_connections.remove(conn);
    }

private static Hashtable _connections = new Hashtable();
	    
}
