/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DemoArjunaResource.javatmpl,v 1.2 1998/07/06 13:29:44 nmcl Exp $
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
import com.arjuna.ArjunaOTS.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CORBA  .SystemException;


public class DemoArjunaResource extends  com.arjuna.ArjunaOTS ._ArjunaSubtranAwareResourceImplBase 
{

public DemoArjunaResource ()
    {
	ORBInterface.orb().connect( this ) ;
    }

public void registerResource (boolean registerSubtran) throws Unavailable, Inactive, NotSubtransaction, SystemException
    {
	OTS_Current current = OTS.current();
	Control myControl = current.get_control();
	Coordinator coord = myControl.get_coordinator();
    
	if (registerSubtran)
	    coord.register_subtran_aware(this);
	else
	    coord.register_resource(this);

	System.out.println("Registered DemoArjunaResource");
    }
 

public org.omg.CosTransactions.Vote prepare_subtransaction () throws SystemException

    {
	System.out.println("DEMOARJUNARESOURCE : PREPARE_SUBTRANSACTION");

	return Vote.VoteCommit;
    }

public void commit_subtransaction (Coordinator parent) throws SystemException
    {
	System.out.println("DEMOARJUNARESOURCE : COMMIT_SUBTRANSACTION");
    }

public void rollback_subtransaction () throws SystemException
    {
	System.out.println("DEMOARJUNARESOURCE : ROLLBACK_SUBTRANSACTION");
    }
 

public org.omg.CosTransactions.Vote prepare () throws SystemException

    {
	System.out.println("DEMOARJUNARESOURCE : PREPARE");
    
	return Vote.VoteCommit;
    }

public void rollback () throws SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard
    {
	System.out.println("DEMOARJUNARESOURCE : ROLLBACK");
    }

public void commit () throws SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard
    {
	System.out.println("DEMOARJUNARESOURCE : COMMIT");
    }

public void forget () throws SystemException
    {
	System.out.println("DEMOARJUNARESOURCE : FORGET");
    }

public void commit_one_phase () throws SystemException, HeuristicHazard
    {
	System.out.println("DEMOARJUNARESOURCE : COMMIT_ONE_PHASE");
    }
 
};
