/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OAPostShutdown.java,v 1.1.28.1.4.1 2000/12/21 11:21:27 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Objects to be called after the OA has shutdown should implement
 * this class.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OAPostShutdown.java,v 1.1.28.1.4.1 2000/12/21 11:21:27 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class OAPostShutdown extends OAShutdown
{
    
public abstract void work ();

protected OAPostShutdown (String name)
    {
	super(name);
    };

};
