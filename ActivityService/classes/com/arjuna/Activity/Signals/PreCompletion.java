/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PreCompletion.java,v 1.2 2002/02/28 11:43:36 nmcl Exp $
 */

package com.arjuna.Activity.Signals;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.Activity.SignalSets.SystemDefined;
import org.omg.CosActivity.CompletionStatus;
import org.omg.CosActivity.Signal;
import org.omg.CosActivity.ActivityInformation;
import org.omg.CosActivity.ActivityInformationHelper;
import org.omg.CORBA.*;

public class PreCompletion
{

public static Signal getSignal (byte[] activityId, CompletionStatus cs)
    {
	ActivityInformation ai = new ActivityInformation(activityId, cs, null);
	Any isd = ORBInterface.orb().create_any();

	ActivityInformationHelper.insert(isd, ai);

	return new Signal("preCompletion", SystemDefined.Synchronization, isd);
    }
 
};
