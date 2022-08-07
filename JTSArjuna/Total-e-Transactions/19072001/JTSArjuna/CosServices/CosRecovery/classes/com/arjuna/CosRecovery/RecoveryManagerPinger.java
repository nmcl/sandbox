/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryManagerPinger.javatmpl,v 1.1.2.2.2.1.28.1.2.1 2001/06/26 09:34:08 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosRecovery;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.CosRecovery.Admin.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.UserException;

/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.1.2.1.28.1.14.1 2001/03/20 16:24:46 nmcl Exp $
 */





/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/





/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;




/**
 * Calls "Admin::RecoveryManagerAdmin::ping()" on the RecoveryManager
 * at intervals
 */
public class RecoveryManagerPinger
{
    public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("ORB/OA initialisation failed: "+e);
	    
	    System.exit(0);
	}
    
	RecoveryManagerAdmin recManagerAdmin = null;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: RecoveryManagerPinger [-help]");
		System.exit(0);
	    }
	}

	// this must be aligned with what happens in RecoveryManager
	
	String recoveryManagerAdminName = RecoveryEnablement.getRecoveryAdminName();

	try
	{
	    org.omg.CORBA.Object obj = ORBServices.getService(recoveryManagerAdminName,
							      null);
	    
	    recManagerAdmin =  com.arjuna.CosRecovery.Admin.RecoveryManagerAdminHelper .narrow(obj);
	}
	catch (Exception sysEx)
	{
	    System.err.println("Failed to bind to RecoveryManagerAdmin: "+sysEx);
	    System.exit(1);
	}

	for (int i=0 ; i<_numberOfPings ; i++)
	{
	    try
	    {
		System.out.println("Ping result = " + recManagerAdmin.ping());
	    }
	    catch (SystemException se)
	    {
		System.out.println("Ping failed");
		break;
	    }

	    try
	    {
		Thread.sleep(_pingPeriod);
	    }
	    catch (InterruptedException e)
	    {
	    }
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
	
	System.exit(0);
    }

    private static int _pingPeriod = 10000;
    private static int _numberOfPings = 1000;

    /** not used */
    private RecoveryManagerPinger()
    {
    }
};

