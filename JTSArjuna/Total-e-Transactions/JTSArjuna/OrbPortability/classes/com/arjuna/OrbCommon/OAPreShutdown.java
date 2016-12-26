/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OAPreShutdown.java,v 1.1.28.1.4.1 2000/12/21 11:21:27 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Instances of classes derived from this interface can be registered with
 * the system and do any tidy-up necessary after the ORB has
 * been shutdown.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OAPreShutdown.java,v 1.1.28.1.4.1 2000/12/21 11:21:27 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class OAPreShutdown extends OAShutdown
{
    
public abstract void work ();

protected OAPreShutdown (String name)
    {
	super(name);
    };

};
