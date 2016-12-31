/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBFilterManager.java,v 1.1.2.1 2000/05/02 08:41:14 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public class ORBFilterManager
{

public ORBFilterManager ()
    {
    }
	    
public static final int FT_PROCESS = 0;
public static final int FT_OBJECT = 1;
    
public static synchronized boolean addFilter (ORBFilter f, int ft)
    {
	if (((ft == FT_OBJECT) && (_theObjectFilter != null)) ||
	    ((ft == FT_PROCESS) && (_theProcessFilter != null)))
	{
	    System.err.println("WARNING - ORBFilterManager::addFilter called multiple times for "+
			       ((ft == FT_OBJECT) ? "object filters." : "process filters."));

	    return false;
	}

	if (ft == FT_OBJECT)
	    _theObjectFilter = f;
	else
	    _theProcessFilter = f;

	return true;
    }

public static boolean removeFilter (int ft)
    {
	if ((ft == FT_OBJECT) && (_theObjectFilter != null))
	{
	    _theObjectFilter = null;
	    return true;
	}
	else
	{
	    if ((ft == FT_PROCESS) && (_theProcessFilter != null))
	    {
		_theProcessFilter = null;
		return true;
	    }
	}

	return false;
    }

public static ORBFilter filter (int ft)
    {
	if (ft == FT_OBJECT)
	    return _theObjectFilter;
	else
	    return _theProcessFilter;
    }

private static ORBFilter _theObjectFilter = null;
private static ORBFilter _theProcessFilter = null;
    
};
