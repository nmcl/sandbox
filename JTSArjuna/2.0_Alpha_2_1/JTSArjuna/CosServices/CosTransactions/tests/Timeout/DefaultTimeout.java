/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DefaultTimeout.java,v 1.1 2000/02/25 14:08:28 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;

public class DefaultTimeout
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    ORBInterface.initOA();
	    String timeout = PropertyManager.getProperty(OTS_Environment.OTS_DEFAULT_TIMEOUT);
	    int sleepTime = 0;
	    
	    if (timeout != null)
	    {
		try
		{
		    Integer i = new Integer(timeout);

		    sleepTime = i.intValue();
		}
		catch (Exception e)
		{
		    System.err.println("Invalid default transaction timeout "+timeout);
		    System.err.println("Caught exception: "+e);
		
		    System.exit(0);
		}
	    }
	    
	    System.out.println("Thread "+Thread.currentThread()+" starting transaction.");
	    
	    OTS.current().begin();

	    Thread.sleep(sleepTime*1000*2, 0);

	    System.out.println("Thread "+Thread.currentThread()+" committing transaction.");

	    OTS.current().commit(false);

	    System.out.println("Transaction committed. Timeout did not go off.");
	    System.out.println("Test did not complete successfully.");
	}
	catch (Exception e)
	{
	    System.out.println("Caught exception: "+e);
	    System.out.println("Timeout went off.");

	    System.out.println("Test completed successfully.");
	}

	ORBInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }

};
