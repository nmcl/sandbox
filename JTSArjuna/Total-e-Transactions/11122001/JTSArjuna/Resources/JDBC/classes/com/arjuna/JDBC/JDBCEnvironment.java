/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCEnvironment.java,v 1.1.2.1.2.1.2.1 2001/02/14 10:41:24 nmcl Exp $
 */

package com.arjuna.JDBC;

import com.arjuna.ArjunaCommon.Common.EnvironmentManager;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JDBCEnvironment.java,v 1.1.2.1.2.1.2.1 2001/02/14 10:41:24 nmcl Exp $
 * @since JTS 2.1.
 */

public class JDBCEnvironment implements EnvironmentManager
{

public static final String JDBC_BACKOFF_PERIOD = "JDBC_BACKOFF_PERIOD";
    
public String getPropertyRange (String propertyName)
    {
	if (propertyName.equals(JDBC_BACKOFF_PERIOD))
	    return _ranges[0];

	return EnvironmentManager.INVALID;
    }

    /*
     * Do not re-order!
     */

private String[] _ranges = { "int:0..#max#" };
 
}
