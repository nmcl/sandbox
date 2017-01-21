/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: setget_i.javatmpl,v 1.2.6.1 1999/02/05 09:57:43 nmcl Exp $
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













































package JavaCosTransactionsTests;

import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;
import com.arjuna.JavaCosTransactions.OTS_ExplicitInterposition;
import TestModule.*;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;
import org.omg.CORBA  .UNKNOWN;

public class setget_i extends  TestModule ._SetGetImplBase 
{

public setget_i ()
    {
	value = 0;
    }

public setget_i (String name)
    {

	
	value = 0;
    }

public void finalize ()
    {
    }

public void set (short n, Control control) throws SystemException
    {
	System.out.println("setget_i.set "+n);

	try
	{
	    if (control != null)
	    {
		OTS_ExplicitInterposition manager = new OTS_ExplicitInterposition();
		
		if (!manager.registerTransaction(control))
		{
		    System.err.println("setget_i.set - error in setting up hierarchy");

		    throw new UNKNOWN();
		}
		else
		    System.out.println("setget_i.set - managed to set up interposition hierarchy");
    
		OTS_Current current = OTS.current();
		Control cont = current.get_control();

		if (cont == null)
		    System.err.println("setget_i.set error - current returned no control!");
		else
		{
		    System.out.println("setget_i.set - current returned a control!");

		    cont = null;
		}
		
		System.out.println("setget_i.set - beginning nested action");

		current.begin();

		cont = current.get_control();

		if (cont != null)
		{
		    Coordinator coord = cont.get_coordinator();

		    System.out.println("setget_i.set - registering self");
		
		    coord.register_resource(this);

		    coord = null;
		    cont = null;
		}
		else
		    System.err.println("setget_i.set - current did not return control after begin!");
	    
		value = n;
	    
		System.out.println("setget_i.set - committing nested action");
	
		current.commit(true);

		manager.unregisterTransaction();

		manager = null;
	    }
	    else
		System.err.println("setget_i::set error - no control!");
	}
	catch (Exception e)
	{
	    System.err.println("setget_i::set - caught exception: "+e);
	}

	System.out.println("setget_i.set - finished");
    }

public short get (Control p) throws SystemException
    {
	return value;
    }

public void commit_subtransaction (Coordinator parent) throws SystemException
    {
	System.out.println("SETGET_I : COMMIT_SUBTRANSACTION");
    }

public void rollback_subtransaction () throws SystemException
    {
	System.out.println("SETGET_I : ROLLBACK_SUBTRANSACTION");
    }


public org.omg.CosTransactions.Vote prepare () throws SystemException

    {
	System.out.println("SETGET_I : PREPARE");
    
	return Vote.VoteCommit;
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	System.out.println("SETGET_I : ROLLBACK");
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	System.out.println("SETGET_I : COMMIT");
    }

public void forget () throws SystemException
    {
	System.out.println("SETGET_I : FORGET");
    }

public void commit_one_phase () throws SystemException, HeuristicHazard
    {
	System.out.println("SETGET_I : COMMIT_ONE_PHASE");
    }

private short value;
 
};
