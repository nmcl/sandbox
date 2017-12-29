/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SynchronizationRecord.java,v 1.1 2003/01/07 10:33:55 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.twophase.arjunacore;

import com.hp.mw.ts.arjuna.ObjectType;
import com.hp.mw.ts.arjuna.coordinator.*;
import com.hp.mw.ts.arjuna.common.*;
import com.hp.mw.ts.arjuna.state.*;
import com.hp.mw.ts.arjuna.gandiva.ClassName;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.participants.Synchronization;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wsas.exceptions.SystemException;

import java.io.PrintWriter;

import java.io.IOException;

/**
 * An implementation of the ArjunaCore synchronization interface.
 * Synchronizations take part in the pre- and post- two-phase protocol and
 * are not persistent.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: SynchronizationRecord.java,v 1.1 2003/01/07 10:33:55 nmcl Exp $
 */

public class SynchronizationRecord implements com.hp.mw.ts.arjuna.coordinator.SynchronizationRecord
{
    
    /**
     * Constructor.
     *
     * @param theResource is the proxy that allows us to call out to the
     * object.
     */

    public SynchronizationRecord (Synchronization theResource, Uid id)
    {
	_resourceHandle = theResource;
	_id = new CoordinatorIdImple(id);
	
	if (_resourceHandle == null)
	    System.err.println("SynchronizationRecord "+_id+"- null participant provided!");
    }

    public void finalize () throws Throwable
    {
        _resourceHandle = null;
    }

    public boolean beforeCompletion ()
    {
	if (_resourceHandle != null)
	{
	    try
	    {
		_resourceHandle.beforeCompletion();
		
		return true;
	    }
	    catch (Exception ex)
	    {
		return false;
	    }
	}
	else
	    return false;
    }

    public boolean afterCompletion (int status)
    {
	if (_resourceHandle != null)
	{
	    try
	    {
		_resourceHandle.afterCompletion(convertStatus(status));
	    }
	    catch (Exception ex)
	    {
	    }
	    
	    return true;
        }

        return false;
    }

    public Uid get_uid ()
    {
	return _id;
    }
    
    private final int convertStatus (int result)
    {
	switch (result)
	{
	case ActionStatus.COMMITTED:
	case ActionStatus.COMMITTING:
	    return CoordinationResult.CONFIRMED;
	case ActionStatus.ABORTED:
	case ActionStatus.ABORTING:
	default:
	    return CoordinationResult.CANCELLED;
	}
    }
    
    private Synchronization    _resourceHandle;
    private CoordinatorIdImple _id;
    
}
