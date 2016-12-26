/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: cloudscape.java,v 1.1.2.3 2001/10/15 11:53:47 nmcl Exp $
 */

package com.arjuna.JDBC2.drivers.modifiers;

import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.xa.Xid;

import com.arjuna.jta.NotImplementedException;
import java.sql.SQLException;

public class cloudscape implements JTA_XAModifier
{

public cloudscape ()
    {
    }

public String initialise (String dbName)
    {
	int index = dbName.indexOf(extensions.reuseConnectionTrue);
	int end = extensions.reuseConnectionTrue.length();
	
	if (index == -1)
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
	return xid;
    }

public XAConnection getConnection (XAConnection conn) throws NotImplementedException
    {
	/*
	 * Would like to call close on the connection here, but
	 * for some reason Cloudscape won't let us, or at least
	 * won't work afterwards!
	 */

	return null;
    }

public boolean canReuseAfterDelist () throws NotImplementedException
    {
	return false;
    }
    
}
