/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OutOfOrderTransaction.java,v 1.1 2002/04/22 14:04:17 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import com.arjuna.CosTransactions.*;
import org.omg.CosActivity.*;
import org.omg.CosTransactions.*;
import java.io.*;

/*
 * Try to terminate a transaction that has running activities.
 */

public class OutOfOrderTransaction
{
    
public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    org.omg.CosActivity.Current current = CosActivity.get_current();
	    org.omg.CosTransactions.Current txCurrent = OTS.get_current();
	    
	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    txCurrent.begin();
	    System.out.println("Created transaction: "+txCurrent.get_transaction_name()+"\n");
	    
	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    System.out.println("rolling back transaction.");
	    
	    txCurrent.rollback();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();

	if (passed)
	    System.out.println("\nPassed.");
	else
	    System.out.println("\nFailed.");
	
	System.exit(0);
    }

};
