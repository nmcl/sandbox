/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerFactory.java,v 1.3.2.1.4.1 2000/05/22 10:23:55 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;

public class OTS_ServerFactory
{

public static OTS_ServerControl create_transaction (Uid u,
						    Control parentControl,
						    OTS_Transaction parentImpl,
						    Coordinator realCoord,
						    Terminator realTerm,
						    int time_out)
    {
	OTS_ServerControl tranControl = new OTS_ServerControl(u, parentControl, parentImpl, realCoord, realTerm);

	if ((time_out != 0) && (parentImpl == null))
	{
	    /*
	     * Currently we do not remove controls from the list once they
	     * have terminated. We should to save time and space!
	     */
	
	    OTS_TransactionReaper reaper = OTS_TransactionReaper.transactionReaper();
	
	    if (reaper == null)
		reaper = OTS_TransactionReaper.create();

	    reaper.insert(new OTS_ControlWrapper((OTS_Control) tranControl), time_out);
	}

	return tranControl;
    }

public static OTS_ServerControl create_subtransaction (Uid actUid, Coordinator realCoord,
						       Terminator realTerm, OTS_ServerControl parent)
    {
	if (parent == null)
	{
	    System.err.println("WARNING - OTS_ServerFactory::create_subtransaction - no parent given!");
	    return null;
	}
    
	OTS_ServerControl toReturn = null;

	try
	{
	    Control handle = parent.getControl();
	    OTS_Transaction tranHandle = parent.getImplHandle ();
	
	    toReturn = new OTS_ServerControl(actUid, handle, tranHandle, realCoord, realTerm);

	    handle = null;
	    tranHandle = null;
	}
	catch (Exception e)
	{
	    if (toReturn != null)
	    {
		try
		{
		    toReturn.destroy();  // will delete itself
		}
		catch (Exception ex)
		{
		}
	    }
	}
    
	return toReturn;
    }
 
};
