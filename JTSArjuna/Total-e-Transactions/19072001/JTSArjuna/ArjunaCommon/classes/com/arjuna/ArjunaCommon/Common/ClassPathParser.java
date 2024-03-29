/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ClassPathParser.java,v 1.1.2.3.8.1 2001/03/07 15:06:32 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.util.Properties;
import java.io.*;

import java.lang.StringIndexOutOfBoundsException;

/**
 * This class parses the CLASSPATH and returns the directories used
 * within it.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ClassPathParser.java,v 1.1.2.3.8.1 2001/03/07 15:06:32 nmcl Exp $
 * @since JTS 2.1.
 */

public class ClassPathParser
{

public ClassPathParser ()
    {
	_classPath = System.getProperty("java.class.path");
	_start = 0;
    }

    /**
     * @return the directory path of the next entry in the CLASSPATH that
     * contains the specified string.
     */

public final String getPath (String contains)
    {
	String toReturn = null;

	if ((_classPath != null) && (contains != null))
	{
	    int indx = _classPath.indexOf(contains, _start);
	    
	    if (indx != -1)
	    {
		int lastIndex = _classPath.indexOf(pathSeparator, _start);
		int sepIndex = lastIndex+1;

		if (lastIndex > indx)  // at start of path?
		    sepIndex = 0;
		else
		{
		    while ((lastIndex < indx) && (lastIndex != -1))
		    {
			lastIndex = _classPath.indexOf(pathSeparator, sepIndex);

			if (lastIndex == -1)
			    lastIndex = _classPath.length();
			else
			{
			    if (lastIndex < indx)
				sepIndex = lastIndex+1;
			}
		    }
		}

		try
		{
		    toReturn = _classPath.substring(sepIndex, lastIndex);

		    _start = indx+1;
		}
		catch (StringIndexOutOfBoundsException e)
		{
		    // nothing left!!
		}
	    }
	}

	return toReturn;
    }
    
    /**
     * Reload the classpath and reset the class ready to re-parse.
     *
     * @return <code>true</code> if a non-null CLASSPATH was loaded,
     * <code>false</code> otherwise.
     */

public final boolean reset ()
    {
	_classPath = System.getProperty("java.class.path");

	_start = 0;
	
	return (boolean) (_classPath != null);
    }
    
private String _classPath;
private int    _start;

private static final char winSeparator = ';';
private static final char unixSeparator = ':';

private static char pathSeparator = unixSeparator;

    static
    {
	pathSeparator = System.getProperty("path.separator").charAt(0);
    }
    
}
