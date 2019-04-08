/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SignalSetCreator.java,v 1.1 2001/07/23 13:44:56 nmcl Exp $
 */

package com.arjuna.Activity.SignalSets;

import org.omg.CosActivity.SignalSet;
import org.omg.CORBA.*;

public interface SignalSetCreator
{

public String name ();
public SignalSet create (java.lang.Object[] params);
    
};
