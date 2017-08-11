/*
 * Copyright (C) 1998, 1999,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 * 
 * $Id: licence.java,v 1.3.2.2 1999/02/17 15:19:26 nmcl Exp $
 */

import java.io.*;
import java.net.InetAddress;
import java.util.Date;

import java.lang.NumberFormatException;
import java.net.UnknownHostException;

public class licence
{

public static void usage (String name)
    {
	System.out.println("Usage: "+name+" -host <host name> -netmask <mask> -date <period> [-check] [-help]");
    }
    
public static void main (String[] args)
    {
	String hostName = null;
	int netmask = 0;
	int date = 0;
	int check = 0;
	boolean noTimeout = false;
	boolean checkTime = false;
    
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-check") == 0)
		checkTime = true;
	    if (args[i].compareTo("-host") == 0)
		hostName = args[i+1];
	    if (args[i].compareTo("-netmask") == 0)
	    {
		try
		{
		    Long m = Long.valueOf(args[i+1], 16);
		    netmask = m.intValue();
		}
		catch (NumberFormatException e)
		{
		    System.err.println("Error - netmask format is illegal: "+args[i+1]);
		    System.exit(1);
		}
	    }
	    if (args[i].compareTo("-date") == 0)
	    {
		try
		{
		    if (args[i+1].compareTo("-1") == 0)
			noTimeout = true;
		    else
		    {
			Integer d = new Integer(args[i+1]);
			date = d.intValue();
		    }
		}
		catch (NumberFormatException e)
		{
		    System.err.println("Error - date format is illegal: "+args[i+1]);
		    System.exit(1);
		}
	    }
	    if (args[i].compareTo("-help") == 0)
	    {
		usage("licence");
		System.exit(0);
	    }
	}
    
	if ((hostName == null) || (netmask == 0) || ((date == 0) && (!noTimeout)))
	{
	    System.err.println("Parameter error.");
	    usage("licence");
	    
	    System.exit(1);
	}

	int localHost = 0;
	InetAddress addr = null;

	try
	{
	    addr = InetAddress.getByName(hostName);
	}
	catch (UnknownHostException e)
	{
	    System.err.println("Error - cannot determine host inet information.");
	    System.exit(1);
	}
	
	byte[] baddr = addr.getAddress();
	
	for (int i = 0; i < baddr.length; i++)
	{
	    /*
	     * Convert signed byte into unsigned.
	     */
		
	    int l = 0x7f & baddr[i];

	    l += (0x80 & baddr[i]);
		    
	    localHost = (localHost << 8) | l;
	}

	localHost = localHost ^ 0xa4cde2b7;

	int localNetwork = localHost & netmask;
	int localDate   = (int) (System.currentTimeMillis()/1000+(long)date*24L*60L*60L);

	if (noTimeout)
	    localDate = 0xffffffff;
	
	if (checkTime)
	{
	    try
	    {
		long l = (long)localDate*1000L;
		Date d = new Date(l);

		System.out.println("licence will timeout on "+d);
	    }
	    catch (Exception e)
	    {
	    }
	}

	localDate = localDate ^ 0x48bafc2e;
	netmask = netmask ^ 0xe24c57ba;

	int calc = 0xa6a6a6a6;
	int a = localDate;
	int b = localNetwork;
	int c = netmask;

	for (int j = 0; j < 17; j++)
	{
	    a = (37 * (a ^ calc)) + 41;
	    b = (27 * (b ^ calc)) + 31;
	    c = (43 * (c ^ calc)) + 29;
	    calc = a ^ b ^ c;
	}

	System.out.println("\nlicence: "+Integer.toHexString(localHost)+"-"+Integer.toHexString(netmask)
			   +"-"+Integer.toHexString(localDate)+"-"+Integer.toHexString(calc));
    }

};