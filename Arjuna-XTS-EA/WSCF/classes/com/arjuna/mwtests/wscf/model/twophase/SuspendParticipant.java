/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SuspendParticipant.java,v 1.1 2003/01/07 10:34:01 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.twophase;

import com.arjuna.mwtests.wscf.common.*;

import com.arjuna.mw.wscf.model.twophase.api.UserCoordinator;
import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

import com.arjuna.mw.wscf.model.twophase.UserCoordinatorFactory;
import com.arjuna.mw.wscf.model.twophase.CoordinatorManagerFactory;

import com.arjuna.mwlabs.wscf.utils.ProtocolLocator;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

import com.arjuna.mw.wscf.exceptions.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SuspendParticipant.java,v 1.1 2003/01/07 10:34:01 nmcl Exp $
 * @since 1.0.
 */

public class SuspendParticipant
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    CoordinatorManager cm = CoordinatorManagerFactory.coordinatorManager();
	    
	    cm.begin();

	    cm.enlistParticipant(new TwoPhaseParticipant());
	    cm.enlistParticipant(new TwoPhaseParticipant());
	    cm.enlistSynchronization(new TwoPhaseSynchronization());
	    
	    System.out.println("Started: "+cm.identifier()+"\n");

	    ActivityHierarchy hier = cm.suspend();

	    System.out.println("Suspended: "+hier+"\n");

	    if (cm.currentActivity() != null)
	    {
		System.out.println("Hierarchy still active.");

		cm.cancel();
	    }
	    else
	    {
		System.out.println("Resumed: "+hier+"\n");
		
		cm.resume(hier);
		
		cm.confirm();

		passed = true;
	    }
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();

	    passed = false;
	}
	
	if (passed)
	    System.out.println("\nPassed.");
	else
	    System.out.println("\nFailed.");
    }

}
