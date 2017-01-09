/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientSynchronizationWrapper.java,v 1.1 1999/09/10 15:04:11 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers.vbroker;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientSynchronizationWrapper extends SynchronizationObjectWrapper
{

public void before_completion () throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	
	super.before_completion();

	return status;
    }

public void after_completion (Status s) throws SystemException
    {
	Control theControl = OTS.get_current().suspend();
	
	super.after_completion(s);

	return status;
    }
    
};
