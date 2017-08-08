/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecoveryDriver.java,v 1.3 1998/07/06 10:49:38 nmcl Exp $
 */

import com.arjuna.JavaGandiva.Common.Uid;
import com.arjuna.JavaCosRecovery.AADoctor.AARecord;

/*
 * This allows the user to explicitly force an atomic action to be
 * resolved. We assume that the user knows what he is doing!
 */
 
public class CrashRecoveryDriver
{

public static void main (String[] args)
    {
	Uid u = null;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-uid") == 0)
	    {
		u = new Uid(args[i+1]);
	    }
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: CrashRecoveryDriver [-help] -uid <uid>");
		System.exit(0);
	    }
	}

	AARecord record = new AARecord(System.out, true, u);

	if (record.resolve())
	    System.out.println("Atomic action "+u+" successfully resolved.");
	else
	    System.out.println("Cannot resolve action "+u);
    }

}
