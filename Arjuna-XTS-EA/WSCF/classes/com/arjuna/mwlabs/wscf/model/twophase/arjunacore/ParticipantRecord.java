/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ParticipantRecord.java,v 1.2 2003/02/07 15:31:48 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.twophase.arjunacore;

import com.hp.mw.ts.arjuna.ObjectType;
import com.hp.mw.ts.arjuna.coordinator.*;
import com.hp.mw.ts.arjuna.common.*;
import com.hp.mw.ts.arjuna.state.*;
import com.hp.mw.ts.arjuna.gandiva.ClassName;

import com.arjuna.mw.wscf.model.twophase.common.TwoPhaseResult;
import com.arjuna.mw.wscf.model.twophase.vote.*;
import com.arjuna.mw.wscf.model.twophase.participants.Participant;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;

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
 * @version $Id: ParticipantRecord.java,v 1.2 2003/02/07 15:31:48 nmcl Exp $
 */

public class ParticipantRecord extends com.hp.mw.ts.arjuna.coordinator.AbstractRecord
{
    
    /**
     * Constructor.
     *
     * @param theResource is the proxy that allows us to call out to the
     * object.
     */

    public ParticipantRecord (Participant theResource, Uid id)
    {
        super(id, null, ObjectType.ANDPERSISTENT);

	_resourceHandle = theResource;
	_timeout = 0;
	_coordId = new CoordinatorIdImple(id);
	
	if (theResource == null)
	    System.err.println("ParticipantRecord "+order()+"- null participant provided!");
    }

    public void finalize () throws Throwable
    {
        _resourceHandle = null;

        super.finalize();
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
	return RecordType.USER_DEF_FIRST0;
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

    // TODO

    public int nestedAbort ()
    {
        try
        {
            if (_resourceHandle != null)
            {
		return TwoPhaseOutcome.FINISH_ERROR;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
	/*
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.nestedAbort "+order()+" caught: "+ex1);
            
            return TwoPhaseOutcome.FINISH_ERROR;
        }
	*/
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
		return TwoPhaseOutcome.FINISH_ERROR;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
	/*
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.nestedCommit "+order()+" - " + ex1);
            return TwoPhaseOutcome.FINISH_ERROR;
        }
	*/
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
		return TwoPhaseOutcome.FINISH_ERROR;
            }
            else
                return TwoPhaseOutcome.PREPARE_NOTOK;
        }
	/*
        catch (SystemException e1)
        {
            System.err.println("ParticipantRecord.nestedPrepare "+order()+" - " + e1);
            return TwoPhaseOutcome.PREPARE_NOTOK;
        }
	*/
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
		try
		{
		    _resourceHandle.cancel();
		}
		catch (InvalidParticipantException ex)
		{
		    return TwoPhaseOutcome.FINISH_ERROR;
		}
		catch (WrongStateException ex)
		{
		    return TwoPhaseOutcome.FINISH_ERROR;
		}
		catch (HeuristicHazardException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_HAZARD;
		}
		catch (HeuristicMixedException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_MIXED;
		}
		catch (HeuristicConfirmException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_COMMIT;
		}
		catch (SystemException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_HAZARD;
		}
		
		return TwoPhaseOutcome.FINISH_OK;
            }
            else
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
		try
		{
		    _resourceHandle.confirm();
		}
		catch (InvalidParticipantException ex)
		{
		    return TwoPhaseOutcome.FINISH_ERROR;
		}
		catch (WrongStateException ex)
		{
		    return TwoPhaseOutcome.NOT_PREPARED;
		}
		catch (HeuristicHazardException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_HAZARD;
		}
		catch (HeuristicMixedException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_MIXED;
		}
		catch (HeuristicCancelException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_ROLLBACK;
		}
		catch (SystemException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_HAZARD;
		}

		return TwoPhaseOutcome.FINISH_OK;
            }
            else
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
		try
		{
		    Vote res = _resourceHandle.prepare();

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
		catch (InvalidParticipantException ex)
		{
		    return TwoPhaseOutcome.FINISH_ERROR;
		}
		catch (WrongStateException ex)
		{
		    return TwoPhaseOutcome.FINISH_ERROR;
		}
		catch (HeuristicHazardException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_HAZARD;
		}
		catch (HeuristicMixedException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_MIXED;
		}
		catch (SystemException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_HAZARD;
		}
            }
            else
                return TwoPhaseOutcome.PREPARE_NOTOK;
        }
        catch (Exception e6)
        {
            System.err.println("ParticipantRecord.topLevelPrepare "+order()+" - ");

	    e6.printStackTrace();
	    
            return TwoPhaseOutcome.PREPARE_OK;
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
		return TwoPhaseOutcome.FINISH_ERROR;
            }
            else
                return TwoPhaseOutcome.FINISH_ERROR;
        }
	/*
        catch (SystemException ex1)
        {
            System.err.println("ParticipantRecord.nestedOnePhaseCommit "+order()+" - " + ex1);
            return TwoPhaseOutcome.FINISH_ERROR;
        }
	*/
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
		try
		{
		    _resourceHandle.confirmOnePhase();
		}
		catch (InvalidParticipantException ex)
		{
		    return TwoPhaseOutcome.FINISH_ERROR;
		}
		catch (WrongStateException ex)
		{
		    return TwoPhaseOutcome.FINISH_ERROR;
		}
		catch (HeuristicHazardException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_HAZARD;
		}
		catch (HeuristicMixedException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_MIXED;
		}
		catch (HeuristicCancelException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_ROLLBACK;
		}
		catch (SystemException ex)
		{
		    return TwoPhaseOutcome.HEURISTIC_HAZARD;
		}

		return TwoPhaseOutcome.FINISH_OK;
            }
            else
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
		try
		{
		    _resourceHandle.forget();
		}
		catch (InvalidParticipantException ex)
		{
		    return false;
		}
		catch (WrongStateException ex)
		{
		    return false;
		}
		catch (SystemException ex)
		{
		    return false;
		}

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
		//TODO

		//                result = _resourceHandle.unpackState(os);

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
		// TODO
		//                result = _resourceHandle.packState(os);

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
	_coordId = null;
    }

    private Participant        _resourceHandle;
    private long               _timeout;
    private CoordinatorIdImple _coordId;

}
