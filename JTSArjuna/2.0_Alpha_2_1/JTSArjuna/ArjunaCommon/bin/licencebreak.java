/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Tyne and Wear,
 * Tyne and Wear,
 * UK.  
 * 
 * $Id: licencebreak.java,v 1.3.4.1 2000/04/25 08:35:53 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;
import java.util.Date;

import java.lang.NumberFormatException;
import java.io.IOException;

/*
 * We could store things like the software type and version
 * encoded within each licence.
 */

public class licencebreak
{

public static void usage (String name)
    {
	System.out.println("Usage: "+name+" -licence <licence> [-help]");
    }
    
public static void main (String[] args)
    {
	String li = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-licence") == 0)
		li = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		usage("licencemanager");
		System.exit(0);
	    }
	}

	if (li == null)
	{
	    System.err.println("Parameter error.");
	    usage("licencemanager");
	    System.exit(1);
	}
	
	boolean ok = true;
	int index1 = li.indexOf('-');
	int index2 = 0;
	String hString = null;
	String nmString = null;
	String dString = null;
	String cString = null;
	
	if (index1 != -1)
	    hString = li.substring(0, index1);
	else
	    ok = false;

	if (ok)
	{
	    index2 = li.indexOf('-', index1 +1);
		    
	    if (index2 != -1)
	    {
		nmString = li.substring(index1 +1, index2);
		index1 = index2;
	    }
	    else
		ok = false;
	}

	if (ok)
	{
	    index2 = li.indexOf('-', index1 +1);
	    
	    if (index2 != -1)
	    {
		dString = li.substring(index1 +1, index2);
		index1 = index2;
	    }
	    else
		ok = false;
	}

	if (ok)
	{
	    cString = li.substring(index1 +1);
	}

	if (ok)
	{
	    /*
	     * We have to use hexStringToLong rather than
	     * hexStringToInt because Java throws an exception for
	     * large (valid!) integer strings such as d0a15428.
	     */

	    int suppliedHost = (int) Utility.hexStringToLong(hString);
	    int suppliedNetmask = (int) Utility.hexStringToLong(nmString);
	    int suppliedDate = (int) Utility.hexStringToLong(dString);
	    int suppliedCheck = (int) Utility.hexStringToLong(cString);

	    int originalNetmask = suppliedNetmask ^ 0xfc451ade;
	    int originalHost = suppliedHost ^ 0x4bf2abc4;
	    int originalDate = suppliedDate ^ 0xbeca54d2;

	    System.out.print("Host: ");

	    int[] mask = new int[4];

	    mask[0] = 0xff000000;
	    mask[1] = 0x00ff0000;
	    mask[2] = 0x0000ff00;
	    mask[3] = 0x000000ff;

	    for (int i = 0; i < 4; i++)
	    {
		int val = (originalHost & mask[i]);

		val = (val >> ((3-i)*8)) & mask[3];

		System.out.print(val);

		if (i != 3)
		    System.out.print(".");
	    }

	    System.out.print("\nNetmask: ");

	    for (int i = 0; i < 4; i++)
	    {
		int val = (originalNetmask & mask[i]);

		val = (val >> ((3-i)*8)) & mask[3];
		
		System.out.print(val);

		if (i != 3)
		    System.out.print(".");
	    }
	    
	    long theTime = (long) (originalDate)*1000L;
	    Date date = new Date(theTime);
	    
	    System.out.println("\nDate: "+date);
	}
	else
	    System.out.println("Licence "+li+" invalid.");
    }
    
};
