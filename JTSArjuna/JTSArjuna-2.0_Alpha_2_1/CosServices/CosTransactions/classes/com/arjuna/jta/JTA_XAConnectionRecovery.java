/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTA_XAConnectionRecovery.java,v 1.2.6.1 2000/07/28 14:45:20 nmcl Exp $
 */

package com.arjuna.jta;

import com.arjuna.ArjunaCommon.Common.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.xa.*;

public interface JTA_XAConnectionRecovery
{

public static final int AUTO_RECOVERY = 0;
public static final int OBJECT_RECOVERY = 1;

public boolean packInto (OutputObjectState os);
public boolean unpackFrom (InputObjectState os);

public XAResource getResource () throws SQLException;
public XAConnection getConnection () throws SQLException;
public XADataSource getDataSource () throws SQLException;

/*
 * Returns the number of XAResource connections.
 */

public int connections ();
public void addConnection () throws SQLException;
public void removeConnection () throws SQLException;

};
