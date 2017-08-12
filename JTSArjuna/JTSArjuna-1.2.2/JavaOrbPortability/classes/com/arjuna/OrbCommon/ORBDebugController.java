/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBDebugController.java,v 1.1 1998/11/12 08:52:34 nmcl Exp $
 */

package com.arjuna.OrbCommon;

public class ORBDebugController
{

public static int getDebugLevel ()
    {
	return _debugLevel;
    }

public static void setDebugLevel (int level)
    {
	_debugLevel = level;
    }

public static void mergeDebugLevel (int level)
    {
	_debugLevel |= level;
    }

public static int getVisibilityLevel ()
    {
	return _visLevel;
    }

public static void setVisibilityLevel (int level)
    {
	_visLevel = level;
    }

public static void mergeVisibilityLevel (int level)
    {
	_visLevel |= level;
    }

public static boolean debugAllowed ()
    {
	return debugAllowed(DebugLevel.FULL_DEBUGGING, VisibilityLevel.VIS_ALL);
    }
    
public static boolean debugAllowed (int dLevel)
    {
	return debugAllowed(dLevel, VisibilityLevel.VIS_ALL);
    }
    
public static boolean debugAllowed (int dLevel, int vLevel)
    {
	if (((dLevel & _debugLevel) != 0) &&
	    ((vLevel & _visLevel) != 0))
	{
	    return true;
	}
	else
	    return false;
    }
    
private static int _debugLevel = DebugLevel.NO_DEBUGGING;
private static int _visLevel = VisibilityLevel.VIS_ALL;
    
};
