/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityFactory.java,v 1.1 2001/07/23 13:44:53 nmcl Exp $
 */

package com.arjuna.Activity.ActivityCoordinator;

import com.arjuna.ArjunaCommon.Common.Uid;
import org.omg.CosActivity.*;
import org.omg.CosTransactions.Control;

public class ActivityFactory
{

public ActivityCoordinatorImple create (Uid uid, int timeout)
    {
	ActivityCoordinatorImple act = new ActivityCoordinatorImple(uid, null);

	return act;
    }

public ActivityCoordinatorImple create (Uid uid, int timeout, Control tx)
    {
	ActivityCoordinatorImple act = new ActivityCoordinatorImple(uid, null, tx);

	return act;
    }
	
public ActivityCoordinatorImple createSubactivity (Uid uid,
						   ActivityCoordinatorImple parent,
						   int timeout)
    {
	ActivityCoordinatorImple act = new ActivityCoordinatorImple(uid, parent);
	
	return act;
    }

public static ActivityFactory factory ()
    {
	return ActivityFactory._activityFactory;
    }
    
private ActivityFactory ()
    {
    }
    
private static ActivityFactory _activityFactory = new ActivityFactory();
	
};
