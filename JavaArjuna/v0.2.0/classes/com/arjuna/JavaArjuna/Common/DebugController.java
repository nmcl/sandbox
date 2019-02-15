/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DebugController.java,v 1.2.4.1 2000/04/25 08:35:54 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Common;

import com.arjuna.JavaArjuna.Environment;

import java.io.PrintWriter;
import java.io.OutputStream;

public class DebugController
{

public static DebugController controller ()
    {
	return _theController;
    }
    
public synchronized void setLevels (int dl, int vl, int fl)
    {
	_debugLevel = dl;
	_visLevel = vl;
	_facLevel = fl;
    }
    
public synchronized int getDebugLevel ()
    {
	return _debugLevel;
    }

public synchronized void setDebugLevel (int level)
    {
	_debugLevel = level;
    }

public synchronized void mergeDebugLevel (int level)
    {
	_debugLevel |= level;
    }

public synchronized int getVisibilityLevel ()
    {
	return _visLevel;
    }

public synchronized void setVisibilityLevel (int level)
    {
	_visLevel = level;
    }

public synchronized void mergeVisibilityLevel (int level)
    {
	_visLevel |= level;
    }

public synchronized int getFacilityCode ()
    {
	return _facLevel;
    }
    
public synchronized void setFacilityCode (int level)
    {
	_facLevel = level;
    }
    
public synchronized void mergeFacilityCode (int level)
    {
	_facLevel |= level;
    }

    /*
     * Can only be set once at initialisation time.
     */

public static boolean enabled ()
    {
	return _enabled;
    }
    
public synchronized boolean debugAllowed ()
    {
	return debugAllowed(DebugLevel.FULL_DEBUGGING, VisibilityLevel.VIS_ALL, FacilityCode.FAC_ALL);
    }

public synchronized boolean debugAllowed (int dLevel)
    {
	return debugAllowed(dLevel, VisibilityLevel.VIS_ALL, FacilityCode.FAC_ALL);
    }
    
public synchronized boolean debugAllowed (int dLevel, int vLevel)
    {
	return debugAllowed(dLevel, vLevel, FacilityCode.FAC_ALL);
    }

public synchronized boolean debugAllowed (int dLevel, int vLevel, int fLevel)
    {
	if (!_enabled)
	    return false;
	
	if (((dLevel & _debugLevel) != 0) &&
	    ((vLevel & _visLevel) != 0) &&
	    ((fLevel & _facLevel) != 0))
	{
	    return true;
	}
	else
	    return false;
    }

    /*
     * Can only be set at initialisation time.
     */

public PrintWriter getStream ()
    {
	return _outputStream;
    }
    
public synchronized void print (int dl, int vl, int fl, String s)
    {
	if (debugAllowed(dl, vl, fl))
	    _outputStream.print(s);
    }
    
public void print (String s)
    {
	if (debugAllowed())
	    _outputStream.print(s);
    }

public synchronized void println (int dl, int vl, int fl, String s)
    {
	if (debugAllowed(dl, vl, fl))
	    _outputStream.println(s);
    }

public void println (String s)
    {
	if (debugAllowed())
	    _outputStream.println(s);
    }

    /*
     * Don't want users creating instances directly.
     */

protected DebugController ()
    {
    }

private static DebugController _theController;
private static int _debugLevel;
private static int _visLevel;
private static int _facLevel;
private static PrintWriter _outputStream;
private static boolean _enabled = false;

    static
    {
	_theController = new DebugController();

	String enabled = System.getProperty(Environment.DEBUG_ENABLED);
	
	if (enabled != null)
	{
	    if (enabled.compareTo("YES") == 0)
		_enabled = true;
	}
	
	String dLevel = System.getProperty(Environment.DEBUG_LEVEL);

	if (dLevel != null)
	{
	    try
	    {
		_debugLevel = (int) Utility.hexStringToLong(dLevel);
	    }
	    catch (Exception e)
	    {
		_debugLevel = DebugLevel.NO_DEBUGGING;
	    }
	}
	else
	    _debugLevel = DebugLevel.NO_DEBUGGING;

	String vLevel = System.getProperty(Environment.VIS_LEVEL);
	
	if (vLevel != null)
	{
	    try
	    {
		_visLevel = (int) Utility.hexStringToLong(dLevel);
	    }
	    catch (Exception e)
	    {
		_visLevel =  VisibilityLevel.VIS_PUBLIC;
	    }
	}
	else
	    _visLevel = VisibilityLevel.VIS_PUBLIC;

	String fLevel = System.getProperty(Environment.FAC_LEVEL);
	
	if (fLevel != null)
	{
	    try
	    {
		_facLevel = (int) Utility.hexStringToLong(dLevel);
	    }
	    catch (Exception e)
	    {
		_facLevel = FacilityCode.FAC_ALL;
	    }
	}
	else
	    _facLevel = FacilityCode.FAC_ALL;

	String stream = System.getProperty(Environment.DEBUG_STREAM);
	
	if (stream != null)
	{
	    try
	    {
		Class c = Class.forName(stream);
		
		_outputStream = new PrintWriter((java.io.Writer) c.newInstance());
	    }
	    catch (Exception e)
	    {
		_outputStream = new PrintWriter(System.out);
	    }
	}
	else
	    _outputStream = new PrintWriter(System.out);
    }
	    
};
