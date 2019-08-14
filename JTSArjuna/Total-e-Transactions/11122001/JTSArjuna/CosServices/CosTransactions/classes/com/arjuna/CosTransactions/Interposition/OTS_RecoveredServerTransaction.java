/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_RecoveredServerTransaction.java,v 1.1.2.1 2002/11/20 16:34:37 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import com.arjuna.CosTransactions.Interposition.*;
import com.arjuna.CosTransactions.Interposition.Arjuna.OTS_ServerRecoveryTopLevelAction;
import com.arjuna.CosTransactions.OTS_Utility;
import org.omg.CosTransactions.*;
import java.io.IOException;
import java.util.Date;

import org.omg.CORBA.SystemException;

/**
 * @version $Id: OTS_RecoveredServerTransaction.java,v 1.1.2.1 2002/11/20 16:34:37 nmcl Exp $
 */

public class OTS_RecoveredServerTransaction extends OTS_ServerTransaction
{
    /**
     * actionUid is the saving transaction identification for the remote
     * transaction.
     */

    public OTS_RecoveredServerTransaction (Uid actionUid)
    {
	super(actionUid);

	try
	{
	    if ((store().currentState(getSavingUid(), type()) != ObjectStore.OS_UNKNOWN))
	    {
		_activated = activate();
		
		if (!_activated)
		    ErrorStream.warning().println("OTS_RecoveredServerTransaction - activate of "+getSavingUid()+" failed!");
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("OTS_RecoveredServerTransaction - activate of "+getSavingUid()+" failed: "+e);
	}
    }

    public synchronized Status get_status () throws SystemException
    {
	Status theStatus = Status.StatusUnknown;

	if (!_activated)
	    theStatus = Status.StatusRolledBack;
	else
	    theStatus = super.get_status();

	return theStatus;
    }

    private boolean _activated;
    
}
