/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBShutdown.java,v 1.1 2000/02/25 14:09:23 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public abstract class ORBShutdown
{
    
public abstract void work ();

protected ORBShutdown (String name)
    {
	_name = name;
    }

private String _name;

};
