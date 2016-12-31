/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBPreShutdown.java,v 1.1 2000/02/25 14:09:22 nmcl Exp $
 */

/**
 * Classes derived from this interface can be registered with
 * the system and do any tidy-up necessary after the ORB has
 * been shutdown.
 */

package com.arjuna.OrbCommon;

public abstract class ORBPreShutdown extends ORBShutdown
{
    
public abstract void work ();

protected ORBPreShutdown ()
    {
	super("ORBPreShutdown");
    }
    
protected ORBPreShutdown (String name)
    {
	super(name);
    };

};
