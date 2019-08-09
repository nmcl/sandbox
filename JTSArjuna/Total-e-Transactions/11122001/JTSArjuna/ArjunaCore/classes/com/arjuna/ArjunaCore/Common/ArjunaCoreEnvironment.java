/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ArjunaCoreEnvironment.java,v 1.1.4.2.2.1.2.1.2.2.2.1.38.1 2001/07/19 13:45:55 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Common;

import com.arjuna.ArjunaCommon.Common.EnvironmentManager;

/**
 * The various runtime configuration options for
 * ArjunaCore.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaCoreEnvironment.java,v 1.1.4.2.2.1.2.1.2.2.2.1.38.1 2001/07/19 13:45:55 nmcl Exp $
 * @since JTS 1.0.
 * @see com.arjuna.ArjunaCommon.Common.EnvironmentManager
 */

public class ArjunaCoreEnvironment implements com.arjuna.ArjunaCommon.Common.EnvironmentManager
{

public static final String ACTION_STORE= "ACTION_STORE";    
public static final String ASYNC_COMMIT = "ASYNC_COMMIT";
public static final String ASYNC_PREPARE = "ASYNC_PREPARE";
public static final String COMMIT_ONE_PHASE = "COMMIT_ONE_PHASE";
public static final String LOCALOSROOT = "LOCALOSROOT";
public static final String LOCKSTORE_DIR = "LOCKSTORE_DIR";
public static final String LOCKSTORE_TYPE = "LOCKSTORE_TYPE";
public static final String MAINTAIN_HEURISTICS = "MAINTAIN_HEURISTICS";
public static final String MULTIPLE_LOCKSTORE = "MULTIPLE_LOCKSTORE";    
public static final String OBJECTSTORE_DIR = "OBJECTSTORE_DIR";
public static final String OBJECTSTORE_SYNC = "OBJECTSTORE_SYNC";
public static final String OBJECTSTORE_TYPE = "OBJECTSTORE_TYPE";
public static final String HASHED_DIRECTORIES = "HASHED_DIRECTORIES";
public static final String SINGLE_LOCKSTORE = "SINGLE_LOCKSTORE";    
public static final String TRANSACTION_LOG = "TRANSACTION_LOG";
public static final String TRANSACTION_SYNC = "TRANSACTION_SYNC";
public static final String READONLY_OPTIMISATION = "READONLY_OPTIMISATION";
public static final String CLASSIC_PREPARE = "CLASSIC_PREPARE";
public static final String ALLOW_NESTED_LOCKING = "ALLOW_NESTED_LOCKING";
public static final String ENABLE_STATISTICS = "ENABLE_STATISTICS";
public static final String SHARED_TRANSACTION_LOG = "SHARED_TRANSACTION_LOG";

    /**
     * @since JTS 2.2.
     */

public static final String START_DISABLED = "START_DISABLED";
    
public String getPropertyRange (String propertyName)
    {
	if (propertyName.equals(ACTION_STORE))
	    return _ranges[0];
	if (propertyName.equals(ASYNC_COMMIT))
	    return _ranges[1];
	if (propertyName.equals(ASYNC_PREPARE))
	    return _ranges[2];
	if (propertyName.equals(COMMIT_ONE_PHASE))
	    return _ranges[3];
	if (propertyName.equals(LOCALOSROOT))
	    return _ranges[4];
	if (propertyName.equals(LOCKSTORE_DIR))
	    return _ranges[5];
	if (propertyName.equals(LOCKSTORE_TYPE))
	    return _ranges[6];
	if (propertyName.equals(MAINTAIN_HEURISTICS))
	    return _ranges[7];
	if (propertyName.equals(MULTIPLE_LOCKSTORE))
	    return _ranges[8];
	if (propertyName.equals(OBJECTSTORE_DIR))
	    return _ranges[9];
	if (propertyName.equals(OBJECTSTORE_SYNC))
	    return _ranges[10];
	if (propertyName.equals(OBJECTSTORE_TYPE))
	    return _ranges[11];
	if (propertyName.equals(HASHED_DIRECTORIES))
	    return _ranges[12];
	if (propertyName.equals(SINGLE_LOCKSTORE))
	    return _ranges[13];
	if (propertyName.equals(TRANSACTION_LOG))
	    return _ranges[14];
	if (propertyName.equals(TRANSACTION_SYNC))
	    return _ranges[15];
	if (propertyName.equals(READONLY_OPTIMISATION))
	    return _ranges[16];
	if (propertyName.equals(CLASSIC_PREPARE))
	    return _ranges[17];
	if (propertyName.equals(ALLOW_NESTED_LOCKING))
	    return _ranges[18];
	if (propertyName.equals(ENABLE_STATISTICS))
	    return _ranges[19];
	if (propertyName.equals(SHARED_TRANSACTION_LOG))
	    return _ranges[20];
	if (propertyName.equals(START_DISABLED))
	    return _ranges[21];

	return EnvironmentManager.INVALID;
    }

    /*
     * Do not re-order!
     */

private String[] _ranges = { "string:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "string:#notnull#", "string:#notnull#", "string:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "string:#notnull#", "onoff:#notnull#", "string:#notnull#", "int:0...#max#", "yesno:#notnull#", "string:#notnull#", "onoff:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "yesno:#notnull#", "yesno:#notnull#" };

}

