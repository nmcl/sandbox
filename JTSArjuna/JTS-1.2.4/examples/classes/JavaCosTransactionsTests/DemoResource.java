/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DemoResource.javatmpl,v 1.3 1998/07/06 13:29:45 nmcl Exp $
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

import com.arjuna.OrbCommon.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CORBA  .SystemException;

public class DemoResource extends  org.omg.CosTransactions ._ResourceImplBase 
{
    
public DemoResource ()
    {
	ORBInterface.orb().connect( this ) ;
    }

public void registerResource () throws Unavailable, Inactive, SystemException
    {
	OTS_Current current = OTS.current();
	Control myControl = current.get_control();
	Coordinator coord = myControl.get_coordinator();

	coord.register_resource(this);

	System.out.println("Registered DemoResource");
    }


public org.omg.CosTransactions.Vote prepare () throws SystemException

    {
	System.out.println("DEMORESOURCE : PREPARE");
    
	return Vote.VoteCommit;
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	System.out.println("DEMORESOURCE : ROLLBACK");
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	System.out.println("DEMORESOURCE : COMMIT");
    }

public void forget () throws SystemException
    {
	System.out.println("DEMORESOURCE : FORGET");
    }

public void commit_one_phase () throws HeuristicHazard, SystemException
    {
	System.out.println("DEMORESOURCE : COMMIT_ONE_PHASE");
    }

};

