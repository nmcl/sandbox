/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SynchronizationCreator.java,v 1.1 2001/07/23 13:44:57 nmcl Exp $
 */

package com.arjuna.Activity.SignalSets;

import org.omg.CosActivity.SignalSet;
import com.arjuna.Activity.SignalSets.SignalSetCreator;

public class SynchronizationCreator implements SignalSetCreator
{

public String name ()
    {
	return SystemDefined.Synchronization;
    }
    
public SignalSet create (java.lang.Object[] params)
    {
	return new Synchronization(params).getReference();
    }
 
};
