/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LicenceTest.java,v 1.1.2.1 2000/08/29 09:00:38 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;

public class LicenceTest
{
    
public static void main (String[] args)
    {
	String licence = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].equals("-licence"))
		licence = args[i+1];
	    if (args[i].equals("-help"))
	    {
		System.out.println("Usage: LicenceTest -licence <licence> [-help]");
		System.exit(0);
	    }
	}

	if (licence == null)
	    licence = PropertyManager.getProperty("JAVA_ARJUNA_LICENCE");
	
	if (licence == null)
	{
	    System.err.println("Error - no licence specified.");
	    System.exit(0);
	}
	
	Uid u = new Uid();  // make sure licence lets us get this far!

	System.out.println("Licence "+licence+" enables: "+com.arjuna.ArjunaCommon.Core.LicenceType.checkAll());
    }
    
};
