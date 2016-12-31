/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Tyne and Wear,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ApplyLicence.java,v 1.2 2000/03/16 16:17:06 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;

import java.io.FileNotFoundException;
import java.io.IOException;

public class ApplyLicence
{

public static void main (String[] args)
    {
	String licence = null;
	String licenceName = "JAVA_ARJUNA_LICENCE";
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
		usage();
	    if (args[i].compareTo("-licence") == 0)
		licence = args[i+1];
	    if (args[i].compareTo("-name") == 0)
		licenceName = args[i+1];
	    if (args[i].compareTo("-version") == 0)
	    {
		System.out.println("ArjunaCommon version: "+Configuration.version());
		System.exit(0);
	    }
	}

	if (licence == null)
	    usage();
	else
	{
	    String theFile = PropertyManager.getProperty(com.arjuna.ArjunaCommon.Common.Environment.PROPERTIES_FILE);

	    if (theFile == null)
	    {
		System.err.println("FATAL - no licence file specified!");
		System.exit(0);
	    }
	    
	    try
	    {
		File oldFile = new File(theFile);
		LineNumberReader input = new LineNumberReader(new FileReader(oldFile));
		String tmpFile = theFile+".tmp";
		File nuFile = new File(tmpFile);
		PrintWriter output = new PrintWriter(new FileOutputStream(nuFile));
		String line = null;

		do
		{
		    line = input.readLine();

		    if (line != null)
		    {
			if (!line.startsWith(licenceName))
			    output.println(line);
		    }
		    
		} while (line != null);

		output.println(licenceName+"="+licence);
		output.flush();
		
		if (!nuFile.renameTo(oldFile))
		    System.err.println("WARNING - Could not rename "+tmpFile+" to "+theFile);
	    }
	    catch (FileNotFoundException e)
	    {
		System.err.println("FATAL - Could not open property file "+theFile);
		System.exit(0);
	    }
	    catch (IOException e)
	    {
		System.err.println("FATAL - caught IOException: "+e);
		System.exit(0);
	    }
	    catch (Exception e)
	    {
		System.err.println("FATAL - caught exception: "+e);
		System.exit(0);
	    }
	}
    }

private static void usage ()
    {
	System.out.println("Usage: ApplyLicence -licence <licence> [-name <licence name>] [-version] [-help]");
	System.exit(0);
    }
 
};
