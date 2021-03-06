/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCAccess.java,v 1.1.2.2.10.1.62.1 2001/09/28 10:29:23 nmcl Exp $
 */

package com.arjuna.JDBC2.Implementation.ObjectStore;

import java.sql.Connection;

import java.sql.SQLException;

/**
 * Do not return a connection which participates within the
 * transaction 2-phase commit protocol! All connections will have
 * auto-commit set to true, or we will not be able to use them.
 * So don't return an Arjuna JDBC 1.0 or 2.x connection.
 *
 * @since JTS 2.1.
 */

public interface JDBCAccess
{

public Connection getConnection () throws SQLException;
public String tableName ();
public boolean dropTable ();
 
}
