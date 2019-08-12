/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Init.java,v 1.1.6.2.24.1 2000/11/08 14:30:27 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Context.jbroker;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
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
	    tsIdent.identifyTransactionService(new OTS_ContextFilter());
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.FATAL).println("OTS_ContextFilter initialisation failed with: "+e);

	    throw new com.arjuna.CosTransactions.OTS_Error(e.toString());
	}
    }
    
};
