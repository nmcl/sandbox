/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ChildLifetimeCreator.java,v 1.1 2001/07/23 13:44:56 nmcl Exp $
 */

package com.arjuna.Activity.SignalSets;

import org.omg.CosActivity.SignalSet;
import com.arjuna.Activity.SignalSets.SignalSetCreator;

public class ChildLifetimeCreator implements SignalSetCreator
{

public String name ()
    {
	return SystemDefined.ChildLifetime;
    }
    
public SignalSet create (java.lang.Object[] params)
    {
	return new ChildLifetime(params).getReference();
    }
 
};
