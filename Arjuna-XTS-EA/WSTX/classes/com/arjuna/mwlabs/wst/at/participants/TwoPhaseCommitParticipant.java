/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TwoPhaseCommitParticipant.java,v 1.6 2003/02/24 13:21:46 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.participants;

import com.arjuna.wst.TwoPCParticipant;

import com.arjuna.mw.wscf.model.twophase.common.*;
import com.arjuna.mw.wscf.model.twophase.outcomes.*;
import com.arjuna.mw.wscf.model.twophase.participants.*;
import com.arjuna.mw.wscf.model.twophase.exceptions.*;
import com.arjuna.mw.wscf.model.twophase.vote.*;

import com.arjuna.mw.wscf.exceptions.*;

import com.arjuna.mw.wsas.exceptions.SystemException;
import com.arjuna.mw.wsas.exceptions.WrongStateException;
import com.arjuna.mw.wsas.exceptions.ProtocolViolationException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TwoPhaseCommitParticipant.java,v 1.6 2003/02/24 13:21:46 nmcl Exp $
 * @since 1.0.
 */

public class TwoPhaseCommitParticipant implements Participant
{

    public TwoPhaseCommitParticipant (TwoPCParticipant resource)
    {
	_resource = resource;
    }
    
    public Vote prepare () throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, SystemException
    {
	try
	{
	    if (_resource != null)
	    {
		//		com.arjuna.mw.wst.vote.Vote vt = _resource.prepare();
		com.arjuna.wst.Vote vt = _resource.prepare();
	    
		if (vt instanceof com.arjuna.wst.ReadOnly)
		    return new VoteReadOnly();
		else
		{
		    if (vt instanceof com.arjuna.wst.Prepared)
			return new VoteConfirm();
		    else
			return new VoteCancel();
		}
	    }
	    else
		return new VoteCancel();
	}
	//	catch (com.arjuna.mw.wst.exceptions.WrongStateException ex)
	catch (com.arjuna.wst.WrongStateException ex)
	{
	    throw new WrongStateException(ex.toString());
	}
	/*
	catch (com.arjuna.mw.wst.exceptions.HeuristicHazardException ex
	{
	    throw new HeuristicHazardException(ex.toString());
	}
	catch (com.arjuna.mw.wst.exceptions.HeuristicMixedException ex)
	{
	    throw new HeuristicMixedException(ex.toString());
	}
	*/
	//	catch (com.arjuna.mw.wst.exceptions.SystemException ex)
	catch (com.arjuna.wst.SystemException ex)
	{
	    throw new SystemException(ex.toString());
	}
    }

    public void confirm () throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicCancelException, SystemException
    {
	if (_resource != null)
	{
	    try
	    {
		_resource.commit();
	    }
	    //	    catch (com.arjuna.mw.wst.exceptions.WrongStateException ex)
	    catch (com.arjuna.wst.WrongStateException ex)
	    {
		throw new WrongStateException(ex.toString());
	    }
	    /*
	    catch (com.arjuna.mw.wst.exceptions.HeuristicHazardException ex)
	    {
		throw new HeuristicHazardException(ex.toString());
	    }
	    catch (com.arjuna.mw.wst.exceptions.HeuristicMixedException ex)
	    {
		throw new HeuristicMixedException(ex.toString());
	    }
	    catch (com.arjuna.mw.wst.exceptions.HeuristicRollbackException ex)
	    {
		throw new HeuristicCancelException(ex.toString());
	    }
	    */
	    //	    catch (com.arjuna.mw.wst.exceptions.SystemException ex)
	    catch (com.arjuna.wst.SystemException ex)
	    {
		throw new SystemException(ex.toString());
	    }
	}
	else
	    throw new InvalidParticipantException();
    }
    
    public void cancel () throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicConfirmException, SystemException
    {
	if (_resource != null)
	{
	    try
	    {
		_resource.rollback();
	    }
	    //	    catch (com.arjuna.mw.wst.exceptions.WrongStateException ex)
	    catch (com.arjuna.wst.WrongStateException ex)
	    {
		throw new WrongStateException(ex.toString());
	    }
	    /*
	    catch (com.arjuna.mw.wst.exceptions.HeuristicHazardException ex)
	    {
		throw new HeuristicHazardException(ex.toString());
	    }
	    catch (com.arjuna.mw.wst.exceptions.HeuristicMixedException ex)
	    {
		throw new HeuristicMixedException(ex.toString());
	    }
	    catch (com.arjuna.mw.wst.exceptions.HeuristicCommitException ex)
	    {
		throw new HeuristicConfirmException(ex.toString());
	    }
	    */
	    //	    catch (com.arjuna.mw.wst.exceptions.SystemException ex)
	    catch (com.arjuna.wst.SystemException ex)
	    {
		throw new SystemException(ex.toString());
	    }
	}
	else
	    throw new InvalidParticipantException();
    }
    
    public void confirmOnePhase () throws InvalidParticipantException, WrongStateException, HeuristicHazardException, HeuristicMixedException, HeuristicCancelException, SystemException
    {
	if (_resource != null)
	{
	    try
	    {
		_resource.commitOnePhase();
	    }
	    //	    catch (com.arjuna.mw.wst.exceptions.WrongStateException ex)
	    catch (com.arjuna.wst.WrongStateException ex)
	    {
		throw new WrongStateException(ex.toString());
	    }
	    catch (com.arjuna.wst.TransactionRolledBackException ex)
	    {
		throw new HeuristicCancelException();
	    }
	    /*
	    catch (com.arjuna.mw.wst.exceptions.HeuristicHazardException ex)
	    {
		throw new HeuristicHazardException(ex.toString());
	    }
	    catch (com.arjuna.mw.wst.exceptions.HeuristicMixedException ex)
	    {
		throw new HeuristicMixedException(ex.toString());
	    }
	    catch (com.arjuna.mw.wst.exceptions.HeuristicRollbackException ex)
	    {
		throw new HeuristicCancelException(ex.toString());
	    }
	    */
	    //	    catch (com.arjuna.mw.wst.exceptions.SystemException ex)
	    catch (com.arjuna.wst.SystemException ex)
	    {
		throw new SystemException(ex.toString());
	    }
	}
	else
	    throw new InvalidParticipantException();
    }
    
    public void forget () throws InvalidParticipantException, WrongStateException, SystemException
    {
    }

    public void unknown () throws SystemException
    {
	/*
	 * If the transaction is unknown, then we assume it rolled back.
	 */

	try
	{
	    cancel();
	}
	catch (Exception ex)
	{
	    // TODO
	}
    }
    
    private TwoPCParticipant _resource;
    
}
