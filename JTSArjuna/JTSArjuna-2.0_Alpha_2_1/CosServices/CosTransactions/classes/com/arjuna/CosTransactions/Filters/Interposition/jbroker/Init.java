/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Init.java,v 1.1 2000/02/25 14:05:50 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.jbroker;

import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import com.objectEra.jbroker.api.transaction.TSIdentification;

public class Init
{

public Init ()
    {
	try
	{
	    TSIdentification tsIdent = (TSIdentification) ORBInterface.orb().resolve_initial_references("TSIdentification");
	    tsIdent.identifyTransactionService(new OTS_InterpositionFilter());
	}
	catch (Exception e)
	{
	    System.err.println("OTS_InterpositionFilter initialisation failed with: "+e);
	    System.exit(0);
	}
    }
    
};
