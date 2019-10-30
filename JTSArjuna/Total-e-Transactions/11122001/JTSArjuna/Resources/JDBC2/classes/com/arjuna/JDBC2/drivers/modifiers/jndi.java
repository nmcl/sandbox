/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: jndi.java,v 1.1.2.1 2001/10/15 11:53:48 nmcl Exp $
 */

package com.arjuna.JDBC2.drivers.modifiers;

import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.xa.Xid;

import com.arjuna.jta.NotImplementedException;

public class jndi implements JTA_XAModifier
{

public jndi ()
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
	throw new NotImplementedException();
    }

public XAConnection getConnection (XAConnection conn) throws NotImplementedException
    {
	throw new NotImplementedException();
    }

public boolean canReuseAfterDelist () throws NotImplementedException
    {
	throw new NotImplementedException();
    }
    
}
