/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: XAResourceImple.java,v 1.1 2003/01/07 10:33:53 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.as.coordinator.jta;

import javax.transaction.xa.*;

import com.arjuna.mw.wscf.model.as.coordinator.xa.messages.*;
import com.arjuna.mw.wscf.model.as.coordinator.xa.outcomes.*;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wscf.model.as.coordinator.Participant;

import com.arjuna.mw.wsas.exceptions.SystemException;

import java.io.IOException;
import java.io.Serializable;

/**
 * JTA resource that wraps the activity service coordinator Participant.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: XAResourceImple.java,v 1.1 2003/01/07 10:33:53 nmcl Exp $
 */

public class XAResourceImple implements XAResource, Serializable
{
    
    /**
     * Constructor.
     *
     * @param theResource is the proxy that allows us to call out to the
     * object.
     */

    public XAResourceImple (Participant theResource, Qualifier[] quals)
    {
	_resourceHandle = theResource;
	_quals = quals;
	
	if (_resourceHandle == null)
	    System.err.println("XAResourceImple "+_resourceHandle+"- null participant provided!");
    }


    public int getTransactionTimeout() throws XAException
    {
	// TODO

	//        return _timeout;

	return 0;
    }

    public boolean isSameRM (XAResource xares) throws XAException
    {
        return (xares.equals(this));
    }

    public boolean setTransactionTimeout(int seconds) throws XAException
    {
        return true;
    }

    public void start (Xid xid, int flags) throws XAException
    {
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new XAStart(xid, flags));
		
