/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientRecoveryCoordinatorWrapper.java,v 1.1 1999/09/10 15:04:09 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.ObjectWrappers.vbroker;

import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;

public class OTS_ClientRecoveryCoordinatorWrapper extends RecoveryCoordinatorObjectWrapper
{

public Status replay_completion () throws NotPrepared, SystemException
    {
	Control theControl = OTS.get_current().suspend();
	Status status = super.replay_completion();

	OTS.get_current().resume(theControl);

	return status;
    }

};
