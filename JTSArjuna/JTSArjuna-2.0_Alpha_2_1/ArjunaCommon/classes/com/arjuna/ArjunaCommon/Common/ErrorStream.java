/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ErrorStream.java,v 1.1.2.3.2.1 2000/07/18 13:16:41 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.io.PrintWriter;
import java.io.OutputStream;
import java.util.Date;
import java.util.Calendar;

public class ErrorStream
{

public static final int WARNING = 0;
public static final int FATAL = 1;

public static PrintWriter stream ()
    {
	return _theStream;
    }
    
public static PrintWriter stream (int severity)
    {
	return ErrorStream.stream("", severity);
    }
    
public static PrintWriter stream (String s, int severity)
    {
	if (severity == 0)
	    _theStream.print(s+"WARNING ");
	else
	    _theStream.print(s+"FATAL ");
	
	_theStream.print(_theCal.getTime()+": ");
	
	return _theStream;
    }
    
    /*
     * Don't want users creating instances directly.
     */

private ErrorStream ()
    {
    }

private static PrintWriter _theStream = null;
private static Calendar    _theCal = Calendar.getInstance();
    
    static
    {
	String stream = PropertyManager.getProperty(Environment.ERROR_STREAM);
	
	if (stream != null)
	{
	    if (stream.equals("DISABLE"))
		System.err.println("WARNING - error stream has been disabled!");
	    else
	    {
		try
		{
		    Class c = Class.forName(stream);
		
		    _theStream = new PrintWriter((java.io.Writer) c.newInstance(), true);
		}
		catch (Exception e)
		{
		    System.err.println("ErrorStream init caught: "+e);
		
		    _theStream = new PrintWriter(System.err, true);
		}
	    }
	}
	else
	    _theStream = new PrintWriter(System.err, true);
    }
	    
};
