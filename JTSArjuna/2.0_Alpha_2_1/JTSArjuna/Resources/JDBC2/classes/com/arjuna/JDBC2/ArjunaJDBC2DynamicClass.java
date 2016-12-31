/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2DynamicClass.java,v 1.2 2000/04/06 11:00:32 nmcl Exp $
 */

package com.arjuna.JDBC2;

import java.util.*;
import java.sql.*;
import javax.sql.*;

import java.sql.SQLException;

/*
 * This class is responsible for maintaining connection information
 * in such a manner that we can recover the connection to the XA
 * database in the event of a failure.
 */

public interface ArjunaJDBC2DynamicClass
{

public XADataSource getDataSource (String dbName) throws SQLException;
public XADataSource getDataSource (String dbName, boolean create) throws SQLException;
public void shutdownDataSource (XADataSource ds) throws SQLException;

};
