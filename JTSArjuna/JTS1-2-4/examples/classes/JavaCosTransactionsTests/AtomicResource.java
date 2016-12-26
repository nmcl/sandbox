/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicResource.javatmpl,v 1.3 1998/07/06 13:29:40 nmcl Exp $
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

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.OTS_Factory;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;

public class AtomicResource extends  org.omg.CosTransactions ._ResourceImplBase 
{

public AtomicResource (boolean doCommit)
    {
	ORBInterface.orb().connect( this ) ;
	
	shouldCommit = doCommit;
    }

public org.omg.CosTransactions.Vote prepare () throws SystemException, HeuristicMixed, HeuristicHazard 
    {
	System.out.println("ATOMIC : PREPARE");

	if (shouldCommit)
	{
	    System.out.println("\tATOMIC : VoteCommit");

	    return Vote.VoteCommit;
	}
	else
	{
	    System.out.println("\tATOMIC : VoteRollback");
	    
	    return Vote.VoteRollback;
	}
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	System.out.println("ATOMIC : ROLLBACK");
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	System.out.println("ATOMIC : COMMIT");
    }

public void forget () throws SystemException
    {
	System.out.println("ATOMIC : FORGET");
    }

public void commit_one_phase () throws SystemException, HeuristicHazard
    {
	System.out.println("ATOMIC : COMMIT_ONE_PHASE");
    }

private boolean shouldCommit;

};