		if (res != null)
		{
		    if (res instanceof XAOutcome)
		    {
			generateException((XAOutcome) res);
		    }
		    else
			throw new XAException(XAException.XAER_INVAL);
		}
            }
            else
		throw new XAException(XAException.XAER_INVAL);
        }
        catch (SystemException ex1)
        {
            System.err.println("XAResourceImple.start "+xid+" caught: "+ex1);

	    throw new XAException(XAException.XAER_INVAL);            
        }
        catch (Exception ex6)
        {
            System.err.println("XAResourceImple.start "+xid+" caught: ");

	    ex6.printStackTrace();

	    throw new XAException(XAException.XAER_INVAL);
        }
    }

    public void end (Xid xid, int flags) throws XAException
    {
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new XAEnd(xid, flags));
		
		if (res != null)
		{
		    if (res instanceof XAOutcome)
		    {
			generateException((XAOutcome) res);
		    }
		    else
			throw new XAException(XAException.XAER_INVAL);
		}
            }
            else
		throw new XAException(XAException.XAER_INVAL);
        }
        catch (SystemException ex1)
        {
            System.err.println("XAResourceImple.end "+xid+" caught: "+ex1);

	    throw new XAException(XAException.XAER_INVAL);            
        }
        catch (Exception ex6)
        {
            System.err.println("XAResourceImple.end "+xid+" caught: ");

	    ex6.printStackTrace();

	    throw new XAException(XAException.XAER_INVAL);
        }
    }

    /**
     * The record is being driven through top-level rollback.
     */

    public void rollback (Xid xid) throws XAException
    {
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new XARollback(xid));
		
		if (res != null)
		{
		    if (res instanceof XAOutcome)
		    {
			generateException((XAOutcome) res);
		    }
		    else
			throw new XAException(XAException.XAER_RMERR);
		}
            }
            else
		throw new XAException(XAException.XAER_RMERR);
        }
        catch (SystemException ex1)
        {
            System.err.println("XAResourceImple.rollback "+xid+" caught: "+ex1);

	    throw new XAException(XAException.XA_HEURHAZ);            
        }
        catch (Exception ex6)
        {
            System.err.println("XAResourceImple.rollback "+xid+" caught: ");

	    ex6.printStackTrace();

	    throw new XAException(XAException.XA_HEURHAZ);
        }
    }

    /**
     * The record is being driven through top-level commit.
     */

    public void commit(Xid id, boolean onePhase) throws XAException
    {
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new XACommit(id, onePhase));
		
		if (res != null)
		{
		    if (res instanceof XAOutcome)
		    {
			generateException((XAOutcome) res);
		    }
		    else
			throw new XAException(XAException.XA_HEURHAZ);
		}
            }
            else
		throw new XAException(XAException.XA_HEURHAZ);
        }
        catch (SystemException ex1)
        {
            System.err.println("XAResourceImple.commit "+id+" - " + ex1);

	    throw new XAException(XAException.XA_HEURHAZ);
        }
        catch (Exception ex6)
        {
            System.err.println("XAResourceImple.commit "+id+" - ");

	    throw new XAException(XAException.XA_HEURHAZ);
        }
    }

    /**
     * The record is being driven through top-level prepare.
     */

    public int prepare (Xid xid) throws XAException
    {
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new XAPrepare(xid));
		
		if (res != null)
		{
		    if (res instanceof XAPrepareOutcome)
			return ((XAPrepareOutcome) res).vote();
		    else
		    {
			if (res instanceof XAOutcome)
			    generateException((XAOutcome) res);
			else
			    throw new XAException(XAException.XA_HEURHAZ);
		    }
		}
		else
		    throw new XAException(XAException.XA_HEURHAZ);
            }
            else
		throw new XAException(XAException.XAER_PROTO);
        }
        catch (SystemException e1)
        {
            /*
            * Assume that this exception is thrown to indicate a
            * communication failure or some other system-like
            * exception. In which case, crash recovery should try to
            * recover for us.
            */

            System.err.println("XAResourceImple.prepare "+xid+" - " + e1);

	    throw new XAException(XAException.XA_HEURHAZ);
        }
        catch (Exception e6)
        {
            System.err.println("XAResourceImple.prepare "+xid+" - ");

	    e6.printStackTrace();

        }

	throw new XAException(XAException.XA_HEURHAZ);	    
    }

    /**
     * The record generated a heuristic and can now forget about it.
     */

    public void forget (Xid xid) throws XAException
    {
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new XAForget(xid));
		
		if (res != null)
		{
		    if (res instanceof XAOutcome)
		    {
			generateException((XAOutcome) res);
		    }
		    else
			throw new XAException(XAException.XAER_RMERR);
		}
            }
            else
		throw new XAException(XAException.XAER_RMFAIL);
        }
        catch (SystemException ex1)
        {
            System.err.println("XAResourceImple.forget "+xid+" - " + ex1);

	    throw new XAException(XAException.XAER_RMERR);
        }
        catch (Exception ex6)
        {
            System.err.println("XAResourceImple.forget "+xid+" - ");

	    throw new XAException(XAException.XAER_RMERR);
        }
    }

    public Xid[] recover (int flag) throws XAException
    {
        try
        {
            if (_resourceHandle != null)
            {
		Outcome res = _resourceHandle.processMessage(new XARecover(flag));
		
		if (res != null)
		{
		    if (res instanceof XARecoverOutcome)
			return ((XARecoverOutcome) res).xids();
		    
		    if (res instanceof XAOutcome)
		    {
			generateException((XAOutcome) res);
		    }
		    else
			throw new XAException(XAException.XAER_RMERR);
		}
		else
		    return null;
            }
            else
		throw new XAException(XAException.XAER_RMFAIL);
        }
        catch (SystemException ex1)
        {
            System.err.println("XAResourceImple.forget "+ ex1);

	    throw new XAException(XAException.XAER_RMERR);
        }
        catch (Exception ex6)
        {
            System.err.println("XAResourceImple.forget "+ ex6);
        }

	throw new XAException(XAException.XAER_RMERR);
    }

    private final void generateException (XAOutcome res) throws XAException
    {
	try
	{
	    Object data = res.data();
	
	    if (data instanceof XAException)
		throw (XAException) data;
	}
	catch (SystemException ex)
	{
	    throw new XAException(XAException.XAER_RMERR);
	}
    }
    
    private Participant _resourceHandle;
    private Qualifier[] _quals;
    
}

