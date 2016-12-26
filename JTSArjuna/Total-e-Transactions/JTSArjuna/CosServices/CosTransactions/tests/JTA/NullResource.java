/*
 * Copyright (C) 2002,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: NullResource.java,v 1.1.2.1 2002/06/19 12:44:10 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.jta.*;
import javax.transaction.*;
import javax.transaction.xa.*;

import java.lang.IllegalAccessException;

public class NullResource
{

    public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("Initialisation failed: "+e);
	}

	boolean passed = false;
	
	try
	{
	    for (int i = 0; i < 1000; i++)
	    {
		TransactionManager tm = JTA_TransactionManager.transactionManager();

		tm.begin();

		tm.getTransaction().rollback();

		tm.suspend();
	    }

	    passed = true;
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}

	if (passed)
	    System.out.println("\nTest completed successfully.");
	else
	    System.out.println("\nTest did not complete successfully.");

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }

}
