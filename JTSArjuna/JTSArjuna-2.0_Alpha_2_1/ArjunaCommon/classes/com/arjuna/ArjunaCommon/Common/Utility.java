/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Utility.java,v 1.1 2000/02/25 14:02:41 nmcl Exp $
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

/*
 * Default visibility.
 */

public class Utility
{

    /*
     * Convert integer to hex String.
     */
    
public static String intToHexString (int number) throws NumberFormatException
    {
	return Integer.toString(number, 16);
    }

public static int hexStringToInt (String s) throws NumberFormatException
    {
	Integer i = Integer.valueOf(s, 16);

	return i.intValue();
    }

public static String longToHexString (long number) throws NumberFormatException
    {
	return Long.toString(number, 16);
    }

public static long hexStringToLong (String s) throws NumberFormatException
    {
	Long i = Long.valueOf(s, 16);

	return i.longValue();
    }    

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

private static int myAddr = 0;
    
}
