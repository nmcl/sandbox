/*
 * Copyright (C) 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init.java,v 1.1 2000/01/27 15:25:37 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Context.orbacus;

import com.arjuna.JavaCosTransactions.*;
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
	
	ORBFilterManager.addFilter(filt);
	ProcessInterceptorManager.add(filt);
    }

}
