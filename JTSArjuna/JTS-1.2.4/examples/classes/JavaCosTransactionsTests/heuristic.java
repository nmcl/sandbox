/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: heuristic.javatmpl,v 1.4 1998/08/13 14:34:34 nmcl Exp $
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

public class heuristic extends  org.omg.CosTransactions ._ResourceImplBase 
{
    
public heuristic (boolean p)
    {
	ORBInterface.orb().connect( this ) ;
	
	heuristicPrepare = p;
    }

public org.omg.CosTransactions.Vote prepare () throws HeuristicMixed, HeuristicHazard, SystemException
    {
	System.out.println("HEURISTIC : PREPARE");
	
	if (heuristicPrepare)
	{
	    System.out.println("HEURISTIC : throwing HeuristicHazard");

	    throw new HeuristicHazard();
	}

	System.out.println("\tHEURISTIC : VoteCommit");
    
	return Vote.VoteCommit;
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	System.out.println("HEURISTIC : ROLLBACK");
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	System.out.println("HEURISTIC : COMMIT");
    
	if (!heuristicPrepare)
	{
	    System.out.println("HEURISTIC : throwing HeuristicRollback");

	    throw new HeuristicRollback();
	}
    }

public void forget () throws SystemException
    {
	System.out.println("HEURISTIC : FORGET");
    }

public void commit_one_phase () throws SystemException, HeuristicHazard
    {
	System.out.println("HEURISTIC : COMMIT_ONE_PHASE");
    }

private boolean heuristicPrepare;
    
};
