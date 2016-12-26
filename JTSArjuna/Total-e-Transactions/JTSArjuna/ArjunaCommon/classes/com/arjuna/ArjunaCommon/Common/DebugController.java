/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DebugController.java,v 1.2.4.1.6.3.2.3.20.1.2.2.2.1.2.1.56.1.4.2.14.1 2002/03/13 15:49:17 rbegg Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.io.PrintWriter;
import java.io.OutputStream;

/**
 * This class controls the debugging that can be output by the
 * system.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DebugController.java,v 1.2.4.1.6.3.2.3.20.1.2.2.2.1.2.1.56.1.4.2.14.1 2002/03/13 15:49:17 rbegg Exp $
 * @since JTS 1.2.4.
 */

public class DebugController
{

    /**
     * Return the controller to use.
     */

public static DebugController controller ()
    {
	initialise();

	return _theController;
    }

    /**
     * Set the debug level, the visibility level, and the
     * facility code.
     */

public synchronized void setLevels (long dl, long vl, long fl)
    {
	_debugLevel = dl;
	_visLevel = vl;
	_facLevel = fl;
    }

    /**
     * Return the debug level.
     */

public synchronized long getDebugLevel ()
    {
	return _debugLevel;
    }

    /**
     * Set the debug level.
     */

public synchronized void setDebugLevel (long level)
    {
	_debugLevel = level;
    }

    /**
     * Merge the debug level provided with that currently used by
     * the controller.
     */

public synchronized void mergeDebugLevel (long level)
    {
	_debugLevel |= level;
    }

    /**
     * Return the visibility level.
     */

public synchronized long getVisibilityLevel ()
    {
	return _visLevel;
    }

    /**
     * Set the visibility level.
     */

public synchronized void setVisibilityLevel (long level)
    {
	_visLevel = level;
    }

    /**
     * Merge the visibility level provided with that currently used by
     * the controller.
     */

public synchronized void mergeVisibilityLevel (long level)
    {
	_visLevel |= level;
    }

    /**
     * Return the facility code.
     */

public synchronized long getFacilityCode ()
    {
	return _facLevel;
    }

    /**
     * Set the facility code.
     */

public synchronized void setFacilityCode (long level)
    {
	_facLevel = level;
    }

    /**
     * Merge the debug level provided with that currently used by
     * the controller.
     */
    
public synchronized void mergeFacilityCode (long level)
    {
	_facLevel |= level;
    }

    /**
     * Is debugging enabled?
     *
     * Not synchronized, so beware!
     */

public static boolean enabled ()
    {
    	initialise();
    	
	return _enabled;
    }

public static boolean enable (boolean val)
    {
	boolean oldVal = _enabled;
	
	_enabled = val;

	return oldVal;
    }
    
    /**
     * Is it allowed to print debugging statements?
     *
     * This method assumes <pre>DebugLevel.FULL_DEBUGGING,
     * VisibilityLevel.VIS_ALL, FacilityCode.FAC_ALL)</pre>
     */

public synchronized boolean debugAllowed ()
    {
	return debugAllowed(DebugLevel.FULL_DEBUGGING, VisibilityLevel.VIS_ALL, FacilityCode.FAC_ALL);
    }

    /**
     * Is it allowed to print debugging statements?
     *
     * This method assumes <pre>VisibilityLevel.VIS_ALL,
     * FacilityCode.FAC_ALL)</pre>
     */

public synchronized boolean debugAllowed (long dLevel)
    {
	return debugAllowed(dLevel, VisibilityLevel.VIS_ALL, FacilityCode.FAC_ALL);
    }

    /**
     * Is it allowed to print debugging statements?
     *
     * This method assumes <pre> FacilityCode.FAC_ALL)</pre>
     */
    
public synchronized boolean debugAllowed (long dLevel, long vLevel)
    {
	return debugAllowed(dLevel, vLevel, FacilityCode.FAC_ALL);
    }

    /*
     * Is it allowed to print debugging statements?
     */

public synchronized boolean debugAllowed (long dLevel, long vLevel, long fLevel)
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

