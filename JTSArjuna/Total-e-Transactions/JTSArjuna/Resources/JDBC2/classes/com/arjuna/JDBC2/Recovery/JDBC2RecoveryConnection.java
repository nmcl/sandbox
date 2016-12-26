/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBC2RecoveryConnection.java,v 1.1.2.1.2.2 2001/03/07 15:23:51 nmcl Exp $
 */

package com.arjuna.JDBC2.Recovery;

import com.arjuna.JDBC2.ArjunaJDBC2Driver;
import com.arjuna.JDBC2.ArjunaJDBC2Connection;

import java.util.Properties;
import java.sql.*;
import javax.sql.*;
import javax.transaction.*;
import javax.transaction.xa.*;

/**
 * To perform recovery on arbitrary connections we may need
 * to recreate those connections.
 *
 * The ArjunaJDBC2Connection class must not be used directly by
 * applications, hence the requirement for this class.
 *
 * @since JTS 2.1.
 */

public class JDBC2RecoveryConnection extends ArjunaJDBC2Connection
{

public JDBC2RecoveryConnection (String dbName, Properties info) throws SQLException
    {
	super(dbName, info);
    }

public JDBC2RecoveryConnection (String dbName, String user, String passwd) throws SQLException
    {
	this(dbName, user, passwd, null);
    }
    
public JDBC2RecoveryConnection (String dbName, String user, String passwd, String dynamic) throws SQLException
    {
	super(dbName, user, passwd, dynamic);
    }
 
}

