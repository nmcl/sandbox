/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SynchronizationRecord.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore;

import com.hp.mw.ts.arjuna.ObjectType;
import com.hp.mw.ts.arjuna.coordinator.*;
import com.hp.mw.ts.arjuna.common.*;
import com.hp.mw.ts.arjuna.state.*;
import com.hp.mw.ts.arjuna.gandiva.ClassName;

import com.arjuna.mw.wscf.model.as.coordinator.twophase.outcomes.*;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.messages.*;
import com.arjuna.mw.wscf.model.as.coordinator.Participant;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.*;

import com.arjuna.mw.wscf.model.twophase.common.TwoPhaseResult;
import com.arjuna.mw.wscf.model.twophase.outcomes.*;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.completionstatus.*;

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
 * @version $Id: SynchronizationRecord.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 */

public class SynchronizationRecord implements com.hp.mw.ts.arjuna.coordinator.SynchronizationRecord
{
    
    /**
     * Constructor.
     *
     * @param theResource is the proxy that allows us to call out to the
     * object.
     */

    public SynchronizationRecord (Participant theResource, Uid id, int priority, Qualifier[] quals)
    {
	_resourceHandle = theResource;
	_priority = priority;
	_quals = quals;
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
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new BeforeCompletion(_id));
		
		if (res != null)
		{
		    if (res instanceof CoordinationOutcome)
		    {
			return true;
		    }
		    else
			return false;
		}
		else
		    return true;
            }
            else
                System.err.println("SynchronizationRecord.beforeCompletion for "+_id+" called without a resource!");
        }
        catch (Exception e)
        {
            System.err.println("SynchronizationRecord.beforeCompletion "+_id+" caught exception: ");

	    e.printStackTrace();
        }

        return false;
    }

    public boolean afterCompletion (int status)
    {
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new AfterCompletion(_id, convertStatus(status)));
		
		if (res != null)
		{
		    if (res instanceof CoordinationOutcome)
		    {
			return true;
		    }
		    else
			return false;
		}
		else
		    return true;
            }
            else
                System.err.println("SynchronizationRecord.afterCompletion for "+_id+" called without a resource!");
        }
        catch (Exception e)
        {
            System.err.println("SynchronizationRecord.afterCompletion "+_id+" caught exception: ");

	    e.printStackTrace();
        }

        return false;
    }

    public Uid get_uid ()
    {
	return _id;
    }
    
    private final CompletionStatus convertStatus (int result)
    {
	switch (result)
	{
	case ActionStatus.COMMITTED:
	case ActionStatus.COMMITTING:
	    return Success.instance();
	case ActionStatus.ABORTED:
	case ActionStatus.ABORTING:
	default:
	    return Failure.instance();
	}
    }
    
    private Participant        _resourceHandle;
    private int                _priority;
    private Qualifier[]        _quals;
    private CoordinatorIdImple _id;
    
}
