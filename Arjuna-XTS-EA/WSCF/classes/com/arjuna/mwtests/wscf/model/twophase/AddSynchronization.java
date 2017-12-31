/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: AddSynchronization.java,v 1.1 2003/01/07 10:34:00 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.twophase;

import com.arjuna.mwtests.wscf.common.*;

import com.arjuna.mw.wscf.model.twophase.api.UserCoordinator;
import com.arjuna.mw.wscf.model.twophase.api.CoordinatorManager;

import com.arjuna.mw.wscf.model.twophase.UserCoordinatorFactory;
import com.arjuna.mw.wscf.model.twophase.CoordinatorManagerFactory;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;
import com.arjuna.mw.wscf.model.twophase.participants.*;

import com.arjuna.mw.wscf.common.*;

import com.arjuna.mw.wsas.activity.*;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: AddSynchronization.java,v 1.1 2003/01/07 10:34:00 nmcl Exp $
 * @since 1.0.
 */

public class AddSynchronization
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    CoordinatorManager cm = CoordinatorManagerFactory.coordinatorManager();
	    
	    cm.begin();

	    cm.enlistSynchronization(new TwoPhaseSynchronization());
	    
	    System.out.println("Started: "+cm.identifier()+"\n");

	    cm.confirm();

	    passed = true;
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
	
	if (passed)
	    System.out.println("\nPassed.");
	else
	    System.out.println("\nFailed.");
    }

}
