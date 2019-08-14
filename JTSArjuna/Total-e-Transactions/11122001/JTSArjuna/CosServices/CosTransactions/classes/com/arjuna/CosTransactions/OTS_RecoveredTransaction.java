/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_RecoveredTransaction.java,v 1.1.2.1 2002/11/20 16:34:36 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.CosTransactions.OTS_Transaction;
import com.arjuna.CosTransactions.OTS_Utility;

import org.omg.CosTransactions.*;
import java.io.IOException;

import org.omg.CORBA.SystemException;

/**
 * Used to determine the current state of a transaction from the object store.
 *
 * @version $Id: OTS_RecoveredTransaction.java,v 1.1.2.1 2002/11/20 16:34:36 nmcl Exp $
 */

public class OTS_RecoveredTransaction extends OTS_Transaction
{

    public OTS_RecoveredTransaction (Uid actionUid)
    {
	super(actionUid);

	try
	{
	    if ((store().currentState(actionUid, type()) != ObjectStore.OS_UNKNOWN))
	    {
		_activated = activate();
		
		if (!_activated)
		    ErrorStream.warning().println("OTS_RecoveredTransaction - activate of "+actionUid+" failed!");
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("OTS_RecoveredTransaction - activate of "+actionUid+" failed: "+e);
	}
    }

    /**
     * Get the status of the transaction. If we successfully activated
     * the transaction then we return whatever the transaction reports
     * otherwise we return RolledBack as we're using presumed abort.
     */

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
