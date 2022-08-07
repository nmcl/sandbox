/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTA_XAConnectionRecovery.java,v 1.2.6.1.8.2.8.2.4.1 2000/12/21 11:21:24 nmcl Exp $
 */

package com.arjuna.jta;

import com.arjuna.ArjunaCommon.Common.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.xa.*;

/**
 * XA connection implementations must provide an implementation
 * of this class.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JTA_XAConnectionRecovery.java,v 1.2.6.1.8.2.8.2.4.1 2000/12/21 11:21:24 nmcl Exp $
 * @since JTS 1.2.4.
 */

public interface JTA_XAConnectionRecovery
{

public static final int AUTO_RECOVERY = 0;
public static final int OBJECT_RECOVERY = 1;

public boolean packInto (OutputObjectState os);
public boolean unpackFrom (InputObjectState os);

public XAResource getResource () throws SQLException;
public XAConnection getConnection () throws SQLException;
public XAConnection getCurrentConnection () throws SQLException;
public XADataSource getDataSource () throws SQLException;

/**
 * Returns the number of XAResource connections.
 */

public int connections ();
public void removeConnection (JTA_Transaction tx) throws SQLException;

}

