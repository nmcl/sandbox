/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OAAttribute.java,v 1.1 2000/02/25 14:09:14 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Classes to be called before or after the OA is initialised.
 * Override the postOAInit method to determine where in the
 * initialisation the class should be called.
 */

public abstract class OAAttribute
{
    
public abstract void initialise (String[] params);

public boolean postOAInit ()
    {
	return true;
    }
    
};
