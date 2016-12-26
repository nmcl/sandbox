/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBShutdown.java,v 1.1.8.1.22.1.4.1 2000/12/21 11:21:29 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Some common methods for the ORB shutdown classes.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBShutdown.java,v 1.1.8.1.22.1.4.1 2000/12/21 11:21:29 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class ORBShutdown
{
    
public abstract void work ();

public final String name ()
    {
	return _name;
    }
    
protected ORBShutdown (String theName)
    {
	_name = theName;
    }

private String _name;

}

