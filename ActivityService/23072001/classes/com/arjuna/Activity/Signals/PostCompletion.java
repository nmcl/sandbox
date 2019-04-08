/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PostCompletion.java,v 1.1 2001/07/23 13:44:59 nmcl Exp $
 */

package com.arjuna.Activity.Signals;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.Activity.SignalSets.SystemDefined;
import com.arjuna.Activity.Utility;
import com.arjuna.Activity.CosActivity;
import com.arjuna.Activity.ActivityCoordinator.ActivityCoordinatorWrapper;
import org.omg.CosActivity.CompletionStatus;
import org.omg.CosActivity.Outcome;
import org.omg.CosActivity.Signal;
import org.omg.CosActivity.ActivityInformation;
import org.omg.CosActivity.ActivityInformationHelper;
import org.omg.CORBA.*;

public class PostCompletion
{

public static Signal getSignal (byte[] activityId, CompletionStatus cs)
    {
	Outcome outcome = CosActivity.current().getCoordinator().getOutcome();
	ActivityInformation ai = new ActivityInformation(activityId, cs, outcome);
	Any isd = ORBInterface.orb().create_any();

	ActivityInformationHelper.insert(isd, ai);

	return new Signal("postCompletion", SystemDefined.Synchronization, isd);
    }
    
};
