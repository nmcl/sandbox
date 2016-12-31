/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBAttribute.java,v 1.1 2000/02/25 14:09:16 nmcl Exp $
 */

/**
 * Classes to be called before or after the ORB is initialised.
 * Override the postORBInit method to determine where in the
 * initialisation the class should be called.
 */

package com.arjuna.OrbCommon;

public abstract class ORBAttribute
{
    
public abstract void initialise (String[] params);

public boolean postORBInit ()
    {
	return true;
    }
 
};
