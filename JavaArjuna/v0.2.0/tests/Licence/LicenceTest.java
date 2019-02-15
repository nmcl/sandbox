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

import com.arjuna.JavaArjuna.Common.*;

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

	Utility.loadProperties();
	
	if (licence == null)
	    licence = System.getProperty("JAVA_ARJUNA_LICENCE");
	
	if (licence == null)
	{
	    System.err.println("Error - no licence specified.");
	    System.exit(0);
	}
	
	Uid u = new Uid();  // make sure licence lets us get this far!

	/*
	 * LicenceType existed in later versions because the licence became capable of
	 * enabling or disabling certain features such as 2PC. In the earlier versions
	 * it was simply a binary choice, i.e., if you had a licence you could use JavaArjuna and
	 * if there was no such licence then you couldn't.

	System.out.println("Licence "+licence+" enables: "+com.arjuna.ArjunaCommon.Core.LicenceType.checkAll());
	*/
    }
    
};
