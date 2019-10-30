/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: EnvironmentManager.java,v 1.1.2.1.4.2 2001/01/11 14:02:06 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

/**
 * Each module which provides configuration options via an Environment
 * class must provide an implementation of this interface. The
 * implementation returns information about the valid range of values
 * which can be attributed to a specific attribute, and also about
 * whether a specific value is valid or not in the context of that
 * attribute.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: EnvironmentManager.java,v 1.1.2.1.4.2 2001/01/11 14:02:06 nmcl Exp $
 * @since JTS 2.1.
 */

public interface EnvironmentManager
{

public static final String ANY = "#ANY#";
public static final String INVALID = "#INVALID#";

/**
 * @return the value range for the specified property.
 */

public String getPropertyRange (String propertyName);
 
}
