/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_Environment.java,v 1.2.2.1.30.2.4.3.4.1 2001/02/14 10:40:15 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.EnvironmentManager;

/**
 * The various property values that may be specified at runtime to
 * change the configuration and behaviour of the system.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_Environment.java,v 1.2.2.1.30.2.4.3.4.1 2001/02/14 10:40:15 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_Environment implements com.arjuna.ArjunaCommon.Common.EnvironmentManager
{

public static final String OTS_TRANSACTION_MANAGER = "OTS_TRANSACTION_MANAGER";
public static final String OTS_NEED_TRAN_CONTEXT = "OTS_NEED_TRAN_CONTEXT";
public static final String OTS_ALWAYS_PROPAGATE_CONTEXT = "OTS_ALWAYS_PROPAGATE_CONTEXT";
public static final String OTS_INTERPOSITION = "OTS_INTERPOSITION";
public static final String OTS_CHECKED_TRANSACTIONS = "OTS_CHECKED_TRANSACTIONS";
public static final String OTS_TX_REAPER_TIMEOUT = "OTS_TX_REAPER_TIMEOUT";
public static final String OTS_SUPPORT_SUBTRANSACTIONS = "OTS_SUPPORT_SUBTRANSACTIONS";
public static final String OTS_SUPPORT_ROLLBACK_SYNC = "OTS_SUPPORT_ROLLBACK_SYNC";
public static final String OTS_SUPPORT_INTERPOSED_SYNCHRONIZATION = "OTS_SUPPORT_INTERPOSED_SYNCHRONIZATION";
public static final String OTS_DEFAULT_TIMEOUT = "OTS_DEFAULT_TIMEOUT";
public static final String OTS_PROPAGATE_TERMINATOR = "OTS_PROPAGATE_TERMINATOR";
public static final String OTS_TX_REAPER_MODE = "OTS_TX_REAPER_MODE";
public static final String OTS_CONTEXT_PROP_MODE = "OTS_CONTEXT_PROP_MODE";
    
public String getPropertyRange (String propertyName)
    {
	if (propertyName.equals(OTS_TRANSACTION_MANAGER))
	    return _ranges[0];
	if (propertyName.equals(OTS_NEED_TRAN_CONTEXT))
	    return _ranges[1];
	if (propertyName.equals(OTS_ALWAYS_PROPAGATE_CONTEXT))
	    return _ranges[2];
	if (propertyName.equals(OTS_INTERPOSITION))
	    return _ranges[3];
	if (propertyName.equals(OTS_CHECKED_TRANSACTIONS))
	    return _ranges[4];
	if (propertyName.equals(OTS_TX_REAPER_TIMEOUT))
	    return _ranges[5];
	if (propertyName.equals(OTS_SUPPORT_SUBTRANSACTIONS))
	    return _ranges[6];
	if (propertyName.equals(OTS_SUPPORT_ROLLBACK_SYNC))
	    return _ranges[7];
	if (propertyName.equals(OTS_SUPPORT_INTERPOSED_SYNCHRONIZATION))
	    return _ranges[8];
	if (propertyName.equals(OTS_DEFAULT_TIMEOUT))
	    return _ranges[9];
	if (propertyName.equals(OTS_PROPAGATE_TERMINATOR))
	    return _ranges[10];
	if (propertyName.equals(OTS_TX_REAPER_MODE))
	    return _ranges[11];
	if (propertyName.equals(OTS_CONTEXT_PROP_MODE))
	    return _ranges[12];

	return EnvironmentManager.INVALID;
    }

    /*
     * Do not re-order!
     */

private String[] _ranges = { "yesno:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "string:ArjunaXID,ArjunaStrictXID,ArjunaRestrictedXID,OSI", "yesno:#notnull#", "int:0...#max#", "yesno:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "int:0...#max#", "yesno:#notnull#", "string:NORMAL,DYNAMIC", "string:NONE,CONTEXT,INTERPOSITION" };
    
}
