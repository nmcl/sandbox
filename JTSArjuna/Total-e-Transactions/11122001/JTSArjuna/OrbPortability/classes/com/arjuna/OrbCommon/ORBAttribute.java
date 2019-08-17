/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBAttribute.java,v 1.1.28.1.4.1 2000/12/21 11:21:27 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Classes to be called before or after the ORB is initialised.
 * Override the postORBInit method to determine where in the
 * initialisation the class should be called.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBAttribute.java,v 1.1.28.1.4.1 2000/12/21 11:21:27 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class ORBAttribute
{
    
public abstract void initialise (String[] params);

public boolean postORBInit ()
    {
	return true;
    }
 
};
