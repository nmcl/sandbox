/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: oracle.java,v 1.1 2000/04/06 11:00:33 nmcl Exp $
 */

package com.arjuna.JDBC2.drivers.modifiers;

import com.arjuna.jta.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import oracle.jdbc.xa.OracleXid;
import javax.transaction.xa.Xid;

import com.arjuna.jta.NotImplementedException;
import java.sql.SQLException;

/*
 * This is a stateless class to allow us to get round
 * problems in Oracle. For example, they can't work with
 * an arbitrary implementation of Xid - it has to be their
 * own implementation!
 */

public class oracle implements JTA_XAModifier
{

public Xid createXid (JTA_XID xid) throws NotImplementedException
    {
	try
	{
	    return new OracleXid(xid.getFormatId(), xid.getGlobalTransactionId(),
				 xid.getBranchQualifier());
	}
	catch (Exception e)
	{
	    System.err.println(e);
	    
	    return xid;
	}
    }
    
};
