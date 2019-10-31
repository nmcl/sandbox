/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Utility.java,v 1.1.10.1.20.1.2.1.2.1.4.3.32.1.4.3 2001/07/19 13:42:00 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.util.Properties;
import java.io.*;
import java.net.InetAddress;

import java.net.UnknownHostException;
import java.lang.NumberFormatException;
import java.lang.StringIndexOutOfBoundsException;
import java.io.IOException;
import java.io.FileNotFoundException;

/**
 * Various useful functions that we wrap in a single class.
 * Some of these functions are needed simply for backwards
 * compatibility with older versions of Java.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Utility.java,v 1.1.10.1.20.1.2.1.2.1.4.3.32.1.4.3 2001/07/19 13:42:00 nmcl Exp $
 * @since JTS 1.0.
 */

public class Utility
{

    /**
     * Convert integer to hex String.
     */
    
public static String intToHexString (int number) throws NumberFormatException
    {
	return Integer.toString(number, 16);
    }

    /**
     * Convert a hex String to an integer.
     */

public static int hexStringToInt (String s) throws NumberFormatException
    {
	String toUse = s;
	
	if (s.startsWith(Utility.hexStart))
	    toUse = s.substring(Utility.hexStart.length());
	
	Integer i = Integer.valueOf(toUse, 16);

	return i.intValue();
    }

    /**
     * Convert a long to a hex String.
     */

public static String longToHexString (long number) throws NumberFormatException
    {
	return Long.toString(number, 16);
    }

    /**
     * Convert a hex String to a long.
     */

public static long hexStringToLong (String s) throws NumberFormatException
    {
	String toUse = s;
	
	if (s.startsWith(Utility.hexStart))
	    toUse = s.substring(Utility.hexStart.length());

	Long i = Long.valueOf(toUse, 16);

	return i.longValue();
    }    

    /**
     * @return an integer representing the ip address of the local
     * machine. Essentially the bytes of the InetAddress are shuffled
     * into the integer.
     *
     * This was once part of the Uid class but has been separated for
     * general availability.
     *
     * @since JTS 2.1.
     */

public static synchronized int hostInetAddr () throws UnknownHostException
    {
	/*
	 * Calculate only once.
	 */

	if (myAddr == 0)
	{
	    InetAddress addr = InetAddress.getLocalHost();
	    byte[] b = addr.getAddress();
	
	    for (int i = 0; i < b.length; i++)
	    {
		/*
		 * Convert signed byte into unsigned.
		 */
		
		int l = 0x7f & b[i];

		l += (0x80 & b[i]);
		    
		myAddr = (myAddr << 8) | l;
	    }
	}

	return myAddr;
    }

    /**
     * @return the process id. This had better be unique between processes
     * on the same machine. If not we're in trouble!
     *
     * @since JTS 2.1.
     */
    
public static final int getpid ()
    {
	if (Utility.processId == 0)
	{
	    /*
	     * All of this is just to ensure uniqueness!
	     */

	    synchronized (Utility.hexStart)
		{
		    int retry = 1000;
		    int pid = (int) System.currentTimeMillis();
		    String dir = PropertyManager.getProperty(Environment.VAR_DIR);

		    pid = Math.abs(pid);
		    
		    if (dir == null)
		    {
			ClassPathParser cp = new ClassPathParser();

			dir = cp.getPath(File.separator + "var" + File.separator + "tmp");

			if (dir == null)
			{
			    if (Utility.isWindows())  // cygwin issue
				dir = cp.getPath("/var/tmp");
			}

			if (dir == null)
			    throw new FatalError("Utility.getpid - could not locate temporary directory. Ensure it is on your CLASSPATH.");
		    }
		    else
			dir = dir + File.separator + "tmp";

		    for (int i = 0; i < retry; i++)
		    {
			try
			{
			    File f = File.createTempFile("pid", new String(""+pid), new File(dir));

			    f.deleteOnExit();  // problem if we crash?

			    processId = pid;
		    
			    break;
			}
			catch (IOException e)
			{
			}
		    }

		    if (processId == 0)
			throw new FatalError("Utility.getpid could not create unique file.");
		}
	}

	return processId;
    }

    /**
     * @return a Uid representing this process.
     *
     * @since JTS 2.1.
     */

public static final synchronized Uid getProcessUid ()
    {
	if (processUid == null)
	    processUid = new Uid();

	return processUid;
    }

public static final boolean isWindows ()
    {
	String os = PropertyManager.getProperty("os.name");
	
	if ((os == "WIN32") || (os.indexOf("Windows") != -1))
	    return true;
	else
	    return false;
    }

private static int myAddr = 0;
private static int processId = 0;
private static Uid processUid = null;
    
private static final String hexStart = "0x";
    
}
