/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ChildLifetimeComplete.java,v 1.1 2001/07/23 13:44:57 nmcl Exp $
 */

package com.arjuna.Activity.Signals;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.Activity.SignalSets.SystemDefined;
import com.arjuna.Activity.Utility;
import org.omg.CosActivity.Signal;
import org.omg.CosActivity.CompletionStatus;
import org.omg.CosActivity.ActivityInformation;
import org.omg.CosActivity.ActivityInformationHelper;
import org.omg.CORBA.*;

public class ChildLifetimeComplete
{

    /*
     * We have a single SignalSet, and the coordinator will
     * fill in the isd with the child activity's global id.
     */

public static Signal getSignal (byte[] activityId, CompletionStatus cs)
    {
	ActivityInformation ai = new ActivityInformation(activityId, cs);
	Any isd = ORBInterface.orb().create_any();

	ActivityInformationHelper.insert(isd, ai);

	return new Signal("childComplete", SystemDefined.ChildLifetime, isd);
    }
    
};
