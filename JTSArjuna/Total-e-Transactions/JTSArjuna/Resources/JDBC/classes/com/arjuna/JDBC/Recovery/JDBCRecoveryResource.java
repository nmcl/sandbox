/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBCRecoveryResource.java,v 1.1.2.1 2000/05/31 13:53:17 nmcl Exp $
 */

package com.arjuna.JDBC.Recovery;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.JDBC.JDBCResource;

import com.arjuna.JDBC.JDBCException;
import java.sql.SQLException;

/*
 * Used for crash recovery.
 */

public class JDBCRecoveryResource extends JDBCResource
{
 
public JDBCRecoveryResource (Uid id) throws SQLException, JDBCException
    {
	super(id);
    }

public boolean recover () 
    {
	return super.recover();
    }

};


