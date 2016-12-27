/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init.java,v 1.1 1999/09/10 14:53:29 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Context.orbix;

import com.arjuna.JavaCosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CosTransactions.Unavailable;

public class Init
{

public Init ()
    {
	ORBFilterManager.addFilter(new OTS_ContextFilter());
    }
    
};
