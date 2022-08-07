/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBC2Environment.java,v 1.1.2.1.10.3.2.2.2.1.2.1 2001/02/14 10:41:44 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.ArjunaCommon.Common.EnvironmentManager;

/**
 * The module specific properties which can be set to configure the
 * system at runtime.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JDBC2Environment.java,v 1.1.2.1.10.3.2.2.2.1.2.1 2001/02/14 10:41:44 nmcl Exp $
 * @since JTS 2.0.
 */

public class JDBC2Environment implements com.arjuna.ArjunaCommon.Common.EnvironmentManager
{
 
public static final String JDBC2_MULTIPLEX_CONNECTION = "JDBC2_MULTIPLEX_CONNECTION";
public static final String JDBC2_USER_DB_ACCESS = "JDBC2_USER_DB_ACCESS";
public static final String JDBC2_TX_DB_ACCESS = "JDBC2_TX_DB_ACCESS";
public static final String JDBC2_BACKOFF_PERIOD = "JDBC2_BACKOFF_PERIOD";
    
public String getPropertyRange (String propertyName)
    {
	if (propertyName.equals(JDBC2_MULTIPLEX_CONNECTION))
	    return _ranges[0];
	if (propertyName.equals(JDBC2_USER_DB_ACCESS))
	    return _ranges[1];
	if (propertyName.equals(JDBC2_TX_DB_ACCESS))
	    return _ranges[2];
	if (propertyName.equals(JDBC2_BACKOFF_PERIOD))
	    return _ranges[3];

	return EnvironmentManager.INVALID;
    }

    /*
     * Do not re-order!
     */

private String[] _ranges = { "yesno:#notnull#", "string:#notnull#", "string:#notnull#", "int:0..#max#" };
    
}


