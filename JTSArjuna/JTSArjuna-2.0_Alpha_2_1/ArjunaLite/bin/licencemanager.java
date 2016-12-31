/*
 * Copyright (C) 1998, 1999, 2000,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 * 
 * $Id: licencemanager.java,v 1.1 2000/02/25 14:03:33 nmcl Exp $
 */

import java.io.*;
import java.util.Date;

import java.lang.NumberFormatException;
import java.io.IOException;

/*
 * We could store things like the software type and version
 * encoded within each licence.
 */

public class licencemanager
{

public static void usage (String name)
    {
	System.out.println("Usage: "+name+" -licensee <name> -licence <licence> -version <number> [-file <db file>] [-ip <ip addr>] [-help]");
    }
    
private static String dbFile = "/var/tmp/JTSAITLicence.db";

public static void main (String[] args)
    {
	String fileName = dbFile;
	String licensee = null;
	String licence = null;
	String version = null;
	String ipaddr = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-licensee") == 0)
		licensee = args[i+1];
	    if (args[i].compareTo("-licence") == 0)
		licence = args[i+1];
	    if (args[i].compareTo("-version") == 0)
		version = args[i+1];	
	    if (args[i].compareTo("-file") == 0)
		fileName = args[i+1];
	    if (args[i].compareTo("-ip") == 0)
		ipaddr = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		usage("licencemanager");
		System.exit(0);
	    }
	}

	if ((fileName == null) || (licensee == null) || (licence == null) || (version == null))
	{
	    System.err.println("Parameter error.");
	    usage("licencemanager");
	    System.exit(1);
	}

	int index1 = -1;
	
	for (int j = 0; j < 2; j++)
	{
	    index1 = licence.indexOf('-', index1+1);

	    if (index1 == -1)
	    {
		System.err.println("Licence format error: "+licence);
		System.exit(1);
	    }
	}

	int index2 = licence.indexOf('-', index1+1);

	String dString = licence.substring(index1 +1, index2);
	long suppliedDate = 0;

	try
	{
	    Long m = Long.valueOf(dString, 16);
	    suppliedDate = m.intValue();
	}
	catch (NumberFormatException e)
	{
	    System.err.println(e);
	    System.exit(1);
	}

	try
	{
	    PrintWriter output = new PrintWriter(new FileWriter(fileName, true));

	    long theTime = (long) (suppliedDate ^ 0x8a021bd6)*1000L;
	    Date date = new Date(theTime);

	    output.print("JTSAIT: "+version+"\t"+licensee+"\t"+licence+"\t"+date+"\t");

	    if (ipaddr == null)
		output.println("subnet wide");
	    else
		output.println("for machine "+ipaddr);
	    
	    output.flush();
	}
	catch (IOException e)
	{
	    System.err.println(e);
	    System.exit(1);
	}
    }
    
};
