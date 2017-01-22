/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BOAInit.java,v 1.1 1998/11/12 08:52:33 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public abstract class BOAInit
{
    
public abstract void initialise (String[] params);

public boolean postBOAInit ()
    {
	return true;
    }
    
};
