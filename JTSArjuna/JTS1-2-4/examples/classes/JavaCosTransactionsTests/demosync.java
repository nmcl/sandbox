/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: demosync.javatmpl,v 1.4.6.1 1999/02/16 14:56:33 nmcl Exp $
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
import com.arjuna.JavaCosTransactions.OTS;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UNKNOWN;

public class demosync extends  org.omg.CosTransactions ._SynchronizationImplBase 
{
    
public demosync ()
    {
	ORBInterface.orb().connect( this ) ;
    }

public void before_completion () throws SystemException
    {
	System.out.println("DEMOSYNC : BEFORE_COMPLETION");
	System.out.println("Synchronization throwing exception.");
	
	throw new UNKNOWN();
    }


public void after_completion (org.omg.CosTransactions.Status status) throws SystemException

    {
	System.out.print("DEMOSYNC : AFTER_COMPLETION ( ");
	OTS.printStatus(System.out, status);
	System.out.println(" )");

	throw new UNKNOWN(); // should not cause any affect!
    }

};
