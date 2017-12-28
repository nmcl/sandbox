/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ParticipantRecord.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore;

import com.hp.mw.ts.arjuna.ObjectType;
import com.hp.mw.ts.arjuna.coordinator.*;
import com.hp.mw.ts.arjuna.common.*;
import com.hp.mw.ts.arjuna.state.*;
import com.hp.mw.ts.arjuna.gandiva.ClassName;

import com.arjuna.mw.wscf.model.as.coordinator.*;

import com.arjuna.mw.wscf.model.as.coordinator.twophase.outcomes.*;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.messages.*;
import com.arjuna.mw.wscf.model.as.coordinator.Participant;
import com.arjuna.mw.wscf.model.as.coordinator.twophase.*;

import com.arjuna.mw.wscf.model.twophase.common.TwoPhaseResult;
import com.arjuna.mw.wscf.model.twophase.outcomes.*;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wscf.common.Qualifier;

import com.arjuna.mw.wsas.exceptions.*;

import com.arjuna.mw.wscf.exceptions.*;

import java.io.PrintWriter;

import java.io.IOException;

/**
 * Arjuna abstract record to handle two-phase participants.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ParticipantRecord.java,v 1.1 2003/01/07 10:33:52 nmcl Exp $
 */

public class ParticipantRecord extends com.hp.mw.ts.arjuna.coordinator.AbstractRecord
{
    
    /**
     * Constructor.
     *
     * @param theResource is the proxy that allows us to call out to the
     * object.
     */

    public ParticipantRecord (Participant theResource, Uid id, int priority, Qualifier[] quals)
    {
        super(id, null, ObjectType.ANDPERSISTENT);

	_resourceHandle = new ParticipantWrapper(theResource);
	_timeout = 0;
	_priority = priority;
	_quals = quals;
	_coordId = new CoordinatorIdImple(id);
	_state = ActionStatus.RUNNING;
	
	if (theResource == null)
	    System.err.println("ParticipantRecord "+order()+"- null participant provided!");
    }

    public void finalize () throws Throwable
    {
        _resourceHandle = null;

        super.finalize();
    }

    public final void setResponse (Message notification, Outcome response) throws WrongStateException
    {
	boolean ok = false;
	
	if ((notification instanceof TopLevelPrepare) || (notification instanceof NestedPrepare))
	{
	    if (((_state == ActionStatus.PREPARING) && (response instanceof Vote)) || (_state == ActionStatus.RUNNING))
	    {
		ok = true;
	    }
	}

	if ((notification instanceof TopLevelConfirm) || (notification instanceof NestedConfirm))
	{
	    if (_state == ActionStatus.COMMITTING)
		ok = true;
	}
	
	if ((notification instanceof TopLevelCancel) || (notification instanceof NestedCancel))
	{
	    if ((_state == ActionStatus.ABORTING) || (_state == ActionStatus.RUNNING))
	    {
		ok = true;
	    }
	}

	if (ok)
	    _resourceHandle.setResponse(notification, response);
	else
	    throw new WrongStateException();
    }
    
    /**
     * Override AbstractRecord.propagateOnCommit
     */

    public boolean propagateOnCommit ()
    {
        return true;
    }

    /**
     * The type of this abstract record.
     */

    public int typeIs ()
    {
	return _priority;
    }

    /**
     * The class name for this record.
     */

    public ClassName className ()
    {
	return new ClassName(toString());
    }

    /**
     * The internal value.
     */

    public Object value ()
    {
        return _resourceHandle;
    }

    /**
     * Set the internal value. Not allowed for this class.
     */

    public void setValue (Object o)
    {
        System.err.println("ParticipantRecord.setValue() called illegally.");
    }

    /**
     * The record is being driven through nested rollback.
     */

    public int nestedAbort ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		_state = ActionStatus.ABORTING;

		Outcome res = _resourceHandle.processMessage(new NestedCancel(_coordId));

		_state = ActionStatus.ABORTED;

		if (res != null)
		{
		    if (res instanceof CoordinationOutcome)
		    {
			return convertResult(((CoordinationOutcome) res).result());
		    }
		    else
			return TwoPhaseOutcome.FINISH_ERROR;
		}

