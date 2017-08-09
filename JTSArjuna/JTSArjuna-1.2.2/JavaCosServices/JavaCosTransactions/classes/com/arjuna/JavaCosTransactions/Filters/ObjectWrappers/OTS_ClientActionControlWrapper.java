/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientActionControlWrapper.java,v 1.2 1998/07/06 13:29:03 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientActionControlWrapper extends ActionControlObjectWrapper
{

public Control getParentControl () throws Unavailable, NotSubtransaction, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Control c = super.getParentControl();

	OTS.get_current().resume(theControl);

	return c;
    }

public void destroy () throws ActiveTransaction, ActiveThreads, BadControl, Destroyed, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	
	super.destroy();

	OTS.get_current().resume(theControl);
    }    
    
public Terminator get_terminator () throws Unavailable
    {
	Control theControl = OTS.get_current().suspend();
	Terminator theTerminator = super.get_terminator();

	OTS.get_current().resume(theControl);

	return theTerminator;
    }
    
public Coordinator get_coordinator () throws Unavailable
    {
	Control theControl = OTS.get_current().suspend();
	Coordinator theCoordinator = super.get_coordinator();

	OTS.get_current().resume(theControl);

	return theCoordinator;
    }

};
