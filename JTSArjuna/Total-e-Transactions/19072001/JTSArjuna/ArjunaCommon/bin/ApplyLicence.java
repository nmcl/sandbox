/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Tyne and Wear,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ApplyLicence.java,v 1.2.28.1.14.1 2001/03/19 15:52:24 nmcl Exp $
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
		String tmpFile = theFile;
		LineNumberReader input = null;
		
		try
		{
		    input = new LineNumberReader(new FileReader(oldFile));
		    tmpFile = tmpFile+".tmp";
		}
		catch (FileNotFoundException e)
		{
		    input = null;
		}

		File nuFile = new File(tmpFile);
		PrintWriter output = new PrintWriter(new FileOutputStream(nuFile), true);
		String line = null;

		if (input != null)
		{
		    do
		    {
			line = input.readLine();

			if (line != null)
			{
			    if (!line.startsWith(licenceName))
				output.println(line);
			}
			
		    } while (line != null);

		    input.close();
		}
		
		output.println(licenceName+"="+licence);
		output.close();

		if ((input != null) && oldFile.exists())
		    oldFile.delete();
		
		if ((input != null) && !nuFile.renameTo(oldFile))
		    System.err.println("WARNING - Could not rename "+nuFile+" to "+oldFile);
	    }
	    catch (IOException e)
	    {
		System.err.println("FATAL - caught IOException: "+e);
		
		e.printStackTrace();
		
		System.exit(0);
	    }
	    catch (Exception e)
	    {
		System.err.println("FATAL - caught exception: "+e);

		e.printStackTrace();
		
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