    /**
     * @return the PrintWriter used to output debugging statements.
     * The default is to send to standard output.
     *
     * Can only be set at initialisation time.
     */

public PrintWriter getStream ()
    {
	return _outputStream;
    }

    /**
     * @return the PrintWriter used to output debugging statements
     * if the correct debugging level is set. Otherwise return null.
     *
     * @since JTS 2.1.
     */

public PrintWriter getStream (long dl, long vl, long fl)
    {
	if (debugAllowed(dl, vl, fl))
	    return _outputStream;
	else
	    return null;
    }
    
    /**
     * Print the provided string out on the debug stream only if
     * the specified debug level, visibility level, and facility code
     * match those allowed for debugging output.
     */

public synchronized void print (long dl, long vl, long fl, String s)
    {
	if (debugAllowed(dl, vl, fl))
	{
	    _outputStream.print(s);
	    _outputStream.flush();
	}
    }

    /**
     * Print the specified string if debugging defaults are enabled.
     */

public synchronized void print (String s)
    {
	if (debugAllowed())
	{
	    _outputStream.print(s);
	    _outputStream.flush();
	}
    }

    /**
     * Print the provided string out on the debug stream only if
     * the specified debug level, visibility level, and facility code
     * match those allowed for debugging output.
     */

public synchronized void println (long dl, long vl, long fl, String s)
    {
	if (debugAllowed(dl, vl, fl))
	    _outputStream.println(s);
    }

    /**
     * Print the specified string if debugging defaults are enabled.
     */

public synchronized void println (String s)
    {
	if (debugAllowed())
	    _outputStream.println(s);
    }

    /**
     * Print a carriage-return/line-feed to the debug stream.
     */

public synchronized void println ()
    {
	if (debugAllowed())
	    _outputStream.println();
    }

    /**
     * We don't want users creating instances directly.
     */

protected DebugController ()
    {
    }

private synchronized static final void initialise ()
    {
	if (_theController == null)
	{
	    _theController = new DebugController();

	    String enabled = PropertyManager.getProperty(Environment.DEBUG_ENABLED);

	    if (enabled != null)
	    {
		if (enabled.compareTo("YES") == 0)
		    _enabled = true;
	    }
	
	    String dLevel = PropertyManager.getProperty(Environment.DEBUG_LEVEL);
	    
	    if (dLevel != null)
	    {
		try
		{
		    _debugLevel = Utility.hexStringToInt(dLevel);
		}
		catch (Exception e)
		{
		    _debugLevel = DebugLevel.getLevel(dLevel);
		}
	    }

	    String vLevel = PropertyManager.getProperty(Environment.VIS_LEVEL);

	    if (vLevel != null)
	    {
		try
		{
		    _visLevel = Utility.hexStringToInt(vLevel);
		}
		catch (Exception e)
		{
		    _visLevel = VisibilityLevel.getLevel(vLevel);
		}
	    }

	    String fLevel = PropertyManager.getProperty(Environment.FAC_LEVEL);

	    if (fLevel != null)
	    {
		try
		{
		    _facLevel = Utility.hexStringToInt(fLevel);
		}
		catch (Exception e)
		{
		    _facLevel = FacilityCode.getLevel(fLevel);
		}
	    }

	    String stream = PropertyManager.getProperty(Environment.DEBUG_STREAM);
	
	    if (stream != null)
	    {
		try
		{
		    Class c = Class.forName(stream);
		
		    _outputStream = new PrintWriter((java.io.Writer) c.newInstance(), true);
		}
		catch (Exception e)
		{
		    ErrorStream.warning().println("DebugController init caught: "+e);
		    
		    _outputStream = new PrintWriter(System.out, true);
		}
	    }
	    else
		_outputStream = new PrintWriter(System.out, true);
	}
    }

private static DebugController _theController = null;
private static long            _debugLevel = DebugLevel.NO_DEBUGGING;
private static long            _visLevel = VisibilityLevel.VIS_PUBLIC;
private static long            _facLevel = FacilityCode.FAC_ALL;
private static PrintWriter     _outputStream = null;
private static boolean         _enabled = false;
	    
};
