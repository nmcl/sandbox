/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JTA_XAModifier.java,v 1.1.28.1.4.1.70.3 2001/10/15 11:52:20 nmcl Exp $
 */

package com.arjuna.jta;

import com.arjuna.jta.JTA_XID;
import javax.transaction.xa.Xid;
import javax.sql.*;
import javax.transaction.*;
import javax.transaction.xa.*;

/**
 * Instances of this class enable us to work around problems
 * in certain databases (specifically Oracle).
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JTA_XAModifier.java,v 1.1.28.1.4.1.70.3 2001/10/15 11:52:20 nmcl Exp $
 * @since JTS 1.2.4.
 */

public interface JTA_XAModifier
{

    /**
     * Initialise the modifier.
     *
     * @return the database name to use.
     * @since JTS 2.2.
     */

public String initialise (String dbName);
 
public Xid createXid (JTA_XID xid) throws NotImplementedException;
public XAConnection getConnection (XAConnection conn) throws NotImplementedException;

public boolean canReuseAfterDelist () throws NotImplementedException;
 
};
