/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Init.java,v 1.1.4.1 2000/05/02 08:41:13 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.orbix;

import com.arjuna.CosTransactions.*;
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
	ORBFilterManager.addFilter(new OTS_InterpositionFilter(),
				   ORBFilterManager.FT_PROCESS);
    }
    
};
