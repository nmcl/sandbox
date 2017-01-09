/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init.java,v 1.1 2000/01/27 15:27:17 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Interposition.orbacus;

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
	OTS_InterpositionFilter filt = new OTS_InterpositionFilter();
	
	ORBFilterManager.addFilter(filt);
	ProcessInterceptorManager.add(filt);
    }
    
};
