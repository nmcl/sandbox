/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTA_XAConnectionRecovery.java,v 1.2.6.1.8.2.8.2.4.1.62.2 2001/09/21 12:00:21 nmcl Exp $
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
 * @version $Id: JTA_XAConnectionRecovery.java,v 1.2.6.1.8.2.8.2.4.1.62.2 2001/09/21 12:00:21 nmcl Exp $
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
 * @return true if the connection is being used within a transaction,
 * false otherwise.
 */

public boolean inuse ();

/**
 * @since JTS 2.2.
 */
	   
public void reset ();  // reset the connection for new useage

/**
 * @since JTS 2.2.
 */

public void close ();  // close the connection and make available for gc
 
/**
 * Remember the transaction we are being used within.
 *
 * @since JTS 2.2.
 */

public boolean setTransaction (JTA_Transaction tx);

/**
 * Is this the same transaction?
 *
 * @return true if the connection can be used by this
 * transaction, false otherwise.
 * @since JTS 2.2.
 */

public boolean validTransaction (JTA_Transaction tx);
 
}

