/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SystemDefined.java,v 1.1 2001/07/23 13:44:57 nmcl Exp $
 */

package com.arjuna.Activity.SignalSets;

/*
 * The system signal sets.
 */

public class SystemDefined
{

public static final String Synchronization = "org.omg.Synchronization";
public static final String ChildLifetime = "org.omg.ChildLifetime";

public static final boolean member (String signalSetName)
    {
	if ((signalSetName.equals(SystemDefined.Synchronization)) ||
	    (signalSetName.equals(SystemDefined.ChildLifetime)))
	{
	    return true;
	}
	else
	    return false;
    }
    
};
