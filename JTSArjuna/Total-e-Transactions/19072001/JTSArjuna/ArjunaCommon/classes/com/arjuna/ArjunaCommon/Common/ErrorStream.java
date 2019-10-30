/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ErrorStream.java,v 1.1.2.3.2.1.6.2.20.1.2.1.2.3 2001/01/11 14:02:06 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.io.PrintWriter;
import java.io.OutputStream;
import java.util.Date;
import java.util.Calendar;

/**
 * The error stream class. An instance of this class is used to
 * control the output of error and warning messages.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ErrorStream.java,v 1.1.2.3.2.1.6.2.20.1.2.1.2.3 2001/01/11 14:02:06 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class ErrorStream
{

    /**
     * The types of error/warning message available.
     */

public static final int WARNING = 0;
public static final int FATAL = 1;

    /**
     * @return the PrintWriter used to output error messages.
     * The default is to send to standard error.
     */

public static PrintWriter stream ()
    {
	return _theStream;
    }

    /**
     * Print a warning string (WARNING).
     */

public static PrintWriter warning ()
    {
	return stream(ErrorStream.WARNING);
    }

    /**
     * Print a fatal string (FATAL).
     */

public static PrintWriter fatal ()
    {
	return stream(ErrorStream.FATAL);
    }

    /**
     * Print a string for the specified severity.
     */

public static PrintWriter stream (int severity)
    {
	return ErrorStream.stream("", severity);
    }

public static synchronized void print (String s)
    {
	_theStream.print(s);
    }
    
public static synchronized void println(String s)
    {
	_theStream.println(s);
    }
    
    /**
     * Print a string for the specified severity, followed by the provided
     * string. The time will also be output.
     */

public static synchronized PrintWriter stream (String s, int severity)
    {
	if (severity == 0)
	    _theStream.print(s+"WARNING ");
	else
	    _theStream.print(s+"FATAL ");
	
	_theStream.print(Calendar.getInstance().getTime()+": ");
	
	_theStream.flush();

	return _theStream;
    }
    
    /*
     * Don't want users creating instances directly.
     */

private ErrorStream ()
    {
    }

private static PrintWriter _theStream = null;
    
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
	    
}

