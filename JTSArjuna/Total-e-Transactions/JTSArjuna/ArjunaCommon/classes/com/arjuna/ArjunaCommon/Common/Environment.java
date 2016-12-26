/*
 * Copyright (C) 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Environment.java,v 1.2.4.1.6.1.2.1.22.4.2.1.2.1.2.1.28.1.2.1.2.3 2001/07/19 13:41:59 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

/**
 * The various property variables that can be set at
 * runtime to configure the some of the classes within
 * the package.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Environment.java,v 1.2.4.1.6.1.2.1.22.4.2.1.2.1.2.1.28.1.2.1.2.3 2001/07/19 13:41:59 nmcl Exp $
 * @since JTS 1.0.
 */

public class Environment implements EnvironmentManager
{

public static final String PROPERTIES_FILE = "PROPERTIES_FILE";

    /**
     * @since JTS 2.1.
     */

public static final String VERBOSE_PROPERTY_MANAGER = "VERBOSE_PROPERTY_MANAGER";
public static final String DEBUG_ENABLED = "DEBUG_ENABLED";
public static final String DEBUG_LEVEL = "DEBUG_LEVEL";
public static final String VIS_LEVEL = "VIS_LEVEL";
public static final String FAC_LEVEL = "FAC_LEVEL";
public static final String DEBUG_STREAM = "DEBUG_STREAM";
public static final String ERROR_STREAM = "ERROR_STREAM";

    /**
     * @since JTS 2.1.
     */

public static final String STATIC_INVENTORY_IMPLE = "STATIC_INVENTORY_IMPLE";

    /**
     * @since JTS 2.2.
     */

public static final String PROPERTY_MANAGER_IMPLEMENTATION = "PROPERTY_MANAGER_IMPLEMENTATION";

    /**
     * @since JTS 2.1.1.
     */

public static final String VAR_DIR = "VAR_DIR";

public String getPropertyRange (String propertyName)
    {
	if (propertyName.equals(PROPERTIES_FILE))
	    return _ranges[0];
	if (propertyName.equals(VERBOSE_PROPERTY_MANAGER))
	    return _ranges[1];
	if (propertyName.equals(DEBUG_ENABLED))
	    return _ranges[2];
	if (propertyName.equals(DEBUG_LEVEL))
	    return _ranges[3];
	if (propertyName.equals(VIS_LEVEL))
	    return _ranges[4];
	if (propertyName.equals(FAC_LEVEL))
	    return _ranges[5];
	if (propertyName.equals(DEBUG_STREAM))
	    return _ranges[6];
	if (propertyName.equals(ERROR_STREAM))
	    return _ranges[7];
	if (propertyName.equals(STATIC_INVENTORY_IMPLE))
	    return _ranges[8];
	if (propertyName.equals(PROPERTY_MANAGER_IMPLEMENTATION))
	    return _ranges[9];
	if (propertyName.equals(VAR_DIR))
	    return _ranges[10];

	return EnvironmentManager.INVALID;
    }

    /*
     * Do not re-order!
     */

private String[] _ranges = { "filename:#notnull#", "onoff:#notnull#", "yesno:#notnull#", "int:0x0...0xffff", "int:0x0...0xffff", "long:0x0...0xffffffff", "string:#notnull#", "string:#notnull#", "expression_startswith:STATIC_INVENTORY_IMPLE", "string:#notnull#", "string:#notnull#" };
    
}
