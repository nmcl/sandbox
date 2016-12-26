/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ExplicitArjunaClient.javatmpl,v 1.1.4.1 1999/01/28 19:04:07 nmcl Exp $
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
import org.omg.CORBA  .IntHolder;

public class ExplicitArjunaClient
{

public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
    
	OTS_Current current = OTS.current();
	String refFile = "/tmp/explicitstack.ref";
	int value = 1;
	Control cont = null;
    
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-reffile") == 0)
		refFile = args[i+1];
	    if (args[i].compareTo("-value") == 0)
	    {
		try
		{
		    Integer val = new Integer(args[i+1]);
		    value = val.intValue();
		}
		catch (Exception e)
		{
		    System.err.println(e);
		    System.exit(0);
		}
	    }
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: ExplicitArjunaClient [-reffile <file>] [-value <number>] [-help]");
		System.exit(0);
	    }
	}

	try
	{
	    System.out.println("Starting initialising top-level transaction.");
	
	    current.begin();

	    System.out.println("Initialising transaction name: "+current.get_transaction_name());
	}
	catch (Exception e)
	{
	    System.err.println(e);
	    System.exit(1);
	}

	ExplicitStack stackVar = null;   // pointer the grid object that will be used.
	
	try
	{
	    stackVar =  ExplicitStackHelper .narrow(ORBObject.fileToObject(refFile));
	}
	catch (Exception e)
	{
	    System.err.println("Bind error: "+e);
	    System.exit(1);
	}

	try
	{
	    System.out.println("pushing "+value+" onto stack");

	    cont = current.get_control();
	    stackVar.push(value, cont);

	    System.out.println("\npushing "+(value+1)+" onto stack");

	    stackVar.push(value+1, cont);

	    cont = null;
	}
	catch (Exception e)
	{
	    System.err.println("call to push failed: "+e);
	    System.exit(1);
	}

	try
	{
	    current.commit(true);

	    System.out.println("Committed top-level transaction");
	    System.out.println("\nBeginning top-level transaction");
    
	    current.begin();

	    System.out.println("Top-level name: "+current.get_transaction_name());
    
	    IntHolder val = new IntHolder(-1);
    
	    cont = current.get_control();
    
	    if (stackVar.pop(val, cont) == 0)
	    {
		System.out.println("popped top of stack "+val.value);
		System.out.println("\nbeginning nested transaction");

		current.begin();

		System.out.println("nested name: "+current.get_transaction_name());

		cont = null;
		cont = current.get_control();    
		stackVar.push(value+2, cont);

		System.out.println("pushed "+(value+2)+" onto stack. Aborting nested action.");

		cont = null;  // current will destroy this control!
		current.rollback();
		cont = current.get_control();
	
		System.out.println("current transaction name: "+current.get_transaction_name());
		System.out.println("rolledback nested transaction");

		stackVar.pop(val, cont);

		System.out.println("\npopped top of stack is "+val.value);

		System.out.println("\nTrying to print stack contents - should fail.");
	
		stackVar.printStack();
	
		cont = null;
		current.commit(true);

		System.out.println("\nCommitted top-level transaction");
	
		if (current.get_transaction_name() == null)
		    System.out.println("current transaction name: null");
		else
		    System.out.println("Error - current transaction name: "
				       +current.get_transaction_name());
	
		if (val.value == value)
		    System.out.println("\nThis is correct.");
		else
		    System.out.println("\nThis is incorrect. Value should be "+value);
	    }
	    else
	    {
		System.out.println("Error getting stack value.");
	
		current.rollback();

		System.out.println("\nRolledback top-level transaction.");
	    }

	    try
	    {
		System.out.println("\nPrinting stack contents (should be empty).");
	
		stackVar.printStack();
	    }
	    catch (Exception e)
	    {
		System.out.println("\nError - could not print.");
	    }
	}
	catch (Exception e)
	{
	    System.err.println("Caught unexpected exception: "+e);
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }

};
