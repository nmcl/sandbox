/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBFilterManager.java,v 1.1.2.1.6.3.22.1.4.1 2000/12/21 11:21:28 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import com.arjuna.ArjunaCommon.Common.ErrorStream;

/**
 * The filter manager is responsible for managing object and process-level
 * filters.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBFilterManager.java,v 1.1.2.1.6.3.22.1.4.1 2000/12/21 11:21:28 nmcl Exp $
 * @since JTS 1.0.
 */

public class ORBFilterManager
{
    
    /**
     * The types of filters we support. Current implementations only
     * allow one filter per type.
     */

public static final int FT_PROCESS = 0;
public static final int FT_OBJECT = 1;

    /**
     * Create a new filter manager.
     */

public ORBFilterManager ()
    {
    }
	    
    /**
     * Add the specified filter type.
     */

public static synchronized boolean addFilter (ORBFilter f, int ft)
    {
	if (((ft == FT_OBJECT) && (_theObjectFilter != null)) ||
	    ((ft == FT_PROCESS) && (_theProcessFilter != null)))
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("ORBFilterManager::addFilter called multiple times for "+
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
    
}
