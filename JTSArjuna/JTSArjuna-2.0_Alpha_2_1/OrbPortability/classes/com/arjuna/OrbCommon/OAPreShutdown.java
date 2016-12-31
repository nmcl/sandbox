/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OAPreShutdown.java,v 1.1 2000/02/25 14:09:15 nmcl Exp $
 */

/**
 * Classes derived from this interface can be registered with
 * the system and do any tidy-up necessary after the ORB has
 * been shutdown.
 */

package com.arjuna.OrbCommon;

public abstract class OAPreShutdown extends OAShutdown
{
    
public abstract void work ();

protected OAPreShutdown (String name)
    {
	super(name);
    };

};
