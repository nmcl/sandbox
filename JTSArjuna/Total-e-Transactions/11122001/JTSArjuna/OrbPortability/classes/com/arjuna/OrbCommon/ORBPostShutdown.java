/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBPostShutdown.java,v 1.1.28.1.4.1 2000/12/21 11:21:29 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Objects which should be invoked after the ORB has shutdown should
 * be derived from this class.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBPostShutdown.java,v 1.1.28.1.4.1 2000/12/21 11:21:29 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class ORBPostShutdown extends ORBShutdown
{
    
public abstract void work ();

protected ORBPostShutdown ()
    {
	super("ORBPostShutdown");
    }
    
protected ORBPostShutdown (String name)
    {
	super(name);
    }

}

