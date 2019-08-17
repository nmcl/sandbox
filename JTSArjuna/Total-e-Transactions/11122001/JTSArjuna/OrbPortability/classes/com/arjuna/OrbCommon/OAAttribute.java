/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OAAttribute.java,v 1.1.32.1 2000/12/21 11:21:26 nmcl Exp $
 */

package com.arjuna.OrbCommon;

/**
 * Classes to be called before or after the OA is initialised.
 * Override the postOAInit method to determine where in the
 * initialisation the class should be called.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OAAttribute.java,v 1.1.32.1 2000/12/21 11:21:26 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class OAAttribute
{
    
public abstract void initialise (String[] params);

public boolean postOAInit ()
    {
	return true;
    }
    
};
