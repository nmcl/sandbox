/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBInit.java,v 1.4 1998/11/12 08:52:34 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public abstract class ORBInit
{
    
public abstract void initialise (String[] params);

public boolean postORBInit ()
    {
	return true;
    }
 
};
