/*
 * Copyright (C) 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OAShutdown.java,v 1.1 2000/02/25 14:09:15 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public abstract class OAShutdown
{
    
public abstract void work ();

protected OAShutdown (String name)
    {
	_name = name;
    }

private String _name;

};
