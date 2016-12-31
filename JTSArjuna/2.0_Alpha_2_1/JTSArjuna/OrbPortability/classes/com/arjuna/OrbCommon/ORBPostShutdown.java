/*
 * Copyright (C) 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBPostShutdown.java,v 1.1 2000/02/25 14:09:21 nmcl Exp $
 */

package com.arjuna.OrbCommon;

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
    };

};
