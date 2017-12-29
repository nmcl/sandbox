/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DemoXAParticipant.java,v 1.2 2003/01/07 10:33:57 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.common;

import com.hp.mw.ts.arjuna.common.*;
import com.hp.mw.ts.arjuna.state.*;

import com.arjuna.mw.wsas.completionstatus.Failure;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wscf.model.as.coordinator.Participant;
import com.arjuna.mw.wscf.model.as.coordinator.Message;

import com.arjuna.mw.wscf.model.as.coordinator.xa.outcomes.*;
import com.arjuna.mw.wscf.model.as.coordinator.xa.messages.*;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.ProtocolViolationException;

import javax.transaction.xa.*;

import java.util.Hashtable;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DemoXAParticipant.java,v 1.2 2003/01/07 10:33:57 nmcl Exp $
 * @since 1.0.
 */

public class DemoXAParticipant implements Participant
{

    public DemoXAParticipant ()
    {
	this(true);
    }

    public DemoXAParticipant (boolean readonly)
    {
	_readonly = readonly;
	_id = new Uid();
    }
    
    public Outcome processMessage (Message notification) throws WrongStateException, ProtocolViolationException, SystemException
    {
	System.out.println("DemoXAParticipant.processMessage ( "+notification+" ) : "+_id);
	
	if (notification instanceof XAPrepare)
	{
	    if (_readonly)
	    {
		System.out.println("\nReturning read-only to prepare");
	    
		return new XAPrepareOutcome(XAResource.XA_RDONLY);
	    }
	    else
	    {
		System.out.println("\nReturning commit to prepare");
	    
		return new XAPrepareOutcome(XAResource.XA_OK);
	    }
	}
	else
	{
	    if (notification instanceof XACommit)
	    {
		if (((XACommit) notification).onePhaseCommit())
		{
		    System.out.println("\nOne-phase commit ok");
			
		    return new XAOutcome();
		}
		else
		{
		    System.out.println("\nCommit ok");
			
		    return new XAOutcome();
		}
	    }
	    else
	    {
		if (notification instanceof XAStart)
		{
		    System.out.println("\nStarting ok");
		    
		    return new XAOutcome();
		}
		else
		{
		    if (notification instanceof XAEnd)
		    {
			System.out.println("\nEnding ok");
		    
			return new XAOutcome();
		    }
		    else
		    {
			if (notification instanceof XARollback)
			{
			    System.out.println("\nRolling back ok");
			
			    return new XAOutcome(Failure.instance());
			}
			else
			{
			    System.out.println("\nReturning finish-error for "+notification);
		
			    return new XAOutcome(new XAException(XAException.XAER_INVAL));
			}
		    }
		}
	    }
	}
    }
    
    public String identity () throws SystemException
    {
	return "DemoXAParticipant: "+_id;
    }

    /**
     * These methods are required so that the coordinator can serialise and
     * de-serialise information about the inferior during completion and
     * recovery.
     */

    public boolean packState (OutputObjectState os)
    {
	return true;
    }

    public boolean unpackState (InputObjectState os)
    {
	return true;
    }

    private boolean _readonly;
    private Uid     _id;

}
