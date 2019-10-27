/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBPreShutdown.java,v 1.1.28.1.4.1 2000/12/21 11:21:29 nmcl Exp $
 */

/**
 * Insances of classes derived from this interface can be registered with
 * the system and do any tidy-up necessary after the ORB has
 * been shutdown.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBPreShutdown.java,v 1.1.28.1.4.1 2000/12/21 11:21:29 nmcl Exp $
 * @since JTS 1.0.
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