		return TwoPhaseOutcome.FINISH_OK;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.nestedAbort "+order()+" caught: "+ex1);
            
            return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (Exception ex6)
        {
            System.err.println("ParticipantRecord.nestedAbort "+order()+" caught: ");

	    ex6.printStackTrace();
            
            return TwoPhaseOutcome.FINISH_ERROR;
        }
    }

    /**
     * The record is being driven through nested commit.
     */

    public int nestedCommit ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		_state = ActionStatus.COMMITTING;

		Outcome res = _resourceHandle.processMessage(new NestedConfirm(_coordId));
		
		if (res != null)
		{
		    if (res instanceof CoordinationOutcome)
		    {
			return convertResult(((CoordinationOutcome) res).result());
		    }
		    else
			return TwoPhaseOutcome.FINISH_ERROR;
		}

		return TwoPhaseOutcome.FINISH_OK;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.nestedCommit "+order()+" - " + ex1);
            return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (Exception ex6)
        {
            System.err.println("ParticipantRecord.nestedCommit "+order()+" - ");

	    ex6.printStackTrace();
	    
            return TwoPhaseOutcome.FINISH_ERROR;
        }
    }

    /**
     * The record is being driven through nested prepare.
     */

    public int nestedPrepare ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		_state = ActionStatus.PREPARING;

		Outcome res = _resourceHandle.processMessage(new NestedPrepare(_coordId));
		
		if (res != null)
		{
		    if (res instanceof VoteConfirm)
		    {
			return TwoPhaseOutcome.PREPARE_OK;
		    }
		    else
		    {
			if (res instanceof VoteReadOnly)
			{
			    return TwoPhaseOutcome.PREPARE_READONLY;
			}
			else
			{
			    return TwoPhaseOutcome.PREPARE_NOTOK;
			}
		    }
		}
		else
		    return TwoPhaseOutcome.FINISH_ERROR;
            }
            else
                return TwoPhaseOutcome.PREPARE_NOTOK;
        }
        catch (SystemException e1)
        {
            /*
            * Assume that this exception is thrown to indicate a
            * communication failure or some other system-like
            * exception. In which case, crash recovery should try to
            * recover for us.
            */

            System.err.println("ParticipantRecord.nestedPrepare "+order()+" - " + e1);
            return TwoPhaseOutcome.PREPARE_NOTOK;
        }
        catch (Exception e6)
        {
            System.err.println("ParticipantRecord.nestedPrepare "+order()+" - ");

	    e6.printStackTrace();
	    
            return TwoPhaseOutcome.HEURISTIC_HAZARD;
        }
    }

    /**
     * The record is being driven through top-level rollback.
     */

    public int topLevelAbort ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		_state = ActionStatus.ABORTING;

		Outcome res = _resourceHandle.processMessage(new TopLevelCancel(_coordId));
		
		if (res != null)
		{
		    if (res instanceof CoordinationOutcome)
		    {
			return convertResult(((CoordinationOutcome) res).result());
		    }
		    else
			return TwoPhaseOutcome.FINISH_ERROR;
		}

		return TwoPhaseOutcome.FINISH_OK;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.topLevelAbort "+order()+" caught: "+ex1);
            
            return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (Exception ex6)
        {
            System.err.println("ParticipantRecord.topLevelAbort "+order()+" caught: ");

	    ex6.printStackTrace();
            
            return TwoPhaseOutcome.FINISH_ERROR;
        }
    }

    /**
     * The record is being driven through top-level commit.
     */

    public int topLevelCommit ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		_state = ActionStatus.COMMITTING;

		Outcome res = _resourceHandle.processMessage(new TopLevelConfirm(_coordId));
		
		if (res != null)
		{
		    if (res instanceof CoordinationOutcome)
		    {
			return convertResult(((CoordinationOutcome) res).result());
		    }
		    else
			return TwoPhaseOutcome.FINISH_ERROR;
		}

		return TwoPhaseOutcome.FINISH_OK;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.topLevelCommit "+order()+" - " + ex1);
            return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (Exception ex6)
        {
            System.err.println("ParticipantRecord.topLevelCommit "+order()+" - ");

	    ex6.printStackTrace();
	    
            return TwoPhaseOutcome.FINISH_ERROR;
        }
    }

    /**
     * The record is being driven through top-level prepare.
     */

    public int topLevelPrepare ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		_state = ActionStatus.PREPARING;

		Outcome res = _resourceHandle.processMessage(new TopLevelPrepare(_coordId));
		
		if (res != null)
		{
		    if (res instanceof VoteConfirm)
		    {
			return TwoPhaseOutcome.PREPARE_OK;
		    }
		    else
		    {
			if (res instanceof VoteReadOnly)
			{
			    return TwoPhaseOutcome.PREPARE_READONLY;
			}
			else
			{
			    return TwoPhaseOutcome.PREPARE_NOTOK;
			}
		    }
		}
		else
		    return TwoPhaseOutcome.FINISH_ERROR;
            }
            else
                return TwoPhaseOutcome.PREPARE_NOTOK;
        }
        catch (SystemException e1)
        {
            /*
            * Assume that this exception is thrown to indicate a
            * communication failure or some other system-like
            * exception. In which case, crash recovery should try to
            * recover for us.
            */

            System.err.println("ParticipantRecord.topLevelPrepare "+order()+" - " + e1);
            return TwoPhaseOutcome.PREPARE_NOTOK;
        }
        catch (Exception e6)
        {
            System.err.println("ParticipantRecord.topLevelPrepare "+order()+" - ");

	    e6.printStackTrace();
	    
            return TwoPhaseOutcome.HEURISTIC_HAZARD;
        }
    }
    
    /**
     * The record is being driven through nested commit and is the only
     * resource.
     */

    public int nestedOnePhaseCommit ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		_state = ActionStatus.COMMITTING;

		Outcome res = _resourceHandle.processMessage(new NestedOnePhaseCommit(_coordId));
		
		if (res != null)
		{
		    if (res instanceof CoordinationOutcome)
		    {
			return convertResult(((CoordinationOutcome) res).result());
		    }
		    else
			return TwoPhaseOutcome.FINISH_ERROR;
		}

		return TwoPhaseOutcome.FINISH_OK;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.nestedOnePhaseCommit "+order()+" - " + ex1);
            return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (Exception ex6)
        {
            System.err.println("ParticipantRecord.nestedOnePhaseCommit "+order()+" - ");

	    ex6.printStackTrace();
	    
            return TwoPhaseOutcome.FINISH_ERROR;
        }
    }

    /**
     * The record is being driven through top-level commit and is the only
     * resource.
     */

    public int topLevelOnePhaseCommit ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		_state = ActionStatus.COMMITTING;

		Outcome res = _resourceHandle.processMessage(new TopLevelOnePhaseCommit(_coordId));
		
		if (res != null)
		{
		    if (res instanceof CoordinationOutcome)
		    {
			return convertResult(((CoordinationOutcome) res).result());
		    }
		    else
			return TwoPhaseOutcome.FINISH_ERROR;
		}

		return TwoPhaseOutcome.FINISH_OK;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.topLevelOnePhaseCommit "+order()+" - " + ex1);
            return TwoPhaseOutcome.FINISH_ERROR;
        }
        catch (Exception ex6)
        {
            System.err.println("ParticipantRecord.topLevelOnePhaseCommit "+order()+" - ");

	    ex6.printStackTrace();
	    
            return TwoPhaseOutcome.FINISH_ERROR;
        }
    }

    /**
     * The record generated a heuristic and can now forget about it.
     */

    public boolean forgetHeuristic ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		//		_state = ActionStatus.COMMITTING;

		Outcome res = _resourceHandle.processMessage(new ForgetHeuristic(_coordId));
		
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
                System.err.println("ParticipantRecord.forgetHeuristic for "+order()+" called without a resource!");
        }
        catch (Exception e)
        {
            System.err.println("ParticipantRecord.forgetHeuristic "+order()+" caught exception: ");

	    e.printStackTrace();
        }

        return false;
    }

    public static AbstractRecord create ()
    {
        return new ParticipantRecord();
    }

    public void remove (AbstractRecord toDelete)
    {
        toDelete = null;
    }
    
    public void print (PrintWriter strm)
    {
        super.print(strm);

        strm.print("ParticipantRecord");
        strm.print(_resourceHandle);
    }

    public boolean restore_state (InputObjectState os, int t)
    {
        boolean result = super.restore_state(os, t);

        if (result)
        {
            try
            {
                result = _resourceHandle.unpackState(os);

		_priority = os.unpackInt();
		_timeout = os.unpackLong();

		/*
		 * TODO: unpack qualifiers and coord id.
		 */
            }
            catch (Exception ex)
            {
                System.err.println("ParticipantRecord.restore_state: "+ex);
                
                result = false;
            }
        }
        
        return result;
    }

    public boolean save_state (OutputObjectState os, int t)
    {
        boolean result = super.save_state(os, t);

        if (result)
        {
            try
            {
                result = _resourceHandle.packState(os);

		os.packInt(_priority);
		os.packLong(_timeout);

		/*
		 * TODO: pack qualifiers and coord id.
		 */
            }
            catch (Exception ex)
            {
                System.err.println("ParticipantRecord.save_state: "+ex);
                
                result = false;
            }
        }
        
        return result;
    }

    public String type ()
    {
        return "/StateManager/AbstractRecord/WSCF/ArjunaCore/ParticipantRecord";
    }

    public boolean doSave ()
    {
        return true;
    }

    public void merge (AbstractRecord a)
    {
    }

    public void alter (AbstractRecord a)
    {
    }

    public boolean shouldAdd (AbstractRecord a)
    {
        return false;
    }
    
    public boolean shouldAlter (AbstractRecord a)
    {
        return false;
    }

    public boolean shouldMerge (AbstractRecord a)
    {
        return false;
    }

    public boolean shouldReplace (AbstractRecord rec)
    {
        return false;
    }

    /*
    * Protected constructor used by crash recovery.
    */

    protected ParticipantRecord ()
    {
        super();

        _resourceHandle = null;
        _timeout = 0;
	_priority = 0;
        _quals = null;
	_coordId = null;
	_state = ActionStatus.RUNNING;
    }

    private final int convertResult (int result)
    {
	switch (result)
	{
	case TwoPhaseResult.PREPARE_OK:
	    return TwoPhaseOutcome.PREPARE_OK;
	case TwoPhaseResult.PREPARE_NOTOK:
	    return TwoPhaseOutcome.PREPARE_NOTOK;
	case TwoPhaseResult.PREPARE_READONLY:
	    return TwoPhaseOutcome.PREPARE_READONLY;
	case TwoPhaseResult.HEURISTIC_CANCEL:
	    return TwoPhaseOutcome.HEURISTIC_ROLLBACK;
	case TwoPhaseResult.HEURISTIC_CONFIRM:
	    return TwoPhaseOutcome.HEURISTIC_COMMIT;
	case TwoPhaseResult.HEURISTIC_MIXED:
	    return TwoPhaseOutcome.HEURISTIC_MIXED;
	case TwoPhaseResult.HEURISTIC_HAZARD:
	    return TwoPhaseOutcome.HEURISTIC_HAZARD;
	case TwoPhaseResult.FINISH_OK:
	case TwoPhaseResult.CANCELLED:
	case TwoPhaseResult.CONFIRMED:
	    return TwoPhaseOutcome.FINISH_OK;
	case TwoPhaseResult.NOT_PREPARED:
	    return TwoPhaseOutcome.NOT_PREPARED;
	case TwoPhaseResult.FINISH_ERROR:
	default:
	    return TwoPhaseOutcome.FINISH_ERROR;
	}
    }
    
    private ParticipantWrapper _resourceHandle;
    private long               _timeout;
    private int                _priority;
    private Qualifier[]        _quals;
    private CoordinatorIdImple _coordId;
    private int                _state;

}
