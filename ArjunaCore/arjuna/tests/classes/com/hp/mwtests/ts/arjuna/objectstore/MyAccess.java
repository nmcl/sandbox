package com.hp.mwtests.ts.arjuna.objectstore;

/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: MyAccess.java,v 1.3 2003/06/19 10:51:19 nmcl Exp $
 */

import com.arjuna.ats.arjuna.gandiva.ObjectName;

import java.sql.*;

public class MyAccess implements com.arjuna.ats.arjuna.objectstore.jdbc.JDBCAccess
{

    public Connection getConnection () throws SQLException
    {
	return null;
    }
    
    public String tableName ()
    {
	return null;
    }

    public boolean dropTable ()
    {
	return false;
    }

    public void initialise (ObjectName objName)
    {
    }
    
}
