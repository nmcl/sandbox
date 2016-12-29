/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Init.java,v 1.1 2000/02/25 14:05:35 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Context.orbacus;

import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import com.ooc.CORBA.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CosTransactions.Unavailable;

public class Init
{

public Init ()
    {
	/*
	 * Rather than require linking of 2 object files.
	 * (Maybe thread filters should be in the library
	 * by default?)
	 */

	OTS_ContextFilter filt = new OTS_ContextFilter();
	
	ORBFilterManager.addFilter(filt, ORBFilterManager.FT_PROCESS);
	ProcessInterceptorManager.add(filt);
    }

}
