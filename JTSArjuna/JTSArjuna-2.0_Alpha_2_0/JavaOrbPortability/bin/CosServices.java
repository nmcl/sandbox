/*
 * Copyright (C) 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CosServices.javatmpl,v 1.3.8.2 1999/08/25 11:00:11 nmcl Exp $
 */

/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_krc.h,v 1.3 1999/09/10 13:49:11 nmcl Exp $
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
	String cosservicesRoot = System.getProperty(environment.INITIAL_REFERENCES_ROOT, "/usr/local/JTSArjuna/etc/");
	String configLocation = System.getProperty(environment.INITIAL_REFERENCES_FILE, "CosServices.cfg");
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
