/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 * 
 * $Id: licence.java,v 1.3.10.1.6.3.14.1.2.1.2.1.12.2.4.3 2001/07/20 14:38:31 nmcl Exp $
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
	System.out.println("Usage: "+name+" -key <id key> -date <period> [-netmask <mask>] [-check] [-enable <all|commit_one_phase|transactional_objects|restricted_two_phase|restricted_one_phase [-number <100|1000|10000|100000>]>] [-help]");
    }
    
public static void main (String[] args)
    {
	String hostName = null;
	int netmask = 0;
	int date = 0;
	int check = 0;
	int number = 100;  // default number of restricted transactions
	boolean noTimeout = false;
	boolean checkValues = false;
	boolean onePhaseRestriction = false;
	boolean restrictedTwoTransactions = false;
	boolean restrictedOneTransactions = false;
	boolean enableAll = false;
	int code = com.arjuna.ArjunaCommon.Core.LicenceType.TRANSACTION_CORE;
    
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-check") == 0)
		checkValues = true;
	    if (args[i].compareTo("-number") == 0)
	    {
		try
		{
		    Long m = Long.valueOf(args[i+1]);
		    number = m.intValue();
		}
		catch (NumberFormatException e)
		{
		    System.err.println(e);
		    System.exit(0);
		}
	    }
	    if (args[i].compareTo("-key") == 0)
		hostName = args[i+1];
	    if (args[i].compareTo("-enable") == 0)
	    {
		if (args[i+1].equals("all"))
		{
		    /*
		     * Enable all functionality.
		     * 
		     * At present this is just the same as enabling
		     * transactional objects, but may not be in future.
		     */

		    enableAll = true;
		}
		else
		{
		    if (args[i+1].equals("commit_one_phase"))
			onePhaseRestriction = true;
		    else
		    {
			if (args[i+1].equals("transactional_objects"))
			{
			    code = code | com.arjuna.ArjunaCommon.Core.LicenceType.TRANSACTIONAL_OBJECTS;
			}
			else
			{
			    if (args[i+1].equals("restricted_two_phase"))
				restrictedTwoTransactions = true;
			    else
			    {
				if (args[i+1].equals("restricted_one_phase"))
				    restrictedOneTransactions = true;
				else
				{
				    try
				    {
					String hexStart = "0x";
					String hexCode = args[i+1];
				
					if (args[i+1].startsWith(hexCode))
					    hexCode = args[i+1].substring(hexCode.length());
		    
					Long m = Long.valueOf(hexCode, 16);
					code = code | m.intValue();
				    }
				    catch (NumberFormatException e)
				    {
					System.err.println("Error - enable code is illegal: "+args[i+1]);
					System.exit(1);
				    }
				}
			    }
			}
		    }
		}

	    }
	    if (args[i].compareTo("-netmask") == 0)
	    {
		int dotStart = args[i+1].indexOf(".");
		int dotEnd = 0;

		if (dotStart == -1)
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
		else
		{
		    /*
		     * Dot notation.
		     */

		    String s = null;
		    dotEnd = dotStart;
		    dotStart = 0;

		    for (int ii = 0; ii < 4; ii++)
		    {
			s = args[i+1].substring(dotStart, dotEnd);

			try
			{
			    Long m = Long.valueOf(s);
			    int v = m.intValue();
			    
			    v = v << 8*(3-ii);

			    netmask = netmask | v;
			}
			catch (NumberFormatException e)
			{
			    System.err.println("Error - netmask format is illegal: "+args[i+1]);
			    System.exit(1);
			}

			dotStart = dotEnd+1;
			dotEnd = args[i+1].indexOf(".", dotEnd+1);

			if (dotEnd == -1)
			    dotEnd = args[i+1].length();
		    }
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

	if ((hostName == null) || ((date == 0) && (!noTimeout)))
	{
	    System.err.println("Parameter error.");
	    usage("licence");
	    
	    System.exit(1);
	}

	if (enableAll)
	{
	    code = code | com.arjuna.ArjunaCommon.Core.LicenceType.TRANSACTIONAL_OBJECTS;
	}
	
	if (!onePhaseRestriction)
	    code = code | com.arjuna.ArjunaCommon.Core.LicenceType.COMMIT_TWO_PHASE;

	boolean restricted = false;
	
	if (!restrictedTwoTransactions)
	{
	    code = code | com.arjuna.ArjunaCommon.Core.LicenceType.UNLIMITED_TWO_PHASE_TRANSACTIONS;
	}
	else
	    restricted = true;

	if (!restrictedOneTransactions)
	{
	    code = code | com.arjuna.ArjunaCommon.Core.LicenceType.UNLIMITED_ONE_PHASE_TRANSACTIONS;
	}
	else
	    restricted = true;
	
	if (restricted)
	{
	    switch (number)
	    {
	    case 100:
		code = code | com.arjuna.ArjunaCommon.Core.LicenceType.NUMBER_TRANSACTIONS_100;
		break;
	    case 1000:
		code = code | com.arjuna.ArjunaCommon.Core.LicenceType.NUMBER_TRANSACTIONS_1000;
		break;
	    case 10000:
		code = code | com.arjuna.ArjunaCommon.Core.LicenceType.NUMBER_TRANSACTIONS_10000;
		break;
	    case 100000:
		code = code | com.arjuna.ArjunaCommon.Core.LicenceType.NUMBER_TRANSACTIONS_100000;
		break;
	    default:
		System.err.println("Valid values are: 100, 1000, 10000, 100000");
		System.exit(0);
	    }
	}

	int localHost = 0;
	byte[] baddr = null;
	InetAddress addr = null;
	
	if (netmask != 0)
	{
	    try
	    {
		addr = InetAddress.getByName(hostName);
	    }
	    catch (UnknownHostException e)
	    {
		System.err.println("Error - cannot determine host inet information for: "+hostName);
		System.exit(1);
	    }
	
	    baddr = addr.getAddress();
	}
	else
	{
	    /*
	     * Check that the key is not an ip address. If it is, then print a
	     * warning message about the netmask being zero. Just in case!
	     */

	    try
	    {
		addr = InetAddress.getByName(hostName);

		System.err.println("WARNING: ZERO netmask specified!\n");
	    }
	    catch (UnknownHostException e)
	    {
	    }
	
	    baddr = hostName.getBytes();
	}
	
	for (int i = 0; i < baddr.length; i++)
	{
	    /*
	     * Convert signed byte into unsigned.
	     */
		
	    int l = 0x7f & baddr[i];

	    l += (0x80 & baddr[i]);
	    
	    localHost = (localHost << 8) | l;
	}

	localHost = localHost ^ 0x4bf2abc4;

	int localNetwork = localHost & netmask;
	int localDate   = (int) (System.currentTimeMillis()/1000+(long)date*24L*60L*60L);

	if (noTimeout)
	    localDate = 0xffffffff;
	
	if (checkValues)
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

	    int[] mask = new int[4];

	    mask[0] = 0xff000000;
	    mask[1] = 0x00ff0000;
	    mask[2] = 0x0000ff00;
	    mask[3] = 0x000000ff;

	    int originalHost = localHost ^ 0x4bf2abc4;

	    System.out.print("key "+hostName+" is ");
	    
	    for (int i = 0; i < 4; i++)
	    {
		int val = (originalHost & mask[i]);

		val = (val >> ((3-i)*8)) & mask[3];

		System.out.print(val);
		    
		if (i != 3)
		    System.out.print(".");
	    }

	    System.out.println();
	}

	localDate = localDate ^ 0xbeca54d2;
	netmask = netmask ^ 0xfc451ade;
	code = code ^ 0x1dc4fa42;

	int calc = 0xa6a6a6a6;
	int a = localDate;
	int b = localNetwork;
	int c = netmask;
	int d = code;

	for (int j = 0; j < 17; j++)
	{
	    a = (37 * (a ^ calc)) + 41;
	    b = (27 * (b ^ calc)) + 31;
	    c = (43 * (c ^ calc)) + 29;
	    d = (19 * (d ^ calc)) + 7;
	    calc = a ^ b ^ c ^ d;
	}

	System.out.println("\nJAVA_ARJUNA_LICENCE="
			   +Integer.toHexString(localHost)
			   +"-"+Integer.toHexString(netmask)
			   +"-"+Integer.toHexString(localDate)
			   +"-"+Integer.toHexString(code)
			   +"-"+Integer.toHexString(calc));
    }

};
