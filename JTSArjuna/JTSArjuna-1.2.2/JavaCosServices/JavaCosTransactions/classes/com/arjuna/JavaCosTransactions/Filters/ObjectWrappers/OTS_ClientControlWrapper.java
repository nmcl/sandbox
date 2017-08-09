/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientControlWrapper.java,v 1.2 1998/07/06 13:29:05 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientControlWrapper extends ControlObjectWrapper
{
    
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
