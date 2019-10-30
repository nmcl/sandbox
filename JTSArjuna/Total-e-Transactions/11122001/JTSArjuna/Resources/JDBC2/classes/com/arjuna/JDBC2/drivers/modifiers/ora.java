/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ora.java,v 1.1.2.1 2002/06/10 15:53:40 nmcl Exp $
 */

package com.arjuna.JDBC2.drivers.modifiers;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.xa.Xid;
import oracle.jdbc.xa.OracleXid;

import com.arjuna.jta.NotImplementedException;
import java.sql.SQLException;

/*
 * This is a stateless class to allow us to get round
 * problems in Oracle. For example, they can't work with
 * an arbitrary implementation of Xid - it has to be their
 * own implementation!
 */

public class ora implements JTA_XAModifier
{

public String initialise (String dbName)
    {
	int index = dbName.indexOf(extensions.reuseConnectionTrue);
	int end = extensions.reuseConnectionTrue.length();
	
	if (index != -1)
	    _reuseConnection  = true;
	else
	{
	    index = dbName.indexOf(extensions.reuseConnectionFalse);
	    end = extensions.reuseConnectionFalse.length();
	}

	/*
	 * If at start, then this must be a JNDI URL. So remove component.
	 */

	if (index != 0)
	    return dbName;
	else
	    return dbName.substring(end + 1);  // remember colon
    }
    
public Xid createXid (JTA_XID xid) throws NotImplementedException
    {
	try
	{
	    return new OracleXid(xid.getFormatId(), xid.getGlobalTransactionId(),
				 xid.getBranchQualifier());
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e.toString());
	    
	    return xid;
	}
    }

public XAConnection getConnection (XAConnection conn) throws NotImplementedException
    {
	if (_reuseConnection)
	    return conn;
	else
	{
	    if (conn != null)
	    {
		try
		{
		    conn.close();
		}
		catch (SQLException e)
		{
		}
	    }
	    
	    return null;
	}
    }

public boolean canReuseAfterDelist () throws NotImplementedException
    {
	return true;
    }
    
private boolean _reuseConnection = true;
 
};
