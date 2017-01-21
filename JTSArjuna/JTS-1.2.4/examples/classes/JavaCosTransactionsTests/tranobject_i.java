/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: tranobject_i.javatmpl,v 1.3 1998/07/06 13:29:54 nmcl Exp $
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

public class tranobject_i extends  org.omg.CosTransactions ._ResourceImplBase 
{
    
public tranobject_i ()
    {
	ORBInterface.orb().connect( this ) ;
    }

public void foobar ()
    {
    }

public org.omg.CosTransactions.Vote prepare () throws SystemException
    {
	System.out.println("TRANOBJECT : PREPARE");
    
	return Vote.VoteCommit;
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	System.out.println("TRANOBJECT : ROLLBACK");
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	System.out.println("TRANOBJECT : COMMIT");
    }

public void forget () throws SystemException
    {
	System.out.println("TRANOBJECT : FORGET");
    }

public void commit_one_phase () throws SystemException, HeuristicHazard
    {
	System.out.println("TRANOBJECT : COMMIT_ONE_PHASE");
    }

};
