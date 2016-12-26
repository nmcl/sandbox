/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ExplicitInterClient.javatmpl,v 1.1.4.3 1999/02/22 11:09:59 nmcl Exp $
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
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2 1998/11/12 08:52:40 nmcl Exp $
 */













































import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.JavaCosTransactions.*;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;
import org.omg.CosNaming  .NamingContextPackage.InvalidName;

public class ExplicitInterClient
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();

	OTS_Current current = OTS.current();
	Control theControl = null;
	String objectReference = "/tmp/object.ref";

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-object") == 0)
		objectReference = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: ExplicitInterClient [-object <reference>] [-help]");
		System.exit(0);
	    }
	}

	SetGet SetGetVar = null;
	short h = 0;

	try
	{
	    current.begin();
	    current.begin();
	    current.begin();
	    
	}
	catch (Exception e)
	{
	    System.err.println("Caught exception during begin: "+e);
	    System.exit(0);
	}

	try
	{
	    SetGetVar =  SetGetHelper .narrow(ORBObject.fileToObject(objectReference));
	}
	catch (Exception ex)
	{
	    System.err.println("Failed to bind to setget server: "+ex);
	    System.exit(0);
	}

	try
	{
	    theControl = current.get_control();
	
	    SetGetVar.set((short) 2, theControl);
	    //	    SetGetVar.set((short) 2, theControl);	    

	    theControl = null;
	    
	    System.out.println("Set value.");
	}
	catch (Exception ex1)
	{
	    System.err.println("Unexpected system exception during set: "+ex1);
	    System.exit(0);
	}

	try
	{
	    System.out.println("committing first nested action");
    
	    current.commit(true);

	    //	    SetGetVar.set((short) 4, current.get_control());
	    
	    System.out.println("committing second nested action");
    
	    current.commit(true);
	}
	catch (Exception sysEx)
	{
	    System.err.println("Caught unexpected exception during commit: "+sysEx);
	    System.exit(0);
	}
    
	try
	{
	    theControl = current.get_control();
	
	    h = SetGetVar.get(theControl);

	    theControl = null;
	
	    System.out.println("Got value.");
	}
	catch (Exception ex2)
	{
	    System.err.println("Unexpected system exception during get: "+ex2);
	    System.exit(0);
	}

	try
	{
	    current.commit(true);

	    System.out.println("committed top-level action");
	}
	catch (Exception ep)
	{
	    System.err.println("Caught commit exception for top-level action: "+ep);
	    System.exit(0);
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }
 
};

