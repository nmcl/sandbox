/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CosServices.java,v 1.1 2000/02/25 14:09:06 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;
import java.io.LineNumberReader;
import java.io.FileReader;
import java.io.File;

import java.io.FileNotFoundException;

public class CosServices
{
    
public static void main (String[] args)
    {
	String cosservicesRoot = System.getProperty(ORBEnvironment.INITIAL_REFERENCES_ROOT,
						    com.arjuna.OrbCommon.Configuration.configFileRoot());
	String configLocation = System.getProperty(ORBEnvironment.INITIAL_REFERENCES_FILE,
						   com.arjuna.OrbCommon.Configuration.configFile());
	String configFile = cosservicesRoot+File.separatorChar+configLocation;
	LineNumberReader input = null;

	try
	{
	    input = new LineNumberReader(new FileReader(configFile));
	}
	catch (FileNotFoundException e)
	{
	    System.err.println("CosServices - could not open config file: "+configFile);
	    System.exit(0);
	}

	try
	{
	    boolean finished = false;

	    System.out.println("Contents of configuration file: "+configFile+"\n");
	    
	    while (!finished)
	    {
		String line = input.readLine();

		if (line != null)
		    System.out.println(line);
		else
		    finished = true;
	    }
	}
	catch (Exception e)
	{
	    System.err.println("Caught unexpected exception: "+e);
	}
    }

};
