/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OAPostShutdown.java,v 1.1 2000/02/25 14:09:14 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public abstract class OAPostShutdown extends OAShutdown
{
    
public abstract void work ();

protected OAPostShutdown (String name)
    {
	super(name);
    };

};
