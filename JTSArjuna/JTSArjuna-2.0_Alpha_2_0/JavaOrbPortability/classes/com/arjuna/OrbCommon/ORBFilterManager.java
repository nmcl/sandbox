/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBFilterManager.java,v 1.2.8.1 1999/09/10 13:21:45 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public class ORBFilterManager
{

public ORBFilterManager ()
    {
    }
    
public static boolean addFilter (ORBFilter f)
    {
	if (_theFilter == null)
	{
	    _theFilter = f;
	    return true;
	}
	else
	{
	    System.err.println("WARNING ORBFilterManager.addFilter called multiple times.");
	
	    return false;
	}
    }

public static boolean removeFilter ()
    {
	if (_theFilter != null)
	{
	    _theFilter = null;
	    return true;
	}
	else
	    return false;
    }

public static ORBFilter filter ()
    {
	return _theFilter;
    }

private static ORBFilter _theFilter = null;
    
};
