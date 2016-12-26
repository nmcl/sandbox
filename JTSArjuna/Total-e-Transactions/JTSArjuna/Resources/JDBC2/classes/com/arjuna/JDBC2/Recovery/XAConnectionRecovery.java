/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XAConnectionRecovery.java,v 1.1.2.1.6.1.2.1 2001/02/22 11:07:37 nmcl Exp $
 */

package com.arjuna.JDBC2.Recovery;

import java.sql.*;
import javax.sql.*;
import javax.transaction.*;
import javax.transaction.xa.*;

/**
 * To perform recovery on arbitrary connections we may need
 * to recreate those connections. Users can provide implementations
 * of this interface which we will use at recovery time to re-create
 * db connections and from them perform recovery.
 *
 * @since JTS 2.1.
 */

public interface XAConnectionRecovery
{

public JDBC2RecoveryConnection getConnection () throws SQLException;

public boolean initialise (String p) throws SQLException;

/**
 * Iterate through all of the connections this instance provides
 * access to.
 */
 
public boolean hasMoreConnections ();
 
}

